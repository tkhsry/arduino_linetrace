#pragma once
#include <Arduino.h>

int color_count = 0, red_count = 0, yellow_count = 0, blue_count = 0, ball_count = 0, i, red, green, blue, red_dif, green_dif, blue_dif;
enum color { Red, Yellow, Blue, Non }; //列挙型で色を宣言
color color = Non;

int shiftIn() { //カラーセンサに使用する値を読み込む関数
  int result = 0; //検出結果用の変数を用意（0：初期化）
  for (int i = 0; i < 12; i++) { //12ビット分の繰り返し処理
    digitalWrite(pin_color_ck, HIGH); //1ビット分のクロックパルス出力（HIGH）
    delayMicroseconds(1);//1マイクロ秒待機
    if (digitalRead(pin_color_doubt) == HIGH) { //Dout端子からの出力がHighの場合
      result += (1 << i); //12ビットのi桁目に1を代入（i桁分だけ左にシフト）
    }
    digitalWrite(pin_color_ck, LOW); //1ビット分のクロックパルス出力（LOW）
    delayMicroseconds(1);//1マイクロ秒待機
  }
  delayMicroseconds(3);//3マイクロ秒待機
  return result;//結果を出力
}

void color_set(){
  digitalWrite(pin_color_gate, LOW);
  digitalWrite(pin_color_ck, LOW);
  delayMicroseconds(2000);//2000マイクロ秒待機
  digitalWrite(pin_color_range, color_range);
  digitalWrite(pin_color_gate, HIGH);
  delay(color_time);
  digitalWrite(pin_color_gate, LOW);
  delayMicroseconds(4);//4マイクロ秒待機
  red = shiftIn(); //赤の処理
  green = shiftIn(); //緑の処理
  blue = shiftIn(); //青の処理
  digitalWrite(pin_color_gate, HIGH);
}

int color_red(int dif) { //赤色の値を返す関数
  color_set();
  red = red - dif;
  return red;
}

int color_green(int dif) { //緑色の値を返す関数
  color_set();
  green = green - dif;
  return green;
}

int color_blue(int dif) { //青色の値を返す関数
  color_set();
  blue = blue - dif;
  return blue;
}

int red_different(){ //赤色の値を調整する関数
  int dif = 0, i;
  for(i = 0; i < 20; i++){
    if(color_red(dif) > 100){
       dif+=10;
    }
    if(color_red(dif) < 90){
      dif-=10;   
    }
  }
  return dif;
}

int green_different(){ //緑色の値を調整する関数
  int dif = 0, i;
  for(i = 0; i < 30; i++){
    if(color_green(dif) > 100){
       dif+=10;
    }
    if(color_green(dif) < 80){
      dif-=10;   
    }
  }
  return dif;
}

int blue_different(){ //青色の値を調整する関数
  int dif = 0, i;
  for(i = 0; i < 50; i++){
    if(color_blue(dif) > 100){
       dif+=10;
    }
    if(color_blue(dif) < 80){
      dif-=10;   
    }
  }
  return dif;
}

int color_dicision() { //ボールの色判定を行う関数
  int red, green, blue;
  red = color_red(red_dif);
  green = color_green(green_dif);
  blue = color_blue(blue_dif);

  if (red < 100 && green < 120 && blue < 120) { //どの値でもない時Nonを返す
    return Non;
  }
  if (red > blue && red > green) { //赤の時Redを返す
    return Red;
  }
  if (green > red && green > blue) { //黄色の時Yellowを返す
    return Yellow;
  }
  if (blue > red && blue > green) { //青の時Blueを返す
    return Blue;
  }
}

int color_adjustment(){//色の値を調整する関数（全色）
  red_dif = red_different();
  green_dif = green_different();
  blue_dif = blue_different();
}

void color_setup() {//カラーセンサに関するセットアップ
  pinMode(pin_color_doubt, INPUT);
  pinMode(pin_color_gate, OUTPUT);
  pinMode(pin_color_ck, OUTPUT);
  pinMode(pin_color_range, OUTPUT);
  color_adjustment();
}

void color_sokutei() {
  for (i = 0; i < Count_Base + 10; i++) {
    if (color_dicision() == Red) {
      red_count++; yellow_count = 0; blue_count = 0;
    }
    if (color_dicision() == Yellow) {
      yellow_count++; red_count = 0; blue_count = 0;
    }
    if (color_dicision() == Blue) {
      blue_count++; red_count = 0; yellow_count = 0;
    }
    if (color_dicision() == Non) {
      red_count = 0; yellow_count = 0; blue_count = 0;
    }
  }
}

