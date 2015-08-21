/*
 *                  		Copyright QiCe Tech, 2007-2010
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:      
 *
 */


#ifndef __HASH_H__
#define __HASH_H__

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
#define HASH_MALLOC(size)    malloc(size)
#define HASH_FREE(ptr)       free(ptr)

/*******************************************************************
 *
 * Description:     Data type definitions
 *
 *******************************************************************/
struct hash_datum {
    int size;
    void *data;
};

struct hash_table_datum {
    struct hash_table_datum *next;
    struct hash_datum key;
    int data_length;
    unsigned char data[0];
};


typedef struct {
    struct hash_table_datum *entry;
    int bin;
} hash_search_state_t;

typedef struct hash_table_datum HASH_TABLE;
typedef struct hash_datum HASH_DATUM;

/*******************************************************************
 *
 * Description:     Function prototypes
 *
 *******************************************************************/
extern int hash_open(HASH_TABLE **tbl);
extern int hash_close(HASH_TABLE *tbl);
extern int hash_insert(HASH_TABLE *ptbl, HASH_DATUM *key, HASH_DATUM *dat);
HASH_DATUM *hash_getkey(HASH_TABLE *ptbl, HASH_DATUM *key);
extern int hash_delete(HASH_TABLE *ptbl, char *key);
extern int hash_delete_key(HASH_TABLE *ptbl, HASH_DATUM *key);
extern int hash_insert_ul(HASH_TABLE *ptbl, char *key, unsigned long data);
extern int hash_get_ul(HASH_TABLE *ptbl, char *key, unsigned long *pdata);
extern int hash_insert_str(HASH_TABLE *ptbl, char *key, char *data);
extern int hash_get_str(HASH_TABLE *ptbl, char *key, char *data);
extern int hash_insert_blob(HASH_TABLE *ptbl, char *key, void *vpdata, int size);
extern int hash_get_blob(HASH_TABLE *ptbl, char *key, void *vpdata, int size);
extern int hash_key_exists(HASH_TABLE *ptbl, char *key);
extern struct hash_table_datum *hash_get_first(HASH_TABLE *ptbl, char *pattern, hash_search_state_t *state);
extern struct hash_table_datum *hash_get_next(HASH_TABLE *ptbl, char *pattern, hash_search_state_t *state);

#endif
