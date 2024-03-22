#include <X11/XKBlib.h>
#include <X11/Xlib.h>

#define XWindow  Window
#define XDisplay Display
#define XNone    None
#define XTrue    True
#define XFalse   False

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../core/alloc/arena.h"
#include "../core/data_structures/llist.h"
#include "../core/math.h"
#include "../core/os.h"
#include "../types.h"
#include "app.h"

/////////////////////////////
//// Event circle buffer ////
/////////////////////////////

DEF_SLICE(C4_Event, C4_EventSlice);

typedef struct
{
	C4_Event* ptr;
	usize     head;
	usize     tail;
	usize     capacity;
	usize     count;
} EventCircBuf;

internal EventCircBuf event_circbuf_init(C4_EventSlice buffer)
{
	EventCircBuf circbuf = {
	    .ptr      = buffer.ptr,
	    .head     = 0,
	    .tail     = 0,
	    .capacity = buffer.len,
	    .count    = 0,
	};

	return circbuf;
}

internal bool event_push_back(EventCircBuf rw_* event_cb, C4_Event r_* event)
{
	if (event_cb->count == event_cb->capacity)
		return false;

	event_cb->ptr[event_cb->tail] = *event;

	event_cb->tail = (event_cb->tail + 1) % event_cb->capacity;
	event_cb->count++;

	return true;
}

internal bool event_pop_front(EventCircBuf rw_* event_cb, C4_Event w_* event)
{
	if (event_cb->count == 0)
		return false;

	*event = event_cb->ptr[event_cb->head];

	event_cb->head = (event_cb->head + 1) % event_cb->capacity;
	event_cb->count--;

	return true;
}

//////////////////////
//// Global state ////
//////////////////////

internal Arena _arena;

// X11
internal XDisplay* _display = NULL;
internal i32       _screen  = 0;

// Head of the window free list
internal C4_Window* _first_free_window = NULL;

// doubly linked list of open windows
DEF_LLIST(C4_Window, C4_WindowLList);
internal C4_WindowLList _windows = {.head = NULL, .tail = NULL, .count = 0};

// Event handling
internal EventCircBuf _event_circbuf = {0};
internal C4_KeyCode   _prev_key      = C4_KEY_UNKNOWN;

///////////////////
//// Windowing ////
///////////////////

struct C4_Window
{
	// intrusive linked list
	// - when the window is open, it points to the next and prev windows in use
	// - when the window is closed, it points to the next and prev free slots
	C4_Window* next;
	C4_Window* prev;

	XWindow handle;
	GC      graphic_ctx;
};

void app_init(void)
{
	_display = XOpenDisplay(NULL);
	_screen  = DefaultScreen(_display);
	_arena   = arena_init(4 * os_page_size());

	// https://linux.die.net/man/3/xkbsetdetectableautorepeat
	// TLDR: Xkb allows clients to request detectable auto-repeat.
	// If a client requests and the server supports DetectableAutoRepeat,
	// Xkb generates KeyRelease events only when the key is physically
	// released. If DetectableAutoRepeat is not supported or has not been
	// requested, the server synthesizes a KeyRelease event for each
	// repeating KeyPress event it generates.
	XkbSetDetectableAutoRepeat(_display, true, NULL);

	C4_EventSlice event_buffer = {
	    .ptr = arena_alloc_n(&_arena, C4_Event, 32),
	    .len = 32,
	};

	_event_circbuf = event_circbuf_init(event_buffer);
}

bool app_open_window(C4_WindowOptions options, C4_Window* w_* window_out)
{
	if (_display == NULL)
	{
		printf("Platform not initialized\n");
		return false;
	}

	// allocate window
	C4_Window* window;
	{
		if (_first_free_window != NULL)
		{
			// reuse available slot from free list
			window             = _first_free_window;
			_first_free_window = window->next;

			memset(window, 0, sizeof(C4_Window));
		}
		else
		{
			// allocate a new window in the arena
			window = arena_alloc(&_arena, C4_Window);
		}

		*window_out = window;
	}

	// push window to the open-window linked list
	{
		_windows.count++;

		if (_windows.head == NULL)
			_windows.head = window;

		if (_windows.tail != NULL)
		{
			_windows.tail->next = window;
			window->prev        = _windows.tail;
		}

		_windows.tail = window;
	}

	// X11: create window
	u32 width      = options.size.width;
	u32 height     = options.size.height;
	window->handle = XCreateSimpleWindow(_display, RootWindow(_display, _screen), 0, 0, width, height, 0,
	                                     BlackPixel(_display, _screen), BlackPixel(_display, _screen));

	XStoreName(_display, window->handle, options.title);
	window->graphic_ctx = XCreateGC(_display, window->handle, 0, 0);

	// select literally everything
	long event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | EnterWindowMask |
	                  LeaveWindowMask | PointerMotionMask | PointerMotionHintMask | ButtonMotionMask | KeymapStateMask |
	                  ExposureMask | VisibilityChangeMask | StructureNotifyMask | ResizeRedirectMask |
	                  SubstructureNotifyMask | SubstructureRedirectMask | FocusChangeMask | PropertyChangeMask |
	                  ColormapChangeMask | OwnerGrabButtonMask;

	XSelectInput(_display, window->handle, event_mask);

	XMapWindow(_display, window->handle);
	return true;
}

void app_close_window(C4_Window* window)
{
	XUnmapWindow(_display, window->handle);
	XFreeGC(_display, window->graphic_ctx);

	// pop window out of the open-window linked list
	{
		_windows.count--;

		if (window->prev)
			window->prev->next = window->next;

		if (_windows.head == window)
			_windows.head = window->next;

		if (_windows.tail == window)
			_windows.tail = window->prev;
	}

	// add it to the free list
	{
		window->next       = _first_free_window;
		_first_free_window = window;
	}
}

internal void process_xevent(XEvent* xevent)
{
	switch (xevent->type)
	{
		case KeyPress:
		case KeyRelease:
		{
			XKeyEvent key_event = xevent->xkey;

			// TODO: manage keysym instead of throwing keycode
			C4_KeyCode keycode = key_event.keycode;

			C4_KeyboardEventTag kb_event_tag;
			if (xevent->type == KeyPress)
			{
				if (keycode == _prev_key && _prev_key != C4_KEY_UNKNOWN)
				{
					kb_event_tag = C4_KEYBOARD_EVENT_KEY_REPEAT;
				}
				else
				{
					_prev_key    = keycode;
					kb_event_tag = C4_KEYBOARD_EVENT_KEY_PRESS;
				}
			}
			else
			{
				_prev_key    = C4_KEY_UNKNOWN;
				kb_event_tag = C4_KEYBOARD_EVENT_KEY_RELEASE;
			}

			C4_KeyboardEvent kb_event = {.tag = kb_event_tag, .kind = {.key_press = keycode}};
			C4_Event         result   = {.tag = KEYBOARD, .kind = {.keyboard = kb_event}};

			fprintf(stderr, "[X11] pushing key press/release event\n");
			event_push_back(&_event_circbuf, &result);
			break;
		}

			// TODO: manage other events

		default:
			break;
	}
}

bool app_poll_event(C4_Event w_* event)
{
	if (_windows.count == 0)
		return false;

	while (event_pop_front(&_event_circbuf, event) == false)
	{
		fprintf(stderr, "[X11] pending events\n");
		i32 count = XPending(_display);

		// get all pending events or wait for the next one
		for (i32 i = 0; i < MAX(count, 1); i++)
		{
			XEvent xevent = {0};
			XNextEvent(_display, &xevent);

			// Apparently, this forwards the event to the IME and returns whether the event was consumed.
			// I know, weird. The name of the function is even weirder.
			if (XFilterEvent(&xevent, XNone) == XTrue)
				continue;

			process_xevent(&xevent);
		}

		XFlush(_display);
	}

	fprintf(stderr, "[X11] returning\n");
	return true;
}
