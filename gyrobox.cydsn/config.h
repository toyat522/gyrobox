#include <stdio.h>

#if defined(__GNUC__)
// Add an explicit reference to the floating point printf library
// to allow usage of the floating point conversion specifiers.
asm(".global _printf_float");
#endif

#define USBFS_DEVICE 0u