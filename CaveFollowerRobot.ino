//CaveFollowerRobot.ino

#include "CaveFollower.h"

#define MAX_DISTANCE 200

using namespace cfr;

byte lm[2] = {3, 4};
byte rm[2] = {5, 6};

//TRIG > ECHO > MAX_DISTANCE
byte f_sonar[3] = {11, 10 , MAX_DISTANCE};
byte l_sonar[3] = {9, 8, MAX_DISTANCE};
byte r_sonar[3] = {13, 12, MAX_DISTANCE};

Robot r(lm, rm, l_sonar , f_sonar, r_sonar);

void setup()
{
	Serial.begin(9600);
	Serial.println("Begin!\n");
  Robot::global_speed = 150;
  

}

void loop() {

    
}
