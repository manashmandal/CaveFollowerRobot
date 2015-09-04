#include "CaveFollower.h"

#define MAX_DISTANCE 40
#define FRONT_MAX_DISTANCE 40
#define BACK_MAX_DISTANCE 35

#define frontMedianDistance front
#define leftMedianDistance left
#define rightMedianDistance right
#define backMedianDistance back



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
  Robot::global_speed = 150;
  Robot::global_right_speed = Robot::global_speed; //- 32;
  r.setKp(3.5);
  r.setKd(.1);
  r.ping_number = 2;
  //Optimum -> 150 speed, 3.5 kp and 0.1 kd
 // r.run(Robot::global_speed, 0, Robot::Forward, Robot::Forward);

 
}

void loop() {
  if (r.left() > 3 && r.right() > 3) r.followWall();

  else if (r.left() > 3 && r.right() == 0){

    r.bluetooth->println("Going Right");
    
    r.run();
    delay(10);
    if (r.back() == 0){
      
      r.bluetooth->println("Rotating START");
      
        r.bluetooth->println("Rotating");
        r.run(Robot::global_speed, 0 , Robot::Right);  
      
//        else r.run();
    }

    else  if (r.back() > 3 && r.back() < 30) {
          r.run();
          if (r.right() == 0) r.run(Robot::global_speed, Robot::global_speed, Robot::Forward, Robot::Forward);
        }
        
  }


  else if (r.right() > 3 && r.left() == 0){

    r.bluetooth->println("Going Right");
    
    r.run();
    delay(10);
    if (r.back() == 0){
      
      r.bluetooth->println("Rotating START");
      
        r.bluetooth->println("Rotating");
        r.run(0, Robot::global_speed, Robot::Left);  
      
//        else r.run();
    }

    else  if (r.back() > 3 && r.back() < 30) {
          r.run();
          if (r.left() == 0) r.run(Robot::global_speed, Robot::global_speed, Robot::Forward, Robot::Forward);
        }
        
  }
  

   else r.run();


   if (r.bluetooth->read() == 'a') r.printViaBluetooth();

   

  

}

  


