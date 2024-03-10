#pragma once

#include "../types.h"
#include "keyboard.h"

typedef struct
{
	usize raw;
} WindowHandle;

typedef struct
{
	i32 x;
	i32 y;
} WindowPos;

typedef struct
{
	u32 width;
	u32 height;
} WindowSize;

typedef enum
{
	WINDOWED   = 0,
	BORDERLESS = 1,
	FULLSCREEN = 2,
} ScreenMode;

typedef struct
{
	usize raw;
} MonitorHandle;

typedef enum
{
	WINDOW_FLAG_CENTERED    = 1 << 0,
	WINDOW_FLAG_RESIZABLE   = 1 << 1,
	WINDOW_FLAG_MAXIMIZED   = 1 << 2,
	WINDOW_FLAG_TRANSPARENT = 1 << 3,
	WINDOW_FLAG_HIGH_DPI    = 1 << 4,
} WindowFlags;

typedef struct
{
	String8       title;
	WindowPos     position;
	WindowSize    size;
	MonitorHandle monitor;
	ScreenMode    screen_mode;
	WindowFlags   flags;
} WindowOptions;

void app_init(void);

bool app_create_window(WindowOptions options, WindowHandle out handle);
void app_close_window(WindowHandle handle);

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
} EventTag;

typedef struct
{
	WindowSize size;
} EventKindResized;

typedef struct
{
	WindowPos pos;
} EventKindMoved;

typedef struct
{
	KeyboardEvent kb_event;
} EventKindKeyboard;

typedef union
{
	EventKindResized resized;
	EventKindMoved   moved;

	// EventKindFileDropped          file_dropped;
	// EventKindFileHovered          file_hovered;
	// EventKindFileHoveredCancelled file_hovered_cancelled;

} EventKind;

typedef struct
{
	u64          time;
	WindowHandle window;
	EventTag     tag;
	EventKind    kind;
} Event;

bool app_poll_event(Event out event);
