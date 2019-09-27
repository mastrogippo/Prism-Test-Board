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

#ifndef PRISM_CONFIG_HPP
#define PRISM_CONFIG_HPP

#define RELAY_SET_TIME 4  // relay set time [ms]

// VOLTAGE TEST
// Power line sense voltage - value represented as fraction of ADC voltage
// (3.3v). We represent it in this form as read() return value from 0 to 1 (see
// docs)
// ADC 12bit - see schematic for voltage divider ratios
// Example: 3.3V line has voltage divider ratio of 0.25. Expected voltage
// is 3.3*0.25=0.825, analog value is 0.825*/3.3=0.25
// Voltage ratio are then corrected after real measurements to include deviation
// on resistors nominal values
#define VOLTAGE_RATIO_3V3 0.26221  // 0.25
#define VOLTAGE_RATIO_5V 0.25498   // 0.25
#define VOLTAGE_RATIO_12V 0.23910  // 0.25

#define SENSE_3V3_VAL 3.3 * VOLTAGE_RATIO_3V3 / 3.3
#define SENSE_5V_VAL 5 * VOLTAGE_RATIO_5V / 3.3
#define SENSE_12V_VAL 12 * VOLTAGE_RATIO_12V / 3.3
#define SENSE_MIN_12V_VAL 0.34

// Power lines voltage tolerance, from 0 t0 1 (0% to 100%)
#define VOLTAGE_TOL 0.1
#define SENSE_3V3_TOL VOLTAGE_TOL
#define SENSE_5V_TOL VOLTAGE_TOL
#define SENSE_12V_TOL VOLTAGE_TOL
#define SENSE_MIN_12V_TOL VOLTAGE_TOL

// CURRENT TEST
#define TEST_CURR_VAL 0.05  // expected test current (w limiting resistor) [A]
#define CURR_VAL 0.02       // expected current consumpion from test board [A]
#define CURR_OFFSET 1.61724

#define CURR_TOL 0.1
#define CURR_RATIO 0.126  // TODO correct value from measurements

#endif