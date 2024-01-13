#ifndef _CMDS_H
#define _CMDS_H 

int scpi_cmd(int fd, const char* cmd_format, ...);

#define SET_VOLT(voltage) scpi_cmd(ps_fd, "VSET1:%.2f", voltage);
#define SET_CURR(current) scpi_cmd(ps_fd, "ISET1:%.2f", current);

#endif /* _CMDS_H */ 
