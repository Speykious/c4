#pragma once

#include "../types.h"
#include "app_keycode.h"

typedef enum
{
	C4_KEYBOARD_EVENT_KEY_PRESS,
	C4_KEYBOARD_EVENT_KEY_RELEASE,
	C4_KEYBOARD_EVENT_KEY_REPEAT,
	C4_KEYBOARD_EVENT_IME_COMMIT,
} C4_KeyboardEventTag;

typedef union
{
	C4_KeyCode key_press;
	C4_KeyCode key_release;
	C4_KeyCode key_repeat;

	/** This string's lifetime ends when the next event polling starts. */
	String8 ime_commit;
} C4_KeyboardEventKind;

typedef struct
{
	C4_KeyboardEventTag  tag;
	C4_KeyboardEventKind kind;
} C4_KeyboardEvent;
