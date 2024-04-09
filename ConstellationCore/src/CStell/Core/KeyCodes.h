#pragma once

namespace CStell
{
	typedef enum class KeyCode : uint16_t
	{
		CSTELL_KEY_SPACE             = 32,
		CSTELL_KEY_APOSTROPHE        = 39,	/* ' */
		CSTELL_KEY_COMMA             = 44,	/* , */
		CSTELL_KEY_MINUS             = 45,	/* - */
		CSTELL_KEY_PERIOD            = 46,	/* . */
		CSTELL_KEY_SLASH             = 47,	/* / */
		CSTELL_KEY_0                 = 48,
		CSTELL_KEY_1                 = 49,
		CSTELL_KEY_2                 = 50,
		CSTELL_KEY_3                 = 51,
		CSTELL_KEY_4                 = 52,
		CSTELL_KEY_5                 = 53,
		CSTELL_KEY_6                 = 54,
		CSTELL_KEY_7                 = 55,
		CSTELL_KEY_8                 = 56,
		CSTELL_KEY_9                 = 57,
		CSTELL_KEY_SEMICOLON         = 59,	/* ; */
		CSTELL_KEY_EQUAL             = 61,  /* = */
		CSTELL_KEY_A                 = 65,
		CSTELL_KEY_B                 = 66,
		CSTELL_KEY_C                 = 67,
		CSTELL_KEY_D                 = 68,
		CSTELL_KEY_E                 = 69,
		CSTELL_KEY_F                 = 70,
		CSTELL_KEY_G                 = 71,
		CSTELL_KEY_H                 = 72,
		CSTELL_KEY_I                 = 73,
		CSTELL_KEY_J                 = 74,
		CSTELL_KEY_K                 = 75,
		CSTELL_KEY_L                 = 76,
		CSTELL_KEY_M                 = 77,
		CSTELL_KEY_N                 = 78,
		CSTELL_KEY_O                 = 79,
		CSTELL_KEY_P                 = 80,
		CSTELL_KEY_Q                 = 81,
		CSTELL_KEY_R                 = 82,
		CSTELL_KEY_S                 = 83,
		CSTELL_KEY_T                 = 84,
		CSTELL_KEY_U                 = 85,
		CSTELL_KEY_V                 = 86,
		CSTELL_KEY_W                 = 87,
		CSTELL_KEY_X                 = 88,
		CSTELL_KEY_Y                 = 89,
		CSTELL_KEY_Z                 = 90,
		CSTELL_KEY_LEFT_BRACKET      = 91,	/* [ */
		CSTELL_KEY_BACKSLASH         = 92,	/* \ */
		CSTELL_KEY_RIGHT_BRACKET     = 93,	/* ] */
		CSTELL_KEY_GRAVE_ACCENT      = 96,	/* ` */
		CSTELL_KEY_WORLD_1           = 161,	/* non-US #1 */
		CSTELL_KEY_WORLD_2           = 162,	/* non-US #2 */

		/*function keys*/
		CSTELL_KEY_ESCAPE            = 256,
		CSTELL_KEY_ENTER             = 257,
		CSTELL_KEY_TAB               = 258,
		CSTELL_KEY_BACKSPACE         = 259,
		CSTELL_KEY_INSERT            = 260,
		CSTELL_KEY_DELETE            = 261,
		CSTELL_KEY_RIGHT             = 262,
		CSTELL_KEY_LEFT              = 263,
		CSTELL_KEY_DOWN              = 264,
		CSTELL_KEY_UP                = 265,
		CSTELL_KEY_PAGE_UP           = 266,
		CSTELL_KEY_PAGE_DOWN         = 267,
		CSTELL_KEY_HOME              = 268,
		CSTELL_KEY_END               = 269,
		CSTELL_KEY_CAPS_LOCK         = 280,
		CSTELL_KEY_SCROLL_LOCK       = 281,
		CSTELL_KEY_NUM_LOCK          = 282,
		CSTELL_KEY_PRINT_SCREEN      = 283,
		CSTELL_KEY_PAUSE             = 284,
		CSTELL_KEY_F1                = 290,
		CSTELL_KEY_F2                = 291,
		CSTELL_KEY_F3                = 292,
		CSTELL_KEY_F4                = 293,
		CSTELL_KEY_F5                = 294,
		CSTELL_KEY_F6                = 295,
		CSTELL_KEY_F7                = 296,
		CSTELL_KEY_F8                = 297,
		CSTELL_KEY_F9                = 298,
		CSTELL_KEY_F10               = 299,
		CSTELL_KEY_F11               = 300,
		CSTELL_KEY_F12               = 301,
		CSTELL_KEY_F13               = 302,
		CSTELL_KEY_F14               = 303,
		CSTELL_KEY_F15               = 304,
		CSTELL_KEY_F16               = 305,
		CSTELL_KEY_F17               = 306,
		CSTELL_KEY_F18               = 307,
		CSTELL_KEY_F19               = 308,
		CSTELL_KEY_F20               = 309,
		CSTELL_KEY_F21               = 310,
		CSTELL_KEY_F22               = 311,
		CSTELL_KEY_F23               = 312,
		CSTELL_KEY_F24               = 313,
		CSTELL_KEY_F25               = 314,
		CSTELL_KEY_KP_0              = 320,
		CSTELL_KEY_KP_1              = 321,
		CSTELL_KEY_KP_2              = 322,
		CSTELL_KEY_KP_3              = 323,
		CSTELL_KEY_KP_4              = 324,
		CSTELL_KEY_KP_5              = 325,
		CSTELL_KEY_KP_6              = 326,
		CSTELL_KEY_KP_7              = 327,
		CSTELL_KEY_KP_8              = 328,
		CSTELL_KEY_KP_9              = 329,
		CSTELL_KEY_KP_DECIMAL        = 330,
		CSTELL_KEY_KP_DIVIDE         = 331,
		CSTELL_KEY_KP_MULTIPLY       = 332,
		CSTELL_KEY_KP_SUBTRACT       = 333,
		CSTELL_KEY_KP_ADD            = 334,
		CSTELL_KEY_KP_ENTER          = 335,
		CSTELL_KEY_KP_EQUAL          = 336,
		CSTELL_KEY_LEFT_SHIFT        = 340,
		CSTELL_KEY_LEFT_CONTROL      = 341,
		CSTELL_KEY_LEFT_ALT          = 342,
		CSTELL_KEY_LEFT_SUPER        = 343,
		CSTELL_KEY_RIGHT_SHIFT       = 344,
		CSTELL_KEY_RIGHT_CONTROL     = 345,
		CSTELL_KEY_RIGHT_ALT         = 346,
		CSTELL_KEY_RIGHT_SUPER       = 347,
		CSTELL_KEY_MENU              = 348

	};
}

// From glfw3.h
#define CSTELL_KEY_SPACE              32
#define CSTELL_KEY_APOSTROPHE         39  /* ' */
#define CSTELL_KEY_COMMA              44  /* , */
#define CSTELL_KEY_MINUS              45  /* - */
#define CSTELL_KEY_PERIOD             46  /* . */
#define CSTELL_KEY_SLASH              47  /* / */
#define CSTELL_KEY_0                  48
#define CSTELL_KEY_1                  49
#define CSTELL_KEY_2                  50
#define CSTELL_KEY_3                  51
#define CSTELL_KEY_4                  52
#define CSTELL_KEY_5                  53
#define CSTELL_KEY_6                  54
#define CSTELL_KEY_7                  55
#define CSTELL_KEY_8                  56
#define CSTELL_KEY_9                  57
#define CSTELL_KEY_SEMICOLON          59  /* ; */
#define CSTELL_KEY_EQUAL              61  /* = */
#define CSTELL_KEY_A                  65
#define CSTELL_KEY_B                  66
#define CSTELL_KEY_C                  67
#define CSTELL_KEY_D                  68
#define CSTELL_KEY_E                  69
#define CSTELL_KEY_F                  70
#define CSTELL_KEY_G                  71
#define CSTELL_KEY_H                  72
#define CSTELL_KEY_I                  73
#define CSTELL_KEY_J                  74
#define CSTELL_KEY_K                  75
#define CSTELL_KEY_L                  76
#define CSTELL_KEY_M                  77
#define CSTELL_KEY_N                  78
#define CSTELL_KEY_O                  79
#define CSTELL_KEY_P                  80
#define CSTELL_KEY_Q                  81
#define CSTELL_KEY_R                  82
#define CSTELL_KEY_S                  83
#define CSTELL_KEY_T                  84
#define CSTELL_KEY_U                  85
#define CSTELL_KEY_V                  86
#define CSTELL_KEY_W                  87
#define CSTELL_KEY_X                  88
#define CSTELL_KEY_Y                  89
#define CSTELL_KEY_Z                  90
#define CSTELL_KEY_LEFT_BRACKET       91  /* [ */
#define CSTELL_KEY_BACKSLASH          92  /* \ */
#define CSTELL_KEY_RIGHT_BRACKET      93  /* ] */
#define CSTELL_KEY_GRAVE_ACCENT       96  /* ` */
#define CSTELL_KEY_WORLD_1            161 /* non-US #1 */
#define CSTELL_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define CSTELL_KEY_ESCAPE             256
#define CSTELL_KEY_ENTER              257
#define CSTELL_KEY_TAB                258
#define CSTELL_KEY_BACKSPACE          259
#define CSTELL_KEY_INSERT             260
#define CSTELL_KEY_DELETE             261
#define CSTELL_KEY_RIGHT              262
#define CSTELL_KEY_LEFT               263
#define CSTELL_KEY_DOWN               264
#define CSTELL_KEY_UP                 265
#define CSTELL_KEY_PAGE_UP            266
#define CSTELL_KEY_PAGE_DOWN          267
#define CSTELL_KEY_HOME               268
#define CSTELL_KEY_END                269
#define CSTELL_KEY_CAPS_LOCK          280
#define CSTELL_KEY_SCROLL_LOCK        281
#define CSTELL_KEY_NUM_LOCK           282
#define CSTELL_KEY_PRINT_SCREEN       283
#define CSTELL_KEY_PAUSE              284
#define CSTELL_KEY_F1                 290
#define CSTELL_KEY_F2                 291
#define CSTELL_KEY_F3                 292
#define CSTELL_KEY_F4                 293
#define CSTELL_KEY_F5                 294
#define CSTELL_KEY_F6                 295
#define CSTELL_KEY_F7                 296
#define CSTELL_KEY_F8                 297
#define CSTELL_KEY_F9                 298
#define CSTELL_KEY_F10                299
#define CSTELL_KEY_F11                300
#define CSTELL_KEY_F12                301
#define CSTELL_KEY_F13                302
#define CSTELL_KEY_F14                303
#define CSTELL_KEY_F15                304
#define CSTELL_KEY_F16                305
#define CSTELL_KEY_F17                306
#define CSTELL_KEY_F18                307
#define CSTELL_KEY_F19                308
#define CSTELL_KEY_F20                309
#define CSTELL_KEY_F21                310
#define CSTELL_KEY_F22                311
#define CSTELL_KEY_F23                312
#define CSTELL_KEY_F24                313
#define CSTELL_KEY_F25                314
#define CSTELL_KEY_KP_0               320
#define CSTELL_KEY_KP_1               321
#define CSTELL_KEY_KP_2               322
#define CSTELL_KEY_KP_3               323
#define CSTELL_KEY_KP_4               324
#define CSTELL_KEY_KP_5               325
#define CSTELL_KEY_KP_6               326
#define CSTELL_KEY_KP_7               327
#define CSTELL_KEY_KP_8               328
#define CSTELL_KEY_KP_9               329
#define CSTELL_KEY_KP_DECIMAL         330
#define CSTELL_KEY_KP_DIVIDE          331
#define CSTELL_KEY_KP_MULTIPLY        332
#define CSTELL_KEY_KP_SUBTRACT        333
#define CSTELL_KEY_KP_ADD             334
#define CSTELL_KEY_KP_ENTER           335
#define CSTELL_KEY_KP_EQUAL           336
#define CSTELL_KEY_LEFT_SHIFT         340
#define CSTELL_KEY_LEFT_CONTROL       341
#define CSTELL_KEY_LEFT_ALT           342
#define CSTELL_KEY_LEFT_SUPER         343
#define CSTELL_KEY_RIGHT_SHIFT        344
#define CSTELL_KEY_RIGHT_CONTROL      345
#define CSTELL_KEY_RIGHT_ALT          346
#define CSTELL_KEY_RIGHT_SUPER        347
#define CSTELL_KEY_MENU               348