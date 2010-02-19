//# include "cpumon.h"
#include <stdio.h>
#include <string.h>

#define BUFSIZE 16
#define TENMIN 1
#define FIVEMIN 2
#define ONEMIN 3


/*                         variables
*  
*/
FILE * f;
char cpuload[BUFSIZE];
int tenMin;
int fiveMin;
int oneMin;

/*                         loadFile
*  returns a file given an input string
*/
FILE * getLoadFile(){
	
	return fopen("/proc/loadavg","r");
}

/*                         getCpuload
*  returns the average cpu load for the last 10 min, 5 min, and 1 min
*/
int getCpuLoad(FILE *f, int t){
	float load;
	char temp[4];
	if(f == NULL){
		printf("file not found\n");
	}
		fgets(cpuload, BUFSIZE,f);
		printf("%s\n",cpuload);

	switch(t){

		case TENMIN:
			strncpy(temp,cpuload, 4);
				
			break;
		case FIVEMIN:
			printf("fivemin\n");
			break;
		case ONEMIN:
			printf("onemin\n");
			break;

	}
	printf("temp: %s \n",temp);
	printf("load: %f \n",load);
	return load;
}
/*                         main
*  creates file
*  getCpuLoad
* 
*/
void main(void){

	f = getLoadFile();
	getCpuLoad(f,TENMIN);
	
}
//case statement
//getrid of printf f make return integer
//for 1 5 or 10
