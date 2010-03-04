# include "cpumon.h"

/* convertToD
* converts an input string to a double
*/
float convertToD(char c[TEMPSIZE]){
	float d;
	int i;
	float temp;
        int nDecimalDigits;
printf ("%s\n", c);
	nDecimalDigits = 1;

	for(i = 0; i < strlen(c); i ++){
		if(c[i] == '.')
			i++;
		temp = c[i] - '0';
		d += temp / nDecimalDigits;
		nDecimalDigits *= 10;
		
	}
	return d;

}


/* seperateString
* seperates out the values of the given input string
*/
int seperateString(char * c, int timeInt){

	char  one[TEMPSIZE];
	char five[TEMPSIZE];
	char  ten[TEMPSIZE];
	int i, z,w, y;
	float load;
   w = 0;
	y = 0;
	z = 0;
	


	for(i = 0; i < BUFSIZE; i++){
		if(i < 4){
				ten[y] = c[i];
				y++;
				ten[y] = '\0';
		}
		if(i > 4 && i < 9){
				five[z] = c[i];
				z++;
				five[z] = '\0';
		}
		if(i > 9 && i < 14){
				one[w] = c[i];
				w++;
				one[w] = '\0';
		}

	}
	switch(timeInt){
 
		case TENMIN:
			load = convertToD(ten);
			break;

		case FIVEMIN:
			load = convertToD(five);
			break;
		case ONEMIN:
			load = convertToD(one);
			break;
 
	}
	//printf("load: %f\n", load);

	return (int)(load * 100);
}
/* loadFile
* returns a file given an input string
*/
FILE * getLoadFile(){
 
return fopen("/proc/loadavg","r");
}
 
/* getCpuload
* returns the average cpu load for the last 10 min, 5 min, and 1 min
*/
int getCpuLoad(FILE *f, int t){

	float load;
	if(f == NULL){
		printf("file not found\n");
	}
	char cpuload[BUFSIZE];
	fgets(cpuload, BUFSIZE,f);

 	load = seperateString(cpuload, 1);
	
	return load;
}

extern int cpuTen (void){
 
	FILE * f;
	f = getLoadFile();
	int retVal = getCpuLoad(f,TENMIN);
        return retVal;	
}
