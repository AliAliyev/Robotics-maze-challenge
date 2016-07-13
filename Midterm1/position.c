//Ali Aliyev, Istvan Hoffer

#include <stdio.h>
#include <stdlib.h>
#include "picomms.h"
#include "magic.h"
#include <math.h>



void control(){
    set_ir_angle(LEFT, 0);
    set_ir_angle(RIGHT, -45);
    
    while (1) {
        fill_var();
        
        turning_degree();
        mapping(0);
		follow_left();
        if(follow_left() == 0){
            break;
        }
    }
    finalDist();
    turn(180.0);
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
