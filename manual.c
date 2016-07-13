#include <stdio.h>
#include <stdlib.h>
//#include <windows.h>
#include "picomms.h"
#include "conio.h"

int i=0;
int main(int argc, char *argv[]) {
	
while(1) {

switch (getch()) {

case 87:
 while(i<3){	
 set_motors(100, 100);
 i++;
           }
 i=0;
 set_motors(0,0);
 break;
 
case 65: 
 while(i<3){	
 set_motors(-100, 100);
 i++;
           }
 i=0;
 set_motors(0,0);
 break;
 
 case 68:
 while(i<3){	
 set_motors(100, -100);
 i++;
           }
 i=0;
 set_motors(0,0);
 break;
 
 case 83:
 while(i<3){	
 set_motors(-100, -100);
 i++;
           }
 i=0;
 set_motors(0,0);
 break;
 
}
}
	return 0;
}
