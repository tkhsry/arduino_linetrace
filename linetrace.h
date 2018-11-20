#include "led.h"
#include "def.h"
#include "arm.h"
#include "modifi.h"


#define DELAY_PROC(proc, sec) \
{\
  static unsigned long last_time = millis() - sec;\
  if (last_time + sec <= millis()) { proc; last_time = millis(); }\
}

int lt_white[][6] = {  {0,0,1,1,0,0},\
            {0,0,0,1,0,0},\
            {0,0,1,0,0,0},\
            {0,0,0,1,0,0},\
            {0,0,0,0,0,0} };
int ch_white[][6] = {  {0,0,1,1,0,0},\
            {0,0,0,1,0,0},\
            {0,0,1,0,0,0},\
            {0,0,0,1,0,0},\
            {0,1,1,0,0,0},\
            {0,1,0,0,0,0},\
            {0,1,1,1,0,0} };
int lt_blackLine[][6] = {
						{1,1,1,1,1,1},\
						{1,1,1,0,1,1} };
//右に傾いている
int lt_right[][6] = {	{0,1,0,0,0,0},\
						{1,1,0,0,0,0},\
						{1,0,0,0,0,0},\
            {0,1,1,0,0,0},\
						{1,1,1,0,0,0} };
//左に傾いている
int lt_left[][6] = {	{0,0,0,0,0,1},\
						{0,0,0,0,1,1},\
						{0,0,0,0,1,0},\
            {0,0,0,1,1,0},\
						{0,0,0,1,1,1} };

// 引数 :: 色情報
// かごの線まで来たときどのように回転するべきかを決める
// 色情報や線の情報をリセットして次の回収に繋げる

void ball_discharge(int option) {
	Mortor_Stop;
	switch (option) {
		case BLUE:
			Mortor_Straight;
			delay(2000);		//要計測
			gate_switch();
			break;
		// 色判定しないなら以下は各必要がない
		case RED:
		case YELLOW:
			//TODO ボール排出処理
			// gate_switch();
			break;
	}

	/*
		linetrace 処理のリセット
		色情報のリセット line_count2 = -1 にする
		line_count2 -> 0	帰り
		line_count2 -> -1	行き
	*/
	Mortor_Stop;
	color_reset();
	line_count2 = -1;
}


//linetrace
int line_trace(int option);

void line_counting(int line_num) {
	if (line_num == GO) {
		line_count++;
	}
	else if (line_num == BACK) {
		line_count2++;
	}
}


void line_trace_sec(int sec) {
	int linetrace_count;
	for (linetrace_count = 0; linetrace_count < sec; linetrace_count++) {
		line_trace(Normal);
	}
}


/* Function: ライントレース
 *  Args:
 *   option = 'Search_Ball' | 'Normal'
 *  Return:
 *   Option が Search_Ball の時,意味のある値を返す
 *   
 *  Option:
 *   Search_Ball
 *    -> ボールを探しながらライントレース
 *   Normal (or Other)
 *    -> 通常のライントレース
 */


int line_trace(int option) {

	// linetrace for search the ball
	if (option == Search_Ball) {
		if (ballCount >= ball_max) {
			// line_count が最大に達したら戻るようにプログラムされている
			line_count = LineMax;
			
			//仮に ball_max = 1 とした場合
			/*
			for (;;) {
				color_loop();
				if (color_flag == true)
					break;
			}
			*/

		}
		search_ball();
		// 1s 進んでボールを探す
		line_trace_sec(1000);
	}

	// linetrace for normal black lines
	else {    //else1 begin
		get_pinLine();
		if (checkArray(lt_white, pinLineState, 4)) {
			Mortor_Straight;
		}
		if (checkArray(lt_left, pinLineState, 4)) {
			Mortor_Right;
		}
		if (checkArray(lt_right, pinLineState, 4)) {
			Mortor_Left;
		}
		if (checkArray(lt_blackLine, pinLineState, 1)) {
			if (line_count < LineMax) {
				DELAY_PROC({ line_counting(GO); }, 1000);
			}
			else {
				DELAY_PROC({ line_counting(BACK); }, 2000);
			}
		}
	}//else1 end
	
}

void ChanceBall() {
  if(ChanceBallFlag == true && line_count == 2){
    int i;
    int const c_right = 2650;
    int const c_left  = 2900;
    
    Mortor_Stop;
    delay(1000);
  	whlRight;
  	delay(c_right);
  	Mortor_Stop;
    Mortor_Back;
    delay(800);
    Mortor_Stop;
  	gate_switch();
  	whlLeft;
  	delay(c_left-1900);
    for(;;){
        get_pinLine();
        if (checkArray(ch_white, pinLineState, 6)) {
          whlRight;
          delay(200);
          Mortor_Stop;
          break;
        }
         whlLeft;
      }
  	Mortor_Stop;
  	ChanceBallFlag = false;
  }
}


// main loop
void line_trace_loop() {

  // check line_count
  // Serial.println(line_count);

	if(line_count < LineMax) {
		switch (line_count) {
			case 0:
			case 1:
			case 2:
				//print_pinLine();
				//modifiLine();
			case 3:
			case 4:
        line_trace(Normal);
        ChanceBall();
        break;
			// ボールエリア  
			case 5:
			case 6:							// すっぽんのとき一度バックしたので二度線を踏むから必要だった
				Mortor_Stop;
				line_trace(Search_Ball);	// ボールがキャッチできたか判定して 帰りのルートへ変更する処理はline_trace()で行う
				break;
		}
	}

	//back the line
	if (line_count >= LineMax) {
		switch (line_count2) {
			case -1:
				Mortor_Stop;
				delay(1000);

				// 方向を逆にするように右回転させる(実測) --> coming soon..
				Mortor_Right;
				delay(2000);
        
				line_count2++;
				break;
			case 0:
			case 1:
				line_trace(Normal);
				break;
			case 2:
				//Blue
				if (color == Blue)
					ball_discharge(BLUE);
				else
					line_trace(Normal);
				break;
			case 3:
				// yellow
				if (color == Yellow)
					// 90度回転するように右回転させる
					ball_discharge(YELLOW);
					// さらに90度回転するように右回転させて逆方向へ(REDも同様)
				else
					line_trace(Normal);
				break;
			case 4:
				//red
				ball_discharge(RED);
				break;
		}
	}
}
