#include <openssl/sm3.h>

#include "sm3_locl.h"

#include <string.h>

static const uint8_t SM3_PAD[64] = {
	0x80, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

static const uint32_t T[64] = {
	0x79CC4519, 0x79CC4519, 0x79CC4519, 0x79CC4519, 0x79CC4519, 0x79CC4519, 0x79CC4519, 0x79CC4519,
	0x79CC4519, 0x79CC4519, 0x79CC4519, 0x79CC4519, 0x79CC4519, 0x79CC4519, 0x79CC4519, 0x79CC4519,

	0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A,
	0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A,
	0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A,	0x7A879D8A, 0x7A879D8A, 0x7A879D8A,
	0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A,
	0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A,
	0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A, 0x7A879D8A
};

static void SM3_Update_block(SM3_CTX *c, const unsigned char * data);

int SM3_Init(SM3_CTX *c) {
	c->length = 0;
	c->v[0] = 0x7380166F;
	c->v[1] = 0x4914B2B9;
	c->v[2] = 0x172442D7;
	c->v[3] = 0xDA8A0600;
	c->v[4] = 0xA96F30BC;
	c->v[5] = 0x163138AA;
	c->v[6] = 0xE38DEE4D;
	c->v[7] = 0xB0FB0E4E;
	return 0;
}

static void SM3_Update_block(SM3_CTX *c, const unsigned char * data) {

	register int j;

	uint32_t W[68];
	uint32_t W1[64];

	for (j = 0; j < 16; ++j) {
		BIN_TO_UINT32(data + j * 4, W[j])
	}

	for (j = 16; j < 68; j++) {
		W[j] = P1(W[j - 16] ^ W[j - 9] ^ ROTL(W[j - 3], 15)) ^ ROTL(W[j - 13], 7) ^ W[j - 6];
	}

	for (j = 0; j < 64; j++) {
		W1[j] = W[j] ^ W[j + 4];
	}

	register uint32_t SS1, SS2, TT1, TT2;
	register uint32_t A, B, C, D, E, F, G, H;

	A = c->v[0];
	B = c->v[1];
	C = c->v[2];
	D = c->v[3];
	E = c->v[4];
	F = c->v[5];
	G = c->v[6];
	H = c->v[7];

	for (j = 0; j < 16; j++) {
		SS1 = ROTL((ROTL(A, 12) + E + ROTL(T[j], j)), 7);
		SS2 = SS1 ^ ROTL(A, 12);
		TT1 = FF0(A,B,C) + D + SS2 + W1[j];
		TT2 = GG0(E,F,G) + H + SS1 + W[j];
		D = C;
		C = ROTL(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = ROTL(F, 19);
		F = E;
		E = P0(TT2);
	}

	for (j = 16; j < 64; j++) {
		SS1 = ROTL((ROTL(A,12) + E + ROTL(T[j],j)), 7);
		SS2 = SS1 ^ ROTL(A, 12);
		TT1 = FF16(A, B, C) + D + SS2 + W1[j];
		TT2 = GG16(E, F, G) + H + SS1 + W[j];
		D = C;
		C = ROTL(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = ROTL(F, 19);
		F = E;
		E = P0(TT2);
	}

	c->v[0] ^= A;
	c->v[1] ^= B;
	c->v[2] ^= C;
	c->v[3] ^= D;
	c->v[4] ^= E;
	c->v[5] ^= F;
	c->v[6] ^= G;
	c->v[7] ^= H;
}

int SM3_Update(SM3_CTX *c, const void *data, size_t len) {

	uint8_t *p = (uint8_t *) data;

	uint64_t left = c->length % 64;
	uint64_t fill = 64 - left;

	c->length += len;

	if (left > 0 && len >= fill) {
		memcpy(c->buffer + left, data, fill);
		SM3_Update_block(c, c->buffer);
		p += fill;
		len -= fill;
		left = 0;
	}

	while (len >= 64) {
		SM3_Update_block(c, p);
		p += 64;
		len -= 64;
	}

	if (len > 0) {
		memcpy(c->buffer + left, p, len);
	}

	return 0;
}

int SM3_Final(unsigned char *md, SM3_CTX *c) {

	uint64_t padlen = 64 - (c->length % 64);
	if (padlen >= 56) {
		padlen += 64;
	}
	SM3_Update(c, SM3_PAD, padlen);

	uint8_t bit_len_bin[8];
	uint64_t n = c->length * 8;
	UINT64_TO_BIN(n, bit_len_bin)
	SM3_Update(c, bit_len_bin, 8);

	for (int i = 0; i < 8; ++i) {
		uint32_t n = c->v[i];
		uint8_t *bin = md + i * 4;
		UINT32_TO_BIN(n, bin)
	}

	return 0;
}
