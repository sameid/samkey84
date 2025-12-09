#include QMK_KEYBOARD_H
#include "analog.h"
#include "bootloader.h"
#include "matrix_pins_test.h"
#include "quantum.h"

#define MAIN_LIGHT_PIN C7

#define TOP_ROW_BACKLIGHT_PIN A6
#define BOTTOM_ROW_BACKLIGHT_PIN A7
#define MIDDLE_ROW_BACKLIGHT_PIN C8

#define CAPS_LOCKS_LIGHT_PIN B8

#define TEST_ADC_PIN B2

enum mode_switch_state {
	MODE_WIRED,
	MODE_24G,
	MODE_BLE,
};

static enum mode_switch_state currentMode = MODE_WIRED;

static enum mode_switch_state readModeSwitch(void) {
	// 10-bit value: 0..1023
	uint16_t v = analogReadPin(TEST_ADC_PIN);

	// TODO: adjust thresholds after you see real values

	uprintf("Mode switch ADC value: %u\r\n", v);

	if (v < 300) {

		return MODE_WIRED;
	} else if (v < 700) {
		return MODE_24G;
	} else {
		return MODE_BLE;
	}
}

void turnOnBacklight(void) {
	setPinOutput(TOP_ROW_BACKLIGHT_PIN);
	writePinHigh(TOP_ROW_BACKLIGHT_PIN);

	setPinOutput(MIDDLE_ROW_BACKLIGHT_PIN);
	writePinHigh(MIDDLE_ROW_BACKLIGHT_PIN);

	setPinOutput(BOTTOM_ROW_BACKLIGHT_PIN);
	writePinHigh(BOTTOM_ROW_BACKLIGHT_PIN);

	setPinOutput(MAIN_LIGHT_PIN);
	writePinHigh(MAIN_LIGHT_PIN);
}

void turnOffBacklight(void) {
	setPinOutput(TOP_ROW_BACKLIGHT_PIN);
	writePinLow(TOP_ROW_BACKLIGHT_PIN);

	setPinOutput(MIDDLE_ROW_BACKLIGHT_PIN);
	writePinLow(MIDDLE_ROW_BACKLIGHT_PIN);

	setPinOutput(BOTTOM_ROW_BACKLIGHT_PIN);
	writePinLow(BOTTOM_ROW_BACKLIGHT_PIN);

	setPinOutput(MAIN_LIGHT_PIN);
	writePinLow(MAIN_LIGHT_PIN);
}

bool isCapsLocksOn = false;

void toggleCapsLocksLight(void) {

	if (isCapsLocksOn) {
		writePinLow(CAPS_LOCKS_LIGHT_PIN);
	} else {
		writePinHigh(CAPS_LOCKS_LIGHT_PIN);
	}

	isCapsLocksOn = !isCapsLocksOn;
}

// This runs once after USB is up
void keyboard_post_init_user(void) {
	debug_enable = true;
	debug_keyboard = true;
	debug_matrix = false;
	debug_mouse = false;

	// Set caps lock indicator pin as output
	setPinOutput(CAPS_LOCKS_LIGHT_PIN);
	turnOnBacklight();

	setPinInput(TEST_ADC_PIN);
}

void matrix_scan_user(void) {

	static uint16_t lastSwitchTime = 0;
	uint16_t now = timer_read();

	static enum mode_switch_state lastMode = MODE_WIRED;

	if (TIMER_DIFF_16(now, lastSwitchTime) > 1000) {
		lastSwitchTime = now;

		currentMode = readModeSwitch();

		if (currentMode != lastMode) {
			switch (currentMode) {
			case MODE_WIRED:
				uprintf("Mode switched to: WIRED\r\n");
				break;
			case MODE_24G:
				uprintf("Mode switched to: 2.4G\r\n");
				break;
			case MODE_BLE:
				uprintf("Mode switched to: BLE\r\n");
				break;
			default:
				break;
			}
			lastMode = currentMode;
		}

		/* for (int i = 0; i < NUM_TEST_PINS; i++) { */
		/* 	setPinInput(test_pins[i]); */
		/* 	bool pin_state = readPin(test_pins[i]); */
		/* 	uprintf("%s: %u\r\n", pin_name_str(test_pins[i]), pin_state); */
		/* } */
	}

	/* bool wired = readPin(B2); */
	/* if (wired) { */
	/* 	uprintf("Escape key wired to ground detected! Remapping ESC to GRV.\r\n"); */
	/* } else { */
	/* 	uprintf("Escape key not wired to ground.\r\n"); */
	/* } */
}

static bool esc_down = false;
static bool f4_down = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed) {
		uprintf("key pressed: %u\r\n", keycode);
	} else {
		uprintf("key released: %u\r\n", keycode);
	}
	switch (keycode) {

	case KC_ESC:
		esc_down = record->event.pressed;

		if (esc_down && f4_down) {
			wait_ms(20);
			bootloader_jump(); // Enter DFU mode
		}
		break;

	case KC_F4:
		f4_down = record->event.pressed;

		if (f4_down && esc_down) {
			wait_ms(20);
			bootloader_jump(); // Enter DFU mode
		}
		break;

	case BL_DOWN:
		if (record->event.pressed) {
			turnOffBacklight();
		}
		break;
	case BL_UP:
		if (record->event.pressed) {
			turnOnBacklight();
		}
		break;

	case KC_CAPS:
		if (record->event.pressed) {
			toggleCapsLocksLight();
		}
		break;

	default:
		break;
	}

	return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	// clang-format off
    /* [0] = LAYOUT_75_ansi( */
    /*     KC_Q */
    /* ) */
    [0] = LAYOUT_75_ansi(
        KC_ESC,  KC_BRID,   KC_BRIU,   KC_F3,   KC_F4,   BL_DOWN,   BL_UP,   KC_MPRV,   KC_MPLY,   KC_MNXT,   KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_PSCR, KC_PAUS, KC_DEL,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_HOME,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_END,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGUP,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   KC_PGDN,
        KC_LCTL, KC_LALT, KC_LGUI,                           KC_SPC,                             KC_RGUI, MO(1), KC_RALT, KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [1] = LAYOUT_75_ansi(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR, KC_PAUS, KC_DEL,
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_HOME,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,          KC_END,
        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,           KC_PGUP,
        KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   KC_PGDN,
        KC_LCTL, KC_LALT, KC_LGUI,                           KC_SPC,                             KC_RGUI, KC_TRNS, KC_RALT, KC_LEFT, KC_DOWN, KC_RGHT
    )
	// clang-format on
};
