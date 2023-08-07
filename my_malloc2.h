#ifndef _MY_MALLOC2_H_
#define _MY_MALLOC2_H_

#include <stdint.h>
#include <stdlib.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define MEMORY_LENGTH 10000

void *my_malloc(size_t length);
void my_free(void *p);
u16 whether_can_malloc_the_size(u16 length);
u8 check_first_item(void);

#endif
