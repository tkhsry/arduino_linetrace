#pragma once

// 軌道修正済み -> 1 , 修正前 -> 0
int isMod[5] = { 0 };

int pin_line[] = { 0,1,2,3,4,5 };
int pinLineCenter[] = { 0,0,1,1,0,0 };

// pinLineState -> 0..6までblack(1) or white(0)の情報を代入する配列
int pinLineState[5] = { 0 };

void get_pinLine() {
  int i;
  for (i = 0; i<6; i++) {
    pinLineState[i] = isBlack(pin_line[i]);
  }
}

void print_pinLine(){
  int i;
  get_pinLine();
  for(i=0;i<6;i++){
    Serial.print(pinLineState[i]);
  }
  Serial.println("");
  delay(200);
}

bool eqArray(int array1[], int array2[], int array_size) {
	int i;
	for (i = 0; i< array_size; i++) {
		if (array1[i] != array2[i]) {
			return false;
		}
	}
	return true;
}

/*
	int array[ 'size'][6] = { {0,0,0,0,0,0} ... } 
	一つでもcheck_array[] と等しいものがあればtrueを返す
*/
bool checkArray(int arrays[][6],int check_array[], int size) {
	int i;
	for (i = 0; i <= size; i++) {
		if (eqArray(arrays[i], check_array, 6))
			return true;
	}
	return false;
}

void modifiLine() {
	if (isMod[line_count] == 0) {
		switch (line_count) {
			case 2:
				Mortor_Straight;
				delay(500);

				while (true) {
					Mortor_Stop;
					get_pinLine();
					if (eqArray(pinLineState, pinLineCenter, 6) == true) {
						break;
					}
					Mortor_Right;
					delay(60);

				}

				break;  // case 2 END
		}
		isMod[line_count]++;

	}
}


