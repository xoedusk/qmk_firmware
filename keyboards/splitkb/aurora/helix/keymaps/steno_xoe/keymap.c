// Note to self: Compile with
// $ qmk compile -kb splitkb/aurora/helix -km steno_xoe
//
// Move with
// $ rsync -r /Users/brian/Repos/qmk_firmware/splitkb_aurora_helix_rev1_steno_xoe_liatris.uf2 /Volumes/RPI-RP2



#include QMK_KEYBOARD_H
//#include "keymap_steno.h"
#include "transactions.h"
#include <string.h>

enum my_helix_layers {
	_STENO,
	_QWERTY,
	_SILENT,
	_NUMBER,
	_SYMBOL
};

#define QWERTY DF(_QWERTY)
#define STENO DF(_STENO)
#define SILENT DF(_SILENT)
#define NUMBER MO(_NUMBER)
#define SYMBOL MO(_SYMBOL)
#define MAX_STROKE_LIST_SIZE 4
#define MAX_STROKE_LENGTH 30

//char newStroke[MAX_STROKE_LENGTH] = "STOEUN";

char strokeList[MAX_STROKE_LIST_SIZE][MAX_STROKE_LENGTH] = {"\n\nby", "XoXo", " ", "2023"};
uint8_t nextStrokeListIndex = 0;






//This combo is used to prevent me from typing in steno KPA because that sends a Command+Q stroke if I accidentally am in QWERTY.
enum combo_events {
	ACCIDENTAL_KPA_CHORD_EVENT,
};

const uint16_t PROGMEM kpa_qwerty_chord[] = {KC_LGUI, KC_D, KC_X, COMBO_END};

combo_t key_combos[] = {
	[ACCIDENTAL_KPA_CHORD_EVENT] = COMBO(kpa_qwerty_chord, KC_NO),
};


void add_stroke_to_oled(char* newString) {
	strncpy(strokeList[nextStrokeListIndex], newString, MAX_STROKE_LENGTH);
	dprintf("Added %s to position %i.\n", newString, nextStrokeListIndex);
	nextStrokeListIndex++;
	if (nextStrokeListIndex==MAX_STROKE_LIST_SIZE) nextStrokeListIndex=0;
}


void process_combo_event(uint16_t combo_index, bool pressed) {
	switch (combo_index) {
		case ACCIDENTAL_KPA_CHORD_EVENT:
			if (pressed) {
				dprint("ACCIDENTAL_KPA_CHORD_EVENT detected.\n");

				/*
				1 Fn  #1  #2 #3 #4 #5   #6
				0 S1- S2- T- K- P- W-   H-
				0 R-  A-  O- *1 *2 res1 res2
				0 pwr *3  *4 -E -U -F   -R
				0 -P  -B  -L -G -T -S   -D
				0 #7  #8  #9 #A #B #C   -Z
				*/

				//																		  KP        A
				uint8_t myBytes[6] = {0b10000000, 0b00001100, 0b00100000, 0b00000000, 0b00000000, 0b00000000};

				default_layer_set(1<<_STENO);
				send_live_steno_chord_gemini(myBytes);

				// Now update the OLED with the new stroke. This doesn't happen automatically because
				// the user didn't do a steno stroke. 
					add_stroke_to_oled("KPA");
					// strncpy(strokeList[nextStrokeListIndex], "KPA", MAX_STROKE_LENGTH);
					// dprintf("Added KPA to position %i. This exists within process_combo_event().\n", nextStrokeListIndex);
					// nextStrokeListIndex++;
					// if (nextStrokeListIndex==MAX_STROKE_LIST_SIZE) nextStrokeListIndex=0;

			}
			break;
	}
}




// // function called upon receiving the string just copies the value onto slave's variable
// void user_string_sync(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer, uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
//     memcpy(newStroke, initiator2target_buffer, initiator2target_buffer_size);
// }

// void keyboard_post_init_user(void) {
//    transaction_register_rpc(RPC_ID_USER_STR, user_string_sync);
// }

// void housekeeping_task_user(void) {
// 	    // if slave tries and send data, we blow up :P
//     if (!is_keyboard_master()) {
//         return;
//     }
//     static uint32_t str_sync = 0;

//     // synch every 500ms (half a second)
//     if (timer_elapsed32(str_sync) < 500) {
//         // too soon, just quit
//         return;
//     }

//     // store current time for next checks
//     str_sync = timer_read32();
    
//     // send it
//     // `<your_string>` has to be defined on this same file, so ARRAY_SIZE can get its length
//     // otherwise you could use `strlen(<your_string>) + 1`
//     transaction_rpc_send(RPC_ID_USER_STR, ARRAY_SIZE(newStroke), newStroke);
// }

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[_QWERTY] = LAYOUT(
			KC_GRV,		KC_1,			KC_2,			KC_3,			KC_4,			KC_5,													KC_6,			KC_7,			KC_8,			KC_9,			KC_0,			KC_DEL,
      KC_TAB,		KC_Q,			KC_W,			KC_E,			KC_R,			KC_T,													KC_Y,			KC_U,			KC_I,			KC_O,			KC_P,			KC_BSPC,
      KC_CAPS,	KC_A,			KC_S,			KC_D,			KC_F,			KC_G,													KC_H,			KC_J,			KC_K,			KC_L,			KC_SCLN,	KC_QUOT,
      KC_LSFT,	KC_Z,			KC_X,			KC_C,			KC_V,			KC_B,		G(C(KC_X)),	SILENT,		KC_N,			KC_M,			KC_COMM,	KC_DOT,		KC_SLSH,	KC_ENT ,
      XXXXXXX,	XXXXXXX,	KC_LCTL,	KC_LALT,	KC_LGUI, 	KC_SPC,	NUMBER,			SYMBOL,		KC_SPC,		KC_RSFT,	KC_LEFT,	KC_UP,		KC_DOWN,	KC_RGHT
		),

	[_STENO] = LAYOUT(
		XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,												XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,
		XXXXXXX,	STN_N1,		STN_N2,		STN_N3,		STN_N4,		XXXXXXX,												XXXXXXX,	STN_N5,		STN_N6,		STN_N7,		STN_N8,		STN_N9,
		XXXXXXX,	STN_S1,		STN_TL,		STN_PL,		STN_HL,		STN_ST1,												STN_ST3,	STN_FR,		STN_PR,		STN_LR,		STN_TR,		STN_DR,
		KC_LSFT,	STN_S2,		STN_KL,		STN_WL,		STN_RL,		STN_ST2,	G(C(KC_X)),	SILENT,		STN_ST4,	STN_RR,		STN_BR,		STN_GR,		STN_SR,		STN_ZR,
		XXXXXXX,	XXXXXXX,	XXXXXXX,	KC_LALT,	STN_A,		STN_O,		NUMBER,			SYMBOL,		STN_E,		STN_U,		XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX
		),

	[_SILENT] = LAYOUT(
		XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,												XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,
		XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,												XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,
		XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,												XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,
		XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	G(C(KC_X)),	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,
		XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	NUMBER,			SYMBOL,		XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX
		),

	[_NUMBER] = LAYOUT(
		KC_ESC,		XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,												XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	KC_DEL,
		XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,												XXXXXXX,	KC_P7,		KC_P8,		KC_P9,		XXXXXXX,	KC_BSPC,
		XXXXXXX,	XXXXXXX,	XXXXXXX,	KC_UP,		XXXXXXX,	KC_PGUP,												XXXXXXX,	KC_P4,		KC_P5,		KC_P6,		XXXXXXX,	XXXXXXX,
		XXXXXXX,	XXXXXXX,	KC_LEFT,	KC_DOWN,	KC_RIGHT,	KC_PGDN,	XXXXXXX,		XXXXXXX,	XXXXXXX,	KC_P1,		KC_P2,		KC_P3,		XXXXXXX,	KC_ENT,
		XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	_______,		STENO,		XXXXXXX,	KC_P0,		XXXXXXX,	KC_PDOT,	XXXXXXX,	XXXXXXX
		),

	[_SYMBOL] = LAYOUT(
		KC_ESC,		RGB_HUI,	RGB_SAI,	RGB_VAI,	RGB_MOD,	XXXXXXX,												DB_TOGG,	XXXXXXX,	KC_UP,		XXXXXXX,	XXXXXXX,	KC_DEL,
		XXXXXXX,	RGB_HUD,	RGB_SAD,	RGB_VAD,	RGB_RMOD,	XXXXXXX,												XXXXXXX,	KC_LEFT,	KC_DOWN,	KC_RIGHT,	S(KC_LBRC),	KC_BSPC,
		XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,												XXXXXXX,	XXXXXXX,	KC_RBRC,	KC_MINUS,	KC_EQL,		KC_BSLS,
		KC_LSFT,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,		EE_CLR,		XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	KC_LBRC,	KC_ENT,
		XXXXXXX,	XXXXXXX,	KC_LCTL,	KC_LALT,	KC_LGUI,	KC_SPC,		QWERTY,			_______,	KC_SPC,		KC_RSFT,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX
		)
};

// #ifdef OLED_ENABLE
// 	// bool oled_task_user(void) {
// 	// 	// if(is_keyboard_master()){
// 	// 	// 	oled_write_P(PSTR("Layer\n-----\n\n"), false);
// 	// 	// }

// 	// 	// if (is_keyboard_master()) {
// 	// 	// 	oled_write_P(PSTR("Layer:\n"), false);

// 	// 	// 	switch(get_highest_layer(layer_state)) {
// 	// 	// 	case _QWERTY:
// 	// 	// 		oled_write_P(PSTR("QWERTY\n"), false);
// 	// 	// 		break;
// 	// 	// 	case _STENO:
// 	// 	// 		oled_write_P(PSTR("STENO\n"), false);
// 	// 	// 		break;
// 	// 	// 	case _SILENT:
// 	// 	// 		oled_write_P(PSTR("Silent\n"), false);
// 	// 	// 		break;
// 	// 	// 	default:
// 	// 	// 		oled_write_P(PSTR("Undefined\n"), false);
// 	// 	// 	}
// 	// 	// }

// 	// 	return false;
// 	// }

// layer_state_t default_layer_state_set_user(layer_state_t state) {
// 	if (is_keyboard_master()){
// 		//oled_write_P(PSTR("Layer\n-----\n\n"), false);
// 		switch(get_highest_layer(state)){
// 			case _QWERTY:
// 				oled_write_P(PSTR("qwer\n"), false);
// 				break;
// 			case _STENO:
// 				oled_write_P(PSTR("sten\n"), false);
// 				break;
// 			case _SILENT:
// 				oled_write_P(PSTR("off\n"), false);
// 				break;
// 			case _NUMBER:
// 				oled_write_P(PSTR("numb\n"), false);
// 				break;
// 			case _SYMBOL:
// 				oled_write_P(PSTR("symb\n"), false);
// 				break;
// 			default:
// 				oled_write_P(PSTR("???\n"), false);
// 		}
// 	}
// 	return state;
// }

// 	layer_state_t layer_state_set_user(layer_state_t state) {
// 	if (is_keyboard_master()){
// 		//oled_write_P(PSTR("Layer\n-----\n\n"), false);
// 		switch(get_highest_layer(state)){
// 			case _QWERTY:
// 				oled_write_P(PSTR("qwer\n"), false);
// 				break;
// 			case _STENO:
// 				oled_write_P(PSTR("sten\n"), false);
// 				break;
// 			case _SILENT:
// 				oled_write_P(PSTR("off\n"), false);
// 				break;
// 			case _NUMBER:
// 				oled_write_P(PSTR("numb\n"), false);
// 				break;
// 			case _SYMBOL:
// 				oled_write_P(PSTR("symb\n"), false);
// 				break;
// 			default:
// 				oled_write_P(PSTR("???\n"), false);
// 		}
// 	}

//     return state;	
// }
// #endif

void keyboard_pre_init_user(void) {
  // Set our LED pin as output
  setPinOutput(24);
  // Turn the LED off
  // (Due to technical reasons, high is off and low is on)
  writePinHigh(24);
}










bool my_layer_state_is(uint8_t layer) {
    return layer_state_cmp(layer_state | default_layer_state, layer);
} 

#ifdef OLED_ENABLE
// NOTE: Most of the OLED code was originally written by Soundmonster for the Corne,
// and has been copied directly from `crkbd/soundmonster/keymap.c`

oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

void render_mod_status_xoe(uint8_t modifiers) {
	static const char PROGMEM gui_on_xoe[] = {0xd1, 0};
	static const char PROGMEM alt_on_xoe[] = {0xd2, 0};
	static const char PROGMEM ctrl_on_xoe[] = {0xd3, 0};
	static const char PROGMEM shft_on_xoe[] = {0xd4, 0};

	static const char PROGMEM gui_off_xoe[] = {0x20, 0};
	static const char PROGMEM alt_off_xoe[] = {0x20, 0};
	static const char PROGMEM ctrl_off_xoe[] = {0x20, 0};
	static const char PROGMEM shft_off_xoe[] = {0x20, 0};

	oled_set_cursor(0,15);

	if (modifiers & MOD_MASK_GUI) {
		oled_write_P(gui_on_xoe, false);
	} else {
		oled_write_P(gui_off_xoe, false);
	}

	if (modifiers & MOD_MASK_ALT) {
		oled_write_P(alt_on_xoe, false);
	} else {
		oled_write_P(alt_off_xoe, false);
	}

	led_t led_state = host_keyboard_led_state();
   oled_write_P(led_state.caps_lock ? PSTR("C") : PSTR(" "), false);

	if (modifiers & MOD_MASK_CTRL) {
		oled_write_P(ctrl_on_xoe, false);
	} else {
		oled_write_P(ctrl_off_xoe, false);
	}

	if (modifiers & MOD_MASK_SHIFT) {
		oled_write_P(shft_on_xoe, false);
	} else {
		oled_write_P(shft_off_xoe, false);
	}

} // end render_mod_status_xoe



// void render_mod_status_gui_alt(uint8_t modifiers) {
//     static const char PROGMEM gui_off_1[] = {0x85, 0x86, 0};
//     static const char PROGMEM gui_off_2[] = {0xa5, 0xa6, 0};
//     static const char PROGMEM gui_on_1[] = {0x8d, 0x8e, 0};
//     static const char PROGMEM gui_on_2[] = {0xad, 0xae, 0};

//     static const char PROGMEM alt_off_1[] = {0x87, 0x88, 0};
//     static const char PROGMEM alt_off_2[] = {0xa7, 0xa8, 0};
//     static const char PROGMEM alt_on_1[] = {0x8f, 0x90, 0};
//     static const char PROGMEM alt_on_2[] = {0xaf, 0xb0, 0};

//     // fillers between the modifier icons bleed into the icon frames
//     static const char PROGMEM off_off_1[] = {0xc5, 0};
//     static const char PROGMEM off_off_2[] = {0xc6, 0};
//     static const char PROGMEM on_off_1[] = {0xc7, 0};
//     static const char PROGMEM on_off_2[] = {0xc8, 0};
//     static const char PROGMEM off_on_1[] = {0xc9, 0};
//     static const char PROGMEM off_on_2[] = {0xca, 0};
//     static const char PROGMEM on_on_1[] = {0xcb, 0};
//     static const char PROGMEM on_on_2[] = {0xcc, 0};

//     if(modifiers & MOD_MASK_GUI) {
//         oled_write_P(gui_on_1, false);
//     } else {
//         oled_write_P(gui_off_1, false);
//     }

//     if ((modifiers & MOD_MASK_GUI) && (modifiers & MOD_MASK_ALT)) {
//         oled_write_P(on_on_1, false);
//     } else if(modifiers & MOD_MASK_GUI) {
//         oled_write_P(on_off_1, false);
//     } else if(modifiers & MOD_MASK_ALT) {
//         oled_write_P(off_on_1, false);
//     } else {
//         oled_write_P(off_off_1, false);
//     }

//     if(modifiers & MOD_MASK_ALT) {
//         oled_write_P(alt_on_1, false);
//     } else {
//         oled_write_P(alt_off_1, false);
//     }

//     if(modifiers & MOD_MASK_GUI) {
//         oled_write_P(gui_on_2, false);
//     } else {
//         oled_write_P(gui_off_2, false);
//     }

//     if (modifiers & MOD_MASK_GUI & MOD_MASK_ALT) {
//         oled_write_P(on_on_2, false);
//     } else if(modifiers & MOD_MASK_GUI) {
//         oled_write_P(on_off_2, false);
//     } else if(modifiers & MOD_MASK_ALT) {
//         oled_write_P(off_on_2, false);
//     } else {
//         oled_write_P(off_off_2, false);
//     }

//     if(modifiers & MOD_MASK_ALT) {
//         oled_write_P(alt_on_2, false);
//     } else {
//         oled_write_P(alt_off_2, false);
//     }
// }

// void render_mod_status_ctrl_shift(uint8_t modifiers) {
//     static const char PROGMEM ctrl_off_1[] = {0x89, 0x8a, 0};
//     static const char PROGMEM ctrl_off_2[] = {0xa9, 0xaa, 0};
//     static const char PROGMEM ctrl_on_1[] = {0x91, 0x92, 0};
//     static const char PROGMEM ctrl_on_2[] = {0xb1, 0xb2, 0};

//     static const char PROGMEM shift_off_1[] = {0x8b, 0x8c, 0};
//     static const char PROGMEM shift_off_2[] = {0xab, 0xac, 0};
//     static const char PROGMEM shift_on_1[] = {0xcd, 0xce, 0};
//     static const char PROGMEM shift_on_2[] = {0xcf, 0xd0, 0};

//     // fillers between the modifier icons bleed into the icon frames
//     static const char PROGMEM off_off_1[] = {0xc5, 0};
//     static const char PROGMEM off_off_2[] = {0xc6, 0};
//     static const char PROGMEM on_off_1[] = {0xc7, 0};
//     static const char PROGMEM on_off_2[] = {0xc8, 0};
//     static const char PROGMEM off_on_1[] = {0xc9, 0};
//     static const char PROGMEM off_on_2[] = {0xca, 0};
//     static const char PROGMEM on_on_1[] = {0xcb, 0};
//     static const char PROGMEM on_on_2[] = {0xcc, 0};

//     if(modifiers & MOD_MASK_CTRL) {
//         oled_write_P(ctrl_on_1, false);
//     } else {
//         oled_write_P(ctrl_off_1, false);
//     }

//     if ((modifiers & MOD_MASK_CTRL) && (modifiers & MOD_MASK_SHIFT)) {
//         oled_write_P(on_on_1, false);
//     } else if(modifiers & MOD_MASK_CTRL) {
//         oled_write_P(on_off_1, false);
//     } else if(modifiers & MOD_MASK_SHIFT) {
//         oled_write_P(off_on_1, false);
//     } else {
//         oled_write_P(off_off_1, false);
//     }

//     if(modifiers & MOD_MASK_SHIFT) {
//         oled_write_P(shift_on_1, false);
//     } else {
//         oled_write_P(shift_off_1, false);
//     }

//     if(modifiers & MOD_MASK_CTRL) {
//         oled_write_P(ctrl_on_2, false);
//     } else {
//         oled_write_P(ctrl_off_2, false);
//     }

//     if (modifiers & MOD_MASK_CTRL & MOD_MASK_SHIFT) {
//         oled_write_P(on_on_2, false);
//     } else if(modifiers & MOD_MASK_CTRL) {
//         oled_write_P(on_off_2, false);
//     } else if(modifiers & MOD_MASK_SHIFT) {
//         oled_write_P(off_on_2, false);
//     } else {
//         oled_write_P(off_off_2, false);
//     }

//     if(modifiers & MOD_MASK_SHIFT) {
//         oled_write_P(shift_on_2, false);
//     } else {
//         oled_write_P(shift_off_2, false);
//     }
// }

void render_logo(void) {
    static const char PROGMEM aurora_logo[] = {
        0x00, 0x00, 0x00, 0xe0, 0x00, 0xf8, 0xc0, 0xf8, 0xe0, 0xc0, 0xfc, 0x00, 0x7e, 0x18, 0x00, 0x80, 
        0x00, 0x02, 0x80, 0xf0, 0x00, 0xc0, 0x80, 0xf8, 0xc0, 0xe0, 0x70, 0x60, 0x3c, 0x38, 0x3c, 0x1c, 
        0x00, 0x3f, 0x0c, 0x0f, 0x1f, 0x03, 0x07, 0x01, 0xc3, 0x00, 0xe0, 0x80, 0x00, 0xe0, 0x80, 0xf8, 
        0x80, 0xc0, 0xf7, 0xc7, 0x6f, 0x7b, 0x39, 0x30, 0x00, 0x80, 0x00, 0xc0, 0x00, 0xc0, 0xc2, 0xe0, 
        0x00, 0x40, 0x38, 0x30, 0x38, 0x1e, 0x18, 0x1e, 0x0f, 0x0c, 0x07, 0x07, 0x07, 0x03, 0x03, 0x21, 
        0x21, 0x31, 0x30, 0x18, 0x18, 0x1c, 0x08, 0x0c, 0x0e, 0x07, 0x06, 0x07, 0x03, 0xc3, 0x03, 0x01, 
        0x4c, 0xcc, 0xc2, 0xc2, 0x41, 0x49, 0x09, 0x2b, 0x2a, 0x6a, 0x6e, 0x24, 0x24, 0x04, 0x92, 0x92, 
        0xb1, 0xf1, 0xf1, 0xf2, 0xe6, 0xa4, 0xa4, 0x04, 0x04, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28
    };
    oled_write_raw_P(aurora_logo, sizeof(aurora_logo));
    oled_set_cursor(0, 4);
}

void render_logo_text(void) {
    oled_write_P(PSTR("helix"), false);
}

void render_kb_LED_state(void) {
    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("N ") : PSTR("  "), false);
    oled_write_P(led_state.caps_lock ? PSTR("C ") : PSTR("  "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("S ") : PSTR("  "), false);
}

void render_layer_state(void) {
    // static const char PROGMEM default_layer[] = {
    //     0x20, 0x94, 0x95, 0x96, 0x20,
    //     0x20, 0xb4, 0xb5, 0xb6, 0x20,
    //     0x20, 0xd4, 0xd5, 0xd6, 0x20, 0};
    // static const char PROGMEM raise_layer[] = {
    //     0x20, 0x97, 0x98, 0x99, 0x20,
    //     0x20, 0xb7, 0xb8, 0xb9, 0x20,
    //     0x20, 0xd7, 0xd8, 0xd9, 0x20, 0};
    // static const char PROGMEM lower_layer[] = {
    //     0x20, 0x9a, 0x9b, 0x9c, 0x20,
    //     0x20, 0xba, 0xbb, 0xbc, 0x20,
    //     0x20, 0xda, 0xdb, 0xdc, 0x20, 0};
    // static const char PROGMEM adjust_layer[] = {
    //     0x20, 0x9d, 0x9e, 0x9f, 0x20,
    //     0x20, 0xbd, 0xbe, 0xbf, 0x20,
    //     0x20, 0xdd, 0xde, 0xdf, 0x20, 0};
    static const char PROGMEM silent_layer[] = {
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 
				0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
				0xc0, 0x30, 0x0c, 0xc2, 0xf1, 0xf9, 0xfc, 0xfc, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfc, 0xfc, 
				0xf8, 0xf0, 0xc0, 0x00, 0x00, 0x00, 0x9c, 0xa2, 0xa2, 0x9c, 0x01, 0x01, 0x02, 0x0c, 0x30, 0xc0, 
				0x0f, 0x30, 0xc0, 0x0f, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
				0x7f, 0x3f, 0x0f, 0x00, 0x00, 0x00, 0xef, 0xa2, 0xa2, 0x20, 0x00, 0x00, 0x00, 0xc0, 0x30, 0x0f,
				0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x04, 0x04, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x08, 0x08, 
				0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x0b, 0x08, 0x04, 0x04, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00};
    static const char PROGMEM symb_layer[] = {
        0x00, 0x00, 0x00, 0xc0, 0xe0, 0x30, 0x30, 0x30, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdc, 0xde, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0xf8, 0xf8, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xf8, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x03, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00};
    static const char PROGMEM num_layer[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x78, 0x18, 0x98, 0x98, 0xf8, 0x70, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x78, 0x18, 0x18, 
        0x18, 0xb8, 0xf8, 0xe0, 0x00, 0x00, 0x00, 0x1c, 0x38, 0x60, 0x61, 0x61, 0x3f, 0x3e, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x08, 0x0c, 0xfe, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x3c, 0x3e, 0x26, 
        0x23, 0x23, 0x21, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    static const char PROGMEM steno_layer[] = {
        0x00, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0x00, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0x00, 0x00, 
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0x00, 
				0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf9, 0xfb, 0xfb, 0xfb, 0xfb, 0xf9, 0x00, 
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xc0, 
				0xc0, 0xc0, 0xc0, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x07, 0x0f, 0x0f, 0x0f, 0x0f, 0x07, 0x00, 
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x1f, 0x1f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x0f, 
				0x1f, 0x1f, 0x0f, 0x00, 0x0f, 0x1f, 0x1f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    static const char PROGMEM qwerty_layer[] = {
        0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0xfe, 0xff, 0x83, 0x01, 0x00, 0x00, 0x80, 0x81, 0x83, 0xff, 0xff, 0x00, 0x00, 0x80, 
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x07, 0x06, 0x07, 0x03, 0x03, 0x07, 0x06, 0x00, 0x00, 0x03, 
        0x3f, 0xff, 0xe0, 0xc0, 0xf8, 0x7f, 0x07, 0x3f, 0xfc, 0xc0, 0xe0, 0xfe, 0x3f, 0x07, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x03, 0x07, 0x07, 0x03, 0x00, 0x00, 0x00, 0x03, 0x07, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00};
    if(my_layer_state_is(_SYMBOL)) {
        oled_write_raw_P(symb_layer, sizeof(symb_layer));
    } else if(my_layer_state_is(_NUMBER)) {
       oled_write_raw_P(num_layer, sizeof(num_layer));
    } else if (my_layer_state_is(_SILENT)) {
        oled_write_raw_P(silent_layer, sizeof(silent_layer));
    } else if(my_layer_state_is(_STENO)) {
        oled_write_raw_P(steno_layer, sizeof(steno_layer));
    } else if (my_layer_state_is(_QWERTY)){
         oled_write_raw_P(qwerty_layer, sizeof(qwerty_layer));
    }

    oled_set_cursor(0,10);
}


bool oled_task_kb(void) {
    if (!oled_task_user()) {
        return false;
    }
    if (!is_keyboard_master()) {
        // Renders the current keyboard state (layers and mods)
        render_logo();
        render_logo_text();
        oled_advance_page(false);
        render_layer_state();
        oled_advance_page(false);
        //render_mod_status_gui_alt(get_mods()|get_oneshot_mods());
        //render_mod_status_ctrl_shift(get_mods()|get_oneshot_mods());
        render_kb_LED_state();
    } else {
        // clang-format off
        // static const char PROGMEM aurora_art[] = {
        //     0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1c, 0x08, 0x00, 0x00, 
        //     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 
        //     0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x40, 
        //     0xe0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x80, 
        //     0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x80, 0x00, 0xf0, 0x00, 0x00, 0xc0, 
        //     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 
        //     0x81, 0x00, 0xc0, 0x00, 0xfe, 0x00, 0xfc, 0x00, 0xff, 0x20, 0xff, 0xf0, 0x0f, 0xf0, 0x00, 0xff, 
        //     0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0xf8, 0x00, 0x00, 0xf8, 
        //     0xff, 0x10, 0xff, 0x84, 0xff, 0x60, 0xff, 0x36, 0xff, 0x0f, 0xff, 0x3f, 0x00, 0x5f, 0x00, 0x05, 
        //     0x80, 0x00, 0x80, 0x00, 0xc0, 0x38, 0x00, 0xec, 0xf0, 0x00, 0xfb, 0x80, 0xff, 0xf0, 0xff, 0xef, 
        //     0xff, 0xe8, 0xff, 0x03, 0xff, 0x0c, 0xff, 0x00, 0xff, 0x00, 0x03, 0x00, 0x00, 0xf8, 0x00, 0x80, 
        //     0xff, 0x20, 0xff, 0xd0, 0xff, 0xe0, 0xfe, 0xf8, 0xff, 0xfc, 0xff, 0xff, 0x0f, 0xff, 0x01, 0x3f, 
        //     0xff, 0x00, 0x0f, 0x00, 0x01, 0x00, 0x03, 0x00, 0xfe, 0x80, 0xfe, 0x00, 0xc0, 0xff, 0xc4, 0xfb, 
        //     0xff, 0xfe, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xff, 0x07, 0xff, 0x03, 0x3f, 0x00, 0x0f, 0xc0, 0x00, 
        //     0x00, 0x00, 0xb8, 0x00, 0xff, 0x40, 0xbe, 0xf0, 0xff, 0xf1, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 
        //     0x1f, 0xff, 0x67, 0x00, 0xef, 0x00, 0x1f, 0x00, 0x00, 0x07, 0x00, 0x00, 0xe0, 0x00, 0xff, 0xf0, 
        //     0xff, 0x88, 0xff, 0xc4, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0x7f, 0x0f, 0xff, 
        //     0x00, 0x07, 0xfe, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0xc0, 0x3f, 0xf8, 0xe7, 0xff, 
        //     0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0x1f, 0x3f, 0x01, 0xff, 0x0b, 0x00, 0xff, 0x00, 0x00, 0x05, 
        //     0x00, 0x00, 0x00, 0xe0, 0x00, 0xf8, 0x60, 0x80, 0xfe, 0xe3, 0xfc, 0xff, 0x1e, 0xff, 0xff, 0x23, 
        //     0xff, 0x09, 0xff, 0x20, 0x00, 0x3f, 0x02, 0x00, 0x00, 0x0f, 0x00, 0x40, 0x00, 0xc0, 0x00, 0xfc, 
        //     0xe0, 0xfc, 0xf0, 0xff, 0xff, 0x7f, 0xfc, 0xff, 0x0f, 0xff, 0x07, 0x1f, 0x00, 0x01, 0x0f, 0x00, 
        //     0x0f, 0x00, 0x81, 0x70, 0x0c, 0xf0, 0x80, 0x00, 0x00, 0xe4, 0xf8, 0xe6, 0x70, 0x3f, 0xcf, 0xff, 
        //     0x1f, 0xff, 0x48, 0xff, 0x0f, 0x00, 0x07, 0x00, 0x00, 0x43, 0x60, 0xf8, 0xf0, 0xfe, 0x38, 0xfe, 
        //     0x00, 0xfc, 0x03, 0x00, 0xc8, 0x72, 0xcf, 0xfc, 0x00, 0x03, 0x0f, 0x01, 0xe0, 0x1c, 0xe0, 0x03, 
        //     0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x06, 0xf9, 0x00, 0x03, 0x00, 0x07, 
        //     0xff, 0x00, 0x10, 0x12, 0xc9, 0xf0, 0xcf, 0xb4, 0x7f, 0x80, 0xe0, 0x1e, 0x01, 0x40, 0x65, 0x5e, 
        //     0xe0, 0x00, 0x00, 0xf0, 0x0c, 0xf0, 0x00, 0x80, 0x7e, 0x01, 0x80, 0x93, 0xfc, 0xc0, 0x00, 0x00, 
        //     0x00, 0x00, 0x00, 0x00, 0x89, 0x18, 0x2c, 0x46, 0x00, 0x07, 0x21, 0x10, 0x10, 0x80, 0x09, 0x13, 
        //     0x31, 0xbf, 0xff, 0x00, 0x08, 0x1a, 0xf7, 0x0f, 0x00, 0x00, 0x44, 0x45, 0x34, 0xbf, 0xb8, 0x00, 
        //     0x10, 0xf0, 0x08, 0xf4, 0x18, 0x11, 0xfc, 0x18, 0xfb, 0x0e, 0x10, 0xf8, 0x04, 0xf8, 0x10, 0x20, 
        //     0x18, 0x09, 0xff, 0x0c, 0xea, 0x1f, 0x28, 0x60, 0x30, 0xf8, 0x20, 0xc0, 0x42, 0x33, 0x21, 0x00
        // };
        // clang-format on
        //oled_write_raw_P(aurora_art, sizeof(aurora_art));
    	oled_clear();
    	oled_set_cursor(0,0);
    	//oled_write_ln((newStroke), false);

    	// If the variable nextStrokeListIndex is 2, the most recent array that was filled
    	// was 1. To make 1 last in the drawing, we should start at 2. Thus, we start at
    	// nextStrokeListIndex, wrapping around to 0 using mod as necessary.
    	for(int i = nextStrokeListIndex;i<nextStrokeListIndex + MAX_STROKE_LIST_SIZE;i++) {
    		int j = i - nextStrokeListIndex; //need one that starts from 0 too.

    		oled_set_cursor(0,j*4); // 4 lines in between chords.
    		oled_write_ln((strokeList[i%MAX_STROKE_LIST_SIZE]), false);
    		//oled_write_ln(" ", false); // Probably a better way to skip a line.
    	}

    	render_mod_status_xoe(get_mods()|get_oneshot_mods());
    }
    return false;
}
#endif

#ifdef ENCODER_ENABLE
bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (!encoder_update_user(index, clockwise)) {
        return false;
    }
    // 0 is left-half encoder,
    // 1 is right-half encoder
    if (index == 0) {
        // Volume control
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    } else if (index == 1) {
        // Page up/Page down
        if (clockwise) {
            tap_code16(A(KC_RIGHT));
        } else {
            tap_code16(A(KC_LEFT));
        }
    }
    return true;
}
#endif


// append one character to an existing char array.
void string_append(char* str, char c){
	int len = strlen(str);
	str[len] = c;
	str[len+1] = '\0';
}

const char fullLetterMatrix[48] = "--######-SSTKPWH-RAO**----**EUFR-PBLGTSD-######Z";

bool post_process_steno_user(uint16_t keycode, keyrecord_t *record, steno_mode_t mode, uint8_t chord[MAX_STROKE_SIZE], int8_t n_pressed_keys) {

	// if it's not about to send, I don't care about it.
	if(!((record->event.pressed) == false && (n_pressed_keys < 1))) return true;


		//newStroke[2] = 'J';
	//strncpy(newStroke,"",0);
	//newStroke[0] = '\0';
	char myFullStringToBuild[MAX_STROKE_LENGTH] = "";

	bool num_pressed = false;
	bool star_pressed = false;
	bool s_pressed = false;
	bool has_vowels = false;
	bool has_hyphened = false;
	bool should_append;
	uint8_t byteToInspect; // ranges from 0 to 5, corresponding to the 6 bytes
	int8_t highestMatchedChar = -1; // I want to know how far into the fullLetterMatrix I got.

	for(int i = 0; i<48;i++) {
		should_append = true; // set to false later if repeated symbol

		//if we are at 0,8,16,24,32,40, then we need to change arrays
		if(i%8==0) byteToInspect =chord[i/8];

		uint8_t leftMask = 0b10000000; //will look at leftmost bit

		if((leftMask & byteToInspect) == leftMask && fullLetterMatrix[i]!='-') {
			// We know that fullLetterMatrix[i] was pressed and it's a printable character


			if (i >= 18 && i <= 29) has_vowels = true; // This range corresponds to the vowels and the stars.


			// If we start to hit the ending consonants (index of 30) and we've yet to print a charactor,
			// then we better print a hyphen
			if(i >= 30 && !has_hyphened && (highestMatchedChar == -1 || !has_vowels)) {
				string_append(myFullStringToBuild, '-');
				has_hyphened = true;
			}

			highestMatchedChar = i;

			switch(fullLetterMatrix[i]) {
			case '#':
				(num_pressed) ? (should_append = false) : (num_pressed = true);
				break;
			case '*':
				(star_pressed) ? (should_append = false) : (star_pressed = true);
				break;
			case 'S':
				(s_pressed) ? (should_append = false) : (s_pressed = true);
			}

			if (should_append) string_append(myFullStringToBuild, fullLetterMatrix[i]);
		}

		byteToInspect = byteToInspect << 1; 
	}

	//string_append(myFullStringToBuild, '\0');
	string_append(myFullStringToBuild, '\0');
	//strncpy(newStroke, myFullStringToBuild, MAX_STROKE_LENGTH);

	//Add it to the array of the correct index
	add_stroke_to_oled(myFullStringToBuild);

	// strncpy(strokeList[nextStrokeListIndex], myFullStringToBuild, MAX_STROKE_LENGTH);
	// dprintf("Added %s to position %i.\n", myFullStringToBuild, nextStrokeListIndex);
	// nextStrokeListIndex++;
	// if (nextStrokeListIndex==MAX_STROKE_LIST_SIZE) nextStrokeListIndex=0;


	return true; // allows the rest of the processing to happen as normal.
}


/* The following code is how to implement this on my slave

// lets define a custom data type to make things easier to work with
typedef struct {
    uint8_t position; // position of the string on the array
    uint8_t length;
    char    str[RPC_S2M_BUFFER_SIZE - 2]; // this is as big as you can fit on the split comms message
} split_msg_t;
_Static_assert(sizeof(split_msg_t) == RPC_S2M_BUFFER_SIZE, "Wrong size");


// instead of
    transaction_rpc_send(RPC_ID_USER_STR, ARRAY_SIZE(stringToWrite), stringToWrite);
// you now do:
    split_msg_t msg = {0};
    msg.position = <your_variable>;
    msg.length = strlen(<your_string>) + 1;
    if (msg.length > ARRAY_SIZE(split_msg_t.str)) {
        // too big to fit
        // do something here if you like, but do not send the message
        return;
    }
    strcpy(msg.str, <your_string>);
    transaction_rpc_send(RPC_ID_USER_STR, sizeof(msg), &msg);

// instead of
    memcpy(stringToWrite, initiator2target_buffer, initiator2target_buffer_size);
// you now do:
    split_msg_t *msg = (split_msg_t *)initiator2target_buffer;
    memcpy(<your_array>[msg->position], msg->str, msg->length);
    
*/
