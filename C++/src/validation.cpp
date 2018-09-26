#include "validation.h"

using namespace std;

bool isMenuInputStringValid(string option, char intervalBegin, char intervalEnd) {
    bool result = true;

    if (option.length() != 1) {
        result = false;
    } else {
        result = isSelectedOptionValid(option[0], intervalBegin, intervalEnd);
    }

    return result;
}

bool isSelectedOptionValid(char option, char intervalBegin, char intervalEnd) {
    return (option >= intervalBegin && option <= intervalEnd);
}

void printInvalidOptionMessage() {
    system ("clear");
    cout << "Opção inválida!" << endl;
    cout << "Pressione qualquer tecla para continuar..." << endl;
    cin.get();
    system ("clear");
}