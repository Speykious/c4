#pragma once

#include "../types.h"
#include "app_keyboard.h"

typedef struct C4_Window C4_Window;

typedef struct
{
	i32 x;
	i32 y;
} C4_WindowPos;

#define WINDOW_POS_EQ(a, b) (a.x == b.x && a.y == b.y)

typedef struct
{
	u32 width;
	u32 height;
} C4_WindowSize;

#define WINDOW_SIZE_EQ(a, b) (a.width == b.width && a.height == b.height)

typedef enum
{
	WINDOWED   = 0,
	BORDERLESS = 1,
	FULLSCREEN = 2,
} C4_ScreenMode;

typedef struct
{
	usize raw;
} C4_MonitorHandle;

typedef enum
{
	WINDOW_FLAG_CENTERED    = 1 << 0,
	WINDOW_FLAG_RESIZABLE   = 1 << 1,
	WINDOW_FLAG_MAXIMIZED   = 1 << 2,
	WINDOW_FLAG_TRANSPARENT = 1 << 3,
	WINDOW_FLAG_HIGH_DPI    = 1 << 4,
} C4_WindowFlags;

typedef struct
{
	char*            title;
	C4_WindowPos     position;
	C4_WindowSize    size;
	C4_MonitorHandle monitor;
	C4_ScreenMode    screen_mode;
	C4_WindowFlags   flags;
} C4_WindowOptions;

/** Initializes the global state necessary for window management. */
void app_init(void);

/** Opens a new window. Returns whether it succeeded in opening it. */
bool app_open_window(C4_WindowOptions options, C4_Window* w_* window);

/** Closes and frees a window. */
void app_close_window(C4_Window rw_* window);

////////////////////////
//// Event handling ////
////////////////////////

typedef enum
{
	C4_EVENT_RESIZED = 1,
	C4_EVENT_MOVED   = 2,

	C4_EVENT_CLOSE_REQUESTED = 3,
	C4_EVENT_DESTROYED       = 4,

	// C4_EVENT_FILE_DROPPED,
	// C4_EVENT_FILE_HOVERED,
	// C4_EVENT_FILE_HOVERED_CANCELLED,

	C4_EVENT_KEYBOARD = 5,
	C4_EVENT_MOUSE    = 6,
	C4_EVENT_TOUCH    = 7,

	C4_EVENT_REDRAW = 8,

	C4_EVENT_FOCUS_IN  = 9,
	C4_EVENT_FOCUS_OUT = 10,
} C4_EventTag;

typedef struct
{
	C4_WindowSize size;
} C4_EventKindResized;

typedef struct
{
	C4_WindowPos pos;
} C4_EventKindMoved;

typedef union
{
	C4_EventKindResized resized;
	C4_EventKindMoved   moved;
	C4_KeyboardEvent    keyboard;

	// EventKindFileDropped          file_dropped;
	// EventKindFileHovered          file_hovered;
	// EventKindFileHoveredCancelled file_hovered_cancelled;

} C4_EventKind;

typedef struct
{
	C4_Window*   window;
	C4_EventTag  tag;
	C4_EventKind kind;
} C4_Event;

bool app_poll_event(C4_Event w_* event);

//////////////////
//// Graphics ////
//////////////////

typedef u32 Pixel;

typedef struct
{
	Pixel* ptr;
	u32    width;
	u32    height;
} Framebuffer;

/** Get the framebuffer for the next frame. Its lifetime, obviously, is one of a single frame. */
Framebuffer app_get_next_framebuffer(C4_Window rw_* window);
void        app_commit_framebuffer(C4_Window rw_* window);
