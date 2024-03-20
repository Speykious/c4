#include <X11/Xlib.h>
typedef Window  XWindow;
typedef Display XDisplay;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../core/alloc/arena.h"
#include "../types.h"
#include "app.h"

// Global state
internal XDisplay* _display = NULL;
internal i32       _screen  = 0;
internal Arena     _arena;

// Head of the free list
internal C4_Window* _first_free_window = NULL;

// Head and tail of the doubly linked list of open windows
internal C4_Window* _first_window = NULL;
internal C4_Window* _last_window  = NULL;

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
	_arena   = arena_init(64 * sizeof(C4_Window));
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
		if (_first_window == NULL)
			_first_window = window;

		if (_last_window != NULL)
		{
			_last_window->next = window;
			window->prev       = _last_window;
		}

		_last_window = window;
	}

	// X11: create window
	u32 width      = options.size.width;
	u32 height     = options.size.height;
	window->handle = XCreateSimpleWindow(_display, RootWindow(_display, _screen), 0, 0, width, height, 0,
	                                     BlackPixel(_display, _screen), WhitePixel(_display, _screen));

	XStoreName(_display, window->handle, (char*)options.title.ptr);
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
		if (window->prev)
			window->prev->next = window->next;

		if (_first_window == window)
			_first_window = window->next;

		if (_last_window == window)
			_last_window = window->prev;
	}

	// add it to the free list
	{
		window->next       = _first_free_window;
		_first_free_window = window;
	}
}
