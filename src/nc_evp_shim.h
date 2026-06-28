#pragma once
#ifndef NC_EVP_SHIM_H
#define NC_EVP_SHIM_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Status of a particular EVP symbol after shim initialisation. */
typedef enum nc_evp_sym_status {
    NC_EVP_SYM_OK      = 0,
    NC_EVP_SYM_MISSING = 1   /* symbol absent in the system libcrypto */
} nc_evp_sym_status_t;

/*
 * Open the system libcrypto and resolve function pointers.
 * Idempotent — safe to call multiple times.
 * Returns 1 if at least dlopen succeeded (some symbols may still be missing),
 * 0 if the library could not be opened at all.
 */
int nc_evp_shim_init(void);

/* Per-feature availability queries (call after nc_evp_shim_init). */
nc_evp_sym_status_t nc_evp_sha256_available(void);
nc_evp_sym_status_t nc_evp_hmac_available(void);
nc_evp_sym_status_t nc_evp_rand_available(void);

/*
 * Wrapped crypto operations.
 * Return  0 (NC_OK equivalent) on success.
 * Return -3 (NC_ERR_CRYPTO_BACKEND) when the symbol is missing or the
 * underlying call fails.
 * Return -2 (NC_ERR_BAD_ARGUMENT) on NULL pointer arguments.
 */
int nc_shim_sha256(const void *msg, size_t len, void *out_digest_32);
int nc_shim_hmac_sha256(const void *key, size_t key_len,
                        const void *msg, size_t msg_len,
                        void *out_mac_32);
int nc_shim_rand_bytes(void *buf, size_t n);

#ifdef __cplusplus
}
#endif
#endif /* NC_EVP_SHIM_H */
