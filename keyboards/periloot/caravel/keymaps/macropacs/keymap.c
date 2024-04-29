// Copyright 2024 Kamran Shah (@whyaaronbailey)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "raw_hid.h"
#include "print.h"

enum custom_keycodes {
	COPYACC = SAFE_RANGE,
	OPENGE,
	OPENEPIC,
	PASTE,
	DICTATE,
	WL_SOFTTISSUE,
	WL_BONE,
	WL_BRAIN,
	WL_STROKE,
	WL_LUNG,
	WL_VASCULAR,
	WL_HARDWARE,
	ARROW,
	ZOOM_CINE,
	MEASURE,
	SCROLLUP,
	SCROLLDOWN,
	WHEELUP,
	WHEELDOWN,
	SPINE_C,
	SPINE_T,
	SPINE_L,
	NEXTFIELD,
	DELETE,
	ANNOTATIONS,
	PREVFIELD,
	FAST_SCROLLUP,
	FAST_SCROLLDOWN,
	SIGNREPORT
};

enum {
	TD_PAN_CINE = 0,
};
	

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
		WL_SOFTTISSUE,
        WL_STROKE, 
        WL_LUNG,
        ANNOTATIONS,
        ZOOM_CINE, 
        WL_BONE,  
        WL_BRAIN, 
        SCROLLUP,
        OPENEPIC,  
        ARROW, 
        TD(TD_PAN_CINE),
        FAST_SCROLLUP,  
        FAST_SCROLLDOWN, 
        SCROLLDOWN,
        MEASURE, 
        COPYACC,  
        OPENGE,  
        SPINE_C,  
        SPINE_L, 
        SPINE_T,  
        MO(1),   
        DICTATE, 
        DELETE, 
        NEXTFIELD
    ),
    [1] = LAYOUT(
        KC_4,
		KC_6,
		KC_5,
		KC_GRV,
		KC_PGUP, 
		KC_HOME, 
		KC_END,  
		KC_UP,   
		LSFT(KC_TAB), 
		KC_PGDN, 
		_______, 
		KC_LEFT, 
		KC_RGHT, 
		KC_DOWN, 
		QK_BOOT, 
		_______, 
		_______, 
		KC_7,    
		KC_9,    
		KC_8,    
		_______, 
		SIGNREPORT, 
		_______, 
		_______
    )
};

//Tap Dance FOR GE PAN / CINE
tap_dance_action_t tap_dance_actions[] = {
  [TD_PAN_CINE]  = ACTION_TAP_DANCE_DOUBLE(KC_P, KC_U)  // tap once for P, twice for U
};

static uint16_t REP_DELAY = 275; // Common delay for both scroll up and down
static uint16_t REP_DELAY_FAST = 150; // Common delay for both scroll up and down

uint32_t wh_callback(uint32_t trigger_time, void* cb_arg) {
    bool is_up = (bool)cb_arg;
    if (is_up) {
        tap_code(KC_MS_WH_UP);
    } else {
        tap_code(KC_MS_WH_DOWN);
    }
    return REP_DELAY;
}

uint32_t wh_callback_fast(uint32_t trigger_time, void* cb_arg) {
    bool is_up = (bool)cb_arg;
    if (is_up) {
        tap_code(KC_MS_WH_UP);
    } else {
        tap_code(KC_MS_WH_DOWN);
    }
    return REP_DELAY_FAST;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	
    static deferred_token token = INVALID_DEFERRED_TOKEN;

    switch (keycode) {
		
/* 		case RECORD:
			if (record->event.pressed) { // When key is pressed
				uint8_t data[30] = {0}; // Initialize all to zero
				data[1] = 0x04; // Set the HID Data for button press, adjust index as needed
				raw_hid_send(data, 30);
			} else { // When key is released
                uint8_t data[30] = {0}; // Initialize all to zero
				// HID Data for button release is already all zeros
			raw_hid_send(data, 30);
		}
		return false; // Skip all further processing of this key
		
  		case RECORD:
            switch (record->event.pressed) {
                case true:
                    // Send the HID report for the button press
                    uint8_t data_down[3] = {0x00, 0x04, 0x00};
                    raw_hid_send(data_down, sizeof(data_down));
                    break;

                case false:
                    // Send the HID report for the button release
                    uint8_t data_up[3] = {0x00, 0x00, 0x00};
                    raw_hid_send(data_up, sizeof(data_up));
                    break;
            }
        return false;  // Skip all further processing of this key */
     
		
        case SCROLLUP:
            if (record->event.pressed) {
                if (token) {
                    cancel_deferred_exec(token);
                    token = INVALID_DEFERRED_TOKEN;
                } else {
                    tap_code(KC_MS_WH_UP);
                    token = defer_exec(REP_DELAY, wh_callback, (void*)true);
                }
            }
            return false;

        case SCROLLDOWN:
            if (record->event.pressed) {
                if (token) {
                    cancel_deferred_exec(token);
                    token = INVALID_DEFERRED_TOKEN;
                } else {
                    tap_code(KC_MS_WH_DOWN);
                    token = defer_exec(REP_DELAY, wh_callback, (void*)false);
                }
            }
            return false;
			
        case FAST_SCROLLUP:
            if (record->event.pressed) {
                if (token) {
                    cancel_deferred_exec(token);
                    token = INVALID_DEFERRED_TOKEN;
                } else {
                    tap_code(KC_MS_WH_UP);
                    token = defer_exec(REP_DELAY_FAST, wh_callback_fast, (void*)true);
                }
            }
            return false;

        case FAST_SCROLLDOWN:
            if (record->event.pressed) {
                if (token) {
                    cancel_deferred_exec(token);
                    token = INVALID_DEFERRED_TOKEN;
                } else {
                    tap_code(KC_MS_WH_DOWN);
                    token = defer_exec(REP_DELAY_FAST, wh_callback_fast, (void*)false);
                }
            }
            return false;
					
        case COPYACC:  // Copies the accession number to the clipboard
            if (record->event.pressed) {
                SEND_STRING(
                    SS_LSFT("`")
                    SS_LSFT("`")
                    SS_TAP(X_ENTER)
                    SS_DELAY(100)
                    SS_LSFT(SS_LCTL(SS_TAP(X_LEFT)))
                    SS_LCTL("c")
                    SS_LSFT(SS_TAP(X_HOME))
                    SS_TAP(X_DEL) 
                    SS_TAP(X_DEL) 
                );
            }
            return false;

        case OPENGE: // Pastes the accession number in GE PACs
            if (record->event.pressed) {
                SEND_STRING(
                    SS_LSFT(SS_TAP(X_TAB))
                    SS_TAP(X_END)
                    SS_DELAY(75)
                    SS_TAP(X_TAB)
                    SS_LCTL("v")
                    SS_DELAY(150)
                    SS_TAP(X_ENTER)
                );
            }
            return false;

        case OPENEPIC: // Pastes the accession number in EPIC
            if (record->event.pressed) {
                SEND_STRING(
                    SS_LCTL("2")
                    SS_DELAY(1500)
                    SS_LCTL("v")
                    SS_TAP(X_ENTER)
                    SS_DELAY(1500)
                    SS_LALT(SS_LSFT("a"))
                );
            }
            return false;

        case PASTE: // Pastes the accession number in EPIC
            if (record->event.pressed) {
                SEND_STRING(
                    SS_LCTL("v")
                );
            }
            return false;
			
		case WL_SOFTTISSUE:
			if (record->event.pressed) {
				SEND_STRING(
					SS_TAP(X_BTN1)
					SS_DELAY(50)
					SS_TAP(X_KP_1)
				);
			}
		return false;
		
		case WL_BONE:
			if (record->event.pressed) {
				SEND_STRING(				
					SS_TAP(X_BTN1)
					SS_DELAY(50)
					SS_TAP(X_KP_2)
				);
			}
		return false;
		
		case WL_BRAIN:
			if (record->event.pressed) {
				SEND_STRING(
					SS_TAP(X_BTN1)
					SS_DELAY(50)
					SS_TAP(X_KP_3)
				);
			}
		return false;
        
		case WL_STROKE:
			if (record->event.pressed) {
				SEND_STRING(
					SS_TAP(X_BTN1)
					SS_DELAY(50)
					SS_TAP(X_KP_0)
				);
			}
		return false;

		case WL_LUNG:
			if (record->event.pressed) {
				SEND_STRING(
					SS_TAP(X_BTN1)
					SS_DELAY(50)
					SS_TAP(X_KP_5)
				);
			}
		return false;

		case WL_HARDWARE:
			if (record->event.pressed) {
				SEND_STRING(
					SS_TAP(X_BTN1)
					SS_DELAY(50)
					SS_TAP(X_KP_6)
				);
			}
		return false;
		
		case WL_VASCULAR:
			if (record->event.pressed) {
				SEND_STRING(
					SS_TAP(X_BTN1)
					SS_DELAY(50)
					SS_TAP(X_KP_8)
				);
			}
		return false;
		
		case ARROW:
			if (record->event.pressed) {
				SEND_STRING(
					SS_TAP(X_BTN1)
					SS_DELAY(50)
					SS_TAP(X_A)
				);
			}
		return false;
		
		case MEASURE:
			if (record->event.pressed) {
				SEND_STRING(
					SS_TAP(X_BTN1)
					SS_DELAY(50)
					SS_TAP(X_M)
				);
			}
		return false;
		
		case ZOOM_CINE:
			if (record->event.pressed) {
				SEND_STRING(
					SS_TAP(X_BTN1)
					SS_DELAY(50)
					SS_TAP(X_U)
				);
			}
		return false;
		
 		case ANNOTATIONS:
			if (record->event.pressed) {
				SEND_STRING(
					SS_TAP(X_BTN1)
					SS_DELAY(50)
					SS_TAP(X_Y)
				);
			}
		return false; 
		
		case SPINE_C:
			if (record->event.pressed) {
				SEND_STRING(
					SS_TAP(X_BTN1)
					SS_DELAY(50)
					SS_TAP(X_C)
				);
			}
		return false;

		case SPINE_T:
			if (record->event.pressed) {
				SEND_STRING(
					SS_TAP(X_BTN1)
					SS_DELAY(50)
					SS_TAP(X_T)
				);
			}
		return false;

		case SPINE_L:
			if (record->event.pressed) {
				SEND_STRING(
					SS_TAP(X_BTN1)
					SS_DELAY(50)
					SS_TAP(X_L)
				);
			}
		return false;
		
		case DELETE:
			if (record->event.pressed) {
				SEND_STRING(
					SS_TAP(X_F15) 
				);
			}
		return false;
		
		case WHEELUP:
			if (record->event.pressed) {
				SEND_STRING(
					SS_TAP(X_MS_WH_UP)
				);
			}
		return false;
		
		case WHEELDOWN:
			if (record->event.pressed) {
				SEND_STRING(
					SS_TAP(X_MS_WH_DOWN)
				);
			}
		return false;
		
		case DICTATE: // Pastes the accession number in EPIC SEND_STRING(SS_TAP(X_F13));
			if (record->event.pressed) {
				SEND_STRING(SS_TAP(X_F13));	
			}
		return false;

		case PREVFIELD:
			if (record->event.pressed) {
				SEND_STRING(SS_TAP(X_F15));
			}
		return false;	
		
		case NEXTFIELD: //SEND_STRING(SS_TAP(X_F14));
			if (record->event.pressed) {
				SEND_STRING(SS_TAP(X_F14));
			}
		return false;
		
		case SIGNREPORT: //SEND_STRING(SS_TAP(X_F16));
			if (record->event.pressed) {
				SEND_STRING(SS_TAP(X_F16));
			}
		return false;
    }
    return true; // Process all other keycodes normally.
}