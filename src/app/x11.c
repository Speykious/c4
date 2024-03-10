#include <X11/Xlib.h>

int main(void)
{
	Display* display = XOpenDisplay(NULL);
	int      screen  = DefaultScreen(display);
	Window   window  = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, 320, 240, 0,
	                                       BlackPixel(display, screen), WhitePixel(display, screen));

	XStoreName(display, window, "X11 Window");
	XSelectInput(display, window, ExposureMask | KeyPressMask);
	XMapWindow(display, window);
	for (;;)
	{
		XEvent e;
		XNextEvent(display, &e);
		// handle events here
	}
	return XCloseDisplay(display);
}
