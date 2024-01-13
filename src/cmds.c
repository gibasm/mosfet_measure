#include "cmds.h"
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

int scpi_cmd(int fd, const char* cmd_format, ...)
{
    va_list args;
    va_start(args, cmd_format); 
    
    char cmdbuf[256];

    vsnprintf(cmdbuf, 256, cmd_format, args);
    
    if(write(fd, cmdbuf, strlen(cmdbuf)) != strlen(cmdbuf)) 
        return -errno;

    puts(cmdbuf);

    va_end(args);
    
    return 0;
}
