
/*
    Minimal version for teaching.  Mark A. Yoder 26-July-2011
    i2cget.c - A user-space program to read an I2C register.
    Copyright (C) 2005-2010  Jean Delvare <khali@linux-fr.org>

    Based on i2cset.c:
    Copyright (C) 2001-2003  Frodo Looijaard <frodol@dds.nl>, and
                             Mark D. Studebaker <mdsxyz123@yahoo.com>
    Copyright (C) 2004-2005  Jean Delvare <khali@linux-fr.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA 02110-1301 USA.
*/
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>	
#include "gpio-utils.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "i2c-dev.h"
#include <fcntl.h>

#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64

int main(int argc, char *argv[])
{
	struct pollfd fdset[3];
	int nfds = 3;
	char *end;
	int res1, res2, i2cbus1, i2cbus2, address1, address2, size, file1, file2, rc, len;
	int daddress1, daddress2;
	char filename1[20];
	char filename2[20];
	char buf[MAX_BUF];
	int gpio_fd1, gpio_fd2;
	unsigned int gpio1, gpio2;
	/* handle (optional) flags first */
	if(argc < 3) {
		fprintf(stderr, "Usage:  %s <i2c-bus> <i2c-address> <register>\n", argv[0]);
		exit(1);
	}

	i2cbus1 = atoi(argv[1]);
	i2cbus2 = atoi(argv[2]);
	address1 = atoi(argv[3]);
	address2  = atoi(argv[4]);
	daddress1 = atoi(argv[5]);
	daddress2 = atoi(argv[6]);
	gpio1 = atoi(argv[7]);
	gpio2 = atoi(argv[8]);
	size = I2C_SMBUS_BYTE;
	gpio_export(gpio1);
	gpio_export(gpio2);
	gpio_set_dir(gpio1, "in");
	gpio_set_dir(gpio2, "in");
	gpio_set_edge(gpio1, "falling");
	gpio_set_edge(gpio2, "falling");
	gpio_fd1 = gpio_fd_open(gpio1, O_RDONLY);
	gpio_fd2 = gpio_fd_open(gpio2, O_RDONLY);
	sprintf(filename1, "/dev/i2c-%d", i2cbus1);
	sprintf(filename2, "/dev/i2c-%d", i2cbus2);

	char buff1[50];
	char buff2[50];
	char buff3[50];
	char buff4[50];
	sprintf(buff1, " i2cset -y %d %d %d %d", 1, address1, 2, 27);
	sprintf(buff2, " i2cset -y %d %d %d %d", 1, address2, 2, 27);
	sprintf(buff3, " i2cset -y %d %d %d %d", 1, address1, 3, 27);
	sprintf(buff4, " i2cset -y %d %d %d %d", 1, address2, 3, 27);
	system(buff1);
	system(buff2);
	system(buff3);
	system(buff4);
	file1 = open(filename1, O_RDWR);
	file2 = open(filename2, O_RDWR);
	if (file1<0) {
		if (errno == ENOENT) {
			fprintf(stderr, "Error: Could not open file "
				"/dev/i2c-%d: %s\n", i2cbus1, strerror(ENOENT));
		} else {
			fprintf(stderr, "Error: Could not open file "
				"`%s': %s\n", filename1, strerror(errno));
			if (errno == EACCES)
				fprintf(stderr, "Run as root?\n");
		}
		exit(1);
	}

	if (file2<0) {
		if (errno == ENOENT) {
			fprintf(stderr, "Error: Could not open file "
				"/dev/i2c-%d: %s\n", i2cbus2, strerror(ENOENT));
		} else {
			fprintf(stderr, "Error: Could not open file "
				"`%s': %s\n", filename2, strerror(errno));
			if (errno == EACCES)
				fprintf(stderr, "Run as root?\n");
		}
		exit(1);
	}

	if (ioctl(file1, I2C_SLAVE, address1) < 0) {
		fprintf(stderr,
			"Error: Could not set address to 0x%02x: %s\n",
			address1, strerror(errno));
		return -errno;
	}

	if (ioctl(file2, I2C_SLAVE, address2) < 0) {
		fprintf(stderr,
			"Error: Could not set address to 0x%02x: %s\n",
			address2, strerror(errno));
		return -errno;
	}
/*
	res = i2c_smbus_write_byte(file, daddress);
	if (res < 0) {
		fprintf(stderr, "Warning - write failed, filename=%s, daddress=%d\n",
			filename, daddress);
	}
*/
	res1 = i2c_smbus_read_byte_data(file1, 0);
	res2 = i2c_smbus_read_byte_data(file2, 0);
	

	if (res1 < 0) {
		fprintf(stderr, "Error: Read failed, res=%d\n", res1);
		exit(2);
	}

	if(res2 < 0) {
		fprintf(stderr, "Error: Read failed, res=%d\n", res2);
		exit(2);
	}

	printf("0x%02x (%d)\n", res1, res1);
	printf("0x%02x (%d)\n", res2, res2);
	printf("F1: %f \n", ((res1  * (9.0/5.0)) + 32));
	printf("F2: %f \n", ((res2  * (9.0/5.0)) + 32));
	
	while (1) {
		
		memset((void*)fdset, 0, sizeof(fdset));

		fdset[0].fd = STDIN_FILENO;
		fdset[0].events = POLLIN;
      
		fdset[1].fd = gpio_fd1;
		fdset[1].events = POLLPRI;

		fdset[2].fd = gpio_fd2;
		fdset[2].events = POLLPRI;
		

		rc = poll(fdset, nfds, 1);      

		if (rc < 0) {
			printf("\npoll() failed!\n");
			return -1;
		}
      
		int i = 1;
            	for(i; i <= 2; i++){
			if (fdset[i].revents & POLLPRI) {
				lseek(fdset[i].fd, 0, SEEK_SET);  // Read from the start of the file
				len = read(fdset[i].fd, buf, MAX_BUF);
				
				if( i == 1){
					
					res1 = i2c_smbus_read_byte_data(file1, 0);
					printf("F3: %f \n", ((res1  * (9.0/5.0)) + 32));
	
				}
				if(i == 2){
					
					res2 = i2c_smbus_read_byte_data(file2, 0);
					
					printf("F4: %f \n", ((res2  * (9.0/5.0)) + 32));
				}
			}
		}


		if (fdset[0].revents & POLLIN) {
			(void)read(fdset[0].fd, buf, 1);
			printf("\npoll() stdin read 0x%2.2X\n", (unsigned int) buf[0]);
		}

		fflush(stdout);
	}
	close(file1);
	close(file2);
	exit(0);
}
