#include <cstring> // For memset and memcpy
#include "aes.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************/
/* Defines:                                                                  */
/*****************************************************************************/
// The number of columns comprising a state in AES. This is a constant in AES. Value=4
#define Nb 4

#if defined(AES256) && (AES256 == 1)
#define Nk 8
#define Nr 14
#elif defined(AES192) && (AES192 == 1)
#define Nk 6
#define Nr 12
#else
#define Nk 4        // The number of 32 bit words in a key.
#define Nr 10       // The number of rounds in AES Cipher.
#endif

#ifndef MULTIPLY_AS_A_FUNCTION
#define MULTIPLY_AS_A_FUNCTION 0
#endif

/*****************************************************************************/
/* Private variables:                                                        */
/*****************************************************************************/
typedef uint8_t state_t[4][4];

static const uint8_t sbox[256] = {
    // ... (unchanged from your original code, omitted here for brevity)
};

#if (defined(CBC) && CBC == 1) || (defined(ECB) && ECB == 1)
static const uint8_t rsbox[256] = {
    // ... (unchanged from your original code, omitted here for brevity)
};
#endif

static const uint8_t Rcon[11] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

/*****************************************************************************/
/* Private functions:                                                        */
/*****************************************************************************/
#define getSBoxValue(num) (sbox[(num)])

static void KeyExpansion(uint8_t* RoundKey, const uint8_t* Key) {
    // ... (same logic, code unchanged)
}

void AES_init_ctx(struct AES_ctx* ctx, const uint8_t* key) {
    KeyExpansion(ctx->RoundKey, key);
}

#if (defined(CBC) && (CBC == 1)) || (defined(CTR) && (CTR == 1))
void AES_init_ctx_iv(struct AES_ctx* ctx, const uint8_t* key, const uint8_t* iv) {
    KeyExpansion(ctx->RoundKey, key);
    memcpy(ctx->Iv, iv, AES_BLOCKLEN);
}

void AES_ctx_set_iv(struct AES_ctx* ctx, const uint8_t* iv) {
    memcpy(ctx->Iv, iv, AES_BLOCKLEN);
}
#endif

static void AddRoundKey(uint8_t round, state_t* state, const uint8_t* RoundKey) {
    // ... (same logic, code unchanged)
}

static void SubBytes(state_t* state) {
    // ... (same logic, code unchanged)
}

static void ShiftRows(state_t* state) {
    // ... (same logic, code unchanged)
}

static uint8_t xtime(uint8_t x) {
    return ((x << 1) ^ (((x >> 7) & 1) * 0x1b));
}

static void MixColumns(state_t* state) {
    // ... (same logic, code unchanged)
}

#if MULTIPLY_AS_A_FUNCTION
static uint8_t Multiply(uint8_t x, uint8_t y) {
    // ... (same logic, code unchanged)
}
#else
#define Multiply(x, y) \
(((y & 1) * x) ^ \
 ((y >> 1 & 1) * xtime(x)) ^ \
 ((y >> 2 & 1) * xtime(xtime(x))) ^ \
 ((y >> 3 & 1) * xtime(xtime(xtime(x)))) ^ \
 ((y >> 4 & 1) * xtime(xtime(xtime(xtime(x))))))
#endif

static void Cipher(state_t* state, const uint8_t* RoundKey) {
    // ... (same logic, code unchanged)
}

#if (defined(CBC) && CBC == 1) || (defined(ECB) && ECB == 1)
static void InvCipher(state_t* state, const uint8_t* RoundKey) {
    // ... (same logic, code unchanged)
}
#endif

/*****************************************************************************/
/* Public functions:                                                         */
/*****************************************************************************/
#if defined(ECB) && (ECB == 1)
void AES_ECB_encrypt(const struct AES_ctx* ctx, uint8_t* buf) {
    Cipher((state_t*)buf, ctx->RoundKey);
}

void AES_ECB_decrypt(const struct AES_ctx* ctx, uint8_t* buf) {
    InvCipher((state_t*)buf, ctx->RoundKey);
}
#endif

#if defined(CBC) && (CBC == 1)
static void XorWithIv(uint8_t* buf, const uint8_t* Iv) {
    for (uint8_t i = 0; i < AES_BLOCKLEN; ++i) {
        buf[i] ^= Iv[i];
    }
}

void AES_CBC_encrypt_buffer(struct AES_ctx* ctx, uint8_t* buf, size_t length) {
    uint8_t* Iv = ctx->Iv;
    for (size_t i = 0; i < length; i += AES_BLOCKLEN) {
        XorWithIv(buf, Iv);
        Cipher((state_t*)buf, ctx->RoundKey);
        Iv = buf;
        buf += AES_BLOCKLEN;
    }
    memcpy(ctx->Iv, Iv, AES_BLOCKLEN);
}

void AES_CBC_decrypt_buffer(struct AES_ctx* ctx, uint8_t* buf, size_t length) {
    uint8_t storeNextIv[AES_BLOCKLEN];
    for (size_t i = 0; i < length; i += AES_BLOCKLEN) {
        memcpy(storeNextIv, buf, AES_BLOCKLEN);
        InvCipher((state_t*)buf, ctx->RoundKey);
        XorWithIv(buf, ctx->Iv);
        memcpy(ctx->Iv, storeNextIv, AES_BLOCKLEN);
        buf += AES_BLOCKLEN;
    }
}
#endif

#if defined(CTR) && (CTR == 1)
void AES_CTR_xcrypt_buffer(struct AES_ctx* ctx, uint8_t* buf, size_t length) {
    uint8_t buffer[AES_BLOCKLEN];
    for (size_t i = 0, bi = AES_BLOCKLEN; i < length; ++i, ++bi) {
        if (bi == AES_BLOCKLEN) {
            memcpy(buffer, ctx->Iv, AES_BLOCKLEN);
            Cipher((state_t*)buffer, ctx->RoundKey);

            for (int bi = (AES_BLOCKLEN - 1); bi >= 0; --bi) {
                if (ctx->Iv[bi] == 255) {
                    ctx->Iv[bi] = 0;
                    continue;
                }
                ctx->Iv[bi] += 1;
                break;
            }
            bi = 0;
        }
        buf[i] ^= buffer[bi];
    }
}
#endif

#ifdef __cplusplus
}
#endif
