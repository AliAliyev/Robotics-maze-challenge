#include <stdio.h>
#include <stdlib.h>
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
double alpha = 0.0;
double beta = 0.0;

int temp_left = 0;
int temp_right = 0;

int i = 0;

char map[50];
int mapC = 0;
int flag = 0;
int flagda = 0;
int flagB = 0;

#define WHEEL_LEN 29.9
#define ROBOT_LEN 22.6


void adjust() {
	//while(alpha % 360 < 3)
}

void convert_left() {
	int i = 0;
	int j = 0;
	int k = 0;
	int temp = mapC;
	while (k <= (temp - 1) / 2) {

		for (i = 0; i < mapC; i++) {
			if (map[i] == 'B') {
				flagB = 1;
				if (map[i - 1] == 'S' && map[i + 1] == 'S'){
					map[i - 1] = 'B';
					for (j = i; j < mapC - 2; j++)
						map[j] = map[j + 2];
					mapC = mapC - 2;
				}
				else if (map[i - 1] == 'R' && map[i + 1] == 'L'){
					map[i - 1] = 'B';
					for (j = i; j < mapC - 2; j++)
						map[j] = map[j + 2];
					mapC = mapC - 2;
				}
				else if (map[i - 1] == 'L' && map[i + 1] == 'R'){
					map[i - 1] = 'B';
					for (j = i; j < mapC - 2; j++)
						map[j] = map[j + 2];
					mapC = mapC - 2;
				}
				else if (map[i - 1] == 'L' && map[i + 1] == 'S'){
					map[i - 1] = 'R';
					for (j = i; j < mapC - 2; j++)
						map[j] = map[j + 2];
					mapC = mapC - 2;
				}
				else	if (map[i - 1] == 'S' && map[i + 1] == 'L'){
					map[i - 1] = 'R';
					for (j = i; j < mapC - 2; j++)
						map[j] = map[j + 2];
					mapC = mapC - 2;
				}
				else if (map[i - 1] == 'L' && map[i + 1] == 'L'){
					map[i - 1] = 'S';
					for (j = i; j < mapC - 2; j++)
						map[j] = map[j + 2];
					mapC = mapC - 2;
				}
			}
		}
		k++;
	}

}

void convert_right() {
	int i = 0;
	int j = 0;
	int k = 0;
	int temp = mapC;
	while (k <= (temp - 1) / 2) {

		for (i = 0; i < mapC; i++) {
			if (map[i] == 'B') {
				flagB = 1;
				if (map[i - 1] == 'S' && map[i + 1] == 'S'){
					map[i - 1] = 'B';
					for (j = i; j < mapC - 2; j++)
						map[j] = map[j + 2];
					mapC = mapC - 2;
				}
				else if (map[i - 1] == 'R' && map[i + 1] == 'L'){
					map[i - 1] = 'B';
					for (j = i; j < mapC - 2; j++)
						map[j] = map[j + 2];
					mapC = mapC - 2;
				}
				else if (map[i - 1] == 'L' && map[i + 1] == 'R'){
					map[i - 1] = 'B';
					for (j = i; j < mapC - 2; j++)
						map[j] = map[j + 2];
					mapC = mapC - 2;
				}
				else if (map[i - 1] == 'R' && map[i + 1] == 'S'){
					map[i - 1] = 'L';
					for (j = i; j < mapC - 2; j++)
						map[j] = map[j + 2];
					mapC = mapC - 2;
				}
				else	if (map[i - 1] == 'S' && map[i + 1] == 'R'){
					map[i - 1] = 'L';
					for (j = i; j < mapC - 2; j++)
						map[j] = map[j + 2];
					mapC = mapC - 2;
				}
				else if (map[i - 1] == 'R' && map[i + 1] == 'R'){
					map[i - 1] = 'S';
					for (j = i; j < mapC - 2; j++)
						map[j] = map[j + 2];
					mapC = mapC - 2;
				}
			}
		}
		k++;
	}

}

void turnRight()
{
	double temp_value = 10.0;
	int counter = 0;
	get_motor_encoders(&left_enc, &right_enc);
	int temp_enc = left_enc;
	while (counter < 180)
	{

		set_motors(10, -10);
		counter++;
	}
	while (temp_value > 0)
	{
		get_motor_encoders(&left_enc, &right_enc);
		set_motors(temp_value, -temp_value);
		temp_value = temp_value - 0.05;
		if (left_enc - temp_enc > 211)
			break;
	}
	set_motors(0, 0);
	
}


void turnLeft()
{
	double temp_value = 10.0;
	int counter = 0;
	get_motor_encoders(&left_enc, &right_enc);
	int temp_enc = right_enc;
	while (counter < 180)
	{
		set_motors(-10, 10);
		counter++;
	}
	while (temp_value > 0)
	{
		get_motor_encoders(&left_enc, &right_enc);
		set_motors(-temp_value, temp_value);
		temp_value = temp_value - 0.05;
		if (right_enc - temp_enc > 211)
			break;
	}
	set_motors(0, 0);
}

void turnOver()
{
	double temp_value = 10.0;
	int counter = 0;
	get_motor_encoders(&left_enc, &right_enc);
	int temp_enc = left_enc;
	while (counter < 400)
	{

		set_motors(10, -10);
		counter++;
	}
	while (temp_value > 0)
	{
		get_motor_encoders(&left_enc, &right_enc);
		set_motors(temp_value, -temp_value);
		temp_value = temp_value - 0.03;
		if (left_enc - temp_enc > 425)
			break;
	}
	set_motors(0, 0);
	
}

void turn(double degree) {

	get_motor_encoders(&left_enc, &right_enc);
	int temp_enc = left_enc;
	int counter = 0;
	while (1) {
		if (degree > 0)
			set_motors(20, -20);
		else set_motors(-20, 20);
		get_motor_encoders(&left_enc, &right_enc);
		if (abs(left_enc) - temp_enc > 838 * degree / 360)
			break;
	}
}

void goStraight() {
	double temp_value = 20.0;
	int counter = 0;
	get_motor_encoders(&left_enc, &right_enc);
	int temp_enc = left_enc;
	while (counter < 360)
	{
		set_motors(30, 30);
		counter++;
	}
	while (temp_value > 0)
	{
		get_motor_encoders(&left_enc, &right_enc);
		set_motors(temp_value, temp_value);
		temp_value = temp_value - 0.125;
		if (left_enc - temp_enc > 715)
			break;
	}
	set_motors(0, 0);
	//printf("%lf\n", temp_value);
}

void goLeft(){
	turnLeft();
	goStraight();
}

void goRight() {
	turnRight();
	goStraight();
}

void goStraightFast() {
	int counter = 0;
	while(counter < 63) {
		set_motors(127, 127);
		counter++;
	}
	set_motors(0,0);
}

void turnRightFast() {
	int counter = 0;
	while(counter < 49) {
		set_motors(127, -127);
		counter++;
	}
	set_motors(0,0);
}

void turnLeftFast() {
	int counter = 0;
	while(counter < 49) {
		set_motors(-127, 127);
		counter++;
	}
	set_motors(0,0);
}

void goRightFast() {
	turnRightFast();
	set_motors(0,0);
	goStraightFast();
}

void goLeftFast() {
	turnLeftFast();
	goStraightFast();
}

void move() {
	int i;
	for (i = 0; i < mapC; i++){

		if (map[i] == 'S')
			goStraightFast();
		else if (map[i] == 'R')
			goRightFast();
		else if (map[i] == 'L')
			goLeftFast();

	}
}

void moveabit() {
	double temp_value = 20.0;
	int counter = 0;
	get_motor_encoders(&left_enc, &right_enc);
	int temp_enc = left_enc;
	while (counter < 150)
	{
		set_motors(20, 20);
		counter++;
	}
	while (temp_value > 0)
	{
		get_motor_encoders(&left_enc, &right_enc);
		set_motors(temp_value, temp_value);
		temp_value = temp_value - 0.065;
		if (left_enc - temp_enc > 515)
			break;
	}
	set_motors(0, 0);
	printf("%lf\n", temp_value);
}


void mapping()
{

	dl = WHEEL_LEN * (left_enc - temp_left) / 360;
	dr = WHEEL_LEN * (right_enc - temp_right) / 360;

	//in case robot turns right
	if (dl > dr)
	{
		beta = (dl - dr) / ROBOT_LEN;  // turning degree
		alpha = alpha + beta;
	}

	//in case robot turns left
	else
	{
		beta = (dr - dl) / ROBOT_LEN;  // turning degree
		alpha = alpha - beta;
	}

	Sy = ((dl + dr) / 2) * cos(alpha);
	Sx = ((dl + dr) / 2) * sin(alpha);
	y_dist += Sy;
	x_dist += Sx;
	total = sqrt(x_dist*x_dist + y_dist*y_dist);
	temp_left = left_enc;
	temp_right = right_enc;
}



void follow_wall_left()
{
    set_ir_angle(RIGHT, 25);
    set_ir_angle(LEFT, -25);
	
	current_dist = get_front_ir_dist(LEFT);
	right_dist = get_front_ir_dist(RIGHT);

	if (current_dist < 35) {
		ratio_R = speed*current_dist / dist;
		ratio_L = speed * dist / current_dist;
	}

	else {
		ratio_L = 22;
		ratio_R = 50;
	}
	
	if (get_us_dist() < 25) {
		flag = 1;
		turn(90);
	}
	else
		set_motors(ratio_L, ratio_R);
		
}

void follow_wall_right()
{
	set_ir_angle(RIGHT, 25);
    set_ir_angle(LEFT, -25);
	
	current_dist = get_front_ir_dist(LEFT);
	right_dist = get_front_ir_dist(RIGHT);

	if (right_dist < 25){
		ratio_L = speed*right_dist / dist;
		ratio_R = speed * dist / right_dist;
	}

	else {
		ratio_L = 45;
		ratio_R = 20;
	}

	if (get_us_dist() < 30)
	{
		flag = 1;
		turn(-90);
	}

	else
		set_motors(ratio_L, ratio_R);
}

void follow_left() {

        set_ir_angle(RIGHT, 45);
		set_ir_angle(LEFT, -45);
	
		if (get_front_ir_dist(LEFT) > 35){
			goLeft();
			if (flagda == 0)
				map[mapC] = 'L';
		}
		else if (get_us_dist() > 38) {
			goStraight();
			if (flagda == 0)
				map[mapC] = 'S';
		}
		else if (get_front_ir_dist(RIGHT) > 35) {
			goRight();
			if (flagda == 0)
				map[mapC] = 'R';
		}
		else {
			flag = 1;
			turnOver();
			goStraight();
			if (flagda == 0)
				map[mapC] = 'B';
		}
		mapC++;
	
	
}

void follow_right() {

    set_ir_angle(RIGHT, 45);
    set_ir_angle(LEFT, -45);
	
	if (get_front_ir_dist(RIGHT) > 35){
		goRight();
		if (flagda == 0)
		map[mapC] = 'R';
	}
	else if (get_us_dist() > 38) {
		goStraight();
		if (flagda == 0)
		map[mapC] = 'S';
	}
	else if (get_front_ir_dist(LEFT) > 35) {
		goLeft();
		if (flagda == 0)
		map[mapC] = 'L';
	}
	else {
		flag = 1;
		turnOver();
		goStraight();
		if (flagda == 0)
		map[mapC] = 'B';
	}
	mapC++;
}

void func()
{
	while (1)
	{	
		get_motor_encoders(&left_enc, &right_enc);
		mapping();

		if (total > 270)
			flagda = 1;

		set_point(x_dist, y_dist);

		//printf("%lf %lf\n", x_dist, y_dist);
		printf("alpha: %lf\n", alpha * 180 / M_PI);
		
        if (flagda == 0) {
			//follow_left();
			follow_right();
	}
		else {
		//follow_wall_left();
		follow_wall_right();
		}
		
		if (y_dist < -13) {
			//turn(162);
			set_motors(10, -10);
			
			if ((int) (alpha*180/M_PI) % 360 > 352 ) 
			break;
			
		}
				
	}
	
}

moveFast()
{
	int count = 0;
	while(count < 65) {
		set_motors(120,120);
		count++;
	}
}

int main()
{
    int i=0;
	int iC = 0;
	connect_to_robot();
	initialize_robot();
	set_ir_angle(RIGHT, 45);
	set_ir_angle(LEFT, -45);
	set_origin();
	
	
	moveabit();
	func();
	set_motors(0, 0);
	//convert_left();
	convert_right();
	//goStraight();
	while(i< 50) {
	set_ir_angle(RIGHT, 45);
	set_ir_angle(LEFT, -45);
	set_ir_angle(RIGHT, 0);
	set_ir_angle(LEFT, 0);
	i++;
	}
	moveFast();
    move();
	
	
	
	for (i = 0; i < mapC; i++)
		printf("%c ", map[i]);
	
	set_motors(0, 0);
	return 0;
}
