#include <X11/Xlib.h>
typedef Window  XWindow;
typedef Display XDisplay;

#include <stdio.h>
#include <stdlib.h>

#include "../types.h"
#include "app.h"

// Global state
internal XDisplay* _display = NULL;
internal i32       _screen  = 0;

struct C4_Window
{
	XWindow handle;
	GC      graphic_ctx;
};

void app_init(void)
{
	_display = XOpenDisplay(NULL);
	_screen  = DefaultScreen(_display);
}

bool app_create_window(C4_WindowOptions options, C4_Window* w_* window)
{
	if (_display == NULL)
	{
		printf("Platform not initialized\n");
		return false;
	}

	// Should I use a different allocation strategy there?
	C4_Window* w = malloc(sizeof(C4_Window));
	if (w == NULL)
		return false;

	*window = w;

	u32 width  = options.size.width;
	u32 height = options.size.height;
	w->handle  = XCreateSimpleWindow(_display, RootWindow(_display, _screen), 0, 0, width, height, 0,
	                                 BlackPixel(_display, _screen), WhitePixel(_display, _screen));

	XStoreName(_display, w->handle, (char*)options.title.ptr);
	w->graphic_ctx = XCreateGC(_display, w->handle, 0, 0);

	// select literally everything
	long event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | EnterWindowMask |
	                  LeaveWindowMask | PointerMotionMask | PointerMotionHintMask | ButtonMotionMask | KeymapStateMask |
	                  ExposureMask | VisibilityChangeMask | StructureNotifyMask | ResizeRedirectMask |
	                  SubstructureNotifyMask | SubstructureRedirectMask | FocusChangeMask | PropertyChangeMask |
	                  ColormapChangeMask | OwnerGrabButtonMask;

	XSelectInput(_display, w->handle, event_mask);

	XMapWindow(_display, w->handle);
	return true;
}

void app_close_window(C4_Window* window)
{
	XUnmapWindow(_display, window->handle);
	XFreeGC(_display, window->graphic_ctx);
	free(window);
}
