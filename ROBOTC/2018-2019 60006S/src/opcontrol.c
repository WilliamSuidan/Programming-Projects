
#include "main.h"


 /*
 Joystick Axis
 1 = Right Stick Horizontal
 2 = Right Stick Vertical
 3 = Left Stick Vertical
 4 = Left Stick Horizontal
*/
void driveControl();
void liftControl();

void flywheelControl();
void indexor();

int lDCurrent = 0, rDCurrent = 0;

float power,turn;
int deadzone = 15;
float multiplier = (4.0/4.0);
const int driveStyle = 1;
int lD,rD;
int driveLeft[10] = {0};
int driveRight[10] = {0};
//0 = tank
//1 = arcade
bool auton = false;
void operatorControl() {
	while (1) {
		driveControl();
		flywheelControl();
		indexor();
		liftControl();
		flipperControl();
		delay(20);
	}
	while(1){
		if(auton == false && button(8,'U')){
			autonomous();
			auton = true;
		}
	}
}
void liftControl(){
	if(button(5,'U')){
		setLift(127);
	}
	else if(button(5,'D')){
		setLift(-127);
	}
	else{
		setLift(0);
	}
}
void driveControl(){
	if(driveStyle == 0){
		if(abs(joystick(3)) > deadzone){
			lD = joystick(3);
		}
		else{
			lD = 0;
		}

		if(abs(joystick(2)) > deadzone){
			rD = joystick(2);
		}
		else{
			rD = 0;
		}


		/*int step = 5;
		int stepPauseMS = 20;
		if(lD > lDCurrent){
			lDCurrent += step;
		} else if (lD < lDCurrent) {
			lDCurrent -= step;
		}
		if(rD > rDCurrent){
			rDCurrent += step;
		} else if (rD < rDCurrent) {
			rDCurrent -= step;
		}
		delay(stepPauseMS);

		setDrive(lDCurrent, rDCurrent);*/

	}
	if(driveStyle == 1){
		if(abs(joystick(3)) > deadzone){
			power = joystick(3) * multiplier;
		}
		else{
			power = 0;
		}

		if(abs(joystick(4)) > deadzone){
			turn = joystick(4) * multiplier;
		}
		else{
			turn = 0;
		}
		setDrive(-power - turn, -power + turn);
	}
}
void flywheelControl(){
	if(button(8,'U')){
		setFlyWheel(127);
	}
	if(button(8,'D')){
		setFlyWheel(0);
	}
}

void flipperControl(){
	if(button(8,'L')){
		setFlipper(50);
	}
	else if(button(8,'R')){
		setFlipper(-50);
	}
	else{
		setFlipper(0);
	}
}
void indexor(){
	if(button(6,'U')){
		setIndexor(127);
	}
	else{
		setIndexor(0);
	}

}
