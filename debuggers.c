void matrix_scan_user(void) {
	static int drive_pin_index = 0;
	static int current_drive_index = -1;
	static uint16_t last_switch_time = 0;

	uint16_t now = timer_read();

	// Switch drive pin every 300 ms
	if (TIMER_DIFF_16(now, last_switch_time) > 1000) {
		last_switch_time = now;

		for (int i = 0; i < NUM_TEST_PINS; i++) {
			setPinInputHigh(test_pins[i]);
		}

		current_drive_index = drive_pin_index;
		setPinOutput(test_pins[current_drive_index]);
		writePinHigh(test_pins[current_drive_index]);

		uprintf("Driving HIGH on %s (idx %d)\r\n", pin_name_str(test_pins[current_drive_index]), current_drive_index);

		// prepare next drive index
		drive_pin_index++;
		if (drive_pin_index >= NUM_TEST_PINS) {
			drive_pin_index = 0;
		}
	}
}

void matrix_scan_user(void) {
	static int drive_pin_index = 0;
	static int current_drive_index = -1;
	static uint16_t last_switch_time = 0;

	static bool baseline[64]; // > NUM_TEST_PINS
	static bool baseline_valid = false;

	uint16_t now = timer_read();

	// Switch drive pin every 300 ms
	if (TIMER_DIFF_16(now, last_switch_time) > 1000) {
		last_switch_time = now;

		// 1) All pins as pulldown inputs
		for (int i = 0; i < NUM_TEST_PINS; i++) {
			setPinInputLow(test_pins[i]);
		}

		// 2) Take a baseline snapshot with no pin driven
		for (int i = 0; i < NUM_TEST_PINS; i++) {
			baseline[i] = readPin(test_pins[i]);
		}
		baseline_valid = true;

		// 3) Choose drive pin and drive it high
		current_drive_index = drive_pin_index;
		setPinOutput(test_pins[current_drive_index]);
		writePinHigh(test_pins[current_drive_index]);

		uprintf("Driving HIGH on %s (idx %d)\r\n", pin_name_str(test_pins[current_drive_index]), current_drive_index);

		// prepare next drive index
		drive_pin_index++;
		if (drive_pin_index >= NUM_TEST_PINS) {
			drive_pin_index = 0;
		}
	}

	// 4) Look for pins that changed from baseline while current pin is driven
	if (baseline_valid && current_drive_index >= 0) {
		for (int i = 0; i < NUM_TEST_PINS; i++) {
			if (i == current_drive_index)
				continue; // skip the driven pin

			bool now_high = readPin(test_pins[i]);

			// We're interested in pins that were 0 before, now 1
			if (!baseline[i] && now_high) {
				uprintf("Detected bridge: drive=%s read=%s (idx %d -> %d)\r\n", pin_name_str(test_pins[current_drive_index]), pin_name_str(test_pins[i]),
						current_drive_index, i);
			}
		}
	}
}
