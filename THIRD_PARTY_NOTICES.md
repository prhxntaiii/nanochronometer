# Third-party notices

This source package can link against OpenSSL and/or libsodium when the matching files are present under `externals/`.

Before distributing a binary release that includes OpenSSL or libsodium DLLs/import libraries, include the official license and notice files from the exact versions you bundled. This repository intentionally does not guess or rewrite those notices because they must match the shipped binaries.

Recommended release layout:

```text
dist\
  licenses\
    openssl-LICENSE.txt
    openssl-NOTICE.txt
```

## Cryptography Libraries and Dependencies
This project may link against or bundle third-party cryptographic libraries:
- **wolfSSL / wolfCrypt**: Dual-licensed under GPLv2/GPLv3 and commercial licenses. This project utilizes wolfSSL under the terms of the **GNU General Public License v3.0**. See [wolfssl.com](https://www.wolfssl.com) for details.
- **OpenSSL**: Used or referenced in compliance with their respective open-source licenses.
