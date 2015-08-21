/*
 *                  		Copyright QiCe Tech, 2007-2010
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:      
 *
 */


#ifndef _WATCH_LIST_H_
#define _WATCH_LIST_H_


#define WL_PTR(wle)             (*(wle))
#define WL_PTR_OF(wle,type)     ((type *)(*(wle)))

struct watch_list {
    int size;
    int free;
    void *list[0];
};

struct watch_list *wl_init(int size);
void wl_destroy(struct watch_list *wl);
void **wl_add_entry(struct watch_list *wl, void *entry);
void wl_free_entry(void **entry);
void wl_cleanup(struct watch_list *wl);


#endif
