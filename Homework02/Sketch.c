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
	
	int xbound = atoi(argv[1]);
	int ybound = atoi(argv[2]);
	
	struct pollfd fdset[7];
	int nfds = 7;
	int gpio_fd1, gpio_fd2, gpio_fd3, gpio_fd4, gpio_fd5, gpio_fd6, timeout, rc;


	
	
	char buf[MAX_BUF];
	unsigned int gpio1, gpio2, gpio3, gpio4, gpio5, gpio6, gpio7, gpio8;
	int len;

	if (argc < 2) {
		printf("Usage: gpio-int <gpio-pin>\n\n");
		printf("Waits for a change in the GPIO pin voltage level or input on stdin\n");
		exit(-1);
	}

	// Set the signal callback for Ctrl-C
	signal(SIGINT, signal_handler);
		

	gpio1 = atoi(argv[5]);
	gpio2 = atoi(argv[6]);
	gpio3 = atoi(argv[7]);
	gpio4 = atoi(argv[8]);
	gpio5 = atoi(argv[9]);
	gpio6 = atoi(argv[10]);
	gpio_export(gpio1);
	gpio_export(gpio2);
	gpio_export(gpio3);
	gpio_export(gpio4);
	gpio_export(gpio5);
	gpio_export(gpio6);
	gpio_set_dir(gpio1, "in");
	gpio_set_dir(gpio2, "in");
	gpio_set_dir(gpio3, "in");
	gpio_set_dir(gpio4, "in");
	gpio_set_dir(gpio5, "in");
	gpio_set_dir(gpio6, "in");
	gpio_set_edge(gpio1, "rising");
	gpio_set_edge(gpio2, "rising");
	gpio_set_edge(gpio3, "rising");
	gpio_set_edge(gpio4, "rising");
	gpio_set_edge(gpio5, "rising");
	gpio_set_edge(gpio6, "rising");
	gpio_fd1 = gpio_fd_open(gpio1, O_RDONLY);
	gpio_fd2 = gpio_fd_open(gpio2, O_RDONLY);
	gpio_fd3 = gpio_fd_open(gpio3, O_RDONLY);
	gpio_fd4 = gpio_fd_open(gpio4, O_RDONLY);
	gpio_fd5 = gpio_fd_open(gpio5, O_RDONLY);
	gpio_fd6 = gpio_fd_open(gpio6, O_RDONLY);

	timeout = POLL_TIMEOUT;
	
	int erase = 1;
	
	char **a;
	a = (char **) malloc(xbound*sizeof(char *));
	int t = 0;
	for(t; t <= atoi(argv[1]) ; t++){
		a[t] = (char *) malloc(ybound*sizeof(char));
	}
	
	int r = 0;
	int s = 0;
	for(r; r <= xbound; r++)
	{
		s = 0;
		for(s; s <= ybound; s++)
		{
			
			a[r][s] = 'a';
			printf("%c", a[r][s]);
			
		}
		printf("\n");
		
	}
	
	
	int posx = atoi(argv[3]);
	int posy = atoi(argv[4]);
	a[posx][posy] = 'X';
 
	while (keepgoing) {
		memset((void*)fdset, 0, sizeof(fdset));

		fdset[0].fd = STDIN_FILENO;
		fdset[0].events = POLLIN;
      
		fdset[1].fd = gpio_fd1;
		fdset[1].events = POLLPRI;
		
		fdset[2].fd = gpio_fd2;
		fdset[2].events = POLLPRI;
		
		fdset[3].fd = gpio_fd3;
		fdset[3].events = POLLPRI;
		
		fdset[4].fd = gpio_fd4;
		fdset[4].events = POLLPRI;
		
		fdset[5].fd = gpio_fd5;
		fdset[5].events = POLLPRI;
		
		fdset[6].fd = gpio_fd6;
		fdset[6].events = POLLPRI;
		

		rc = poll(fdset, nfds, timeout);      

		if (rc < 0) {
			printf("\npoll() failed!\n");
			return -1;
		}
      
		
            	int i = 1;
		for( i; i <= 6; i++){
			if (fdset[i].revents & POLLPRI) {
				lseek(fdset[i].fd, 0, SEEK_SET);  // Read from the start of the file
				len = read(fdset[i].fd, buf, MAX_BUF);
				
				if(i == 5)
				{
					printf("Switching draw modes \n");
					erase = !erase;
				}
				else if(i == 6)
				{
					system(" clear ");
					r = 0;
					s = 0;
					for(r; r <= atoi(argv[1]); r++)
					{
						s = 0;
						for(s; s <= atoi(argv[2]); s++)
						{
							a[r][s] = 'a';
							printf("%c", a[r][s]);
						}
						printf("\n");
					}
					
				}
				else if(i == 1)
				{
					system(" clear ");
					if(posx == xbound)
					{
						
						printf("Cant go past bounds \n");
						r = 0;
						s = 0;
						for(r; r <= atoi(argv[1]); r++)
						{
							s = 0;
							for(s; s <= atoi(argv[2]); s++)
							{
								printf("%c", a[r][s]);
							}
							printf("\n");
						}
					}
					else
					{
						r = 0;
						s = 0;
						posx +=1;
						if(erase == 0)
						{
							a[posx][posy] = 'X';
						}
						else{
							a[posx][posy] = 'a';
						}
						for(r; r <= atoi(argv[1]); r++)
						{
							s = 0;
							for(s; s <= atoi(argv[2]); s++)
							{
								
								printf("%c", a[r][s]);
							}
							printf("\n");
						}
					}
				}
				else if(i == 2)
				{
					system(" clear ");
					if(posx == 0)
					{
						
						printf("Cant go past bounds \n");
						r = 0;
						s = 0;
						for(r; r <= atoi(argv[1]); r++)
						{
							s =0;
							for(s; s <= atoi(argv[2]); s++)
							{
								
								printf("%c", a[r][s]);
							}
							printf("\n");
						}
					}
					else
					{
						r = 0;
						s = 0;
						posx -=1;
						if(erase == 0)
						{
							a[posx][posy] = 'X';
						}
						else
						{
							a[posx][posy] = 'a';
						}
						
						for(r; r <= atoi(argv[1]); r++)
						{
							s = 0;
							for(s; s <= atoi(argv[2]); s++)
							{
								
								printf("%c", a[r][s]);
							}
							printf("\n");
						}
					}
				}
				else if(i == 3)
				{
					system( "clear");
					if(posy == ybound)
					{
						
						printf("Cant go past bounds \n");
						r = 0;
						s = 0;
						for(r; r <= atoi(argv[1]); r++)
						{
							s = 0;
							for(s; s <= atoi(argv[2]); s++)
							{
								
								printf("%c", a[r][s]);
							}
							printf("\n");
						}
					}
					else
					{
						r = 0;
						s = 0;
						posy +=1;
						if(erase == 0)
						{
							a[posx][posy] = 'X';
						}
						else
						{
							a[posx][posy] = 'a';
						}
						
						for(r; r <= atoi(argv[1]); r++)
						{
							s= 0;
							for(s; s <= atoi(argv[2]); s++)
							{
								
								printf("%c", a[r][s]);
							}
							printf("\n");
						}
					}
				}
				else if( i == 4)
				{
					system(" clear ");
					if(posy == 0)
					{
						
						printf("Cant go past bounds \n");
						r = 0;
						s = 0;
						for(r; r <= atoi(argv[1]); r++)
						{
							s = 0;
							for(s; s <= atoi(argv[2]); s++)
							{
								
								printf("%c", a[r][s]);
							}
							printf("\n");
						}
					}
					else
					{
						r = 0;
						s = 0;
						posy -=1;
						if(erase == 0)
						{
							a[posx][posy] = 'X';
						}
						else
						{
							a[posx][posy] = 'a';
						}
						for(r; r <= atoi(argv[1]); r++)
						{
							s = 0;
							for(s; s <= atoi(argv[2]); s++)
							{
								
								printf("%c", a[r][s]);
							}
							printf("\n");
						}
					}
				}
				else{
					continue;
				}
				
			}
		}

		if (fdset[0].revents & POLLIN) {
			(void)read(fdset[0].fd, buf, 1);
			printf("\npoll() stdin read 0x%2.2X\n", (unsigned int) buf[0]);
		}

		fflush(stdout);
	}

	gpio_fd_close(gpio_fd1);
	gpio_fd_close(gpio_fd2);
	gpio_fd_close(gpio_fd3);
	gpio_fd_close(gpio_fd4);
	return 0;
}

