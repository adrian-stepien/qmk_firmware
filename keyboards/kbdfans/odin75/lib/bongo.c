/* Copyright 2022 HorrorTroll <https://github.com/HorrorTroll>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "bongo.h"
#include <stdlib.h>
#include "oled_driver.h"
#include "progmem.h"
#include "timer.h"
#include "wpm.h"
#include "util.h"

// Include the appropriate pixel art data based on variant
#ifdef BONGO_VARIANT_BADGER
#include "bongobadger.h"
#elif defined(BONGO_VARIANT_CAT)
#include "bongocat.h"
#else
// Default to cat if no variant specified
#include "bongocat.h"
#endif

// WPM-responsive animation stuff here
#define IDLE_SPEED 10  // below this wpm value your animation will idle
// #define PREP_FRAMES 1 // uncomment if >1
#define ANIM_WPM_LOWER 20  // above this wpm value typing animation to trigger
#define ANIM_FRAME_DURATION_MAX 450 // longest animation duration in ms
#define ANIM_FRAME_DURATION_MIN 100 // shortest animation duration in ms
#define IDLE_FRAME_DURATION 300  // how long each frame lasts in ms
#define ANIM_FRAME_RATIO 2.5 // how aggressively animation speeds up with wpm


static uint32_t curr_anim_duration = 0; // variable animation duration
static uint32_t bongo_timer = 0;
static uint32_t bongo_sleep = 0;
static uint8_t  current_idle_frame = 0;
// uint8_t current_prep_frame = 0; // uncomment if PREP_FRAMES >1
static uint8_t current_tap_frame = 0;

void animation_phase(void) {
    if (get_current_wpm() <= IDLE_SPEED) {
        current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
        oled_write_raw_P(idle[abs((IDLE_FRAMES - 1) - current_idle_frame)], ANIM_SIZE);
    }

    if (get_current_wpm() > IDLE_SPEED && get_current_wpm() < ANIM_WPM_LOWER) {
        // oled_write_raw_P(prep[abs((PREP_FRAMES-1)-current_prep_frame)], ANIM_SIZE); // uncomment if IDLE_FRAMES >1
        oled_write_raw_P(prep[0], ANIM_SIZE);  // remove if IDLE_FRAMES >1
    }

    if (get_current_wpm() >= ANIM_WPM_LOWER) {
        current_tap_frame = (current_tap_frame + 1) % TAP_FRAMES;
        oled_write_raw_P(tap[abs((TAP_FRAMES - 1) - current_tap_frame)], ANIM_SIZE);
    }
}


void render_bongo(void) {
    // variable animation duration. Don't want this value to get near zero as it'll bug out.
    curr_anim_duration = MAX(ANIM_FRAME_DURATION_MIN, ANIM_FRAME_DURATION_MAX - ANIM_FRAME_RATIO * get_current_wpm());

    if (get_current_wpm() > ANIM_WPM_LOWER) {
        oled_on();  // not essential but turns on animation OLED with any alpha keypress

        if (timer_elapsed32(bongo_timer) > curr_anim_duration) {
            bongo_timer = timer_read32();
            animation_phase();
        }

        bongo_sleep = timer_read32();
    } else {
        if (timer_elapsed32(bongo_sleep) > OLED_TIMEOUT) {
            oled_off();
        } else {
            if (timer_elapsed32(bongo_timer) > IDLE_FRAME_DURATION) {
                bongo_timer = timer_read32();
                animation_phase();
            }
        }
    }
}
