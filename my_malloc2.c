#include "my_malloc2.h"

u8 memory[MEMORY_LENGTH];

#define is_legal(x) ((char *)(x) < (char *)memory + sizeof(memory))

#pragma pack(1)
struct _malloc_struct
{
    u8 is_malloc;
    u16 before; // 这一个结构体到下一个结构体距离
    u16 after;
};
#pragma pack()
static u8 check_this_struct(struct _malloc_struct *p, u16 length);

void *my_malloc(size_t length)
{
    if (!length)
        return 0;

    static u8 is_init = 0;
    struct _malloc_struct *p = (struct _malloc_struct *)memory, *previous_p = NULL,
                          *insert_p = NULL, *later_p = NULL;

    if (!is_init)
    {
        p->after = sizeof(memory);
        is_init = 1;
    }

    while (!check_this_struct(p, length))
    {
        previous_p = p;
        p = (struct _malloc_struct *)((char *)p + p->after);
        if ((char *)p >= (char *)memory + MEMORY_LENGTH)
            return 0; // 满了
    }

    // 找到
    p->is_malloc = 1;
    p->before = previous_p == NULL ? 0 : previous_p->after;
    later_p = (struct _malloc_struct *)((char *)p + p->after);
    insert_p = (struct _malloc_struct *)((char *)p + sizeof(struct _malloc_struct) + length); // 创建一个later结构malloc数据结构
    if (insert_p != later_p)
    {
        // 可以容得下insert_p
        insert_p->is_malloc = 0;
        insert_p->before = sizeof(struct _malloc_struct) + length;
        insert_p->after = p->after - sizeof(struct _malloc_struct) - length;
        if (is_legal(later_p))
            later_p->before = insert_p->after;
        p->after = insert_p->before;
    }
    return (char *)p + sizeof(struct _malloc_struct);
}

void my_free(void *p)
{
    struct _malloc_struct *s = (struct _malloc_struct *)((char *)p - sizeof(struct _malloc_struct));
    struct _malloc_struct *previous = (struct _malloc_struct *)((char *)s - s->before),
                          *later = (struct _malloc_struct *)((char *)s + s->after);
    s->is_malloc = 0;
    if (is_legal(later) && !later->is_malloc) // later存在且空
    {
        s->after += later->after;
        later = (struct _malloc_struct *)((char *)s + s->after);
        if (is_legal(later))
            later->before = s->after;
    }
    if (((void *)s != (void *)memory) && !previous->is_malloc) // 前者存在且空
    {
        previous->after += s->after;
        if (is_legal(later)) // 后者存在
            later->before = previous->after;
    }
}

u16 whether_can_malloc_the_size(u16 length)
{
    struct _malloc_struct *p = (struct _malloc_struct *)memory;
    while ((char *)p - (char *)memory < MEMORY_LENGTH)
    {
        if (!p->is_malloc)
        {
            if (length + sizeof(struct _malloc_struct) == p->after)
                return 1;
            if (length + 2 * sizeof(struct _malloc_struct) <= p->after)
                return 1;
        }
        p = (struct _malloc_struct *)((char *)p + p->after);
    }
    return 0;
}

u8 check_first_item(void)
{
    struct _malloc_struct *s = (struct _malloc_struct *)memory;
    if (s->is_malloc)
        return 1;
    if (s->before)
        return 2;
    if (s->after != MEMORY_LENGTH)
        return 3;
    return 0;
}

/**
 * @brief 检验当前结构是否可以满足申请大小要求
 *
 * @param p
 * @param length
 * @return u8
 */
static u8 check_this_struct(struct _malloc_struct *p, u16 length)
{
    if (p->is_malloc)
        return 0;
    if (p->after < sizeof(struct _malloc_struct) + length)
        return 0;
    if (p->after == sizeof(struct _malloc_struct) + length)
        return 1;
    if (p->after > sizeof(struct _malloc_struct) + length &&
        p->after < 2 * sizeof(struct _malloc_struct) + length)
        return 0;
    return 1;
}
