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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "watch_list.h"

/*******************************************************************
 *
 * Description:     Allocate and initialize a new watch list.
 *
 * Modified args:   None
 *
 * Return value:    Ptr to watch list if success NULL if error.
 *
 *******************************************************************/
struct watch_list *
wl_init(int size)
{
    struct watch_list *wl;

    
    wl = (struct watch_list *)malloc(sizeof(struct watch_list) + size * sizeof(void *));
    if (wl != NULL)
    {
        memset(wl, 0, sizeof(struct watch_list) + size * sizeof(void *));
        wl->size = size;
        wl->free = 0;
    }
    
    return wl;
}

/*******************************************************************
 *
 * Description:     Free a watch list.
 *
 * Modified args:   None
 *
 * Return value:    None
 *
 *******************************************************************/
void
wl_destroy(struct watch_list *wl)
{
    wl_cleanup(wl);
    free(wl);
}

/*******************************************************************
 *
 * Description:     Add an entry to the watch list.
 *
 * Modified args:   wl
 *
 * Return value:    Pointer to watch list entry if success NULL if
 *                  error.
 *
 *******************************************************************/
void **
wl_add_entry(struct watch_list *wl, void *entry)
{
    int i;
    void **vp;

    /* Check if there is any free entry */
    if (wl->free >= wl->size)
    {
        /* Nope, search from start of list if any entry is free */
        for (i = 0; i < wl->size; i++)
        {
            if (wl->list[i] == NULL)
            {
                wl->free = i;
            }
        }
    }

    /* If still no entry is free bail out */
    if (wl->free >= wl->size)
    {
        return NULL;
    }

    /* Insert entry */
    wl->list[wl->free] = entry;
    vp = &(wl->list[wl->free++]);

    /* Find next free entry in list */
    while ((wl->free < wl->size) && (wl->list[wl->free] != NULL))
    {
        wl->free++;
    }

    return vp;

}

/*******************************************************************
 *
 * Description:     Free a watch list entry.
 *
 * Modified args:   *entry
 *
 * Return value:    None
 *
 *******************************************************************/
void
wl_free_entry(void **entry)
{
    free(*entry);
    *entry = NULL;
}

/*******************************************************************
 *
 * Description:     Free all entries in watch list.
 *
 * Modified args:   wl
 *
 * Return value:    None
 *
 *******************************************************************/
void
wl_cleanup(struct watch_list *wl)
{
    int i;

    for (i = 0; i < wl->size; i++)
    {
        if (wl->list[i] != NULL)
        {
            free(wl->list[i]);
            wl->list[i] = NULL;
        }
    }
    wl->free = 0;
}
