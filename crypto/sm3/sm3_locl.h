#ifndef CRYPTO_SM3_SM3_LOCL_H_
#define CRYPTO_SM3_SM3_LOCL_H_

#include <stdint.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define BIN_TO_UINT32(b, n) \
{ \
	(n) = ((uint32_t)(b)[0] << 24) | ((uint32_t)(b)[1] << 16) | ((uint32_t)(b)[2] << 8) | ((uint32_t)(b)[3]); \
}

#define UINT32_TO_BIN(n, b) \
{ \
    (b)[0] = (uint8_t) ((n) >> 24 ); \
    (b)[1] = (uint8_t) ((n) >> 16 ); \
    (b)[2] = (uint8_t) ((n) >>  8 ); \
    (b)[3] = (uint8_t) (n); \
}

#define UINT64_TO_BIN(n, b) \
{ \
	(b)[0] = (uint8_t) ((n) >> 56 ); \
	(b)[1] = (uint8_t) ((n) >> 48 ); \
	(b)[2] = (uint8_t) ((n) >> 40 ); \
	(b)[3] = (uint8_t) ((n) >> 32 ); \
	(b)[4] = (uint8_t) ((n) >> 24 ); \
	(b)[5] = (uint8_t) ((n) >> 16 ); \
	(b)[6] = (uint8_t) ((n) >>  8 ); \
	(b)[7] = (uint8_t) (n); \
}

#define ROTL(x, n) ((((x) & 0xFFFFFFFF) << (n)) | ((x) >> (32 - (n))))

#define FF0(x, y, z) ((x) ^ (y) ^ (z))
#define FF16(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))

#define GG0(x, y, z) ((x) ^ (y) ^ (z))
#define GG16(x, y, z) (((x) & (y)) | ((~(x)) & (z)))

#define P0(x) ((x) ^  ROTL((x), 9) ^ ROTL((x), 17))
#define P1(x) ((x) ^  ROTL((x), 15) ^ ROTL((x), 23))

#ifdef  __cplusplus
}
#endif

#endif
