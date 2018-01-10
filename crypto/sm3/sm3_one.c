#include <openssl/sm3.h>

unsigned char * SM3(const unsigned char *d, size_t n, unsigned char *md) {

	static uint8_t m[SM3_DIGEST_LENGTH];

	if (md == NULL) {
		md = m;
	}

	SM3_CTX c;
	SM3_Init(&c);
	SM3_Update(&c, d, n);
	SM3_Final(md, &c);

	return (md);
}
