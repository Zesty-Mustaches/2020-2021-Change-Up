#include "main.h"
#include <iostream>
#include<string>
#include <cstdio>
#include <ctime>
using namespace std;

int frontRightPort = 9; //Variable with the front right motor port
int frontLeftPort = 1; //Variable with the front left motor port
int backRightPort = 10; //Variable with the back right motor port
int backLeftPort = 2; //Variable with the back left motor port

Motor FrontRight(frontRightPort, E_MOTOR_ENCODER_DEGREES); //Setting Up the Front Right Motor
Motor FrontLeft(frontLeftPort, E_MOTOR_ENCODER_DEGREES);  //Setting Up the Front Left Motor
Motor BackRight(backRightPort, E_MOTOR_ENCODER_DEGREES);  //Setting Up the Back Right Motor
Motor BackLeft(backLeftPort, E_MOTOR_ENCODER_DEGREES); //Setting Up the Back Left Motor

ADIEncoder Ltraking ('G', 'H', false);
ADIEncoder Rtraking ('A', 'B', true);
ADIEncoder Stracking('C', 'D', false);



namespace Drive{
int LeftPos;
int BackPos;
int RightPos;
float SmallDistance= 1.375/360;
float BigDistance= 10/360;

int PrevLeft;
int PrevBack;
int PrevRight;

float deltaLeft;
float deltaRight;
float deltaBack;

float totalDeltaDistR;
float totalDeltaDistL;

float currentAbsoluteOrriantion;
float StratTheta=0;

float LeftTrakingRadius=5.5;
float RightTrakinRadius=5.5;

float prevTheta;
float ThetaDelta;

float sTrakingRadius = 5.5;

float deltaXLocal;
float deltaYLocal;

float deltaXGlobal;
float deltaYGlobal;

float XPostionGlobal;
float YPostionGlobal;

float averageThetaArk;

 void Odemtry() {
   LeftPos = Ltraking.get_value();
   RightPos = Rtraking.get_value();
   BackPos = Stracking.get_value();

   deltaLeft = (LeftPos-PrevLeft)*SmallDistance;
   deltaRight = (RightPos-PrevRight)*SmallDistance;
   deltaBack = (BackPos-PrevBack)*BigDistance;

   PrevBack=BackPos;
   PrevLeft=LeftPos;
   PrevRight=RightPos;

  totalDeltaDistL += deltaLeft;
  totalDeltaDistR += deltaRight;

  currentAbsoluteOrriantion = StratTheta - ( (totalDeltaDistL-totalDeltaDistR)/(LeftTrakingRadius+RightTrakinRadius));  //Radians

  ThetaDelta = currentAbsoluteOrriantion - prevTheta;

  prevTheta = currentAbsoluteOrriantion;

  if (ThetaDelta == 0) {
    deltaXLocal = deltaBack;
    deltaYLocal = deltaRight;
  }

  else {
    deltaXLocal = sin(ThetaDelta/2)*((deltaBack/ThetaDelta)+sTrakingRadius);
    deltaYLocal = sin(ThetaDelta/2)*((deltaLeft/ThetaDelta)+LeftTrakingRadius);
  }

  averageThetaArk = currentAbsoluteOrriantion - (ThetaDelta/2);

  deltaXGlobal = (deltaYLocal*cos(averageThetaArk))-(deltaXLocal*sin(averageThetaArk));
   deltaYGlobal = (deltaYLocal * sin(averageThetaArk)) + (deltaXLocal * cos(averageThetaArk));

   XPostionGlobal += deltaXGlobal;
   YPostionGlobal += deltaYGlobal;
  
  std::string X = std::to_string(XPostionGlobal);
  std::string Y = std::to_string(YPostionGlobal);
    lcd::initialize();
    pros::lcd::set_text(1, X);
    pros::lcd::set_text(2, Y);


 }
  void opcontrol() {
    Odemtry();
    int power = master.get_analog(ANALOG_LEFT_Y);
	   // Turning is set to the x axis of the right joystick
	  int turn = master.get_analog(ANALOG_RIGHT_X);
	   // Strafe is set to the x axis of the left joystick
   	int strafe = master.get_analog(ANALOG_LEFT_X);
	   // We set the power of the Left Front Motor to the postion of the varaible power postion + the turn postion + the strafe postion
    int lf = power + turn - strafe;
    int lb = power + turn + strafe;
    int rf = power - turn - strafe;
    int rb = power - turn + strafe;


     FrontRight.move(-rf);
     FrontLeft.move(lf);
     BackRight.move(-rb);
     BackLeft.move(lb);
  }
}