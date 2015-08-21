/*
 *                  		Copyright QiCe Tech, 2007-2010
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:      
 *
 */

#ifndef __CSIPC_H__
#define __CSIPC_H__


#define CS_SRV_PORT         7668
#define CS_SRV_DATA_SIZE    2048*64
#define CS_SRV_BUF_SIZE     (sizeof(struct csipcrec) + CS_SRV_DATA_SIZE)


typedef enum {
    CSIPC_CLOSE,
    CSIPC_SET,
    CSIPC_SHUTDOWN
} csipc_cmd_t;

struct csipcrec 
{
    csipc_cmd_t cmd;
    int result;
    int data_size;
    char data[0];
};

#endif
