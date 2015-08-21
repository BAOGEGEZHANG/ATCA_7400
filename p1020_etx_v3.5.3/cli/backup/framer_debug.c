/*
 * Copyright (c) 2010-2015 Qice Technology Ltd, Beijing, China.
 * All rights reserved.
 *
 * This source code is proprietary to Qice Technology Limited and no part
 * of it may be redistributed, published or disclosed except as outlined in
 * the written contract supplied with this product.
 *
 *******************************************************************************
 * File Name  		: 	reg_debug.c
 * Description		:	this file demos how to use the necessary APIs to capture
 *                             network data. You can dump the captured data or save 
 *                             them into a log file.
 *******************************************************************************/

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* C Standard Library headers. */
#include <ctype.h>
#include <errno.h>
#include <regex.h>

/* POSIX headers. */
#include <fcntl.h>
#include <netinet/in.h>    /* for ntohs() */
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "include/qnfapi.h"


static char qnfname[20] = "/dev/";


int main(int argc, char *argv[])
{
    int qnffd = -1;
    unsigned int cnum = 0;
    unsigned int menu;
    unsigned int addr;
    unsigned int value;
    
    char tmpname[10];
    
    if (argc != 2)
    {
        printf("Wrong args, please specify which card to access\n");
        exit(0);
    }
    
    strncpy(tmpname, argv[1], 3);
    if (strncmp(tmpname, "qnf", 3))
    {
        printf("Wrong card name, should be qnf.\n");
        exit(0);
    }
    
    if (!isdigit(argv[1][3]))
    {
        printf("Wrong card number, should be 0 or 1.\n");
        exit(0);
    }        
    strncpy(tmpname,argv[1]+3,5);
    sscanf(tmpname,"%u",&cnum);
    if (cnum > 9)
    {
        printf("Unsupported card number %u, must be in 0-9.\n",cnum);
        exit(0);
    }
    strncat(qnfname,argv[1],20);
        
    if((qnffd = qnf_open(qnfname)) < 0)
    {
        printf("Fail to open device %s.\n",qnfname);
        return 0;
    }
    
    while(1)
    {
        printf("1: FPGA Reg Write \n");
        printf("2: FPGA Reg Read \n");
        printf("3: Meta Reg Test \n");
        printf("4: exit \n");
        
        scanf("%d",&menu);
        
        switch (menu)
	 { 
            case 1: 
		    printf("Input FPGA Reg address: \n");
		    scanf("%x",&addr);
                    addr += 0x100000;
		    printf("input value: \n");
		    scanf("%x",&value);
		    reg_write(qnffd,addr, value);
		    break;
            case 2:
		    printf("Input FPGA Reg address: \n");
		    scanf("%x",&addr);
                    addr += 0x100000;
 		    value = reg_read(qnffd,addr);
		    printf("FPGA reg: 0x%x value is 0x%x\n",addr,value);
		    break;
	    case 3:
	            reg_write(qnffd, 0x100044, 0x0);
		    reg_write(qnffd, 0x100040, 0x5);
		    value = reg_read(qnffd, 0x10004c);
		    printf("value is: 0x%x\n", value);
 		    break;
            case 4: goto end;
            default : break;
        } // switch
    } // while
      
end:
    close(qnffd);
    
    return 0;
}

