/*
    my2cset.c - First try at controlling Adafruit 8x8matrix.
    Mark A. Yoder, 14-Aug-2012.
    Mark A. Yoder, 26-Oct-2012.  Cleaned up.
    Page numbers are from the HT16K33 manual
    http://www.adafruit.com/datasheets/ht16K33v110.pdf
*/
/*
    i2cset.c - A user-space program to write an I2C register.
    Copyright (C) 2001-2003  Frodo Looijaard <frodol@dds.nl>, and
                             Mark D. Studebaker <mdsxyz123@yahoo.com>
    Copyright (C) 2004-2010  Jean Delvare <khali@linux-fr.org>

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



#include <fcntl.h>
#include <poll.h>
#include <signal.h>	
#include "gpio-utils.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "i2c-dev.h"
#include "i2cbusses.h"

#define BICOLOR		// undef if using a single color display

// The upper btye is RED, the lower is GREEN.
// The single color display responds only to the lower byte
static __u16 smile_bmp[]=
	{0x3c, 0x42, 0xa9, 0x85, 0x85, 0xa9, 0x42, 0x3c};
static __u16 frown_bmp[]=
	{0x3c00, 0x4200, 0xa900, 0x8500, 0x8500, 0xa900, 0x4200, 0x3c00};
static __u16 neutral_bmp[]=
	{0x3c3c, 0x4242, 0xa9a9, 0x8989, 0x8989, 0xa9a9, 0x4242, 0x3c3c};
static __u16 test[] = {};

static void help(void) __attribute__ ((noreturn));

static void help(void) {
	fprintf(stderr, "Usage: matrixLEDi2c (hardwired to bus 3, address 0x70)\n");
	exit(1);
}

static int check_funcs(int file) {
	unsigned long funcs;

	/* check adapter functionality */
	if (ioctl(file, I2C_FUNCS, &funcs) < 0) {
		fprintf(stderr, "Error: Could not get the adapter "
			"functionality matrix: %s\n", strerror(errno));
		return -1;
	}

	if (!(funcs & I2C_FUNC_SMBUS_WRITE_BYTE)) {
		fprintf(stderr, MISSING_FUNC_FMT, "SMBus send byte");
		return -1;
	}
	return 0;
}

// Writes block of data to the display
static int write_block(int file, __u16 *data) {
	int res;
#ifdef BICOLOR
	res = i2c_smbus_write_i2c_block_data(file, 0x00, 16, 
		(__u8 *)data);
	return res;
#else
/*
 * For some reason the single color display is rotated one column, 
 * so pre-unrotate the data.
 */
	int i;
	__u16 block[I2C_SMBUS_BLOCK_MAX];
//	printf("rotating\n");
	for(i=0; i<8; i++) {
		block[i] = (data[i]&0xfe) >> 1 | 
			   (data[i]&0x01) << 7;
	}
	res = i2c_smbus_write_i2c_block_data(file, 0x00, 16, 
		(__u8 *)block);
	return res;
#endif
}

int main(int argc, char *argv[])
{
	int res, i2cbus, address, file;
	char filename[20];
	int force = 0;
	
	struct pollfd fdset[7];
	int nfds = 7;
	int gpio_fd1, gpio_fd2, gpio_fd3, gpio_fd4, gpio_fd5, gpio_fd6, timeout, rc, address1, address2, size, file1, file2, daddress1, daddress2;
	char filename1[20];
	char filename2[20];
	
	char buf[MAX_BUF];
	unsigned int gpio1, gpio2, gpio3, gpio4, gpio5, gpio6;
	int len;
	
	
	gpio1 = atoi(argv[1]);
	gpio2 = atoi(argv[2]);
	gpio3 = atoi(argv[3]);
	gpio4 = atoi(argv[4]);
	gpio5 = atoi(argv[5]);
	gpio6 = atoi(argv[6]);
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
	address1 = atoi(argv[7]);
	address2  = atoi(argv[8]);
	

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
	i2cbus = lookup_i2c_bus("1");
	printf("i2cbus = %d\n", i2cbus);
	if (i2cbus < 0)
		help();

	address = parse_i2c_address("0x70");
	printf("address = 0x%2x\n", address);
	if (address < 0)
		help();

	file = open_i2c_dev(i2cbus, filename, sizeof(filename), 0);
//	printf("file = %d\n", file);
	if (file < 0
	 || check_funcs(file)
	 || set_slave_addr(file, address, force))
		exit(1);

	// Check the return value on these if there is trouble
	

//	Display a series of pictures
	int **a;
	a = (int **) malloc(8*sizeof(int *));
	int t = 0;
	int x = 0;
	int y = 0;
	for(t; t <= 8 ; t++){
		a[t] = (int *) malloc(8*sizeof(int));
	}
	for(x = 0; x <=7; x++){
		for(y = 0; y <= 7; y++){
			a[x][y] = 0;
			
		}
		
	}
	a[0][0] = 0;
	
	int row = 0;
	int column = 0;
	int posx = 0;
	int posy = 0;	
	char input;
	int writeMode = 0;
	while(1){
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
				if ( i == 1){
					if(row < 7){
						row += 1;
						a[row][column] = writeMode;
						posx += 1;
					}
				}
				if (i == 2) {
					if(row > 0){
						row -= 1;
						a[row][column] = writeMode;
						posx -= 1;
					}
				}
				if (i == 4){
					if(column > 0){
						column -= 1;
						a[row][column] = writeMode;
						posy -= 1;
					}
				}
				if (i == 3){
					if(column < 7){
						column += 1;
						a[row][column] = writeMode;
						posy += 1;
					}
				}
				if (i == 5){ 
					
					for(x = 0; x < 8; ++x){
						for(y = 0; y < 8; ++y){
							a[x][y]= 0;
						}
					}
					
				}
				if(i == 6){
					
					if(writeMode == 1){
						writeMode = 0;
					}
					else{
						writeMode = 1;
					}
					
				}
			}
		}
		
		
		static int const BoardSize = 8;
		__u16 *block = (__u16 *) calloc(BoardSize, sizeof(__u16));
		for (x = 0; x < BoardSize; ++x)
		{
			for (y = 0; y < BoardSize; ++y)
			{
				
				if(a[x][y] == 1){
					
				 	block[x] = block[x] |  (1 << y);	
				}
				if(a[x][y]  == 0){
					block[x] = block[x] | (0 << y);
				}
				if(x == posx && y == posy){
					block[x] = block[x] | ( 1 << y + 8);
				}
				
			}
			
		}
		
		write_block(file, block);
		free(block);
	}
// Fade the display
	int daddress;
	for(daddress = 0xef; daddress >= 0xe0; daddress--) {
//	    printf("writing: 0x%02x\n", daddress);
	    res = i2c_smbus_write_byte(file, daddress);
	    usleep(100000);	// Sleep 0.1 seconds
	}

	if (res < 0) {
		fprintf(stderr, "Error: Write failed\n");
		close(file);
		exit(1);
	}
	exit(0);
}
