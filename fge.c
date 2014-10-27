#include "fge.h"


int main(int argc, char **argv)
{
	int numSegments = 0;
	uint32_t  kid = malloc(4);
	uint32_t magic = 1420420420;
	uint32_t start = 1111111111;
	typedef struct segment  {
		uint32_t start;
		uint32_t length;
		char * content;
	};
	unsigned char key[MAXKEYLEN];/* Encryption key */
	int fdp; /* Plaintext file */
	if(argc < 3){
		printf("%s\n", "Not enough args!"); 
		return;
	} else if(strcmp(argv[1],"-s") == 0){
		initialize_key(key);
		numSegments = (argc - 3)/2;
		struct segment segments[numSegments];
		//open the file
		fdp=open(argv[2],O_RDONLY);/* Plaintext */
		int argNum = 3;
		int i =0;
		for(i; i<numSegments; i++){
			segments[i].start = atoi(argv[argNum]);
			segments[i].length = atoi(argv[++argNum]);
			
			lseek(fdp,segments[i].start,SEEK_SET);
			char * buf = malloc(sizeof(char)*segments[i].length);
			read(fdp, buf, segments[i].length);
			segments[i].content = buf;
			free(buf);
			
			argNum++;
		}
		kid = rand();
		char *encFile; /* Ciphertext file name */
		FILE * fde; /* Ciphertext file */
		encFile=malloc(strlen(argv[2])+5); /* filename.enc */
		strcpy(encFile,argv[2]); strcat(encFile,".enc");
		fde=fopen(encFile,"w+");
		fprintf(fde,"%d", kid);
		fprintf(fde,"%d", magic);
		fprintf(fde,"%d", start);
		unsigned char * passphrase = getpass("Enter password:");
		unsigned char sha[SHA_DIGEST_LENGTH];
		hash(passphrase,sha);
		fprintf(fde,"%s",sha);
		int mlen = strlen(key) + 1;
		unsigned char * keyCipher = allocate_ciphertext(mlen);
		unsigned char * kidCipher = allocate_ciphertext(8);
		encrypt(key, sha, keyCipher);
		encrypt(key, sha, kidCipher);
		fprintf(fde,"%d", kidCipher);
		fprintf(fde,"%s", keyCipher);
		
		return;
	} else if(strcmp(argv[1] , "-c") == 0){
		
	} else if(strcmp(argv[1] ,"-u") == 0){
	
	} else if(strcmp(argv[1],"-key") == 0){
		
	}
	
	EVP_CIPHER_CTX *ctx; /* Encryption context*/
	EVP_CIPHER *cipher; /* Cipher */
	char ivec[EVP_MAX_IV_LENGTH] = {0};
	int fdk; /* Key to this file */
	char buf[BUFSIZE]; /* Hold plaintext */
	int mlen; /* Length of plaintext */
	int fde; /* Ciphertext file */
	unsigned char *ciphertext;/* Pointer to ciphertext */
	int ctlen; /* Length of ciphertext */
	
		char *encFile; 
	
	/* Generate the key -- may be more bytes than needed*/
	//initialize_key(key,MAXKEYLEN);
	fprintf(stdout,"Raw file key bytes <");
	fprint_string_as_hex(stdout, key, MAXKEYLEN);
	fprintf(stdout,">\n");
	
	/* Write key to file */
	fdk=open(argv[2],O_CREAT|O_TRUNC|O_RDWR, S_IRUSR|S_IWUSR );
	write(fdk,key,ACTKEYLEN);
	printf("Key is <");
	fprint_string_as_hex(stdout,key,ACTKEYLEN);
	printf(">\n");
	close(fdk);

	/* Encrypt file contents */
	//fdp=open(argv[1],O_RDONLY);/* Plaintext */
	encFile=malloc(strlen(argv[1])+5); /* filename.enc */
	strcpy(encFile,argv[1]); strcat(encFile,".enc");
	printf("Writing encrypted file content to <%s>\n",encFile);
	fde=open(encFile,O_CREAT|O_TRUNC|O_WRONLY|O_APPEND,S_IRUSR|S_IWUSR);
	
	cipher=(EVP_CIPHER *)EVP_bf_cbc();
	ctx = (EVP_CIPHER_CTX *)malloc(sizeof(EVP_CIPHER_CTX));
	EVP_CIPHER_CTX_init(ctx);
	EVP_EncryptInit_ex(ctx,cipher,NULL,NULL,NULL);
	EVP_CIPHER_CTX_set_key_length(ctx,ACTKEYLEN);
	EVP_EncryptInit_ex(ctx,NULL,NULL,key,ivec);
	ciphertext=allocate_ciphertext(BUFSIZE);
	ctlen=0;
	while ((mlen=read(fdp,buf,BUFSIZE))>0) {
		printf("Read %d bytes of plaintext <",mlen);
		fprint_string_as_hex(stdout,buf,mlen);printf(">\n");
		EVP_EncryptUpdate(ctx, ciphertext, &ctlen, buf, mlen);
		write(fde,ciphertext,ctlen);
		printf("Wrote %d bytes of ciphertext <",ctlen);
		fprint_string_as_hex(stdout,ciphertext,ctlen);
		printf(">\n");
	}
	
	ctlen=0;
	EVP_EncryptFinal_ex(ctx,ciphertext, &ctlen);
	write(fde,ciphertext,ctlen);
	printf("Wrote %d bytes of ciphertext <",ctlen);
	fprint_string_as_hex(stdout,ciphertext,ctlen);
	printf(">\n");

}


