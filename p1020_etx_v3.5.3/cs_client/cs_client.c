/*
 *                  		Copyright QiCe Tech, 2007-2010
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
 
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>

#include <stdarg.h>
#include <netinet/in.h>
#include <string.h>

#include "utils/errprint.h"
#include "cs_server/cs_server.h"
#include "cs_client.h"

/*******************************************************************
 *
 * Description:     Constant definitions
 *
 *******************************************************************/

/*******************************************************************
 *
 * Description:     Macro definitions
 *
 *******************************************************************/
#define DEBUG(lvl, msg...)  if (csdebug >= (lvl)) { printf(msg); }
#ifndef MIN
#define MIN(a,b)    (((a) <= (b)) ? (a) : (b))
#endif
    
/*******************************************************************
 *
 * Description:     Data type definitions
 *
 *******************************************************************/

/*******************************************************************
 *
 * Description:     Function prototypes
 *
 *******************************************************************/
static int cs_cl_send_req(CONSRV *cs, struct csipcrec *req);
static int cs_cl_read_res(CONSRV *cs, struct csipcrec *req);

/*******************************************************************
 *
 * Description:     Static (global) variables
 *
 *******************************************************************/
static int csdebug = 0;


/*******************************************************************
 *
 * Description:     Open a connection to the cs server.
 *
 * Modified args:   None
 *
 * Return value:    0 if ok, negative if error.
 *
 *******************************************************************/
int
cs_cl_open(CONSRV *cs)
{
    struct sockaddr_in srvaddr;
    
    if ((cs->reqbuf = malloc(CS_SRV_BUF_SIZE)) == NULL)
    {
        errprint("Can't allocate request buffer for CONSRV client.\n");
        return -1;
    }
    
    memset(cs->reqbuf, 0, CS_SRV_BUF_SIZE);
    if ((cs->fd_srv = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        errprint("Can't create client socket.\n");
        free(cs->reqbuf);
        return -1;
    }
    
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    srvaddr.sin_port = htons(CS_SRV_PORT);
    
    DEBUG(1, "Connecting to cs server at port %d.\n", ntohs(srvaddr.sin_port));
    
    if (connect(cs->fd_srv, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0)
    {
        errprint("Can't connect to server socket.\n");
        free(cs->reqbuf);
        return -1;
    }
    
    DEBUG(1, "connected to server.\n");
    
    return 0;
}

/*******************************************************************
 *
 * Description:     Close connection to cs server.
 *
 * Modified args:   None
 *
 * Return value:    0 if ok, negative if error.
 *
 *******************************************************************/
int
cs_cl_close(CONSRV *cs)
{
    cs->reqbuf->cmd = CSIPC_CLOSE;
    cs->reqbuf->data_size = 0;
    
    if (cs_cl_send_req(cs, cs->reqbuf) < 0)
    {
        errprint("Can't send close request to cs srv.\n");
        return -1;
    }
    
    if (cs_cl_read_res(cs, cs->reqbuf) < 0)
    {
        errprint("Can't read response from cs server.\n");
        return -1;
    }
    
    if (cs->reqbuf->result < 0)
    {
        errprint("Close command returned error from cs server.\n");
    }
    
    close(cs->fd_srv);
    free(cs->reqbuf);
    cs->reqbuf = NULL;
    
    return 0;
}

/*******************************************************************
 *
 * Description:     Add a string key to the CONSRV.
 *
 * Modified args:   None
 *
 * Return value:    0 if ok, negative if error.
 *
 *******************************************************************/
int
cs_cl_insert_str(CONSRV *cs, char *data, res_result_t *result)
{
    cs->reqbuf->cmd = CSIPC_SET;
    cs->reqbuf->data_size = strlen(data); //not include last '0', but include last '\n'
    strcpy(cs->reqbuf->data, data); // this will include the last '0'
    
    DEBUG(2, "cs_cl_insert_str, data_size is: %d, data is: %s\n", cs->reqbuf->data_size, cs->reqbuf->data);
    
    if (cs_cl_send_req(cs, cs->reqbuf) < 0)
    {
        errprint("Can't send add key command to cs server.\n");
        return -1;
    }
    
    /* check result */
    if (cs_cl_read_res(cs, cs->reqbuf) < 0)
    {
        errprint("Can't read result from cs server in cs_cl_insert_str.\n");
        return -1;
    }
    
    if (cs->reqbuf->result < 0)
    {
	      errprint("cs_cl_insert_str, result < 0\n");	
        return -1;
    }
    else if (cs->reqbuf->result == 1)    
    {
        if (cs->reqbuf->data_size > 0)
        {
            memcpy(result->data, cs->reqbuf->data, cs->reqbuf->data_size); //not include last '0'
            result->data[cs->reqbuf->data_size] = '\0';
	    result->data_size = cs->reqbuf->data_size;
        }
        else
        {
            result->data[0] = '\0';
	    result->data_size = 0;
            cs->reqbuf->data_size = 0;
        }    
        DEBUG(2, "cs_cl_insert_str, data_size returned: %d\n", cs->reqbuf->data_size);
    }
    else
    {
        printf("wrong ack result!\n");
        return -1;
    }
    
    return 0;
}

/*******************************************************************
 *
 * Description:     Add a string key to the CONSRV.
 *
 * Modified args:   None
 *
 * Return value:    0 if ok, negative if error.
 *
 *******************************************************************/
int
cs_cl_shutdown(CONSRV *cs)
{
    cs->reqbuf->cmd = CSIPC_SHUTDOWN;
    cs->reqbuf->data_size = 0; 
    
    DEBUG(2, "cs_cl_get_str, data_size is: %d\n", cs->reqbuf->data_size);
    
    if (cs_cl_send_req(cs, cs->reqbuf) < 0)
    {
        errprint("Can't send add key command to cs server.\n");
        return -1;
    }
    
    if (cs_cl_read_res(cs, cs->reqbuf) < 0)
    {
        errprint("Can't read result from cs server in cs_cl_insert_str.\n");
        return -1;
    }
        
    if (cs->reqbuf->result < 0)
    {
	      errprint("cs_cl_shutdown returned error!\n");	
        // return -1;
    }

    close(cs->fd_srv);
    free(cs->reqbuf);
    cs->reqbuf = NULL;
    
    return 0;
}


/*******************************************************************
 *
 * Description:     Send request to cs server.
 *
 * Modified args:   None
 *
 * Return value:    0 ok, negative if error.
 *
 *******************************************************************/
static int
cs_cl_send_req(CONSRV *cs, struct csipcrec *req)
{
    int bytes_written;
    int bytes_left;
    unsigned char *wrbuf;
    
    
    bytes_left = sizeof(struct csipcrec) + req->data_size;
    
    wrbuf = (unsigned char *)req;
    do
    {
        if ((bytes_written = write(cs->fd_srv, (void *)wrbuf, bytes_left)) < 0)
        {
            if (errno != EINTR)
            {
                errprint("Can't write request to cs server.\n");
                return -1;
            }
        }
        
        bytes_left -= bytes_written;
        wrbuf += bytes_written;
    } while (bytes_left > 0);
    
    return 0;
}

/*******************************************************************
 *
 * Description:     Read response form cs server.
 *
 * Modified args:   None
 *
 * Return value:    0 ok, negative if error.
 *
 *******************************************************************/
static int
cs_cl_read_res(CONSRV *cs, struct csipcrec *req)
{
    int bytes_read;
    int bytes_left;
    unsigned char *rdbuf;
    
    /* Read request struct first */
    bytes_left = sizeof(struct csipcrec);
    rdbuf = (unsigned char *)req;
    do
    {
        if ((bytes_read = read(cs->fd_srv, rdbuf, bytes_left)) < 0)
        {
            if (errno != EINTR)
            {
                return -1;
            }
        }

        bytes_left -= bytes_read;
        rdbuf += bytes_read;
    } while (bytes_left > 0);
    
    /* Read rest of request data */
    bytes_left = req->data_size;
    rdbuf = (unsigned char *)req->data;
    do
    {
        if ((bytes_read = read(cs->fd_srv, rdbuf, bytes_left)) < 0)
        {
            if (errno != EINTR)
            {
                return -1;
            }
        }

        bytes_left -= bytes_read;
        rdbuf += bytes_read;
    } while (bytes_left > 0);
    
    return 0;
}

