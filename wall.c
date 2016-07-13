


#include <stdio.h>
#include "picomms.h"
#include <math.h>

int ratio_L = 0;
int ratio_R = 0;
double speed = 30;     // default speed
int current_dist = 0;  // reading of left IR sensor 
int dist = 32;         // default distance from the wall
int right_dist = 0;    // reading of right IR sensor

double dl = 0.0;
double dr = 0.0;

double Sx = 0.0;
double Sy = 0.0;

double y_dist = 0.0;
double x_dist = 0.0;

double total = 0.0;

int left_enc = 0;
int right_enc = 0;

double rad = 0.0;
double alfa = 0.0;
double beta = 0.0;

int temp_left = 0;
int temp_right = 0;

#define WHEEL_LEN 31.4
#define ROBOT_LEN 23


void follow()
{
	while (1)
	{

		get_motor_encoders(&left_enc, &right_enc);
		current_dist = get_front_ir_dist(LEFT);
		right_dist = get_front_ir_dist(RIGHT);


		ratio_R = speed*current_dist / dist;
		ratio_L = speed * dist / current_dist;
		
		//printf("%d %d\n", left_enc-temp_left, right_enc-temp_right);

		dl = WHEEL_LEN * (left_enc - temp_left) / 360;
		dr = WHEEL_LEN * (right_enc - temp_right) / 360;
		rad = abs(dl - dr) / ROBOT_LEN;
		Sy = ((dl + dr) / 2) * cos(rad);
		Sx = ((dl + dr) / 2) * sin(rad);
		y_dist += Sy;
		x_dist += Sx;
		total = sqrt(y_dist*y_dist + x_dist * x_dist);
		temp_left = left_enc;
		temp_right = right_enc;

		//in case robot turns right
		if (dl > dr) {
			beta = (dl - dr) / ROBOT_LEN;  // turning degree
			alfa = alfa + beta;
		}

		//in case robot turns left
		else {
			beta = (dr - dl) / ROBOT_LEN;  // turning degree
			alfa = alfa - beta;
		}

		if (get_us_dist() > 25) {

			if (right_dist < 30)
				set_motors(30, -30);

			else if (current_dist < 40 || (current_dist>50 && current_dist < 99))

				set_motors(ratio_L, ratio_R);

			else if (current_dist > 99) {
				set_ir_angle(LEFT, -45);
				set_motors(ratio_L * 2, ratio_R / 2);
				set_ir_angle(LEFT, 0);
			}

			else set_motors(40, 40);
    }


      else	 {		
		  int i = 0;
			while (i < 300){
				 speed=speed/1.015;
				set_motors(speed, speed);
				i++;
			 }

			break;
}

}
	
}


int main()
{
	connect_to_robot();
	initialize_robot();
	follow();
	printf("%lf\n", total);
	printf("%lf\n", alfa * 180 / M_PI);
	set_motors(0, 0);

	return 0;
}
