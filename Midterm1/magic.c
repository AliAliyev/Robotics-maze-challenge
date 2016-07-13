//Ali Aliyev, Istvan Hoffer

#include <stdio.h>
#include <stdlib.h>
#include "picomms.h"
#include <math.h>
#include "track.h"

void mapping(int add);
void turning_degree();

int ratio_L = 0;
int ratio_R = 0;

double speed = 30;     // default speed
int left_dist = 0;  // reading of left IR sensor
int dist = 24;         // default distance from the wall
int right_dist = 0;

double dl = 0.0;
double dr = 0.0;

float Sx = 0.0;
float Sy = 0.0;

double y_dist = 0.0;
double x_dist = 0.0;

double total = 0.0;

int left_enc = 0;
int right_enc = 0;

double alfa = 0.0;
double beta = 0.0;

int temp_left = 0;
int temp_right = 0;

#define WHEEL_LEN 31.4
#define ROBOT_LEN 23.5

void fill_var(){
    right_dist = get_front_ir_dist(RIGHT);
    left_dist = get_front_ir_dist(LEFT);
    get_motor_encoders(&left_enc, &right_enc);
}

void front_stop(){
    double dinamic_stop = 0;
    while (get_us_dist() > 14){
        int distance_left = 32 - get_us_dist();
        dinamic_stop = speed - (((speed-10.0)/20.0) * distance_left);
        set_motors(dinamic_stop, dinamic_stop);
        fill_var();
        turning_degree();
        mapping(0);
    }
    set_motors(0, 0);
    
}

int follow_left()
{
    ratio_R = speed*left_dist / dist;
    ratio_L = speed * dist / left_dist;
    
    if (get_us_dist() > 32) {
        
       // if (right_dist < 30)
         //   set_motors(30, -30);
        
        if (left_dist < 40 || (left_dist>50 && left_dist < 99))
            
            set_motors(ratio_L, ratio_R);
        
        else if (left_dist > 99) {
            set_motors(ratio_L * 2, ratio_R / 2);
        }
        
        else set_motors(40, 40);
        return 1;
    }
    
    
    else	 {
        front_stop();
        return 0;
    }
}

void turning_degree(){
    dl = WHEEL_LEN * (left_enc - temp_left) / 360;
    dr = WHEEL_LEN * (right_enc - temp_right) / 360;
    //printf("wheels %f %f\n", dl, dr);
    
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
	temp_left = left_enc;
	temp_right = right_enc;
    
}

void mapping(int add){
    Sy = ((dl + dr) / 2) * cos(alfa);
    Sx = ((dl + dr) / 2) * sin(alfa);
    y_dist += Sy;
    x_dist += Sx;
    if(add == 0){
        addNode(createNode(x_dist, y_dist));
    }
    
    set_point(x_dist, y_dist);
    //printf("X cord: %.2lf Y cord: %.2lf\n", x_dist, y_dist);
    //printf("Radian compared to origin: %.4lf\n", alfa);


    
}

void finalDist(){
    total = sqrt(y_dist*y_dist + x_dist * x_dist);
    //printList();
    printf("Distance: %lf\n", total);
    printf("Degree compared to origin: %lf\n", alfa * 180 / M_PI);
}

void turn(double degree){
    int left_speed = 20;
    int right_speed = -20;
    
    degree = fabs(degree);
    if(degree >= 180){
        left_speed = -20;
        right_speed = 20;
    }
    
    get_motor_encoders(&left_enc, &right_enc);
    int left_enc_compare = left_enc;
    int right_enc_compare = right_enc;
    double turn_average = (double)(abs(left_enc - left_enc_compare) +  abs(right_enc - right_enc_compare)) / 2.0;
    double turn_wheels = turn_average * 360.0 / 828.0;
    
    while(turn_wheels < degree){
        set_motors(left_speed, right_speed);
        turn_average = (double)(abs(left_enc - left_enc_compare) +  abs(right_enc - right_enc_compare)) / 2.0;
        turn_wheels = turn_average * 360.0 / 828.0;
        turning_degree();
        mapping(1);
        get_motor_encoders(&left_enc, &right_enc);
    }
    //printf("comp %i now %i", right_enc_compare, right_enc);
    set_motor(LEFT, 0);
    set_motor(RIGHT, 0);
}

void line(double go){
    get_motor_encoders(&left_enc, &right_enc);
    int left_enc_compare = left_enc;
    int right_enc_compare = right_enc;
    double turn_average = (double)(abs(left_enc - left_enc_compare) +  abs(right_enc - right_enc_compare)) / 2.0;
    double turn_wheels = turn_average * 31.42 / 360.0;
    
    while(turn_wheels < go){
        set_motors(20, 20);
        turn_average = (double)(abs(left_enc - left_enc_compare) +  abs(right_enc - right_enc_compare)) / 2.0;
        turn_wheels = turn_average * 31.42 / 360.0;
        turning_degree();
        mapping(1);
        get_motor_encoders(&left_enc, &right_enc);
    }
}

void crawl_back(){
    float* current = return_head();
    double next_distance = 0.0;
    double to_turn = 0.0;
    while(current[0] != 0.0 && current[1] != 0.0){
        next_distance = sqrt(pow(x_dist - current[0], 2) + pow(y_dist - current[1], 2));
        to_turn = atan(fabs(x_dist-current[0]) / fabs(y_dist-current[1])) + alfa;
        to_turn = to_turn * 180.0 / M_PI;
        printf("to turn: %lf \n", to_turn);
        printf("next coords %f %f \n", current[0], current[1]);
        printf("current coords %f %f\n", x_dist, y_dist);
        printf("distance: %lf \n", next_distance);
        printf("current degree: %lf \n", alfa * 180 / M_PI);
        turn(current[2]);
        line(next_distance);
        mapping(1);
        get_motor_encoders(&left_enc, &right_enc);
        current = return_head();
    }
}