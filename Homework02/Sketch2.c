#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv)
{
	int xbound = atoi(argv[1]);
	int ybound = atoi(argv[2]);
	int xpos = atoi(argv[3]);
	int ypos = atoi(argv[4]);
	
	
	char **a;
	a = (char **) malloc(xbound*sizeof(char *));
	int t = 0;
	for(t; t < xbound ; t++){
		a[t] = (char *) malloc(ybound*sizeof(char));
	}
	
	int r = 0;
	int s = 0;
	for(r; r < xbound; r++)
	{
		s = 0;
		for(s; s < ybound; s++)
		{
			
			a[r][s] = '-';
		}
	}
	char writeChar = 'x';
	char input;
	
	while(1)
	{
		system(" clear ");
		printf("Press w to go up, s down, a left, d right, c to clear, r to toggle write mod, and q to quit, after pressing the character hit enter \n");
		r = 0;
		s = 0;
		for(r; r < xbound; r++)
		{
			s = 0;
			for(s; s < ybound; s++)
			{
			
			
			printf("%c", a[r][s]);
			
			}
			printf("\n");
		
		}
		scanf("%c", &input);
		if(input == 'w')
		{
			xpos -= 1;
			a[xpos][ypos] = writeChar;
			continue;
			
		}
		if(input == 's')
		{
			xpos += 1;
			a[xpos][ypos] = writeChar;
			continue;
		}
		if(input == 'a')
		{
			ypos -= 1;
			a[xpos][ypos] = writeChar;
			continue;
		}
		if(input == 'd')
		{
			ypos += 1;
			a[xpos][ypos] = writeChar;
			continue;
		}
		if(input == 'c')
		{
			for(r; r < xbound; r++)
			{
				s = 0;
				for(s; s < ybound; s++)
				{
					a[r][s] = '-';
				}
			}
			continue;
		}
		if(input == 'r')
		{
			if(writeChar == '-')
			{
				writeChar = 'x';
			}
			else
			{
				writeChar = '-';
			}
			continue;
		}
		if(input == 'q')
		{
			break;
		}
	}
	return 0;
}
