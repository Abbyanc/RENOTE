#pragma once
#include <string>

using namespace std;

class Cifrador {
public:
    string encriptar(string texto) {
        string secreto = "";
        for (char letra : texto) {
            secreto += (letra + 1);
        }
        return secreto;
    }

    string desencriptar(string texto) {
        string original = "";
        for (char letra : texto) {
            original += (letra - 1);
        }
        return original;
    }
};