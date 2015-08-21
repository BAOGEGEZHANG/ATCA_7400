/*
 *                  		Copyright QiCe Tech, 2007-2010
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:      
 *
 */

#ifndef __CS_CLIENT_H__
#define __CS_CLIENT_H__


/* define cmd result buf size */
#define MAX_TTY_BUF_SIZE             2048*64

typedef struct consoleserver_handle 
{
    int fd_srv;
    struct csipcrec *reqbuf;
} CONSRV;

typedef struct res_result
{
    int data_size;
    char data[MAX_TTY_BUF_SIZE];
} res_result_t;

extern int cs_cl_open(CONSRV *cs);
extern int cs_cl_close(CONSRV *cs);
extern int cs_cl_insert_str(CONSRV *cs, char *data, res_result_t *result);
extern int cs_cl_shutdown(CONSRV *cs);


#endif
