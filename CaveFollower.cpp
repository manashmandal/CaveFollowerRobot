#include "CaveFollower.h"

uint cfr::Robot::global_speed = 150;

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

void cfr::Robot::printDistances(byte short_delay, uint long_delay)
{
	updateDistances();
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
