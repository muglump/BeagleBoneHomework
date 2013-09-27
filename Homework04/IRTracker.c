#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h> 
#include <signal.h> 
#include "gpio.h"

#define PIN_MUX_PATH "/sys/kernel/debug/omap_mux/"
#define MAX_CYCLES = 20


void problemOne(unsigned int pins[4], int *pos){
	clockwiseRotation(pins, &pos);
	counterClockwiseRotation(pins, &pos);
}

void clockwiseRotation(unsigned int pins[4], int *pos){
	switch(*pos){
		case(0):
			gpio_set_value(0, pin[0]);
			gpio_set_value(1, pin[1]);
			gpio_set_value(1, pin[2]);
			gpio_set_value(0, pin[3]);
		case(1):
			gpio_set_value(1, pin[0]);
			gpio_set_value(1, pin[1]);
			gpio_set_value(0, pin[2]);
			gpio_set_value(0, pin[3]);
		case(3):
			gpio_set_value(1, pin[0]);
			gpio_set_value(0, pin[1]);
			gpio_set_value(0, pin[2]);
			gpio_set_value(1, pin[3]);
		case(4):
			gpio_set_value(0, pin[0]);
			gpio_set_value(0, pin[1]);
			gpio_set_value(1, pin[2]);
			gpio_set_value(1, pin[3]);
		
	}
	delay(50);
	*pos = (*pos+1)%4; 	
}

void counterClockwiseRotation(unsigned int pin[4], int *pos){
	switch(*pos){
		case(0):
			gpio_set_value(0, pin[1]);
			gpio_set_value(1, pin[2]);
			gpio_set_value(1, pin[2]);
			gpio_set_value(0, pin[4]);
		case(1):
			gpio_set_value(0, pin[1]);
			gpio_set_value(0, pin[2]);
			gpio_set_value(1, pin[3]);
			gpio_set_value(1, pin[4]);
		case(3):
			gpio_set_value(1, pin[1]);
			gpio_set_value(0, pin[2]);
			gpio_set_value(0, pin[3]);
			gpio_set_value(1, pin[4]);
		case(4):
			gpio_set_value(1, pin[1]);
			gpio_set_value(1, pin[2]);
			gpio_set_value(0, pin[3]);
			gpio_set_value(0, pin[4]);
	}
	delay(50);
	*pos = (*pos-1)%4; 	
}

int main(int argc, char *argv[]){
	//The gpio port numbers needed to drive the motor
	unsigned int gpio1 = 30;
	unsigned int gpio2 = 31;
	unsigned int gpio3 = 48;
	unsigned int gpio4 = 51;
	unsigned int gpio5 = 15;
	unsigned int gpios[4] = {gpio1, gpio2, gpio3, gpio4};
	int behavior = argv[1]
	int rc;
	int len;
	//Port numbers for the analog in
	char[] AnalogIn1 = "AIN0";
	char[] AnalogIn2 = "AIN1";
	char buf[64];
	//Arrays to hold the values we get back from the sensors
	int Sensor1_val;
	int Sensor2_val;
	//int Sensor_sum;
	
	//Initializes the position as zero, but we dont really know where this is
	int pos = 0;
	
	//Set up the gpios so we can use them to drive the motor
	gpio_export(gpio1);
	gpio_export(gpio2);
	gpio_export(gpio3);
	gpio_export(gpio4);
	gpio_export(gpio5);
	
	gpio_set_dir(gpio1, "out");
	gpio_set_dir(gpio2, "out");
	gpio_set_dir(gpio3, "out");
	gpio_set_dir(gpio4, "out");
	gpio_set_dir(gpio5, "in");
	
	gpio_fd1 = gpio_fd_open(gpio1, O_RDONLY);
	gpio_fd2 = gpio_fd_open(gpio2, O_RDONLY);
	gpio_fd3 = gpio_fd_open(gpio3, O_RDONLY);
	gpio_fd4 = gpio_fd_open(gpio4, O_RDONLY);
	gpio_fd5 = gpio_fd_open(gpio5, O_RDONLY);
	
	int cycle = 0;
	int minValue = 999999;
	int minPosition = 0;
	int Sensor_avg = 0;
	
	struct pollfd fdset[2];
	//Wait for the start button to be pushed before we acutally start doing anything
	printf("Press the start button to begin the program \n");
	
	while(1){
		memset((void*)fdset, 0, sizeof(fdset));
		fdest[0].fd = STDIN_FILENO;
		fdset[0]events = POLLIN;
		fdset[1].fd = gpio_fd;
		fdset[1].events = POLLPRI;
		
		rc = poll(fdset, nfds, 3000);
		
		if(rc < 0){
			printf("Poll failed \n");
		}
		if(fdset[1].revents & POLLPRI){
			lseek(fdset[1].fd, 0, SEEK_SET);
			len = read(fdset[1].fd, buf, 64);
			problemOne(gpios, &pos);
		}
		if(fdset[0].revents & POLLIN){
			(void) read(fdset[0].fd, buf, 1);
		}
		
		fflush(stdout);
	}
	
	
	
	
	
	
	
	
	printf("Begining our steps to attempt to come up with ")
	for(cycle = 0; cycle <= MAX_CYCLES; cycle++){
		printf("One cycle number: %d", cycle);
		Sensor1_val = analogIn(AnalogIn1);
		Sensor2_val = analogIn(AnalogIn2);
		Sensor_avg = (Sensor1_val[cycle] + Sensor2_val[cycle])/ 2;
		if(Sensor_avg < minValue){
			minValue = Sensor_avg;
			minPosition = cycle;
		}
		//Should move the IR sensors here
	}
	
	if(minPosition <=10){
		//Rotate clockwise to position
	}
	else{
		//Rotate counterclockwise to position
	}
	
	while(1){
		Sensor1_val = analogIn(AnalogIn1);
		Sensor2_val = analogIn(AnalogIn2);
		if(Sensor1_val > Sensor2_val){
			//Rotate clockwise once
		}
		else{
			//Rotate counterclockwise 
		}
	}
	
	return 0;
}