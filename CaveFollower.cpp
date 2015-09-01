#include "CaveFollower.h"

uint cfr::Robot::global_speed = 150;

int cfr::Robot::set_point = 1;

cfr::Robot::Robot(byte *lm, byte *rm, byte *l_sonar, byte *f_sonar, byte *r_sonar)
{
	//Assigning Motor Pins
	for (int i = 0; i < 2; i++){
		left_motor[i] = lm[i];
		right_motor[i] = rm[i];
	}

	//Assigning sonar pins
	for (int i = 0; i < 3; i++){
		fs[i] = f_sonar[i];
		rs[i] = r_sonar[i];
		ls[i] = l_sonar[i];
	}

	initialize();
}


cfr::Robot::Robot(byte *lm, byte *rm, byte *l_sonar, byte *f_sonar, byte *r_sonar, byte *rxtx)
{
  //Assigning Motor Pins
  for (int i = 0; i < 2; i++){
    left_motor[i] = lm[i];
    right_motor[i] = rm[i];
    rxTx[i] = rxtx[i];
  }

  //Assigning sonar pins
  for (int i = 0; i < 3; i++){
    fs[i] = f_sonar[i];
    rs[i] = r_sonar[i];
    ls[i] = l_sonar[i];
  }


  //Initializing Bluetooth RX TX
  bluetooth = new SoftwareSerial(rxTx[0], rxTx[1]);
  bluetooth->begin(9600);
  bluetooth->println("*****Initialized Bluetooth******");
  
  initialize();
}

void cfr::Robot::printViaBluetooth(byte short_delay, uint long_delay)
{
  updateDistances();
  bluetooth->println("******* DISTANCE BEGIN *********");
  bluetooth->println("Front distance: " + String(front_distance));
  delay(short_delay);
  bluetooth->println("Left distance: " + String(left_distance));
  delay(short_delay);
  bluetooth->println("Right distance: " + String(right_distance));
  delay(short_delay);
  bluetooth->println("\n ******** DISTANCE END ******* ");
  delay(long_delay);
}

void cfr::Robot::initialize(void)
{
	//Initializing motors
	for (int i = 0; i < 2; i++){
		pinMode(left_motor[i], OUTPUT);
		pinMode(right_motor[i], OUTPUT);
	}

	//Initializing sonars
	front_sonar = new NewPing(fs[0], fs[1], fs[2]);
	left_sonar = new NewPing(ls[0], ls[1], ls[2]);
	right_sonar = new NewPing(rs[0], rs[1], rs[2]);

	//Initialize distance with 0 value
	front_distance = 0;
	left_distance = 0;
	right_distance = 0;
  error = 0;
  previous_error = 0;
  setKp(0);
  setKd(0);
}

void cfr::Robot::run(uint left_speed, uint right_speed, Robot::dir left_dir, Robot::dir right_dir){
	if (left_dir == Forward && right_dir == Forward){
		analogWrite(left_motor[0], left_speed);
		analogWrite(left_motor[1], 0);
		analogWrite(right_motor[0], right_speed);
		analogWrite(right_motor[1], 0);
	} else if (left_dir == Backward && right_dir == Backward){
		analogWrite(left_motor[0], 0);
		analogWrite(left_motor[1], left_speed);
		analogWrite(right_motor[0], 0);
		analogWrite(right_motor[1], right_speed);
	} else run(Nowhere);
}


void cfr::Robot::run(uint left_speed, uint right_speed, Robot::dir direction){
	if (direction == Clockwise){
		analogWrite(left_motor[0], left_speed);
		analogWrite(left_motor[1], 0);
		analogWrite(right_motor[0], 0);
		analogWrite(right_motor[1], right_speed);
	}
	else if (direction == AntiClockwise){
		analogWrite(left_motor[1], left_speed);
		analogWrite(left_motor[0], 0);
		analogWrite(right_motor[0], right_speed);
		analogWrite(right_motor[1], 0);
	}

	else if (direction == Right){
		analogWrite(left_motor[0], left_speed);
		analogWrite(left_motor[1], 0);
		analogWrite(right_motor[0], 0);
		analogWrite(right_motor[1], 0);
	}

	else if (direction == Left){
		analogWrite(left_motor[0], 0);
		analogWrite(left_motor[1], 0);
		analogWrite(right_motor[0], right_speed);
		analogWrite(right_motor[1], 0);
	}
	else if (direction == BackwardLeft){
		analogWrite(left_motor[0], 0);
		analogWrite(left_motor[1], left_speed);
		analogWrite(right_motor[0], 0);
		analogWrite(right_motor[1], 0);
	}

	else if (direction == BackwardRight){
		analogWrite(left_motor[0], 0);
		analogWrite(left_motor[1], 0);
		analogWrite(right_motor[0], 0);
		analogWrite(right_motor[1], right_speed);
	}

	else run(Nowhere);
}


void cfr::Robot::run(Robot::dir direction){

	for (int i = 0; i < 2; i++){
		analogWrite(left_motor[i], 0);
		analogWrite(right_motor[i], 0);
	}
	
}

//Returns distance from front sonar [Average value]
uint cfr::Robot::frontDistance(void)
{
	front_distance = front_sonar->ping_cm();
	return front_distance;
}

uint cfr::Robot::leftDistance(void)
{
	left_distance = left_sonar->ping_cm();
	return left_distance;
}

uint cfr::Robot::rightDistance(void)
{
	right_distance = right_sonar->ping_cm();
	return right_distance;
}

void cfr::Robot::updateDistances(void)
{
	frontDistance();
	leftDistance();
	rightDistance();
}

void cfr::Robot::updateMedianDistances(void)
{
  front_distance = frontMedianDistance();
  left_distance = leftMedianDistance();
  right_distance = rightMedianDistance();
}

void cfr::Robot::printDistances(byte short_delay, uint long_delay)
{
//	updateDistances();
  updateMedianDistances();
	Serial.println("******* DISTANCE BEGIN *********");
	Serial.println("Front distance: " + String(front_distance));
	delay(short_delay);
	Serial.println("Left distance: " + String(left_distance));
	delay(short_delay);
	Serial.println("Right distance: " + String(right_distance));
	delay(short_delay);
	Serial.println("\n ******** DISTANCE END ******* ");
	delay(long_delay);
}

void cfr::Robot::followWall(void)
{

  if (checkTurn() == Robot::FollowCave) bluetooth->println("Follow Cave");
  else if (checkTurn() == Robot::TurnRight) bluetooth->println("Turn Right");
  else if (checkTurn() == Robot::TurnLeft) bluetooth->println("Turn Left");
  
  previous_error = error;
  error = calculateError();

  double add_value = kp * error + kd * (previous_error - error);

  /*
   * Debugging Purpose, uncomment if needed
   * 
   */
  //bluetooth->println("Add value: " + String(add_value));
  //Serial.println("Add value: " + String(add_value));
  
  if (add_value == 0.0) run(global_speed, global_speed, Forward, Forward);
  else if (add_value < 0.0) run(global_speed, global_speed - add_value, Forward, Forward);
  else if (add_value > 0.0) run(global_speed + add_value, global_speed, Forward, Forward);

  //while (frontMedianDistance() < 7) run(Nowhere);
}

int cfr::Robot::calculateError(void)
{
  int ld = leftMedianDistance();
  int rd = rightMedianDistance();
  //Serial.println("Ld: " + String(ld));
  //Serial.println("Rd: " + String(rd));
//  bluetooth->println("difference: " + String(rd - ld));
  return rd - ld;
}

int cfr::Robot::checkTurn(void)
{
  //bluetooth->println("Front Median Distance: " + String(frontMedianDistance()) + "\nLeft Median Distance: " + String(leftMedianDistance()) + "\nRight Median Distance: " + String(rightMedianDistance()));
  if (frontMedianDistance() >= 3 && frontMedianDistance() <= 9) {
    if (leftMedianDistance() >= 3 && leftMedianDistance() <= 12) return Robot::TurnRight;
    else if (rightMedianDistance() >= 3 && rightMedianDistance() <= 12) return Robot::TurnLeft; 
  } else return Robot::FollowCave;
}


int cfr::Robot::frontMedianDistance(void)
{
  int uS = front_sonar->ping_median();
//  Serial.println("Front Distance: " + String(uS / US_ROUNDTRIP_CM));
//  bluetooth->println("Front Distance: " + String(uS / US_ROUNDTRIP_CM));
  return uS / US_ROUNDTRIP_CM;
}


int cfr::Robot::leftMedianDistance(void)
{
  int uS = left_sonar->ping_median();
  //Serial.println("Left Distance: " + String(uS / US_ROUNDTRIP_CM));
//  bluetooth->println("Left Distance: " + String(uS / US_ROUNDTRIP_CM));
  return uS / US_ROUNDTRIP_CM;
}



int cfr::Robot::rightMedianDistance(void)
{
  int uS = right_sonar->ping_median();
//  Serial.println("Right Distance: " + String(uS / US_ROUNDTRIP_CM));
//  bluetooth->println("Right Distance: " + String(uS / US_ROUNDTRIP_CM));
  return uS / US_ROUNDTRIP_CM;
}
