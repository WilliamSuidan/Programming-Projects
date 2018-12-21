#include "main.h"
#include "fbc.h"
#include "fbc_pid.h"
#include "mtrmgr.h"

/*
 OK for all declarations of motors USE THESE FUNCTIONS
 This is because motors cannot be reversed using pros so we must set them to negative in these functions
*/

void setLift(int speed){
  motorSet(LiftL,-speed);
  motorSet(LiftR,speed);
}
void setLDrive(int speed){
  motorSet(LDF, speed);
  motorSet(LDB, speed);
  //blrsMotorSet(LDF, speed, false);
  //blrsMotorSet(LDB, -speed, false);

}
void setRDrive(int speed){
  motorSet(RDF, -speed);
  motorSet(RDB, -speed);
//  blrsMotorSet(RDF, -speed, false);
  //blrsMotorSet(RDB, speed, false);

}
void setFlyWheel(int speed){
  motorSet(FWheelL, -speed);
  motorSet(FWheelR, -speed);
}
void setIndexor(int speed){
  motorSet(Indexor,-speed);
}
void setDrive(int left,int right){
  setLDrive(left);

  setRDrive(right);
}
void setFlipper(int speed){
  motorSet(flipper,speed);
}


/*
Joystick Axis
1 = Right Stick Horizontal
2 = Right Stick Vertical
3 = Left Stick Vertical
4 = Left Stick Horizontal
*/
int joystick(int axis){
  return joystickGetAnalog (1,axis);
}



int button(int btnGroup, char dir){
  if(dir == 'L') dir = JOY_LEFT;
  if(dir == 'R') dir = JOY_RIGHT;
  if(dir == 'U') dir = JOY_UP;
  if(dir == 'D') dir = JOY_DOWN;
  //IF YOU PUT LEFT OR RIGHT FOR 5 or 6
  //Will return undefined
  return joystickGetDigital(1, btnGroup, dir);
}

//Auton functions
int inchesToTicks(int inches){
  return (360 / (4 * 3.14159)) * inches;
}
int cmToTicks(int cm){
  return (360/(10.16*3.14159)) * cm;
}
int lError = 0;
//my PID
void driveForward(int distance, double kP){
  float kD = 0.0;
  float kI = 0;
  int integral = 0;
  int start = millis();
  encoderReset(driveL);
  encoderReset(driveR);
  int dL = distance - encoderGet(driveL);
  int dR = distance - encoderGet(driveR);
  int output = dL - dR;
  setLDrive(70);
  setRDrive(70-10);
  while(dL >= 0){
    dL = distance - encoderGet(driveL);
    dR = distance - encoderGet(driveR);
    int error = encoderGet(driveL) - encoderGet(driveR);
    integral = error + integral;
    output = (error * kP + kD * (error - lError) + kI * integral);
    setLDrive(70);
    setRDrive(-motorGet(RDB) - output);
    lError = encoderGet(driveL) - encoderGet(driveR);

    delay(100);
  }
  setDrive(-10, -10);
  delay(200);
  setDrive(0, 0);
}
void driveBackward(int distance,int breakout,double kP){
  float kD = 0.0;
  float kI = -0.0005;
  int integral = 0;
  int start = millis();
  encoderReset(driveL);
  encoderReset(driveR);
  int dL = distance + encoderGet(driveL);
  int dR = distance + encoderGet(driveR);
  int output = dL - dR;
  setLDrive(-80);
  setRDrive(-80+10);
  while(dL >= 0){
    dL = distance + encoderGet(driveL);
    dR = distance + encoderGet(driveR);
    int error = encoderGet(driveL) - encoderGet(driveR);
    integral = error + integral;
    output = (error * kP + kD * (error - lError) + kI * integral);
    setLDrive(-80);
    setRDrive(-motorGet(RDB) - output);
    lError = encoderGet(driveL) - encoderGet(driveR);

    delay(100);
  }
  setDrive(-10, 10);
  delay(200);
  setDrive(0, 0);
}
//17 cmToTicks is like 90 degrees
//20 cmToTicks is like 135 degrees
void turnEncoder(int dist){
  encoderReset(driveL);
  encoderReset(driveR);
  int turningSpeed = 70;
  setLDrive(turningSpeed * sgn(dist));
  setRDrive(turningSpeed * sgn(dist) * -1);
  int error = dist - encoderGet(driveL);
  while(error * sgn(dist) > 0){
      if(abs(error) < 200){
        turningSpeed = 40;
      }
      setLDrive(turningSpeed * sgn(dist));
      setRDrive(turningSpeed * sgn(dist) * -1);
      error = dist - encoderGet(driveL);
      delay(50);
  }
  setDrive(-10 * sgn(dist), 10 * sgn(dist));
  delay(75);
  setDrive(0, 0);
}
int sgn(int in){
  if(in > 0){
    return 1;
  }
  else if(in < 0){
    return -1;
  }
  else{
    return 0;
  }
}
int increment = 10;
int lE = 0;
int lR = 0;

void driveSlewRate(){
  lE = lDriveGoal + motorGet(LDF);
  lR = rDriveGoal - motorGet(RDB);
  if(abs(lE) > 5){
    setLDrive(motorGet(LDF) + lE * 0.25);
  }
  if(abs(lR) > 5){
    setRDrive(motorGet(RDB) + lR * 0.25);
  }

}


//This deserves some space


int lastE = 0;
int change = 0;
int encoderValue = 0;
int lastMillis = 0;
int currMillis = 0;
double ticks = 0;
int speed = 0;
int rpm = 0;
int error = 0;
int drive = 0;
float gain = 0.025;
int prev_error = 1;
int tbh = 0;
int firstCross = 1;
int flyWheelMethod = 1;
int inte = 0;
void flyWheelSpeedManager(){

  encoderValue = encoderGet(flyWheel);
  change = encoderValue - lastE;
  //printf("%d change\n",change);
  rpm = calcSpeed(change);

  error = flyWheelTargetSpeed - rpm;
  printf("%d\n",error);

  //printf("Error: %d",error);
  if(flyWheelTargetSpeed == 0){
    setFlyWheel(0);
  }
  else{
    if(flyWheelMethod == 0){
      drive = drive + (error * gain);
      if(sgn(error) != sgn(prev_error)){
        if(firstCross == 1){
          drive = 100;
          tbh = drive;
          firstCross = 0;
        }
        else{
          drive = 0.5 * (drive + tbh);
          tbh = drive;
        }
      }
      prev_error = error;
      setFlyWheel(drive);
    }
    else if(flyWheelMethod == 1){
      inte += error;
      setFlyWheel(-motorGet(FWheelL) + error * 0.055 + inte * 0.05 + (error - prev_error) * 0.01);
      prev_error = error;
    }
  }

  lastE = encoderValue;
}
int calcSpeed(int change){
  currMillis = millis();
  //printf("%d\n",currMillis - lastMillis);

  speed = (1000.0 / 100) * change * 60.0 / (360) * 5;
  //speed = ticks*5/360;
  lastMillis = currMillis;
  return speed;
}
//ticks per ms = change/change in Ms
//ticks per s = change/ (change in Ms/1000ms)
//ticks per minute = change/(change in Ms/1000/60)
//revolutions per minute = ticks per minute*5
