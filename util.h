#ifndef _UTIL_H
#define _UTIL_H
void dump(void *bfr, int count);
char *printBits(unsigned char in);
char *printBits(char in);
char *printBits(unsigned short in);
char *printBits(short in);

void swap2(void *inPtr, void *outPtr);
void swap4(void *inPtr, void *outPtr);
void swap8(void *inPtr, void *outPtr);

void decodeBER(void *ptr, unsigned int *nBytesMsg, unsigned int *nBytesLen);
int  encodeBER(int len, void *out, int outSize);

double getDoubleTime(void);
#endif // _UTIL_H
