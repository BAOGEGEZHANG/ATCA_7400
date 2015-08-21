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
#include <stdarg.h>
#include <errno.h>

#include "errprint.h"


/*******************************************************************
 *
 * Description:     Local preprocessor macros
 *
 *******************************************************************/

/* Use this macro since va_copy() is only present in C99 */
#define IMPLEMENT_ERRPRINT                                              \
    if (!(severity & errp_data.severity_mask))                          \
    {                                                                   \
        return 0;                                                       \
    }                                                                   \
    va_start(ap, format);                                               \
    if (errp_data.fp_out != NULL)                                       \
    {                                                                   \
        retval = fprintf(errp_data.fp_out, "%s: ",                      \
                         errp_severity_str(severity));                  \
        retval += vfprintf(errp_data.fp_out, format, ap);               \
    }                                                                   \
    else                                                                \
    {                                                                   \
        retval = printf("%s: ", errp_severity_str(severity));           \
        retval += vprintf(format, ap);                                  \
    }                                                                   \
    va_end(ap)

/*******************************************************************
 *
 * Description:     File local variables
 *
 *******************************************************************/

struct err_print_data {
    FILE *fp_out;
    char *id;
    unsigned int severity_mask;
};

static struct err_print_data errp_data = { 
    NULL, EP_DEFIDSTR, EP_DEF_SEVERITYMASK
};

/*******************************************************************
 *
 * Description:     File local functions
 *
 *******************************************************************/

char *
errp_severity_str(errp_severity severity)
{
    switch(severity) {
    case ERRP_CRITICAL:
            return "CR";
    case ERRP_ERROR:
            return "ER";
    case ERRP_WARN:
            return "WA";
    case ERRP_NOTICE:
            return "NO";
    case ERRP_INFO:
            return "IN";
    case ERRP_DEBUG:
            return "DE";
    default:
            return "UN";
    }
}

/*******************************************************************
 *
 * Description:     Error print initialisation (optional)
 *
 * Modified args:   None
 *
 * Return value:    Returns 0 on success, or an error code on failure.
 *
 *******************************************************************/
int
err_print_init(unsigned int severity_mask, FILE *fp_out, char *id)
{
    errp_data.fp_out = fp_out;
    if (id != NULL)
    {
        if (errp_data.id != NULL)
        {
            free(errp_data.id);
        }
        errp_data.id = strdup(id);
    }
    errp_data.severity_mask = severity_mask;

    return 0;
}


/*******************************************************************
 *
 * Description:     Get mask for printed message types
 *
 * Modified args:   None
 *
 * Return value:    Returns 0 on success, or an error code on failure.
 *
 *******************************************************************/
unsigned int
err_print_get_mask(void)
{
    return errp_data.severity_mask;
}

/*******************************************************************
 *
 * Description:     Adds to mask for printed message types
 *
 * Modified args:   None
 *
 * Return value:    Returns 0 on success, or an error code on failure.
 *
 *******************************************************************/
int
err_print_add_mask(unsigned int severity_mask)
{
    errp_data.severity_mask |= severity_mask;
    return 0;
}

/*******************************************************************
 *
 * Description:     Removes from mask for printed message types
 *
 * Modified args:   None
 *
 * Return value:    Returns 0 on success, or an error code on failure.
 *
 *******************************************************************/
int
err_print_del_mask(unsigned int severity_mask)
{
    errp_data.severity_mask &= ~severity_mask;
    return 0;
}

/*******************************************************************
 *
 * Description:     Set mask for printed message types
 *
 * Modified args:   None
 *
 * Return value:    Returns 0 on success, or an error code on failure.
 *
 *******************************************************************/
int
err_print_set_mask(unsigned int severity_mask)
{
    errp_data.severity_mask = severity_mask;
    return 0;
}

/*******************************************************************
 *
 * Description:     Print message as given by format string
 *
 * Modified args:   None
 *
 * Return value:    Returns number of characters printed on success or -1
 *
 *******************************************************************/
int
err_print(errp_severity severity, char *format, ...)
{
    va_list ap;
    int retval;

    IMPLEMENT_ERRPRINT;

    return retval;                                             
}

#ifdef ERRP_NO_VARIADIC_MACROS
/*******************************************************************
 *
 * Description:     Print error message as given by format string
 *
 * Modified args:   None
 *
 * Return value:    Returns number of characters printed on success or -1
 *
 *******************************************************************/
int
errprint(char *format, ...)
{
    errp_severity severity = ERRP_ERROR;
    va_list ap;
    int retval;

    IMPLEMENT_ERRPRINT;

    return retval;
}

/*******************************************************************
 *
 * Description:     Print warning message as given by format string
 *
 * Modified args:   None
 *
 * Return value:    Returns number of characters printed on success or -1
 *
 *******************************************************************/
int
warnprint(char *format, ...)
{
    errp_severity severity = ERRP_WARN;
    va_list ap;
    int retval;

    IMPLEMENT_ERRPRINT;

    return retval;
}

/*******************************************************************
 *
 * Description:     Print debug message as given by format string
 *
 * Modified args:   None
 *
 * Return value:    Returns number of characters printed on success or -1
 *
 *******************************************************************/
int
debugprint(char *format, ...)
{
    errp_severity severity = ERRP_DEBUG;
    va_list ap;
    int retval;

    IMPLEMENT_ERRPRINT;

    return retval;
}
#endif /* ERRP_NO_VARIADIC_MACROS */
