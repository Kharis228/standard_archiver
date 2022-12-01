#pragma once
#include "CharCode.h"

class Archiver {
public:
    void NextCode(Code& code) {
        for (size_t i = code.size(); i > 0; --i) {
            code[i - 1] = !code[i - 1];
            if (code[i - 1]) {
                return;
            }
        }
    }
};