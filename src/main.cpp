#include "main.h"
//Varables with the ports for the ball System

int SharedRollersPort=8; //Shared Roller Port
int SinglePort=3; //Single Roller Port
int OpticalSensorPort=4; //Optical Sensor Port

pros::Controller master(CONTROLLER_MASTER);
//Seting up the motors


void autonomous() {
	Drive::TurnSlide(0,12,0, 100, 0, 0, 5, 5, 0, 0);

}
void opcontrol() {
}
