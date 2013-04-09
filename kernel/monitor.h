#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

#define VGA_CMD_PORT    0x3D4
#define VGA_DAT_PORT    0x3D5

#define BACKGROUND_COLOR    07      // light gray
#define FOREGROUND_COLOR    14      // light brown
#define CHAR_ATTRIBUTE      ((BACKGROUND_COLOR << 4) | FOREGROUND_COLOR)
#define CHAR_BLANK          (0x20 | (CHAR_ATTRIBUTE << 8))

#define SCRN_H  25
#define SCRN_W  80

void monitor_clear();
void monitor_putchar(char c);
void monitor_put(char *c);

#endif
