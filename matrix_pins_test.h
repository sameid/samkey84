
#pragma once

#include "quantum.h"

/*
 * List of candidate pins to probe for the key matrix on WB32FQ95.
 * You can comment out pins that are clearly not used on your board
 * (e.g. crystal pins or anything that causes a crash), but this set
 * is a good general starting point.
 *
 * NOTE:
 *  - We skip A11/A12 (USB D-/D+) to avoid messing with USB.
 *  - We also skip A13/A14/A15 (SWD/JTAG) to avoid debug pins.
 */

static const pin_t test_pins[] = {

    // Port A (skip A11..A15)
    A0,
    A1,
    A2,
    A3,
    A4,
    A5,
    A6,
    A7,
    A8,
    A9,
    A10,
    A11,
    A12,
    A13,
    A14,
    A15,

    // Port B
    B0,
    B1,
    B2,
    B3,
    B4,
    B5,
    B6,
    B7,
    B8,
    B9,
    B10,
    B11,
    B12,
    B13,
    B14,
    B15,

    // Port C (on STM32-style MCUs, only C13–C15 are typically GPIO)
    C0,
    C1,
    C2,
    C3,
    C4,
    C5,
    C6,
    C7,
    C8,
    C9,
    C10,
    C11,
    C12,
    C13,
    C14,
    C15,
};

static const int NUM_TEST_PINS = sizeof(test_pins) / sizeof(test_pins[0]);

static inline const char *pin_name_str(pin_t pin) {
    switch (pin) {
        // Port A
        case A0:
            return "A0";
        case A1:
            return "A1";
        case A2:
            return "A2";
        case A3:
            return "A3";
        case A4:
            return "A4";
        case A5:
            return "A5";
        case A6:
            return "A6";
        case A7:
            return "A7";
        case A8:
            return "A8";
        case A9:
            return "A9";
        case A10:
            return "A10";
        case A13:
            return "A13";
        // A11..A15 intentionally omitted from test_pins

        // Port B
        case B0:
            return "B0";
        case B1:
            return "B1";
        case B2:
            return "B2";
        case B3:
            return "B3";
        case B4:
            return "B4";
        case B5:
            return "B5";
        case B6:
            return "B6";
        case B7:
            return "B7";
        case B8:
            return "B8";
        case B9:
            return "B9";
        case B10:
            return "B10";
        case B11:
            return "B11";
        case B12:
            return "B12";
        case B13:
            return "B13";
        case B14:
            return "B14";
        case B15:
            return "B15";

        // Port C
        case C0:
            return "C0";
        case C1:
            return "C1";
        case C2:
            return "C2";
        case C3:
            return "C3";
        case C4:
            return "C4";
        case C5:
            return "C5";
        case C6:
            return "C6";
        case C7:
            return "C7";
        case C8:
            return "C8";
        case C9:
            return "C9";
        case C10:
            return "C10";
        case C11:
            return "C11";
        case C12:
            return "C12";
        case C13:
            return "C13";
        case C14:
            return "C14";
        case C15:
            return "C15";

        default:
            return "??";
    }
}
