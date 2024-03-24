#pragma once

#include <X11/X.h>

#include "../keycode.h"

C4_KeyCode keysym_to_keycode(XID keysym);
