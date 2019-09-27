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

#include <ctcomm.h>
#include <math.h>
#include <mbed.h>

#include "config.hpp"
#include "pins.hpp"

bool tolerance_test(float expected, float measured, float tolerance,
                    char unit_of_meas);
void voltage_test();
void current_test();
void serial_cmd();
void store_cmd(unsigned char* cmd, int len);
void parse_cmd();

DigitalOut test_relay_5V(RELAY_5V_TEST_PIN);
DigitalOut curr_meas_relay_5V(RELAY_5V_PIN);
DigitalOut ev_charging(EV_CHARGING_PIN);
DigitalOut ev_charging1(EV_CHARGING1_PIN);
DigitalOut rcd_ctrl(RCD_CTRL_PIN);

AnalogIn sense_3v3(SENSE_3V3_PIN);
AnalogIn sense_5v(SENSE_5V_PIN);
AnalogIn sense_12v(SENSE_12V_PIN);
AnalogIn sense_min12v(SENSE_MIN_12V_PIN);
AnalogIn curr_meas_5v(CURR_MEAS_5V_PIN);
DigitalIn pilot_sense(PILOT_SENSE_PIN);
DigitalIn pilot1_sense(PILOT1_SENSE_PIN);
DigitalIn rcd_test_sense(RCD_TEST_SENSE_PIN);
DigitalIn ledout_sense(LEDOUT_SENSE_PIN);

DigitalIn aux2(AUX2_PIN);

// Use to stringify SERIAL_BUFF_LEN and use it with scanf for variable lenght
// specifier
#define STR2(x) #x
#define STR(x) STR2(x)

volatile int rx_in = 0;

// Serial pc(TX_PIN, RX_PIN);
CTCOMM com(TX_PIN, RX_PIN, 115200);
static uint8_t cmd_buff[RX_BUF_LENGTH + 2];
static volatile uint8_t cmd_len = 0;

char tmps[50] = "";

int main() {
  com.attach(&store_cmd);

  test_relay_5V = 0;
  curr_meas_relay_5V = 0;
  ev_charging = 0;
  ev_charging1 = 0;
  rcd_ctrl = 0;

  current_test();

  while (1) {
    parse_cmd();
  }
}

/**
 * @brief Enum with valid commands
 *
 */
enum cmd {
  VOLT_TEST = 0x30,
  CURR_TEST = 0x31,

};

void parse_cmd() {
  // parse only if we have received something
  if (cmd_len == 0) return;

  // uint16_t t = 0;

  switch (cmd_buff[0]) {
    // switch case base don command received
    case VOLT_TEST:
      voltage_test();
      break;

    default:
      com.dbg((char*)"Invalid cmd");
      break;
  }

  cmd_len = 0;
  // com.send_command(cmd, len);
}

/**
 * @brief Test if measured value is around nominal value, with given tolerance.
 * Also print results on serial line
 *
 * @param expected  nominal value
 * @param measured  measured value
 * @param tolerance tolerance, bw 0 and 1 (0-100%)
 */
bool tolerance_test(float expected, float measured, float tolerance,
                    char unit_of_meas) {
  bool res = false;

  char res_txt[4];

  if ((expected * (1 - tolerance)) < measured &&
      measured < (expected * (1 + tolerance))) {
    strcpy(res_txt, "OK ");
    res = true;
  } else {
    strcpy(res_txt, "ERR");
  }

  sprintf(tmps, "%s ( %.3f %c\terr %.3f%% )\n", res_txt, measured, unit_of_meas,
          (measured - expected) * 100 / expected);
  // com.dbg(" ( %.3f V\terr %.3f%% )\n", measured,
  //          (measured - expected) * 100 / expected);

  com.dbg(tmps);

  return res;
}

/**
 * @brief Test all power lines voltages
 *
 */
void voltage_test() {
  com.dbg((char*)"---- VOLTAGE TEST START ----");

  // Power lines voltage test
  com.dbg((char*)"3.3V: ");

  tolerance_test(3.3, sense_3v3.read() * 3.3 / VOLTAGE_RATIO_3V3, SENSE_3V3_TOL,
                 'V');

  com.dbg((char*)"5V  : ");
  tolerance_test(5, sense_5v.read() * 3.3 / VOLTAGE_RATIO_5V, SENSE_5V_TOL,
                 'V');

  com.dbg((char*)"12V : ");
  tolerance_test(12, sense_12v.read() * 3.3 / VOLTAGE_RATIO_12V, SENSE_12V_TOL,
                 'V');

  // -12V need slightly different calculation, due to offset voltage
  // Invert tolerance sign for correct calculations
  com.dbg((char*)"-12V: ");
  tolerance_test(-12, sense_min12v.read() * 3.3 * 11.50 - 24.78,
                 -SENSE_MIN_12V_TOL, 'V');

  com.dbg((char*)"---- VOLTAGE TEST END ----");

  return;
}

void store_cmd(unsigned char* cmd, int len) {
  // se ho un comando in coda, ignoro e droppo. implementare coda?
  if (cmd_len == 0) {
    for (uint8_t i = 0; i < len; i++) {
      cmd_buff[i] = cmd[i + 1];
    }
    cmd_len = len - 1;
  }
}

/**
 * @brief Meaure current consumpion
 *
 */
// TODO slit test in half
void current_test() {
  com.dbg((char*)"---- CURRENT TEST START ----");

  // Test current with limiting resistor, to avoid short-circuit
  curr_meas_relay_5V = 0;
  wait_ms(RELAY_SET_TIME);
  test_relay_5V = 1;
  wait_ms(RELAY_SET_TIME);

  // check if current is bw expected range
  bool res = tolerance_test(
      TEST_CURR_VAL, ((curr_meas_5v.read() * 3.3) - CURR_OFFSET) / CURR_RATIO,
      CURR_TOL, 'A');

  if (!res) {
    com.dbg((char*)"Cannot proceed with test, check board!");
    com.dbg((char*)"---- CURRENT TEST END ----");
    return;
  }

  // No short-circuit, continue with actual current measurements
  test_relay_5V = 0;
  wait_ms(RELAY_SET_TIME);
  curr_meas_relay_5V = 1;
  wait_ms(RELAY_SET_TIME);

  tolerance_test(CURR_VAL,
                 ((curr_meas_5v.read() * 3.3) - CURR_OFFSET) / CURR_RATIO,
                 CURR_TOL, 'A');

  com.dbg((char*)"---- CURRENT TEST END ----");
}
