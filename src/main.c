#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <argp.h>
#include <errno.h>
#include <string.h>
#include "tcp.h"
#include "serial.h"

#ifndef EXIT_SUCCESS 
#define EXIT_SUCCESS 0
#endif /* EXIT_SUCCESS */
#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif /* EXIT_FAILURE */

static const char prog_desc[] = "Use remote controlled power supply and multimeter to measure MOSFET characteristics";
static const char args_doc[] = "ARG1 ARG2";

struct prog_args
{
    union {
        struct {
            char* devpath;
            int baudrate;
        };
        struct {
            char* ipaddr;
            int port; 
        };
    } powersupply;
    int ps_uses_inet;

    union {
        struct {
            char* devpath;
            int baudrate;
        };
        struct {
            char* ipaddr;
            int port; 
        };
    } multimeter;
    int mm_uses_inet;
   
    char* config_path;    
};

static const struct argp_option options[] = {
    { "config", 'c', "FILEPATH", OPTION_ARG_OPTIONAL, "Use config file FILEPATH" },
    { "powersupply-serial", 'p', "DEVICEPATH", OPTION_ARG_OPTIONAL, "Use power supply device connected over USB/RS232" },
    { "pwbaudrate", 'b', "BAUDRATE", OPTION_ARG_OPTIONAL, "Use power supply device connected over USB/RS232" },
    { "powersupply", 'P', "IPADDR", OPTION_ARG_OPTIONAL, "Use remote controlled power supply over TCP" },
    { "pwport", 'o', "PORT", OPTION_ARG_OPTIONAL, "Power supply TCP port" },
    { "multimeter-serial", 'm', "DEVICEPATH", OPTION_ARG_OPTIONAL, "Use multimeter connected over USB/RS232" },
    { "mmbaudrate", 'B', "BAUDRATE", OPTION_ARG_OPTIONAL, "Baudrate for a multimeter connected over USB/RS232" },
    { "multimeter", 'M', "IPADDR", OPTION_ARG_OPTIONAL, "User remote controlled multimeter over TCP" }, 
    { "mmport", 'O', "PORT", OPTION_ARG_OPTIONAL, "Multimeter TCP port" },
    { 0 }
};

static error_t parse_opt(int key, char* arg, struct argp_state* state)
{
    struct prog_args *arguments = state->input;
    
    switch (key)
    {
        case 'c': 
            arguments->config_path = arg; 
            break; 
        case 'p':
            arguments->powersupply.devpath = arg;
            arguments->ps_uses_inet = 0;
            break;
        case 'b':
            arguments->powersupply.baudrate = atoi(arg);
            break;
        case 'P': 
            arguments->powersupply.ipaddr = arg;
            arguments->ps_uses_inet = 1;
            break;
        case 'o':
            arguments->powersupply.port = atoi(arg);
            break;
        case 'm':
            arguments->multimeter.devpath = arg;
            arguments->mm_uses_inet = 0;
            break;
        case 'B':
            arguments->multimeter.baudrate = atoi(arg);
            break;
        case 'M':
            arguments->multimeter.ipaddr = arg;
            arguments->mm_uses_inet = 1;
            break;
        case 'O':
            arguments->multimeter.port = atoi(arg);
            break;
        case ARGP_KEY_ARG:
            break;
        case ARGP_KEY_END:
            break;

        default:
            return ARGP_ERR_UNKNOWN;
    }

    return EXIT_SUCCESS;
}

static struct argp argp = { options, parse_opt, args_doc, prog_desc };

static void sigint_handler(int sig);

static int ps_fd = -1, mm_fd = -1;

static int open_devices(struct prog_args* args);

int main(int argc, char** argv) 
{
    signal(SIGINT, sigint_handler);

    struct prog_args args;
    
    args.config_path = NULL;

    argp_parse(&argp, argc, argv, 0, 0, &args);

    if(open_devices(&args) == EXIT_FAILURE)
        return EXIT_FAILURE;
   
    const char* identify_cmd = "*IDN?\n"; 
    char buf[64];

    write(mm_fd, identify_cmd, strlen(identify_cmd));
    ssize_t ret = read(mm_fd, buf, 64);
    if(ret != -1) {
        buf[ret] = 0;
        printf("Multimeter ID:\t%s\n", buf);
    }

    write(ps_fd, identify_cmd, strlen(identify_cmd)-1);
    ret = read(ps_fd, buf, 22);
    if(ret != -1) {
        buf[ret] = 0;
        printf("Power supply ID:\t%s\n", buf);  
    }
    
    close(ps_fd);
    close(mm_fd);
    return EXIT_SUCCESS;
}

static int open_devices(struct prog_args* args)
{ 
    assert(args);

    if(args->mm_uses_inet) {
        mm_fd = tcp_connect(args->multimeter.ipaddr, args->multimeter.port);   
    } else {
        speed_t baudrate = args->multimeter.baudrate == 9600 ? B9600 : B115200;
        mm_fd = serial_open(args->multimeter.devpath, baudrate); 
    }
    
    if(mm_fd == -1) {
        fprintf(stderr, "Error while connecting to multimeter: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    if(args->ps_uses_inet) {
        ps_fd = tcp_connect(args->powersupply.ipaddr, args->powersupply.port);   
    } else {
        speed_t baudrate = args->powersupply.baudrate == 9600 ? B9600 : B115200;
        ps_fd = serial_open(args->powersupply.devpath, baudrate); 
    }

    if(ps_fd == -1) {
        close(ps_fd);
        fprintf(stderr, "Error while connecting to power supply: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static void sigint_handler(int sig) {
    if(ps_fd != -1)
        close(ps_fd);
    if(mm_fd != -1)
        close(mm_fd);
}
