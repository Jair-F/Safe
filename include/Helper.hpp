#pragma once

unsigned int power(unsigned short num, unsigned short exp)
{
    if (exp == 0)
    {
        return 1;
    }
    if (exp > 1)
    {
        return num * power(num, exp - 1);
    }
    else
    {
        return num;
    }
}

// converts the binary-data b to the decimal equivalent
unsigned int bin_to_dec(byte b)
{
    constexpr unsigned short LEN_OF_BYTE = sizeof(byte) * 8;
    unsigned short ret = 0;

    // extracting the single bytes
    bool bytes[LEN_OF_BYTE];
    for (unsigned short i = 0; i < LEN_OF_BYTE; ++i)
    {
        bool tmp;
        if (b & 1)
        { // bitwise OR - to extract the last bit
            tmp = 1;
        }
        else
        {
            tmp = 0;
        }
        bytes[LEN_OF_BYTE - 1 - i] = tmp; // add the values from the back to front - taken the values also from back to front
        b = b >> 1;
    }
    for (unsigned short i = 0; i < LEN_OF_BYTE; ++i)
    {
        // Serial.print(bytes[i]);
        if (bytes[LEN_OF_BYTE - 1 - i])
        { // begin from back
            ret += power(2, i);
        }
    }
    // Serial.println();
    return ret;
}

String to_bin(byte b)
{
    constexpr unsigned short LEN_OF_BYTE = sizeof(byte) * 8;
    String ret;

    // extracting the single bytes
    bool bytes[LEN_OF_BYTE];
    for (unsigned short i = 0; i < LEN_OF_BYTE; ++i)
    {
        bool tmp;
        if (b & 1)
        { // bitwise OR - to extract the last bit
            tmp = 1;
        }
        else
        {
            tmp = 0;
        }
        bytes[LEN_OF_BYTE - 1 - i] = tmp; // add the values from the back to front - taken the values also from back to front
        b = b >> 1;
    }
    for (unsigned short i = 0; i < LEN_OF_BYTE; ++i)
    {
        ret += (bytes[i] == true ? '1' : '0');
    }
    return ret;
}

/*
    checks wether the char c is a hex-char(1-9,a-f|A-F)
*/
bool is_hex_char(char c)
{
    return ((c <= '9' && c >= '0') && (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}
