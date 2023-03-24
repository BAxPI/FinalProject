#include "globals.h"

/*
    this function recieves a string and encodes it to the data image.
    @param str - The string to be encoded.
    @param data_image - The data image to be encoded to.
    @param dc - The data counter.
*/
void encode_string(char str[], data_w *data_image, long *dc);

/*
    this function recieves an array of integers and encodes it to the data image.
    @param num_array - The array of integers to be encoded.
    @param num_count - The number of integers in the array.
    @param data_image - The data image to be encoded to.
    @param dc - The data counter.
*/
void encode_data(int num_array[], int num_count, data_w *data_image, long *dc);