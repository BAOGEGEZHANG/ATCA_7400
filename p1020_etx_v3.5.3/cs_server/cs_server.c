/*
 *                  		Copyright QiCe Tech, 2007-2010
 *
 *                          ALL RIGHTS RESERVED
 *
 * Description:     cs_server.c 
 *
 */
 
/*******************************************************************
 *
 * Description:     Includes
 *
 *******************************************************************/ 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <pty.h>
#include <errno.h>
#include <ctype.h>
#include <netinet/in.h>
#include <string.h>

#include "utils/errprint.h"
#include "cs_server.h"
#include "cs_client/cs_client.h"


/*******************************************************************
 *
 * Description:     Constant definitions
 *
 *******************************************************************/
#define CS_SRV_LISTEN_Q_SIZE         5
#define CS_SRV_MON_TASK_PRIO         50
#define CS_SRV_MON_TASK_STACK        8192
#define CS_SRV_CLIENT_TASK_PRIO      50
#define CS_SRV_CLIENT_TASK_STACK     8192

#define MAX_TTY_BUF_SIZE             2048*64
#define BCM_SHELL_PATH               "/usr/local/sbin/sdk"

/*******************************************************************
 *
 * Description:     Macro definitions
 *
 *******************************************************************/
#define DEBUG(lvl, msg...)  if (csdebug >= (lvl)) { printf(msg); }
#define DO_DEBUG(lvl, blk)  if (csdebug >= (lvl)) blk


/*******************************************************************
 *
 * Description:     Function prototypes
 *
 *******************************************************************/
static int csipc_send_response(int clifd, struct csipcrec *req);
static int csipc_read_request(int clifd, struct csipcrec *req);
static int csipc_set(int clifd, struct csipcrec *req);
static int csipc_close(int clifd, struct csipcrec *req);
static void *console_server(void *vp);

static int start_bcm_app();

/*******************************************************************
 *
 * Description:     Static (global) variables
 *
 *******************************************************************/
static int csdebug = 0;

static int cs_run = 1;
static int tty_run = 1;
static int ttyfd, appfd;

static pthread_mutex_t g_cs_mutex;
static pthread_mutex_t g_tty_mutex;

static char ttybuf[MAX_TTY_BUF_SIZE];
static int ttyCount = 0;

/*******************************************************************
 *
 * Description:     Description....
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
static void
start_console_srv()
{
    pthread_attr_t tattr;
    pthread_t pid;
    
    int listenfd;
    int connfd;
    struct sockaddr_in srvaddr;
    struct sockaddr_in cliaddr;
    
    unsigned int len;
    
    
    DEBUG(1, "cs server process monitor started.\n");
    
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        errprint("Can't create server socket.\n");
        return;
    }
    
    memset(&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    srvaddr.sin_port = htons(CS_SRV_PORT);
    
    if (bind(listenfd, (struct sockaddr *) &srvaddr, sizeof(srvaddr)) < 0)
    {
        errprint("Can't bind to server socket.\n");
        return;
    }
    
    if (listen(listenfd, CS_SRV_LISTEN_Q_SIZE) < 0)
    {
        errprint("Can't listen to server socket.\n");
        return;
    }
    DEBUG(1, "db server listening on port %d.\n", ntohs(srvaddr.sin_port));
    
    while (cs_run)
    {        
        len = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len)) < 0)
        {
            errprint("Can't accept on server socket.\n");
            return;
        }
        
        if (!cs_run)
        {
            break;
        }
        
        DEBUG(1, "Creating new client process for handle %d.\n", connfd);
        
        pthread_attr_init(&tattr);
        pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);
        pthread_attr_setscope(&tattr, PTHREAD_SCOPE_SYSTEM);
        pthread_attr_setschedpolicy(&tattr, SCHED_OTHER);
        
        if (pthread_create(&pid, &tattr, console_server, (void *)connfd) != 0)
        {
            errprint("Can't create db server process.\n");
            close(connfd);
        }
    }
    
    close(listenfd);
    
    return;
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
console_server(void *vp)
{
    int clifd;
    struct csipcrec *req;
    void *buf;
    
    clifd = (int)vp;
    
    DEBUG(1, "new cs client process started for handle %d.\n", clifd);
    
    if ((buf = malloc(CS_SRV_BUF_SIZE)) == NULL)
    {
        errprint("Can't allocate request buffer.\n");
        close(clifd);
        return NULL;
    }
    
    req = (struct csipcrec *) buf;
    
    while (1)
    {
        if (csipc_read_request(clifd, req) < 0)
        {
            errprint("Error reading request.\n");
            free(buf);
            close(clifd);
            break;
        }
        
        switch(req->cmd)
        {
            case CSIPC_CLOSE:
                csipc_close(clifd, req);
		if (buf == NULL)
		    errprint("buf is NULL!!!\n");
		            else
                    free(buf);
                return NULL;
                
            case CSIPC_SET:
                csipc_set(clifd, req);
                break;
                
            case CSIPC_SHUTDOWN:
                if (buf == NULL)
		                errprint("buf is NULL!!!\n");
		            else
                    free(buf);
                close(clifd);
                cs_run = 0;
                return NULL;
                
            default:
                errprint("Got unknown cmd.\n");
                break;        
        }
    }
    
    return NULL;
}

static int
csipc_set(int clifd, struct csipcrec *req)
{
    int config_len, index;
    int timeout=2;
    
    // first try to get the mutex
    if (pthread_mutex_lock(&g_cs_mutex) != 0)
    {
        errprint("failed to lock cs mutex.\n");
        return -1;
    }
    
    //last '\0' is not send to us, so add it mannually
    req->data[req->data_size] = '\0';
    config_len = req->data_size;
    
    // send it to ttyfd, not include last '0', but include '\n'
    DEBUG(2, "csipc_set, req->data_size is: %d, req->data is: %s\n", req->data_size, req->data);
    write(ttyfd, req->data, req->data_size); 
    fsync(ttyfd);
    
    sleep(2); // wait for 2s to get all the response
#if defined(BUILD_QT10) || defined(BUILD_QT20)
    // do nothing
#else
    if ((strncmp(req->data, "ps", 2) == 0) || (strncmp(req->data, "egress show", 11) == 0))
    {
        // ps/egress show will cost much more time
        while ((ttyCount < 1024) && timeout)
	{
            sleep(2);
            timeout--;
        }
    }
#endif
    
    {
        index = ttyCount;
        DEBUG(2,"before update ttyCount is: %d\n", ttyCount);      
        
        // first try to get the mutex
        if (pthread_mutex_lock(&g_tty_mutex) != 0)
        {
            errprint("failed to lock cs mutex.\n");
            return -1;
        }
        
        // update counter
        ttyCount = 0;
        
        // release
        if (pthread_mutex_unlock(&g_tty_mutex) != 0)
        {
            errprint("failed to unlock cs mutex\n");
            return -1;
        }
        
        // send out the response 
        {
            if (index > (config_len + 2))
            {
                memcpy(req->data, &ttybuf[config_len + 2], (index-config_len - 2));
                req->data_size = index-config_len - 2;
            }
            else
            {
                req->data_size = 0;
            }
        }
        req->data[index] = '\0';
        req->result = 1;
        
        csipc_send_response(clifd, req);
    }
    
    // release the mutex
    if (pthread_mutex_unlock(&g_cs_mutex) != 0)
    {
        errprint("failed to unlock cs mutex\n");
        return -1;
    }
    
    DEBUG(2, "csipc_set, release the mutex and return...\n");
    
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
csipc_send_response(int clifd, struct csipcrec *req)
{
    int bytes_written;
    int bytes_left;
    unsigned char *wrbuf;
    
    
    bytes_left = sizeof(struct csipcrec) + req->data_size;
    
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
csipc_read_request(int clifd, struct csipcrec *req)
{
    int bytes_read;
    int bytes_left;
    unsigned char *rdbuf;
    
    
    /* Read request struct first */
    bytes_left = sizeof(struct csipcrec);
    rdbuf = (unsigned char *)req;
    do
    {
        // if there's no data, read will block here, good
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
    
    /* Read rest of request data */
    bytes_left = req->data_size;
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
 * Description:     close a client connection to the dbsrv.
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
static int
csipc_close(int clifd, struct csipcrec *req)
{
    req->result = 0;
    
    csipc_send_response(clifd, req);
    
    close(clifd);
    
    return 0;
}



/*******************************************************************
 *
 * Description:     tty2net.
 *
 * Modified args:   None
 *
 * Return value:    0 if sucessful negative if error.
 *
 *******************************************************************/
static void *
tty2net()
{
    unsigned char data;
    unsigned int  tmpCnt;
    unsigned char tmpbuf[1024];
    
    memset(ttybuf, 0, MAX_TTY_BUF_SIZE);
    
    while(tty_run)
    {
        tmpCnt = 0;
        memset(tmpbuf, 0, 1024);
        do 
        {
	    if(read(ttyfd, &data, 1) != 1)
            {
            	close(ttyfd);
            	break;
	    }
            tmpbuf[tmpCnt++] = data;
        } while (data != '\n');
        
        // new line enter
        {
            // add the data to global buf
            if (ttyCount + tmpCnt < MAX_TTY_BUF_SIZE)
            {
                // update data in the ttybuf
                //ttybuf[ttyCount] = data;
                memcpy(ttybuf+ttyCount, tmpbuf, tmpCnt);
                
                // first try to get the mutex
                if (pthread_mutex_lock(&g_tty_mutex) != 0)
                {
                    errprint("failed to lock tty mutex.\n");
                    return NULL;
                }
                
                // update counter                
                //ttyCount++;
                ttyCount += tmpCnt;
                
                // release the mutex
                if (pthread_mutex_unlock(&g_tty_mutex) != 0)
                {
                    errprint("failed to unlock tty mutex\n");
                    return NULL;
                }
            }
            else
            {
                errprint("cs_server sdk response dropped cause ttyBuf overflow!\n");
            }
        }
    }
    
    return NULL;
}


/*******************************************************************
 *
 * Description:     Fork of a child
 *
 * Modified args:   None.
 *
 * Return value:    Returns 0 on success, or an error code on failure.
 *
 *******************************************************************/
static int
start_bcm_app()
{
    pid_t pid;
    char *exec_argv[2];
    
    if ((pid = fork()) < 0)
    {
        errprint("start_app fork error!\n");
        return -1;
    }
    else if (pid > 0)
    {
        /* parent */
        return pid;
    }
    
    chdir("/usr/local/bin");
    
    /* child */
    dup2(appfd, 0);
    dup2(appfd, 1);
    dup2(appfd, 2);
    
    exec_argv[0] = BCM_SHELL_PATH;
    exec_argv[1] = NULL;
    
    execv(BCM_SHELL_PATH, exec_argv);
    
    sleep(1);
    
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
    int pid;
    pthread_mutexattr_t mattr;
    pthread_t ttyid, csid;
    
    /* step 1. Demo us */
    pid = fork();
    if (pid < 0)
    {
        errprint("fork error!\n");
        exit(0);
    } 
    else if (pid > 0)
    {
        /* parent */
        exit(0);
    }
    
    setsid();
    signal(SIGPIPE, SIG_IGN);
    
    /* Get a pseudo tty */
    if (openpty(&ttyfd, &appfd, NULL, NULL, NULL) < 0)
    {
        errprint("open pesedu tty failed!\n");
        exit(0);
    }
    
    /* attach bcm.user to appfd */    
    pid = start_bcm_app();
    
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&g_cs_mutex, &mattr);
    pthread_mutex_init(&g_tty_mutex, &mattr);
    
    if(pthread_create(&ttyid, NULL, tty2net, (void *)ttyfd) < 0)
    {
        errprint("pthread_create failed!\n");
        exit(0);
    }
    
    if(pthread_create(&csid, NULL, start_console_srv, NULL) < 0)
    {
        errprint("pthread_create failed!\n");
        exit(0);
    }
    
    /* wait for sdk to exit */
    waitpid(pid, NULL, 0);
    
    tty_run = 0;
    
    cs_run = 0;
    
    /* destroy mutexattr */
    pthread_mutexattr_destroy(&mattr);
    
    /* destroy mutex */
    pthread_mutex_destroy(&g_cs_mutex);
    pthread_mutex_destroy(&g_tty_mutex);
    
    return 0;
}

