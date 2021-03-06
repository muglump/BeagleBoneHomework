#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h> 
#include <signal.h>
#include <poll.h> 
#include "gpio-utils.h"
#include <math.h>

#define PIN_MUX_PATH "/sys/kernel/debug/omap_mux/"
#define MAX_CYCLES 20
#define ANALOG_INPUTS 2
#define MAX_BUF 64


int analogRead(char *analog){ 
	char analogpath[64], val[64];
	char buffer[1024];
	int fd, value;
	FILE *fp;
	
	snprintf(analogpath, sizeof analogpath, "/sys/devices/ocp.2/helper.14/%s", analog);
	//sprintf(analogpath, "/sys/devices/ocp.2/helper.14/%s", analog);
	
	if((fp = fopen(analogpath, "r")) == NULL){
		printf("Can't open this pin, %s\n", analog);
		return 1;
	}
	fgets(val, MAX_BUF, fp);

	fclose(fp);
	return atoi(val);	
	//fd = open(buffer, O_RDONLY); 
	//val = read(fd, buffer, sizeof(buffer)); 
	//value = atoi(buffer); 

	//close(fd); 
	
	//printf("val = %s, value = %d \n", val, value);
	//fflush(stdout);

	//return value;
}

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

int smallClockwiseRotation(unsigned int pins[], int pos){
	int checker = pos;
	printf("Enter clockwise, with pos = %d \n", checker);
	
	
	switch(checker){
		case(0):
			gpio_set_value(pins[0], 0);
			gpio_set_value(pins[1], 0);
			gpio_set_value(pins[2], 1);
			gpio_set_value(pins[3], 0);
			break;
		case(1):
			gpio_set_value(pins[0], 0);
			gpio_set_value(pins[1], 1);
			gpio_set_value(pins[2], 1);
			gpio_set_value(pins[3], 0);
			break;
		case(2):
			gpio_set_value(pins[0], 0);
			gpio_set_value(pins[1], 1);
			gpio_set_value(pins[2], 0);
			gpio_set_value(pins[3], 0);
			break;
		case(3):
			gpio_set_value(pins[0], 1);
			gpio_set_value(pins[1], 1);
			gpio_set_value(pins[2], 0);
			gpio_set_value(pins[3], 0);
			break;
		case(4):
			gpio_set_value(pins[0], 1);
			gpio_set_value(pins[1], 0);
			gpio_set_value(pins[2], 0);
			gpio_set_value(pins[3], 0);
			break;
		case(5):
			gpio_set_value(pins[0], 1);
			gpio_set_value(pins[1], 0);
			gpio_set_value(pins[2], 0);
			gpio_set_value(pins[3], 1);
			break;
		case(6):
			gpio_set_value(pins[0], 0);
			gpio_set_value(pins[1], 0);
			gpio_set_value(pins[2], 0);
			gpio_set_value(pins[3], 1);
			break;
		case(7):
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
	usleep(40000);
	return(checker+1)%8; 	
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

int smallCounterClockwiseRotation(unsigned int pins[], int pos){
	int checker = pos;
	printf("Enter counterclockwise, with pos = %d \n", checker);
	switch(checker){
		case(0):
			gpio_set_value(pins[0], 1);
			gpio_set_value(pins[1], 0);
			gpio_set_value(pins[2], 0);
			gpio_set_value(pins[3], 1);
			break;
		case(1):
			gpio_set_value(pins[0], 1);
			gpio_set_value(pins[1], 0);
			gpio_set_value(pins[2], 0);
			gpio_set_value(pins[3], 0);
			break;
		case(2):
			gpio_set_value(pins[0], 1);
			gpio_set_value(pins[1], 1);
			gpio_set_value(pins[2], 0);
			gpio_set_value(pins[3], 0);
			break;
		case(3):
			gpio_set_value(pins[0], 0);
			gpio_set_value(pins[1], 1);
			gpio_set_value(pins[2], 0);
			gpio_set_value(pins[3], 0);
			break;
		case(4):
			gpio_set_value(pins[0], 0);
			gpio_set_value(pins[1], 1);
			gpio_set_value(pins[2], 1);
			gpio_set_value(pins[3], 0);
			break;
		case(5):
			gpio_set_value(pins[0], 0);
			gpio_set_value(pins[1], 0);
			gpio_set_value(pins[2], 1);
			gpio_set_value(pins[3], 0);
			break;
		case(6):
			gpio_set_value(pins[0], 0);
			gpio_set_value(pins[1], 0);
			gpio_set_value(pins[2], 1);
			gpio_set_value(pins[3], 1);
			break;
		case(7):
			gpio_set_value(pins[0], 0);
			gpio_set_value(pins[1], 0);
			gpio_set_value(pins[2], 0);
			gpio_set_value(pins[3], 1);
			break;
	}
	usleep(40000);
	return (checker + 1)%8; 	
}

int problemOne(unsigned int pins[], int pos){
	printf("Entering Problem one with position %d\n", pos);
	int i =0;
	for(i = 0; i < 40; i++){
		pos = smallClockwiseRotation(pins, pos);
	}
	printf("After rotating clockwise pos is, %d \n", pos);
	for(i = 0; i < 40; i++){
		pos = smallCounterClockwiseRotation(pins, pos);
	}
	return pos;
}


int problemTwo(unsigned int pins[], char a0[], char a1[], int pos, int mode){
	printf("Entering Problem two with positions %d\n", pos);
	
	int values[40];
	int average, min, minpos; 
	char t0;
	min = 10000;
	
	int i = 0;
	for(i = 0; i < 40; i++){
		average = 0;
		average = (analogRead(a0) + analogRead(a1))/ANALOG_INPUTS;
		if(average < min){
			min = average;
			minpos = i;
			//printf("minpos delta is %d \n", minpos);
		}
		values[i] = average;
		printf("the current average was found to be %d \n", average);
		pos = smallClockwiseRotation(pins, pos);
	}

	printf("The minimum position was found at %d \n", minpos);
	
	pos = problemThree(pins, pos, minpos);
	
	problemFour(pins, pos, a0, a1, mode);
	return pos;

}

int problemThree(unsigned int pins[], int pos, int min){
	printf("problem three triggered");
	int i = 0;
	if(min < 20){ //rotate clockwise
		for(i = 0; i < min; i++){
			pos = smallClockwiseRotation(pins, pos);
		}
	}
	else{ //rotate counterclockwise
		for(i = 0; i < 40-min; i++){
			pos = smallCounterClockwiseRotation(pins, pos);
		}
	}
	
	return pos;
}

int problemFour(int pins[], int pos, char a0[], char a1[], int mode){
	int v0, v1; //analog inputs
	int delta = 0; //minimum acceptable difference 100
	
	while(1){
		v0 = analogRead(a0);
		v1 = analogRead(a1);
		delta = v0 - v1;
		if (mode == 1){
			delta = -delta;
		}
		//printf("The delta is: %d \n", delta);
		if (abs(delta) > 300){
			if (delta < 0){
				pos = smallClockwiseRotation(pins, pos);
			}
			if (delta > 0){
				pos = smallCounterClockwiseRotation(pins, pos);
			}
		
		}
		//usleep(1000000);
	
	}


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
	//char analogs[] = {AnalogIn1, AnalogIn2};
	//printf("checking init analogs: %s \n", analogs[0]);
	char buf[64];
	//Arrays to hold the values we get back from the sensors
	struct pollfd fdset[2];
	int Sensor1_val;
	int Sensor2_val;
	//int Sensor_sum;
	
	int mode = atoi(argv[1]);
	
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
			//pos = problemOne(gpios, pos);
			pos = problemTwo(gpios, AnalogIn1, AnalogIn2, pos, mode);
			//break;
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
	
	printf("%d\n", minValue);
	
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
