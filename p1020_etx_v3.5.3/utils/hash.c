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
#include <string.h>
#include "hash.h"
#include "match.h"

/*******************************************************************
 *
 * Description:     Constant definitions
 *
 *******************************************************************/
#define HASH_LOG_SIZE   10
#define HASH_TABLE_SIZE hashsize(HASH_LOG_SIZE)

/*******************************************************************
 *
 * Description:     Macro definitions
 *
 *******************************************************************/
#define hashsize(n) ((ub4) 1 << (n))
#define hashmask(n) (hashsize(n) - 1)
#define mix(a,b,c)  \
                    {\
                        a -= b; a -= c; a ^= (c >> 13); \
                        b -= c; b -= a; b ^= (a << 8);  \
                        c -= a; c -= b; c ^= (b >> 13); \
                        a -= b; a -= c; a ^= (c >> 12); \
                        b -= c; b -= a; b ^= (a << 16); \
                        c -= a; c -= b; c ^= (b >> 5);  \
                        a -= b; a -= c; a ^= (c >> 3);  \
                        b -= c; b -= a; b ^= (a << 10); \
                        c -= a; c -= b; c ^= (b >> 15); \
                    }
#define HASH(k)     ((hash((ub1 *)(k)->data,(k)->size,0x42424242)) & hashmask(HASH_LOG_SIZE))
#define HASH_STR(k) ((hash((ub1 *)(k),strlen(k),0x42424242)) & hashmask(HASH_LOG_SIZE))

/*******************************************************************
 *
 * Description:     Data type definitions
 *
 *******************************************************************/
typedef unsigned long ub4;
typedef unsigned char ub1;

/*******************************************************************
 *
 * Description:     Function prototypes
 *
 *******************************************************************/

/*******************************************************************
 *
 * Description:     Static (global) variables
 *
 *******************************************************************/


/*
--------------------------------------------------------------------
hash() -- hash a variable-length key into a 32-bit value
  k       : the key (the unaligned variable-length array of bytes)
  len     : the length of the key, counting by bytes
  initval : can be any 4-byte value
Returns a 32-bit value.  Every bit of the key affects every bit of
the return value.  Every 1-bit and 2-bit delta achieves avalanche.
About 6*len+35 instructions.

The best hash table sizes are powers of 2.  There is no need to do
mod a prime (mod is sooo slow!).  If you need less than 32 bits,
use a bitmask.  For example, if you need only 10 bits, do
  h = (h & hashmask(10));
In which case, the hash table should have hashsize(10) elements.

If you are hashing n strings (ub1 **)k, do it like this:
  for (i=0, h=0; i<n; ++i) h = hash( k[i], len[i], h);

By Bob Jenkins, 1996.  bob_jenkins@burtleburtle.net.  You may use this
code any way you wish, private, educational, or commercial.  It's free.

See http://burtleburtle.net/bob/hash/evahash.html
Use for hash table lookup, or anything where one collision in 2^^32 is
acceptable.  Do NOT use for cryptographic purposes.
--------------------------------------------------------------------
*/

static ub4
hash( ub1 *k,         /* the key */
          ub4 length,     /* the length of the key */
          ub4 initval     /* the previous hash, or an arbitrary value */
        )
{
    ub4 a,b,c,len;

   /* Set up the internal state */
    len = length;
    a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
    c = initval;         /* the previous hash value */

   /*---------------------------------------- handle most of the key */
    while (len >= 12)
    {
        a += (k[0] +((ub4)k[1]<<8) +((ub4)k[2]<<16) +((ub4)k[3]<<24));
        b += (k[4] +((ub4)k[5]<<8) +((ub4)k[6]<<16) +((ub4)k[7]<<24));
        c += (k[8] +((ub4)k[9]<<8) +((ub4)k[10]<<16)+((ub4)k[11]<<24));
        mix(a,b,c);
        k += 12; len -= 12;
    }

   /*------------------------------------- handle the last 11 bytes */
    c += length;
    switch(len)              /* all the case statements fall through */
    {
        case 11: c+=((ub4)k[10]<<24);
        case 10: c+=((ub4)k[9]<<16);
        case 9 : c+=((ub4)k[8]<<8);
      /* the first byte of c is reserved for the length */
        case 8 : b+=((ub4)k[7]<<24);
        case 7 : b+=((ub4)k[6]<<16);
        case 6 : b+=((ub4)k[5]<<8);
        case 5 : b+=k[4];
        case 4 : a+=((ub4)k[3]<<24);
        case 3 : a+=((ub4)k[2]<<16);
        case 2 : a+=((ub4)k[1]<<8);
        case 1 : a+=k[0];
     /* case 0: nothing left to add */
    }
    
    mix(a,b,c);
   /*-------------------------------------------- report the result */
    return c;
}

/*******************************************************************
 *
 * Description:     Allocate memory for a new hash table.
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
int
hash_open(HASH_TABLE **ptbl)
{
    
    if ((*ptbl = HASH_MALLOC(HASH_TABLE_SIZE * sizeof(void *))) == (void *)0)
    {
        return -1;
    }
    memset(*ptbl, 0, HASH_TABLE_SIZE * sizeof(void *));

    return 0;
}

/*******************************************************************
 *
 * Description:     Free hash table memory.
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
int
hash_close(HASH_TABLE *tbl)
{
    int i;
    struct hash_table_datum *entry;
    struct hash_table_datum *free_entry;

    for (i = 0; i < (int)HASH_TABLE_SIZE; i++)
    {
        entry = ((struct hash_table_datum **)tbl)[i];
        if (entry != NULL)
        {
            do {
                free_entry = entry;
                entry = entry->next;
                HASH_FREE(free_entry);
            } while(entry != NULL);
        }
    }
    
    HASH_FREE(tbl);

    return 0;
}

/*******************************************************************
 *
 * Description:     Lookup a key in the hash table.
 *
 * Modified args:   None
 *
 * Return value:    Pointer to table entry if found, otherwise null
 *
 *******************************************************************/
static struct hash_table_datum *
hash_lookup(HASH_TABLE *ptbl, HASH_DATUM *key)
{
    struct hash_table_datum *pentry;
    int index;

    index = HASH(key);
    pentry = ((struct hash_table_datum **)ptbl)[index];
    while (pentry != NULL && (key->size != pentry->key.size || (memcmp(key->data, pentry->key.data, key->size) != 0)))
    {
        pentry = pentry->next;
    }

    return pentry;
}

/*******************************************************************
 *
 * Description:     Get a key/value pair copy from the table.
 *
 * Modified args:   None
 *
 * Return value:    Pointer to data if found, otherwise null.
 *
 *******************************************************************/
HASH_DATUM *
hash_getkey(HASH_TABLE *ptbl, HASH_DATUM *key)
{
    struct hash_table_datum *entry;
    HASH_DATUM *new_copy;

    if ((entry = hash_lookup(ptbl, key)) == NULL)
    {
        return NULL;
    }

    if ((new_copy = HASH_MALLOC(sizeof(HASH_DATUM) + entry->data_length)) == NULL)
    {
        return NULL;
    }

    new_copy->size = entry->data_length;
    new_copy->data = (char *)new_copy + sizeof(HASH_DATUM);
    
    /* Copy data value */
    memcpy((char *)new_copy + sizeof(HASH_DATUM), entry->data, entry->data_length);

    return new_copy;
}

/*******************************************************************
 *
 * Description:     Insert a entry in the table.
 *
 * Modified args:   None
 *
 * Return value:    0 if successful, negative if error.
 *
 *******************************************************************/
int
hash_insert(HASH_TABLE *ptbl, HASH_DATUM *key, HASH_DATUM *dat)
{
    struct hash_table_datum *pentry;
    struct hash_table_datum *new_entry;
    struct hash_table_datum *old_entry;
    struct hash_table_datum *prev_entry = NULL;
    int size;
    int index;

    index = HASH(key);
    old_entry = pentry = ((struct hash_table_datum **)ptbl)[index];

    /* Find out if there is an old entry */
    while (old_entry != NULL && (key->size != old_entry->key.size || (memcmp(key->data, old_entry->key.data, key->size) != 0)))
    {
        prev_entry = old_entry;      /* Keep the previous entry, we need it when unlinking */
        old_entry = old_entry->next;
    }

    if (old_entry != NULL)
    {
        /* Unlink the old entry, and free memory */
        if (prev_entry != NULL)
        {
            prev_entry->next = old_entry->next;
        }
        else
        {
            /* Old entry was the first in the list */
            ((struct hash_table_datum **)ptbl)[index] = old_entry->next;
        }
        HASH_FREE(old_entry);
    }

    /* Allocate memory for a new entry */
    size = dat->size + key->size + sizeof(struct hash_table_datum);
    if ((new_entry = HASH_MALLOC(size)) == NULL)
    {
        return -1;
    }

    /* Fill in the new entry */
    memcpy(new_entry->data, dat->data, dat->size);
    memcpy(&new_entry->data[dat->size], key->data, key->size);
    new_entry->key.size = key->size;
    new_entry->key.data = &new_entry->data[dat->size];
    new_entry->data_length = dat->size;

    /* Link in the new entry */
    new_entry->next = ((struct hash_table_datum **)ptbl)[index];
    ((struct hash_table_datum **)ptbl)[index] = new_entry;

    return 0;
}

/*******************************************************************
 *
 * Description:     Delete a key in the hash table.
 *
 * Modified args:   None
 *
 * Return value:    0 if deleted, negative if not found.
 *
 *******************************************************************/
int
hash_delete(HASH_TABLE *ptbl, char *key)
{
    HASH_DATUM key_datum;
    struct hash_table_datum *entry;
    struct hash_table_datum *prev_entry = NULL;
    int index;

    key_datum.size = strlen(key) + 1;
    key_datum.data = key;
    
    index = HASH(&key_datum);
    entry = ((struct hash_table_datum **)ptbl)[index];

    while (entry != NULL && (key_datum.size != entry->key.size || (memcmp(key_datum.data, entry->key.data, key_datum.size) != 0)))
    {
        prev_entry = entry;      /* Keep the previous entry, we need it when unlinking */
        entry = entry->next;
    }

    if (entry != NULL)
    {
        /* Unlink the old entry, and free memory */
        if (prev_entry != NULL)
        {
            prev_entry->next = entry->next;
        }
        else
        {
            /* Old entry was the first in the list */
            ((struct hash_table_datum **)ptbl)[index] = entry->next;
        }
        HASH_FREE(entry);

        return 0;
    }
    
    return -1;  /* Not found */
}

/*******************************************************************
 *
 * Description:     Delete a key in the hash table.
 *
 * Modified args:   None
 *
 * Return value:    0 if deleted, negative if not found.
 *
 *******************************************************************/
int
hash_delete_key(HASH_TABLE *ptbl, HASH_DATUM *key)
{
    struct hash_table_datum *entry;
    struct hash_table_datum *prev_entry = NULL;
    int index;

    index = HASH(key);
    entry = ((struct hash_table_datum **)ptbl)[index];

    while (entry != NULL && (key->size != entry->key.size || (memcmp(key->data, entry->key.data, key->size) != 0)))
    {
        prev_entry = entry;      /* Keep the previous entry, we need it when unlinking */
        entry = entry->next;
    }

    if (entry != NULL)
    {
        /* Unlink the old entry, and free memory */
        if (prev_entry != NULL)
        {
            prev_entry->next = entry->next;
        }
        else
        {
            /* Old entry was the first in the list */
            ((struct hash_table_datum **)ptbl)[index] = entry->next;
        }
        HASH_FREE(entry);

        return 0;
    }

    return -1;  /* Not found */
}

/*******************************************************************
 *
 * Description:     Get a unsigned value copy from the table.
 *
 * Modified args:   None
 *
 * Return value:    0 if key exists, negative if not.
 *
 *******************************************************************/
int
hash_get_ul(HASH_TABLE *ptbl, char *key, unsigned long *pdata)
{
    struct hash_table_datum *entry;
    HASH_DATUM key_datum;
    
    key_datum.size = strlen(key) + 1;
    key_datum.data = key;
    if ((entry = hash_lookup(ptbl, &key_datum)) == NULL)
    {
        return -1;
    }

    *pdata = *((unsigned long *)entry->data);
    
    return 0;
}

/*******************************************************************
 *
 * Description:     Insert an unsigned value into the table.
 *
 * Modified args:   None
 *
 * Return value:    0 if ok, negative if not.
 *
 *******************************************************************/
int
hash_insert_ul(HASH_TABLE *ptbl, char *key, unsigned long data)
{
    HASH_DATUM key_datum;
    HASH_DATUM datum;

    key_datum.size = strlen(key) + 1;
    key_datum.data = key;
    datum.size = sizeof(unsigned long);
    datum.data = &data;
    return hash_insert(ptbl, &key_datum, &datum);
    
    return 0;
}

/*******************************************************************
 *
 * Description:     Insert a string value into the table.
 *
 * Modified args:   None
 *
 * Return value:    0 if ok, negative if not.
 *
 *******************************************************************/
int
hash_insert_str(HASH_TABLE *ptbl, char *key, char *data)
{
    HASH_DATUM key_datum;
    HASH_DATUM datum;

    key_datum.size = strlen(key) + 1;
    key_datum.data = key;
    datum.size = strlen(data) + 1;
    datum.data = data;
    return hash_insert(ptbl, &key_datum, &datum);

    return 0;
}

/*******************************************************************
 *
 * Description:     Retrieve a copy of a string value
 *
 * Modified args:   None
 *
 * Return value:    0 if key exists, negative if not.
 *
 *******************************************************************/
int
hash_get_str(HASH_TABLE *ptbl, char *key, char *data)
{
    struct hash_table_datum *entry;
    HASH_DATUM key_datum;

    key_datum.size = strlen(key) + 1;
    key_datum.data = key;
    if ((entry = hash_lookup(ptbl, &key_datum)) == NULL)
    {
        return -1;
    }

    memcpy(data, entry->data, entry->data_length);

    return 0;
}

/*******************************************************************
 *
 * Description:     Check if a key exists.
 *
 * Modified args:   None
 *
 * Return value:    0 if not exists, non zero otherwise.
 *
 *******************************************************************/
int
hash_key_exists(HASH_TABLE *ptbl, char *key)
{
    HASH_DATUM key_datum;

    key_datum.size = strlen(key) + 1;
    key_datum.data = key;
    if (hash_lookup(ptbl, &key_datum) == NULL)
    {
        return 0;
    }
    
    return -1;
}

/*******************************************************************
 *
 * Description:     Insert a blob value into the table.
 *
 * Modified args:   None
 *
 * Return value:    0 if ok, negative if not.
 *
 *******************************************************************/
int
hash_insert_blob(HASH_TABLE *ptbl, char *key, void *vpdata, int size)
{
    HASH_DATUM key_datum;
    HASH_DATUM datum;

    key_datum.size = strlen(key) + 1;
    key_datum.data = key;
    datum.size = size;
    datum.data = vpdata;
    return hash_insert(ptbl, &key_datum, &datum);
}

/*******************************************************************
 *
 * Description:     Retrieve a copy of a value, size must match datasize.
 *
 * Modified args:   None
 *
 * Return value:    0 if key exists and sizes match, negative if not.
 *
 *******************************************************************/
int
hash_get_blob(HASH_TABLE *ptbl, char *key, void *vpdata, int size)
{
    struct hash_table_datum *entry;
    HASH_DATUM key_datum;

    key_datum.size = strlen(key) + 1;
    key_datum.data = key;
    if ((entry = hash_lookup(ptbl, &key_datum)) == NULL)
    {
        return -1;
    }

    if (size != entry->data_length)
    {
        return -1;
    }

    memcpy(vpdata, entry->data, entry->data_length);

    return 0;
}

/*******************************************************************
 *
 * Description:     Get first entry matching pattern.
 *
 * Modified args:   None
 *
 * Return value:    Pointer to data if found, otherwise null.
 *
 *******************************************************************/
struct hash_table_datum *
hash_get_first(HASH_TABLE *ptbl, char *pattern, hash_search_state_t *state)
{
    struct hash_table_datum *new_copy;

    for (state->bin = 0; state->bin < (int)HASH_TABLE_SIZE; state->bin++)
    {
        state->entry = ((struct hash_table_datum **)ptbl)[state->bin];
        while (state->entry != NULL)
        {
            if (match(pattern, (char *)(state->entry->key.data)))
            {
                if ((new_copy = HASH_MALLOC(sizeof(struct hash_table_datum) + state->entry->key.size + state->entry->data_length)) == NULL)
                {
                    return NULL;
                }
                
                new_copy->next = NULL;

                /* Copy data value */
                new_copy->data_length = state->entry->data_length;
                memcpy(new_copy->data, state->entry->data, state->entry->data_length);

                /* Copy key info */
                new_copy->key.size = state->entry->key.size;
                new_copy->key.data = &new_copy->data[state->entry->data_length];
                memcpy(&new_copy->data[state->entry->data_length], state->entry->key.data, state->entry->key.size);

                /* Advance search position */
                state->entry = state->entry->next;
                
                return new_copy;
            }
            state->entry = state->entry->next;
        }
    }

    return NULL;
}

/*******************************************************************
 *
 * Description:     Get next entry matching pattern.
 *
 * Modified args:   None
 *
 * Return value:    Pointer to data if found, otherwise null.
 *
 *******************************************************************/
struct hash_table_datum *
hash_get_next(HASH_TABLE *ptbl, char *pattern, hash_search_state_t *state)
{
    struct hash_table_datum *new_copy;

    while (state->bin < (int)HASH_TABLE_SIZE)
    {
        while (state->entry != NULL)
        {
            if (match(pattern, (char *)(state->entry->key.data)))
            {
                if ((new_copy = HASH_MALLOC(sizeof(struct hash_table_datum) + state->entry->key.size + state->entry->data_length)) == NULL)
                {
                    return NULL;
                }

                new_copy->next = NULL;

                /* Copy data value */
                new_copy->data_length = state->entry->data_length;
                memcpy(new_copy->data, state->entry->data, state->entry->data_length);

                /* Copy key info */
                new_copy->key.size = state->entry->key.size;
                new_copy->key.data = &new_copy->data[state->entry->data_length];
                memcpy(&new_copy->data[state->entry->data_length], state->entry->key.data, state->entry->key.size);

                state->entry = state->entry->next;
                return new_copy;
            }
            state->entry = state->entry->next;
        }
        state->bin++;
        if (state->bin < (int)HASH_TABLE_SIZE)
        {
            state->entry = ((struct hash_table_datum **)ptbl)[state->bin];
        }
    }

    return NULL;
}
