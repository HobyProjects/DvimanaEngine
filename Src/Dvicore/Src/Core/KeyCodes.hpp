#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#ifdef APIENTRY
#undef APIENTRY
#endif

namespace Dvimana {

    enum KeyCodes : int32_t {

		KEY_NUM_0 = GLFW_KEY_0,
		KEY_NUM_1 = GLFW_KEY_1,
		KEY_NUM_2 = GLFW_KEY_2,
		KEY_NUM_3 = GLFW_KEY_3,
		KEY_NUM_4 = GLFW_KEY_4,
		KEY_NUM_5 = GLFW_KEY_5,
		KEY_NUM_6 = GLFW_KEY_6,
		KEY_NUM_7 = GLFW_KEY_7,
		KEY_NUM_8 = GLFW_KEY_8,
		KEY_NUM_9 = GLFW_KEY_9,

		KEY_A = GLFW_KEY_A,
		KEY_B = GLFW_KEY_B,
		KEY_C = GLFW_KEY_C,
		KEY_D = GLFW_KEY_D,
		KEY_E = GLFW_KEY_E,
		KEY_F = GLFW_KEY_F,
		KEY_G = GLFW_KEY_G,
		KEY_H = GLFW_KEY_H,
		KEY_I = GLFW_KEY_I,
		KEY_J = GLFW_KEY_J,
		KEY_K = GLFW_KEY_K,
		KEY_L = GLFW_KEY_L,
		KEY_M = GLFW_KEY_M,
		KEY_N = GLFW_KEY_N,
		KEY_O = GLFW_KEY_O,
		KEY_P = GLFW_KEY_P,
		KEY_Q = GLFW_KEY_Q,
		KEY_R = GLFW_KEY_R,
		KEY_S = GLFW_KEY_S,
		KEY_T = GLFW_KEY_T,
		KEY_U = GLFW_KEY_U,
		KEY_V = GLFW_KEY_V,
		KEY_W = GLFW_KEY_W,
		KEY_X = GLFW_KEY_X,
		KEY_Y = GLFW_KEY_Y,
		KEY_Z = GLFW_KEY_Z,

		KEY_F1  = GLFW_KEY_F1,
		KEY_F2  = GLFW_KEY_F2,
		KEY_F3  = GLFW_KEY_F3,
		KEY_F4  = GLFW_KEY_F4,
		KEY_F5  = GLFW_KEY_F5,
		KEY_F6  = GLFW_KEY_F6,
		KEY_F7  = GLFW_KEY_F7,
		KEY_F8  = GLFW_KEY_F8,
		KEY_F9  = GLFW_KEY_F9,
		KEY_F10 = GLFW_KEY_F10,
		KEY_F11 = GLFW_KEY_F11,
		KEY_F12 = GLFW_KEY_F12,

        KEY_SEMICOLON       = GLFW_KEY_SEMICOLON,
        KEY_EQUAL           = GLFW_KEY_EQUAL,
        KEY_SPACE           = GLFW_KEY_SPACE,
        KEY_ESCAPE          = GLFW_KEY_ESCAPE,
        KEY_APOSTROPHE      = GLFW_KEY_APOSTROPHE,
        KEY_COMMA           = GLFW_KEY_COMMA,
        KEY_MINUS           = GLFW_KEY_MINUS,
        KEY_PERIOD          = GLFW_KEY_PERIOD,
        KEY_SLASH           = GLFW_KEY_SLASH,
        KEY_LEFT_BRACKET    = GLFW_KEY_LEFT_BRACKET,
        KEY_BACKSLASH       = GLFW_KEY_BACKSLASH,
        KEY_RIGHT_BRACKET   = GLFW_KEY_RIGHT_BRACKET,
        KEY_GRAVE_ACCENT    = GLFW_KEY_GRAVE_ACCENT,
        KEY_WORLD_1         = GLFW_KEY_WORLD_1,
        KEY_WORLD_2         = GLFW_KEY_WORLD_2,
        KEY_ENTER           = GLFW_KEY_ENTER,
        KEY_TAB             = GLFW_KEY_TAB,
        KEY_BACKSPACE       = GLFW_KEY_BACKSPACE,
        KEY_INSERT          = GLFW_KEY_INSERT,
        KEY_DELETE          = GLFW_KEY_DELETE,
        KEY_RIGHT           = GLFW_KEY_RIGHT,
        KEY_LEFT            = GLFW_KEY_LEFT,
        KEY_DOWN            = GLFW_KEY_DOWN,
        KEY_UP              = GLFW_KEY_UP,
        KEY_PAGE_UP         = GLFW_KEY_PAGE_UP,
        KEY_PAGE_DOWN       = GLFW_KEY_PAGE_DOWN,
        KEY_HOME            = GLFW_KEY_HOME,
        KEY_END             = GLFW_KEY_END,
        KEY_CAPS_LOCK       = GLFW_KEY_CAPS_LOCK,
        KEY_SCROLL_LOCK     = GLFW_KEY_SCROLL_LOCK,
        KEY_NUM_LOCK        = GLFW_KEY_NUM_LOCK,
        KEY_PRINT_SCREEN    = GLFW_KEY_PRINT_SCREEN,
        KEY_PAUSE           = GLFW_KEY_PAUSE,

        KEY_KP_0 = GLFW_KEY_KP_0,
        KEY_KP_1 = GLFW_KEY_KP_1,
        KEY_KP_2 = GLFW_KEY_KP_2,
        KEY_KP_3 = GLFW_KEY_KP_3,
        KEY_KP_4 = GLFW_KEY_KP_4,
        KEY_KP_5 = GLFW_KEY_KP_5,
        KEY_KP_6 = GLFW_KEY_KP_6,
        KEY_KP_7 = GLFW_KEY_KP_7,
        KEY_KP_8 = GLFW_KEY_KP_8,
        KEY_KP_9 = GLFW_KEY_KP_9,

        KEY_KP_DECIMAL      = GLFW_KEY_KP_DECIMAL,
        KEY_KP_DIVIDE       = GLFW_KEY_KP_DIVIDE,
        KEY_KP_MULTIPLY     = GLFW_KEY_KP_MULTIPLY,
        KEY_KP_SUBTRACT     = GLFW_KEY_KP_SUBTRACT,
        KEY_KP_ADD          = GLFW_KEY_KP_ADD,
        KEY_KP_ENTER        = GLFW_KEY_KP_ENTER,
        KEY_KP_EQUAL        = GLFW_KEY_KP_EQUAL,

        KEY_LEFT_SHIFT      = GLFW_KEY_LEFT_SHIFT,
        KEY_LEFT_CONTROL    = GLFW_KEY_LEFT_CONTROL,
        KEY_LEFT_ALT        = GLFW_KEY_LEFT_ALT,
        KEY_LEFT_SUPER      = GLFW_KEY_LEFT_SUPER,

        KEY_RIGHT_SHIFT     = GLFW_KEY_RIGHT_SHIFT,
        KEY_RIGHT_CONTROL   = GLFW_KEY_RIGHT_CONTROL,
        KEY_RIGHT_ALT       = GLFW_KEY_RIGHT_ALT,
        KEY_RIGHT_SUPER     = GLFW_KEY_RIGHT_SUPER,

        KEY_MENU            = GLFW_KEY_MENU,
        KEY_UNKNOWN         = GLFW_KEY_UNKNOWN

	};

    enum MouseButton : uint32_t {
        MOUSE_BUTTON_LEFT   = GLFW_MOUSE_BUTTON_LEFT,
        MOUSE_BUTTON_RIGHT  = GLFW_MOUSE_BUTTON_RIGHT,
        MOUSE_BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
    };

    inline KeyCodes GetKeyCode(int key){
        switch(key) {
            case GLFW_KEY_A: return KEY_A;
            case GLFW_KEY_B: return KEY_B;
            case GLFW_KEY_C: return KEY_C;
            case GLFW_KEY_D: return KEY_D;
            case GLFW_KEY_E: return KEY_E;
            case GLFW_KEY_F: return KEY_F;
            case GLFW_KEY_G: return KEY_G;
            case GLFW_KEY_H: return KEY_H;
            case GLFW_KEY_I: return KEY_I;
            case GLFW_KEY_J: return KEY_J;
            case GLFW_KEY_K: return KEY_K;
            case GLFW_KEY_L: return KEY_L;
            case GLFW_KEY_M: return KEY_M;
            case GLFW_KEY_N: return KEY_N;
            case GLFW_KEY_O: return KEY_O;
            case GLFW_KEY_P: return KEY_P;
            case GLFW_KEY_Q: return KEY_Q;
            case GLFW_KEY_R: return KEY_R;
            case GLFW_KEY_S: return KEY_S;
            case GLFW_KEY_T: return KEY_T;
            case GLFW_KEY_U: return KEY_U;
            case GLFW_KEY_V: return KEY_V;
            case GLFW_KEY_W: return KEY_W;
            case GLFW_KEY_X: return KEY_X;
            case GLFW_KEY_Y: return KEY_Y;
            case GLFW_KEY_Z: return KEY_Z;
            case GLFW_KEY_SPACE: return KEY_SPACE;
            case GLFW_KEY_ENTER: return KEY_ENTER;
            case GLFW_KEY_ESCAPE: return KEY_ESCAPE;
            case GLFW_KEY_LEFT_SHIFT: return KEY_LEFT_SHIFT;
            case GLFW_KEY_RIGHT_SHIFT: return KEY_RIGHT_SHIFT;
            case GLFW_KEY_LEFT_CONTROL: return KEY_LEFT_CONTROL;
            case GLFW_KEY_RIGHT_CONTROL: return KEY_RIGHT_CONTROL;
            case GLFW_KEY_LEFT_ALT: return KEY_LEFT_ALT;
            case GLFW_KEY_RIGHT_ALT: return KEY_RIGHT_ALT;
            case GLFW_KEY_TAB: return KEY_TAB;
            case GLFW_KEY_BACKSPACE: return KEY_BACKSPACE;
            case GLFW_KEY_INSERT: return KEY_INSERT;
            case GLFW_KEY_DELETE: return KEY_DELETE;
            case GLFW_KEY_HOME: return KEY_HOME;
            case GLFW_KEY_END: return KEY_END;
            case GLFW_KEY_PAGE_UP: return KEY_PAGE_UP;
            case GLFW_KEY_PAGE_DOWN: return KEY_PAGE_DOWN;
            case GLFW_KEY_LEFT: return KEY_LEFT;
            case GLFW_KEY_RIGHT: return KEY_RIGHT;
            case GLFW_KEY_UP: return KEY_UP;
            case GLFW_KEY_DOWN: return KEY_DOWN;
            case GLFW_KEY_F1: return KEY_F1;
            case GLFW_KEY_F2: return KEY_F2;
            case GLFW_KEY_F3: return KEY_F3;
            case GLFW_KEY_F4: return KEY_F4;
            case GLFW_KEY_F5: return KEY_F5;
            case GLFW_KEY_F6: return KEY_F6;
            case GLFW_KEY_F7: return KEY_F7;
            case GLFW_KEY_F8: return KEY_F8;
            case GLFW_KEY_F9: return KEY_F9;
            case GLFW_KEY_F10: return KEY_F10;
            case GLFW_KEY_F11: return KEY_F11;
            case GLFW_KEY_F12: return KEY_F12;
            case GLFW_KEY_0: return KEY_NUM_0;
            case GLFW_KEY_1: return KEY_NUM_1;
            case GLFW_KEY_2: return KEY_NUM_2;
            case GLFW_KEY_3: return KEY_NUM_3;
            case GLFW_KEY_4: return KEY_NUM_4;
            case GLFW_KEY_5: return KEY_NUM_5;
            case GLFW_KEY_6: return KEY_NUM_6;
            case GLFW_KEY_7: return KEY_NUM_7;
            case GLFW_KEY_8: return KEY_NUM_8;
            case GLFW_KEY_9: return KEY_NUM_9;
            case GLFW_KEY_KP_0: return KEY_KP_0;
            case GLFW_KEY_KP_1: return KEY_KP_1;
            case GLFW_KEY_KP_2: return KEY_KP_2;
            case GLFW_KEY_KP_3: return KEY_KP_3;
            case GLFW_KEY_KP_4: return KEY_KP_4;
            case GLFW_KEY_KP_5: return KEY_KP_5;
            case GLFW_KEY_KP_6: return KEY_KP_6;
            case GLFW_KEY_KP_7: return KEY_KP_7;
            case GLFW_KEY_KP_8: return KEY_KP_8;
            case GLFW_KEY_KP_9: return KEY_KP_9;
            case GLFW_KEY_KP_DECIMAL: return KEY_KP_DECIMAL;
            case GLFW_KEY_KP_DIVIDE: return KEY_KP_DIVIDE;
            case GLFW_KEY_KP_MULTIPLY: return KEY_KP_MULTIPLY;
            case GLFW_KEY_KP_SUBTRACT: return KEY_KP_SUBTRACT;
            case GLFW_KEY_KP_ADD: return KEY_KP_ADD;
            case GLFW_KEY_KP_ENTER: return KEY_KP_ENTER;
            case GLFW_KEY_KP_EQUAL: return KEY_KP_EQUAL;
            case GLFW_KEY_LEFT_SUPER: return KEY_LEFT_SUPER;
            case GLFW_KEY_RIGHT_SUPER: return KEY_RIGHT_SUPER;
            case GLFW_KEY_MENU: return KEY_MENU;
            default: return KEY_UNKNOWN;
        }
    }
}