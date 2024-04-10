#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include "./Client/Client.h"
#include "./Server/Server.h"
#include "./RasberryPI/RasberryPI.h"

using namespace std;

bool testServer() {
    cout << "Testing Server... \n";

    if (placeHolderServer() != 0) { // Place holder
        cerr << "Place holder server test failed\n";
        return false;
    }

    return true;
}

bool testPI() {
    cout << "Testing Rasberry PI... \n";

    if (placeHolderPI() != 0) { // Place holder
        cerr << "Place holder Rasberry PI test failed\n";
        return false;
    }

    return true;
}

bool testClient() {
    cout << "Testing Client... \n";

    if (placeHolderClient() != 0) { // Place holder
        cerr << "Place holder client test failed\n";
        return false;
    }

    return true;
}

int main() {
    cout << "Tester started\n";

    if (!testClient() || !testServer() || !testPI()) {
        cerr << "Failures occured\n";
        return 1;
    }

    return 0;
}
