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
#include "db_server/db_server.h"
#include "db_client.h"

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
#define DEBUG(lvl, msg...)  if (dbdebug >= (lvl)) { printf(msg); }
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
static int db_cl_send_req(DATABASE *db, struct dbipcrec *req);
static int db_cl_read_res(DATABASE *db, struct dbipcrec *req);

/*******************************************************************
 *
 * Description:     Static (global) variables
 *
 *******************************************************************/
static int dbdebug = 0;

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
 * Description:     Open a connection to the db server.
 *
 * Modified args:   None
 *
 * Return value:    0 if ok, negative if error.
 *
 *******************************************************************/
int
db_cl_open(DATABASE *db)
{
    struct sockaddr_in srvaddr;
    
    if ((db->reqbuf = malloc(DB_SRV_BUF_SIZE)) == NULL)
    {
        errprint("Can't allocate request buffer for database client.\n");
        return -1;
    }

    memset(db->reqbuf, 0, DB_SRV_BUF_SIZE);
    if ((db->fd_srv = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        errprint("Can't create client socket.\n");
        free(db->reqbuf);
        return -1;
    }

    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    srvaddr.sin_port = htons(DB_SRV_PORT);

    DEBUG(1, "Connecting to db server at port %d.\n", ntohs(srvaddr.sin_port));
    
    if (connect(db->fd_srv, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0)
    {
        errprint("Can't connect to server socket.\n");
        free(db->reqbuf);
        return -1;
    }

    DEBUG(1, "connected to server.\n");

    return 0;
}

/*******************************************************************
 *
 * Description:     Close connection to db server.
 *
 * Modified args:   None
 *
 * Return value:    0 if ok, negative if error.
 *
 *******************************************************************/
int
db_cl_close(DATABASE *db)
{
    db->reqbuf->cmd = DBIPC_CLOSE;
    
    if (db_cl_send_req(db, db->reqbuf) < 0)
    {
        errprint("Can't send close request to db srv.\n");
        return -1;
    }

    if (db_cl_read_res(db, db->reqbuf) < 0)
    {
        errprint("Can't read response from db server.\n");
        return -1;
    }

    if (db->reqbuf->result < 0)
    {
        errprint("Close command returned error from db server.\n");
    }
    
    close(db->fd_srv);
    free(db->reqbuf);
    db->reqbuf = NULL;

    return 0;
}

/*******************************************************************
 *
 * Description:     Shutdown db server.
 *
 * Modified args:   None
 *
 * Return value:    0 if ok, negative if error.
 *
 *******************************************************************/
int
db_cl_shutdown(DATABASE *db)
{
    db->reqbuf->cmd = DBIPC_SHUTDOWN;

    if (db_cl_send_req(db, db->reqbuf) < 0)
    {
        errprint("Can't send shutdown request to db srv.\n");
        return -1;
    }

    if (db_cl_read_res(db, db->reqbuf) < 0)
    {
        errprint("Can't read response from db server.\n");
        return -1;
    }

    if (db->reqbuf->result < 0)
    {
        errprint("Shutdown command returned error from db server.\n");
    }

    close(db->fd_srv);
    free(db->reqbuf);
    db->reqbuf = NULL;
    
    /* Make a new socket connection to make sure db_server_monitor terminates. */
    {
        struct sockaddr_in srvaddr;

        if ((db->fd_srv = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            return 0;
        }

        memset(&srvaddr, 0, sizeof(srvaddr));
        srvaddr.sin_family = AF_INET;
        srvaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        srvaddr.sin_port = htons(DB_SRV_PORT);

        DEBUG(1, "Connecting to db server at port %d.\n", ntohs(srvaddr.sin_port));
    
        if (connect(db->fd_srv, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0)
        {
            close(db->fd_srv);
            return 0;
        }
        close(db->fd_srv);
    }

    return 0;
}

/*******************************************************************
 *
 * Description:     Send request to db server.
 *
 * Modified args:   None
 *
 * Return value:    0 ok, negative if error.
 *
 *******************************************************************/
static int
db_cl_send_req(DATABASE *db, struct dbipcrec *req)
{
    int data_len;
    int bytes_written;
    int bytes_left;
    unsigned char *wrbuf;

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

    bytes_left = sizeof(struct dbipcrec) + data_len;
    wrbuf = (unsigned char *)req;
    do
    {
        if ((bytes_written = write(db->fd_srv, (void *)wrbuf, bytes_left)) < 0)
        {
            if (errno != EINTR)
            {
                errprint("Can't write request to db server.\n");
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
 * Description:     Read response form db server.
 *
 * Modified args:   None
 *
 * Return value:    0 ok, negative if error.
 *
 *******************************************************************/
static int
db_cl_read_res(DATABASE *db, struct dbipcrec *req)
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
        if ((bytes_read = read(db->fd_srv, rdbuf, bytes_left)) < 0)
        {
            if (errno != EINTR)
            {
                return -1;
            }
        }

        bytes_left -= bytes_read;
        rdbuf += bytes_read;
    } while (bytes_left > 0);
    
    switch (req->cmd)
    {
        case DBIPC_GETKEY:
            data_len = req->res.c_getkey.data_size + req->req.c_getkey.key_size;
            break;

        case DBIPC_GETFIRST:
            data_len = req->res.c_getfirst.data_size + req->res.c_getfirst.key_size;
            break;

        case DBIPC_GETNEXT:
            data_len = req->res.c_getnext.data_size + req->res.c_getnext.key_size;
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
        if ((bytes_read = read(db->fd_srv, rdbuf, bytes_left)) < 0)
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

/*******************************************************************
 *
 * Description:     Add an unsigned long key to the database.
 *
 * Modified args:   None
 *
 * Return value:    0 if ok, negative if error.
 *
 *******************************************************************/
int
db_cl_insert_ul(DATABASE *db, char *key, unsigned long data)
{
    db->reqbuf->cmd = DBIPC_ADDKEY;
    db->reqbuf->req.c_addkey.key_size = strlen(key) + 1;
    db->reqbuf->req.c_addkey.data_size = sizeof(unsigned long);
    strcpy(db->reqbuf->data, key);
    memcpy(&db->reqbuf->data[strlen(key) + 1], &data, sizeof(unsigned long));

    if (db_cl_send_req(db, db->reqbuf) < 0)
    {
        errprint("Can't send add key command to db server.\n");
        return -1;
    }

    if (db_cl_read_res(db, db->reqbuf) < 0)
    {
        errprint("Can't read result from db server in db_cl_insert_ul.\n");
        return -1;
    }

    DEBUG(3, "insert ul: %s, data: %08lx)\n", key, data);
    
    if (db->reqbuf->result < 0)
    {
        return -1;
    }
    
    return 0;
}

/*******************************************************************
 *
 * Description:     Get an unsigned long key from the database.
 *
 * Modified args:   None
 *
 * Return value:    0 if ok, negative if error.
 *
 *******************************************************************/
int
db_cl_get_ul(DATABASE *db, char *key, unsigned long *data)
{
    db->reqbuf->cmd = DBIPC_GETKEY;
    db->reqbuf->req.c_getkey.key_size = strlen(key) + 1;
    strcpy(db->reqbuf->data, key);

    if (db_cl_send_req(db, db->reqbuf) < 0)
    {
        errprint("Can't send get key command to db server.\n");
        return -1;
    }

    if (db_cl_read_res(db, db->reqbuf) < 0)
    {
        errprint("Can't read result from db server in db_cl_get_ul.\n");
        return -1;
    }

    if (db->reqbuf->result < 0)
    {
        return -1;
    }

    memcpy(data, &db->reqbuf->data[db->reqbuf->req.c_getkey.key_size], sizeof(unsigned long));
    
    DEBUG(3, "get ul: %s, data: %08lx)\n", key, *data);
    
    return 0;
}

/*******************************************************************
 *
 * Description:     Add a string key to the database.
 *
 * Modified args:   None
 *
 * Return value:    0 if ok, negative if error.
 *
 *******************************************************************/
int
db_cl_insert_str(DATABASE *db, char *key, char *data)
{
    db->reqbuf->cmd = DBIPC_ADDKEY;
    db->reqbuf->req.c_addkey.key_size = strlen(key) + 1;
    db->reqbuf->req.c_addkey.data_size = strlen(data) + 1;
    strcpy(db->reqbuf->data, key);
    strcpy(&db->reqbuf->data[strlen(key) + 1], data);

    if (db_cl_send_req(db, db->reqbuf) < 0)
    {
        errprint("Can't send add key command to db server.\n");
        return -1;
    }

    if (db_cl_read_res(db, db->reqbuf) < 0)
    {
        errprint("Can't read result from db server in db_cl_insert_str.\n");
        return -1;
    }

    if (db->reqbuf->result < 0)
    {
        return -1;
    }

    return 0;
}

/*******************************************************************
 *
 * Description:     Get a string key from the database.
 *
 * Modified args:   None
 *
 * Return value:    0 if ok, negative if error.
 *
 *******************************************************************/
int
db_cl_get_str(DATABASE *db, char *key, char *data)
{
    db->reqbuf->cmd = DBIPC_GETKEY;
    db->reqbuf->req.c_getkey.key_size = strlen(key) + 1;
    strcpy(db->reqbuf->data, key);

    if (db_cl_send_req(db, db->reqbuf) < 0)
    {
        errprint("Can't send get key command to db server.\n");
        return -1;
    }

    if (db_cl_read_res(db, db->reqbuf) < 0)
    {
        errprint("Can't read result from db server in db_cl_get_str.\n");
        return -1;
    }

    if (db->reqbuf->result < 0)
    {
        return -1;
    }

    memcpy(data, &db->reqbuf->data[db->reqbuf->req.c_getkey.key_size], db->reqbuf->res.c_getkey.data_size);

    return 0;
}

/*******************************************************************
 *
 * Description:     Add a blob key to the database.
 *
 * Modified args:   None
 *
 * Return value:    0 if ok, negative if error.
 *
 *******************************************************************/
int
db_cl_insert_blob(DATABASE *db, char *key, void *data, int size)
{
    db->reqbuf->cmd = DBIPC_ADDKEY;
    db->reqbuf->req.c_addkey.key_size = strlen(key) + 1;
    db->reqbuf->req.c_addkey.data_size = size;
    strcpy(db->reqbuf->data, key);
    memcpy(&db->reqbuf->data[strlen(key) + 1], data, size);

    if (db_cl_send_req(db, db->reqbuf) < 0)
    {
        errprint("Can't send add key command to db server.\n");
        return -1;
    }

    if (db_cl_read_res(db, db->reqbuf) < 0)
    {
        errprint("Can't read result from db server in db_cl_insert_blob.\n");
        return -1;
    }

    if (db->reqbuf->result < 0)
    {
        return -1;
    }

    return 0;
}

/*******************************************************************
 *
 * Description:     Get a blob key from the database.
 *
 * Modified args:   None
 *
 * Return value:    0 if ok, negative if error.
 *
 *******************************************************************/
int
db_cl_get_blob(DATABASE *db, char *key, void *data, int size)
{
    db->reqbuf->cmd = DBIPC_GETKEY;
    db->reqbuf->req.c_getkey.key_size = strlen(key) + 1;
    strcpy(db->reqbuf->data, key);

    if (db_cl_send_req(db, db->reqbuf) < 0)
    {
        errprint("Can't send get key command to db server.\n");
        return -1;
    }

    if (db_cl_read_res(db, db->reqbuf) < 0)
    {
        errprint("Can't read result from db server in db_cl_get_blob.\n");
        return -1;
    }

    if (db->reqbuf->result < 0)
    {
        return -1;
    }

    memcpy(data, &db->reqbuf->data[db->reqbuf->req.c_getkey.key_size], MIN(size, db->reqbuf->res.c_getkey.data_size));

    return 0;
}

/*******************************************************************
 *
 * Description:     Delete a key in the database
 *
 * Modified args:   None
 *
 * Return value:    0 if deleted, negative if not found.
 *
 *******************************************************************/
int
db_cl_delete(DATABASE *db, char *key)
{
    db->reqbuf->cmd = DBIPC_DELKEY;
    db->reqbuf->req.c_delkey.key_size = strlen(key) + 1;
    strcpy(db->reqbuf->data, key);

    if (db_cl_send_req(db, db->reqbuf) < 0)
    {
        errprint("Can't send delete key command to db server.\n");
        return -1;
    }

    if (db_cl_read_res(db, db->reqbuf) < 0)
    {
        errprint("Can't read result from db server in db_cl_delete.\n");
        return -1;
    }

    if (db->reqbuf->result < 0)
    {
        return -1;
    }
    
    return 0;
}

/*******************************************************************
 *
 * Description:     Check if a key exists.
 *
 * Modified args:   None
 *
 * Return value:    0 if not exist, > 0 if exist, negative if error.
 *
 *******************************************************************/
int
db_cl_key_exists(DATABASE *db, char *key)
{
    db->reqbuf->cmd = DBIPC_KEYEXISTS;
    db->reqbuf->req.c_getkey.key_size = strlen(key) + 1;
    strcpy(db->reqbuf->data, key);

    if (db_cl_send_req(db, db->reqbuf) < 0)
    {
        errprint("Can't send keyexists command to db server.\n");
        return -1;
    }

    if (db_cl_read_res(db, db->reqbuf) < 0)
    {
        errprint("Can't read result from db server in db_cl_key_exists.\n");
        return -1;
    }

    return db->reqbuf->result;
}

/*******************************************************************
 *
 * Description:     Get first key matching pattern.
 *
 * Modified args:   None
 *
 * Return value:    0 if ok, negative if error.
 *
 *******************************************************************/
struct hash_table_datum *
db_cl_get_first(DATABASE *db, char *pattern, db_cl_search_state_t *state)
{
    struct hash_table_datum *entry;
    int size;
    
    db->reqbuf->cmd = DBIPC_GETFIRST;
    db->reqbuf->req.c_getfirst.pattern_size = strlen(pattern) + 1;
    strcpy(db->reqbuf->data, pattern);

    if (db_cl_send_req(db, db->reqbuf) < 0)
    {
        errprint("Can't send getfirst command to db server.\n");
        return NULL;
    }

    if (db_cl_read_res(db, db->reqbuf) < 0)
    {
        errprint("Can't read result from db server in db_cl_get_first.\n");
        return NULL;
    }

    if (db->reqbuf->result < 0)
    {
        return NULL;
    }

    size = sizeof(struct hash_table_datum) + db->reqbuf->res.c_getfirst.key_size + db->reqbuf->res.c_getfirst.data_size;
    if ((entry = malloc(size)) == NULL)
    {
        errprint("Out of memory in db_cl_get_first.\n");
        return NULL;
    }
    
    *state = (db_cl_search_state_t)db->reqbuf->res.c_getfirst.h_state;
    entry->key.size = db->reqbuf->res.c_getfirst.key_size;
    entry->data_length = db->reqbuf->res.c_getfirst.data_size;
    entry->key.data = &entry->data[entry->data_length];

    memcpy(entry->data, &db->reqbuf->data[entry->key.size], entry->data_length);
    memcpy(entry->key.data, db->reqbuf->data, entry->key.size);

    return entry;
}

/*******************************************************************
 *
 * Description:     Get first key matching pattern.
 *
 * Modified args:   None
 *
 * Return value:    0 if ok, negative if error.
 *
 *******************************************************************/
struct hash_table_datum *
db_cl_get_next(DATABASE *db, char *pattern, db_cl_search_state_t *state)
{
    struct hash_table_datum *entry;
    int size;

    db->reqbuf->cmd = DBIPC_GETNEXT;
    db->reqbuf->req.c_getnext.pattern_size = strlen(pattern) + 1;
    db->reqbuf->req.c_getnext.h_state = *state;
    strcpy(db->reqbuf->data, pattern);

    if (db_cl_send_req(db, db->reqbuf) < 0)
    {
        errprint("Can't send getnext command to db server.\n");
        return NULL;
    }

    if (db_cl_read_res(db, db->reqbuf) < 0)
    {
        errprint("Can't read result from db server in db_cl_get_next.\n");
        return NULL;
    }

    if (db->reqbuf->result < 0)
    {
        return NULL;
    }

    size = sizeof(struct hash_table_datum) + db->reqbuf->res.c_getnext.key_size + db->reqbuf->res.c_getnext.data_size;
    if ((entry = malloc(size)) == NULL)
    {
        errprint("Out of memory in db_cl_get_first.\n");
        return NULL;
    }

    *state = (db_cl_search_state_t)db->reqbuf->res.c_getnext.h_state;
    entry->key.size = db->reqbuf->res.c_getnext.key_size;
    entry->data_length = db->reqbuf->res.c_getnext.data_size;
    entry->key.data = &entry->data[entry->data_length];

    memcpy(entry->data, &db->reqbuf->data[entry->key.size], entry->data_length);
    memcpy(entry->key.data, db->reqbuf->data, entry->key.size);

    return entry;
}

/*******************************************************************
 *
 * Description:     Lock the database.
 *
 * Modified args:   None
 *
 * Return value:    0 if successful, negative if error.
 *
 *******************************************************************/
int
db_cl_lock(DATABASE *db)
{
    db->reqbuf->cmd = DBIPC_LOCK;

    if (db_cl_send_req(db, db->reqbuf) < 0)
    {
        errprint("Can't send lock command to db server.\n");
        return -1;
    }

    if (db_cl_read_res(db, db->reqbuf) < 0)
    {
        errprint("Can't read result from db server in db_cl_get_ul.\n");
        return -1;
    }

    return db->reqbuf->result;
}

/*******************************************************************
 *
 * Description:     Unlock the database.
 *
 * Modified args:   None
 *
 * Return value:    0 if successful, negative if error.
 *
 *******************************************************************/
int
db_cl_unlock(DATABASE *db)
{
    db->reqbuf->cmd = DBIPC_UNLOCK;

    if (db_cl_send_req(db, db->reqbuf) < 0)
    {
        errprint("Can't send lock command to db server.\n");
        return -1;
    }

    if (db_cl_read_res(db, db->reqbuf) < 0)
    {
        errprint("Can't read result from db server in db_cl_get_ul.\n");
        return -1;
    }

    return db->reqbuf->result;
}

/*******************************************************************
 *
 * Description:     Utility func to set db->key_buf
 *
 * Modified args:   None
 *
 * Return value:    0 if successful, negative if error.
 *
 *******************************************************************/
int
db_cl_set_key_buf(DATABASE *db, char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    vsprintf(db->key_buf, format, ap);
    va_end(ap);

    return 0;
}
