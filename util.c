#include "fge.h"


/**
main(int argc, char **argv){
	int ret, total, keySize, urandFd;
	char* keyPtr;
	
	keySize=atoi(argv[1]);
	keyPtr= (char *) malloc(keySize);
	urandFd=open("/dev/urandom",O_RDONLY);
	total=0;
	while (total<keySize) {
		ret=read(urandFd,&keyPtr[total],keySize-total);
		total+=ret;
	}
	printf("%s",keyPtr);
	printf("Generated key with size <%d>\n",total);
	close(urandFd);
}
**/

