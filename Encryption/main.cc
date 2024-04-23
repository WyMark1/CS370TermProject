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
    string in = "This is a test file to be encrypted ./,123455678910!@#$%^&*(()_+\n\n\n)\n;'";
    string out;
    string finOut;
    Encrypt(in, out, key);
    cout << "Encrypted: \n" << out << "\n";

    Decrypt(out, finOut, key);
    cout << "Decrypted: " << finOut << "\n";
}