#include "keycode.h"

// #include <X11/keysym.h>

// clang-format off
C4_KeyCode keysym_to_keycode(XID keysym)
{
	switch (keysym)
	{
		case 0x31:                return C4_KEY_1;
		case 0x32:                return C4_KEY_2;
		case 0x33:                return C4_KEY_3;
		case 0x34:                return C4_KEY_4;
		case 0x35:                return C4_KEY_5;
		case 0x36:                return C4_KEY_6;
		case 0x37:                return C4_KEY_7;
		case 0x38:                return C4_KEY_8;
		case 0x39:                return C4_KEY_9;
		case 0x30:                return C4_KEY_0;

		case 0x7e:                return C4_KEY_TILDE;
		case 0x60:                return C4_KEY_BACKTICK;
		case 0x21:                return C4_KEY_EXCLAMATION_MARK;
		case 0x40:                return C4_KEY_AT;
		case 0x23:                return C4_KEY_HASHTAG;
		case 0x24:                return C4_KEY_DOLLAR;
		case 0x25:                return C4_KEY_PERCENT;
		case 0x5e:                return C4_KEY_CARET;
		case 0x26:                return C4_KEY_AMPERSAND;
		case 0x2a:                return C4_KEY_ASTERISK;
		case 0x28:                return C4_KEY_L_PARENS;
		case 0x29:                return C4_KEY_R_PARENS;
		case 0x5f:                return C4_KEY_UNDERSCORE;
		case 0x2d:                return C4_KEY_MINUS;
		case 0x2b:                return C4_KEY_PLUS;
		case 0x3d:                return C4_KEY_EQUALS;
		case 0xff08:              return C4_KEY_BACKSPACE;

		case 0x41: case 0x61:     return C4_KEY_A;
		case 0x42: case 0x62:     return C4_KEY_B;
		case 0x43: case 0x63:     return C4_KEY_C;
		case 0x44: case 0x64:     return C4_KEY_D;
		case 0x45: case 0x65:     return C4_KEY_E;
		case 0x46: case 0x66:     return C4_KEY_F;
		case 0x47: case 0x67:     return C4_KEY_G;
		case 0x48: case 0x68:     return C4_KEY_H;
		case 0x49: case 0x69:     return C4_KEY_I;
		case 0x4a: case 0x6a:     return C4_KEY_J;
		case 0x4b: case 0x6b:     return C4_KEY_K;
		case 0x4c: case 0x6c:     return C4_KEY_L;
		case 0x4d: case 0x6d:     return C4_KEY_M;
		case 0x4e: case 0x6e:     return C4_KEY_N;
		case 0x4f: case 0x6f:     return C4_KEY_O;
		case 0x50: case 0x70:     return C4_KEY_P;
		case 0x51: case 0x71:     return C4_KEY_Q;
		case 0x52: case 0x72:     return C4_KEY_R;
		case 0x53: case 0x73:     return C4_KEY_S;
		case 0x54: case 0x74:     return C4_KEY_T;
		case 0x55: case 0x75:     return C4_KEY_U;
		case 0x56: case 0x76:     return C4_KEY_V;
		case 0x57: case 0x77:     return C4_KEY_W;
		case 0x58: case 0x78:     return C4_KEY_X;
		case 0x59: case 0x79:     return C4_KEY_Y;
		case 0x5a: case 0x7a:     return C4_KEY_Z;

		case 0x20:                return C4_KEY_SPACE;

		case 0x5b:                return C4_KEY_L_BRACKET;
		case 0x5d:                return C4_KEY_R_BRACKET;
		case 0x7b:                return C4_KEY_L_BRACE;
		case 0x7d:                return C4_KEY_R_BRACE;
		case 0x7c:                return C4_KEY_PIPE;
		case 0x5c:                return C4_KEY_BACKSLASH;
		case 0x3a:                return C4_KEY_COLON;
		case 0x3b:                return C4_KEY_SEMICOLON;
		case 0x22:                return C4_KEY_DOUBLE_QUOTE;
		case 0x27:                return C4_KEY_SINGLE_QUOTE;
		case 0x3c:                return C4_KEY_L_ANGLE_BRACKET;
		case 0x3e:                return C4_KEY_R_ANGLE_BRACKET;
		case 0x3f:                return C4_KEY_QUESTION_MARK;
		case 0x2c:                return C4_KEY_COMMA;
		case 0x2e:                return C4_KEY_POINT;
		case 0x2f:                return C4_KEY_SLASH;

		case 0xff09:              return C4_KEY_TAB;
		case 0xffe5:              return C4_KEY_CAPS_LOCK;
		case 0xff0d:              return C4_KEY_ENTER;

		case 0xffe1:              return C4_KEY_L_SHIFT;
		case 0xffe2:              return C4_KEY_R_SHIFT;
		case 0xffe3:              return C4_KEY_L_CTRL;
		case 0xffe4:              return C4_KEY_R_CTRL;
		case 0xffe9:              return C4_KEY_L_ALT;
		case 0xfe03:              return C4_KEY_R_ALT;

		case 0xffeb:              return C4_KEY_L_SUPER;

		case 0xff51:              return C4_KEY_ARROW_LEFT;
		case 0xff52:              return C4_KEY_ARROW_UP;
		case 0xff53:              return C4_KEY_ARROW_RIGHT;
		case 0xff54:              return C4_KEY_ARROW_DOWN;

		case 0xff50:              return C4_KEY_HOME;
		case 0xff55:              return C4_KEY_PAGE_UP;
		case 0xff57:              return C4_KEY_END;
		case 0xff56:              return C4_KEY_PAGE_DOWN;

		case 0xff1b:              return C4_KEY_ESCAPE;

		case 0xffbe:              return C4_KEY_F1;
		case 0xffbf:              return C4_KEY_F2;
		case 0xffc0:              return C4_KEY_F3;
		case 0xffc1:              return C4_KEY_F4;
		case 0xffc2:              return C4_KEY_F5;
		case 0xffc3:              return C4_KEY_F6;
		case 0xffc4:              return C4_KEY_F7;
		case 0xffc5:              return C4_KEY_F8;
		case 0xffc6:              return C4_KEY_F9;
		case 0xffc7:              return C4_KEY_F10;
		case 0xffc8:              return C4_KEY_F11;
		case 0xffc9:              return C4_KEY_F12;

		case 0xffff:              return C4_KEY_DELETE;

		case 0x1008ff14:          return C4_KEY_PLAY;
		case 0x1008ff15:          return C4_KEY_STOP;
		case 0x1008ff16:          return C4_KEY_PREV_TRACK;
		case 0x1008ff17:          return C4_KEY_NEXT_TRACK;

		case 0xff61: case 0xff15: return C4_KEY_PRINT_SCREEN;

		default:                  return C4_KEY_UNKNOWN;
	}
}

char* keycode_display(C4_KeyCode keycode)
{
	switch (keycode)
	{
		case C4_KEY_UNKNOWN:          return "(unknown)";
		case C4_KEY_1:                return "1";
		case C4_KEY_2:                return "2";
		case C4_KEY_3:                return "3";
		case C4_KEY_4:                return "4";
		case C4_KEY_5:                return "5";
		case C4_KEY_6:                return "6";
		case C4_KEY_7:                return "7";
		case C4_KEY_8:                return "8";
		case C4_KEY_9:                return "9";
		case C4_KEY_0:                return "0";
		case C4_KEY_TILDE:            return "~";
		case C4_KEY_BACKTICK:         return "`";
		case C4_KEY_EXCLAMATION_MARK: return "!";
		case C4_KEY_AT:               return "@";
		case C4_KEY_HASHTAG:          return "#";
		case C4_KEY_DOLLAR:           return "$";
		case C4_KEY_PERCENT:          return "%";
		case C4_KEY_CARET:            return "^";
		case C4_KEY_AMPERSAND:        return "&";
		case C4_KEY_ASTERISK:         return "*";
		case C4_KEY_L_PARENS:         return "(";
		case C4_KEY_R_PARENS:         return ")";
		case C4_KEY_UNDERSCORE:       return "_";
		case C4_KEY_MINUS:            return "-";
		case C4_KEY_PLUS:             return "+";
		case C4_KEY_EQUALS:           return "=";
		case C4_KEY_BACKSPACE:        return "Backspace";
		case C4_KEY_A:                return "A";
		case C4_KEY_B:                return "B";
		case C4_KEY_C:                return "C";
		case C4_KEY_D:                return "D";
		case C4_KEY_E:                return "E";
		case C4_KEY_F:                return "F";
		case C4_KEY_G:                return "G";
		case C4_KEY_H:                return "H";
		case C4_KEY_I:                return "I";
		case C4_KEY_J:                return "J";
		case C4_KEY_K:                return "K";
		case C4_KEY_L:                return "L";
		case C4_KEY_M:                return "M";
		case C4_KEY_N:                return "N";
		case C4_KEY_O:                return "O";
		case C4_KEY_P:                return "P";
		case C4_KEY_Q:                return "Q";
		case C4_KEY_R:                return "R";
		case C4_KEY_S:                return "S";
		case C4_KEY_T:                return "T";
		case C4_KEY_U:                return "U";
		case C4_KEY_V:                return "V";
		case C4_KEY_W:                return "W";
		case C4_KEY_X:                return "X";
		case C4_KEY_Y:                return "Y";
		case C4_KEY_Z:                return "Z";
		case C4_KEY_SPACE:            return "Space";
		case C4_KEY_L_BRACKET:        return "[";
		case C4_KEY_R_BRACKET:        return "]";
		case C4_KEY_L_BRACE:          return "{";
		case C4_KEY_R_BRACE:          return "}";
		case C4_KEY_PIPE:             return "|";
		case C4_KEY_BACKSLASH:        return "\\";
		case C4_KEY_COLON:            return ":";
		case C4_KEY_SEMICOLON:        return ";";
		case C4_KEY_DOUBLE_QUOTE:     return "\"";
		case C4_KEY_SINGLE_QUOTE:     return "'";
		case C4_KEY_L_ANGLE_BRACKET:  return "<";
		case C4_KEY_R_ANGLE_BRACKET:  return ">";
		case C4_KEY_QUESTION_MARK:    return "?";
		case C4_KEY_COMMA:            return ",";
		case C4_KEY_POINT:            return ".";
		case C4_KEY_SLASH:            return "/";
		case C4_KEY_TAB:              return "Tab";
		case C4_KEY_CAPS_LOCK:        return "CapsLock";
		case C4_KEY_ENTER:            return "Enter";
		case C4_KEY_L_SHIFT:          return "LShift";
		case C4_KEY_R_SHIFT:          return "RShift";
		case C4_KEY_L_CTRL:           return "LCtrl";
		case C4_KEY_R_CTRL:           return "RCtrl";
		case C4_KEY_L_ALT:            return "LAlt";
		case C4_KEY_R_ALT:            return "RAlt";
		case C4_KEY_L_SUPER:          return "LSuper";
		case C4_KEY_R_SUPER:          return "RSuper";
		case C4_KEY_ARROW_LEFT:       return "←";
		case C4_KEY_ARROW_UP:         return "↑";
		case C4_KEY_ARROW_RIGHT:      return "→";
		case C4_KEY_ARROW_DOWN:       return "↓";
		case C4_KEY_HOME:             return "Home";
		case C4_KEY_PAGE_UP:          return "PageUp";
		case C4_KEY_END:              return "End";
		case C4_KEY_PAGE_DOWN:        return "PageDown";
		case C4_KEY_ESCAPE:           return "Escape";
		case C4_KEY_F1:               return "F1";
		case C4_KEY_F2:               return "F2";
		case C4_KEY_F3:               return "F3";
		case C4_KEY_F4:               return "F4";
		case C4_KEY_F5:               return "F5";
		case C4_KEY_F6:               return "F6";
		case C4_KEY_F7:               return "F7";
		case C4_KEY_F8:               return "F8";
		case C4_KEY_F9:               return "F9";
		case C4_KEY_F10:              return "F10";
		case C4_KEY_F11:              return "F11";
		case C4_KEY_F12:              return "F12";
		case C4_KEY_DELETE:           return "Delete";
		case C4_KEY_PLAY:             return "Play";
		case C4_KEY_STOP:             return "Stop";
		case C4_KEY_PREV_TRACK:       return "PrevTrack";
		case C4_KEY_NEXT_TRACK:       return "NextTrack";
		case C4_KEY_PRINT_SCREEN:     return "PrintScreen";
		case C4_KEY_NUM_LOCK:         return "NumLock";
		case C4_KEY_NUMPAD_0:         return "Numpad_0";
		case C4_KEY_NUMPAD_1:         return "Numpad_1";
		case C4_KEY_NUMPAD_2:         return "Numpad_2";
		case C4_KEY_NUMPAD_3:         return "Numpad_3";
		case C4_KEY_NUMPAD_4:         return "Numpad_4";
		case C4_KEY_NUMPAD_5:         return "Numpad_5";
		case C4_KEY_NUMPAD_6:         return "Numpad_6";
		case C4_KEY_NUMPAD_7:         return "Numpad_7";
		case C4_KEY_NUMPAD_8:         return "Numpad_8";
		case C4_KEY_NUMPAD_9:         return "Numpad_9";
		case C4_KEY_NUMPAD_ADD:       return "NumpadAdd";
		case C4_KEY_NUMPAD_DIVIDE:    return "NumpadDivide";
		case C4_KEY_NUMPAD_DECIMAL:   return "NumpadDecimal";
		case C4_KEY_NUMPAD_COMMA:     return "NumpadComma";
		case C4_KEY_NUMPAD_ENTER:     return "NumpadEnter";
		case C4_KEY_NUMPAD_EQUALS:    return "NumpadEquals";
		case C4_KEY_NUMPAD_MULTIPLY:  return "NumpadMultiply";
		case C4_KEY_NUMPAD_SUBTRACT:  return "NumpadSubtract";
	}
}
// clang-format on
