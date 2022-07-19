#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Jeston_nano_GPIO.h"

int main(){
 int  gp_pin,value, i;
 char buff[256];
 FILE *jetson_led_gpio;
 
 printf("gp:");
 scanf("%d",&gp_pin);

 gpio_export(gp_pin);

gpio_set_dir(gp_pin,1);


  
 value = 0;
 while(1){
 gpio_set_value(gp_pin,value);
  
 sleep(1);
  value = !value;
 }
 fclose(jetson_led_gpio);

 gpio_unexport(gp_pin);

 return 0;
}
