#ifndef COMMON_H
#define COMMON_H

#define NULL 0

typedef unsigned int    uint32_t;
typedef          int    int32_t;
typedef unsigned short  uint16_t;
typedef          short  int16_t;
typedef unsigned char   uint8_t;
typedef          char   int8_t;

#define PANIC(msg)  panic(msg, __FILE__, __LINE__);

#define offsetof(TYPE, MEMBER) ((uint32_t) &((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type, member) );})

void panic(const char *msg, const char *file, uint32_t line);

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t inl(uint16_t port);

void memset(uint8_t *dest, uint8_t val, uint32_t len);

void enable_interrupts();
void disable_interrupts();

static inline int is_power_of_2(unsigned int n) {
    return (n != 0 && ((n & (n - 1)) == 0));
}

#endif
