/*
 *                  		Copyright QiCe Tech, 2007-2010
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:      
 *
 */


#ifndef __ERRPRINT_H__
#define __ERRPRINT_H__

#include <stdio.h>              /* Needed for FILE definition */
/*******************************************************************
 *
 * Description:     Constant definitions
 *
 *******************************************************************/

#define EP_DEFIDSTR NULL
#define EP_DEF_SEVERITYMASK ((ERRP_CRITICAL)|(ERRP_ERROR)|(ERRP_WARN))

#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif

/*******************************************************************
 *
 * Description:     Data type declarations
 *
 *******************************************************************/

typedef enum {
    ERRP_CRITICAL = 1,
    ERRP_ERROR = 2,
    ERRP_WARN = 4,
    ERRP_NOTICE = 8,
    ERRP_INFO = 16,
    ERRP_DEBUG = 32
} errp_severity;

#ifndef bool_t
typedef unsigned char bool_t;
#endif /* bool_t */

/*******************************************************************
 *
 * Description:     Function prototypes
 *
 *******************************************************************/

int err_print_init(unsigned int severity_mask, FILE *fp_out, char *id);
int err_print_set_mask(unsigned int severity_mask);
int err_print_add_mask(unsigned int severity_mask);
int err_print_del_mask(unsigned int severity_mask);
unsigned int err_print_get_mask(void);
int err_print(errp_severity severity, char *format, ...);

/*******************************************************************
 *
 * Description:     Utility macros, where supported
 *
 *******************************************************************/

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    /* C99 defines variadic macros which are very useful */
#  define ERR_PRINT_STDC_VER __STDC_VERSION__
#else
#  if defined(__STDC_ISO_10646__) && __STDC_ISO_10646__ >= 199901L
     /* ISO 10646 defines variadic macros just as C99, very useful */
#    define ERR_PRINT_STDC_VER __STDC_ISO_10646__
#  endif /* defined(__STDC_ISO_10646__) */
#endif /* defined(__STDC_VERSION__) */

#if defined(ERR_PRINT_STDC_VER) && ERR_PRINT_STDC_VER >= 199901L && !defined(__STRICT_ANSI__)
/* C99 defines variadic macros which are very useful */
#  define errprint(...) err_print(ERRP_ERROR, __VA_ARGS__)
#  define warnprint(...) err_print(ERRP_WARN, __VA_ARGS__)
#  ifdef DEBUGPRINT_ENABLE
#    define debugprint(...) err_print(ERRP_DEBUG, __VA_ARGS__)
#  else /* DEBUGPRINT_ENABLE */
#    define debugprint(...) do { ; } while (0)
#  endif /* DEBUGPRINT_ENABLE */
#else
/* Older versions of gcc does not provide C99, but supports variadic macros */
#  if defined(__GNUC__) && __GNUC__ > 2 || (__GNUC__ >= 2 && __GNUC_MINOR__ >= 7)
#    define errprint(format...) err_print(ERRP_ERROR, format)
#    define warnprint(format...) err_print(ERRP_WARN, format)
#    ifdef DEBUGPRINT_ENABLE
#      define debugprint(format...) err_print(ERRP_DEBUG, format)
#    else /* DEBUGPRINT_ENABLE */
#      define debugprint(format...) do { ; } while (0)
#    endif /* DEBUGPRINT_ENABLE */
#  else /* defined(__GNUC__) */
/* Not gcc or ancient gcc, use variadic functions as last resort */
#    define ERRP_NO_VARIADIC_MACROS  "Bummer!"
     extern int errprint(char *format, ...);
     extern int warnprint(char *format, ...);
#    ifdef DEBUGPRINT_ENABLE
       extern int debugprint(char *format, ...);
#    else /* DEBUGPRINT_ENABLE */
#      define debugprint(format...) do { ; } while (0)
#    endif /* DEBUGPRINT_ENABLE */
#  endif /* defined(__GNUC__) */
#endif /* defined(ERR_PRINT_STDC_VER)... */

#endif /* __ERRPRINT_H__ */
