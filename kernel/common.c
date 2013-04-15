#include "common.h"
#include "monitor.h"

/**
 * Raise a kernel panic.
 */
void panic(const char *msg, const char *file, uint32_t line) {
    asm volatile("cli");

    monitor_put("KERNEL PANIC(");
    monitor_put((char *)msg);
    monitor_put(") at ");
    monitor_put((char *)file);
    monitor_put(":");
    monitor_putdec(line);
    monitor_putchar('\n');

    // Going into infinite loop.
    for(; ;);
}

/**
 * Write a byte out to the specified port.
 */
void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a" (value), "Nd" (port));
}

/**
 * Read a byte from the specified port.
 */
uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a" (ret) : "Nd" (port));
    return ret;
}

/**
 * Read a word (two bytes) from the specified port.
 */
uint16_t inw(uint16_t port) {
    uint16_t ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "Nd" (port));
    return ret;
}

/**
 * Read a DW (four bytes) from the specified port.
 */
uint32_t inl(uint16_t port) {
    uint32_t ret;
    asm volatile ("inl %1, %0" : "=a" (ret) : "Nd" (port));
    return ret;
}

/**
 * Write len copies of val into dest.
 */
void memset(uint8_t *dest, uint8_t val, uint32_t len) {
    uint8_t *cur = dest;
    for (; len != 0; len--) {
        *cur++ = val;
    }
}

void enable_interrupts() {
    __asm__ __volatile__("sti");
}

void disable_interrupts() {
    __asm__ __volatile__("cli");
}
