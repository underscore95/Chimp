#pragma once

// https://github.com/underscore95/Spark/blob/main/src/window/api/KeysAndButtons.h

namespace Chimp
{
	class Mouse {
	private:
		Mouse() = default;

	public:
		typedef int Button;

		static constexpr Button LEFT = 0;
		static constexpr Button RIGHT = 1;
		static constexpr Button MIDDLE = 2;

		static constexpr Button LAST_BUTTON = MIDDLE;
	};

	class Keyboard {
	private:
		Keyboard() = default;

	public:
		// These are equal to the same values as GLFW keys.

		typedef int Key;

		static constexpr Key SPACE = 32;
		static constexpr Key APOSTROPHE = 39;
		static constexpr Key COMMA = 44;
		static constexpr Key MINUS = 45;
		static constexpr Key PERIOD = 46;
		static constexpr Key SLASH = 47;
		static constexpr Key NUM_0 = 48;
		static constexpr Key NUM_1 = 49;
		static constexpr Key NUM_2 = 50;
		static constexpr Key NUM_3 = 51;
		static constexpr Key NUM_4 = 52;
		static constexpr Key NUM_5 = 53;
		static constexpr Key NUM_6 = 54;
		static constexpr Key NUM_7 = 55;
		static constexpr Key NUM_8 = 56;
		static constexpr Key NUM_9 = 57;
		static constexpr Key SEMICOLON = 59;
		static constexpr Key EQUAL = 61;
		static constexpr Key A = 65;
		static constexpr Key B = 66;
		static constexpr Key C = 67;
		static constexpr Key D = 68;
		static constexpr Key E = 69;
		static constexpr Key F = 70;
		static constexpr Key G = 71;
		static constexpr Key H = 72;
		static constexpr Key I = 73;
		static constexpr Key J = 74;
		static constexpr Key K = 75;
		static constexpr Key L = 76;
		static constexpr Key M = 77;
		static constexpr Key N = 78;
		static constexpr Key O = 79;
		static constexpr Key P = 80;
		static constexpr Key Q = 81;
		static constexpr Key R = 82;
		static constexpr Key S = 83;
		static constexpr Key T = 84;
		static constexpr Key U = 85;
		static constexpr Key V = 86;
		static constexpr Key W = 87;
		static constexpr Key X = 88;
		static constexpr Key Y = 89;
		static constexpr Key Z = 90;
		static constexpr Key LEFT_BRACKET = 91;
		static constexpr Key BACKSLASH = 92;
		static constexpr Key RIGHT_BRACKET = 93;
		static constexpr Key GRAVE_ACCENT = 96;
		static constexpr Key WORLD_1 = 161;
		static constexpr Key WORLD_2 = 162;
		static constexpr Key ESCAPE = 256;
		static constexpr Key ENTER = 257;
		static constexpr Key TAB = 258;
		static constexpr Key BACKSPACE = 259;
		static constexpr Key INSERT = 260;
		static constexpr Key DELETE = 261;
		static constexpr Key RIGHT = 262;
		static constexpr Key LEFT = 263;
		static constexpr Key DOWN = 264;
		static constexpr Key UP = 265;
		static constexpr Key PAGE_UP = 266;
		static constexpr Key PAGE_DOWN = 267;
		static constexpr Key HOME = 268;
		static constexpr Key END = 269;
		static constexpr Key CAPS_LOCK = 280;
		static constexpr Key SCROLL_LOCK = 281;
		static constexpr Key NUM_LOCK = 282;
		static constexpr Key PRINT_SCREEN = 283;
		static constexpr Key PAUSE = 284;
		static constexpr Key F1 = 290;
		static constexpr Key F2 = 291;
		static constexpr Key F3 = 292;
		static constexpr Key F4 = 293;
		static constexpr Key F5 = 294;
		static constexpr Key F6 = 295;
		static constexpr Key F7 = 296;
		static constexpr Key F8 = 297;
		static constexpr Key F9 = 298;
		static constexpr Key F10 = 299;
		static constexpr Key F11 = 300;
		static constexpr Key F12 = 301;
		static constexpr Key F13 = 302;
		static constexpr Key F14 = 303;
		static constexpr Key F15 = 304;
		static constexpr Key F16 = 305;
		static constexpr Key F17 = 306;
		static constexpr Key F18 = 307;
		static constexpr Key F19 = 308;
		static constexpr Key F20 = 309;
		static constexpr Key F21 = 310;
		static constexpr Key F22 = 311;
		static constexpr Key F23 = 312;
		static constexpr Key F24 = 313;
		static constexpr Key F25 = 314;
		static constexpr Key KP_0 = 320;
		static constexpr Key KP_1 = 321;
		static constexpr Key KP_2 = 322;
		static constexpr Key KP_3 = 323;
		static constexpr Key KP_4 = 324;
		static constexpr Key KP_5 = 325;
		static constexpr Key KP_6 = 326;
		static constexpr Key KP_7 = 327;
		static constexpr Key KP_8 = 328;
		static constexpr Key KP_9 = 329;
		static constexpr Key KP_DECIMAL = 330;
		static constexpr Key KP_DIVIDE = 331;
		static constexpr Key KP_MULTIPLY = 332;
		static constexpr Key KP_SUBTRACT = 333;
		static constexpr Key KP_ADD = 334;
		static constexpr Key KP_ENTER = 335;
		static constexpr Key KP_EQUAL = 336;
		static constexpr Key LEFT_SHIFT = 340;
		static constexpr Key LEFT_CONTROL = 341;
		static constexpr Key LEFT_ALT = 342;
		static constexpr Key LEFT_SUPER = 343;
		static constexpr Key RIGHT_SHIFT = 344;
		static constexpr Key RIGHT_CONTROL = 345;
		static constexpr Key RIGHT_ALT = 346;
		static constexpr Key RIGHT_SUPER = 347;
		static constexpr Key MENU = 348;

		static constexpr Key LAST_KEY = MENU;
	};
}