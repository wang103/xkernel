#ifndef TIMER_H
#define TIMER_H

#include "common.h"

#define PIT_DATA_0_PORT         0x40
#define PIT_DATA_1_PORT         0x41
#define PIT_DATA_2_PORT         0x42
#define PIT_COMMAND_PORT        0x43

#define PIT_MODE_REPEATING      0x36

#define CLOCK_MAX_FREQ_HZ       1193180
#define TIMER_DEFAULT_FREQ_HZ   100

void init_timer(uint32_t freq);

#endif
