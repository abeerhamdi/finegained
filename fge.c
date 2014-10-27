#include "fge.h"


int main(int argc, char **argv)
{
	EVP_CIPHER_CTX *ctx; /* Encryption context*/
	EVP_CIPHER *cipher; /* Cipher */
	char ivec[EVP_MAX_IV_LENGTH] = {0};
	int fdk; /* Key to this file */
	unsigned char key[MAXKEYLEN];/* Encryption key */
	int fdp; /* Plaintext file */
	char buf[BUFSIZE]; /* Hold plaintext */
	int mlen; /* Length of plaintext */
	int fde; /* Ciphertext file */
	char *encFile; /* Ciphertext file name */
	unsigned char *ciphertext;/* Pointer to ciphertext */
	int ctlen; /* Length of ciphertext */
	
	/* Generate the key -- may be more bytes than needed*/
	//initialize_key(key,MAXKEYLEN);
	initialize_key(key);
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
	fdp=open(argv[1],O_RDONLY);/* Plaintext */
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


