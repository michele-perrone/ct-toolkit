#include <iostream>
#include <vector>
#include <bitset>

#define N_OF_BITS_PIXEL 12
#define N_OF_BITS_BYTE 8

namespace BinaryTools
{
    // Read a value from a binary vector
    short int read_short_from_binary_vector(std::vector<bool> bits, int vector_index);
    uint8_t read_byte_from_binary_vector(std::vector<bool> bits, unsigned long vector_index);

    // Write a value to a binary vector
    void push_short_to_binary_vector(short int number, std::vector<bool>* destination);
    void push_byte_to_binary_vector(uint8_t byte, std::vector<bool>* destination);
}


