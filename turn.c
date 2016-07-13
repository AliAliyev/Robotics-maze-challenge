#include <stdio.h>
#include <stdlib.h>
#include "picomms.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
int i=0;
int left_enc=0;
int right_enc= 0;
 void turn()
 {
 	while(left_enc<846) {
 		set_motors(20, -20);
 		get_motor_encoders(&left_enc, &right_enc);
	 }
 }
int main() {
	initialize_robot();
	connect_to_robot();

	turn();
	get_motor_encoders(&left_enc, &right_enc);
	set_motors(0,0);
	printf("%d %d", left_enc, right_enc);
	return 0;
}
