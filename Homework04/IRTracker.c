#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h> 
#include <signal.h>
#include <poll.h> 
#include "gpio-utils.h"

#define PIN_MUX_PATH "/sys/kernel/debug/omap_mux/"
#define MAX_CYCLES 20

int clockwiseRotation(unsigned int pins[], int pos){
	int checker = pos;
	printf("Enter clockwise, with pos = %d \n", checker);
	
	
	switch(checker){
		case(0):
			printf("Case 1 \n");
			gpio_set_value(pins[0], 0);
			gpio_set_value(pins[1], 1);
			gpio_set_value(pins[2], 1);
			gpio_set_value(pins[3], 0);
			break;
		case(1):
			printf("Case 2 \n");
			gpio_set_value(pins[0], 1);
			gpio_set_value(pins[1], 1);
			gpio_set_value(pins[2], 0);
			gpio_set_value(pins[3], 0);
			break;
		case(2):
			printf("Case 3 \n");
			gpio_set_value(pins[0], 1);
			gpio_set_value(pins[1], 0);
			gpio_set_value(pins[2], 0);
			gpio_set_value(pins[3], 1);
			break;
		case(3):
			printf("Case 4 \n");
			gpio_set_value(pins[0], 0);
			gpio_set_value(pins[1], 0);
			gpio_set_value(pins[2], 1);
			gpio_set_value(pins[3], 1);
			break;
		default:
			printf("Fell \n");
			break;
		

	}
	printf("Reached the botton, value: %d\n", checker);
	sleep(1);
	return(checker+1)%4; 	
}

int counterClockwiseRotation(unsigned int pins[], int pos){
	int checker = pos;
	printf("Enter counterclockwise, with pos = %d \n", checker);
	switch(checker){
		case(0):
			printf("Case 1B \n");
			gpio_set_value(pins[0], 1);
			gpio_set_value(pins[1], 0);
			gpio_set_value(pins[2], 0);
			gpio_set_value(pins[3], 1);
			break;
		case(1):
			printf("Case 2B \n");
			gpio_set_value(pins[0], 1);
			gpio_set_value(pins[1], 1);
			gpio_set_value(pins[2], 0);
			gpio_set_value(pins[3], 0);
			break;
		case(2):
			printf("Case 3B \n");
			gpio_set_value(pins[0], 0);
			gpio_set_value(pins[1], 1);
			gpio_set_value(pins[2], 1);
			gpio_set_value(pins[3], 0);
			break;
		case(3):
			printf("Case 4B \n");
			gpio_set_value(pins[0], 0);
			gpio_set_value(pins[1], 0);
			gpio_set_value(pins[2], 1);
			gpio_set_value(pins[3], 1);
			break;
	}
	sleep(1);
	return (checker + 1)%4; 	
}


int problemOne(unsigned int pins[], int pos){
	printf("Entering Problem one with position %d\n", pos);
	int i =0;
	for(i = 0; i < 20; i++){
		pos = clockwiseRotation(pins, pos);
	}
	printf("After rotating clockwise pos is, %d \n", pos);
	for(i = 0; i < 20; i++){
		pos = counterClockwiseRotation(pins, pos);
	}
	return pos;
}


int main(int argc, char *argv[]){
	//The gpio port numbers needed to drive the motor
	unsigned int gpio1 = 30;
	unsigned int gpio2 = 31;
	unsigned int gpio3 = 48;
	unsigned int gpio4 = 51;
	unsigned int gpio5 = 15;
	unsigned int gpios[4] = {gpio1, gpio2, gpio3, gpio4};
	int gpio_fd1, gpio_fd2, gpio_fd3, gpio_fd4, gpio_fd5;
	int behavior = atoi(argv[1]);
	int rc;
	int len;
	int nfds = 2;
	//Port numbers for the analog in
	char AnalogIn1[] = "AIN0";
	char AnalogIn2[] = "AIN1";
	char buf[64];
	//Arrays to hold the values we get back from the sensors
	struct pollfd fdset[2];
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
	
	gpio_set_edge(gpio5, "rising");
	
	gpio_fd1 = gpio_fd_open(gpio1, O_RDONLY);
	gpio_fd2 = gpio_fd_open(gpio2, O_RDONLY);
	gpio_fd3 = gpio_fd_open(gpio3, O_RDONLY);
	gpio_fd4 = gpio_fd_open(gpio4, O_RDONLY);
	gpio_fd5 = gpio_fd_open(gpio5, O_RDONLY);
	
	int cycle = 0;
	int minValue = 999999;
	int minPosition = 0;
	int Sensor_avg = 0;
	
	
	//Wait for the start button to be pushed before we acutally start doing anything
	printf("Press the start button to begin the program \n");
	len = read(gpio_fd5, buf, 64);
	while(1){
		memset((void*)fdset, 0, sizeof(fdset));
		fdset[0].fd = STDIN_FILENO;
		fdset[0].events = POLLIN;
		fdset[1].fd = gpio_fd5;
		fdset[1].events = POLLPRI;
		
		rc = poll(fdset, nfds, 3000);
		
		if(rc < 0){
			printf("Poll failed \n");
		}
		if(fdset[1].revents & POLLPRI){
			lseek(fdset[1].fd, 0, SEEK_SET);
			len = read(fdset[1].fd, buf, 64);
			printf("Triggered \n");
			pos = problemOne(gpios, pos);
		}
		
		
	}
	
	
	
	
	
	
	
	
	printf("Begining our steps to attempt to come up with ");
	for(cycle = 0; cycle <= MAX_CYCLES; cycle++){
		printf("One cycle number: %d", cycle);
		Sensor1_val = analogIn(AnalogIn1);
		Sensor2_val = analogIn(AnalogIn2);
		Sensor_avg = (Sensor1_val + Sensor2_val)/ 2;
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