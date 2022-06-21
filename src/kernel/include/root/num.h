#ifndef quuxNUMHxuuq
#define quuxNUMHxuuq

#ifdef __cplusplus
extern "C" {
#endif

char *intToString(char *buffer, int i, int base);
char *longToString(char *buffer, long l, int base);
char *longLongToString(char *buffer, long long l, int base);

char *uintToString(char *buffer, unsigned int i, int base);
char *ulongToString(char *buffer, unsigned long l, int base);
char *ulongLongToString(char *buffer, unsigned long long l, int base);

#ifdef __cplusplus
}
#endif

#endif
