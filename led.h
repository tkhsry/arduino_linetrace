#include "def.h"
#include "color.h"
#pragma once

/*
  led def
*/
#define pin_led_blue  2
#define pin_led_red   3
#define pin_led_green 4


int pin_led = {2,3,4};
int white[] = {50,50,50};
int red[] = {50,0,0,};
int blue[] = {0,0,50};
int green[] = {0,50,0};
int yellow[] = {50,50,0};
int off[] = {0,0,0};

// led (red,green,blue)
class led {
    public:
        int pinMax = 2;
        int pin[];
        led(int pinNo[]);
        void on(int l_ary[]);
};

led::led(int pinNo[pinMax]){
    int i,lp_max = ARRAY_SIZE(pinNo);
    for(i=0;i<lp_max;i++){
        pinMode(pinNo[i],OUTPUT);
        pin[i] = pinNo[i];
    }
}

void led::on(int l_ary[]){
    int i;
    for(i=0;i<pinMax;i++){
        analogWrite(pinNo[i],l_ary[i]);
    }
}

int color_loop() {
  if (color_flag == false) {
    color_sokutei();
    if (red_count > Count_Base) {
      LED_RED; color = Red;
      color_flag=true;
      return 0;
    }
    if (yellow_count > Count_Base) {
      LED_YELLOW; color = Yellow; color_flag=true;
      return 0;
    }
    if (blue_count > Count_Base) {
      LED_BLUE; color = Blue; color_flag=true;
      return 0;
    }
    if (red_count < Count_Base && yellow_count < Count_Base && blue_count < Count_Base) {
      color_flag = false;
      return 1;
    }
  }
}

void color_reset() {
	LED_OFF;
	color_flag = false;
	red_count = 0; yellow_count = 0; blue_count = 0;
	color = Non;
}

