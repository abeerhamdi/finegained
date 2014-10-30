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


		fdp=open(argv[2],O_RDONLY);/* Plaintext 
		uint32_t pkid;
		uint32_t ekid;
		uint32_t ckid;
		uint32_t maybeMagic;
		read(fdp, &pkid, 4);
		read(fdp, &maybeMagic, 4);
		if(maybeMagic != MAGIC){
			printf("%s\n", "This is not an fge secured file!");
			return;
		}
		unsigned char * passphrase = getpass("Enter password:");
		unsigned char sha[SHA_DIGEST_LENGTH];
		hash(passphrase, sha);
		lseek(fdp, 4, SEEK_CUR); //jump over start
		read(fdp, &ekid, 4);
		decrypt((char*)&ckid, sha, (char*)&ekid, 4);
		if(ckid == ekid){
			printf("%s\n", "Yay decryption worked!");
		}
		else{
			printf("%s\n", "Fuck...");
		}
**/
