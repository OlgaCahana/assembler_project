#include "weird32.h"

#define BITS_PER_DIGIT 5
#define DIGIT_BIT_MASK 0x1F

static char digits[32] = {
        '!', '@', '#', '$',
        '%', '^', '&', '*',
        '<', '>', 'a', 'b',
        'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j',
        'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r',
        's', 't', 'u', 'v'
};

/* Encodes the given 10-bit word in the weird 32 base, into the given buffer. */
void weirdEncodeWord(unsigned short word, char *buf) {
    buf[0] = digits[(word >> BITS_PER_DIGIT) & DIGIT_BIT_MASK];
    buf[1] = digits[word & DIGIT_BIT_MASK];
    buf[2] = '\0';
}

/* Encodes the given integer in the weird 32 base, into the given buffer. */
void weirdEncodeNumber(int n, char *buf) {
    int i = 0;
    int j = 0;

    /* Calculate how many digits are needed */
    do {
        i++;
    } while ((n >> (i * BITS_PER_DIGIT)) != 0);

    buf[i] = '\0';

    /* Encode digits in reverse order */
    for (j = i-1; j >= 0; j--) {
        buf[j] = digits[(n >> ((i-j-1) * BITS_PER_DIGIT)) & DIGIT_BIT_MASK];
    }
}