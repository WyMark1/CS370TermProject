#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include "Encryption.h"

using namespace std;

void blockEncrypt(ifstream &in, ofstream &out, string &key) {
    int blockSize = 16;
    char block[16];
    char moddedKey[16];
    string swapped;

    bool empty = false;
    if (in.peek() == ifstream::traits_type::eof()) empty = true;

    for (int i = 0; i < blockSize; ++i) {
        if (key[i] % 2 == 0)  moddedKey[i] = '0';
        else moddedKey[i] = '1';
        block[i] = '\x81';
    }

    while (in) {
        string unswapped;
        in.read(block, blockSize);
        for (int i = 0; i < blockSize; ++i) {
            block[i] = block[i] ^ key[i];
            unswapped += block[i];
            block[i] = '\x81';
        }
        int start = 0;
        int end = unswapped.length();
        while (end - start > 0) {
            if (moddedKey[start%blockSize] == '1') {
                char temp = unswapped[start];
                unswapped[start] = unswapped[end-1];
                unswapped[end-1] = temp;
                end--;
            }
            start++;
        }
        swapped += unswapped;
    }
    
    if (!empty) out << swapped;
}

void blockDecrypt(ifstream &in, ofstream &out, string &key) {
    int blockSize = 16;
    char moddedKey[16];
    string unswapped;
    char buffer[16];

    for (int i = 0; i < blockSize; ++i) {
        if (key[i] % 2 == 0) moddedKey[i] = '0';
        else moddedKey[i] = '1';
    }

    while (in.read(buffer, blockSize)) {
        int start = 0;
        int end = blockSize;
        while (end - start > 0) {
            if (moddedKey[start%blockSize] == '1') {
                char temp = buffer[start];
                buffer[start] = buffer[end-1];
                buffer[end-1] = temp;
                end--;
            }
            start++;
        }
        unswapped.append(buffer, blockSize);
    }

    int endOF = 0;
    bool padding = false;
    for (auto i = 0u; i < unswapped.length(); ++i) {
        unswapped[i] = unswapped[i] ^ key[i%blockSize];
        if (unswapped[i] == '\x81') {
            padding = true;
            endOF = i;
            break; 
        }
    }

    if (padding) out << unswapped.substr(0,endOF);
    else out << unswapped;
}

void Encrypt(string &in, string &out, string &key) {
    ofstream file_out;
    ifstream file_in(in);
    if (!file_in) throw runtime_error("Input File Does Not Exist\n");
    file_out.open(out);
    if (!file_out) throw runtime_error("Output File Not Found\n");
    if (key.length() != 16) throw runtime_error("Key needs to be length 16\n");
    blockEncrypt(file_in, file_out, key);
    file_out.close();
}

void Decrypt(string &in, string &out, string &key) {
    ofstream file_out;
    ifstream file_in(in);
    if (!file_in) throw runtime_error("Input File Does Not Exist\n");
    file_out.open(out);
    if (!file_out) throw runtime_error("Output File Not Found\n");
    if (key.length() != 16) throw runtime_error("Key needs to be length 16\n");
    blockDecrypt(file_in, file_out, key);
    file_out.close();
}

