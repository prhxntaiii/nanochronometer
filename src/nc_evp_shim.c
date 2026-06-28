#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#include "nc_evp_shim.h"
#include <stdint.h>
#include <string.h>

/* ── platform dynamic-loader ──────────────────────────────────────────────── */
#if defined(_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
typedef HMODULE nc_dl_handle_t;
static nc_dl_handle_t nc_dlopen_(const char *name){ return LoadLibraryA(name); }
static void *nc_dlsym_(nc_dl_handle_t h, const char *sym){ return (void*)GetProcAddress(h,sym); }
#elif defined(__ANDROID__) || defined(__linux__) || defined(__unix__)
#  include <dlfcn.h>
typedef void *nc_dl_handle_t;
static nc_dl_handle_t nc_dlopen_(const char *name){ return dlopen(name,RTLD_LAZY|RTLD_GLOBAL); }
static void *nc_dlsym_(nc_dl_handle_t h, const char *sym){ return dlsym(h,sym); }
#else
/* Unsupported platform — shim always reports symbols missing. */
typedef void *nc_dl_handle_t;
static nc_dl_handle_t nc_dlopen_(const char *n){ (void)n; return (void*)0; }
static void *nc_dlsym_(nc_dl_handle_t h, const char *s){ (void)h;(void)s; return (void*)0; }
#endif

/* ── resolved function pointer types ─────────────────────────────────────── */
typedef unsigned char *(*fn_sha256_t)(const unsigned char*, size_t, unsigned char*);
typedef unsigned char *(*fn_hmac_t)(const void*, int, const unsigned char*, size_t,
                                    unsigned char*, unsigned int*);
typedef int (*fn_rand_bytes_t)(unsigned char*, int);

/* EVP_sha256 is needed only for the HMAC wrapper: it returns a const EVP_MD*
 * which we treat as an opaque pointer here. */
typedef const void *(*fn_evp_sha256_t)(void);

static nc_dl_handle_t  s_handle       = (nc_dl_handle_t)0;
static fn_sha256_t     s_sha256       = (fn_sha256_t)0;
static fn_hmac_t       s_hmac         = (fn_hmac_t)0;
static fn_rand_bytes_t s_rand_bytes   = (fn_rand_bytes_t)0;
static fn_evp_sha256_t s_evp_sha256   = (fn_evp_sha256_t)0;
static int             s_init_done    = 0;
static int             s_lib_opened   = 0;

/* Candidate library names in search order. */
static const char *const s_libcrypto_names[] = {
#if defined(_WIN32)
    "libcrypto-3-x64.dll",
    "libcrypto-3.dll",
    "libcrypto-1_1-x64.dll",
    "libcrypto-1_1.dll",
#elif defined(__APPLE__)
    "libcrypto.3.dylib",
    "libcrypto.1.1.dylib",
    "libcrypto.dylib",
#else
    "libcrypto.so.3",
    "libcrypto.so.1.1",
    "libcrypto.so",
#endif
    (const char*)0
};

int nc_evp_shim_init(void)
{
    if (s_init_done) return s_lib_opened;
    s_init_done = 1;

    for (int i = 0; s_libcrypto_names[i]; ++i) {
        s_handle = nc_dlopen_(s_libcrypto_names[i]);
        if (s_handle) { s_lib_opened = 1; break; }
    }
    if (!s_lib_opened) return 0;

    s_sha256     = (fn_sha256_t)    nc_dlsym_(s_handle, "SHA256");
    s_evp_sha256 = (fn_evp_sha256_t)nc_dlsym_(s_handle, "EVP_sha256");
    s_hmac       = (fn_hmac_t)      nc_dlsym_(s_handle, "HMAC");
    s_rand_bytes = (fn_rand_bytes_t)nc_dlsym_(s_handle, "RAND_bytes");
    return 1;
}

nc_evp_sym_status_t nc_evp_sha256_available(void)
{
    nc_evp_shim_init();
    return s_sha256 ? NC_EVP_SYM_OK : NC_EVP_SYM_MISSING;
}

nc_evp_sym_status_t nc_evp_hmac_available(void)
{
    nc_evp_shim_init();
    return (s_hmac && s_evp_sha256) ? NC_EVP_SYM_OK : NC_EVP_SYM_MISSING;
}

nc_evp_sym_status_t nc_evp_rand_available(void)
{
    nc_evp_shim_init();
    return s_rand_bytes ? NC_EVP_SYM_OK : NC_EVP_SYM_MISSING;
}

int nc_shim_sha256(const void *msg, size_t len, void *out_digest_32)
{
    if (!msg || !out_digest_32) return -2;  /* NC_ERR_BAD_ARGUMENT */
    nc_evp_shim_init();
    if (!s_sha256) return -3;               /* NC_ERR_CRYPTO_BACKEND */
    return s_sha256((const unsigned char*)msg, len, (unsigned char*)out_digest_32)
           ? 0 : -3;
}

int nc_shim_hmac_sha256(const void *key, size_t key_len,
                        const void *msg, size_t msg_len,
                        void *out_mac_32)
{
    if (!key || !msg || !out_mac_32) return -2;
    nc_evp_shim_init();
    if (!s_hmac || !s_evp_sha256) return -3;
    unsigned int out_len = 0;
    const void *md = s_evp_sha256();
    return s_hmac(md, (int)key_len,
                  (const unsigned char*)msg, msg_len,
                  (unsigned char*)out_mac_32, &out_len)
           ? 0 : -3;
}

int nc_shim_rand_bytes(void *buf, size_t n)
{
    if (!buf) return -2;
    nc_evp_shim_init();
    if (!s_rand_bytes) return -3;
    return (s_rand_bytes((unsigned char*)buf, (int)n) == 1) ? 0 : -3;
}
