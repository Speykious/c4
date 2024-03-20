#pragma once

#include "../types.h"
#include "keyboard.h"

typedef struct C4_Window C4_Window;

typedef struct
{
	i32 x;
	i32 y;
} C4_WindowPos;

typedef struct
{
	u32 width;
	u32 height;
} C4_WindowSize;

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
	String8          title;
	C4_WindowPos     position;
	C4_WindowSize    size;
	C4_MonitorHandle monitor;
	C4_ScreenMode    screen_mode;
	C4_WindowFlags   flags;
} C4_WindowOptions;

void app_init(void);

bool app_open_window(C4_WindowOptions options, C4_Window* w_* window);
void app_close_window(C4_Window* window);

// Event sum type

typedef enum
{
	RESIZED,
	MOVED,

	CLOSE_REQUESTED,
	DESTROYED,

	// FILE_DROPPED,
	// FILE_HOVERED,
	// FILE_HOVERED_CANCELLED,

	KEYBOARD,
	MOUSE,
	TOUCH,

	REDRAW,

	FOCUS_IN,
	FOCUS_OUT,
} C4_EventTag;

typedef struct
{
	C4_WindowSize size;
} C4_EventKindResized;

typedef struct
{
	C4_WindowPos pos;
} C4_EventKindMoved;

typedef struct
{
	C4_KeyboardEvent kb_event;
} C4_EventKindKeyboard;

typedef union
{
	C4_EventKindResized resized;
	C4_EventKindMoved   moved;

	// EventKindFileDropped          file_dropped;
	// EventKindFileHovered          file_hovered;
	// EventKindFileHoveredCancelled file_hovered_cancelled;

} C4_EventKind;

typedef struct
{
	u64          time;
	C4_Window*   window;
	C4_EventTag  tag;
	C4_EventKind kind;
} C4_Event;

bool app_poll_event(C4_Event w_* event);
