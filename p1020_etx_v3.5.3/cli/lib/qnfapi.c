/*
 * Copyright (c) 2010-2015 Qice Technology Ltd, Beijing, China.
 * All rights reserved.
 *
 * This source code is proprietary to Qice Technology Limited and no part
 * of it may be redistributed, published or disclosed except as outlined in
 * the written contract supplied with this product.
 *
 *******************************************************************************
 *  qnfapi.c: this file defines the api functions
 *******************************************************************************/

#include "qnfapi.h"

#ifdef BUILD_MC10P_FLOW
// for cs1777
#include "m2r_gen.h"
#include "m2r_public.h"
#include "m2r_stats.h"
#include "m2r_mpif.h"
#include "m2r_spi42.h"
#include "m2r_lif.h"
#include "m2r_pprbs.h"
#include "m2r_framer.h"
#endif

#ifdef BUILD_MC20P_MC4P25
// for cs1331
#include "arn5_gen_api.h"
#include "arn5_gen_data.h"
#include "arn5_mpif_api.h"
#include "arn5_print.h"
#include "arn5_stats.h"
#include "arn5_spi_api.h"
#include "arn5_debug.h"
#include "arn5_framer_pkt.h"
#endif


// definitions for MDIO access
/************************************************************/
#define PRER_LO             	0x00 // Low byte clock prescaler register
#define PRER_HI             	0x01 // High byte clock prescaler register   
#define CTR                 	0x02 // Control register 
#define TXR                 	0x03 // Transmit byte register   
#define CR                  		0x04 // Command register   
#define RXR                 	0x03 // Receive byte register
#define SR                  		0x04 // Status register
#define RD                  		0x1
#define WR                  	0x0
#define NOERROR             	0x0
#define OFFLINE             	0x1
#define LINK_FAILURE      	0x2
#define AUTO_NEG_ERR     0x3

#define DRV_CONF_1777_FILE  "/etc/sysconfig/qnf_mc10p.conf"
#define DRV_CONF_1331_FILE  "/etc/sysconfig/qnf_mc4p25.conf"


// gloabl var
/*************************************************************/
static  sheep_t  *herd;


// debug information
/*************************************************************/
//#define DEBUG
#ifdef DEBUG
# define API_DEBUG(MSG)    printf MSG
#else
# define API_DEBUG(MSG)
#endif


__inline__ unsigned int
reg_read(int qnffd, unsigned int offset )
{
    reg32_t reg32;
    unsigned int byte0, byte1, byte2, byte3, value;
        
    reg32.offset = offset;
    
    if(ioctl(qnffd, QNFIOCR32, &reg32) < 0)
    {
        printf("QNFIOCR32 error!\n");
        return -1;
    }
    
    // do the swap      
    byte0 = reg32.value & 0xff;
    byte1 = (reg32.value >> 8) & 0xff;
    byte2 = (reg32.value >> 16) & 0xff;
    byte3 = (reg32.value >> 24) & 0xff;
    value = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
    
    return value;
}

__inline__ unsigned int
reg_write(int qnffd, unsigned int offset, unsigned int value )
{
    reg32_t reg32;
    unsigned int byte0, byte1, byte2, byte3;
    
    // do the swap
    byte0 = value & 0xff;
    byte1 = (value >> 8) & 0xff;
    byte2 = (value >> 16) & 0xff;
    byte3 = (value >> 24) & 0xff;
    
    reg32.offset = offset;
    reg32.value = (byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3;
    
    if(ioctl(qnffd, QNFIOCW32, &reg32) < 0)
    {
        printf("QNFIOCW32 error!\n");
        return -1;  
    } 
    
    return 0;
}

// open qnf card
int qnf_open(char *qnfname)
{
    int qnffd, herdsize;
    
    if((qnffd = open(qnfname, O_RDWR)) < 0)
    {
        //fprintf(stderr, "[qnf_open] open device failed!\n");
        goto fail;
    }
    
    if(herd == NULL) 
    {
        herdsize = sysconf(_SC_OPEN_MAX) * sizeof(sheep_t);
        herd = malloc(herdsize);
        if(herd == NULL)
        {
            fprintf(stderr, "[qnf_open] malloc(herdsize) error!\n");
            goto fail;
        }
        
        memset(herd, 0, herdsize);
    }
    
    if(qnffd >= sysconf(_SC_OPEN_MAX)) 
    {
        fprintf(stderr, "[qnf_open] sysconf(_SC_OPEN_MAX) error!\n");
        goto fail;
    }
    
    if(ioctl(qnffd, QNFIOCGINFO, &herd[qnffd].qnfinf) < 0)
    {
        printf("QNFIOCW32 error!\n");
        return -1;  
    }
    
    strcpy(herd[qnffd].qnfname, qnfname);
    herd[qnffd].opened = 1;
    
    return qnffd;

fail:
    if (qnffd > 0)
    {
        // Zero the entry for the failed qnf file descriptor.
        memset(&herd[qnffd], 0, sizeof(sheep_t));
    }
    
    return -1;
}

int qnf_close(int qnffd)
{    
    if(!herd[qnffd].opened) 
    {
        fprintf(stderr, "[qnf_close] card not opened!\n");
        return -1;
    }
    
    memset(&herd[qnffd], 0, sizeof(herd[qnffd]));
    
    return close(qnffd);
}


#ifdef BUILD_MC10P_FLOW
/*******************************************************************************/
__inline__ unsigned int
framer_read(int qnffd, unsigned int offset, unsigned short *value )
{
    reg16_t reg16;
    unsigned short lo, hi;
    
    if (qnffd != -1)
    {
        reg16.offset = offset + FRAMER_BASE;
    
        if (ioctl(qnffd, QNFIOCR16, &reg16) < 0)
        {    
            printf("QNFIOCR16 error!\n");
            return -1;
        }
            
        lo = reg16.value & 0xff;
        hi = (reg16.value >> 8) & 0xff; 
            
        // do the swap
        *value = (lo << 8) | hi ;
    }
    else
    {
        printf("Wrong qnffd!\n");
        return -1;
    }
    
    return 0;
}

__inline__ unsigned int
framer_write(int qnffd, unsigned int offset, unsigned short value )
{
    reg16_t reg16;
    unsigned short lo, hi;  
    
    if (qnffd != -1)
    {
        reg16.offset = offset + FRAMER_BASE;
            
        // do the swap
        lo = value & 0xff;
        hi = (value >> 8) & 0xff;
        reg16.value = (lo << 8) | hi ;
            
        if (ioctl(qnffd, QNFIOCW16, &reg16) < 0)
        {
            printf("QNFIOCW16 error!\n");
            return -1;
        }   
    }
    else
    {
        printf("Wrong qnffd!\n");
        return -1;
    }
    
    return 0;
}

int qnf_framer_mc10p_init_1(confs_t *cfg)
{
    char config_file[32]= DRV_CONF_1777_FILE;
    char tmps[256];
    char *options, *keys;
    unsigned int value0, value1;
    FILE *fp;
        
    if(m2r_load_driver() != CS_OK)
    {
        printf("Loading framer driver failed.\n");
        return -1;
    }
        
    if ((fp=fopen(config_file,"r"))==NULL)      
        printf("Can not open configuration file %s for read, using default value.\n",config_file);
    
    while(fgets(tmps, 256, fp))
    {
        options = strtok(tmps, " ");
        keys = strtok(NULL, " ");
        if ((!options) || (!keys))
        {
            break;
        }
        sscanf(keys, "%u,%u", &value0, &value1);
                
        if (strncmp("LINE_RATE",options,9)==0)
        {
            cfg->configs[0].line_rate = value0;
            cfg->configs[1].line_rate = value1;
        }
        else if (strncmp("SONET_MODE",options,10)==0)
        {
            cfg->configs[0].frame = value0;
            cfg->configs[1].frame = value1;
        }
        else if (strncmp("PPP_MODE",options,8)==0)
        {
            cfg->configs[0].encap = value0;
            cfg->configs[1].encap = value1;
        }
        else if (strncmp("SONET_SCRAMBLE",options,14)==0)
        {
            cfg->configs[0].framer_scramble = value0;
            cfg->configs[1].framer_scramble = value1;
        }       
        else if (strncmp("PAYLOAD_SCRAMBLE",options,16)==0)
        {
            cfg->configs[0].mapper_scramble = value0;
            cfg->configs[1].mapper_scramble = value1;
        }       
        else if (strncmp("CRC32_MODE",options,10)==0)
        {
            cfg->configs[0].fcs = value0;
            cfg->configs[1].fcs = value1;
        }       
        else if (strncmp("PASS_CRC",options,8)==0)
        {
            cfg->configs[0].pass_crc = value0;
            cfg->configs[1].pass_crc = value1;
        }
        else if (strncmp("MAX_PKT_SIZE",options,12)==0)
        {
            cfg->configs[0].max_pkt_size = value0;
            cfg->configs[1].max_pkt_size = value1;
        }
        else if (strncmp("MIN_PKT_SIZE",options,12)==0)
        {
            cfg->configs[0].min_pkt_size = value0;
            cfg->configs[1].min_pkt_size = value1;
        }
        else
        {
            printf("unsupported configs.\n");
        }
    } // while
    fclose(fp); 
    
    return 0;
}

int qnf_framer_mc10p_init_2(int qnffd, int device, confs_t *cfg)
{
    m2r_port_cfg_t *pCfg=NULL;
    
    //for (device = 0; device < MAX_OC_NUM ; device++)
    {    
        // we encode qnffd into base address
        //if (m2r_dev_register(0, (unsigned long)(qnffd*0x100000)) != CS_OK)
        if (m2r_dev_register(device, qnffd*0x100000) != CS_OK)
        {
            printf("Registering framer device address failed.\n" );
            return -1;
        }
        
        if(m2r_dev_init(device) != CS_OK)
        {
            printf("Initializing framer device failed.\n" );
            return -1;
        }
            
            /* begin config */
            pCfg = (m2r_port_cfg_t *) CS_MALLOC( sizeof(m2r_port_cfg_t));
            CS_MEMSET( (void *)pCfg, 0, sizeof(m2r_port_cfg_t) ) ;
                
            /* pCfg->summ must be filled before calling the m2r_port_get_default_cfg function */
            pCfg->summ.mac_mode = M2R_PORT_IN_BYPASS_MODE;
                {
                    pCfg->summ.line_rate =  (cfg->configs[device].line_rate == 0) ? M2R_PORT_RATE_OC192C : M2R_PORT_RATE_OC48C;
                    pCfg->summ.l2_prot = (cfg->configs[device].encap == 1) ? M2R_PORT_PROT_PPP : M2R_PORT_PROT_POS;
            }
                pCfg->summ.lif_mode = M2R_PORT_IF_XFP;
            pCfg->summ.rmac_ms_mode = M2R_MS_UNKNOWN;
        
            if(m2r_port_get_default_cfg(device, pCfg) != CS_OK)
            {
                printf("Can't get framer default port configuration.\n");
                return -1;
            }
            
            /*Modify the default configuration when needed */
            pCfg->framer.sonet_mode = TRUE;
            pCfg->framer.sonet_scrambling = TRUE;
            pCfg->framer.payload_scrambling = TRUE;
            pCfg->framer.mode.pos_cfg.crc32 = TRUE;/*default FALSE*/
            pCfg->framer.mode.pos_cfg.passFCS = FALSE;
            pCfg->framer.mode.pos_cfg.max_pktSize = 1600;/*default 0xffff*/
            pCfg->framer.mode.pos_cfg.min_pktSize = 40;  /*default 8*/
            pCfg->framer.mode.pos_cfg.drop_long = TRUE;
            pCfg->framer.mode.pos_cfg.drop_short= TRUE;
            pCfg->lif.tx_ckref = M2R_LIF_REF_CLK_155;/*default M2R_LIF_REF_CLK_622*/
            pCfg->spi42.rdclk_mult = 4;/*default 1, RDCLK = RDREFCLK*/
            {
                pCfg->framer.sonet_mode = (cfg->configs[device].frame==1)? TRUE:FALSE;
                pCfg->framer.sonet_scrambling = (cfg->configs[device].framer_scramble==1)? TRUE:FALSE;
                pCfg->framer.payload_scrambling = (cfg->configs[device].mapper_scramble==1)? TRUE:FALSE;
                pCfg->framer.mode.pos_cfg.crc32 = (cfg->configs[device].fcs==1)? TRUE:FALSE;
                pCfg->framer.mode.pos_cfg.passFCS = (cfg->configs[device].pass_crc==1)? TRUE:FALSE;
                pCfg->framer.mode.pos_cfg.max_pktSize = cfg->configs[device].max_pkt_size;
                pCfg->framer.mode.pos_cfg.min_pktSize = cfg->configs[device].min_pkt_size;          
        } // while
        
        if(m2r_port_init(device, pCfg) != CS_OK)
        {
            printf("Can't init framer port configuration.\n");
            errno = EIO;
            return -1;
        }
    }
    
    return 0;
}

int qnf_running_config(int device)
{  
    return m2r_port_dump_running_cfg(device);
}

int qnf_stats(int device)
{
    return m2r_print_stats(device);
}

unsigned long long qnf_framer_stats(int device)
{
    m2r_framer_stats_t fStats;
    unsigned long long pps;
    
    // specify the current f
    m2r_get_blk_stats(device, M2R_ID_FRAMER, &fStats, sizeof (m2r_framer_stats_t));
    
    pps = fStats.strm0.pkt.good_pkts.rx;
    
    return pps;
}
#endif


#ifdef BUILD_MC20P
/*******************************************************************************/
// user can use the following two functions to read/write registers inside QNF cards
__inline__ unsigned int
framer_reg_read(int framerfd, unsigned int offset )
{
    reg32_t reg32;
    unsigned int byte0, byte1, byte2, byte3, value;
      
    reg32.offset = offset;
    
    if(ioctl(framerfd, FRAMERIOCR32, &reg32) < 0)
    {
        printf("FRAMERIOCR32 error!\n");
        return -1;
    }
    
    value = reg32.value;
               
    return value;
}

__inline__ unsigned int
framer_reg_write(int framerfd, unsigned int offset, unsigned int value )
{
    reg32_t reg32;
    unsigned int byte0, byte1, byte2, byte3;
    
    reg32.offset = offset;
    reg32.value = value;
    
    if(ioctl(framerfd, FRAMERIOCW32, &reg32) < 0)
    {
        printf("FRAMERIOCW32 error!\n");
        return -1;  
    } 
    
    return 0;
}

int
framer_reg_dump(int framerfd)
{
    reg32_t reg32;
    
    reg32.offset = 0;
    reg32.value = 0;
    
    if(ioctl(framerfd, FRAMERIOCDUMP, &reg32) < 0)
    {
       printf("FRAMERIOCW32 error!\n");
       return -1;  
    } 
    
    return 0;
}

int capture_start(int qnffd, int mode)
{
        uint32_t tmp = 0x0;
        
        if (mode == 0x01)
            tmp = 0x00000001;
        else if (mode == 0x2)
            tmp = 0x00010000;
        else if (mode == 0x3)
            tmp = 0x00010001;
    
    API_DEBUG(("capture start write in is 0x%x\n",tmp));
        reg_write(qnffd, CAPTURE_CTL_REG, tmp);
    
        return 0;
}

int capture_stop(int qnffd, int mode)
{
    uint32_t tmp1, tmp = 0x0;
        
    if (mode == 0x01)
        tmp = 0xfffffffe;
        else if (mode == 0x2)
        tmp = 0xfffeffff;
        else if (mode == 0x3)
              tmp = 0xfffefffe;
        
        tmp1 = reg_read(qnffd, CAPTURE_CTL_REG);
    API_DEBUG(("capture stop tem is 0x%x\n",tmp));
        tmp &= tmp1;
    
    API_DEBUG(("capture stop write in is 0x%x\n",tmp));
        reg_write(qnffd, CAPTURE_CTL_REG, tmp);
        
        return 0;
}
#endif


#ifdef BUILD_MC20P_GTP
/*******************************************************************************/
// user can use the following two functions to read/write registers inside QNF cards
__inline__ unsigned int
framer_reg_read(int framerfd, unsigned int offset )
{
    reg32_t reg32;
    unsigned int byte0, byte1, byte2, byte3, value;
      
    reg32.offset = offset;
    
    if(ioctl(framerfd, FRAMERIOCR32, &reg32) < 0)
    {
        printf("FRAMERIOCR32 error!\n");
        return -1;
    }
    
    value = reg32.value;
                  
    return value;
}

__inline__ unsigned int
framer_reg_write(int framerfd, unsigned int offset, unsigned int value )
{
    reg32_t reg32;
    unsigned int byte0, byte1, byte2, byte3;
    
    reg32.offset = offset;
    reg32.value = value;
    
    if(ioctl(framerfd, FRAMERIOCW32, &reg32) < 0)
    {
       printf("FRAMERIOCW32 error!\n");
       return -1;  
    } 
    
    return 0;
}

// open framer
int framer_open(char *framername)
{
    int framerfd, herdsize;
    
    
    if((framerfd = open(framername, O_RDWR)) < 0)
    {
        //fprintf(stderr, "[framer_open] open device failed!\n");
        goto fail;
    }
    
    if(herd == NULL) 
    {
        herdsize = sysconf(_SC_OPEN_MAX) * sizeof(sheep_t);
        herd = malloc(herdsize);
        if(herd == NULL)
        {
            fprintf(stderr, "[framer_open] malloc(herdsize) error!\n");
            goto fail;
        }
        
        memset(herd, 0, herdsize);
    }
    
    if(framerfd >= sysconf(_SC_OPEN_MAX)) 
    {
        fprintf(stderr, "[framer_open] sysconf(_SC_OPEN_MAX) error!\n");
        goto fail;
    }
    
    strcpy(herd[framerfd].qnfname, framername);
    herd[framerfd].opened = 1;
    
    return framerfd;

fail:
    if (framerfd > 0)
    {
        // Zero the entry for the failed qnf file descriptor.
        memset(&herd[framerfd], 0, sizeof(sheep_t));
    }
    
    return -1;
}

int framer_close(int framerfd)
{    
    if(!herd[framerfd].opened) 
    {
        fprintf(stderr, "[framer_close] card not opened!\n");
        return -1;
    }
    
    memset(&herd[framerfd], 0, sizeof(herd[framerfd]));
    
    return close(framerfd);
}

int
framer_reg_dump(int framerfd)
{
        reg32_t reg32;
        
        reg32.offset = 0;
        reg32.value = 0;
    
        if(ioctl(framerfd, FRAMERIOCDUMP, &reg32) < 0)
        {
        printf("FRAMERIOCW32 error!\n");
        return -1;  
        } 
        
        return 0;
}

int capture_start(int qnffd, int mode)
{
    uint32_t tmp;

    // enable vlan
    reg_write(qnffd, MAC_RX_CFG, 0x18000000);
    reg_write(qnffd, MAC_TX_CFG, 0x18000000);
    
    if (mode == 0x01)
        tmp = 0x00000001;
    else if (mode == 0x2)
        tmp = 0x00010000;
    else if (mode == 0x3)
        tmp = 0x00010001;
            
        API_DEBUG(("capture start write in is 0x%x\n",tmp));
    reg_write(qnffd, CAPTURE_CTL_REG, tmp);
            
    return 0;
}

int capture_stop(int qnffd, int mode)
{
    uint32_t tmp1, tmp;
        
    if (mode == 0x01)
        tmp = 0xfffffffe;
    else if (mode == 0x2)
        tmp = 0xfffeffff;
    else if (mode == 0x3)
        tmp = 0xfffefffe;
        
    tmp1 = reg_read(qnffd, CAPTURE_CTL_REG);
        API_DEBUG(("capture stop tem is 0x%x\n",tmp));
    tmp &= tmp1;
            
        API_DEBUG(("capture stop write in is 0x%x\n",tmp));
    reg_write(qnffd, CAPTURE_CTL_REG, tmp);
        
    return 0;
}
#endif


#ifdef BUILD_MC20P_MC4P25
/*******************************************************************************/
__inline__ unsigned int
framer_reg_read(int framerfd, unsigned int offset )
{
    reg32_t reg32;
    unsigned int byte0, byte1, byte2, byte3, value;
    
    reg32.offset = offset;
    
    if(ioctl(framerfd, FRAMERIOCR32, &reg32) < 0)
    {
        printf("FRAMERIOCR32 error!\n");
        return -1;
    }
    
    value = reg32.value;
    
    return value;
}

__inline__ unsigned int
framer_reg_write(int framerfd, unsigned int offset, unsigned int value )
{
    reg32_t reg32;
    unsigned int byte0, byte1, byte2, byte3;
    
    reg32.offset = offset;
    reg32.value = value;
    
    if(ioctl(framerfd, FRAMERIOCW32, &reg32) < 0)
    {
        printf("FRAMERIOCW32 error!\n");
        return -1;  
    } 
    
    return 0;
}

int
framer_reg_dump(int framerfd)
{
    reg32_t reg32;
    
    reg32.offset = 0;
    reg32.value = 0;
    
    if(ioctl(framerfd, FRAMERIOCDUMP, &reg32) < 0)
    {
       printf("FRAMERIOCW32 error!\n");
       return -1;  
    } 
    
    return 0;
}

// open framer
int framer_open(char *framername)
{
    int framerfd, herdsize;
    
    
    if((framerfd = open(framername, O_RDWR)) < 0)
    {
        //fprintf(stderr, "[framer_open] open device failed!\n");
        goto fail;
    }
    
    if(herd == NULL) 
    {
        herdsize = sysconf(_SC_OPEN_MAX) * sizeof(sheep_t);
        herd = malloc(herdsize);
        if(herd == NULL)
        {
            fprintf(stderr, "[framer_open] malloc(herdsize) error!\n");
            goto fail;
        }
        
        memset(herd, 0, herdsize);
    }
    
    if(framerfd >= sysconf(_SC_OPEN_MAX)) 
    {
        fprintf(stderr, "[framer_open] sysconf(_SC_OPEN_MAX) error!\n");
        goto fail;
    }
    
    strcpy(herd[framerfd].qnfname, framername);
    herd[framerfd].opened = 1;
    
    return framerfd;

fail:
    if (framerfd > 0)
    {
        // Zero the entry for the failed qnf file descriptor.
        memset(&herd[framerfd], 0, sizeof(sheep_t));
    }
    
    return -1;
}

int framer_close(int framerfd)
{    
    if(!herd[framerfd].opened) 
    {
        fprintf(stderr, "[framer_close] card not opened!\n");
        return -1;
    }
    
    memset(&herd[framerfd], 0, sizeof(herd[framerfd]));
    
    return close(framerfd);
}

__inline__ unsigned int
framer_read(int qnffd, unsigned int offset, unsigned short *value )
{
    reg16_t reg16;
    unsigned short lo, hi;
    
    // because of the different logic design, need divid 2 here
    reg16.offset = offset + FRAMER_BASE_MC4P25;
    
    if (ioctl(qnffd, QNFIOCR16, &reg16) < 0)
    {    
        printf("QNFIOCR16 error!\n");
        return -1;
    }
    
    lo = reg16.value & 0xff;
    hi = (reg16.value >> 8) & 0xff; 
    
    // do the swap
    *value = (lo << 8) | hi ;
    
    //printf("read [0x%05x]:[0x%04x]\n", offset_real, *value); 
    
    return 0;
}

__inline__ unsigned int
framer_write(int qnffd, unsigned int offset, unsigned short value )
{
    reg16_t reg16;
    unsigned short lo, hi;  
    
    reg16.offset = offset + FRAMER_BASE_MC4P25;
    
    // do the swap
    lo = value & 0xff;
    hi = (value >> 8) & 0xff;
    reg16.value = (lo << 8) | hi ;
    
    if (ioctl(qnffd, QNFIOCW16, &reg16) < 0)
    {
        printf("QNFIOCW16 error!\n");
        return -1;
    }
    
    //printf("write [0x%05x]:[0x%04x]\n", offset_real, reg16.value);
    
    return 0;
}

int capture_start(int qnffd, int mode)
{
    uint32_t tmp = 0x0;
        
    if (mode == 0x01)
        tmp = 0x00000001;
    else if (mode == 0x2)
        tmp = 0x00010000;
    else if (mode == 0x3)
        tmp = 0x00010001;
    
    API_DEBUG(("capture start write in is 0x%x\n",tmp));
    reg_write(qnffd, CAPTURE_CTL_REG, tmp);
    
    return 0;
}

int capture_stop(int qnffd, int mode)
{
    uint32_t tmp1, tmp = 0x0;
        
    if (mode == 0x01)
        tmp = 0xfffffffe;
    else if (mode == 0x2)
        tmp = 0xfffeffff;
    else if (mode == 0x3)
        tmp = 0xfffefffe;
        
    tmp1 = reg_read(qnffd, CAPTURE_CTL_REG);
    API_DEBUG(("capture stop tem is 0x%x\n",tmp));
    tmp &= tmp1;
    
    API_DEBUG(("capture stop write in is 0x%x\n",tmp));
    reg_write(qnffd, CAPTURE_CTL_REG, tmp);
        
    return 0;
}

int capture_start_mc4p25(int qnffd, int mode)
{
    API_DEBUG(("capture start write in is 0x%x\n",tmp));
    
    reg_write(qnffd, MAC_ADDR0_MC4P25, 0x2fff1250);
    reg_write(qnffd, MAC_ADDR1_MC4P25, 0x2fff1351);
    reg_write(qnffd, MAC_ADDR2_MC4P25, 0x00010001);
    reg_write(qnffd, MAC_ADDR_UPDATE_MC4P25, 0x1);
    
    reg_write(qnffd, CAPTURE_CTL_REG_MC4P25, 0xffffffff);
    usleep(100);
    reg_write(qnffd, CAPTURE_CTL_REG_MC4P25, 0xffffffff);
    usleep(100);
    reg_write(qnffd, CAPTURE_CTL_REG_MC4P25, 0xffffffff);
    
    return 0;
}

int capture_stop_mc4p25(int qnffd, int mode)
{
    reg_write(qnffd, CAPTURE_CTL_REG_MC4P25, 0x0);
    usleep(100);
    reg_write(qnffd, CAPTURE_CTL_REG_MC4P25, 0x0);
    usleep(100);
    reg_write(qnffd, CAPTURE_CTL_REG_MC4P25, 0x0);
        
    return 0;
}

static int arsenal_dev_init(unsigned char dev_id)
{
    arn5_dev_cfg_t * pDevCfg = NULL ;
    int status = CS_OK ;
    
    pDevCfg = (arn5_dev_cfg_t *) CS_MALLOC( sizeof(arn5_dev_cfg_t) ) ;
    if (pDevCfg == NULL) 
    {
        return (CS_ERROR) ;
    }
    pDevCfg->summ.host_if = ARN5_HOST_IF_SPI42 ;
    
    //printf("calling arn5_dev_get_default_cfg...\n");
    
    /* Get driver's default recommended configuration parameters */
    if ( arn5_dev_get_default_cfg((cs_uint8)dev_id, pDevCfg) == CS_ERROR ) 
    {
        status = CS_ERROR ;
        goto RTN_EXIT ;
    }
    
    /* Customize the configuration parameters as needed */
    
    //printf("calling arn5_dev_init...\n");
    
    /* Initialize the device with the user specified configuration */
    if ( arn5_dev_init((cs_uint8)dev_id, pDevCfg) == CS_ERROR ) 
    {
        status = CS_ERROR ;
        goto RTN_EXIT ;
    }
  
RTN_EXIT :
        if (pDevCfg) CS_FREE(pDevCfg) ;
        
        return (status) ;
}

static void readConfig(configs_t *cfgs)
{
    char config_file[32]=DRV_CONF_1331_FILE;
    char tmps[256];
    char *options, *keys;
    unsigned int value[4];
    FILE *fp;
    int i;
    
    for (i = 0; i < 4; i++)
    {
        cfgs->cfg[i].line_rate = 0;  // OC48
        cfgs->cfg[i].frame = 1; // SONET
        cfgs->cfg[i].encap = 1; //PPP
        cfgs->cfg[i].framer_scramble = 0;
        cfgs->cfg[i].mapper_scramble = 0;
        cfgs->cfg[i].fcs = 1;  // CRC32
        cfgs->cfg[i].pass_crc = 0;
        cfgs->cfg[i].max_pkt_size = 1500;
        cfgs->cfg[i].min_pkt_size = 40;
    }
    
    if ((fp=fopen(config_file, "r")) == NULL)
    {
        CS_PRINT("Can't open configuration file %s for read, using default value.\n", config_file);
    }
    else
    {
        while(fgets(tmps, 256, fp))
        {
            options = strtok(tmps, " ");
            keys = strtok(NULL, " ");
            if ((!options) || (!keys))
            {
                CS_PRINT("Empty line is not supported!\n");
                break;
            }
            sscanf(keys, "%d,%d,%d,%d", &value[0], &value[1], &value[2], &value[3]);
            
            if (!strncmp("LINE_RATE", options, 9))
            {
                for (i = 0 ; i < 4; i++)
                    cfgs->cfg[i].line_rate = value[i];
            }
            else if (!strncmp("SONET_MODE", options, 10))
            {
                for (i = 0; i < 4; i++)
                    cfgs->cfg[i].frame = value[i];
            }
            else if (!strncmp("PPP_MODE", options, 8))
            {
                for (i = 0; i < 4; i++)
                    cfgs->cfg[i].encap= value[i];
            }
            else if (!strncmp("SONET_SCRAMBLE", options, 14))
            {
                for (i = 0; i < 4; i++)
                    cfgs->cfg[i].framer_scramble = value[i];
            }
            else if (!strncmp("PAYLOAD_SCRAMBLE", options, 16))
            {
                for (i = 0; i < 4; i++)
                    cfgs->cfg[i].mapper_scramble = value[i];
            }
            else if (!strncmp("CRC32_MODE", options, 10))
            {
                for (i = 0; i < 4; i++)
                    cfgs->cfg[i].fcs = value[i];
            }
            else if (!strncmp("PASS_CRC", options, 8))
            {
                for (i = 0; i < 4; i++)
                    cfgs->cfg[i].pass_crc = value[i];
            }
            else if (!strncmp("MAX_PKT_SIZE", options, 12))
            {
                for (i = 0; i < 4; i++)
                    cfgs->cfg[i].max_pkt_size= value[i];
            }
            else if (!strncmp("MIN_PKT_SIZE", options, 12))
            {
                for (i = 0; i < 4; i++)
                    cfgs->cfg[i].min_pkt_size= value[i];
            }
            else 
            {
                CS_PRINT("Unkown options!\n");
                break;
            }
        }// while     
    }//else
}

static short arsenal_port_init(unsigned char dev_id,unsigned char slice_num)
{
    unsigned short port_id = ARN5_INVALID_PORT_ID ;
    arn5_port_cfg_t * pCfg = NULL ;
    configs_t cfgs;
    int status = CS_OK ;
    
    pCfg = (arn5_port_cfg_t *) CS_MALLOC( sizeof(arn5_port_cfg_t) ) ;
    if (pCfg == NULL) {
        return (CS_ERROR) ; /* malloc() failed */
    }
    
    CS_MEMSET( (void *)pCfg, 0, sizeof(arn5_port_cfg_t) ) ; 
    
    /* Get config parameters from config file */
    CS_MEMSET((void *)&cfgs, 0, sizeof(configs_t));
    readConfig(&cfgs);
    
    if(cfgs.cfg[slice_num].line_rate==0)    
        pCfg->summ.line_rate = ARN5_PORT_RATE_OC48C ;
    else if(cfgs.cfg[slice_num].line_rate==1)
        pCfg->summ.line_rate = ARN5_PORT_RATE_OC12C ;
    else if(cfgs.cfg[slice_num].line_rate==2)
        pCfg->summ.line_rate = ARN5_PORT_RATE_OC3C ;
    else
    {
        CS_PRINT("wrong line rate, using default oc48!\n");
        pCfg->summ.line_rate = ARN5_PORT_RATE_OC48C;
    }
    
    if(cfgs.cfg[slice_num].encap==1)
        pCfg->summ.l2_prot = ARN5_PORT_PROT_PPP ;
    else
        pCfg->summ.l2_prot = ARN5_PORT_PROT_POS ;
    
    port_id = arn5_port_open((cs_uint8)dev_id, pCfg, (cs_uint8)slice_num) ;
    if (port_id == ARN5_INVALID_PORT_ID) 
    {
        status = CS_ERROR ;
        goto RTN_EXIT;
    }
    
    /* Customize configuration parameters as needed */
    /* pCfg->lif.tx_clk_mode = ARN5_LIF_TX_CLK_LOCAL; */
    pCfg->lif.tx_clk_mode = ARN5_LIF_TX_CLK_LOOP_INTERNAL;  
    
    /*Config parameters from drvconf.ini */
    if(cfgs.cfg[slice_num].frame==1)
        pCfg->framer.sonet_mode = TRUE; /* Sonet mode */
    else
        pCfg->framer.sonet_mode = FALSE;
       
    if(cfgs.cfg[slice_num].framer_scramble==1)
        pCfg->framer.sonet_scrambling = TRUE;
    else
        pCfg->framer.sonet_scrambling = FALSE;
       
    if(cfgs.cfg[slice_num].mapper_scramble==1)
        pCfg->framer.payload_scrambling = TRUE;
    else
        pCfg->framer.payload_scrambling = FALSE;
       
    if(cfgs.cfg[slice_num].fcs==1)
    {
        pCfg->framer.mode.pos_cfg.crc32 =TRUE;
        pCfg->framer.mode.pos_cfg.pktFCS = TRUE;
    }
    else if(cfgs.cfg[slice_num].fcs==0)
    {
        pCfg->framer.mode.pos_cfg.crc32 =FALSE;
        pCfg->framer.mode.pos_cfg.pktFCS = TRUE;
    }
    else
        pCfg->framer.mode.pos_cfg.pktFCS = FALSE;
    
    if(cfgs.cfg[slice_num].pass_crc == 1)
        pCfg->framer.mode.pos_cfg.passFCS = TRUE;
    else
        pCfg->framer.mode.pos_cfg.passFCS = FALSE;
    
    pCfg->framer.mode.pos_cfg.max_pktSize = cfgs.cfg[slice_num].max_pkt_size;
    pCfg->framer.mode.pos_cfg.min_pktSize = cfgs.cfg[slice_num].min_pkt_size;
    
    
    /* Initialize port with the user specified configuration */
    if ( arn5_port_init((cs_uint16)port_id, pCfg) == CS_ERROR ) 
    {
        CS_PRINT("ERROR during port initialization\n") ;
        arn5_port_close((cs_uint16)port_id) ;
        status = CS_ERROR ;
        goto RTN_EXIT ;
    }
    
RTN_EXIT :
    if (pCfg) CS_FREE(pCfg) ;
    return (port_id) ;
}   

int qnf_framer_init(int qnffd)
{
    unsigned char   slice_id;
    unsigned short   result;
    
    if (arn5_drvr_load() == CS_ERROR)
    {      
        CS_PRINT("ERROR loading arsenal driver.\n") ;       
        return CS_ERROR;        
    }
    
    // max offset is 0xf8000
    //printf("qnffd is: %d\n", qnffd);  
    if (arn5_dev_register(0, (cs_uint32)(qnffd*0x100000)) == CS_ERROR)
    {     
        CS_PRINT("ERROR registering arsenal device.\n") ;       
        return CS_ERROR;        
    }   
    
    //printf("arn5_dev_register finished, calling dev_init...\n"); 
    if (arsenal_dev_init(0)==CS_ERROR)
    {     
        CS_PRINT("ERROR initializing arsenal device.\n") ;      
        return CS_ERROR;        
    }   
    
    for(slice_id=0; slice_id<4; slice_id++)
    {
        result = arsenal_port_init(0, slice_id);
        if(result == ARN5_INVALID_PORT_ID)
        {            
            CS_PRINT("ERROR initializing slice %d.\n",slice_id) ;           
            return CS_ERROR;            
        }
    }
    
    /* GPIO 4 output=1 */  
    if(arn5_mpif_write_gpio_output(0, 0x0010, 0x00F0)==CS_ERROR)
    {           
        CS_PRINT("ERROR setting GPIO4=1.\n");           
        return CS_ERROR;            
    }       
   
    CS_PRINT("Framer init Success!\n");
    
 
    return CS_OK ;
}
#endif


#if defined(BUILD_MC20E) || defined(BUILD_MC20E_PPOE)
/*******************************************************************************/
#ifdef BUILD_MC20E
#define	MAX_RULE_NO		64
#else
#define   MAX_RULE_NO            32
#endif

int capture_start(int qnffd, int mode)
{
    uint32_t tmp = 0x0;

    if (mode == 0x01)
        tmp = 0x00000001;
    else if (mode == 0x2)
        tmp = 0x00010000;
    else if (mode == 0x3)
        tmp = 0x00010001;
    
    API_DEBUG(("capture start write in is 0x%x\n",tmp));
    reg_write(qnffd, CAPTURE_CTL_REG, tmp);
    
    return 0;
}

int capture_stop(int qnffd, int mode)
{
    uint32_t tmp1, tmp = 0x0;
        
    if (mode == 0x01)
        tmp = 0xfffffffe;
    else if (mode == 0x2)
        tmp = 0xfffeffff;
    else if (mode == 0x3)
        tmp = 0xfffefffe;
    
    tmp1 = reg_read(qnffd, CAPTURE_CTL_REG);
    API_DEBUG(("capture stop tem is 0x%x\n",tmp));
    tmp &= tmp1;
    
    API_DEBUG(("capture stop write in is 0x%x\n",tmp));
    reg_write(qnffd, CAPTURE_CTL_REG, tmp);
        
    return 0;
}

int qnf_cam_tuple_init(int qnffd)
{   
    //first we set one "match all" item at the last for xe1
    reg_write(qnffd, CAM4_MASK, 0xFFFFFFFF);
    reg_write(qnffd, CAM3_MASK, 0xFFFFFFFF);
    reg_write(qnffd, CAM2_MASK, 0xFFFFFFFF);
    reg_write(qnffd, CAM1_MASK, 0xFFFFFFFF);
    reg_write(qnffd, CAM0_MASK, 0xFFFFFFFF);
    reg_write(qnffd, CAM4_DATA, 0x00000000);
    reg_write(qnffd, CAM3_DATA, 0x00000000);
    reg_write(qnffd, CAM2_DATA, 0x00000000);
    reg_write(qnffd, CAM1_DATA, 0x00000000);
    reg_write(qnffd, CAM0_DATA, 0x00000000);
    reg_write(qnffd, CAM_ADDR , 0x8000001F);//the last rule
    usleep(1);
    
    return 0;
}

int qnf_cam_tuple_set(int qnffd, unsigned int entry, qnf_rule_tuple_t *rule)
{
    uint32_t data;
    int  i;
    
    if (entry >= MAX_RULE_NO){
        printf("Wrong rule entry number %u, must be in 0-%d.\n", entry, MAX_RULE_NO);
        return -1;
    }
    
    // CAM_MASK first, 0 means care, 1 means don't care
    data = (((uint32_t)rule->mask.proto << 24) & 0xff000000) | (((uint32_t)rule->mask.sip >> 8) & 0x00ffffff);
    reg_write(qnffd, CAM4_MASK, data);
    data = (((uint32_t)rule->mask.sip << 24) & 0xff000000) | (((uint32_t)rule->mask.dip >> 8) & 0x00ffffff) ;
    reg_write(qnffd, CAM3_MASK, data);
    data = (((uint32_t)rule->mask.dip << 24) & 0xff000000) | (((uint32_t)rule->mask.sport << 8) & 0x00ffff00) | (((uint32_t)rule->mask.dport >> 8) & 0x000000ff);
    reg_write(qnffd, CAM2_MASK, data);
    data = (((uint32_t)rule->mask.dport<<24) & 0xff000000) | (((uint32_t)rule->mask.tcpflag << 18) & 0x00fc0000) | (((uint32_t)rule->mask.pktlen << 6) & 0x0003ffc0) |0x0000003f;
    reg_write(qnffd, CAM1_MASK, data);
    data = 0xffffffff;
    reg_write(qnffd, CAM0_MASK, data);
    
    // CAM_DATA
    data = (((uint32_t)rule->data.proto << 24) & 0xff000000) |(((uint32_t)rule->data.sip >> 8) & 0x00ffffff);
    reg_write(qnffd, CAM4_DATA, data);
    data = (((uint32_t)rule->data.sip << 24) & 0xff000000) | (((uint32_t)rule->data.dip >> 8) & 0x00ffffff);
    reg_write(qnffd, CAM3_DATA, data);  
    data =(((uint32_t)rule->data.dip  << 24) & 0xff000000) | (((uint32_t)rule->data.sport << 8) & 0x00ffff00) | (((uint32_t)rule->data.dport >> 8) & 0x000000ff) ;
    reg_write(qnffd, CAM2_DATA, data);  
    data = (((uint32_t)rule->data.dport<<24) & 0xff000000) | (((uint32_t)rule->data.tcpflag << 18) & 0x00fc0000) | (((uint32_t)rule->data.pktlen << 6) & 0x0003ffc0) |0x0000003f;
    reg_write(qnffd, CAM1_DATA, data);
    data = 0x0;
    reg_write(qnffd, CAM0_DATA, data);
    
    for (i = 0; i < 10; i++)
    {
        data = reg_read(qnffd,  CAM_ADDR);
        if ((data & 0x80000000) == 0x0)
        {
            break;
        }
        else
        {
            usleep(1);
        }
    }
    if (i == 10)
    {
        printf("qnf_rule_tuple_set: write cam busy!\n");
        return -1;
    }
    
    //  update cam 0
    data = entry | 0x80000000;
    reg_write(qnffd, CAM_ADDR , data);
    
    return 0;
}

// clear TCAM and filter
int qnf_cam_tuple_clear(int qnffd, unsigned entry)
{
    unsigned int i;
    uint32_t     data;
    
    reg_write(qnffd, CAM4_MASK, 0x0);
    reg_write(qnffd, CAM3_MASK, 0x0);
    reg_write(qnffd, CAM2_MASK, 0x0);
    reg_write(qnffd, CAM1_MASK, 0x0);
    reg_write(qnffd, CAM0_MASK, 0x0);
    reg_write(qnffd, CAM4_DATA, 0x0);
    reg_write(qnffd, CAM3_DATA, 0x0);
    reg_write(qnffd, CAM2_DATA, 0x0);
    reg_write(qnffd, CAM1_DATA, 0x0);
    reg_write(qnffd, CAM0_DATA, 0x0);//match none
    
    // first check whether cam busy or not
    for (i = 0; i < 10; i++)
    {
        data = reg_read(qnffd,  CAM_ADDR);
        if ((data & 0x80000000) == 0x0)
        {
            break;
        }
        else
        {
            usleep(1);
        }
    }
    if (i == 10)
    {
        printf("qnf_rule_tuple_set: write cam busy!\n");
        return -1;
    }
    
    reg_write(qnffd, CAM_ADDR, 0x80000000 | entry);
    usleep(1);
    
    return 0;
}

// for cam-payload
int qnf_cam_payload_init(int qnffd)
{
    // set last item to match all for xe1
    reg_write(qnffd, CAM4_MASK, 0xffffffff);
    reg_write(qnffd, CAM3_MASK, 0xffffffff);
    reg_write(qnffd, CAM2_MASK, 0xffffffff);
    reg_write(qnffd, CAM1_MASK, 0xffffffff);
    reg_write(qnffd, CAM0_MASK, 0xffffffff);
    reg_write(qnffd, CAM4_DATA, 0x00000000);
    reg_write(qnffd, CAM3_DATA, 0x00000000);
    reg_write(qnffd, CAM2_DATA, 0x00000000);
    reg_write(qnffd, CAM1_DATA, 0x00000000);
    reg_write(qnffd, CAM0_DATA, 0x00000000);
    reg_write(qnffd, CAM_ADDR,  0x8000007f); //the LAST rule, bit 6 indicate rule_2
    usleep(1);
    
    return 0;
}

int qnf_cam_payload_set(int qnffd, unsigned int entry, qnf_rule_payload_t *rule)
{
    uint32_t data, mask;
    int      i;
    
    if (entry >= MAX_RULE_NO){
        printf("Wrong rule entry number %u, must be in 0-%d.\n", entry, MAX_RULE_NO);
        return -1;
    }
    
    /* write cam mask 1 , mask = 1 means don't care */
    mask = ((uint32_t)rule->mask[16] << 24) | ((uint32_t)rule->mask[17] << 16) | ((uint32_t)rule->mask[18] << 8) | ((uint32_t)rule->mask[19]); 
    reg_write(qnffd, CAM0_MASK, mask);
    mask = ((uint32_t)rule->mask[12] << 24) | ((uint32_t)rule->mask[13] << 16) | ((uint32_t)rule->mask[14] << 8) | ((uint32_t)rule->mask[15]);
    reg_write(qnffd, CAM1_MASK, mask);
    mask = ((uint32_t)rule->mask[8] << 24) | ((uint32_t)rule->mask[9] << 16) | ((uint32_t)rule->mask[10] << 8) | ((uint32_t)rule->mask[11]);
    reg_write(qnffd, CAM2_MASK, mask);  
    mask = ((uint32_t)rule->mask[4] << 24) | ((uint32_t)rule->mask[5] << 16) | ((uint32_t)rule->mask[6] << 8) | ((uint32_t)rule->mask[7]);
    reg_write(qnffd, CAM3_MASK, mask);
    mask = ((uint32_t)rule->mask[0] << 24) | ((uint32_t)rule->mask[1] << 16) | ((uint32_t)rule->mask[2] << 8) | ((uint32_t)rule->mask[3]);
    reg_write(qnffd, CAM4_MASK, mask);
    
    data = ((uint32_t)rule->data[16] << 24) | ((uint32_t)rule->data[17] << 16) | ((uint32_t)rule->data[18] << 8) | ((uint32_t)rule->data[19]); 
    reg_write(qnffd, CAM0_DATA, data);
    data = ((uint32_t)rule->data[12] << 24) | ((uint32_t)rule->data[13] << 16) | ((uint32_t)rule->data[14] << 8) | ((uint32_t)rule->data[15]);
    reg_write(qnffd, CAM1_DATA, data);
    data = ((uint32_t)rule->data[8] << 24) | ((uint32_t)rule->data[9] << 16) | ((uint32_t)rule->data[10] << 8) | ((uint32_t)rule->data[11]);
    reg_write(qnffd, CAM2_DATA, data);  
    data = ((uint32_t)rule->data[4] << 24) | ((uint32_t)rule->data[5] << 16) | ((uint32_t)rule->data[6] << 8) | ((uint32_t)rule->data[7]);
    reg_write(qnffd, CAM3_DATA, data);
    data = ((uint32_t)rule->data[0] << 24) | ((uint32_t)rule->data[1] << 16) | ((uint32_t)rule->data[2] << 8) | ((uint32_t)rule->data[3]);
    reg_write(qnffd, CAM4_DATA, data);  
    
    // check cam busy or not
    for (i = 0; i < 10; i++)
    {
        data = reg_read(qnffd,  CAM_ADDR);
        if ((data & 0x80000000) == 0x0)
        {
            break;
        }
        else
        {
            usleep(1);
        }       
    }   
    if (i == 10)
    {
        printf("qnf_rule_tuple_set: write cam busy!\n");
        return -1;
    }
    
    //  update cam 1
#ifdef BUILD_MC20E    
    data =entry | 0x80000040; //CAM PAYLOAD
#else
    data =entry | 0x80000020; //CAM PAYLOAD
#endif
    reg_write(qnffd, CAM_ADDR , data);
    
    return 0;
}

/* clear the setted payload entry */
int qnf_cam_payload_clear(int qnffd, unsigned int entry)
{
    int             i;
    uint32_t    data;
    
    reg_write(qnffd, CAM4_MASK, 0x0);
    reg_write(qnffd, CAM3_MASK, 0x0);
    reg_write(qnffd, CAM2_MASK, 0x0);
    reg_write(qnffd, CAM1_MASK, 0x0);
    reg_write(qnffd, CAM0_MASK, 0x0);
    reg_write(qnffd, CAM4_DATA, 0x0);
    reg_write(qnffd, CAM3_DATA, 0x0);
    reg_write(qnffd, CAM2_DATA, 0x0);
    reg_write(qnffd, CAM1_DATA, 0x0);
    reg_write(qnffd, CAM0_DATA, 0x0); //match all zero, seems impossible in real world
    
    // check cam busy or not
    for (i = 0; i < 10; i++)
    {
        data = reg_read(qnffd,  CAM_ADDR);
        if ((data & 0x80000000) == 0x0)
        {
            break;
        }
        else
        {
            usleep(1);
        }       
    }
    if (i == 10)
    {
        printf("qnf_rule_tuple_set: write cam busy!\n");
        return -1;
    }
#ifdef BUILD_MC20E    	
    reg_write(qnffd, CAM_ADDR, 0x80000040 | entry);
#else
    reg_write(qnffd, CAM_ADDR, 0x80000020 | entry);
#endif
    usleep(1);
    
    return 0;
}

// although MC20E_PPOE only have 32x32 lut, we still use 64x64 index in fpga
#define     RULE1_INDEX_OFFSET   (21)
#define     RULE2_INDEX_OFFSET   (16)

int qnf_cam_lut_init(int qnffd)
{
    int rule1_index, rule2_index;
    uint32_t    data = 0;
    
    // for lut 0
    for (rule1_index = 0; rule1_index < MAX_RULE_NO; rule1_index++)
        for (rule2_index = 0; rule2_index < MAX_RULE_NO; rule2_index++)
        {
               // default is hash
            data = ((uint16_t)rule1_index << RULE1_INDEX_OFFSET) | ((uint16_t)rule2_index << RULE2_INDEX_OFFSET) | 0x40000020;
            reg_write(qnffd, TAG_MAP_REG, data);
        }
    
    return 0;
}

int qnf_cam_lut_set(int qnffd, int rule1_index, int rule2_index, qnf_action_t *action)
{
    uint32_t    data = 0;
    
    switch(action->cmd) {
        // 2: hash  
        case 2:
            data = ((uint32_t)rule1_index << RULE1_INDEX_OFFSET) | ((uint32_t)rule2_index << RULE2_INDEX_OFFSET) | (0x80000020);
            reg_write(qnffd, TAG_MAP_REG, data);
            break;
        // 3: drop              
        case 3: 
            data = ((uint32_t)rule1_index << RULE1_INDEX_OFFSET) | ((uint32_t)rule2_index << RULE2_INDEX_OFFSET) | (0x80000030);
            reg_write(qnffd, TAG_MAP_REG, data); 
            break;
        default:
            printf("Error: unsupported command!\n"); 
            break;
    }
    
    return 0;
}

int qnf_cam_lut_clear(int qnffd, int rule1_index, int rule2_index)
{
    uint16_t    data = 0;
    
    // default is hash
    data = ((uint16_t)rule1_index << RULE1_INDEX_OFFSET) | ((uint16_t)rule2_index << RULE2_INDEX_OFFSET) | (0x80000020);
    reg_write(qnffd, TAG_MAP_REG, data); 
    
    return 0;
}

int qnf_set_pktlen_range(int qnffd, int index, int range_lo, int range_hi)
{
    uint32_t data = 0;

    data = (range_hi & 0x0000ffff << 16) | (range_lo & 0x0000ffff);
    reg_write(qnffd, PKTLEN_DATA_REG, data);
    usleep(1);
    
    reg_write(qnffd, PKTLEN_ADDR_REG, index);
    usleep(1);
    
    return 0;
}
#endif


#if defined(BUILD_MC20E) || defined(BUILD_MC20P) || defined(BUILD_MC20P_MC4P25) || (BUILD_MC20E_PPOE)
//Remote Update API
/*******************************************************************************/
void set_logic_num(int qnffd,uint64_t sn_logic)
{
    uint16_t temp = 0;  
    uint32_t block_addr = 0;
    /* set configuration register command */
    set_config_register(qnffd, 0xBDDF);
    
    /* clear status register */
    clear_stats_regs(qnffd);
    
    block_unlock(qnffd, 14, 0);
    block_erase(qnffd, 14, 0);
    block_addr = 14*BANK_SIZE_W + 0*0x10000;
    /*set logic serial num*/
    temp =(uint16_t) ( (sn_logic & 0xffff000000000000ull) >> 48);
    word_program(qnffd,block_addr,temp);
    temp =(uint16_t) ( (sn_logic & 0x0000ffff00000000ull) >> 32);
    word_program(qnffd,block_addr+1,temp);
    temp =(uint16_t) ( (sn_logic & 0x00000000ffff0000ull) >> 16);
    word_program(qnffd,block_addr+2,temp);
    temp =(uint16_t) (sn_logic & 0x000000000000ffffull) ;
    word_program(qnffd,block_addr+3,temp);
}

void set_hardware_num(int qnffd,uint64_t no)
{
    uint16_t temp = 0;  
    uint32_t block_addr = 0;
    /* set configuration register command */
    set_config_register(qnffd, 0xBDDF);
    
    /* clear status register */
    clear_stats_regs(qnffd);
    
    block_unlock(qnffd, 14, 1);
    block_erase(qnffd, 14, 1);
    block_addr = 14*BANK_SIZE_W + 1*0x10000;
    /*set logic serial num*/
    temp =(uint16_t) ( (no & 0xffff000000000000ull) >> 48);
    word_program(qnffd,block_addr,temp);
    temp =(uint16_t) ( (no & 0x0000ffff00000000ull) >> 32);
    word_program(qnffd,block_addr+1,temp);
    temp =(uint16_t) ( (no & 0x00000000ffff0000ull) >> 16);
    word_program(qnffd,block_addr+2,temp);
    temp =(uint16_t) (no & 0x000000000000ffffull) ;
    word_program(qnffd,block_addr+3,temp);
}

uint64_t read_logic_num(int qnffd)
{
    uint16_t temp = 0;
    uint64_t sn = 0;
    uint32_t block_addr = 0;
    
    /* set configuration register command */
    set_config_register(qnffd, 0xBDDF);
    
    /* clear status register */
    clear_stats_regs(qnffd);
    
    flash_write(qnffd,14*BANK_SIZE_W,0xff);

    block_addr = 14*BANK_SIZE_W + 0*0x10000;
    temp = flash_read(qnffd,block_addr);
    sn = (uint64_t)temp << 48;
    temp = flash_read(qnffd,block_addr+1);
    sn |= (uint64_t)temp << 32;
    temp = flash_read(qnffd,block_addr+2);
    sn |= (uint64_t)temp << 16;
    temp = flash_read(qnffd,block_addr+3);
    sn |= (uint64_t)temp;
    
    return sn;
}

uint64_t read_hardware_num(int qnffd)
{
    uint16_t temp = 0;
    uint64_t sn = 0;
    uint32_t block_addr = 0;
    
    /* set configuration register command */
    set_config_register(qnffd, 0xBDDF);
    
    /* clear status register */
    clear_stats_regs(qnffd);
    
    flash_write(qnffd,14*BANK_SIZE_W,0xff);

    block_addr = 14*BANK_SIZE_W + 1*0x10000;
    temp = flash_read(qnffd,block_addr);
    sn = (uint64_t)temp << 48;
    temp = flash_read(qnffd,block_addr + 1);
    sn |= (uint64_t)temp << 32;
    temp = flash_read(qnffd,block_addr + 2);
    sn |= (uint64_t)temp << 16;
    temp = flash_read(qnffd,block_addr + 3);
    sn |= (uint64_t)temp;
    
    return sn;
}

int qnf_judge_serial_num(int qnffd)
{
    uint64_t sn = 0;
    uint8_t sn_tmp = 0;
    sn = read_logic_num(qnffd);
    printf("sn is 0x%016llx\n",sn);
    /*read the bits of 47~40*/
    sn_tmp =(uint8_t)((sn & 0x0000ff0000000000ull)>>40);
    printf("tcam type :0x%x\n",sn_tmp);
    
    if(sn_tmp == 1)
    {
        /*in this case, TCAM's type is 72100*/
        return 1;
    }
    else if(sn_tmp == 0)
    {
        /*in this case, TCAM's type is 42100*/
        return 0;
    }
    else
    {
        /*int this case, serial_num in flush is unnormal*/
        return -1;
    }
}

uint16_t bit_swap(uint16_t src)
{   
    uint16_t dst = 0, i = 0;    
    
    for(i = 0; i < 16; i++)     
        dst |= ( (i < 8) ? ((src & (1 << i)) << (15 - i * 2)):((src & (1 << i)) >> (2 * i - 15)) ); 
    
    return dst;
}

uint16_t byte_swap(uint16_t src)
{   
    uint16_t dst = 0, i = 0;
    uint16_t lo, hi, src_tmp;
    
    // do the byte swap
    lo = src & 0xff;
    hi = (src >> 8) & 0xff;
    src_tmp = (lo << 8) | hi;
    
    for(i = 0; i < 16; i++)     
        dst |= ( (i < 8) ? ((src_tmp & (1 << i)) << (15 - i * 2)):((src_tmp & (1 << i)) >> (2 * i - 15)) );
//      dst |= ( (i < 8) ? ((src & (1 << i)) << (15 - i * 2)):((src & (1 << i)) >> (2 * i - 15)) ); 
    
    return dst;
}


#ifdef BUILD_MC20P_MC4P25
void flash_write(int qnffd, uint32_t addr, uint16_t data)
{
    uint32_t result32 = 0;
    
    if (addr > 0x7FFFFF)
    {
        printf("[%s]: addr is out of range(0~0x7FFFFF)\n", __FUNCTION__);
        return;
    }
    
    if (herd[qnffd].qnfinf.device_code == 0x3003)
            result32 = reg_read(qnffd, FLASH_ADDR_MC4P25);
    else
            result32 = reg_read(qnffd, FLASH_ADDR);
    
    while(result32){
            if (herd[qnffd].qnfinf.device_code == 0x3003)
                    result32 = reg_read(qnffd, FLASH_ADDR_MC4P25);
            else
                result32 = reg_read(qnffd, FLASH_ADDR);
    }
    
    if (herd[qnffd].qnfinf.device_code == 0x3003)
    {   
            reg_write(qnffd, FLASH_DATA_MC4P25, data);
            reg_write(qnffd, FLASH_ADDR_MC4P25, 0x80000000 + addr);
        }
        else
        {
              reg_write(qnffd, FLASH_DATA, data);
          reg_write(qnffd, FLASH_ADDR, 0x80000000 + addr);
        }   
}

uint16_t flash_read(int qnffd, uint32_t addr)
{
    uint32_t result32 = 0;
    
    if (addr > 0x7FFFFF)
    {
        printf("[%s]: addr is out of range(0~0x7FFFFF)\n", __FUNCTION__);
        return -1;
    }
    
    if (herd[qnffd].qnfinf.device_code == 0x3003)
            reg_write(qnffd, FLASH_ADDR_MC4P25, addr);
    else
            reg_write(qnffd, FLASH_ADDR, addr);
    
    // wait until data valid
    if (herd[qnffd].qnfinf.device_code == 0x3003)
            result32 = reg_read(qnffd, FLASH_ADDR_MC4P25);
    else
            result32 = reg_read(qnffd, FLASH_ADDR);
            
    while(!(result32 & 0x1)){
        if (herd[qnffd].qnfinf.device_code == 0x3003)
                result32 = reg_read(qnffd, FLASH_ADDR_MC4P25);
        else
                result32 = reg_read(qnffd, FLASH_ADDR);
    }
        
        if (herd[qnffd].qnfinf.device_code == 0x3003)
            result32 = reg_read(qnffd, FLASH_DATA_MC4P25);
    else
            result32 = reg_read(qnffd, FLASH_DATA);     
    
    return (uint16_t)result32;
}

void reconfig(int qnffd)
{
    uint32_t result32 = 0;
    
    if (herd[qnffd].qnfinf.device_code == 0x3003)
            result32 = reg_read(qnffd, FLASH_ADDR_MC4P25);
    else    
            result32 = reg_read(qnffd, FLASH_ADDR);
    
    while(result32){
            if (herd[qnffd].qnfinf.device_code == 0x3003)
                result32 = reg_read(qnffd, FLASH_ADDR_MC4P25);
        else
                result32 = reg_read(qnffd, FLASH_ADDR);
    }
        
        if (herd[qnffd].qnfinf.device_code == 0x3003)
                reg_write(qnffd, FLASH_DATA_MC4P25, 0xC0000000);
        else
            reg_write(qnffd, FLASH_DATA, 0xC0000000);
}
#else
void flash_write(int qnffd, uint32_t addr, uint16_t data)
{
    uint32_t result32 = 0;
    if (addr > 0x7FFFFF)
    {
        printf("[%s]: addr is out of range(0~0x7FFFFF)\n", __FUNCTION__);
        return;
    }
    
    result32 = reg_read(qnffd, FLASH_ADDR);
    
    while(result32){
        result32 = reg_read(qnffd, FLASH_ADDR);
    }
    
    reg_write(qnffd, FLASH_DATA, data);
    reg_write(qnffd, FLASH_ADDR, 0x80000000 + addr);

}

uint16_t flash_read(int qnffd, uint32_t addr)
{
    uint32_t result32 = 0;
    if (addr > 0x7FFFFF)
    {
        printf("[%s]: addr is out of range(0~0x7FFFFF)\n", __FUNCTION__);
        return -1;
    }
    reg_write(qnffd, FLASH_ADDR, addr);
    // wait until data valid
    result32 = reg_read(qnffd, FLASH_ADDR);
    while(!(result32 & 0x1)){
        result32 = reg_read(qnffd, FLASH_ADDR);
    }

    result32 = reg_read(qnffd, FLASH_DATA);
    return (uint16_t)result32;
}

void reconfig(int qnffd)
{
    uint32_t result32 = 0;
    result32 = reg_read(qnffd, FLASH_ADDR);
    while(result32){
        result32 = reg_read(qnffd, FLASH_ADDR);
    }

    reg_write(qnffd, FLASH_DATA, 0xC0000000);
}
#endif

void set_config_register(int qnffd, uint16_t data)
{
    flash_write(qnffd, data, 0x60);
    flash_write(qnffd, data, 0x03);
}

void clear_stats_regs(int qnffd)
{
    /* clear status register */
    flash_write(qnffd, 0, 0x50);
}

void word_program(int qnffd,uint32_t addr,uint16_t data)
{
    status_reg_t stat_reg ;
    stat_reg.value = 0;
    flash_write( qnffd,  addr, 0x40);
    flash_write( qnffd,  addr, data);
    do
    { stat_reg.value= flash_read(qnffd, addr);
    } while(stat_reg.SR_t.sr7== 0);
    
    if (stat_reg.SR_t.sr3 == 1)
    {
        printf("[%s]: VPP invalid error\n", __FUNCTION__);
        exit(1);
    }
    if (stat_reg.SR_t.sr4 == 1)
    {
        printf("[%s]: program error\n", __FUNCTION__);
        exit(2);
    }
    if (stat_reg.SR_t.sr1== 1)
    {
        printf("[%s]: program to protect block  error\n", __FUNCTION__);
        exit(3);
    }   
}

void blank_check_command(int qnffd,uint32_t blockToCheck)
{
    status_reg_t stat_reg ;
    stat_reg.value = 0;
    flash_write( qnffd, blockToCheck, 0XBC);
    flash_write( qnffd, blockToCheck, 0XCB);
    do
    {
        stat_reg.value= flash_read(qnffd, blockToCheck);
    }while(stat_reg.SR_t.sr7== 0);
    if ((stat_reg.SR_t.sr4 == 1)&&(stat_reg.SR_t.sr5 == 1))
    {
        printf("command sequence error\n");
    }
    if (stat_reg.SR_t.sr5== 1)
    {
        printf("blank check  error\n");
    }
}

void buffer_program(int qnffd,uint32_t start_addr,int n,uint16_t * pdata)
{
    if ((n < 0) || (n > 31))
    {
        printf("[%s]: n is out of range\n", __FUNCTION__);
        exit(1);
    }
    int i = 0;
    status_reg_t stat_reg ;
    stat_reg.value = 0;
    uint16_t *pd = pdata;
    uint32_t addr = start_addr;
    do
    {
        flash_write( qnffd, start_addr, 0xE8);
        stat_reg.value= flash_read(qnffd, start_addr);
    }while(stat_reg.SR_t.sr7== 0);
        
    flash_write(qnffd, start_addr, n);
    
    for(i = 0; i <= n; i++)
    {
        flash_write(qnffd, addr, byte_swap(*pd));
//      flash_write(qnffd, addr, bit_swap(*pd));
        addr++;
        pd++;
    }

    flash_write(qnffd,start_addr,0xD0);
    do
    {
        stat_reg.value= flash_read(qnffd, start_addr);
    }while(stat_reg.SR_t.sr7== 0);

    if (stat_reg.SR_t.sr3 == 1)
    {
        printf("VPP invalid error\n");
        exit(1);
    }
    if (stat_reg.SR_t.sr4 == 1)
    {
        printf("program error\n");
        exit(2);
    }
    if (stat_reg.SR_t.sr1== 1)
    {
        printf("program to protect block  error\n");
        exit(3);
    }
}

void block_program(int qnffd, uint8_t bank, uint8_t block, uint32_t w_size, uint16_t* pdata)
{
    if (w_size == 0)
    {
        printf("[%s]: size is error: %d\n", __FUNCTION__, w_size);
        exit(1);
    }
    uint32_t start_addr = bank * 0x80000 + block * 0x10000;
    uint32_t need_buffer = 0;
    uint32_t last_words = 0;
    uint32_t i = 0;
    
    if ( (w_size % 32) == 0 )
    {
        need_buffer = w_size / 32;
        last_words  = 32;
    }
    else
    {
        need_buffer = w_size / 32 + 1;
        last_words  = w_size % 32;
    }

    uint16_t *pd = pdata; 
    for (i = 0; i < need_buffer - 1; i++)
    {
        buffer_program(qnffd, start_addr, 31, pd);
        pd += 32;
        start_addr += 32;
    }
    buffer_program(qnffd, start_addr, last_words-1, pd);
    //printf("bank %d, block %d program successful!\n", bank, block);
}

void bank_program(int qnffd, uint8_t bank, uint32_t w_size, uint16_t* pdata)
{
    uint8_t     need_block = 0;
    uint8_t     last_block = 0;
    uint32_t    last_words = 0;
    int             i = 0;
    uint16_t* pd = pdata;
    
    if (w_size == 0)
    {
        printf("[%s]: size is error\n", __FUNCTION__);
        exit(1);
    }
    
    if ( (w_size % BLOCK_SIZE_W) == 0 )
    {
        need_block = w_size / BLOCK_SIZE_W;
        last_words = BLOCK_SIZE_W;
    }
    else
    {
        need_block = w_size / BLOCK_SIZE_W + 1;
        last_words = w_size % BLOCK_SIZE_W;
    }
    last_block = need_block - 1;
    
    for(i = 0; i < need_block -1; i++)
    {
        block_program(qnffd, bank, i, BLOCK_SIZE_W, pd);
        pd += BLOCK_SIZE_W;
    }
    block_program(qnffd, bank, last_block, last_words, pd);
    printf(".");    
}

void block_unlock(int qnffd, uint8_t bank, uint8_t block)
{
    uint16_t result16 = 0;
    /* block unlock */
    flash_write(qnffd, bank * 0x80000, 0x60);
    flash_write(qnffd, bank * 0x80000 + block * 0x10000, 0xD0);

    /* read block protect status */
    flash_write(qnffd, bank * 0x80000, 0x90);
    result16 = flash_read(qnffd, bank * 0x80000 + block * 0x10000+2);
    
    if (result16 != 0)
    {
        printf("block unlock fail\n");
        exit(1);
    }
}

void bank_unlock(int qnffd, uint8_t bank)
{
    int i = 0;
    for(i = 0; i < 8; i++)
        block_unlock(qnffd, bank, i);
}

void block_erase(int qnffd, uint8_t bank, uint8_t block)
{
    status_reg_t stat_reg ;
    stat_reg.value = 0;
    flash_write(qnffd, bank * 0x80000, 0x20);
    flash_write(qnffd, bank * 0x80000 + block * 0x10000, 0xD0);
    do
    {
        stat_reg.value= flash_read(qnffd, bank * 0x80000);
    }while(stat_reg.SR_t.sr7== 0);

    if (stat_reg.SR_t.sr3 == 1)
    {
        printf("[%s]: VPP invalid error\n", __FUNCTION__);
        exit(1);
    }
    if ((stat_reg.SR_t.sr4 == 1)&&(stat_reg.SR_t.sr5 == 1))
    {
        printf("[%s]: command sequence error\n", __FUNCTION__);
        exit(2);
    }
    if (stat_reg.SR_t.sr5== 1)
    {
        printf("[%s]: blank check  error\n", __FUNCTION__);
        exit(3);
    }
    if (stat_reg.SR_t.sr1== 1)
    {
        printf("[%s]: program to protect block  error\n", __FUNCTION__);
        exit(4);
    }
}

void bank_erase(int qnffd, uint8_t bank)
{
    int i = 0;
    for(i = 0; i < 8; i++)
    {
        block_erase(qnffd, bank, i);
        //printf("bank %d, block %d erase successful!\n", bank, i);
    }
    printf(".");    
}

void locking_operation_command(int qnffd, uint32_t addr, lock_stat_t lock_operation)
{
    uint16_t tempt;
    tempt = 0;
    flash_write(qnffd,addr,0X60);
    if (lock_operation == LOCKED)
        flash_write(qnffd,addr,0X01);
    if (lock_operation == UNLOCKED)
        flash_write(qnffd,addr,0XD0);
    if ((lock_operation == LOCKED_AND_LOCKEDDOWN)||(lock_operation == UNLOCKED_AND_LOCKEDDOWN))
        flash_write(qnffd,addr,0X2F);
    flash_write(qnffd,addr,0X90);
    tempt = (lock_stat_t)flash_read(qnffd,addr);
    if (tempt != lock_operation)
        printf("lock operation don't match lock state, error!\n");
    flash_write(qnffd,addr,0XFF);
}

void protection_register_program_command(int qnffd, uint32_t addr, uint16_t data)
{
    status_reg_t stat_reg ;
    stat_reg.value = 0;
    flash_write(qnffd,addr,0XC0);
    flash_write(qnffd,addr,data);
    do
    {
        stat_reg.value= flash_read(qnffd, addr);
    }while(stat_reg.SR_t.sr7== 0);
    if (stat_reg.SR_t.sr3 == 1)
    {
        printf("VPP invalid error\n");
    }
    if (stat_reg.SR_t.sr4 == 1)
    {
        printf("program error\n");
    }
    if (stat_reg.SR_t.sr1== 1)
    {
        printf("program to protect block  error\n");
    }
}

void buffer_enhanced_factory_program_command(int qnffd,uint32_t start_addr, uint32_t count ,uint16_t data_flow [ ])
{
    int i ;
    uint32_t processed;
    uint32_t remainder;
    status_reg_t stat_reg ;
    stat_reg.value = 0;
    processed = 0;
    remainder = 1;
    remainder = count%32;
    
    if (remainder != 0)
    {
        printf("count should be the multiply of 32!\n");
        return ;
    }
    
    flash_write(qnffd,start_addr,0X80);
    flash_write(qnffd,start_addr,0XD0);
    
    do
    {
        stat_reg.value= flash_read(qnffd, start_addr);
        if (stat_reg.SR_t.sr3 == 1)
        {
            printf("VPP invalid error\n");
        }
        if (stat_reg.SR_t.sr4 == 1)
        {
            printf("program error\n");
        }
        if (stat_reg.SR_t.sr1== 1)
        {
            printf("program to protect block  error\n");
        }
    }while(stat_reg.SR_t.sr7== 1);
    
    for (processed = 0;processed < (count/32) ;processed++)
    {
        for (i = 0;i < 32;i++)
        {
            flash_write(qnffd,start_addr,data_flow[i]);
        }
        do
        {
            stat_reg.value= flash_read(qnffd, start_addr);
        }while(stat_reg.SR_t.sr0== 1);
    }
    
    //address unsure
    flash_write(qnffd,start_addr,0XFFFF);
    do
    {
        stat_reg.value= flash_read(qnffd, start_addr);
    } while(stat_reg.SR_t.sr7== 0);
    
    if (stat_reg.SR_t.sr3 == 1)
    {
        printf("VPP invalid error\n");
    }
    
    if (stat_reg.SR_t.sr4 == 1)
    {
        printf("program error\n");
    }
    
    if (stat_reg.SR_t.sr1== 1)
    {
        printf("program to protect block  error\n");
    }
}

int compare_file_order(char * path1,char *path2)
{
    FILE        *fp1 = NULL;
    FILE        *fp2 = NULL;
    uint16_t    tempt1 = 0;
    uint16_t    tempt2 = 0;
    long int    file_size1 = 0;
    long int    file_size2 = 0;
    
    fp1 = fopen(path1,"rb");
    fp2 = fopen(path2,"rb");
    if (fp1 == NULL)
    {
        printf("check usage of %s\n",path1);
    }
    
    if (fp2 == NULL)
    {
        printf("check usage of %s\n",path2);
    }
    
    fseek(fp1,0L,SEEK_END);
    file_size1 = ftell(fp1);
    fseek(fp2,0L,SEEK_END);
    file_size2 = ftell(fp2);
    rewind(fp1);
    rewind(fp2);
    
    if (file_size1 != file_size2)
    {
        printf("The sizes of two files are different!\n");
        return -1;
    }
    
    if ( (file_size1%2) != 0 )
    {
        printf("The size of %s isn't even! \n",path1);
        return -1;
    }
    
    do
    {
        fread(&tempt1,sizeof(uint16_t),1,fp1);
        if (ftell(fp1) <= (file_size1 - 8))
            tempt1 = byte_swap(tempt1);
//          tempt1 = bit_swap(tempt1);
        
        fread(&tempt2,sizeof(uint16_t),1,fp2);
        if (tempt1 != tempt2)
        {
            printf("%s is different from %s\n",path1,path2);
            return -1;
        }
    } while (ftell(fp1) < file_size1);
    
    fclose(fp1);
    fclose(fp2);
    
    return 0;
}
#endif

