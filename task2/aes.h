#ifndef AES_H
#define AES_H

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;
//из 16 байт ключа генерирует 11 раундовых ключей по 16 байт
void keyExpansion(const unsigned char* key, unsigned char* expandedKey);
//указатель на 16 байт входного блока, указатель на 16 байт для зашифрованного блока
void encryptBlock(const unsigned char* input, const unsigned char* expandedKey, unsigned char* output);
//дополнительно выводит начальный State, промежуточные State после каждого раунда
void encryptBlockDebug(const unsigned char* input, const unsigned char* expandedKey, unsigned char* output);

#endif