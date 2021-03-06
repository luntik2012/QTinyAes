# QTinyAes
A Qt-Wrapper for the AES-implementation kokke/tiny-AES-C (supports AES128/192/256)

This class is simply a wrapper for https://github.com/kokke/tiny-AES-C. It allows to use the simple AES-implementation inside Qt and with Qt's `QByteArray` class. Thanks to recent updates, allowing keys of size 128, 192 and 256.

## Features
 - It's a C++-class instead of just C-functions
 - Easy integration with Qt-Projects thanks to the use of QByteArray
 - Allows plain-texts of any size - [PKCS#7 Padding](https://en.wikipedia.org/wiki/Padding_(cryptography)#PKCS7) is added automatically
 - Supports all common AES keysizes (compile-time switch)
 - Currently supports CTR, CBC and ECB as modes

## Installation
The package is providet as qpm package, [`de.skycoder42.qtinyaes`](https://www.qpm.io/packages/de.skycoder42.qtinyaes/index.html). To install:

1. Install qpm (See [GitHub - Installing](https://github.com/Cutehacks/qpm/blob/master/README.md#installing), for **windows** see below)
2. In your projects root directory, run `qpm install de.skycoder42.qtinyaes`
3. Include qpm to your project by adding `include(vendor/vendor.pri)` to your `.pro` file

Check their [GitHub - Usage for App Developers](https://github.com/Cutehacks/qpm/blob/master/README.md#usage-for-app-developers) to learn more about qpm.

**Important for Windows users:** QPM Version *0.10.0* (the one you can download on the website) is currently broken on windows! It's already fixed in master, but not released yet. Until a newer versions gets released, you can download the latest dev build from here:
- https://storage.googleapis.com/www.qpm.io/download/latest/windows_amd64/qpm.exe
- https://storage.googleapis.com/www.qpm.io/download/latest/windows_386/qpm.exe

## Example
```cpp
QTinyAes aes;

aes.setMode(QTinyAes::CTR);
aes.setKey("randomkey_256bit");// QTinyAes::KeySize (256 bit key by default)
//or with Qt 5.10
aes.setKey(QTinyAes::generateKey());
aes.setIv("random_iv_128bit");// QTinyAes::BlockSize (128 iv vector)

QByteArray plain = "Hello World";
qDebug() << "plain:" << plain
QByteArray cipher = aes.encrypt(plain);
qDebug() << "cipher:" << cipher;
QByteArray result = aes.decrypt(cipher);
qDebug() << "result:" << result;
```

## Changing the key size
By default, your keys must be 256 bit keys. However, you can change this size to 192 or 128 if you need to. This can be done via a qmake variable, as the keysize as a **compile time** switch (due to limitations of kokke/tiny-AES-C).

To change the size, set the `TINYAES_KEYSIZE` qmake variable to the desired keysize *before* including vendor.pri/add the qpmx code. Note that for qpmx this only works on source builds, not on compile builds:

```pro
TINYAES_KEYSIZE = 128 #or 192 or the default, 256

include(vendor/vendor.pri) #or qpmx stuff
```
