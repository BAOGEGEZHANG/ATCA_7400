/*
 *                          Copyright QiCe Tech, 2010-2012
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:      
 *
 */

/*******************************************************************
 *
 * Description:     Includes
 *
 *******************************************************************/
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <ctype.h>
#include <errno.h>
#include <regex.h>

#include <fcntl.h>
#include <netinet/in.h>    /* for ntohs() */
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "drvops.h"
#include "parserops.h"
#include "cs_server/cs_server.h"
#include "include/qnfreg.h"
#include "include/qnfapi.h"
#include "include/tap_ioctl.h"


#define FIRMWARE_FILE_NAME		"/usr/local/bin/firmware.bin"
#define BYPASS_CONFIG_FILE              "/etc/sysconfig/bypass.conf"


#if defined(BUILD_MC20E) || defined(BUILD_MC20P) || defined(BUILD_MC20P_MC4P25) || defined(BUILD_MC20E_PPOE)
/*******************************************************************
 *
 * Description:     System Functions
 *
 *******************************************************************/
int
drv_update_firmware(int oc_no)
{
    int qnffd = -1;
    char qnfname[20];
    
    int binfd = -1;
    int i, j;
    struct stat statbuf;
    uint32_t file_size = 0, file_size_w = 0;
    uint8_t need_bank = 0, last_bank = 0;
    uint32_t last_words = 0;
    
    uint64_t sn_logic = 0;
    uint16_t result16;
    unsigned short pw[BANK_SIZE_W];
    
    FILE* fp;
    
    
    sprintf(qnfname, "/dev/qnf%d", oc_no);
	
    if ((qnffd = qnf_open(qnfname)) < 0)
    {
        //printf("Fail to open device %s.\n",qnfname);
        return -1;
    }
    
    if ((binfd = open(FIRMWARE_FILE_NAME, O_RDONLY)) < 0)
    {
        printf("Fail to open firmware %s.\n", FIRMWARE_FILE_NAME);
        return -1;
    }
    
    if(fstat(binfd, &statbuf) < 0)
    {
        perror("fstat source");
        return -1;
    }
    
    file_size = (uint32_t)statbuf.st_size;
    if( (file_size % 2) != 0 )
    {
        printf("firmware file size error!\n");
        return -1;
    }
    file_size_w = (file_size -8) / 2;
    
    if ( (file_size_w % BANK_SIZE_W) == 0 )
    {
        need_bank = file_size_w / BANK_SIZE_W;
        last_words = BANK_SIZE_W;
    }
    else
    {
        need_bank = file_size_w / BANK_SIZE_W + 1;
        last_words = file_size_w % BANK_SIZE_W;
    }
    last_bank = need_bank - 1;
    
    /* set configuration register command */
    set_config_register(qnffd, 0xBDDF);
    
    /* clear status register */
    clear_stats_regs(qnffd);
    
    /* unlock */
    //printf(PROMPT1("step 1: unlock banks....\n"));
    for(i = 0; i < need_bank; i++)
        bank_unlock(qnffd, i);
    
    /* erase */
    //printf(PROMPT1("step 2: erease banks....\n"));	
    for(i = 0; i < need_bank; i++)
        bank_erase(qnffd, i);
    
    /* program */
    //printf(PROMPT1("step 3: program banks....\n"));	
    for(i = 0; i < need_bank -1; i++)
    {
        read(binfd, pw, BANK_SIZE_W*2);
        bank_program(qnffd, i, BANK_SIZE_W, pw);
    }
    
    // program the last one
    read(binfd, pw, last_words*2);
    bank_program(qnffd, last_bank, last_words, pw);
        
    // program the sn
    read(binfd, pw, 8);
    sn_logic = *(uint64_t *)pw;
    set_logic_num(qnffd, sn_logic);
    
    // readback the sn
    sn_logic = read_logic_num(qnffd);
    close(binfd); // not use pw any more
    
    /* verify */
    fp = fopen("read_flash", "wb+");    
    
    /* read */
    //printf(PROMPT1("step 4: verify banks....\n"));	
    for(j = 0; j < need_bank - 1; j++) // read 3 bank
    {
        flash_write(qnffd, j*BANK_SIZE_W, 0xff);
        for (i = j*BANK_SIZE_W; i < j*BANK_SIZE_W + BANK_SIZE_W; i++)
        {
            result16 = flash_read(qnffd, i);
            fwrite(&result16, 1, 2, fp);
        }
        fflush(stdout);
    }
    
    flash_write(qnffd, last_bank*BANK_SIZE_W, 0xff);
    for(i = last_bank*BANK_SIZE_W; i < j*BANK_SIZE_W + last_words; i++)
    {
        result16 = flash_read(qnffd, i);
        fwrite(&result16, 1, 2, fp);
    }
    
    sn_logic = read_logic_num(qnffd);
    fwrite(&sn_logic, 1, 8, fp);    
    fclose(fp);
  	
    if (compare_file_order(FIRMWARE_FILE_NAME, "read_flash") == 0 )
    {    
        // should we remove read_flash ?
        system("rm read_flash");
    }   
    else
    {
        printf("Verify Error, please reconfig!\n");
        close(qnffd);
        return -1;
    }
    
    /* reconfigure */
    //printf(PROMPT1("step 5: reconfigure....\n"));	
    set_config_register(qnffd, 0x3DDF);
    
    /*reconfigure*/
    reconfig(qnffd);

    printf(PROMPT1("reconfigure success, please reboot the system!\n"));
    	
    close(qnffd);
    
    return 0;
}
#else
int
drv_update_firmware(int oc_no)
{
    printf(PROMPT1("Update firmware not supported in this platform!\n"));
    return 0;
} 
#endif

int
drv_reset_forward_default_config(int oc_no)
{
    int i, j;
#ifdef BUILD_MC10P_FLOW	
    unsigned int addr;
#endif	
    char qnfname[20];
    int qnffd = -1;
    
#if defined(BUILD_MC20E) || defined(BUILD_MC20E_PPOE)
    qnf_action_t action;
#endif    
    
   
    sprintf(qnfname, "/dev/qnf%d", oc_no);

    if ((qnffd = qnf_open(qnfname)) < 0)
    {
        //printf("Failed to open device.\n");
        return 0;
    }   

#ifdef BUILD_MC10P_FLOW    
    for ( i=0; i < MAX_RULE_NO; i++)
        for ( j=0; j < MAX_RULE_NO; j++)
        {
            addr = ((i << 5) | j) & 0x3ff;
            reg_write(qnffd, (unsigned int)TAG_MAP_REG, ((FORWARD_DEF << 16) | addr));
        }
#endif

#if defined(BUILD_MC20E) || defined(BUILD_MC20E_PPOE)
    action.cmd = 2;
    for ( i=0; i < MAX_RULE_NO; i++)
        for ( j=0; j < MAX_RULE_NO; j++)
        {
            qnf_cam_lut_set(qnffd, i, j, &action);
        }
#endif
    
    close(qnffd);
    
    
    return 0;
} 

int
drv_reset_global_default_config(int oc_no)
{
    char qnfname[20];
    int qnffd = -1;
    int i;
    unsigned int uData;	
    
    
    sprintf(qnfname, "/dev/qnf%d", oc_no);
    
    if ((qnffd = qnf_open(qnfname)) < 0)
    {
        //printf("Failed to open device.\n");
        return 0;
    }   

#ifdef BUILD_MC10P_FLOW 
    // flow function enable
    reg_write(qnffd, (unsigned int)FLOW_ENABLE_REG, ENABLED);

    // packet length range
    uData = ((PKTLEN_MAX << 16) & 0xffff0000) | (PKTLEN_MIN & 0xffff);
    for (i = 0; i < 8 ; i++)
    {
        reg_write(qnffd, (unsigned int)(LEN_THRES0 + i*4), uData);
    }
    
    // tcp mms
    reg_write(qnffd, (unsigned int)TCP_MMS_REG, TCP_MMS);
    
    // sample rate
    reg_write(qnffd, (unsigned int)SAMPLE_RATE_REG, SAMPLE_RATE);
    
    // capture start
    reg_write(qnffd, (unsigned int)CAPTURE_START_REG, ENABLED);
#endif

#if defined(BUILD_MC20E) || defined(BUILD_MC20E_PPOE)    
    capture_start(qnffd, 0x3);
#endif
    
#ifdef BUILD_MC20P_GTP
    capture_start(qnffd, 0x3);
#endif

#ifdef BUILD_MC20P
    capture_start(qnffd, 0x3);
#endif

#ifdef BUILD_MC20P_MC4P25
    if (oc_no)	
    	capture_start_mc4p25(qnffd, 0x3);
    else
	capture_start(qnffd, 0x3);
#endif

    close(qnffd);
    
    
    return 0;
}


int
drv_set_ruleset1(int oc_no, rule1options_t *opts)
{
    char qnfname[20];
    int qnffd = -1;

#ifdef BUILD_MC10P_FLOW    	
    unsigned long cam_0, cam_1, cam_2, cam_3;
    unsigned long mask_0, mask_1, mask_2, mask_3;
    unsigned long cam_instr;
    
    unsigned long iCnt;
#endif

#if defined(BUILD_MC20E) || defined(BUILD_MC20E_PPOE)
    qnf_rule_tuple_t rule;
#endif


    sprintf(qnfname, "/dev/qnf%d", oc_no);
    
    if ((qnffd = qnf_open(qnfname)) < 0)
    {
        //printf("Failed to open device.\n");
        return 0;
    }
    
#ifdef BUILD_MC10P_FLOW    
    // default value
    cam_0 = 0x0, mask_0 = 0xffffffff;
    cam_1 = 0x0, mask_1 = 0xffffffff;
    cam_2 = 0x0, mask_2 = 0xffffffff;
    cam_3 = 0x0, mask_3 = 0xffffffff;
    
    if (opts->flag & RULE_OPT_PACKETLEN)
    {
    	cam_0 |= ((0x1 << opts->pktlen_range) << 24) & 0xff000000;
    	mask_0 &= 0x00ffffff;
    }
  	
    if(opts->flag & RULE_OPT_PROTOCOL)
    {
        cam_0 |= (((opts->protocol) & 0xff) << 16) & 0x00ff0000;
        mask_0 &= 0xff00ffff;
    }
	
    if(opts->flag & RULE_OPT_SRCIP)
    {
       // first of all, reverse the mask
        opts->sourceip_mask = ~(opts->sourceip_mask);
	   
    	cam_0 |= (opts->sourceip >> 16) & 0x0000ffff;
    	mask_0 &= ((opts->sourceip_mask >> 16) | 0xffff0000);
    	
    	cam_1 |= (opts->sourceip << 16) & 0xffff0000;
    	mask_1 &=( (opts->sourceip_mask << 16) | 0x0000ffff);
    }	
    
    if (opts->flag & RULE_OPT_DESTIP)
    {
       // first of all, reverse the mask
       opts->destip_mask = ~(opts->destip_mask);
	   
    	cam_1 |= (opts->destip >> 16) & 0x0000ffff;
    	mask_1 &= ((opts->destip_mask >> 16) | 0xffff0000);
    	
    	cam_2 |= (opts->destip << 16) & 0xffff0000;
    	mask_2 &=( (opts->destip_mask << 16) | 0x0000ffff) ;
    }
    
    if (opts->flag & RULE_OPT_SRCL4PORT)
    {
    	cam_2 |= (opts->sourcel4port);
    	mask_2 &= 0xffff0000;
    }
    
    if (opts->flag & RULE_OPT_DESTL4PORT)
    {
    	cam_3 |= (opts->destl4port & 0xffff ) << 16;
    	mask_3 &= 0x0000ffff;
    }
    
    
    reg_write(qnffd, CAM0_DATA, cam_0);
    reg_write(qnffd, CAM0_MASK, mask_0);
    reg_write(qnffd, CAM1_DATA, cam_1);
    reg_write(qnffd, CAM1_MASK, mask_1);
    reg_write(qnffd, CAM2_DATA, cam_2);
    reg_write(qnffd, CAM2_MASK, mask_2);
    reg_write(qnffd, CAM3_DATA, cam_3);
    reg_write(qnffd, CAM3_MASK, mask_3);
    
    
    /* addr(5) = '0' */
    cam_instr = 0x00000000 | (opts->id & 0x3f);
    
    reg_write(qnffd, (unsigned int)CAM_ADDR, (unsigned int)cam_instr);
    
    
    iCnt = 1000;
    do
    {
        usleep(10);
        cam_instr = (unsigned long)reg_read(qnffd, (unsigned int)CAM_ADDR);
        iCnt--;
    } while(((cam_instr & 0x80000000) != 0)&&(iCnt));
	
    if (iCnt == 0)
    {
        errprint("Time out when writing cam entry\n");
        return -1;
    }
#endif

#if defined(BUILD_MC20E) || defined(BUILD_MC20E_PPOE)
    memset(&rule, 0xff, sizeof(qnf_rule_tuple_t));
    if (opts->flag & RULE_OPT_PACKETLEN)
    {
    	rule.data.pktlen = opts->pktlen_range;
	rule.mask.pktlen = ~(0x1 << opts->pktlen_range);
    }
    
    if(opts->flag & RULE_OPT_PROTOCOL)
    {
        rule.data.proto = opts->protocol;
	rule.mask.proto = 0x00;
    }
    
    if(opts->flag & RULE_OPT_SRCIP)
    {
        // first of all, reverse the mask
        opts->sourceip_mask = ~(opts->sourceip_mask);
	   
    	rule.data.sip = opts->sourceip;
	rule.mask.sip = opts->sourceip_mask;
    }
    
    if (opts->flag & RULE_OPT_DESTIP)
    {
        // first of all, reverse the mask
        opts->destip_mask = ~(opts->destip_mask);
	   
    	rule.data.dip = opts->destip;
	rule.mask.dip = opts->destip_mask;
    }
    
    if (opts->flag & RULE_OPT_SRCL4PORT)
    {
    	rule.data.sport = opts->sourcel4port;
	rule.mask.sport = 0x0000;
    }
    
    if (opts->flag & RULE_OPT_DESTL4PORT)
    {
    	rule.data.dport = opts->destl4port;
	rule.mask.dport = 0x0000;
    }
    
    qnf_cam_tuple_set(qnffd, opts->id, &rule);
#endif
    
    close(qnffd);
    
    
    return 0;                   
}

 
int
drv_set_ruleset2(int oc_no, rule2options_t *opts)
{
    unsigned char i, j, offset;
    unsigned char key_tmp[MAX_KEY_NO], mask_tmp[MAX_KEY_NO];
#ifdef BUILD_MC10P_FLOW	
    unsigned long cam_0, cam_1, cam_2, cam_3;
    unsigned long mask_0, mask_1, mask_2, mask_3;
    unsigned long cam_instr; 
    unsigned long iCnt;
#endif
    
#if defined(BUILD_MC20E) || defined(BUILD_MC20E_PPOE)    
    qnf_rule_payload_t payload;
#endif
    
    char qnfname[20];
    int qnffd = -1;
    
    
    sprintf(qnfname, "/dev/qnf%d", oc_no);	
    
    if ((qnffd = qnf_open(qnfname)) < 0)
    {
        //printf("Failed to open device.\n");
        return 0;
    }   
    
    // default value
    if(opts->flag & RULE_OPT_KEY)
    {
        /* deal with offset */
        if(opts->flag & RULE_OPT_OFFSET)
        {
            offset = opts->offset;
            if (offset != 0)
            {
                for (i=0, j=0; i < MAX_KEY_NO; i++)
                {
                    if (i < offset)
                    {
                        key_tmp[i] = 0x0;
                        mask_tmp[i] = 0xff;
                    }
                    else
                    {
                        key_tmp[i] = opts->key[j];
                        mask_tmp[i] = opts->mask[j];
                        j++;
                    }
                }

                for (i=0; i < MAX_KEY_NO; i++)
                {
                    opts->key[i] = key_tmp[i];
                    opts->mask[i] = mask_tmp[i];
                }
            }
        }

#ifdef BUILD_MC10P_FLOW		        
        cam_0 = ((opts->key[0] << 24) & 0xff000000) | ((opts->key[1] << 16) & 0x00ff0000) |
                	((opts->key[2] << 8) & 0x0000ff00) | ((opts->key[3] << 0) & 0x000000ff) ;
        mask_0 = ((opts->mask[0] << 24) & 0xff000000) | ((opts->mask[1] << 16) & 0x00ff0000) |
                	((opts->mask[2] << 8) & 0x0000ff00) | ((opts->mask[3] << 0) & 0x000000ff) ;
        cam_1 = ((opts->key[4] << 24) & 0xff000000) | ((opts->key[5] << 16) & 0x00ff0000) |
                	((opts->key[6] << 8) & 0x0000ff00) | ((opts->key[7] << 0) & 0x000000ff) ;
        mask_1 = ((opts->mask[4] << 24) & 0xff000000) | ((opts->mask[5] << 16) & 0x00ff0000) |
                	((opts->mask[6] << 8) & 0x0000ff00) | ((opts->mask[7] << 0) & 0x000000ff) ;
        cam_2 = ((opts->key[8] << 24) & 0xff000000) | ((opts->key[9] << 16) & 0x00ff0000) |
                	((opts->key[10] << 8) & 0x0000ff00) | ((opts->key[11] << 0) & 0x000000ff) ;
        mask_2 = ((opts->mask[8] << 24) & 0xff000000) | ((opts->mask[9] << 16) & 0x00ff0000) |
                	((opts->mask[10] << 8) & 0x0000ff00) | ((opts->mask[11] << 0) & 0x000000ff) ;
        cam_3 = ((opts->key[12] << 24) & 0xff000000) | ((opts->key[13] << 16) & 0x00ff0000) |
                	((opts->key[14] << 8) & 0x0000ff00) | ((opts->key[15] << 0) & 0x000000ff) ;
        mask_3 = ((opts->mask[12] << 24) & 0xff000000) | ((opts->mask[13] << 16) & 0x00ff0000) |
                	((opts->mask[14] << 8) & 0x0000ff00) | ((opts->mask[15] << 0) & 0x000000ff) ;

	reg_write(qnffd, CAM0_DATA, cam_0);
    	reg_write(qnffd, CAM0_MASK, mask_0);
    	reg_write(qnffd, CAM1_DATA, cam_1);
    	reg_write(qnffd, CAM1_MASK, mask_1);
    	reg_write(qnffd, CAM2_DATA, cam_2);
    	reg_write(qnffd, CAM2_MASK, mask_2);
    	reg_write(qnffd, CAM3_DATA, cam_3);
    	reg_write(qnffd, CAM3_MASK, mask_3);
    	
    	/* addr(5) = '1' */
    	cam_instr = 0x00000020 | (opts->id & 0x3f);    	
    	reg_write(qnffd, (unsigned int)CAM_ADDR, (unsigned int)cam_instr);
    	
    	iCnt = 1000;
    	do
    	{
            usleep(10);
            cam_instr = (unsigned long)reg_read(qnffd, (unsigned int)CAM_ADDR);
            iCnt--;
    	} while(((cam_instr & 0x80000000) != 0)&&(iCnt));
	
    	if (iCnt == 0)
    	{
       	    errprint("Time out when writing cam entry\n");
       	    return -1;
    	}
#endif
	
#if defined(BUILD_MC20E) || defined(BUILD_MC20E_PPOE)
        for (i = 0; i < MAX_KEY_NO; i++)
	{
	    payload.data[i] = opts->key[i];
	    payload.mask[i] = opts->mask[i];
	}
	qnf_cam_payload_set(qnffd, opts->id, &payload);
#endif
    }
    
    close(qnffd);
    
    
    return 0;   
}


int
drv_set_forward_mode(int oc_no, unsigned int data)
{
    char qnfname[20];
    int qnffd = -1;
    int rule1_index, rule2_index;
    int opcode;
#if defined(BUILD_MC20E) || defined(BUILD_MC20E_PPOE)
    qnf_action_t action;
#endif
    
    sprintf(qnfname, "/dev/qnf%d", oc_no);
    
    if ((qnffd = qnf_open(qnfname)) < 0)
    {
        //printf("Failed to open device.\n");
        return 0;
    }

#ifdef BUILD_MC10P_FLOW    
    // first get MAC_ADDR2 value
    reg_write(qnffd, (unsigned int)TAG_MAP_REG, data);
#endif

#if defined(BUILD_MC20E) || defined(BUILD_MC20E_PPOE)
    rule2_index = data & 0x3f;
    rule1_index = (data >> 6) & 0x3f;
    opcode = (data >> 16) & 0xffff;
    if (opcode == 0x0000)
        action.cmd = 2;
    else if (opcode == 0x4000)
        action.cmd = 3;

    qnf_cam_lut_set(qnffd, rule1_index, rule2_index,  &action);
#endif
    
    close(qnffd);
    
    
    return 0;
}


int 
drv_set_pktlen_range(int oc_no, int range_no, int pktlen_min, int pktlen_max)
{
    int qnffd = -1;
    char qnfname[20];
#ifdef BUILD_MC10P_FLOW	
    unsigned long uData;
#endif    
    
    sprintf(qnfname, "/dev/qnf%d", oc_no);
    
    if ((qnffd = qnf_open(qnfname)) < 0)
    {
        //printf("Failed to open device.\n");
        return 0;
    }
    
#ifdef BUILD_MC10P_FLOW    
    uData = ((pktlen_max << 16) & 0xffff0000) | (pktlen_min & 0x0000ffff);
    
    switch (range_no)
    {
    	case 0:
    		reg_write(qnffd, (unsigned int)LEN_THRES0, uData);
    		break;
    	case 1:
    		reg_write(qnffd, (unsigned int)LEN_THRES1, uData);
    		break;
    	case 2:
    		reg_write(qnffd, (unsigned int)LEN_THRES2, uData);
    		break;
    	case 3:
    		reg_write(qnffd, (unsigned int)LEN_THRES3, uData);
    		break;
    	case 4:
    		reg_write(qnffd, (unsigned int)LEN_THRES4, uData);
    		break;
    	case 5:
    		reg_write(qnffd, (unsigned int)LEN_THRES5, uData);
    		break;
    	case 6:
    		reg_write(qnffd, (unsigned int)LEN_THRES6, uData);
    		break;
    	case 7:
    		reg_write(qnffd, (unsigned int)LEN_THRES7, uData);
    		break;
    	default:
    		printf("Wrong packet length range.\n");
    }
#endif
    
#if defined(BUILD_MC20E) || defined(BUILD_MC20E_PPOE)
    qnf_set_pktlen_range(qnffd, range_no, pktlen_min, pktlen_max);
#endif
    
    close(qnffd);
    
    
    return 0;
}

int 
drv_set_sample_rate(int oc_no, int sample_rate)
{
#ifdef BUILD_MC10P_FLOW
    char qnfname[20];
    int qnffd = -1;
    
    sprintf(qnfname, "/dev/qnf%d", oc_no);
    
    if ((qnffd = qnf_open(qnfname)) < 0)
    {
        //printf("Failed to open device.\n");
        return 0;
    }
    
    reg_write(qnffd, (unsigned int)SAMPLE_RATE_REG, sample_rate);
    
    close(qnffd);
#endif    
    
    return 0;
}

int
drv_set_eth_mac(int oc_no, int src_dst, unsigned char mac[6])
{
#ifdef BUILD_MC10P_FLOW
    char qnfname[20];
    int qnffd = -1;
    unsigned int uData0, uData1, uData2;
    
    sprintf(qnfname, "/dev/qnf%d", oc_no);
    
    if ((qnffd = qnf_open(qnfname)) < 0)
    {
        //printf("Failed to open device.\n");
        return 0;
    }   
    
    // first get MAC_ADDR2 value
    uData2 = reg_read(qnffd, (unsigned int)MAC_ADDR2);
    
    // update source mac address
    if (src_dst == 0)
    {
        uData0 = (mac[3] << 24) + (mac[2] << 16) + (mac[1] << 8) + mac[0];
        uData2 = (uData2 & 0xffff0000) | (mac[5] << 8) | mac[4];
        reg_write(qnffd, MAC_ADDR0, uData0);
        reg_write(qnffd, MAC_ADDR2, uData2);
    }
    else
    {
        uData1 = (mac[3] << 24) + (mac[2] << 16) + (mac[1] << 8) + mac[0];
        uData2 = (mac[5] << 24) | (mac[4] << 16) | (uData2 & 0xffff);
        reg_write(qnffd, MAC_ADDR1, uData1);
        reg_write(qnffd, MAC_ADDR2, uData2);
    }
        
    // write the update register
    reg_write(qnffd, MAC_ADDR_UPDATE, 0x1);    
    
    close(qnffd);
#endif    
    
    return 0;   
}


int 
drv_set_gtp_dest_ports(int oc_no, int gtpdestports)
{
#ifdef BUILD_MC20P_GTP
    char qnfname[20];
    int qnffd = -1;
    
    sprintf(qnfname, "/dev/qnf%d", oc_no);
    
    if ((qnffd = qnf_open(qnfname)) < 0)
    {
        //printf("Failed to open device.\n");
        return 0;
    }   
    
    reg_write(qnffd, (unsigned int)GTP_DEST_PORTS, gtpdestports);
    
    close(qnffd);
#endif
    
    return 0;
}


int 
drv_set_gtp_hash_mode(int oc_no, int out_both)
{
#ifdef BUILD_MC20P_GTP
    char qnfname[20];
    int qnffd = -1;
    
    sprintf(qnfname, "/dev/qnf%d", oc_no);
    
    if ((qnffd = qnf_open(qnfname)) < 0)
    {
        //printf("Failed to open device.\n");
        return 0;
    }
    
    reg_write(qnffd, (unsigned int)GTP_HASH_MODE, out_both&0x3);
    
    close(qnffd);
#endif

    return 0;
}


int
drv_fpga_reg_read(int oc_no, unsigned int reg_no, unsigned int *reg_data)
{
    char qnfname[20];
    int qnffd = -1;
    
    
    sprintf(qnfname, "/dev/qnf%d", oc_no);
    
    if ((qnffd = qnf_open(qnfname)) < 0)
    {
        //printf("Failed to open device.\n");
        return 0;
    }
    
    *reg_data = reg_read(qnffd, (unsigned int)(REG_BASE + reg_no));
    
    
    close(qnffd);
    
    return 0;
}

int
drv_fpga_reg_write(int oc_no, unsigned int reg_no, unsigned int reg_data)
{
    char qnfname[20];
    int qnffd = -1;
    
    
    sprintf(qnfname, "/dev/qnf%d", oc_no);
    
    if ((qnffd = qnf_open(qnfname)) < 0)
    {
        //printf("Failed to open device.\n");
        return 0;
    }
    
    reg_write(qnffd, (unsigned int)(REG_BASE + reg_no), reg_data);
    
    close(qnffd);
    
    return 0;
}

int
drv_get_fpga_stats(int oc_no)
{
#ifdef BUILD_MC10P_FLOW

    char qnfname[20];
    int qnffd = -1;
    
    unsigned int value0_hi, value0_lo,value1_hi, value1_lo,value2_hi, value2_lo;
    unsigned int value3_hi, value3_lo,value4_hi, value4_lo,value5_lo;
    unsigned long long value0_last, value0, value1_last, value1,value2_last, value2;
    unsigned long long value3_last, value3,value4_last, value4, value5_last, value5;
    unsigned long long fp_0_pps, fp_1_pps, fp_2_pps, fp_3_pps, fp_4_pps, fp_5_pps;
	
    struct timeval start, stop;
    unsigned int     timeuse;
    
    sprintf(qnfname, "/dev/qnf%d", oc_no);
    
    if ((qnffd = qnf_open(qnfname)) < 0)
    {
        //printf("Failed to open device.\n");
        return 0;
    }
    
    gettimeofday(&start, NULL);
    
    value0_lo = reg_read(qnffd, FPGA_STATS0_IN_LO);
    value0_hi = reg_read(qnffd, FPGA_STATS0_IN_HI);
    value0_last = ((unsigned long long) value0_hi << 32) | value0_lo;
    value1_lo = reg_read(qnffd, FPGA_STATS1_CLS_LO);
    value1_hi = reg_read(qnffd, FPGA_STATS1_CLS_HI);
    value1_last = ((unsigned long long) value1_hi << 32) | value1_lo;
    value2_lo = reg_read(qnffd, FPGA_STATS2_SCH_LO);
    value2_hi = reg_read(qnffd, FPGA_STATS2_SCH_HI);
    value2_last = ((unsigned long long) value2_hi << 32) | value2_lo;
    value3_lo = reg_read(qnffd, FPGA_STATS3_FT_LO);
    value3_hi = reg_read(qnffd, FPGA_STATS3_FT_HI);
    value3_last = ((unsigned long long) value3_hi << 32) | value3_lo;
    value4_lo = reg_read(qnffd, FPGA_STATS4_FC_LO);
    value4_hi = reg_read(qnffd, FPGA_STATS4_FC_HI);
    value4_last = ((unsigned long long) value4_hi << 32) | value4_lo;
    value5_lo = reg_read(qnffd, FPGA_STATS5_DROP);
	
	value5_last = ((unsigned long long) 0x0 << 32) | value5_lo;
         
	sleep(1);
	gettimeofday(&stop, NULL);
	timeuse = (stop.tv_sec - start.tv_sec)*1000000 + (stop.tv_usec - start.tv_usec);
	  
	value0_lo = reg_read(qnffd, FPGA_STATS0_IN_LO);
	value0_hi = reg_read(qnffd, FPGA_STATS0_IN_HI);
	value0 = ((unsigned long long) value0_hi << 32) | value0_lo;
	value1_lo = reg_read(qnffd, FPGA_STATS1_CLS_LO);
	value1_hi = reg_read(qnffd, FPGA_STATS1_CLS_HI);
	value1 = ((unsigned long long) value1_hi << 32) | value1_lo;
	value2_lo = reg_read(qnffd, FPGA_STATS2_SCH_LO);
	value2_hi = reg_read(qnffd, FPGA_STATS2_SCH_HI);
	value2 = ((unsigned long long) value2_hi << 32) | value2_lo;
	value3_lo = reg_read(qnffd, FPGA_STATS3_FT_LO);
	value3_hi = reg_read(qnffd, FPGA_STATS3_FT_HI);
	value3 = ((unsigned long long) value3_hi << 32) | value3_lo;
	value4_lo = reg_read(qnffd, FPGA_STATS4_FC_LO);
	value4_hi = reg_read(qnffd, FPGA_STATS4_FC_HI);
	value4 = ((unsigned long long) value4_hi << 32) | value4_lo;
	value5_lo = reg_read(qnffd, FPGA_STATS5_DROP);	
	value5 = ((unsigned long long) 0x0 << 32) | value5_lo;
	  
	fp_0_pps = (float)((value0 - value0_last)*1000000)/(float)timeuse;
	fp_1_pps = (float)((value1 - value1_last)*1000000)/(float)timeuse;
	fp_2_pps = (float)((value2 - value2_last)*1000000)/(float)timeuse;
	fp_3_pps = (float)((value3 - value3_last)*1000000)/(float)timeuse;
	fp_4_pps = (float)((value4 - value4_last)*1000000)/(float)timeuse;
	fp_5_pps = (float)((value5 - value5_last)*1000000)/(float)timeuse;
		    
	//print out result
	printf("\tFPGA packet rate 0: %llu\n", fp_0_pps);
	printf("\tFPGA packet rate 1: %llu\n", fp_1_pps);
	printf("\tFPGA packet rate 2: %llu\n", fp_2_pps);
	printf("\tFPGA packet rate 3: %llu\n", fp_3_pps);
	printf("\tFPGA packet rate 4: %llu\n", fp_4_pps);
	printf("\tFPGA packet rate 5: %llu\n", fp_5_pps);
    
    
    close(qnffd);
#endif    
    
    return 0;
}

int
drv_get_packet_stats()
{
#if defined(BUILD_MC20E) || defined(BUILD_MC20E_PPOE)

    char qnfname[20] ;
    char ocname[20];
    int qnffd = -1;
    int oc_no = -1;
    
    unsigned int value0_hi, value0_lo,value1_hi, value1_lo,value2_hi, value2_lo, value3_hi, value3_lo;
    unsigned long long value0_last, value0, value1_last, value1,value2_last, value2, value3_last, value3;
    unsigned long long fp_0_pps, fp_1_pps;
    unsigned long long fp_0_bps, fp_1_bps;
    
    struct timeval start, stop;
    unsigned int     timeuse;
    
    
    //print header here
    printf("%10s%14s%17s%16s%17s\n", "port","rx pps", "rx bps", "tx pps", "tx bps");
    printf("%74s\n", "====================================================================");
    
    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no++)
    {
    	sprintf(qnfname, "/dev/qnf%d", oc_no);
    	if ((qnffd = qnf_open(qnfname)) < 0)
    	{
           //doesn't exits, don't print
    	}
    	else
    	{    
            gettimeofday(&start, NULL);
            value0_lo = reg_read(qnffd, FPGA_STATS0_LO);
	    value0_hi = reg_read(qnffd, FPGA_STATS0_HI);
    	    value0_last = ((unsigned long long) value0_hi << 32) | value0_lo;
	    value1_lo = reg_read(qnffd, FPGA_STATS1_LO);
	    value1_hi = reg_read(qnffd, FPGA_STATS1_HI);
    	    value1_last = ((unsigned long long) value1_hi << 32) | value1_lo;
	    value2_lo = reg_read(qnffd, FPGA_STATS2_LO);
	    value2_hi = reg_read(qnffd, FPGA_STATS2_HI);
    	    value2_last = ((unsigned long long) value2_hi << 32) | value2_lo;
	    value3_lo = reg_read(qnffd, FPGA_STATS3_LO);
	    value3_hi = reg_read(qnffd, FPGA_STATS3_HI);
    	    value3_last = ((unsigned long long) value3_hi << 32) | value3_lo;
           
	    sleep(1);
	    gettimeofday(&stop, NULL);
	    timeuse = (stop.tv_sec - start.tv_sec)*1000000 + (stop.tv_usec - start.tv_usec);
           
            value0_lo = reg_read(qnffd, FPGA_STATS0_LO);
	    value0_hi = reg_read(qnffd, FPGA_STATS0_HI);
	    value0 = ((unsigned long long) value0_hi << 32) | value0_lo;
	    value1_lo = reg_read(qnffd, FPGA_STATS1_LO);
	    value1_hi = reg_read(qnffd, FPGA_STATS1_HI);
	    value1 = ((unsigned long long) value1_hi << 32) | value1_lo;
	    value2_lo = reg_read(qnffd, FPGA_STATS2_LO);
	    value2_hi = reg_read(qnffd, FPGA_STATS2_HI);
	    value2 = ((unsigned long long) value2_hi << 32) | value2_lo;
	    value3_lo = reg_read(qnffd, FPGA_STATS3_LO);
	    value3_hi = reg_read(qnffd, FPGA_STATS3_HI);
	    value3 = ((unsigned long long) value3_hi << 32) | value3_lo;
	    
	    fp_0_pps = (float)((value0 - value0_last)*1000000)/(float)timeuse;
	    fp_0_bps = (float)((value1 - value1_last)*8000000)/(float)timeuse;
	    fp_1_pps = (float)((value2 - value2_last)*1000000)/(float)timeuse;
	    fp_1_bps = (float)((value3 - value3_last)*8000000)/(float)timeuse;
           
	    close(qnffd);

	    sprintf(ocname, "xe%d", 0+oc_no*2);
	    printf("%10s%14llu%17llu%16d%17d\n", ocname, fp_0_pps, fp_0_bps, 0, 0);
	    sprintf(ocname, "xe%d", 1+oc_no*2);
	    printf("%10s%14llu%17llu%16d%17d\n", ocname, fp_1_pps, fp_1_bps, 0, 0);
        }  
    }
    
    // print end
    printf("%74s\n\n", "====================================================================");
    
#endif    
    
    return 0;
}

int
drv_get_framer_stats(int oc_no)
{
#ifdef BUILD_MC10P_FLOW
    // not supported right now
    int qnffd = -1;
    char qnfname[20];
    
    unsigned short value_fr_hi, value_fr_lo;
    unsigned long long value_fr, fr_pps;
    
    struct timeval start, stop;
    unsigned int     timeuse;
    
    
    sprintf(qnfname, "/dev/qnf%d", oc_no);
    
    if ((qnffd = qnf_open(qnfname)) < 0)
    {
        //printf("Failed to open device.\n");
        return 0;
    }
    
    gettimeofday(&start, NULL);
    framer_read(qnffd, 0x301a0, &value_fr_hi);
    framer_read(qnffd, 0x301a2, &value_fr_lo);
    
    sleep(1);
    gettimeofday(&stop, NULL);
    timeuse = (stop.tv_sec - start.tv_sec)*1000000 + (stop.tv_usec - start.tv_usec);
    
    framer_read(qnffd, 0x301a0, &value_fr_hi);
    framer_read(qnffd, 0x301a2, &value_fr_lo);
    value_fr = (unsigned long long)((((unsigned int)value_fr_hi << 16)&0xffff0000)|((value_fr_lo)&0xffff));
    
    fr_pps = (float)(value_fr*1000000)/(float)timeuse;
    printf("\tFramer packet rate: %llu\n", fr_pps);
    
    close(qnffd);
#endif    
    
    return 0;
}

int
drv_get_palermo_stats(int oc_no)
{
#ifdef BUILD_MC10P_FLOW
    qnf_stats(oc_no);
#endif
   
    return 0;
}

int
drv_get_framer_running_cfg(int oc_no)
{
#ifdef BUILD_MC10P_FLOW
    qnf_running_config(oc_no);
#endif

    return 0;	
}

	
int 
drv_set_flow_enable(int oc_no, int enable_or_disable)
{
#ifdef BUILD_MC10P_FLOW
    char qnfname[20];
    int qnffd = -1;
    
    sprintf(qnfname, "/dev/qnf%d", oc_no);
    
    if ((qnffd = qnf_open(qnfname)) < 0)
    {
        //printf("Failed to open device.\n");
        return 0;
    }
    
    reg_write(qnffd, (unsigned int)FLOW_ENABLE_REG, enable_or_disable);
    
    close(qnffd);
#endif    
    
    return 0;
}

int 
drv_set_flow_mms(int oc_no, int mms_size)
{
#ifdef BUILD_MC10P_FLOW
    char qnfname[20];
    int qnffd = -1;
    
    sprintf(qnfname, "/dev/qnf%d", oc_no);
    
    if ((qnffd = qnf_open(qnfname)) < 0)
    {
        //printf("Failed to open device.\n");
        return 0;
    }
    
    reg_write(qnffd, (unsigned int)TCP_MMS_REG, mms_size);
    
    close(qnffd);
#endif
    
    return 0;
}

int 
drv_set_traffic_enable(int oc_no, int enable_or_disable)
{
    char qnfname[20];
    int qnffd = -1;
    
    sprintf(qnfname, "/dev/qnf%d", oc_no);
    
    if ((qnffd = qnf_open(qnfname)) < 0)
    {
        //printf("Failed to open device.\n");
        return 0;
    }
    
#ifdef BUILD_MC10P_FLOW
    reg_write(qnffd, (unsigned int)CAPTURE_START_REG, enable_or_disable);
#endif
    
#if defined(BUILD_MC20E) || defined(BUILD_MC20E_PPOE)
    if (enable_or_disable)
        capture_start(qnffd, 0x3);
    else
        capture_stop(qnffd, 0x3);
#endif

#ifdef BUILD_MC20P
    if (enable_or_disable)
        capture_start(qnffd, 0x3);
    else
        capture_stop(qnffd, 0x3);
#endif
    
    close(qnffd);
    
    return 0;
}

int 
drv_set_bcm_cmds(CONSRV *pcs, unsigned char *cmds)
{
    res_result_t result;
    char cmd_str[CS_SRV_BUF_SIZE];
    
    memset(&result, 0, sizeof(res_result_t));
    strcpy(cmd_str, (char *)cmds);
    strcat(cmd_str, "\n");
    
    cs_cl_insert_str(pcs, cmd_str, &result);
    printf("%s", result.data);
    
    return 0;
}


int 
drv_bypass_get()
{
#if defined(BUILD_QT10) || defined(BUILD_QT20)  
    int dev_file;
    int i;
    unsigned long ctl_data;
    
    FILE *fp;
    unsigned int card0, card1, card2;
    unsigned int pbmp_byp_capble = 0x0000;	
    
    dev_file = open(DEVICE_NAME, 0);	
    if (dev_file < 0) 	
    {		
        printf("Can't open device file: %s (%d)\n", DEVICE_NAME, dev_file);		
	exit(-1);	
    }
    
    ioctl(dev_file, IOCTL_GET_REGS, &ctl_data);
    
    /* done, clean up */
    close(dev_file);

#if defined(BUILD_QT10)    
    // print fixed header
    printf(PROMPT1("Line      0           1           2           3\n"));
    printf(PROMPT1("----------------------------------------------------------------\n"));
    printf(PROMPT1("Port  ge0  ge1    ge2  ge3    ge4  ge5    ge6  ge7\n"));
    printf(PROMPT1("----------------------------------------------------------------\n"));
    printf(PROMPT1("Stat"));

    if ((fp = fopen(BYPASS_CONFIG_FILE, "r")) == NULL)
    {
        printf("Fail to open bypass config file!\n");
	 printf("Use default settings: all cards in monitor group.\n");
    }
    else
    {
       fscanf(fp, "%d,%d,%d", &card0, &card1, &card2);
	fclose(fp);
	if (card0) // network, bypass capable
	     pbmp_byp_capble = 0x0001;
	if (card1)
	     pbmp_byp_capble |= 0x0002;
	if (card2)
	     pbmp_byp_capble |= 0x0004;
    }
    
    for (i = 0 ; i < MAX_BYPASS_PORTS ; i++)
    {
        if (pbmp_byp_capble & (1 << i))
        {
            if (ctl_data & (0x1 << i))  // '1' is normal, '0' is bypass
	        printf("%7s     ", "NRM");
	    else
	        printf("%7s     ", "BYP");
        }
	else
	    printf("%7s     ", "-");
    }
    printf("\n\n");
#endif

#if defined(BUILD_QT20)    
    // print fixed header
    printf(PROMPT1("Line    0    1    2    3    4    5    6    7    8    9   10   11\n"));
    printf(PROMPT1("----------------------------------------------------------------\n"));
    printf(PROMPT1("Port  ge0  ge2  ge4  ge6  ge8 ge10 ge12 ge14 ge16 ge18 ge20 ge22\n"));
    printf(PROMPT1("      ge1  ge3  ge5  ge7  ge9 ge11 ge13 ge15 ge17 ge19 ge21 ge23\n"));
    printf(PROMPT1("----------------------------------------------------------------\n"));
    printf(PROMPT1("Stat"));
    
    if ((fp = fopen(BYPASS_CONFIG_FILE, "r")) == NULL)
    {
         printf("Fail to open bypass config file!\n");
	 printf("Use default settings: all cards in monitor group.\n");
    }
    else
    {
        fscanf(fp, "%d,%d,%d", &card0, &card1, &card2);
	fclose(fp);
	if (card0) // network, bypass capable
	     pbmp_byp_capble = 0x000f;
	if (card1)
	     pbmp_byp_capble |= 0x00f0;
	if (card2)
	     pbmp_byp_capble |= 0x0f00;
    }
    
    for (i = 0 ; i < MAX_BYPASS_PORTS ; i++)
    {
        if (pbmp_byp_capble & (1 << i))
        {
            if (ctl_data & (0x1 << i))  // '1' is normal, '0' is bypass
	        printf("%5s", "NRM");
	    else
	        printf("%5s", "BYP");
        }
	else
	    printf("%5s", "-");
    }
    printf("\n\n");
#endif	
#endif
    
    return 0;
}

int 
drv_bypass_set(CONSRV *pcs, int pbmp, int enable_or_disable)
{
#if defined(BUILD_QT10) || defined(BUILD_QT20) 
    int dev_file;
    int i,j, reg;
    int need_restart;
    unsigned long ctl_data;
    
    res_result_t result;
    char cmd_str[CS_SRV_BUF_SIZE];
    unsigned char tmpbuf[3] = { 0, 0, 0, 0};

    memset(&result, 0, sizeof(res_result_t));
    
    dev_file = open(DEVICE_NAME, 0);
    if (dev_file < 0) 	
    {		
        printf("Can't open device file: %s (%d)\n", DEVICE_NAME, dev_file);		
	exit(-1);	
    }

#if defined(BUILD_QT10)    
    /* now update led */
    for (i = 0; i < MAX_BYPASS_PORTS; i++)
    {
        // do update
	if (pbmp & (0x1 << i))
	{
	    if (enable_or_disable == 0) // put into bypass status, led off
	    {
		sprintf(cmd_str, "getreg 0x0 0x18\n");
	        cs_cl_insert_str(pcs, cmd_str, &result);
                tmpbuf[0] = result.data[result.data_size-5]; 
                tmpbuf[1] = result.data[result.data_size-4]; 
                tmpbuf[2] = result.data[result.data_size-3];
		reg = strtol(tmpbuf, NULL, 16);
                //printf("reg 0x18 is: %x\n", reg);
                reg &= (~(0x1 << i*2)) & (~(0x1 << i*2+1));
                sprintf(cmd_str, "setreg 0x0 0x18 0x%x\n", reg);
	        cs_cl_insert_str(pcs, cmd_str, &result);

		sprintf(cmd_str, "getreg 0x0 0x1a\n");
	        cs_cl_insert_str(pcs, cmd_str, &result);
                tmpbuf[0] = result.data[result.data_size-5]; 
                tmpbuf[1] = result.data[result.data_size-4]; 
                tmpbuf[2] = result.data[result.data_size-3];
		reg = strtol(tmpbuf, NULL, 16);
                //printf("reg 0x1a is: %x\n", reg);
                reg &= (~(0x1 << i*2)) & (~(0x1 << i*2+1));
                sprintf(cmd_str, "setreg 0x0 0x1a 0x%x\n", reg);
	        cs_cl_insert_str(pcs, cmd_str, &result);
	    }
	    else  // put into normal status, led normal
	    {
                do {
	      	    need_restart = 0;
		    // before going to normal state, check the link status
		    sprintf(cmd_str, "phy ge%d 0x19\n", i*2);
		    cs_cl_insert_str(pcs, cmd_str, &result);

	            if ((result.data[result.data_size-5] != 'f') && (result.data[result.data_size-5] != '7'))
                    {
                        need_restart = 1;
                        printf("data is: %d, %d, %d, %d\n", result.data[result.data_size-6], 
                                                  result.data[result.data_size-5],
                                                  result.data[result.data_size-4],
                                                  result.data[result.data_size-3]);
                    }
                    
                    memset(&result, 0, sizeof(res_result_t));  
                    sprintf(cmd_str, "phy ge%d 0x19\n", i*2+1);
		    cs_cl_insert_str(pcs, cmd_str, &result);
	            if ((result.data[result.data_size-5] != 'f') && (result.data[result.data_size-5] != '7'))
                    { 
                        need_restart = 1;
                        printf("data is: %d, %d, %d, %d\n", result.data[result.data_size-6], 
                                                  result.data[result.data_size-5],
                                                  result.data[result.data_size-4],
                                                  result.data[result.data_size-3]);
            	    }
		
                    if (need_restart)
                    {	
                         // not ready now, restart
                         sprintf(cmd_str, "phy ge%d 0x0 0x1340\n", i*2);
		         cs_cl_insert_str(pcs, cmd_str, &result);
		         memset(&result, 0, sizeof(res_result_t));
		         sprintf(cmd_str, "phy ge%d 0x0 0x1340\n", i*2+1);
		         cs_cl_insert_str(pcs, cmd_str, &result);
		         sleep(10);
		    }
		} while (need_restart);

                // led turn to normal
		sprintf(cmd_str, "getreg 0x0 0x18\n");
	        cs_cl_insert_str(pcs, cmd_str, &result);
                tmpbuf[0] = result.data[result.data_size-5]; 
                tmpbuf[1] = result.data[result.data_size-4]; 
                tmpbuf[2] = result.data[result.data_size-3];
		reg = strtol(tmpbuf, NULL, 16);
                //printf("reg 0x18 is: %x\n", reg);
                reg |= ((0x1 << i*2) | (0x1 << i*2+1));
                sprintf(cmd_str, "setreg 0x0 0x18 0x%x\n", reg);
	        cs_cl_insert_str(pcs, cmd_str, &result);

		sprintf(cmd_str, "getreg 0x0 0x1a\n");
	        cs_cl_insert_str(pcs, cmd_str, &result);
                tmpbuf[0] = result.data[result.data_size-5]; 
                tmpbuf[1] = result.data[result.data_size-4]; 
                tmpbuf[2] = result.data[result.data_size-3];
		reg = strtol(tmpbuf, NULL, 16);
                //printf("reg 0x1a is: %x\n", reg);
                reg |= ((0x1 << i*2) | (0x1 << i*2+1));
                sprintf(cmd_str, "setreg 0x0 0x1a 0x%x\n", reg);
	        cs_cl_insert_str(pcs, cmd_str, &result);
	    }
	}
    }
#endif

#if defined(BUILD_QT20)    
    /* now update led */
    for (i = 0; i < MAX_BYPASS_PORTS; i++)
    {
        // do update
	if (pbmp & (0x1 << i))
	{
	     if (enable_or_disable == 0) // put into bypass status, led off
	     {
                 sprintf(cmd_str, "phy ge%d 0x1c 0xb4ee\n", i*2);
		 cs_cl_insert_str(pcs, cmd_str, &result);
		 memset(&result, 0, sizeof(res_result_t));
		 sprintf(cmd_str, "phy ge%d 0x1c 0xb4ee\n", i*2+1);
		 cs_cl_insert_str(pcs, cmd_str, &result);
	     }
	     else  // put into normal status, led normal
	     {
		 sprintf(cmd_str, "phy ge%d 0x1c 0xb4aa\n", i*2);
		 cs_cl_insert_str(pcs, cmd_str, &result);
		 memset(&result, 0, sizeof(res_result_t));
		 sprintf(cmd_str, "phy ge%d 0x1c 0xb4aa\n", i*2+1);
		 cs_cl_insert_str(pcs, cmd_str, &result);
	     }
	}
    }
#endif
    
    // first get current settings
    ioctl(dev_file, IOCTL_GET_REGS, &ctl_data);
    // then update the bits
    for (i = 0; i < MAX_BYPASS_PORTS; i++)
    {
	// do update
	if (pbmp & (0x1 << i))
	{
	     if (enable_or_disable == 0)	// '1' is normal, '0' is bypass
		      ctl_data &= ~(0x1 << i);
	     else
		      ctl_data |= (0x1 << i);
	}
    }
    ioctl(dev_file, IOCTL_SET_REGS, &ctl_data);
    /* done, clean up */
    close(dev_file);
#endif

    return 0;
}

