#ifndef CALCTFT_H
#define CALCTFT_H

#include <Arduino.h>

enum class inputType
{
    digit,
    multiply,
    divide,
    add,
    subtract,
    equal,
    clear
};

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#endif  