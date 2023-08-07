#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "my_malloc2.h"
u16 get_random(u16 max);
u8 whether_the_same_value(const void *p, u8 value, size_t length);

int main(void)
{
	srand(time(NULL));
	FILE *file = fopen("log", "w");
	setbuf(file, NULL);

	struct
	{
		void *p;
		u16 length;
	} test[MEMORY_LENGTH];
	memset(test, 0, sizeof(test));

	size_t i = 0, malloc_index = 0, free_index = 0;
	while (i < MEMORY_LENGTH)
	{
		if (i % 3 == 0 || i % 3 == 1)
		{
			u16 temp = get_random(MEMORY_LENGTH);
			test[malloc_index].p = my_malloc(temp);
			if (test[malloc_index].p)
			{
				fprintf(file, "malloc %d success\n", temp);
				test[malloc_index].length = temp;
				memset(test[malloc_index].p, test[malloc_index].length, test[malloc_index].length);
				malloc_index++;
			}
			else
			{
				fprintf(file, "malloc %d fail\n", temp);
				if (whether_can_malloc_the_size(temp))
				{
					fprintf(file, " FATAL ERROR! can malloc %d size at once but fail", temp);
					return 0;
				}
			}
		}
		else
		{
			if (!whether_the_same_value(test[free_index].p, test[free_index].length, test[free_index].length))
			{
				fprintf(file, "FATAL ERROR! The value is wrong!");
				return 0;
			}
			my_free(test[free_index].p);
			fprintf(file, "free\n\n");
			free_index++;
			// if (free_index == malloc_index) // 检验一下首项
			// {
			// 	switch (check_first_item())
			// 	{
			// 	case 1:
			// 		fprintf(file, "s->is_malloc error!");
			// 		return 0;
			// 	case 2:
			// 		fprintf(file, "s->before error!");
			// 		return 0;
			// 	case 3:
			// 		fprintf(file, "s->after error!");
			// 		return 0;
			// 	}
			// 	fclose(file);
			// 	file = fopen("log", "w");
			// }
		}
		i++;
	}
	printf("ok");
	return 0;
}

/**
 * @brief if the size ram is the same value return 1
 *
 * @param p
 * @param value
 * @param length
 * @return u8
 */
u8 whether_the_same_value(const void *p, u8 value, size_t length)
{
	while (length--)
	{
		if (((u8 *)p)[length] != value)
			return 0;
	}
	return 1;
}

/**
 * @brief Get the random object 1~max
 *
 * @param max
 * @return u16
 */
u16 get_random(u16 max)
{
	while (1)
	{
		u32 temp = rand();
		if (temp >= RAND_MAX / 10000 * 10000)
			continue;
		else
		{
			temp %= 10000;
			return ++temp;
		}
	}
}
