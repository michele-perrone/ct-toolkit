#include "binarytools.hpp"

short int BinaryTools::read_short_from_binary_vector(std::vector<bool> bits, int vector_index)
{
    // By default the number is assumed positive (filled with 0-s)
    short int number = 0;

    // However if the first bit is 1, then the number is negative (2s complement)...
    if((bits[vector_index] & 1) == 1)
    {
        // ... and it's necessary to "fill it" with 1-s until the MSB.
        number = ~number;
        number = number << N_OF_BITS_PIXEL;
    }

    for (int shift_index = N_OF_BITS_PIXEL-1; shift_index >= 0; shift_index--)
    {
        number |= (bits[vector_index] << shift_index);
        vector_index++;
    }

    return number;
}

uint8_t BinaryTools::read_byte_from_binary_vector(std::vector<bool> bits, unsigned long vector_index)
{
    uint8_t byte = 0;
    for (int shift_index = N_OF_BITS_BYTE-1; shift_index >= 0; shift_index--)
    {
        byte |= (bits[vector_index] << shift_index);
        vector_index++;
    }

    return byte;
}

void BinaryTools::push_short_to_binary_vector(short int number, std::vector<bool>* destination)
{
    for (int shift_index = N_OF_BITS_PIXEL-1; shift_index >= 0; shift_index--)
    {
        destination->push_back((number >> shift_index) & 1);
    }
}

void BinaryTools::push_byte_to_binary_vector(uint8_t byte, std::vector<bool>* destination)
{
    for (int shift_index = N_OF_BITS_BYTE-1; shift_index >= 0; shift_index--)
    {
        destination->push_back((byte >> shift_index) & 1);
    }
}
