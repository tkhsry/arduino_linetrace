#pragma once

#define D_DELAY 1000
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

// mortor
#define line_thres 90  //黒白の境界値

// left: 0  center:3  right:5
int pr[] = {0,1,2,3,4,5};
int pr_size = ARRAY_SIZE(pr)-1;

class p_ref {
    public:
        int state[pr_size];
        p_ref(int pinNo[]);
        void get_state();
    private:
        int pin[pr_size];
        int const blk = line_thres;
};

p_ref::p_ref(int pinNo[]){
    int i,lp_max = ARRAY_SIZE(pinNo);
    for(i=0;i<lp_max;i++){
        pinMode(pinNo[i],INPUT);
        pin[i] = pinNo[i];
        state[i] = 0;
    }
}

void p_ref::get_state(){
    int i;
    for(i=0;i<pr_size;i++){
        state[i] = (analogRead(pin[i] > blk));
    }
}


// 一つのモーターを作成する
class mortor{
    public:
        mortor(int pinNo[2]);
        int pin[2];
        void move(int st[]);
};

mortor::mortor(int pinNo[2]){
    int i,lp_max=ARRAY_SIZE(pinNo);
    for(i=0;i<lp_max;i++){
        pinMode(pinNo[i],OUTPUT);
    }
}

// st :: high|low , high|low, Speed
void mortor::move(int st[]){
    analogWrite(pinNo[2], st[2]);
    digitalWrite(pinNo[0], st[0]);
    digitalWrite(pinNo[1], st[1]);
}

class wheel {
    public:
        mortor m[1];
        wheel(mortor mt[]);
};

wheel::wheel(mortor mt[]){
    int i,lp_max = 1;
    for(i=0;i<lp_max;i++){
        wheel
    }
}

#define pin_sokkyo A6
// #define pin_sokkyo2 A7

#define pin_color_doubt 22 //カラーセンサ
#define pin_color_range 28
#define pin_color_ck 26
#define pin_color_gate 24

#define pin_led 30

#define pin_smortorl_1 35 //左のモータ
#define pin_smortorl_2 33
#define pin_smortorl_3 13//PWM
#define pin_smortorr_1  43 //右のモータ
#define pin_smortorr_2  41
#define pin_smortorr_3 9//PWM

#define pin_servo_mortor1 8 
#define pin_servo_mortor2 7
#define pin_servo_mortor3 6

#define mortor_base  255 //モータのPWMの最大値、0～254段階
#define mortor_dif 100 //モータのPWMへの制限値
#define color_time 8  //カラーセンサの測光時間
#define red_thres 600
#define green_thres 1500
#define blue_thres 1800
#define color_range 1 //カラーセンサの感度設定

//linetrace
#define GO   1
#define BACK 2
#define LEFT 0
#define RIGHT 1
#define ROTATION 2
#define BLUE 0
#define YELLOW 1
#define RED 2
#define line_trace_second 1000

//ball catch
#define Stop 1
#define Infinity -1

//mortor
#define Mortor_Straight smortor_move(HIGH,LOW,mortor_base-10,HIGH,LOW,mortor_base-95)
#define Mortor_ImStop  smortor_move(HIGH,HIGH,0,HIGH,HIGH,0)
#define Mortor_Stop  smortor_move(LOW,LOW,0,LOW,LOW,0)
#define Mortor_Right smortor_move(HIGH,LOW,mortor_base-10,LOW,HIGH,mortor_base-85)
#define Mortor_Left  smortor_move(LOW,HIGH,mortor_base-10,HIGH,LOW,mortor_base-50)


// chance ball
#define whrRight smortor_move(LOW,LOW,0,LOW,HIGH,mortor_base-85)
#define whrLeft smortor_move(LOW,LOW,0,HIGH,LOW,mortor_base-85)
#define whlLeft smortor_move(LOW,HIGH,mortor_base-10,LOW,LOW,0)
#define whlRight smortor_move(HIGH,LOW,mortor_base-10,LOW,LOW,0)

#define Mortor_Back  smortor_move(LOW,HIGH,mortor_base-10,LOW,HIGH,mortor_base-50)

#define Mortor_SlowRight smortor_move(HIGH,LOW,140,HIGH,LOW,65)
#define Mortor_SlowLeft  smortor_move(HIGH,LOW,140,HIGH,LOW,95)
#define Mortor_SlowBack  smortor_move(LOW,HIGH,mortor_dif,LOW,HIGH,mortor_dif)
#define Mortor_SlowStraight smortor_move(HIGH,LOW,140,HIGH,LOW,55)

//sencer
#define Count_Base 20


//check distance
#define check_ball 0
#define check_ball_catch 1
#define check_wall 2
#define check_LineisBlack 1

//line trace option
#define Normal 0
#define Search_Ball 1


// catchBall
#define ON 1
#define OFF 0

line before_line = Center;


int line_count = 0, line_count2 = -1;
int color_hantei = 0,line_trace_count = 0;
bool return_flag = false, color_flag = false, ball_catch = false, turn_check = false, back_check = false;
bool adjust = false;

// chance ball() は一度のみ実行する
bool ChanceBallFlag = true;

//line trace base function
void linetrace_setup() {
  pinMode(pin_smortorl_1, OUTPUT);
  pinMode(pin_smortorl_2, OUTPUT);
  pinMode(pin_smortorl_3, OUTPUT);
  pinMode(pin_smortorr_1, OUTPUT);
  pinMode(pin_smortorr_2, OUTPUT);
  pinMode(pin_smortorr_3, OUTPUT);

}

void smortor_move(bool A,bool B,int Speed1,bool C,bool D,int Speed2){
    analogWrite(pin_smortorl_3, Speed1);
    analogWrite(pin_smortorr_3, Speed2);
    digitalWrite(pin_smortorl_1, A);
    digitalWrite(pin_smortorl_2, B);
    digitalWrite(pin_smortorr_1, C);
    digitalWrite(pin_smortorr_2, D);
}

int sokkyo_ave(int pin, int count) {
  int sum = 0, i;
  for (i = 0; i < count; i++)
    sum += analogRead(pin);
  return (sum/count);
}
