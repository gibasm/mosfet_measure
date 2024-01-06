#ifndef _SERIAL_H
#define _SERIAL_H

#include <termios.h>

int serial_open(const char* path, speed_t baudrate);

#endif /* _SERIAL_H */
