/*
 *                  		Copyright QiCe Tech, 2007-2010
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:      
 *
 */

#ifndef __DB_CLIENT_H__
#define __DB_CLIENT_H__

#include "utils/hash.h"
#include "utils/errprint.h"
#include "db_server/db_server.h"

#define DB_CL_LOCK(p)   { if (db_cl_lock(p) < 0) errprint("Can't lock database.\n"); }
#define DB_CL_UNLOCK(p)   { if (db_cl_unlock(p) < 0) errprint("Can't unlock database.\n"); }

#define DB_CL_GET_UL(pdb, pul)      if (db_cl_get_ul((pdb), (pdb)->key_buf, (pul))) {\
                                         errprint("Can't get key '%s'.\n", (pdb)->key_buf); \
                                         return -1; } 
#define DB_CL_GET_UL_ERR(pdb, pul, err) if (db_cl_get_ul((pdb), (pdb)->key_buf, (pul))) { \
                                         errprint("Can't get key '%s'.\n", (pdb)->key_buf); \
                                         err\
                                         return -1; }

#define DB_CL_INSERT_UL(pdb, ul)    if (db_cl_insert_ul((pdb), (pdb)->key_buf, (ul))) {\
                                         errprint("Can't insert key '%s'.\n", (pdb)->key_buf); \
                                         return -1; } 

#define DB_CL_INSERT_UL_ERR(pdb, ul, err) if (db_cl_insert_ul((pdb), (pdb)->key_buf, (ul))) {\
                                         errprint("Can't insert key '%s'.\n", (pdb)->key_buf); \
                                         err\
                                         return -1; }

#define DB_CL_GET_STR(pdb, str)      if (db_cl_get_str((pdb), (pdb)->key_buf, (str))) {\
                                         errprint("Can't get key '%s'.\n", (pdb)->key_buf); \
                                         return -1; }

#define DB_CL_GET_STR_ERR(pdb, str, err) if (db_cl_get_str((pdb), (pdb)->key_buf, (str))) {\
                                         errprint("Can't get key '%s'.\n", (pdb)->key_buf); \
                                         err\
                                         return -1; }

#define DB_CL_INSERT_STR(pdb, str)   if (db_cl_insert_str((pdb), (pdb)->key_buf, (str))) {\
                                         errprint("Can't insert key '%s'.\n", (pdb)->key_buf); \
                                         return -1; } 

#define DB_CL_INSERT_STR_ERR(pdb, str, err) if (db_cl_insert_str((pdb), (pdb)->key_buf, (str))) {\
                                         errprint("Can't insert key '%s'.\n", (pdb)->key_buf); \
                                         err\
                                         return -1; } 

#define DB_CL_GET_BLOB(pdb, pv, size) if (db_cl_get_blob((pdb), (pdb)->key_buf, (pv), (size))) {\
                                         errprint("Can't get key '%s'.\n", (pdb)->key_buf); \
                                         return -1; } 

#define DB_CL_GET_BLOB_ERR(pdb, pv, size, err) if (db_cl_get_blob((pdb), (pdb)->key_buf, (pv), (size))) {\
                                         errprint("Can't get key '%s'.\n", (pdb)->key_buf); \
                                         err\
                                         return -1; } 

#define DB_CL_INSERT_BLOB(pdb, pv, size) if (db_cl_insert_blob((pdb), (pdb)->key_buf, (pv), (size))) {\
                                         errprint("Can't insert key '%s'.\n", (pdb)->key_buf); \
                                         return -1; } 

#define DB_CL_INSERT_BLOB_ERR(pdb, pv, size, err) if (db_cl_insert_blob((pdb), (pdb)->key_buf, (pv), (size))) {\
                                         errprint("Can't insert key '%s'.\n", (pdb)->key_buf); \
                                         err\
                                         return -1; } 

#define DB_CL_DELETE(pdb)             if (db_cl_delete((pdb), (pdb)->key_buf) < 0)\
                                          { errprint("Can't delete key '%s'.\n", (pdb)->key_buf); return -1; }

#define DB_CL_DELETE_ERR(pdb, err)    if (db_cl_delete((pdb), (pdb)->key_buf) < 0) \
                                          { errprint("Can't delete key '%s'.\n", (pdb)->key_buf); err return -1; }

#define DB_CL_KEY_EXISTS(pdb)         db_cl_key_exists(pdb, pdb->key_buf)

#define DB_KEY_BUF_SIZE 128

struct database_handle {
    int fd_srv;
    struct dbipcrec *reqbuf;
    char key_buf[DB_KEY_BUF_SIZE];
};

typedef hash_search_state_t db_cl_search_state_t;
typedef struct database_handle DATABASE;

extern int db_cl_open(DATABASE *db);
extern int db_cl_close(DATABASE *db);
extern int db_cl_shutdown(DATABASE *db);
extern int db_cl_insert_ul(DATABASE *db, char *key, unsigned long data);
extern int db_cl_get_ul(DATABASE *db, char *key, unsigned long *data);
extern int db_cl_insert_str(DATABASE *db, char *key, char *data);
extern int db_cl_get_str(DATABASE *db, char *key, char *data);
extern int db_cl_insert_blob(DATABASE *db, char *key, void *data, int size);
extern int db_cl_get_blob(DATABASE *db, char *key, void *data, int size);
extern int db_cl_delete(DATABASE *db, char *key);
extern int db_cl_key_exists(DATABASE *db, char *key);
extern struct hash_table_datum *db_cl_get_first(DATABASE *db, char *pattern, db_cl_search_state_t *state);
extern struct hash_table_datum *db_cl_get_next(DATABASE *db, char *pattern, db_cl_search_state_t *state);
extern int db_cl_lock(DATABASE *db);
extern int db_cl_unlock(DATABASE *db);
extern int db_cl_set_key_buf(DATABASE *db, char *format, ...);

#endif
