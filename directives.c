#include <stdlib.h>
#include "directives.h"
#include "globals.h"

/*
    This functions recieves a char and encodes it to a data word.
    @param c The char to be encoded.
    @return The encoded data word.
*/
static data_w encode_char(char c);

/*
    This functions recieves an integer and encodes it to a data word.
    @param value The integer to be encoded.
    @return The encoded data word.
*/
static data_w encode_int_14_bits(short int value);

void encode_string(char str[], data_w *data_image, long *dc){
    int i;
    for(i = 0; str[i] != '\0'; i ++){
        data_image[(*dc)++] = encode_char(str[i]);
    }
    data_image[(*dc)++] = encode_char('\0');
}

static data_w encode_char(char c){
    data_w word = {0};
    unsigned int n = (unsigned int)c;
    word.data = n;
    return word;
}


void encode_data(int num_array[], int num_count, data_w *data_image, long *dc){
    
    int i;
    for(i = 0; i < num_count; i ++){
        data_image[(*dc)++] = encode_int_14_bits(num_array[i]);
    }
}


static data_w encode_int_14_bits(short int value) {
    data_w result;

    if (value < 0) {
        /* Convert negative value to two's complement representation */
        
        /* Convert to absolute value */
        unsigned int abs_value = abs(value);
        
        /* Complement and add one to get two's complement representation */
        unsigned int complement = (~abs_value) + 1;

        /* Set the most significant bit to indicate negative number */
        result.data = (1 << 13) | (complement & 0x1FFF);
    } else {
        /* Set positive value to binary representation */
        result.data = value & 0x1FFF;
    }

    return result;
}
