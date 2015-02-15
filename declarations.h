#include "Arduino.h"

//#define MEASURING
#define DEBUG
#define led A0
#define ledGnd A1
//#define D_CENT 12.7 //cm
#define WALL_CUTOFF 27.5 //cm
#define WALL_CUTOFF_MIN 17.7 //cm
#define SPEED 150
#define TURN_DELAY 50000 //milliSecond
#define turnFwSpeed 12
#define CORRECTION_LEFT_ULTRA 0.5
#define CORRECTION_RIGHT_ULTRA 1.5 //1.3 for decent 12

#define COLOR_LEFT  8 //3
#define COLOR_RIGHT 9  //2

#define irLeftFw A5
#define irRightFw A4
#define irLeftRv A3
#define irRightRv A2


#define PULL_UP 4

#define URTRIGGER_PIN  12
#define URECHO_PIN     13

#define ULTRIGGER_PIN  2
#define ULECHO_PIN     3

#define LMFW 10
#define LMRV 11  

#define RMFW 5
#define RMRV 6

#define in(x) pinMode(x,INPUT)
#define out(x) pinMode(x,OUTPUT)

#define LEFT true
#define RIGHT false

#define pwm(x,y) analogWrite(x,y)


//////////////////////////////////////
float DistanceFrmLeft,DistanceFrmRight,pDistanceFrmRight,pDistanceFrmLeft,Distance,D_CENT=12.7;
double Setpoint=D_CENT, Input, Output;
int sl=0,sr=0;
bool Wallside=LEFT;
bool isInterstellarSpace=false;
char data;
/////////////////////////////////////
void intiallize(void);
void wallFollow(int CurrentDistance, bool wallSide, int TargetDistance=D_CENT, int Speed=SPEED);
void botMove(int speedL, int speedR);
float wallDist(bool);
void getSpeeds(bool dir);
void turn(bool dir);
void ISRColourSensorLeft();
void ISRColourSensorRight();
bool readIr(int);
void botFwd(void);
