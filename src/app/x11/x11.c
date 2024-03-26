#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define XWindow  Window
#define XDisplay Display
#define XNone    None
#define XTrue    True
#define XFalse   False

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../core/alloc/arena.h"
#include "../../core/data_structures/llist.h"
#include "../../core/math.h"
#include "../../core/os.h"
#include "../../types.h"
#include "../app.h"
#include "x11_keycode.h"

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

internal EventCircBuf _event_circbuf = {0};

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

internal bool event_push_back(C4_Event r_* event)
{
	if (_event_circbuf.count == _event_circbuf.capacity)
		return false;

	_event_circbuf.ptr[_event_circbuf.tail] = *event;

	_event_circbuf.tail = (_event_circbuf.tail + 1) % _event_circbuf.capacity;
	_event_circbuf.count++;

	return true;
}

internal bool event_pop_front(C4_Event w_* event)
{
	if (_event_circbuf.count == 0)
		return false;

	*event = _event_circbuf.ptr[_event_circbuf.head];

	_event_circbuf.head = (_event_circbuf.head + 1) % _event_circbuf.capacity;
	_event_circbuf.count--;

	return true;
}

//////////////////////
//// Global state ////
//////////////////////

internal Arena _arena;

// X11
internal XDisplay rw_* _display = NULL;
internal i32           _screen  = 0;

// Head of the window free list
internal C4_Window* _first_free_window = NULL;

// doubly linked list of open windows
DEF_LLIST(C4_Window, C4_WindowLList);
internal C4_WindowLList _windows = {.head = NULL, .tail = NULL, .count = 0};

// Event handling
internal XIM        _xim      = NULL;
internal C4_KeyCode _prev_key = C4_KEY_UNKNOWN;

// String buffer (for IME events)
internal String8 _str_buffer = {.ptr = NULL, .len = 0};
internal usize   _str_cursor = 0;

/** Expand str buffer by `len` elements. This reallocates it. */
internal void init_or_expand_str_buffer(usize len)
{
	if (_str_buffer.ptr == NULL)
	{
		_str_buffer.ptr = calloc(len, sizeof(char));
	}
	else
	{
		_str_buffer.ptr = realloc(_str_buffer.ptr, _str_buffer.len + len);
		memset(_str_buffer.ptr + _str_buffer.len, 0, len);
	}

	_str_buffer.len += len;
}

internal void reset_str_buffer(void)
{
	_str_cursor = 0;
}

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

	C4_WindowPos  pos;
	C4_WindowSize size;

	Framebuffer framebuffer;
	XImage*     ximage;

	XWindow xhandle;
	GC      xgraphic_ctx;
	XIC     xic;
};

void app_init(void)
{
	// Prepare locale for IME to work properly
	setlocale(LC_CTYPE, "");
	XSetLocaleModifiers("");

	XInitThreads();
	XrmInitialize();

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

	// Initialize IME
	_xim = XOpenIM(_display, NULL, NULL, NULL);

	// TODO: check if _xim is null. What do I do for error handling?

	XFlush(_display);

	C4_EventSlice event_buffer = {
	    .ptr = arena_alloc_n(&_arena, C4_Event, 32),
	    .len = 32,
	};

	_event_circbuf = event_circbuf_init(event_buffer);

	init_or_expand_str_buffer(16);
}

internal void place_ime(XIC xic, i16 x, i16 y)
{
	XPoint        place        = {.x = x, .y = y};
	XVaNestedList preedit_attr = XVaCreateNestedList(0, XNSpotLocation, &place, NULL);
	XSetICValues(xic, XNPreeditAttributes, preedit_attr, NULL);
	XFree(preedit_attr);
}

internal C4_Window rw_* find_window_from_handle(XWindow handle)
{
	if (_windows.head == NULL)
		return NULL;

	for (C4_Window rw_* window = _windows.head; window->next == NULL; window = window->next)
	{
		if (window->xhandle == handle)
			return window;
	}

	return NULL;
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

	u32 width  = options.size.width;
	u32 height = options.size.height;

	C4_WindowPos pos = {.x = 0, .y = 0};
	window->pos      = pos;

	C4_WindowSize size = {.width = width, .height = height};
	window->size       = size;

	// allocate framebuffer
	Pixel*      framebuffer_ptr = malloc((usize)width * (usize)height * sizeof(Pixel));
	Framebuffer framebuffer     = {.ptr = framebuffer_ptr, .width = width, .height = height};
	window->framebuffer         = framebuffer;

	window->ximage = XCreateImage(_display, DefaultVisual(_display, 0), 24, ZPixmap, 0, (char*)window->framebuffer.ptr,
	                              width, height, 32, 0);

	// X11: create window
	XWindow wandle  = XCreateSimpleWindow(_display, RootWindow(_display, _screen), 0, 0, width, height, 0, 0, 0);
	window->xhandle = wandle;

	XStoreName(_display, window->xhandle, options.title);
	window->xgraphic_ctx = XCreateGC(_display, window->xhandle, 0, 0);

	// select literally everything
	long event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | EnterWindowMask |
	                  LeaveWindowMask | PointerMotionMask | PointerMotionHintMask | ButtonMotionMask | KeymapStateMask |
	                  ExposureMask | VisibilityChangeMask | StructureNotifyMask | ResizeRedirectMask |
	                  SubstructureNotifyMask | SubstructureRedirectMask | FocusChangeMask | PropertyChangeMask |
	                  ColormapChangeMask | OwnerGrabButtonMask;

	XSelectInput(_display, wandle, event_mask);
	XMapWindow(_display, wandle);

	// IME context
	window->xic = XCreateIC(_xim, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, XNClientWindow, wandle, NULL);

	// TODO: check if xic is null (error handling?)

	// select IME and position it
	XSetICFocus(window->xic);
	place_ime(window->xic, 0, 0);

	XFlush(_display);

	return true;
}

void app_close_window(C4_Window* window)
{
	// X11 deallocation
	XUnmapWindow(_display, window->xhandle);
	XFreeGC(_display, window->xgraphic_ctx);

	// framebuffer deallocation

	// XDestroyImage also destroys the data it points to for some reason, even though it doesn't allocate that data
	// itself. So we have to unreference the data to make sure we have control over the allocation of our framebuffer.
	window->ximage->data = NULL;
	XDestroyImage(window->ximage);
	free(window->framebuffer.ptr);

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

////////////////////////
//// Event handling ////
////////////////////////

internal XID utf8_lookup_string(C4_Window r_* window, XKeyEvent xpress, String8 w_* text)
{
	XID    keysym = 0;
	Status status = 0;

	{
		String8 txt = {.ptr = _str_buffer.ptr + _str_cursor, .len = _str_buffer.len - _str_cursor};
		*text       = txt;
	}

	xpress.type    = KeyPress;
	i32 char_count = Xutf8LookupString(window->xic, &xpress, text->ptr, (int)text->len, &keysym, &status);

	// reallocating lookup string buffer if it wasn't big enough
	if (status == XBufferOverflow)
	{
		init_or_expand_str_buffer(char_count + 1 - text->len);

		{
			String8 txt = {.ptr = _str_buffer.ptr + _str_cursor, .len = _str_buffer.len - _str_cursor};
			*text       = txt;
		}

		char_count = Xutf8LookupString(window->xic, &xpress, text->ptr, (int)text->len, &keysym, &status);
	}

	text->len             = char_count;
	text->ptr[char_count] = '\0';

	return keysym;
}

internal void resize_framebuffer(C4_Window rw_* window, u32 width, u32 height)
{
	window->ximage->data = NULL;
	XDestroyImage(window->ximage);

	window->framebuffer.ptr    = realloc(window->framebuffer.ptr, (usize)width * (usize)height * sizeof(Pixel));
	window->framebuffer.width  = width;
	window->framebuffer.height = height;

	window->ximage = XCreateImage(_display, DefaultVisual(_display, 0), 24, ZPixmap, 0, (char*)window->framebuffer.ptr,
	                              width, height, 32, 0);
}

internal u32  _event_counter = 0;
internal void process_xevent(XEvent* xevent)
{
	switch (xevent->type)
	{
		case KeyPress:
		case KeyRelease:
		{
			XKeyEvent key_event = xevent->xkey;

			C4_Window rw_* window = find_window_from_handle(key_event.window);

			String8 text   = {0};
			XID     keysym = utf8_lookup_string(window, key_event, &text);

			C4_KeyCode keycode = keysym_to_keycode(keysym);

			// determine key event tag (press/repeat/release)
			C4_KeyboardEventTag kb_evtag;
			if (xevent->type == KeyPress)
			{
				if (keycode == _prev_key && _prev_key != C4_KEY_UNKNOWN)
				{
					kb_evtag = C4_KEYBOARD_EVENT_KEY_REPEAT;
				}
				else
				{
					_prev_key = keycode;
					kb_evtag  = C4_KEYBOARD_EVENT_KEY_PRESS;
				}
			}
			else
			{
				_prev_key = C4_KEY_UNKNOWN;
				kb_evtag  = C4_KEYBOARD_EVENT_KEY_RELEASE;
			}

			// send key press/repeat/release event
			{
				C4_KeyboardEvent kb_event = {.tag = kb_evtag, .kind = {.key_press = keycode}};
				C4_Event         result = {.window = window, .tag = C4_EVENT_KEYBOARD, .kind = {.keyboard = kb_event}};
				event_push_back(&result);
			}

			// Send IME commit only on a non-repeated key press
			bool do_ime = kb_evtag == C4_KEYBOARD_EVENT_KEY_PRESS;

			if (do_ime && text.len > 0)
			{
				place_ime(window->xic, 0, 0);

				C4_KeyboardEvent kb_event = {.tag = C4_KEYBOARD_EVENT_IME_COMMIT, .kind = {.ime_commit = text}};
				C4_Event         result = {.window = window, .tag = C4_EVENT_KEYBOARD, .kind = {.keyboard = kb_event}};
				event_push_back(&result);
			}
			break;
		}

		case ConfigureNotify:
		{
			XConfigureEvent configure_event = xevent->xconfigure;

			C4_Window rw_* window = find_window_from_handle(configure_event.window);

			C4_WindowPos pos = {.x = configure_event.x, .y = configure_event.y};
			if (!WINDOW_POS_EQ(pos, window->pos))
			{
				window->pos = pos;

				C4_EventKindMoved moved_event = {.pos = pos};
				C4_Event          result = {.window = window, .tag = C4_EVENT_MOVED, .kind = {.moved = moved_event}};
				event_push_back(&result);
			}

			C4_WindowSize size = {.width = configure_event.width, .height = configure_event.height};
			if (!WINDOW_SIZE_EQ(size, window->size))
			{
				window->size = size;

				resize_framebuffer(window, size.width, size.height);

				C4_EventKindResized resized_event = {.size = size};
				C4_Event result = {.window = window, .tag = C4_EVENT_RESIZED, .kind = {.resized = resized_event}};
				event_push_back(&result);
			}

			break;
		}

			// TODO: manage other events

		default:
			printf("Other event %d\n", _event_counter++);
			break;
	}
}

bool app_poll_event(C4_Event w_* event)
{
	if (_windows.count == 0)
		return false;

	reset_str_buffer();

	while (event_pop_front(event) == false)
	{
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

	return true;
}

//////////////////
//// Graphics ////
//////////////////

Framebuffer app_get_next_framebuffer(C4_Window rw_* window)
{
	return window->framebuffer;
}

void app_commit_framebuffer(C4_Window rw_* window)
{
	u32 width  = window->framebuffer.width;
	u32 height = window->framebuffer.height;
	XPutImage(_display, window->xhandle, window->xgraphic_ctx, window->ximage, 0, 0, 0, 0, width, height);
    XFlush(_display);
}
