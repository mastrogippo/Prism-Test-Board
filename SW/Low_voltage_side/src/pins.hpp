// Prism test board - LV side
// Copyright (C) 2019 Mastro Gippo
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of  MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef PRISM_PINS_HPP
#define PRISM_PINS_HPP

#include <mbed.h>

// 5V relays
#define RELAY_5V_TEST_PIN PB_8  // relay for 5V line with 100ohm series resistor
#define RELAY_5V_PIN PB_9       // relay for 5V line with fuse

// Voltage sense lines
#define SENSE_3V3_PIN PA_0
#define SENSE_5V_PIN PA_1
#define SENSE_MIN_12V_PIN PB_0
#define SENSE_12V_PIN PB_1

// Current measure
#define CURR_MEAS_5V_PIN PA_2

// Pilot
#define PILOT_SENSE_PIN PA_5
#define PILOT1_SENSE_PIN PA_6

// Switch connected to resistors for emulating EV charging
#define EV_CHARGING_PIN PC_14
#define EV_CHARGING1_PIN PC_15

// AUX
#define AUX1_PIN PB_12
#define AUX2_PIN PA_11
#define AUX3_PIN PA_12
#define AUX4_PIN PA_8

// RCD
#define RCD_CTRL_PIN PA_7        // emulate RCD signal
#define RCD_TEST_SENSE_PIN PB_7  // read test request for RCD

// Communication
#define SWDIO_PIN PA_13
#define SWCLK_PIN PA_14

#define TX_PIN PA_9
#define RX_PIN PA_10

#define SDA_PIN PB_11
#define SCL_PIN PB_10

#define RSA_PIN PA_4
#define RSB_PIN PA_3

#define SCLK_PIN PB_13
#define MISO_PIN PB_14
#define MOSI_PIN PB_15

// Led
#define LEDOUT_SENSE_PIN PA_15

#endif