#ifndef _CMDS_H
#define _CMDS_H 

int scpi_cmd(int fd, const char* cmd_format, ...);

// macros for power supply commands
#define SET_VOLT(voltage) scpi_cmd(ps_fd, "VSET1:%.2f", voltage)
#define SET_CURR(current) scpi_cmd(ps_fd, "ISET1:%.2f", current)

// macros for multimeter commands
#define MEAS_VOLT_RANGE(type, range) scpi_cmd(mm_fd, "MEAS:VOLT:%s? %s\r\n", type, range)
#define MEAS_VOLT_DC_RANGE(range) MEAS_VOLT_RANGE("DC", range)
#define MEAS_VOLT_AC_RANGE(range) MEAS_VOLT_RANGE("AC", range)

#endif /* _CMDS_H */ 
