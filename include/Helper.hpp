#pragma once
#include <Arduino.h>
#include "GlobalConstants.hpp"

// Outputs the given Data on the Debug-Output if DEBUG is defined

#define DEBUG_PRINT(X)      \
    if (!Serial)            \
    {                       \
        Serial.begin(9600); \
        Serial.println();   \
        delay(1000);        \
    }                       \
    Serial.print((X));
#define DEBUG_PRINTLN(X)    \
    if (!Serial)            \
    {                       \
        Serial.begin(9600); \
        Serial.println();   \
        delay(1000);        \
    }                       \
    Serial.println((X));

unsigned int power(unsigned short num, unsigned short exp);

// converts the binary-data b to the decimal equivalent
unsigned int bin_to_dec(byte b);

String to_bin(byte b);

/*
    checks wether the char c is a hex-char(1-9,a-f|A-F)
*/
bool is_hex_char(char c);
