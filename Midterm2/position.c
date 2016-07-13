//Ali Aliyev, Istvan Hoffer

#include <stdio.h>
#include <stdlib.h>
#include "picomms.h"
#include "magic.h"
#include <math.h>

void control(){
    set_ir_angle(LEFT, 0);
    set_ir_angle(RIGHT, 0);
    
    while (1) {
        fill_var();
        
		//follow_left();
        
        if(follow_left() == 0){
            break;
        }
        turning_degree();
        mapping(0);
    }
    finalD();
   // turn(175.0);
	turnda();
	start_robot();
	
    crawl_back();
}



int main()
{
    connect_to_robot();
    initialize_robot();
    set_origin();
    control();
    //turn(90.0);
    //final();
    set_motors(0, 0);
    
    return 0;
}
