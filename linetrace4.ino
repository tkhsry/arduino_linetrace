#include "def.h"
#include "led.h"
#include "color.h"
#include "VarSpeedServo.h"
#include "arm.h"
#include "linetrace.h"

void setup() {
  Serial.begin(9600);
  color_setup();
  led_setup();
  linetrace_setup();
  mod_setup();
}

void loop() {
 line_trace_loop();
}

