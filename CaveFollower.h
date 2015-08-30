
#ifndef CAVEFOLLOWER_H_
#define CAVEFOLLOWER_H_
#include <Arduino.h>
#include <math.h>
#include "NewPing.h"

typedef uint16_t uint;


namespace cfr {
	class Robot {
		private:
			//Motor Pins
			byte left_motor[2];
			byte right_motor[2];

			//Sonar variables
			NewPing *front_sonar;
			NewPing *left_sonar;
			NewPing *right_sonar;


			//Sonar pins
			// TRIG, ECHO, MAX_DISTANCE
			byte fs[3];
			byte ls[3];
			byte rs[3];

			//Unsigned distance variables
			uint front_distance;
			uint right_distance;
			uint left_distance;

			//Kp and Kd constant
			double kp, kd;

			//Error and previous error
			double error, previous_error;

			

		public:
			/*
			DESCRIPTION:
			============
			Constructor for initializing everything

			*/
			Robot(byte *lm, byte *rm, byte *l_sonar, byte *f_sonar,byte *r_sonar);

			/*
				DESCRIPTION:
				============
				Static variable for global speed
			*/

			static uint global_speed;

				/*
				DESCRIPTION:
				------------
				Enumeration for directions

				*/
			enum dir {Forward, Backward, Left, Right, Clockwise, AntiClockwise, BackwardLeft, BackwardRight, Nowhere};
			dir Direction;

				/*
					DESCRIPTION:
		============
		Motor driving functions to meet your needs. Directions are described here.
		[Left arrow for left motor and right arrow for right motor, left and right motors are defined when the robot is faced towards forward]
		Forward: ↑ ↑
		Backward: ↓ ↓
		Left: . ↑
		Right: ↑ .
		BackwardLeft: . ↓
		BackwardRight: ↓ .
		Clockwise: ↑ ↓
		AntiClockwise: ↓ ↑
		Nowhere: . .
		*/
				
		void run(uint left_speed, uint right_speed, dir left_direction, dir right_direction);

		void run(uint left_speed, uint right_speed, dir rotation_direction);

		void run(dir direction = Nowhere);

				/*
		 DESCRIPTION:
		 ===========
		 Setter functions for setting values of kp and kd
		*/

		void setKp(double p)  { kp = p; }
		void setKd(double d)  { kd = d; }

		/*
		  DESCRIPTION:
		  ============
		  Updates all the distances 

		*/

		  void updateDistances(void);

		  /*

			DESCRIPTION:
			=============
			Front, left and right distances


		  */

			uint frontDistance(void);
			uint leftDistance(void);
			uint rightDistance(void);


			/*
			DESCRIPTION:
			============
			Initializes everything

			*/

			void initialize(void);


			/*
				DESCRIPTION:
				=============

				Serial printout the distances



			*/

				void printDistances(byte short_delay = 100, uint long_delay = 1000);

	};
}

#endif
