#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include "./Client/Client.h"
#include "./Server/Server.h"
#include "./RasberryPI/RasberryPI.h"
#include "./Encryption/Encryption.h"

using namespace std;

bool testServer() {
    cout << "Testing Server... \n";

    if (placeHolderServer() != 0) { // Place holder
        cerr << "Place holder server test failed\n";
        return false;
    }
    cout << "Server Testing Complete... \n";
    return true;
}

bool testPI() {
    cout << "Testing Rasberry PI... \n";
    //runner()
    if (0 != 0) { // Place holder
        cerr << "Place holder Rasberry PI test failed\n";
        return false;
    }
    cout << "Rasberry PI Testing Complete... \n";
    return true;
}

bool testClient() {
    cout << "Testing Client... \n";

    if (placeHolderClient() != 0) { // Place holder
        cerr << "Place holder client test failed\n";
        return false;
    }
    cout << "Client Testing Complete... \n";
    return true;
}

bool compareFiles(const string& file1, const string& file2) {
    ifstream file_1(file1);
    ifstream file_2(file2);

    if (!file_1 || !file_2) {
        cerr << "Failed to open files for comparison.\n";
        return false;
    }

    // Compare contents
    char c1, c2;
    while (file_1.get(c1) && file_2.get(c2)) {
        if (c1 != c2) {
            cout << c1 << " :file1 " << c2 << " :file2\n";
            return false; // Files differ
        }
    }

    return true; // Files are identical
}

bool testEncryption() {
    cout << "Testing Encryption... \n";
    string key = "1234567891012131";
    string in = "./TestFiles/TestEncrypt.txt";
    string out = "./TestFiles/TestDecrypt.txt";
    string outFinal = "./TestFiles/TestDecryptFinal.txt";
    Encrypt(in, out, key);
    Decrypt(out, outFinal, key);
    if (!compareFiles(in, outFinal)) {
        cerr << "The Files don't match\n";
        return false;
    }

    cout << "Encryption Testing Complete... \n";
    return true;
}

int main() {
    cout << "Tester started\n";

    if (!testClient() || !testServer() || !testPI() || !testEncryption()) {
        cerr << "Failures occured\n";
        return 1;
    }

    return 0;
}
