//CaveFollowerRobot.ino

#include "CaveFollower.h"

#define MAX_DISTANCE 200
#define FRONT_MAX_DISTANCE 500

using namespace cfr;

byte rm[2] = {3, 4};
byte lm[2] = {5, 6};

byte tx_rx[2] = {51, 50};

//TRIG > ECHO > MAX_DISTANCE
byte f_sonar[3] = {11, 10 , FRONT_MAX_DISTANCE};
byte l_sonar[3] = {9, 8, MAX_DISTANCE};
byte r_sonar[3] = {13, 12, MAX_DISTANCE};

Robot r(lm, rm, l_sonar , f_sonar, r_sonar, tx_rx);

void setup()
{
	Serial.begin(9600);
	Serial.println("Begin!\n");
  Robot::global_speed = 150;
  r.setKp(1);
  r.setKd(0);
  r.run(Robot::global_speed, 0, Robot::Forward, Robot::Forward);
}

void loop() {
   if (r.bluetooth->available() > 0){
      double x = r.bluetooth->parseFloat();
      double y = r.bluetooth->parseFloat();
      r.setKp(x);
      r.setKd(y);
      r.bluetooth->println("Kp : " + String(x));
      r.bluetooth->println("Kd : " + String(y));
   }
    //r.printViaBluetooth(200, 1500);
     // r.printDistances(200, 1500);
    // r.followWall();
}
