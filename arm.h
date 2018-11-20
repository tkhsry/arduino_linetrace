#pragma once

#include "def.h"
#include "VarSpeedServo.h"


// センサーとボールの距離の範囲をmax,minで指定する
// TODO 吸引から挟む方式に変更したので距離を計測し直す
#define ballDistMax 7
#define ballDistMin 6
#define arm_max 4		// アームのモーター(関節)の数
#define ball_max 3		// 運搬できる最大のボール数

// search range min, max
#define s_rng 6
#define s_min 60
#define s_max 120

// データ配列
int pin_arm[arm_max] = { 2,3,4,5 };
int pin_gate[] = {8};
int pin_hand[] = {7,6};
int hand_close[] = {0,90};
int hand_open[]  = {80,0};
int hand_def[] = {20,140};
int gate_open[] = {90};
int gate_close[] = {180};
int gate_def[] = {180};
int arm_def[arm_max] = {90,160,10,90 };

// arm.h
void arm_setup();
void putBall();
void search_ball();

//
int ballCount = 0;		// search_ball() で変更
double distAve = 0;		// アームから地面の距離

class arm {
  public:
    int cnt_deg;  //current  deg
    int std_deg;  //standard deg
    void set(int pinNo, int deg);
    void move(int deg);
    void move(int args[], int t);
    void set(int deg);
    VarSpeedServo vss;
  private:
};

class arms {
	public:
	    int const wrst = 0;
	    int const knee = 1;
      int const axle = 2;
      int const shld = 3;
		  arms(int pinNo[],int deg[]);
		  void move(int deg[]);
      void move(int pinNo[],int deg[]);
      arm a[arm_max];
	private:
};

// pinの配列と角度の配列を与えると関節をすべて定義する
arms::arms(int pinNo[],int deg[]) {
	int i,lp_max = ARRAY_SIZE(pinNo);
	for (i = 0; i <= lp_max; i++) {
		a[i].set(pinNo[i],deg[i]);
	}
}

// deg_array を与えるとその角度にすべての関節を動かす
void arms::move(int deg[]) {
	int i, lp_max = ARRAY_SIZE(deg);
	for (i = 0; i <= lp_max; i++) {
    int mv_deg[] = {deg[i]};
		a[i].move(deg[i]);
	}
}

void arms::move(int pinNo[],int deg[]){
    int i,lp_max = ARRAY_SIZE(deg);
    for(i = 0; i <= lp_max; i++){
        int mv_deg[] = {deg[i]};
        a[pinNo[i]].move(deg[i]);
    }
}


// 関節を一つ作る
// 関節 --> pinNo , standard degree
void arm::set(int pinNo, int deg) {
	vss.attach(pinNo);
	std_deg = deg;
	cnt_deg = deg;
}

// 関節を一つ動かす
void arm::move(int deg) {
	vss.write(deg, 60, false);
}

double dist() {
	int i;
	double result;
	for (i = 0; i<6; i++) {
		int temp = analogRead(A6);  // センサーから読み込み
		double dist = 26.59*pow((double)temp*(5.0 / 1023), -1.209);
		distAve += dist;
		delay(50);
	}
	result = distAve / 6.0;
	distAve = 0;

	// TODO 吸引から挟む方式に変更したので距離を計測し直す
	// Serial.println("dist val = " + String(result) + "[mm]");
	return result;
}


// mod def
arms r_arm(pin_arm,arm_def);
arms hand(pin_hand,hand_open);
arms gate(pin_gate,gate_close);

void mod_setup() {
	pinMode(pin_sokkyo, INPUT); //赤外線センサー
	r_arm.move(arm_def);
    hand.move(hand_def);
    gate.move(gate_def);
}

void putBall() {
    int pb_deg[arm_max] = { 40,10,10,r_arm.a[r_arm.shld].std_deg };
	hand.move(hand_close);
	delay(D_DELAY);
    r_arm.move(pb_deg);
	hand.move(hand_open);
	delay(D_DELAY);
}

void search_ball() {
    int s_deg[] = { 110,180,38,r_arm.a[r_arm.axle].std_deg };
	r_arm.move(s_deg);
	for (r_arm.a[r_arm.axle].cnt_deg = s_min; \
	     r_arm.a[r_arm.axle].cnt_deg < s_max; \
	     r_arm.a[r_arm.axle].cnt_deg += s_rng) 
    {
		r_arm.a[r_arm.axle].move(r_arm.a[r_arm.axle].cnt_deg);
		delay(D_DELAY);
		if (dist() <= ballDistMax && dist() >= ballDistMin) {
			putBall();
			ballCount++;
			break;
		}
	}
}

void gate_switch() {
    gate.move(gate_open); //高橋
    delay(D_DELAY);
    gate.move(gate_close);
	delay(D_DELAY);
}
