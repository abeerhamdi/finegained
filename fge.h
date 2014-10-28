#include <openssl/blowfish.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>

#define MAXKEYLEN 56
#define ACTKEYLEN 32
#define BLOCKSIZE 8
#define BUFSIZE 10
#define MAGIC 1420420420

void fprint_string_as_hex(FILE * f, unsigned char *s, int len);
void initialize_key(unsigned char *key);
unsigned char * allocate_ciphertext(int mlen);
int encrypt(unsigned char *ptxt, unsigned char *key, unsigned char *ctxt);
void hash(unsigned char *keytext, unsigned char *sha);
int decrypt(unsigned char *ptxt, unsigned char *key, unsigned char *ctxt, int ctlen);