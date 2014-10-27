#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <openssl/blowfish.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>


#define MAXKEYLEN 56
#define ACTKEYLEN 32
#define BLOCKSIZE 8
#define BUFSIZE 10

void fprint_string_as_hex(FILE * f, unsigned char *s, int len);
void initialize_key(unsigned char *key);
unsigned char * allocate_ciphertext(int mlen);
void encrypt_and_print(EVP_CIPHER_CTX * ectx, char *msg, int mlen,char *res, int *olen, FILE * f);
void encrypt();
