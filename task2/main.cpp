#include "aes.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <iomanip>

using namespace std;

typedef vector<unsigned char> ByteArray;

const int BLOCK_SIZE = 16;

// Вывод hex выводит массив байтов в шестнадцатеричном виде.
void printHex(const ByteArray& data, const string& label = "") {
    if (!label.empty()) cout << label << ": ";
    for (unsigned char b : data) cout << hex << setw(2) << setfill('0') << (int)b;
    cout << dec << endl;
}

// побайтовый XOR. Используется в OFB для шифрования/расшифрования.
ByteArray xorBytes(const ByteArray& a, const ByteArray& b) {
    ByteArray result(a.size());
    for (size_t i = 0; i < a.size(); i++) result[i] = a[i] ^ b[i];
    return result;
}

// PKCS7 паддинг добавляет байты, чтобы длина стала кратной blockSize
ByteArray pkcs7Pad(const ByteArray& input, size_t blockSize) {
    size_t padLen = blockSize - (input.size() % blockSize);
    if (padLen == 0) padLen = blockSize;
    ByteArray output = input;
    for (size_t i = 0; i < padLen; i++) output.push_back((unsigned char)padLen);
    return output;
}

// PKCS7 анпаддинг
//удаляет добавленные байты (по последнему байту, который хранит длину паддинга)
ByteArray pkcs7Unpad(const ByteArray& input) {  
    if (input.empty()) return input;
    unsigned char padLen = input.back();
    if (padLen < 1 || padLen > 16) return input;
    return ByteArray(input.begin(), input.end() - padLen);
}

// Строка - байты
ByteArray stringToBytes(const string& str) {
    ByteArray result(str.begin(), str.end());
    return result;
}

// Байты - строка
string bytesToString(const ByteArray& bytes) {
    return string(bytes.begin(), bytes.end());
}

// Случайные байты  генерирует len случайных байт
ByteArray randomBytes(size_t len) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);
    ByteArray result(len);
    for (size_t i = 0; i < len; i++) result[i] = (unsigned char)dis(gen);
    return result;
}

// Вывод всех раундовых ключей
void printRoundKeys(const unsigned char* expandedKey) {
    cout << "\nРАУНДОВЫЕ КЛЮЧИ\n";
    for (int round = 0; round <= 10; round++) {
        cout << "Round " << round << ": ";
        for (int i = 0; i < 16; i++) {
            cout << hex << setw(2) << setfill('0') << (int)expandedKey[round * 16 + i];
        }
        cout << dec << endl;
    }
}

// OFB шифрование с отладочным выводом (только для первого блока)
ByteArray ofbEncryptDebug(const ByteArray& plaintext, const ByteArray& key, ByteArray& iv) {
    if (iv.empty()) iv = randomBytes(BLOCK_SIZE);
    
    cout << "\nВЕКТОР ИНИЦИАЛИЗАЦИИ (IV)\n";
    printHex(iv, "IV");
    
    ByteArray padded = pkcs7Pad(plaintext, BLOCK_SIZE);
    
    unsigned char expandedKey[176];
    keyExpansion(key.data(), expandedKey);
    printRoundKeys(expandedKey);
    
    ByteArray ciphertext;
    ByteArray feedback = iv; //текущий вход для AES (начинается с IV)
    unsigned char keystream[BLOCK_SIZE];
    bool firstBlock = true;
    
    for (size_t i = 0; i < padded.size(); i += BLOCK_SIZE) {
        if (firstBlock) {
            cout << "\nПЕРВЫЙ БЛОК\n";
            cout << "Feedback до шифрования: ";
            printHex(feedback);
            
            encryptBlockDebug(feedback.data(), expandedKey, keystream);
            
            cout << "Сгенерированный keystream: ";
            printHex(ByteArray(keystream, keystream + BLOCK_SIZE));
            firstBlock = false;
        } else {
            encryptBlock(feedback.data(), expandedKey, keystream);
        }
        
        ByteArray block(padded.begin() + i, padded.begin() + i + BLOCK_SIZE);
        ByteArray encrypted = xorBytes(block, ByteArray(keystream, keystream + BLOCK_SIZE));
        ciphertext.insert(ciphertext.end(), encrypted.begin(), encrypted.end());
        feedback.assign(keystream, keystream + BLOCK_SIZE);
    }
    
    return ciphertext;
}

// OFB расшифрование (без отладки)
ByteArray ofbDecrypt(const ByteArray& ciphertext, const ByteArray& key, const ByteArray& iv) {
    unsigned char expandedKey[176];
    keyExpansion(key.data(), expandedKey);
    
    ByteArray plaintextPadded;
    ByteArray feedback = iv;
    unsigned char keystream[BLOCK_SIZE];
    
    for (size_t i = 0; i < ciphertext.size(); i += BLOCK_SIZE) {
        encryptBlock(feedback.data(), expandedKey, keystream);
        ByteArray block(ciphertext.begin() + i, ciphertext.begin() + i + BLOCK_SIZE);
        ByteArray decrypted = xorBytes(block, ByteArray(keystream, keystream + BLOCK_SIZE));
        plaintextPadded.insert(plaintextPadded.end(), decrypted.begin(), decrypted.end());
        feedback.assign(keystream, keystream + BLOCK_SIZE);
    }
    return pkcs7Unpad(plaintextPadded);
}

int main() {
    cout << "AES-128 OFB\n\n";
    
    // Выбор ключа
    cout << "1. Ввести ключ\n2. Сгенерировать случайный\nВыбор: ";
    int choice;
    cin >> choice;
    cin.ignore();
    
    ByteArray key;
    if (choice == 2) {
        key = randomBytes(16);
        cout << "\nСлучайный ключ: ";
        printHex(key);
    } else {
        string keyStr;
        cout << "\nВведите ключ (16 символов): ";
        getline(cin, keyStr);
        key = stringToBytes(keyStr);
        key.resize(16, 0);
        cout << "Ключ: ";
        printHex(key);
    }
    
    // Ввод текста (многострочный, пустая строка для завершения)
    cout << "\nВведите текст (пустая строка для завершения):\n";
    string text, line;
    while (getline(cin, line) && !line.empty()) {
        if (!text.empty()) text += '\n';
        text += line;
    }
    
    if (text.empty()) {
        cout << "Ошибка: текст не введён\n";
        return 1;
    }
    
    ByteArray plaintext = stringToBytes(text);
    
    // Шифрование с отладкой
    ByteArray iv;
    ByteArray ciphertext = ofbEncryptDebug(plaintext, key, iv);
    
    cout << "\nИТОГОВЫЙ ШИФРОТЕКСТ\n";
    cout << "Шифротекст: ";
    printHex(ciphertext);
    
    // Сохранение ключа и IV
    ofstream file("aes_key_iv.txt");
    file << "AES-128 OFB\n";
    file << "Key: ";
    for (unsigned char b : key) file << hex << setw(2) << setfill('0') << (int)b;
    file << "\nIV: ";
    for (unsigned char b : iv) file << hex << setw(2) << setfill('0') << (int)b;
    file << "\n";
    file.close();
    cout << "\nКлюч и IV сохранены в aes_key_iv.txt\n";
    
    // Расшифрование
    ByteArray decrypted = ofbDecrypt(ciphertext, key, iv);
    string decryptedText = bytesToString(decrypted);
    
    cout << "\nРАСШИФРОВАНИЕ\n";
    cout << "Расшифрованный текст:\n" << decryptedText << "\n";
    
    if (text == decryptedText) {
        cout << "\nУспех! Текст совпадает.\n";
    } else {
        cout << "\nОшибка! Текст не совпадает.\n";
    }
    
    return 0;
}