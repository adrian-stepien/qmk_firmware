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

#pragma once

#include "progmem.h"

// Animation constants that all variants must conform to
#define IDLE_FRAMES 5
#define TAP_FRAMES 2
#define ANIM_SIZE 525

// All animation data headers must provide these arrays:
// extern const char PROGMEM idle[IDLE_FRAMES][ANIM_SIZE];
// extern const char PROGMEM prep[][ANIM_SIZE];
// extern const char PROGMEM tap[TAP_FRAMES][ANIM_SIZE];

// Main animation function
void render_bongo(void);
