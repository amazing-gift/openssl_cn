#ifndef OPENSSL_SM3_H
#define OPENSSL_SM3_H

#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define SM3_DIGEST_LENGTH 32

typedef struct SM3state_st {
	uint64_t length;
	uint8_t buffer[64];
	uint32_t v[8];
} SM3_CTX;

int SM3_Init(SM3_CTX *c);
int SM3_Update(SM3_CTX *c, const void *data, size_t len);
int SM3_Final(unsigned char *md, SM3_CTX *c);

unsigned char * SM3(const unsigned char *d, size_t n, unsigned char *md);

#ifdef  __cplusplus
}
#endif

#endif
