/*
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>

#include "include/tap_ioctl.h"


int main(int argc, char **argv)
{
    int 		dev_file, ret_val;
    unsigned long 	ctl_data;
    
    /* open the device file */
    dev_file = open(DEVICE_NAME, 0);
    if (dev_file < 0) {
        printf("Can't open device file: %s (%d)\n", DEVICE_NAME, dev_file);
        exit(-1);
    }
    
    while (1) {
        // first get the value we set last time
        ret_val = ioctl(dev_file, IOCTL_GET_REGS, &ctl_data);
		    
        usleep(200);
        
        // then update the bits
        if ((ctl_data & 0x00ff0000) == 0x00000000) {
            ctl_data |= 0x00ff0000;
        } else if ((ctl_data & 0x00ff0000) == 0x00ff0000) {
            ctl_data &= 0xff00ffff;
        }
        
        ret_val = ioctl(dev_file, IOCTL_SET_REGS, &ctl_data);
        
        sleep(2);
    }
    
    /* done, clean up */
    close(dev_file);
    
    return(ret_val);
}
