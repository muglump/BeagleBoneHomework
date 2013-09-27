#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h> 
#include <signal.h> 
#include "gpio.h"

#define PIN_MUX_PATH "/sys/kernel/debug/omap_mux/"

int main(int argc, char *argv[]){
	//The gpio port numbers needed to drive the motor
	unsigned int gpio1 = 30;
	unsigned int gpio2 = 31;
	unsigned int gpio3 = 48;
	unsigned int gpio4 = 51;
	
	int behavior = argv[1]
	
	//Port numbers for the analog in
	char[] AnalogIn1 = "AIN0";
	char[] AnalogIn2 = "AIN1";
	
	//Arrays to hold the values we get back from the sensors
	int Sensor1_val[20];
	int Sensor2_val[20];
	int Sensor_sum[20];
	
	//Initializes the position as zero, but we dont really know where this is
	int pos = 0;
	
}