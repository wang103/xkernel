#include "monitor.h"

static uint16_t *video_memory = (uint16_t *)0xB8000;
static int cursor_x = 0;
static int cursor_y = 0;

/**
 * Update the monitor cursor.
 */
static void move_cursor() {
    uint16_t cursor_loc = cursor_y * SCRN_W + cursor_x;
    outb(VGA_CMD_PORT, 14);     // setting the high cursor byte
    outb(VGA_DAT_PORT, cursor_loc >> 8);
    outb(VGA_CMD_PORT, 15);     // setting the low cursor byte
    outb(VGA_DAT_PORT, cursor_loc);
}

/**
 * Scroll the text on the screen up by one row.
 */
static void scroll() {
    if (cursor_y >= SCRN_H) {
        int i;
        for (i = 0; i < (SCRN_H - 1) * SCRN_W; i++) {
            video_memory[i] = video_memory[i + SCRN_W];
        }

        // Clear the last line.
        for (i = (SCRN_H - 1) * SCRN_W; i < SCRN_H * SCRN_W; i++) {
            video_memory[i] = CHAR_BLANK;
        }

        cursor_y = SCRN_H - 1;
    }
}

/**
 * Clear the screen.
 */
void monitor_clear() {
    int i;
    for (i = 0; i < SCRN_H * SCRN_W; i++) {
        video_memory[i] = CHAR_BLANK;
    }

    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

/**
 * Write a single character on the screen.
 */
void monitor_putchar(char c) {
    if (c == 0x08 && cursor_x) {
        // Handle a backspace, by moving the cursor back one space.
        cursor_x--;
    }
    else if (c == 0x09) {
        // Handle a tab by increasing cursor_x, but it is still divisible by 8.
        cursor_x = (cursor_x + 8) & ~(8 - 1);
    }
    else if (c == '\r') {
        // Handle carriage return.
        cursor_x = 0;
    }
    else if (c == '\n') {
        // Handle newline.
        cursor_x = 0;
        cursor_y++;
    }
    else if (c >= ' ') {
        // Handle other printable character.
        uint16_t *loc = video_memory + (cursor_y * SCRN_W + cursor_x);
        *loc = c | (CHAR_ATTRIBUTE << 8);
        cursor_x++;
    }

    if (cursor_x >= SCRN_W) {
        cursor_x = 0;
        cursor_y++;
    }

    scroll();
    move_cursor();
}

/**
 * Write a decimal number on the screen.
 */
void monitor_putdec(uint32_t i) {
    if (i == 0) {
        monitor_putchar('0');
        return;
    }

    uint32_t leftover = i / 10;
    char digit = '0' + i - leftover * 10;

    if (leftover != 0) {
        monitor_putdec(leftover);
    }

    monitor_putchar(digit);
}

/**
 * Write a hex-decimal number on the screen.
 */
void monitor_puthex(uint32_t i) {
    if (i == 0) {
        monitor_putchar('0');
        return;
    }

    uint32_t leftover = i / 16;
    uint32_t digit = i - leftover * 16;

    if (leftover != 0) {
        monitor_puthex(leftover);
    }

    if (digit <= 9) {
        monitor_putchar('0' + digit);
    } else {
        monitor_putchar('A' + digit - 10);
    }
}

/**
 * Write a string of characters on the screen.
 */
void monitor_put(char *c) {
    int i = 0;
    while (c[i]) {
        monitor_putchar(c[i++]);
    }
}
