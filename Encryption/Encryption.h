#ifndef ENCRYPTION_H_INCLUDED
#define ENCRYPTION_H_INCLUDED
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stdexcept>

void Encrypt(std::string &in, std::string &out, std::string &key);

void Decrypt(std::string &in, std::string &out, std::string &key);

#endif /* ENCRYPTION_H_INCLUDED */