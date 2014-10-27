void fprint_string_as_hex(FILE * f, unsigned char *s, int len)
{
	int i;
	for (i = 0; i < len; i++) {
		fprintf(f, "%02x", s[i]);
	}
}

void initialize_key(unsigned char *key)
{
	FILE *rng;
	int num = 0;
	rng = fopen("/dev/urandom", "r");
	while (num < EVP_MAX_KEY_LENGTH) {
		num += fread(&key[num],1,EVP_MAX_KEY_LENGTH - num,rng);
	}
	fclose(rng);
}
unsigned char * allocate_ciphertext(int mlen)
{
	/* Alloc space for any possible padding. */
	return (unsigned char *) malloc(mlen + BLOCKSIZE);
}
void encrypt_and_print(EVP_CIPHER_CTX * ectx, char *msg, int mlen,char *res, int *olen, FILE * f)
{
	int extlen;
	
	EVP_EncryptUpdate(ectx, res, olen, msg, mlen);
	EVP_EncryptFinal_ex(ectx, &res[*olen], &extlen);
	
	*olen += extlen;
	
	fprintf(f, "Encrypted result: ");
	fprint_string_as_hex(f, res, *olen);
	fprintf(f, "\n");
}
void encrypt(){
	EVP_CIPHER_CTX *ctx; /* SSL context */
	const EVP_CIPHER *cipher = EVP_bf_cbc();/* Cipher */

	unsigned char key[EVP_MAX_KEY_LENGTH];
	unsigned char msg[] = "Sample message.";
	unsigned char * ciphertext;
	char ivec[EVP_MAX_IV_LENGTH] = {0};
	int mlen = strlen(msg) + 1;
	int ctlen;
	ctx = (EVP_CIPHER_CTX *) malloc(sizeof(EVP_CIPHER_CTX));
	EVP_CIPHER_CTX_init(ctx); 
	/* Blowfish CBC mode
	128-bit keys
	(default size)*/
	initialize_key(key);
	/* Print key in HEX */
	fprint_string_as_hex(stdout,key,EVP_CIPHER_key_length(cipher));
	/* Initialization context */
	EVP_EncryptInit_ex(ctx, cipher, NULL, key, ivec);
	/* Allocate space for the ciphertext */
	ciphertext = allocate_ciphertext(mlen);
	/* Encrypt data and print */
	encrypt_and_print(ctx, msg, mlen, ciphertext,&ctlen, stdout);
}
