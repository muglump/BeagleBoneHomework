#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>	
#include "gpio-utils.h"
 
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64

int keepgoing = 1;	


void signal_handler(int sig);

void signal_handler(int sig)
{
	printf( "Ctrl-C pressed, cleaning up and exiting..\n" );
	keepgoing = 0;
}
int main(int argc, char **argv, char **envp)
{
	struct pollfd fdset[5];
	int nfds = 5;
	int gpio_fd, gpio_fd2, gpio_fd3, gpio_fd4, gpio_fd5, gpio_fd6, gpio_fd7, gpio_fd8, timeout, rc;
	char buf[MAX_BUF];
	unsigned int sw1gpio, sw2gpio, sw3gpio, sw4gpio, led1gpio, led2gpio, led3gpio, led4gpio;
	int len;

	if (argc < 9) {
		printf("Usage: gpio-int <gpio-pin>\n\n");
		printf("Waits for a change in the GPIO pin voltage level or input on stdin\n");
		exit(-1);
	}

	
	signal(SIGINT, signal_handler);

	//Gets the gpio numbers for the switches
	sw1gpio = atoi(argv[1]);
	sw2gpio = atoi(argv[2]);
	sw3gpio = atoi(argv[3]);
	sw4gpio = atoi(argv[4]);
	
	
	//Gets the gpio numbers for the leds
	led1gpio = atoi(argv[5]);
	led2gpio = atoi(argv[6]);
	led3gpio = atoi(argv[7]);
	led4gpio = atoi(argv[8]);
	
	int* gpios = (int *)malloc(4*sizeof(int));
	
	gpios[0] = led1gpio;
	gpios[1] = led2gpio;
	gpios[2] = led3gpio;
	gpios[3] = led4gpio;

	//Exporting gpio
	printf("exporting gpios %d, %d, %d, %d, %d, %d ,%d, %d", sw1gpio, sw2gpio, sw3gpio, sw4gpio, led1gpio, led2gpio, led3gpio, led4gpio);
	gpio_export(sw1gpio);
	gpio_export(sw2gpio);
	gpio_export(sw3gpio);
	gpio_export(sw4gpio);
	gpio_export(led1gpio);
	gpio_export(led2gpio);
	gpio_export(led3gpio);
	gpio_export(led4gpio);
	
	//Setting the switch gpio to in and led to out
	gpio_set_dir(sw1gpio, "in");
	gpio_set_dir(sw2gpio, "in");
	gpio_set_dir(sw3gpio, "in");
	gpio_set_dir(sw4gpio, "in");
	gpio_set_dir(led1gpio, "out");
	gpio_set_dir(led2gpio, "out");
	gpio_set_dir(led3gpio, "out");
	gpio_set_dir(led4gpio, "out");

	//Setting the edge
	gpio_set_edge(sw1gpio, "both");
	gpio_set_edge(sw2gpio, "both"); 
	gpio_set_edge(sw3gpio, "both"); 
	gpio_set_edge(sw4gpio, "both"); 
	gpio_set_edge(led1gpio, "both"); 
	gpio_set_edge(led2gpio, "both"); 
	gpio_set_edge(led3gpio, "both"); 
	gpio_set_edge(led4gpio, "both");
	
	//Opening the gpio files
	gpio_fd = gpio_fd_open(sw1gpio, O_RDONLY);
	gpio_fd2 = gpio_fd_open(sw2gpio, O_RDONLY);
	gpio_fd3 = gpio_fd_open(sw3gpio, O_RDONLY);
	gpio_fd4 = gpio_fd_open(sw4gpio, O_RDONLY);
	gpio_fd5 = gpio_fd_open(led1gpio, O_RDONLY);
	gpio_fd6 = gpio_fd_open(led2gpio, O_RDONLY);
	gpio_fd7 = gpio_fd_open(led3gpio, O_RDONLY);
	gpio_fd8 = gpio_fd_open(led4gpio, O_RDONLY);

	timeout = POLL_TIMEOUT;
	//Setting up some toggles
	int toggle1 = 0;
	int toggle2 = 0;
	int toggle3 = 0;
	int toggle4 = 0;
	//Initliazes a set of toggles for the LEDS
	int *toggles = (int *)malloc(4*sizeof(int));
	toggles[0] = toggle1;
	toggles[1] = toggle2;
	toggles[2] = toggle3;
	toggles[3] = toggle4;
	//Loop until we are told to quit
	while (keepgoing) {
		
		memset((void*)fdset, 0, sizeof(fdset));

		fdset[0].fd = STDIN_FILENO;
		fdset[0].events = POLLIN;
      
		fdset[1].fd = gpio_fd;
		fdset[1].events = POLLPRI;

		fdset[2].fd = gpio_fd2;
		fdset[2].events = POLLPRI;
		
		fdset[3].fd = gpio_fd3;
		fdset[3].events = POLLPRI;
		
		fdset[4].fd = gpio_fd4;
		fdset[4].events = POLLPRI;

		rc = poll(fdset, nfds, timeout);      

		if (rc < 0) {
			printf("\npoll() failed!\n");
			return -1;
		}
      
		if (rc == 0) {
			printf(".");
		}
		//Loop over the switch gpios, if they interrupt switch their respective leds values
		int i = 1;
            	for(i; i <= 4; i++){
			if (fdset[i].revents & POLLPRI) {
				lseek(fdset[i].fd, 0, SEEK_SET);  // Read from the start of the file
				len = read(fdset[i].fd, buf, MAX_BUF);
				toggles[i - 1] = !toggles[i - 1];
				gpio_set_value(gpios[i-1], toggles[i-1]);
				printf(" Triggering %d \n", gpios[i - 1]);
			}
		}


		if (fdset[0].revents & POLLIN) {
			(void)read(fdset[0].fd, buf, 1);
			printf("\npoll() stdin read 0x%2.2X\n", (unsigned int) buf[0]);
		}

		fflush(stdout);
	}
	//close the gpios that we have used
	gpio_fd_close(gpio_fd);
	gpio_fd_close(gpio_fd2);
	gpio_fd_close(gpio_fd3);
	gpio_fd_close(gpio_fd4);
	gpio_fd_close(gpio_fd5);
	gpio_fd_close(gpio_fd6);
	gpio_fd_close(gpio_fd7);
	gpio_fd_close(gpio_fd8);
	
	return 0;
}

