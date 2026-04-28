# Android ARM64 externals

Place prebuilt Android NDK `arm64-v8a` crypto libraries here.
Both static and shared variants are supported by CMake via `-DNC_EXTERNAL_LINKAGE=AUTO|STATIC|SHARED`.

Expected layout:

```text
externals/arm64/android/
  libsodium/
    include/sodium.h
    lib/libsodium.a
    lib/libsodium.so
  boringssl/
    include/openssl/evp.h
    lib/libcrypto.a
    lib/libcrypto.so
```

Static archives used inside `libnanochrono.so` should be built with PIC. If they are not PIC, configure with `-DNC_EXTERNAL_LINKAGE=SHARED` or leave the shared target crypto-free and use the static `libnanochrono.a` target.
