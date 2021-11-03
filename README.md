# nanoservices

[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=dmerkushov_nanoservices2&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=dmerkushov_nanoservices2) [![Documentation](https://codedocs.xyz/dmerkushov/nanoservices2.svg)](https://codedocs.xyz/dmerkushov/nanoservices2/)

Nanoservices framework

It is in development by now, a more meaningful readme will be added later

## How to build

You'll need: `cmake` (minimum version is 3.19), `make` and a C++20 compiler (the framework is tested with gcc 10.3.0)

```bash
git clone git@github.com:dmerkushov/nanoservices2.git`
cd nanoservices/
mkdir build
cd build/
cmake ..
cmake --build .
cd ../..
```

## How to develop

### clang-format

`clang-format`, minimum version 12, is advised to support the project's codestyle.