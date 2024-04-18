#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include "Encryption.h"

using namespace std;

int main() {
    string key = "ABCDEFGHIJKLMNOP";
    string in = "./Encryption/TestIn.txt";
    string out = "./Encryption/TestOut.txt";
    string finOut = "./Encryption/TestFinal.txt";
    Encrypt(in, out, key);
    Decrypt(out, finOut, key);
}