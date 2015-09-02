//CaveFollowerRobot.ino

#include "CaveFollower.h"

#define MAX_DISTANCE 29
#define FRONT_MAX_DISTANCE 100
#define BACK_MAX_DISTANCE 30

using namespace cfr;

byte rm[2] = {3, 4};
byte lm[2] = {5, 6};

byte tx_rx[2] = {51, 50};

//TRIG > ECHO > MAX_DISTANCE
byte f_sonar[3] = {11, 10 , FRONT_MAX_DISTANCE};
byte l_sonar[3] = {9, 8, MAX_DISTANCE};
byte r_sonar[3] = {13, 12, MAX_DISTANCE};
byte b_sonar[3] = {31, 30, BACK_MAX_DISTANCE};

Robot r(lm, rm, l_sonar , f_sonar, r_sonar, b_sonar,tx_rx);



void setup()
{
  Serial.begin(9600);
  Serial.println("Begin!\n");
  Robot::global_speed = 245;
  r.setKp(3.5);
  r.setKd(.1);
  r.ping_number = 2;
  //Optimum -> 150 speed, 3.5 kp and 0.1 kd
 // r.run(Robot::global_speed, 0, Robot::Forward, Robot::Forward);

 
}

void loop() {
//
//  if (r.checkTurn() == Robot::TurnRight){
//    r.bluetooth->println("Turn Right");
//    while (r.frontMedianDistance() != 0 && !(r.backMedianDistance() > 3 && r.backMedianDistance() < 12)){ 
//      r.run(Robot::global_speed - 70, Robot::global_speed - 70, Robot::Clockwise);
//    }
//    r.run();
//    delay(250);
//  } 
//
//  else if (r.checkTurn() == Robot::TurnLeft){
//    r.bluetooth->println("Turn Left");
//    while (r.frontMedianDistance() != 0 && !(r.backMedianDistance() > 3 && r.backMedianDistance() < 12)){
//    r.run(Robot::global_speed - 70, Robot::global_speed - 70, Robot::AntiClockwise);
//    }
//    r.run();
//    delay(250);
//  }
//
//  else if (r.calculateError() == -r.leftMedianDistance()){
//    while (!(r.frontMedianDistance() > 2 && r.frontMedianDistance() < 12)) r.run(Robot::global_speed - 50, Robot::global_speed - 50, Robot::Forward, Robot::Forward);
//  }
//     r.bluetooth->print("Error: ");
//    r.bluetooth->println(r.calculateError());
//    r.bluetooth->print("Dist: ");
//    r.bluetooth->println(r.leftMedianDistance());
//    r.bluetooth->print("Front Dist: ");
//    r.bluetooth->println(r.frontMedianDistance());
    
  if (r.calculateError() == -r.leftMedianDistance()){
    while (r.frontMedianDistance() >= 15) 
        {
          r.run(Robot::global_speed - 50, Robot::global_speed - 50, Robot::Forward, Robot::Forward);    
        }

         if (r.frontMedianDistance() < 15) {r.run(); r.bluetooth->println("Broken");}
    }
  

    

  else r.run();

//  else if (r.calculateError() ==

//  else r.followWall();
//  r.followWall();
//  r.printViaBluetooth();
//  r.bluetooth->println(r.calculateError());
 
}

