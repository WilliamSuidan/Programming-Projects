#pragma config(Motor,  port2,           motor1,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           motor2,        tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           motor3,        tmotorVex393_MC29, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	while(true){
		if(vexRT[Btn5U]){
			motor[motor1] = 127;
			motor[motor2] = 127;
			motor[motor3] = 127;
		}
		else if(vexRT[Btn5D]){
			motor[motor1] = -127;
			motor[motor2] = -127;
			motor[motor3] = -127;
		}
		else{
			motor[motor1] = 0;
			motor[motor2] = 0;
			motor[motor3] = 0;
		}
	}


}
