#include "declarations.h"
#include "Ultrasonic.h"
#include <PID_v1.h>


Ultrasonic ultrasonicRight(URTRIGGER_PIN, URECHO_PIN), ultrasonicLeft(ULTRIGGER_PIN, ULECHO_PIN);
PID myPID(&Input, &Output, &Setpoint, 11.5, 0, 0, DIRECT);

void setup()
{
  intiallize();

}

void loop()
{
      botFwd();
      colorDetect();
  //  readIr(irLeftFw)?attachInterrupt(COLOR_LEFT,ISRColourSensorLeft,FALLING):detachInterrupt(COLOR_LEFT);
  //  readIr(irRightFw)?attachInterrupt(COLOR_RIGHT,ISRColourSensorRight,FALLING):detachInterrupt(COLOR_RIGHT);
  //turn(LEFT);
//  if (Serial.available()) {
//    data=Serial.read();
//  }
//  if ( data== '0') botMove(0,0);
//  else if (data == '1') {
//      botFwd();
//      colorDetect();
//    }
}

