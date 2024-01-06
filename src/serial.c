#include "serial.h"
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <stdio.h>

#define PRINTF_FL() \
    fprintf(stderr, "%s:%d: ", __FILE__, __LINE__)

int serial_open(const char* path, speed_t baudrate)
{
    int fd;
    struct termios tty;

    assert(path);

    fd = open(path, O_RDWR | O_NOCTTY);

    if(fd == -1) {
        PRINTF_FL();
        return fd;
    }

    if(tcgetattr(fd, &tty) != 0) {
        PRINTF_FL();
        close(fd);
        return -1;
    }

    tty.c_cflag = CS8;
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 0;

    tty.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
                    INLCR | PARMRK | INPCK | ISTRIP | IXON);

    tty.c_oflag = 0;
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);

    cfsetispeed(&tty, baudrate);
    cfsetospeed(&tty, baudrate);

    if(tcsetattr(fd, TCSANOW, &(tty)) != 0) {
        PRINTF_FL();
        close(fd);
        return -1;
    }

    return fd;
}
