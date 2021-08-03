#ifndef WEIRD32_H
#define WEIRD32_H

/* Encodes the given 10-bit word in the weird 32 base, into the given buffer. */
void weirdEncodeWord(unsigned short word, char *buf);

/* Encodes the given integer in the weird 32 base, into the given buffer. */
void weirdEncodeNumber(int n, char *buf);

#endif /* WEIRD32_H */
