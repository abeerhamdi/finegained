#include "fge.h"


int main(int argc, char **argv)
{
	int numSegments = 0;
	uint32_t  kid;
	uint32_t magic = MAGIC;
	uint32_t start;
	typedef struct {
		uint32_t start;
		uint32_t length;
		char * content;
	}segment;
	unsigned char key[MAXKEYLEN];/* Encryption key */
	int fdp; /* Plaintext file */
	if(argc < 3){
		printf("%s\n", "Not enough args!");
		return;
	} else if(strcmp(argv[1],"-s") == 0){
		numSegments = (argc - 3)/2;
		segment segments[numSegments];
		//open the file
		fdp=open(argv[2],O_RDONLY);/* Plaintext */
		int argNum = 3;
		int i = 0;
		for(i; i<numSegments; i++){
			segments[i].start = atoi(argv[argNum]);
			segments[i].length = atoi(argv[++argNum]);
			
			lseek(fdp,segments[i].start,SEEK_SET);
			char * buf = malloc(sizeof(char)*segments[i].length);
			read(fdp, buf, segments[i].length);
			segments[i].content = buf;
			
			argNum++;
		}
		kid = rand();
		char *encFile; /* Ciphertext file name */
		FILE * fde; /* Ciphertext file */
		encFile=malloc(strlen(argv[2])+5); /* filename.enc */
		strcpy(encFile,argv[2]); strcat(encFile,".enc");
		fde=fopen(encFile,"w+");//open file
		
		fprintf(fde,"0x%08X", kid);
		fprintf(fde,"0x%08X", magic);
		//fprintf(fde,"0x%08X", start);
		
		start = 30;//kid + magic+start
		
		unsigned char * passphrase = getpass("Enter password:");
		unsigned char sha[SHA_DIGEST_LENGTH];
		hash(passphrase,sha);
		
		initialize_key(key);
		int mlen = strlen(key) + 1;
		
		unsigned char * kidCipher = allocate_ciphertext(8);
		int kidCipherLen = encrypt((char *)&kid, sha, kidCipher);
		kidCipher[kidCipherLen] = '\0';
		fprintf(fde,"%s", kidCipher);
		start+=kidCipherLen;
		
		unsigned char * keyCipher = allocate_ciphertext(mlen);
		int keyCipherLen = encrypt(key, sha, keyCipher);
		keyCipher[keyCipherLen] = '\0';
		fprintf(fde,"%s", keyCipher);
		start+=keyCipherLen;
		
		unsigned char * nCipher = allocate_ciphertext(8);
		int nCipherLen = encrypt((char *)&numSegments, key, nCipher);
		nCipher[nCipherLen] = '\0';
		fprintf(fde,"%d", nCipher);
		start+=nCipherLen;
		
		i = 0;
		unsigned char * startCipher;
		unsigned char * lengthCipher;
		unsigned char * contentCipher;
		int startCipherLen;
		int lengthCipherLen;
		int contentCipherLen;
		for(i; i<numSegments; i++){
			startCipher = allocate_ciphertext(8);
			startCipherLen = encrypt((char *)&segments[i].start, key, startCipher);
			startCipher[startCipherLen] = '\0';
			fprintf(fde,"%s", startCipher);
			free(startCipher);
			start+=startCipherLen;
			
			lengthCipher = allocate_ciphertext(8);
			lengthCipherLen = encrypt((char *)&segments[i].length, key, lengthCipher);
			lengthCipher[lengthCipherLen] = '\0';
			fprintf(fde,"%s", lengthCipher);
			free(lengthCipher);
			start+=lengthCipherLen;
			
			mlen = strlen(key) + 1;
			contentCipher = allocate_ciphertext(mlen);
			contentCipherLen = encrypt(segments[i].content, key, contentCipher);
			contentCipher[contentCipherLen] = '\0';
			fprintf(fde,"%s", contentCipher);
			free(contentCipher);
			start+=contentCipherLen;
		}
		fseek(fde, 20, SEEK_SET);
		fprintf(fde,"0x%08X", start);
		fseek(fde, start, SEEK_SET);
		i = 0;
		int lastPosition = 0;
		lseek(fdp, 0, SEEK_SET);
		for(i; i<numSegments; i++){
			char * buf = malloc(sizeof(char)*(segments[i].start-lastPosition));
			read(fdp, buf,(segments[i].start-lastPosition));
			buf[(segments[i].start-lastPosition)] = '\0';
			fprintf(fde,"%s", buf);
			fprintf(fde,"%s", "X");
			lastPosition = lseek(fdp, segments[i].length, SEEK_CUR);
			free(buf);
		}
		i = 0;
		int buflen = 10;
		char * buf = malloc(sizeof(char)*buflen);
		read(fdp, &buf[i] ,1);
		while(buf[i] > 0){
			if(i == buflen - 1){
				buflen += 10;
				buf = realloc(buf, sizeof(char)*buflen);
			}
			i++;
			read(fdp, &buf[i] ,1);
		}
		buf[i-1] = '\0';
		fprintf(fde,"%s", buf);
		free(buf);
		return;
	} else if(strcmp(argv[1] , "-c") == 0){
		
	} else if(strcmp(argv[1] ,"-u") == 0){
		char *encFile = malloc(strlen(argv[2])+5); /* filename.enc */
		strcpy(encFile,argv[2]); strcat(encFile,".enc");
		fdp=open(encFile,O_RDONLY);//encrypted file
		
		char * kidBuf = malloc(sizeof(char)*10);
		read(fdp, kidBuf, 10);
		
		char * magicBuf = malloc(sizeof(char)*10);
		read(fdp, magicBuf, 10);
		if(magic != strtol(magicBuf, NULL, 0)){
			printf("%s\n", "Wrong File");
			return;
		}
		
		char * startBuf = malloc(sizeof(char)*10);
		read(fdp, startBuf, 10);
		
		lseek(fdp, strtol(startBuf, NULL, 0), SEEK_SET);
		
		int buflen = 10;
		int i = 0;
		char * plainTextBuf = malloc(sizeof(char)*buflen);
		read(fdp, &plainTextBuf[i] ,1);
		while(plainTextBuf[i] > 0){
			if(i == buflen - 1){
				buflen += 10;
				plainTextBuf = realloc(plainTextBuf, sizeof(char)*buflen);
			}
			i++;
			read(fdp, &plainTextBuf[i] ,1);
		}
		plainTextBuf[i-1] = '\0';
		printf("%s\n", plainTextBuf);
		free(plainTextBuf);
		free(
		return;
	} else if(strcmp(argv[1],"-key") == 0){
		
	}
}
