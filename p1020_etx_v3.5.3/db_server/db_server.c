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

#define _XOPEN_SOURCE 500
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#include <ctype.h>
#include <netinet/in.h>
#include <string.h>

#include "utils/errprint.h"
#include "utils/hash.h"
#include "db_server.h"

/*******************************************************************
 *
 * Description:     Constant definitions
 *
 *******************************************************************/
#define DB_SRV_LISTEN_Q_SIZE      5
#define DB_SRV_MON_TASK_PRIO      50
#define DB_SRV_MON_TASK_STACK     8192
#define DB_SRV_CLIENT_TASK_PRIO   50
#define DB_SRV_CLIENT_TASK_STACK  8192

/*******************************************************************
 *
 * Description:     Macro definitions
 *
 *******************************************************************/
#define DEBUG(lvl, msg...)  if (dbdebug >= (lvl)) { printf(msg); }
#define DO_DEBUG(lvl, blk)  if (dbdebug >= (lvl)) blk

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
static int dbipc_send_response(int clifd, struct dbipcrec *req);
static int dbipc_read_request(int clifd, struct dbipcrec *req);
static int dbipc_getkey(struct dbipcrec *req);
static int dbipc_addkey(struct dbipcrec *req);
static int dbipc_delkey(struct dbipcrec *req);
static int dbipc_close(struct dbipcrec *req);
static int dbipc_lock(struct dbipcrec *req);
static int dbipc_unlock(struct dbipcrec *req);
static int dbipc_keyexists(struct dbipcrec *req);
static int dbipc_getfirst(struct dbipcrec *req);
static int dbipc_getnext(struct dbipcrec *req);
static void *db_server(void *vp);
static int dump_blob(unsigned char *data, int size);

/*******************************************************************
 *
 * Description:     Static (global) variables
 *
 *******************************************************************/
static int dbdebug = 0;
static HASH_TABLE *g_db_srv_hash;
static int g_run = 1;

static pthread_mutex_t g_db_mutex;


/*******************************************************************
 *
 * Description:     Description....
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/

/*******************************************************************
 *
 * Description:     Description....
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
static void *
db_server_monitor(void *vp)
{
    pthread_attr_t tattr;
    pthread_t pid;

    int listenfd;
    int connfd;
    struct sockaddr_in srvaddr;
    struct sockaddr_in cliaddr;


    DEBUG(1, "db server process monitor started.\n");
    
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        errprint("Can't create server socket.\n");
        return NULL;
    }

    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    srvaddr.sin_port = htons(DB_SRV_PORT);
    

    if (bind(listenfd, (struct sockaddr *) &srvaddr, sizeof(srvaddr)) < 0)
    {
        errprint("Can't bind to server socket.\n");
        return NULL;
    }
    
    if (listen(listenfd, DB_SRV_LISTEN_Q_SIZE) < 0)
    {
        errprint("Can't listen to server socket.\n");
        return NULL;
    }

    DEBUG(1, "db server listening on port %d.\n", ntohs(srvaddr.sin_port));

    while (g_run)
    {
        int len;

        len = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len)) < 0)
        {
            errprint("Can't accept on server socket.\n");
            return NULL;
        }

        if (!g_run)
        {
            break;
        }

        DEBUG(1, "Creating new client process for handle %d.\n", connfd);
        
        pthread_attr_init(&tattr);
        pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);
        pthread_attr_setscope(&tattr, PTHREAD_SCOPE_SYSTEM);
        pthread_attr_setschedpolicy(&tattr, SCHED_OTHER);

        if (pthread_create(&pid, &tattr, db_server, (void *)connfd) != 0)
        {
            errprint("Can't create db server process.\n");
            close(connfd);
        }
        
    }

    close(listenfd);

    return 0;
}

/*******************************************************************
 *
 * Description:     Description....
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
int
start_db_srv()
{
    pthread_mutexattr_t mattr;
    
    if (hash_open(&g_db_srv_hash))
    {
        errprint("dbsrv can't create database.\n");
        return -1;
    }

    DEBUG(1, "hash created.\n");

    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&g_db_mutex, &mattr);

    db_server_monitor(NULL);

    return 0;
}

/*******************************************************************
 *
 * Description:     Description....
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
int
stop_db_srv()
{
    g_run = 0;
    hash_close(g_db_srv_hash);

    return 0;
}

/*******************************************************************
 *
 * Description:     Description....
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
static void *
db_server(void *vp)
{
    int clifd;
    struct dbipcrec *req;
    void *buf;


    clifd = (int)vp;
    
    DEBUG(1, "new db client process started for handle %d.\n", clifd);
    
    if ((buf = malloc(DB_SRV_BUF_SIZE)) == NULL)
    {
        errprint("Can't allocate request buffer.\n");
        close(clifd);
        return NULL;
    }

    req = (struct dbipcrec *) buf;
    
    while (1)
    {
        if (dbipc_read_request(clifd, req) < 0)
        {
            errprint("Error reading request.\n");
            free(buf);
            close(clifd);
            break;
        }
        
        switch (req->cmd)
        {
            case DBIPC_CLOSE:
                DEBUG(2, "Got close cmd.\n");
                dbipc_close(req);
                dbipc_send_response(clifd, req);
                free(buf);
                close(clifd);
                DEBUG(1, "db thread terminates.\n");
                return NULL;

            case DBIPC_GETKEY:
                req->result = dbipc_getkey(req);
                dbipc_send_response(clifd, req);
                break;

            case DBIPC_ADDKEY:
                req->result = dbipc_addkey(req);
                dbipc_send_response(clifd, req);
                break;

            case DBIPC_DELKEY:
                req->result = dbipc_delkey(req);
                dbipc_send_response(clifd, req);
                break;

            case DBIPC_SHUTDOWN:
                DEBUG(2, "Got shutdown cmd.\n");
                g_run = 0;
                req->result = 0;
                dbipc_send_response(clifd, req);
                free(buf);
                close(clifd);
                stop_db_srv();
                DEBUG(1, "db thread terminates.\n");
                return NULL;

            case DBIPC_LOCK:
                DEBUG(2, "Got lock cmd.\n");
                req->result = dbipc_lock(req);
                dbipc_send_response(clifd, req);
                break;
                
            case DBIPC_UNLOCK:
                DEBUG(2, "Got unlock cmd.\n");
                req->result = dbipc_unlock(req);
                dbipc_send_response(clifd, req);
                break;

            case DBIPC_KEYEXISTS:
                req->result = dbipc_keyexists(req);
                dbipc_send_response(clifd, req);
                break;

            case DBIPC_GETFIRST:
                DEBUG(2, "Got getfirst cmd.\n");
                req->result = dbipc_getfirst(req);
                dbipc_send_response(clifd, req);
                break;

            case DBIPC_GETNEXT:
                DEBUG(2, "Got getnext cmd.\n");
                req->result = dbipc_getnext(req);
                dbipc_send_response(clifd, req);
                break;

            default:
                DEBUG(2, "Got unknown cmd.\n");
                errprint("Invalid command in dbsrv.\n");
                break;
        }
    }

    return NULL;
}

/*******************************************************************
 *
 * Description:     close a client connection to the dbsrv.
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
static int
dbipc_close(struct dbipcrec *req)
{
    req->result = 0;

    return 0;
}

/*******************************************************************
 *
 * Description:     Get key from hash.
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
static int
dbipc_getkey(struct dbipcrec *req)
{
    HASH_DATUM *pd;
    HASH_DATUM key;

    key.size = req->req.c_getkey.key_size;
    key.data = req->data;
    if ((pd = hash_getkey(g_db_srv_hash, &key)) == NULL)
    {
        DEBUG(1, "getkey for '%s' failed.\n", (char *)key.data);
        return -1;
    }
    
    DEBUG(3, "get key: %s, data size: %d (First ul: %08lx)\n", (char *)key.data, pd->size, *((unsigned long *)pd->data));
    DO_DEBUG(4,
    {
        dump_blob(pd->data, pd->size);
    });

    memcpy(&req->data[key.size], pd->data, pd->size);
    req->res.c_getkey.data_size = pd->size;

    HASH_FREE(pd);
    
    return 0;
}

/*******************************************************************
 *
 * Description:     Add key to hash.
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
static int
dbipc_addkey(struct dbipcrec *req)
{
    HASH_DATUM key;
    HASH_DATUM data;

    key.size = req->req.c_addkey.key_size;
    key.data = req->data;
    data.size = req->req.c_addkey.data_size;
    data.data = &req->data[key.size];
    
    DEBUG(3, "add key: %s, data size: %d (First ul: %08lx)\n", (char *)key.data, data.size, *((unsigned long *)data.data));
    DO_DEBUG(4,
    {
        dump_blob(data.data, data.size);
    });
    
    if (hash_insert(g_db_srv_hash, &key, &data) < 0)
    {
        return -1;
    }

    return 0;
}

/*******************************************************************
 *
 * Description:     Delete key from hash.
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
static int
dbipc_delkey(struct dbipcrec *req)
{
    HASH_DATUM key;

    key.size = req->req.c_getkey.key_size;
    key.data = ((unsigned char *)req) + sizeof(struct dbipcrec);
    
    DEBUG(3, "delete key: %s\n", (char *)key.data);
    if (hash_delete_key(g_db_srv_hash, &key) < 0)
    {
        return -1;
    }

    return 0;
}

/*******************************************************************
 *
 * Description:     Lock the database.
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
static int
dbipc_lock(struct dbipcrec *req)
{
    if (pthread_mutex_lock(&g_db_mutex) != 0)
    {
        DEBUG(1, "failed to lock db mutex.\n");
        return -1;
    }
    
    return 0;
}

/*******************************************************************
 *
 * Description:     Unlock the database.
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
static int
dbipc_unlock(struct dbipcrec *req)
{
    if (pthread_mutex_unlock(&g_db_mutex) != 0)
    {
        DEBUG(1, "failed to lock db mutex.\n");
        return -1;
    }

    return 0;
}

/*******************************************************************
 *
 * Description:     Check if key exists in database.
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
static int
dbipc_keyexists(struct dbipcrec *req)
{
    HASH_DATUM key;

    key.size = req->req.c_getkey.key_size;
    key.data = ((unsigned char *)req) + sizeof(struct dbipcrec);
    
    return hash_key_exists(g_db_srv_hash, key.data);
}

/*******************************************************************
 *
 * Description:     Get first key mathing a pattern.
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
static int
dbipc_getfirst(struct dbipcrec *req)
{
    struct hash_table_datum *pd;
    hash_search_state_t state;

    if ((pd = hash_get_first(g_db_srv_hash, req->data, &state)) == NULL)
    {
        req->res.c_getfirst.key_size = 0;
        req->res.c_getfirst.data_size = 0;
        
        return -1;
    }

    memcpy(req->data, pd->key.data, pd->key.size);
    req->res.c_getfirst.key_size = pd->key.size;
    memcpy(&req->data[pd->key.size], pd->data, pd->data_length);
    req->res.c_getfirst.data_size = pd->data_length;
    req->res.c_getfirst.h_state = state;

    HASH_FREE(pd);
    
    return 0;
}

/*******************************************************************
 *
 * Description:     Get next key mathing a pattern.
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
static int
dbipc_getnext(struct dbipcrec *req)
{
    struct hash_table_datum *pd;
    hash_search_state_t state;

    state = req->req.c_getnext.h_state;
    if ((pd = hash_get_next(g_db_srv_hash, req->data, &state)) == NULL)
    {
        req->res.c_getnext.key_size = 0;
        req->res.c_getnext.data_size = 0;

        return -1;
    }

    memcpy(req->data, pd->key.data, pd->key.size);
    req->res.c_getnext.key_size = pd->key.size;
    memcpy(&req->data[pd->key.size], pd->data, pd->data_length);
    req->res.c_getnext.data_size = pd->data_length;
    req->res.c_getnext.h_state = state;

    HASH_FREE(pd);

    return 0;
}

/*******************************************************************
 *
 * Description:     Send response back to the client.
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
static int
dbipc_send_response(int clifd, struct dbipcrec *req)
{
    int data_len;
    int bytes_written;
    int bytes_left;
    unsigned char *wrbuf;

    switch (req->cmd)
    {
        case DBIPC_GETKEY:
            data_len = req->res.c_getkey.data_size + req->req.c_getkey.key_size;
            break;
            
        case DBIPC_GETFIRST:
            data_len = req->res.c_getfirst.key_size + req->res.c_getfirst.data_size;
            break;

        case DBIPC_GETNEXT:
            data_len = req->res.c_getnext.key_size + req->res.c_getnext.data_size;
            break;

        default:
            data_len = 0;
            break;
    }

    bytes_left = sizeof(struct dbipcrec) + data_len;
    wrbuf = (unsigned char *)req;
    do
    {
        if ((bytes_written = write(clifd, (void *)wrbuf, bytes_left)) < 0)
        {
            if (errno != EINTR)
            {
                errprint("Can't write response to client.\n");
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
 * Description:     Read command from client.
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
static int
dbipc_read_request(int clifd, struct dbipcrec *req)
{
    int data_len;
    int bytes_read;
    int bytes_left;
    unsigned char *rdbuf;


    /* Read request struct first */
    bytes_left = sizeof(struct dbipcrec);
    rdbuf = (unsigned char *)req;
    do
    {
        if ((bytes_read = read(clifd, rdbuf, bytes_left)) < 0)
        {
            if (errno != EINTR)
            {
                errprint("Can't read request from client.\n");
                return -1;
            }
        }

        bytes_left -= bytes_read;
        rdbuf += bytes_read;
    } while (bytes_left > 0);
    
    switch (req->cmd)
    {
        case DBIPC_GETKEY:
            data_len = req->req.c_getkey.key_size;
            break;

        case DBIPC_ADDKEY:
            data_len = req->req.c_addkey.key_size + req->req.c_addkey.data_size;
            break;

        case DBIPC_DELKEY:
            data_len = req->req.c_delkey.key_size;
            break;

        case DBIPC_KEYEXISTS:
            data_len = req->req.c_keyexists.key_size;
            break;

        case DBIPC_GETFIRST:
            data_len = req->req.c_getfirst.pattern_size;
            break;

        case DBIPC_GETNEXT:
            data_len = req->req.c_getnext.pattern_size;
            break;

        default:
            data_len = 0;
            break;
    }

    /* Read rest of request data */
    bytes_left = data_len;
    rdbuf = (unsigned char *)req->data;
    do
    {
        if ((bytes_read = read(clifd, rdbuf, bytes_left)) < 0)
        {
            if (errno != EINTR)
            {
                errprint("Can't read request from client.\n");
                return -1;
            }
        }

        bytes_left -= bytes_read;
        rdbuf += bytes_read;
    } while (bytes_left > 0);

    return 0;
}

/*******************************************************************
 *
 * Description:     Dump data in hex format.
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
static int
dump_blob(unsigned char *data, int size)
{
    int i;
    int j;
    int breaks;
    unsigned char *p;
    unsigned char *q;

    printf("data size: %d\n", size);
    printf("      %40s  ", "");
    /* Do hex dump */
    breaks = 0;
    p = q = data;
    j = 0;
    for (i = 0; i < size; i++)
    {
        printf("%02x ", *p++);
        if (++breaks % 8 == 0)
        {
            printf("- |");
            do
            {
                if (isprint(*q))
                {
                    printf("%c", *q);
                }
                else
                {
                    printf(".");
                }
                q++;
                j++;
            } while ((j % 8 != 0) && (j < size));
            printf("|\n      %40s  ", "");
        }
    }
    while (i++ % 8)
    {
        printf("   ");
    }
    if (j < size)
    {
        printf("- |");
        while (j < size)
        {
            if (isprint(*q))
            {
                printf("%c", *q);
            }
            else
            {
                printf(".");
            }
            q++;
            j++;
        }
        printf("|\n");
    }

    return 0;
}

/*******************************************************************
 *
 * Description:     Main function used in Linux.
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
int main(int argc, char *argv[])
{
    int val;

    if (argc == 2 && argv[1][0] == '-')
    {
        switch (argv[1][1])
        {
            case 'd':
            case 'D':
                val = atoi(&argv[1][2]);
                dbdebug = val;
                break;
            default:
                printf("Invalid option %c.\n", argv[1][1]);
                exit(1);
        }
    }
    
    start_db_srv();

    return 0;
}
