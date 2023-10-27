// -*- mode: c++ -*-
// Copyright 2016-2022 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details

/**
 * These #include directives pull in the Kaleidoscope firmware core,
 * as well as the Kaleidoscope plugins we use in the Model 100's firmware
 */

// The Kaleidoscope core
#include "Kaleidoscope.h"

// Support for storing the keymap in EEPROM
#include "Kaleidoscope-EEPROM-Settings.h"
#include "Kaleidoscope-EEPROM-Keymap.h"

// Support for communicating with the host via a simple Serial protocol
#include "Kaleidoscope-FocusSerial.h"

// Support for querying the firmware version via Focus
#include "Kaleidoscope-FirmwareVersion.h"

// Support for macros
#include "Kaleidoscope-Macros.h"

// Support for controlling the keyboard's LEDs
#include "Kaleidoscope-LEDControl.h"

// Support for the "Boot greeting" effect, which pulses the 'LED' button for 10s
// when the keyboard is connected to a computer (or that computer is powered on)
#include "Kaleidoscope-LEDEffect-BootGreeting.h"

// Support for shared palettes for other plugins, like Colormap below
#include "Kaleidoscope-LED-Palette-Theme.h"

// Support for an LED mode that lets one configure per-layer color maps
#include "Kaleidoscope-Colormap.h"

// Support for turning the LEDs off after a certain amount of time
#include "Kaleidoscope-IdleLEDs.h"

// Support for setting and saving the default LED mode
#include "Kaleidoscope-DefaultLEDModeConfig.h"

// Support for changing the brightness of the LEDs
#include "Kaleidoscope-LEDBrightnessConfig.h"

// Support for Keyboardio's internal keyboard testing mode
#include "Kaleidoscope-HardwareTestMode.h"

// Support for host power management (suspend & wakeup)
#include "Kaleidoscope-HostPowerManagement.h"

// Support for magic combos (key chords that trigger an action)
#include "Kaleidoscope-MagicCombo.h"

// Support for USB quirks, like changing the key state report protocol
#include "Kaleidoscope-USB-Quirks.h"

// Support for secondary actions on keys
#include "Kaleidoscope-Qukeys.h"

// Support for dynamic, Chrysalis-editable macros
#include "Kaleidoscope-DynamicMacros.h"

// Support for editable layer names
#include "Kaleidoscope-LayerNames.h"

/** This 'enum' is a list of all the macros used by the Model 100's firmware
	* The names aren't particularly important. What is important is that each
	* is unique.
	*
	* These are the names of your macros. They'll be used in two places.
	* The first is in your keymap definitions. There, you'll use the syntax
	* `M(MACRO_NAME)` to mark a specific keymap position as triggering `MACRO_NAME`
	*
	* The second usage is in the 'switch' statement in the `macroAction` function.
	* That switch statement actually runs the code associated with a macro when
	* a macro key is pressed.
	*/

enum {
	MACRO_VERSION_INFO,
	MACRO_ANY,
};


/** The Model 100's key layouts are defined as 'keymaps'. By default, there are three
	* keymaps: The standard QWERTY keymap, the "Function layer" keymap and the "Numpad"
	* keymap.
	*
	* Each keymap is defined as a list using the 'KEYMAP_STACKED' macro, built
	* of first the left hand's layout, followed by the right hand's layout.
	*
	* Keymaps typically consist mostly of `Key_` definitions. There are many, many keys
	* defined as part of the USB HID Keyboard specification. You can find the names
	* (if not yet the explanations) for all the standard `Key_` defintions offered by
	* Kaleidoscope in these files:
	*    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs/keyboard.h
	*    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs/consumerctl.h
	*    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs/sysctl.h
	*    https://github.com/keyboardio/Kaleidoscope/blob/master/src/kaleidoscope/key_defs/keymaps.h
	*
	* Additional things that should be documented here include
	*   using ___ to let keypresses fall through to the previously active layer
	*   using XXX to mark a keyswitch as 'blocked' on this layer
	*   using ShiftToLayer() and LockLayer() keys to change the active keymap.
	*   keeping NUM and FN consistent and accessible on all layers
	*
	* The PROG key is special, since it is how you indicate to the board that you
	* want to flash the firmware. However, it can be remapped to a regular key.
	* When the keyboard boots, it first looks to see whether the PROG key is held
	* down; if it is, it simply awaits further flashing instructions. If it is
	* not, it continues loading the rest of the firmware and the keyboard
	* functions normally, with whatever binding you have set to PROG. More detail
	* here: https://community.keyboard.io/t/how-the-prog-key-gets-you-into-the-bootloader/506/8
	*
	* The "keymaps" data structure is a list of the keymaps compiled into the firmware.
	* The order of keymaps in the list is important, as the ShiftToLayer(#) and LockLayer(#)
	* macros switch to key layers based on this list.
	*
	*

	* A key defined as 'ShiftToLayer(FUNCTION)' will switch to FUNCTION while held.
	* Similarly, a key defined as 'LockLayer(NUMPAD)' will switch to NUMPAD when tapped.
	*/

/**
	* Layers are "0-indexed" -- That is the first one is layer 0. The second one is layer 1.
	* The third one is layer 2.
	* This 'enum' lets us use names like QWERTY, FUNCTION, and NUMPAD in place of
	* the numbers 0, 1 and 2.
	*
	*/

enum {
	SPECIAL,
	PRIMARY,
	SYMBOL,
	NAVIGATION,
	GAMING,
	NUMPAD
};  // layers


/**
	* To change your keyboard's layout from QWERTY to DVORAK or COLEMAK, comment out the line
	*
	* #define PRIMARY_KEYMAP_QWERTY
	*
	* by changing it to
	*
	* // #define PRIMARY_KEYMAP_QWERTY
	*
	* Then uncomment the line corresponding to the layout you want to use.
	*
	*/

#define PRIMARY_KEYMAP_QWERTY

/* This comment temporarily turns off astyle's indent enforcement
 *   so we can make the keymaps actually resemble the physical key layout better
 */
// clang-format off

KEYMAPS(
	[SPECIAL] =  KEYMAP_STACKED
	(___,      Key_F1,                 Key_F2,                   Key_F3,                    Key_F4,					Key_F5,							Key_F6,
	 ___,      XXX,                    XXX,                      XXX,             			XXX,        			Consumer_ScanNextTrack,			___,
	 ___,      XXX,                    XXX,                      XXX,      					XXX, 					Consumer_PlaySlashPause,
	 ___,      ___,                    XXX,                      XXX,           			XXX,      				Consumer_ScanPreviousTrack,     XXX,
	 ___,      Key_Delete,             Key_Delete,               ___,
	 XXX,

	 Key_F7,	Key_F8,                 Key_F9,                   				Key_F10,					Key_F11,								Key_F12,				LockLayer(GAMING),
	 XXX,		XXX,                    XXX,                      				XXX,               			XXX,									XXX,					XXX,
				___,           			LGUI(LCTRL(Key_LeftArrow)),				XXX,						LGUI(LCTRL(Key_RightArrow)),			XXX,					XXX,
	 ___,		___,					XXX,                 					XXX,						XXX,                					XXX,       				XXX,
	 ___,		___,					LSHIFT(Key_Minus),        				___,
	 XXX),

	[PRIMARY] = KEYMAP_STACKED
	(Key_Esc,				Key_1,            		Key_2,          		Key_3,          			Key_4,          		Key_5,          Key_LeftGui,
	 Key_Backtick,   		Key_Q,            		Key_W,          		Key_E,          			Key_R,          		Key_T,          Key_Tab,
	 Key_CapsLock,   		Key_A,            		Key_S,          		Key_D,          			Key_F,          		Key_G,
	 Key_LeftAlt,    		Key_Z,            		Key_X,          		Key_C,          			Key_V,          		Key_B,          Key_Escape,
	 Key_LeftControl,		Key_Backspace,    		Key_Backspace,  		Key_LeftShift,
	 ShiftToLayer(SYMBOL),

	 Key_RightGui,    		Key_6,            		Key_7,          		Key_8,          			Key_9,          		Key_0,         LockLayer(NUMPAD),
	 Key_Enter,       		Key_Y,            		Key_U,          		Key_I,          			Key_O,          		Key_P,         Key_Equals,
							Key_H,            		Key_J,          		Key_K,         	 			Key_L,          		Key_Semicolon, Key_Quote,
	 Key_RightAlt,    		Key_N,            		Key_M,          		Key_Comma,      			Key_Period,     		Key_Slash,     Key_Minus,
	 Key_RightShift,  		Key_Spacebar,     		Key_Spacebar,   		Key_RightControl,
	 ShiftToLayer(NAVIGATION)),


	[SYMBOL] =  KEYMAP_STACKED
	(___,      				Key_F1,                 Key_F2,                   Key_F3,                    Key_F4,                Key_F5,              Key_F6,
	 ___,      				LSHIFT(Key_4),          LSHIFT(Key_2),            Key_LeftParen,             Key_RightParen,        LSHIFT(Key_1),       ___,
	 ___,      				LSHIFT(Key_5),          LSHIFT(Key_3),            Key_LeftCurlyBracket,      Key_RightCurlyBracket, LSHIFT(Key_Equals),
	 ___,      				___,                    XXX,                      Key_LeftBracket,           Key_LeftBracket,       XXX,                 XXX,
	 ___,      				Key_Delete,             Key_Delete,               ___,
	 XXX,

	 Key_F7,				Key_F8,					Key_F9,                   Key_F10,                   Key_F11,               Key_F12,             ___,
	 LCTRL(Key_Enter),		Key_Equals,				Key_Minus,                LSHIFT(Key_Comma),         LSHIFT(Key_Period),    XXX,                 ___,
							Key_Minus,				LSHIFT(Key_8),            LSHIFT(Key_7),             Key_Pipe,              ___,                 ___,
	 ___,      				___,					Key_Backslash,            Key_Slash,                 ___,                   ___,                 ___,
	 ___,      				___,					LSHIFT(Key_Minus),        ___,
	 ShiftToLayer(SPECIAL)),

	[NAVIGATION] =  KEYMAP_STACKED
	(___,      				Key_F1,                 Key_F2,                   Key_F3,                    Key_F4,                Key_F5,             Key_F6,
	 ___,      				LSHIFT(Key_4),          LSHIFT(Key_2),            Key_LeftParen,             Key_RightParen,        LSHIFT(Key_1),      ___,
	 ___,      				LSHIFT(Key_5),          LSHIFT(Key_3),            Key_LeftCurlyBracket,      Key_RightCurlyBracket, LSHIFT(Key_Equals),
	 ___,      				___,                    XXX,                      Key_LeftBracket,           Key_RightBracket,      XXX,                XXX,
	 ___,      				Key_Delete,             Key_Delete,               ___,
	 ShiftToLayer(SPECIAL),

	 Key_F7,   				Key_F8,					Key_F9,                   Key_F10,                   Key_F11,				Key_F12,				___,
	 LCTRL(Key_Enter),		XXX,                    XXX,                      Key_UpArrow,               Key_LeftBracket,		Key_RightBracket,		___,
							___, 					Key_LeftArrow,            Key_DownArrow,             Key_RightArrow,		___,					___,
	 ___,					___,					Key_Home,                 XXX,                       Key_End,				XXX,       				___,
	 ___,					___,					LSHIFT(Key_Minus),        ___,
	 XXX),

	 [GAMING] = KEYMAP_STACKED
	(Key_Esc,				Key_1,            		Key_2,          		Key_3,          		Key_4,          			Key_5,          		Key_LeftGui,
	 XXX, 					Key_Backtick,   		Key_Q,            		Key_W,          		Key_E,          			Key_R,          		Key_T,      
	 XXX, 					Key_CapsLock,   		Key_A,            		Key_S,          		Key_D,          			Key_F,          		
	 XXX, 					Key_LeftAlt,    		Key_Z,            		Key_X,          		Key_C,          			Key_V,          		Key_B,
	 Key_LeftControl,		Key_Backspace,    		Key_Backspace,  		Key_LeftShift,
	 ShiftToLayer(SYMBOL),

	 Key_RightGui,    		Key_6,            		Key_7,          		Key_8,          			Key_9,          		Key_0,         LockLayer(PRIMARY),
	 Key_Enter,       		Key_Y,            		Key_U,          		Key_I,          			Key_O,          		Key_P,         Key_Equals,
							Key_H,            		Key_J,          		Key_K,         	 			Key_L,          		Key_Semicolon, Key_Quote,
	 Key_RightAlt,    		Key_N,            		Key_M,          		Key_Comma,      			Key_Period,     		Key_Slash,     Key_Minus,
	 Key_RightShift,  		Key_Spacebar,     		Key_Spacebar,   		Key_RightControl,
	 ShiftToLayer(NAVIGATION)),

	[NUMPAD] =  KEYMAP_STACKED
	(___, ___, ___, ___, ___, ___, ___,
	 ___, ___, ___, ___, ___, ___, ___,
	 ___, ___, ___, ___, ___, ___,
	 ___, ___, ___, ___, ___, ___, ___,
	 ___, ___, ___, ___,
	 ___,

	 ___,                    ___, Key_7, Key_8,      Key_9,              Key_KeypadSubtract, ___,
	 ___,                    ___, Key_4, Key_5,      Key_6,              Key_KeypadAdd,      ___,
													 ___, Key_1, Key_2,      Key_3,              Key_Equals,         ___,
	 ___,                    ___, Key_0, Key_Period, Key_KeypadMultiply, Key_KeypadDivide,   Key_Enter,
	 ___, ___, ___, ___,
	 ___)
) // KEYMAPS


// Colors names of the EGA palette, for convenient use in colormaps. Should
// match the palette definition below. Optional, one can just use the indexes
// directly, too.
enum {
	BLACK,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	ORANGE,
	LIGHT_GRAY,
	DARK_GRAY,
	BRIGHT_BLUE,
	BRIGHT_GREEN,
	BRIGHT_CYAN,
	BRIGHT_RED,
	BRIGHT_MAGENTA,
	YELLOW,
	WHITE
};

// Define an EGA palette. Conveniently, that's exactly 16 colors, just like the
// limit of LEDPaletteTheme.
PALETTE(
		CRGB(0x00, 0x00, 0x00),  // [0x0] black
		CRGB(0x00, 0x00, 0xaa),  // [0x1] blue
		CRGB(0x00, 0xaa, 0x00),  // [0x2] green
		CRGB(0x00, 0xaa, 0xaa),  // [0x3] cyan
		CRGB(0xaa, 0x00, 0x00),  // [0x4] red
		CRGB(0xaa, 0x00, 0xaa),  // [0x5] magenta
		CRGB(0xFF, 0xA5, 0x00),  // [0x6] orange
		CRGB(0xaa, 0xaa, 0xaa),  // [0x7] light gray
		CRGB(0x55, 0x55, 0x55),  // [0x8] dark gray
		CRGB(0x55, 0x55, 0xff),  // [0x9] bright blue
		CRGB(0x55, 0xff, 0x55),  // [0xa] bright green
		CRGB(0x55, 0xff, 0xff),  // [0xb] bright cyan
		CRGB(0xff, 0x55, 0x55),  // [0xc] bright red
		CRGB(0xff, 0x55, 0xff),  // [0xd] bright magenta
		CRGB(0xff, 0xff, 0x55),  // [0xe] yellow
		CRGB(0xff, 0xff, 0xff)   // [0xf] white
)

COLORMAPS(
    [SPECIAL] = COLORMAP_STACKED
		(BRIGHT_RED,		BLACK, 		BLACK, 		BLACK, 		BLACK, 		BLACK, 		CYAN,
		 WHITE, 	      BLACK,  	BLACK,  	BLACK,  	BLACK,  	GREEN,  	BLACK,
		 WHITE,   	    BLACK,  	BLACK,  	BLACK,  	BLACK,  	GREEN,
		 WHITE,   	    BLACK,  	BLACK,  	BLACK,  	BLACK,  	GREEN,  	BLACK,
		 WHITE, 	      WHITE, 		WHITE, 		WHITE,
		 CYAN,

		 CYAN,		BLACK,		BLACK, 		BLACK, 		BLACK, 		BLACK, 		BRIGHT_RED,
		 WHITE, 	BLACK,		BLACK, 		BLACK,  	BLACK,  	BLACK,  	BLACK,
					BLACK,  	GREEN,  	BLACK,  	GREEN,  	BLACK,   	BLACK,
		 WHITE,     BLACK,  	BLACK,  	BLACK,   	BLACK,   	BLACK,   	RED,
		 WHITE, 	WHITE, 		WHITE, 		WHITE,
		 CYAN),

		[PRIMARY] = COLORMAP_STACKED
		(WHITE,		WHITE, 		WHITE, 		WHITE, 		WHITE, 		WHITE, 		CYAN,
		 WHITE, 	WHITE,  	WHITE,  	WHITE,  	WHITE,  	WHITE,  	WHITE,
		 WHITE,  	WHITE,  	WHITE,  	WHITE,  	WHITE,  	WHITE,
		 WHITE, 	ORANGE,  	WHITE,  	WHITE,  	WHITE,  	WHITE,  	WHITE,
		 WHITE, 	WHITE, 		WHITE, 		WHITE,
		 CYAN,

		 CYAN,		WHITE,		WHITE, 		WHITE, 		WHITE, 		WHITE, 		MAGENTA,
		 WHITE, 	WHITE,		WHITE, 		WHITE,  	WHITE,  	WHITE,  	WHITE,
							WHITE,  	WHITE,  	WHITE,  	WHITE,  	WHITE,   	WHITE,
		 WHITE,   WHITE,  	WHITE,  	WHITE,   	WHITE,   	ORANGE,   WHITE,
		 WHITE, 	WHITE, 		WHITE, 		WHITE,
		 CYAN),


		[SYMBOL] = COLORMAP_STACKED
		(WHITE,		BLUE, 		BLUE, 		BLUE,		BLUE, 		BLUE, 		CYAN,
		 WHITE, 	BLUE,  		BLUE,  		BLUE,  		BLUE,  		BLUE,  		BLUE,
		 WHITE,   	BLUE,  		BLUE,  		BLUE,  		BLUE,  		BLUE,
		 WHITE,   	BLUE,  		BLUE,  		BLUE,  		BLUE,  		BLUE,  		BLUE,
		 WHITE, 	WHITE, 		WHITE, 		WHITE,
		 CYAN,

		 CYAN,		BLUE,		BLUE, 		BLUE, 		BLUE, 		BLUE, 		MAGENTA,
		 WHITE, 	BLUE,		BLUE, 		BLUE,  		BLUE,  		BLUE,  		BLUE,
					BLUE,  		BLUE,  		BLUE,  		BLUE,  		BLUE,   	BLUE,
		 WHITE,     BLUE,  		BLUE,  		BLUE,   	BLUE,   	BLUE,   	BLUE,
		 WHITE, 	WHITE, 		WHITE, 		WHITE,
		 YELLOW),

		[NAVIGATION] = COLORMAP_STACKED
		(WHITE,		BLUE, 		BLUE, 		BLUE, 		BLUE, 		BLUE, 		CYAN,
		 WHITE, 	BLUE,  		BLUE,  		BLUE,  		BLUE,  		BLUE,  		BLUE,
		 WHITE,   	BLUE,  		BLUE,  		BLUE,  		BLUE,  		BLUE,
		 WHITE,   	BLUE,  		BLUE,  		BLUE,  		BLUE,  		BLUE,  		BLUE,
		 WHITE, 	WHITE, 		WHITE, 		WHITE,
		 YELLOW,

		 CYAN,		BLUE,		BLUE, 		BLUE, 		BLUE, 		BLUE, 		MAGENTA,
		 WHITE, 	BLUE,		BLUE, 		RED,  		BLUE,  		BLUE,  		BLUE,
					BLUE,  		RED,  		RED,  		RED,  		BLUE,   	BLUE,
		 WHITE,     BLUE,  		RED,  		BLACK,   	RED,   		BLACK,   	WHITE,
		 WHITE, 	WHITE, 		WHITE, 		WHITE,
		 CYAN),

		 [GAMING] = COLORMAP_STACKED
		(WHITE,		WHITE, 		WHITE, 		WHITE, 		WHITE, 		WHITE, 		CYAN,
		 WHITE, 	WHITE,  	WHITE,  	WHITE,  	WHITE,  	WHITE,  	WHITE,
		 WHITE,  	WHITE,  	WHITE,  	WHITE,  	WHITE,  	WHITE,
		 WHITE, 	ORANGE,  	WHITE,  	WHITE,  	WHITE,  	WHITE,  	WHITE,
		 WHITE, 	WHITE, 		WHITE, 		WHITE,
		 CYAN,

		 CYAN,		WHITE,		WHITE, 		WHITE, 		WHITE, 		WHITE, 		MAGENTA,
		 WHITE, 	WHITE,		WHITE, 		WHITE,  	WHITE,  	WHITE,  	WHITE,
							WHITE,  	WHITE,  	WHITE,  	WHITE,  	WHITE,   	WHITE,
		 WHITE,   WHITE,  	WHITE,  	WHITE,   	WHITE,   	ORANGE,   WHITE,
		 WHITE, 	WHITE, 		WHITE, 		WHITE,
		 CYAN),

		[NUMPAD] =  COLORMAP_STACKED
		(WHITE, 	WHITE, 		WHITE, 		WHITE, 		WHITE, 		WHITE, 		CYAN,
		WHITE, 		WHITE, 		WHITE, 		WHITE, 		WHITE, 		WHITE, 		WHITE,
		WHITE, 		WHITE, 		WHITE, 		WHITE, 		WHITE, 		WHITE,
		WHITE, 		WHITE, 		WHITE, 		WHITE, 		WHITE, 		WHITE, 		WHITE,
		WHITE, 		WHITE, 		WHITE, 		WHITE,
		CYAN,

		CYAN,		WHITE, 		MAGENTA, 	MAGENTA,	MAGENTA,	MAGENTA,	MAGENTA,
		WHITE,		WHITE, 		MAGENTA, 	MAGENTA,	MAGENTA,	MAGENTA,	WHITE,
					WHITE, 		MAGENTA, 	MAGENTA,	MAGENTA, 	MAGENTA, 	WHITE,
		WHITE,		WHITE, 		MAGENTA, 	MAGENTA, 	MAGENTA, 	MAGENTA,   	MAGENTA,
		WHITE, 		WHITE, 		WHITE, 		WHITE,
		CYAN)
)

/* Re-enable astyle's indent enforcement */
// clang-format on

/** versionInfoMacro handles the 'firmware version info' macro
 *  When a key bound to the macro is pressed, this macro
 *  prints out the firmware build information as virtual keystrokes
 */

static void versionInfoMacro(uint8_t key_state) {
	if (keyToggledOn(key_state)) {
		Macros.type(PSTR("Keyboardio Model 100 - Firmware version "));
		Macros.type(PSTR(KALEIDOSCOPE_FIRMWARE_VERSION));
	}
}

/** anyKeyMacro is used to provide the functionality of the 'Any' key.
 *
 * When the 'any key' macro is toggled on, a random alphanumeric key is
 * selected. While the key is held, the function generates a synthetic
 * keypress event repeating that randomly selected key.
 *
 */

static void anyKeyMacro(KeyEvent &event) {
	if (keyToggledOn(event.state)) {
		event.key.setKeyCode(Key_A.getKeyCode() + (uint8_t)(millis() % 36));
		event.key.setFlags(0);
	}
}


/** macroAction dispatches keymap events that are tied to a macro
		to that macro. It takes two uint8_t parameters.

		The first is the macro being called (the entry in the 'enum' earlier in this file).
		The second is the state of the keyswitch. You can use the keyswitch state to figure out
		if the key has just been toggled on, is currently pressed or if it's just been released.

		The 'switch' statement should have a 'case' for each entry of the macro enum.
		Each 'case' statement should call out to a function to handle the macro in question.

 */

const macro_t *macroAction(uint8_t macro_id, KeyEvent &event) {
	switch (macro_id) {

	case MACRO_VERSION_INFO:
		versionInfoMacro(event.state);
		break;

	case MACRO_ANY:
		anyKeyMacro(event);
		break;
	}
	return MACRO_NONE;
}

/** toggleLedsOnSuspendResume toggles the LEDs off when the host goes to sleep,
 * and turns them back on when it wakes up.
 */
void toggleLedsOnSuspendResume(kaleidoscope::plugin::HostPowerManagement::Event event) {
	switch (event) {
	case kaleidoscope::plugin::HostPowerManagement::Suspend:
	case kaleidoscope::plugin::HostPowerManagement::Sleep:
		LEDControl.disable();
		break;
	case kaleidoscope::plugin::HostPowerManagement::Resume:
		LEDControl.enable();
		break;
	}
}

/** hostPowerManagementEventHandler dispatches power management events (suspend,
 * resume, and sleep) to other functions that perform action based on these
 * events.
 */
void hostPowerManagementEventHandler(kaleidoscope::plugin::HostPowerManagement::Event event) {
	toggleLedsOnSuspendResume(event);
}

/** This 'enum' is a list of all the magic combos used by the Model 100's
 * firmware The names aren't particularly important. What is important is that
 * each is unique.
 *
 * These are the names of your magic combos. They will be used by the
 * `USE_MAGIC_COMBOS` call below.
 */
enum {
	// Toggle between Boot (6-key rollover; for BIOSes and early boot) and NKRO
	// mode.
	COMBO_TOGGLE_NKRO_MODE,
	// Enter test mode
	COMBO_ENTER_TEST_MODE
};

/** Wrappers, to be used by MagicCombo. **/

/**
 * This simply toggles the keyboard protocol via USBQuirks, and wraps it within
 * a function with an unused argument, to match what MagicCombo expects.
 */
static void toggleKeyboardProtocol(uint8_t combo_index) {
	USBQuirks.toggleKeyboardProtocol();
}

/**
 * Toggles between using the built-in keymap, and the EEPROM-stored one.
 */
static void toggleKeymapSource(uint8_t combo_index) {
	if (Layer.getKey == Layer.getKeyFromPROGMEM) {
		Layer.getKey = EEPROMKeymap.getKey;
	} else {
		Layer.getKey = Layer.getKeyFromPROGMEM;
	}
}

/**
 *  This enters the hardware test mode
 */
static void enterHardwareTestMode(uint8_t combo_index) {
	HardwareTestMode.runTests();
}


/** Magic combo list, a list of key combo and action pairs the firmware should
 * recognise.
 */
USE_MAGIC_COMBOS({.action = toggleKeyboardProtocol,
									// Left Fn + Esc + Shift
									.keys = {R3C6, R2C6, R3C7}},
								 {.action = enterHardwareTestMode,
									// Left Fn + Prog + LED
									.keys = {R3C6, R0C0, R0C6}},
								 {.action = toggleKeymapSource,
									// Left Fn + Prog + Shift
									.keys = {R3C6, R0C0, R3C7}});

// First, tell Kaleidoscope which plugins you want to use.
// The order can be important. For example, LED effects are
// added in the order they're listed here.
KALEIDOSCOPE_INIT_PLUGINS(
	// ----------------------------------------------------------------------
	// Chrysalis plugins

	// The EEPROMSettings & EEPROMKeymap plugins make it possible to have an
	// editable keymap in EEPROM.
	EEPROMSettings,
	EEPROMKeymap,

	// Focus allows bi-directional communication with the host, and is the
	// interface through which the keymap in EEPROM can be edited.
	Focus,

	// FocusSettingsCommand adds a few Focus commands, intended to aid in
	// changing some settings of the keyboard, such as the default layer (via the
	// `settings.defaultLayer` command)
	FocusSettingsCommand,

	// FocusEEPROMCommand adds a set of Focus commands, which are very helpful in
	// both debugging, and in backing up one's EEPROM contents.
	FocusEEPROMCommand,

	// The FirmwareVersion plugin lets Chrysalis query the version of the firmware
	// programmatically.
	FirmwareVersion,

	// The LayerNames plugin allows Chrysalis to display - and edit - custom layer
	// names, to be shown instead of the default indexes.
	LayerNames,

	// Enables setting, saving (via Chrysalis), and restoring (on boot) the
	
	// default LED mode.
	DefaultLEDModeConfig,

	// Enables controlling (and saving) the brightness of the LEDs via Focus.
	LEDBrightnessConfig,

	// ----------------------------------------------------------------------
	// Keystroke-handling plugins

	// The Qukeys plugin enables the "Secondary action" functionality in
	// Chrysalis. Keys with secondary actions will have their primary action
	// performed when tapped, but the secondary action when held.
	Qukeys,

	// The macros plugin adds support for macros
	Macros,

	// Enables dynamic, Chrysalis-editable macros.
	DynamicMacros,

	// The MagicCombo plugin lets you use key combinations to trigger custom
	// actions - a bit like Macros, but triggered by pressing multiple keys at the
	// same time.
	MagicCombo,

	// ----------------------------------------------------------------------
	// LED mode plugins

	// The boot greeting effect pulses the LED button for 10 seconds after the
	// keyboard is first connected
	BootGreetingEffect,

	// LEDControl provides support for other LED modes
	LEDControl,

	// We start with the LED effect that turns off all the LEDs.
	LEDOff,

	// The LED Palette Theme plugin provides a shared palette for other plugins,
	// like Colormap below
	LEDPaletteTheme,

	// The Colormap effect makes it possible to set up per-layer colormaps
	ColormapEffect,
	DefaultColormap,

	// The HostPowerManagement plugin allows us to turn LEDs off when then host
	// goes to sleep, and resume them when it wakes up.
	HostPowerManagement,

	// Turns LEDs off after a configurable amount of idle time.
	IdleLEDs,
	PersistentIdleLEDs,

	// ----------------------------------------------------------------------
	// Miscellaneous plugins

	// The USBQuirks plugin lets you do some things with USB that we aren't
	// comfortable - or able - to do automatically, but can be useful
	// nevertheless. Such as toggling the key report protocol between Boot (used
	// by BIOSes) and Report (NKRO).
	USBQuirks,

	// The hardware test mode, which can be invoked by tapping Prog, LED and the
	// left Fn button at the same time.
	HardwareTestMode  //,
);

/** The 'setup' function is one of the two standard Arduino sketch functions.
 * It's called when your keyboard first powers up. This is where you set up
 * Kaleidoscope and any plugins.
 */
void setup() {
	// First, call Kaleidoscope's internal setup function
	Kaleidoscope.setup();

	QUKEYS(
		kaleidoscope::plugin::Qukey(0, KeyAddr(3, 1), Key_LeftControl),    // A/Ctrl
		kaleidoscope::plugin::Qukey(0, KeyAddr(3, 14), Key_RightControl),  // Question Mark/Ctrl
	)
	Qukeys.setHoldTimeout(1000);
	Qukeys.setOverlapThreshold(50);
	Qukeys.setMinimumHoldTime(150);
	Qukeys.setMinimumPriorInterval(80);
	Qukeys.setMaxIntervalForTapRepeat(50);

	// Set the hue of the boot greeting effect to something that will result in a
	// nice green color.
	BootGreetingEffect.hue = 85;

	// Set the action key the test mode should listen for to Left Fn
	HardwareTestMode.setActionKey(R3C6);

	// To make the keymap editable without flashing new firmware, we store
	// additional layers in EEPROM. For now, we reserve space for eight layers. If
	// one wants to use these layers, just set the default layer to one in EEPROM,
	// by using the `settings.defaultLayer` Focus command, or by using the
	// `keymap.onlyCustom` command to use EEPROM layers only.
	EEPROMKeymap.setup(8);

	// We need to tell the Colormap plugin how many layers we want to have custom
	// maps for. To make things simple, we set it to eight layers, which is how
	// many editable layers we have (see above).
	ColormapEffect.max_layers(8);
	ColormapEffect.activate();
	DefaultColormap.install();

	// For Dynamic Macros, we need to reserve storage space for the editable
	// macros. A kilobyte is a reasonable default.
	DynamicMacros.reserve_storage(1024);

	// // If there's a default layer set in EEPROM, we should set that as the default
	// // here.
	// Layer.move(EEPROMSettings.default_layer());
	Layer.activate(1);

	// Editable layer names are stored in EEPROM too, and we reserve 16 bytes per
	// layer for them. We need one extra byte per layer for bookkeeping, so we
	// reserve 17 / layer in total.
	LayerNames.reserve_storage(17 * 8);

	// Unless configured otherwise with Chrysalis, we want to make sure that the
	// firmware starts with LED effects off. This avoids over-taxing devices that
	// don't have a lot of power to share with USB devices
	// DefaultLEDModeConfig.activateLEDModeIfUnconfigured(&LEDOff);


}

/** loop is the second of the standard Arduino sketch functions.
	* As you might expect, it runs in a loop, never exiting.
	*
	* For Kaleidoscope-based keyboard firmware, you usually just want to
	* call Kaleidoscope.loop(); and not do anything custom here.
	*/

void loop() {
	Kaleidoscope.loop();
}
