/*
 *                          Copyright QiCe Tech, 2010-2012
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
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <linux/version.h>
#include <pthread.h>
#include <errno.h>

#include "lexer.h"
#include "parserops.h"
#include "database.h"
#include "utils/errprint.h"
#include "drvops.h"



/*******************************************************************
 *
 * Description:     Function prototypes
 *
 *******************************************************************/
static int save_filter_config(struct parser_params *ppar, FILE *fp);
static int save_forward_config(struct parser_params *ppar, FILE *fp);
static int save_global_config(struct parser_params *ppar, FILE *fp);

static int reset_default_config();
static int reset_filter_default_config();
static int reset_forward_default_config();
static int reset_global_default_config();


/*******************************************************************
 *
 * Description:     System Functions
 *
 *******************************************************************/
void
cli_db_lock(struct parser_params *ppar)
{
   	if (ppar->db_lock++ == 0)
   	{
        DB_CL_LOCK(&ppar->db);
   	}
}

void
cli_db_unlock(struct parser_params *ppar)
{
    if (--ppar->db_lock == 0)
    {
        DB_CL_UNLOCK(&ppar->db);
    }
}

int
cli_init_config(struct parser_params *ppar)
{
    return cli_reset_default_config(ppar);
}

int
cli_reset_default_config(struct parser_params *ppar)
{
    cli_db_lock(ppar);
	
    if (reset_default_config() < 0)
    {
        errprint("Can't reset default configuration in driver.\n");
        cli_db_unlock(ppar);
        return -1;
    }
	
    if (db_reset_default_config(&ppar->db) < 0)
    {
        errprint("Can't reset default configuration in database.\n");
        cli_db_unlock(ppar);
        return -1;
    }
	
    cli_db_unlock(ppar);
    
    return 0;
}

int
cli_show_config(struct parser_params *ppar)
{
    printf(PROMPT1("current configuration:\n"));
    
    return cli_save_config(ppar, stdout);
}

int
cli_show_config_file()
{
    DIR *cfg_dir;
    struct dirent *dir_enty;
    char s[5];
    int    len,fileno;
    
    fileno = 0;
    cfg_dir = opendir("/usr/local/etc");
    printf("\n");
    for (dir_enty = readdir(cfg_dir); NULL != dir_enty; dir_enty = readdir(cfg_dir))
    {
        len = strlen(dir_enty->d_name);
        if (len > 5)
        {
            s[0] = dir_enty->d_name[len-5];
        	s[1] = dir_enty->d_name[len-4];
        	s[2] = dir_enty->d_name[len-3];
        	s[3] = dir_enty->d_name[len-2];
        	s[4] = dir_enty->d_name[len-1];
        	if (strncmp(s,".conf",5)==0)
            {
                fileno++;
          		printf("     %d.  %s\n", fileno, dir_enty->d_name);
            }
        }
    }
    printf("\n");
    
    return 0;
}

int
cli_save_config_file(struct parser_params *ppar, char *name)
{
    int res;
    FILE *fp;
    
    if ((fp = fopen(name, "w")) == NULL)
    {
        errprint("Can't open file %s for writing.\n", name);
      	return -1;
    }
    
    res = cli_save_config(ppar, fp);
    fclose(fp);
    
    return res;
}

int
cli_save_config(struct parser_params *ppar, FILE *fp)
{
    cli_db_lock(ppar);
    
    if (save_filter_config(ppar, fp) < 0)
    {
        errprint("Can't save filter configuration.\n");
        cli_db_unlock(ppar);
        return -1;
    }
	
    if (save_forward_config(ppar, fp) < 0)
    {
        errprint("Can't save forward configuration.\n");
        cli_db_unlock(ppar);
        return -1;
    }

    if (save_global_config(ppar, fp) < 0)
    {
        errprint("Can't save global configuration.\n");
        cli_db_unlock(ppar);
        return -1;
    }
    	 
    cli_db_unlock(ppar);
	
    return 0;
}

// internal static functions
static int
save_filter_config(struct parser_params *ppar, FILE *fp)
{
    unsigned int    i;
    unsigned long   oc_no;
    unsigned char   word_fs, word_ls;
    unsigned long   filter_num;
    unsigned long   id;
    unsigned long   id_matched;
    rule1options_t  rule1opts;
    rule2options_t  rule2opts;
    
    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no++)
    {
        /* deal with ruleset1 */    
        if (db_get_filter1_rule_num(&ppar->db, oc_no, &filter_num) < 0)
        {
            errprint("Can't get classify number in database.\n");
            return -1;
        }   
        
        for (id_matched = 0, id = 0; id_matched < filter_num; id++)
        {
            if (db_filter1_rule_exist(&ppar->db, oc_no, id))
            {
                id_matched++;
                    
                rule1opts.id = id;
                if (db_get_filter1_rule(&ppar->db, oc_no, (rule1options_t *)&rule1opts) < 0)
                {
                    errprint("Can't get classify rules from database.\n");
                    return -1;          
                }
                fprintf(fp, PROMPT1("lif set classify oc %d ruleset1 %d "), (unsigned int)oc_no, (unsigned int)rule1opts.id);
                    
                if (rule1opts.flag & RULE_OPT_PROTOCOL)
                {
                    fprintf(fp, "protocol %d ", rule1opts.protocol);
                }
                
                if (rule1opts.flag & RULE_OPT_SRCIP)
                {
                    fprintf(fp, "source ip %d.%d.%d.%d", (int)((rule1opts.sourceip>>24)&0xff),(int)((rule1opts.sourceip>>16)&0xff),
                                (int)((rule1opts.sourceip>>8)&0xff), (int)(rule1opts.sourceip&0xff));
                    fprintf(fp, " netmask %d.%d.%d.%d ", (int)((rule1opts.sourceip_mask>>24)&0xff), (int)((rule1opts.sourceip_mask>>16)&0xff), (int)((rule1opts.sourceip_mask>>8)&0xff), (int)(rule1opts.sourceip_mask&0xff));
                }
                
                if (rule1opts.flag & RULE_OPT_DESTIP)
                {
                    fprintf(fp, "dest ip %d.%d.%d.%d", (int)((rule1opts.destip>>24)&0xff),(int)((rule1opts.destip>>16)&0xff),
                                (int)((rule1opts.destip>>8)&0xff), (int)(rule1opts.destip&0xff));
                    fprintf(fp, " netmask %d.%d.%d.%d ", (int)((rule1opts.destip_mask>>24)&0xff), (int)((rule1opts.destip_mask>>16)&0xff),
                                (int)((rule1opts.destip_mask>>8)&0xff), (int)(rule1opts.destip_mask&0xff));
                }
                
                if (rule1opts.flag & RULE_OPT_SRCL4PORT)
                    fprintf(fp, "source l4_port %d ", rule1opts.sourcel4port);
                
                if (rule1opts.flag & RULE_OPT_DESTL4PORT)
                    fprintf(fp, "dest l4_port %d ", rule1opts.destl4port); 
                
          		if (rule1opts.flag & RULE_OPT_PACKETLEN)
            		fprintf(fp, "packet length range %d", rule1opts.pktlen_range);

                // no flag is set, then must be any
                if (rule1opts.flag == 0)
                    fprintf(fp, "any");					
				
          		fprintf(fp, "\n");
         	} // if
       	} // for (id_matched
        
		/* deal with ruleset2 */
      	if (db_get_filter2_rule_num(&ppar->db, oc_no, &filter_num) < 0)
        {
            errprint("Can't get classify number in database.\n");
            return -1;
        }
           
        for (id_matched = 0, id = 0; id_matched < filter_num; id++)
        {
            if (db_filter2_rule_exist(&ppar->db, oc_no, id))
            {
                id_matched++;
                    
                rule2opts.id = id;
                if (db_get_filter2_rule(&ppar->db, oc_no, (rule2options_t *)&rule2opts) < 0)
                {
                    errprint("Can't get classify rules from database.\n");
                    return -1;
                }
                fprintf(fp, PROMPT1("lif set classify oc %d ruleset2 %d "), (unsigned int)oc_no, (unsigned int)rule2opts.id);
                    
                if (rule2opts.flag & RULE_OPT_OFFSET)
                {
                    if (rule2opts.offset)
                        fprintf(fp, "offset %d ", rule2opts.offset);
                }
                
                if (rule2opts.flag & RULE_OPT_KEY)
                {
                    fprintf(fp, "key \"");
                    // need to do something;
                    for(i=0; i< MAX_KEY_NO; i++)
                    {
              			/* end of key */
                        if ((rule2opts.key[i] == 0x0) && (rule2opts.mask[i] == 0xff))
                            break;
                            
                        word_fs = (rule2opts.key[i] & 0xf0) >> 4;
                        if (word_fs < 0xa)
                            word_fs += 0x30;
                        else
                            word_fs += 0x37;
                        fprintf(fp, "%c", word_fs);
                        
                        word_ls = (rule2opts.key[i] & 0xf);
                        if (word_ls < 0xa)
                            word_ls += 0x30;
                        else
                            word_ls += 0x37;
                        fprintf(fp, "%c", word_ls);                 
                    }
            		fprintf(fp, "\"");
                }
                    
                if (rule2opts.flag == 0)
                    fprintf(fp, "any ");
                    
                fprintf(fp, "\n");      
            }
        }
    }

	
    return 0;
}

static int
save_forward_config(struct parser_params * ppar, FILE * fp)
{
    int i, j;
  	unsigned long oc_no;
    int ruleset2_all;
  	unsigned long fwd_old, fwd_new, mode;
  	unsigned int isvlan, issample, isdrop;
  	unsigned int vlanid;
       
       
  	for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no++)
    	for (i = 0; i < MAX_RULE_NO; i++)
    	{
        	ruleset2_all = '1';
        	//pick up the first one
            if(db_forward_mode_exist(&ppar->db, oc_no,(unsigned long)i, (unsigned long)0))
                db_get_forward_mode(&ppar->db, oc_no,(unsigned long)i, (unsigned long)0, &fwd_old);
            else
                fwd_old = FORWARD_DEF;
        	
        	for (j = 1; j < MAX_RULE_NO; j++)
        	{
                if(db_forward_mode_exist(&ppar->db, oc_no,(unsigned long)i, (unsigned long)j))
                    db_get_forward_mode(&ppar->db, oc_no,(unsigned long)i, (unsigned long)j, &fwd_new);
                else
                    fwd_new = FORWARD_DEF;
                
            	if (fwd_new != fwd_old)
            	{
                	ruleset2_all = '0';
                	break;
            	}
        	} // for (j = 1; 
      		
      		if (ruleset2_all == '1')
      		{
            	mode = (fwd_new >> 12) & 0xf;
            	vlanid = (fwd_new & 0xfff);
            	
              isvlan   = (mode >> 0) & 0x1;
              issample = (mode >> 1) & 0x1;
              isdrop   = (mode >> 2) & 0x1;
                
              if (isdrop)
              {
                  fprintf(fp, PROMPT1("lif set classify oc %d ruleset1 %d ruleset2 all "), (unsigned int)oc_no, i);
                	fprintf(fp, "forward mode drop\n");
              }
              else
              {
                  if (isvlan)
                  {
                      fprintf(fp, PROMPT1("lif set classify oc %d ruleset1 %d ruleset2 all "), (unsigned int)oc_no, i);
                    	fprintf(fp, "forward mode to vlan %d", vlanid);
                    	
                    	if (issample)
                        	fprintf(fp, " sample");
                    	
                    	fprintf(fp, "\n");
                  }
                  else
                  {
                      if (issample)
                      {
                          fprintf(fp, PROMPT1("lif set classify oc %d ruleset1 %d ruleset2 all "), (unsigned int)oc_no, i);
                        	fprintf(fp, "forward mode sample\n");
                      }
                  }
              }
          } // if (ruleset2_all == '1')
          else
      		{
            	for (j = 0; j < MAX_RULE_NO; j++)
            	{
                	if(db_forward_mode_exist(&ppar->db, oc_no,(unsigned long)i, (unsigned long)j))
                      db_get_forward_mode(&ppar->db, oc_no,(unsigned long)i, (unsigned long)j, &fwd_new);
                  else
                      fwd_new = FORWARD_DEF;
                    
                  mode = (fwd_new >> 12) & 0xf;
                	vlanid = fwd_new & 0xfff;
                	
                  isvlan   = (mode >> 0) & 0x1;
                  issample = (mode >> 1) & 0x1;
                  isdrop   = (mode >> 2) & 0x1;
                    
                  if (isdrop)
                  {
                      fprintf(fp, PROMPT1("lif set classify oc %d ruleset1 %d ruleset2 %d "), (unsigned int)oc_no, i, j);
                    	fprintf(fp, "forward mode drop\n");
                  }
                  else
                  {
                      if (isvlan)
                      {
                          fprintf(fp, PROMPT1("lif set classify oc %d ruleset1 %d ruleset2 %d "), (unsigned int)oc_no, i, j);
                        	fprintf(fp, "forward mode to vlan %d", vlanid);
                        	
                        	if (issample)
                            	fprintf(fp, " sample");
                        	
                        	fprintf(fp, "\n");
                      }
                      else
                      {
                          if (issample)
                          {
                              fprintf(fp, PROMPT1("lif set classify oc %d ruleset1 %d ruleset2 %d "), (unsigned int)oc_no, i, j);
                            	fprintf(fp, "forward mode sample\n");
                          }
                      }
                  }
              } // for (j = 0;
          } // else   
      }
    
    
    return 0;
}


static int
save_global_config(struct parser_params * ppar, FILE * fp)
{
    int range_no;
    unsigned long oc_no;
    unsigned long pktlen_min, pktlen_max;
    unsigned long enable_or_disable;	
    unsigned long sample_rate;
    unsigned long mms_size;
    unsigned long gtpdestports, gtphashmode;
    
    
    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no++)
    {
	if (db_get_flow_enable(&ppar->db, oc_no, &enable_or_disable) < 0)
    	{
        	errprint("Failed to get flow function in database.\n");
        	cli_db_unlock(ppar);
        	return -1;
    	}
	if (enable_or_disable != ENABLED)
		fprintf(fp, PROMPT1("lif set oc %d flow disable\n"), (unsigned int)oc_no);
		
    	if (db_get_flow_mms(&ppar->db, oc_no, &mms_size) < 0)
    	{
        	errprint("Failed to set flow function in database.\n");
        	cli_db_unlock(ppar);
        	return -1;
    	}
	if (mms_size != TCP_MMS)
		fprintf(fp, PROMPT1("lif set oc %d flow tcp mms %d\n"), (unsigned int)oc_no, (unsigned int)mms_size);
	    	
    	if (db_get_sample_rate(&ppar->db, oc_no, &sample_rate) < 0)
    	{
        	errprint("Failed to get sample rate in database.\n");
        	cli_db_unlock(ppar);
        	return -1;
    	}
	if (sample_rate != SAMPLE_RATE)
		fprintf(fp, PROMPT1("lif set oc %d sample %d\n"), (unsigned int)oc_no, (unsigned int)sample_rate);
	
	
	for (range_no = 0; range_no < MAX_PKT_RANGE; range_no++)
	{
		if (db_get_pktlen_range(&ppar->db, oc_no, range_no, &pktlen_min, &pktlen_max) < 0)
    		{
        		errprint("Failed to get packet length range in database.\n");
        		cli_db_unlock(ppar);
        		return -1;
    		}
    		if ((pktlen_min != PKTLEN_MIN) || (pktlen_max != PKTLEN_MAX))
			fprintf(fp, PROMPT1("lif set oc %d packet length range %d %d-%d\n"), 
		            (unsigned int)oc_no, (unsigned int)range_no, (unsigned int)pktlen_min, (unsigned int)pktlen_max);
    	}
	
    	if (db_get_traffic_enable(&ppar->db, oc_no, &enable_or_disable) < 0)
    	{
        	errprint("Failed to get traffic enable in database.\n");
        	cli_db_unlock(ppar);
        	return -1;
    	}
    	if (enable_or_disable != ENABLED)
		fprintf(fp, PROMPT1("lif set oc %d traffic disable\n"), (unsigned int)oc_no);
	
	// gtp
	if (db_get_gtp_dest_ports(&ppar->db, oc_no, &gtpdestports) < 0)
    	{
        	errprint("Failed to get traffic enable in database.\n");
        	cli_db_unlock(ppar);
        	return -1;
    	}
    	if (gtpdestports != 24)
		fprintf(fp, PROMPT1("lif set gtp oc %d hash dest %d\n"), (unsigned int)oc_no, (unsigned int)gtpdestports);
	
	if (db_get_gtp_hash_mode(&ppar->db, oc_no, &gtphashmode) < 0)
    	{
        	errprint("Failed to get traffic enable in database.\n");
        	cli_db_unlock(ppar);
        	return -1;
    	}
    	if (gtphashmode != 1)
		fprintf(fp, PROMPT1("lif set gtp oc %d hash mode outer\n"), (unsigned int)oc_no);
    }
    
    return 0;
}


// drv related
static int
reset_default_config()
{
    if (reset_filter_default_config() < 0)
    {
        errprint("Can't reset port default config in driver.\n");
        return -1;
    }
  	
    if (reset_forward_default_config() < 0)
    {
        errprint("Can't reset port default config in driver.\n");
        return -1;
    }
    
  	//added for global switch inside fpga
    if (reset_global_default_config() < 0)
    {
        errprint("Can't reset port default config in driver.\n");
        return -1;
    }
    
    return 0;
}

static int
reset_filter_default_config()
{
    unsigned int oc_no;
    unsigned int filter_id;
    rule1options_t opts1;
    rule2options_t opts2;
    
    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no ++)
    {
        for (filter_id = 0; filter_id < MAX_RULE_NO - 1; filter_id++)
        {
            memset(&opts1, 0, sizeof(opts1));
            opts1.id = filter_id;
            opts1.flag = 0x3f;
            drv_set_ruleset1(oc_no, &opts1);
        }
        
        memset(&opts1, 0, sizeof(opts1));
        opts1.id = filter_id;
        drv_set_ruleset1(oc_no, &opts1);
        
        
        for (filter_id = 0; filter_id < MAX_RULE_NO - 1; filter_id++)
        {
            memset(&opts2, 0, sizeof(opts2));
            opts2.id = filter_id;
            opts2.flag = 0x3;
            drv_set_ruleset2(oc_no, &opts2);
        }
        
        memset(&opts2, 0, sizeof(opts2));
        opts2.id = filter_id;
        drv_set_ruleset2(oc_no, &opts2);
    } // for (oc_no = 0;
	
    return 0;
}

static int
reset_forward_default_config()
{
    unsigned int oc_no;

    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no ++)
    {	
        // first reset oc 0
        drv_reset_forward_default_config(oc_no);
    }
	
    return 0;
}


static int
reset_global_default_config()
{
    unsigned int oc_no;

    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no ++)
    {	
        // first reset oc 0
       drv_reset_global_default_config(oc_no);
    }
    
    return 0;
}


int
cli_update_firmware(int oc_no)
{
    printf(PROMPT1("Updating system sub-card %d firmware\n"), oc_no);
    printf(PROMPT1("This process will take about 20 minutes, please wait.\n"));
    
    if (!drv_update_firmware(oc_no))
    {
        printf(PROMPT1("Update success, please reboot system.\n"));
    }
    else
    {
        printf(PROMPT1("Update failed, please try again.\n"));
    }
    
    return 0;
}

int
cli_etxconfig()
{
    char cmd[256];
    sprintf(cmd, "vi %s\n", ETXCONFIGFILE);
    
    return (system(cmd));
}

int
cli_show_system()
{
    printf("\n");
    printf(PROMPT2("Board type:", "%s\n"), "EX10/EX20");
    printf(PROMPT2("Sub type:", "%s\n"), "4 10GE -> 24 GE");
    
    printf(PROMPT2("Application Version:", "%s\n"),"V3.3");
    printf(PROMPT2("Update Time:","%s\n"),"2012-12-28");    
    printf(PROMPT2("Update Time:","%s\n"),"2013-01-11");
    printf(PROMPT2("Update Time:","%s\n"),"2013-11-02");
    printf(PROMPT2("Update Time:","%s\n\n"),"2013-11-06");
    
    printf(PROMPT2("FPGA Version:", "%s\n"), "V3.0");
    printf(PROMPT2("Build Time:","%s\n\n"),"2012-12-28");
    
    printf(PROMPT1("System uptime:\n"));
    system("/usr/bin/uptime");
    printf("\n");
    
    return 0;
}

/*******************************************************************
 *
 * Description:     OC Settings Functions
 *
 *******************************************************************/
int
cli_set_oc_line_rate(struct parser_params *ppar, int oc_no, unsigned long line_rate)
{
    cli_db_lock(ppar);
    
    if (db_set_oc_line_rate(&ppar->db, (unsigned long)oc_no, line_rate) < 0)
    {
        errprint("Can't set oc line mode in database.\n");
        cli_db_unlock(ppar);
        return -1;
    }
    
    cli_db_unlock(ppar);
    
    return 0;
}

int
cli_set_oc_line_mode(struct parser_params *ppar, int oc_no, unsigned long line_mode)
{
    cli_db_lock(ppar);
    
    if (db_set_oc_line_mode(&ppar->db, (unsigned long)oc_no, line_mode) < 0)
    {
        errprint("Can't set oc line mode in database.\n");
        cli_db_unlock(ppar);
        return -1;
    }
    
    cli_db_unlock(ppar);
    
    return 0;
}

int
cli_set_oc_encap_mode(struct parser_params *ppar, int oc_no, unsigned long encap_mode)
{
    cli_db_lock(ppar);
    
    if (db_set_oc_encap_mode(&ppar->db, (unsigned long)oc_no, encap_mode) < 0)
    {   
        errprint("Can't set oc encap_mode in database.\n");
        cli_db_unlock(ppar);
        return -1;
    }
    
    cli_db_unlock(ppar);
    
    return 0;
}

int
cli_set_oc_sonnet_scramble_enable(struct parser_params *ppar, int oc_no, unsigned long enable_or_disable)
{
    cli_db_lock(ppar);
    
    if (db_set_oc_sonnet_scramble_enable(&ppar->db, (unsigned long)oc_no, enable_or_disable) < 0)
    {
        errprint("Can't set oc sonnet scramble in database.\n");
        cli_db_unlock(ppar);
        return -1;  
    }
    
    cli_db_unlock(ppar);
    
    return 0;
}

int
cli_set_oc_payload_scramble_enable(struct parser_params *ppar, int oc_no, unsigned long enable_or_disable)
{
    cli_db_lock(ppar);
    
    if (db_set_oc_payload_scramble_enable(&ppar->db, (unsigned long)oc_no, enable_or_disable) < 0)
    {
        errprint("Can't set oc payload scramble in database.\n");
        cli_db_unlock(ppar);
        return -1;  
    }
    
    cli_db_unlock(ppar);
    
    return 0;
}

int 
cli_set_oc_crc_mode(struct parser_params *ppar, int oc_no, unsigned long crc_mode)
{
    cli_db_lock(ppar);
    
    if (db_set_oc_crc_mode(&ppar->db, (unsigned long)oc_no, crc_mode) < 0)
    {
        errprint("Can't set oc crc mode in database.\n");
        cli_db_unlock(ppar);
        return -1;  
    }
    
    cli_db_unlock(ppar);
    
    return 0;
}

int
cli_get_oc_settings(struct parser_params *ppar)
{
    int oc_no;
    unsigned long line_rate[MAX_OC_NUM];
    unsigned long line_mode[MAX_OC_NUM];
    unsigned long encap_mode[MAX_OC_NUM];
    unsigned long sonnet_scramble[MAX_OC_NUM];
    unsigned long payload_scramble[MAX_OC_NUM];
    unsigned long crc_mode[MAX_OC_NUM];
    unsigned long min_pkt[MAX_OC_NUM];
    unsigned long max_pkt[MAX_OC_NUM];
    
    /* print header */
    printf("\n");
    printf("     =================================================================\n");
    printf("     OC   Line   SDH   Encap  LineScram  PayloadScram    CRC    MinLen    MaxLen\n");
    printf("     =================================================================\n");
    
    cli_db_lock(ppar);
    
    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no++)
    {
    	if (db_get_oc_line_rate(&ppar->db, (unsigned long)oc_no, &line_rate[oc_no]) < 0)
        {   
            errprint("Can't get oc line_rate in database.\n");
            cli_db_unlock(ppar);
            return -1;
        }
        
      	if (db_get_oc_line_mode(&ppar->db, (unsigned long)oc_no, &line_mode[oc_no]) < 0)
      	{ 
          	errprint("Can't get oc line_mode in database.\n");
          	cli_db_unlock(ppar);
          	return -1;
      	}
      	    
      	if (db_get_oc_encap_mode(&ppar->db, (unsigned long)oc_no, &encap_mode[oc_no]) < 0)
      	{ 
          	errprint("Can't get oc encap_mode in database.\n");
          	cli_db_unlock(ppar);
          	return -1;
    	}
    
     	if (db_get_oc_sonnet_scramble_enable(&ppar->db, (unsigned long)oc_no, &sonnet_scramble[oc_no]) < 0)
      	{
          	errprint("Can't get oc sonnet scramble in database.\n");
          	cli_db_unlock(ppar);
          	return -1;    
      	}
        
      	if (db_get_oc_payload_scramble_enable(&ppar->db, (unsigned long)oc_no, &payload_scramble[oc_no]) < 0)
      	{
          	errprint("Can't get oc payload scramble in database.\n");
          	cli_db_unlock(ppar);
          	return -1;    
      	} 
        
      	if (db_get_oc_crc_mode(&ppar->db, (unsigned long)oc_no, &crc_mode[oc_no]) < 0)
      	{
          	errprint("Can't get oc crc mode in database.\n");
          	cli_db_unlock(ppar);
          	return -1;    
      	}

      	if (db_get_oc_min_pkt_size(&ppar->db, (unsigned long)oc_no, &min_pkt[oc_no]) < 0)
      	{
          	errprint("Can't get oc crc mode in database.\n");
          	cli_db_unlock(ppar);
          	return -1;    
      	}

      	if (db_get_oc_max_pkt_size(&ppar->db, (unsigned long)oc_no, &max_pkt[oc_no]) < 0)
      	{
          	errprint("Can't get oc crc mode in database.\n");
          	cli_db_unlock(ppar);
          	return -1;    
      	}		
		
  	} // for
  	
  	cli_db_unlock(ppar);
    
  	for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no++)
  	{
      	printf("     %2d %6s %6s   %5s  %11s  %13s  %5s %10u %10u\n", 
            oc_no,
            ((line_rate[oc_no] == OC_LINERATE_192C) ? "OC192" : "OC48"),
            ((line_mode[oc_no] == OC_LINE_SDH) ? "SDH" : "SONET"),
            ((encap_mode[oc_no] == OC_ENCAP_PPP) ? "PPP" : "HDLC"),
            ((sonnet_scramble[oc_no] == ENABLED) ? "Enable" : "Disable"),
            ((payload_scramble[oc_no] == ENABLED) ? "Enable" : "Disable"),
            ((crc_mode[oc_no] == OC_CRC_32) ? "CRC32" : ((crc_mode[oc_no] == OC_CRC_CCITT) ? "CCITT":"None")),
            (unsigned int)min_pkt[oc_no],
            (unsigned int)max_pkt[oc_no]
            );
        
      	printf("     -----------------------------------------------------------------\n\n");
    }
    
    return 0;       
}

int
cli_save_oc_settings(struct parser_params *ppar)
{       
    FILE *fp_tmp;
  	unsigned long result0, result1;
    
  	// open a tmp file for write
    if((fp_tmp = fopen(DRV_CONF_FILE_TMP,"wb")) == NULL)
    {
        errprint("Can't save oc settings while writing to file.\n");
        return -1;
    }
    
    
    cli_db_lock(ppar);
    
  	{
        if (db_get_oc_line_rate(&ppar->db, (unsigned long)0, &result0) < 0)
        {   
            errprint("Can't get oc line_mode in database.\n");
            cli_db_unlock(ppar);
            return -1;
        }   
        if (db_get_oc_line_rate(&ppar->db, (unsigned long)1, &result1) < 0)
        {   
            errprint("Can't get oc line_mode in database.\n");
            cli_db_unlock(ppar);
            return -1;
        }
    	fprintf(fp_tmp, "LINE_RATE        %u,%u\n", (unsigned int)result0, (unsigned int)result1);
	
    	
      	if (db_get_oc_line_mode(&ppar->db, (unsigned long)0, &result0) < 0)
      	{ 
            errprint("Can't get oc line_mode in database.\n");
            cli_db_unlock(ppar);
            return -1;
      	}
	if (db_get_oc_line_mode(&ppar->db, (unsigned long)1, &result1) < 0)
      	{ 
            errprint("Can't get oc line_mode in database.\n");
            cli_db_unlock(ppar);
            return -1;
      	}
    	fprintf(fp_tmp, "SONET_MODE        %u,%u\n", (unsigned int)result0, (unsigned int)result1);
       
               
      	if (db_get_oc_encap_mode(&ppar->db, (unsigned long)0, &result0) < 0)
      	{
            errprint("Can't get oc encap_mode in database.\n");
            cli_db_unlock(ppar);
            return -1;
      	}
	if (db_get_oc_encap_mode(&ppar->db, (unsigned long)1, &result1) < 0)
      	{
            errprint("Can't get oc encap_mode in database.\n");
            cli_db_unlock(ppar);
            return -1;
      	}
    	fprintf(fp_tmp, "PPP_MODE        %u,%u\n", (unsigned int)result0, (unsigned int)result1);
       
       
    	if (db_get_oc_sonnet_scramble_enable(&ppar->db, (unsigned long)0, &result0) < 0)
        {
            errprint("Can't get oc sonnet scramble in database.\n");
            cli_db_unlock(ppar);
            return -1;
        }
	 if (db_get_oc_sonnet_scramble_enable(&ppar->db, (unsigned long)1, &result1) < 0)
        {
            errprint("Can't get oc sonnet scramble in database.\n");
            cli_db_unlock(ppar);
            return -1;
        }
        fprintf(fp_tmp, "SONET_SCRAMBLE        %u, %u\n", (unsigned int)result0, (unsigned int)result1);
        
        
        if (db_get_oc_payload_scramble_enable(&ppar->db, (unsigned long)0, &result0) < 0)
        {
            errprint("Can't get oc payload scramble in database.\n");
            cli_db_unlock(ppar);
            return -1;
        }
        if (db_get_oc_payload_scramble_enable(&ppar->db, (unsigned long)1, &result1) < 0)
        {
            errprint("Can't get oc payload scramble in database.\n");
            cli_db_unlock(ppar);
            return -1;
        }
        fprintf(fp_tmp, "PAYLOAD_SCRAMBLE        %u, %u\n", (unsigned int)result0, (unsigned int)result1);
        
            
        if (db_get_oc_crc_mode(&ppar->db, (unsigned long)0, &result0) < 0)
        {
            errprint("Can't get oc crc mode in database.\n");
            cli_db_unlock(ppar);
            return -1;  
        }
	if (db_get_oc_crc_mode(&ppar->db, (unsigned long)1, &result1) < 0)
        {
            errprint("Can't get oc crc mode in database.\n");
            cli_db_unlock(ppar);
            return -1;  
        }
        fprintf(fp_tmp, "CRC32_MODE        %u, %u\n", (unsigned int)result0, (unsigned int)result1);


	 if (db_get_oc_pass_crc_mode(&ppar->db, (unsigned long)0, &result0) < 0)
        {
            errprint("Can't get oc crc mode in database.\n");
            cli_db_unlock(ppar);
            return -1;  
        }
	if (db_get_oc_pass_crc_mode(&ppar->db, (unsigned long)1, &result1) < 0)
        {
            errprint("Can't get oc crc mode in database.\n");
            cli_db_unlock(ppar);
            return -1;  
        }
        fprintf(fp_tmp, "PASS_CRC        %u, %u\n", (unsigned int)result0, (unsigned int)result1);


	 if (db_get_oc_max_pkt_size(&ppar->db, (unsigned long)0, &result0) < 0)
        {
            errprint("Can't get oc crc mode in database.\n");
            cli_db_unlock(ppar);
            return -1;  
        }
	if (db_get_oc_max_pkt_size(&ppar->db, (unsigned long)1, &result1) < 0)
        {
            errprint("Can't get oc crc mode in database.\n");
            cli_db_unlock(ppar);
            return -1;  
        }
        fprintf(fp_tmp, "MAX_PKT_SIZE        %u, %u\n", (unsigned int)result0, (unsigned int)result1);


	 if (db_get_oc_min_pkt_size(&ppar->db, (unsigned long)0, &result0) < 0)
        {
            errprint("Can't get oc crc mode in database.\n");
            cli_db_unlock(ppar);
            return -1;  
        }
	if (db_get_oc_min_pkt_size(&ppar->db, (unsigned long)1, &result1) < 0)
        {
            errprint("Can't get oc crc mode in database.\n");
            cli_db_unlock(ppar);
            return -1;  
        }
        fprintf(fp_tmp, "MIN_PKT_SIZE        %u, %u\n", (unsigned int)result0, (unsigned int)result1);
    }   
    
    fclose(fp_tmp);
    
    if(rename(DRV_CONF_FILE_TMP,DRV_CONF_FILE) != 0)
    {
        errprint("Can't replace the old drvconfig file.\n");
        cli_db_unlock(ppar);
        return -1;
    }
    
    cli_db_unlock(ppar);
    
    
    return 0;
}


/**********************************************************************
 *
 * Description: FPGA Functions
 *
 *********************************************************************/
int
cli_fpga_set_ruleset1(struct parser_params *ppar, int oc_no, rule1options_t *opts)
{
    unsigned long ruleno;
    unsigned long rule_num;
    
    
    ruleno = opts->id;
	
    cli_db_lock(ppar);
    
    {
        // rule already exist? modify it
        if (db_filter1_rule_exist(&ppar->db, (unsigned long)oc_no, ruleno) == 0)
        {
            if (db_get_filter1_rule_num(&ppar->db, (unsigned long)oc_no, &rule_num) < 0)
            {
                errprint("Can't get classify rule numbers from database.\n");
                cli_db_unlock(ppar);
                return -1;          
            }
            
            rule_num ++;
            
            if (db_set_filter1_rule_num(&ppar->db, (unsigned long)oc_no, rule_num) < 0)
            {
                errprint("Can't set classify rule numbers to database.\n");
                cli_db_unlock(ppar);
                return -1;              
            }
            
            if (db_set_filter1_rule(&ppar->db, (unsigned long)oc_no, (rule1options_t *)opts) < 0)
            {
                errprint("Can't set classify rules to database.\n");
                cli_db_unlock(ppar);
                return -1;          
            }
        }
        // not, then add it
        else
        {
            if (db_set_filter1_rule(&ppar->db, (unsigned long)oc_no, (rule1options_t *)opts) < 0)
            {
                errprint("Can't set classify rules to database.\n");
                cli_db_unlock(ppar);
                return -1;
            }   
        }
    }
    
    cli_db_unlock(ppar);
    
    if (drv_set_ruleset1(oc_no, opts))
    {
        errprint("Can't set classify rules in driver.\n");
        return -1;
    }
    
    return 0;
}

int
cli_fpga_set_ruleset2(struct parser_params *ppar, int oc_no, rule2options_t *opts)
{
    unsigned long  ruleno;
    unsigned long  rule_num;
    
    ruleno = opts->id;
    
    cli_db_lock(ppar);
    
    {
        if (db_filter2_rule_exist(&ppar->db, (unsigned long)oc_no, ruleno) == 0)
        {
            if (db_get_filter2_rule_num(&ppar->db, (unsigned long)oc_no, &rule_num) < 0)
            {
                errprint("Can't get classify rule numbers from database.\n");
                cli_db_unlock(ppar);
                return -1;          
            }
            
            rule_num ++;
            
            if (db_set_filter2_rule_num(&ppar->db, (unsigned long)oc_no, rule_num) < 0)
            {
                errprint("Can't set classify rule numbers to database.\n");
                cli_db_unlock(ppar);
                return -1;              
            }
            
            if (db_set_filter2_rule(&ppar->db, (unsigned long)oc_no, (rule2options_t *)opts) < 0)
            {
                errprint("Can't set classify rules to database.\n");
                cli_db_unlock(ppar);
                return -1;
            }
        }
        else
        {
            if (db_set_filter2_rule(&ppar->db, (unsigned long)oc_no, (rule2options_t *)opts) < 0)
            {
                errprint("Can't set classify rules to database.\n");
                cli_db_unlock(ppar);
                return -1;
            }
        }
    }
    
    cli_db_unlock(ppar);
    
    if (drv_set_ruleset2(oc_no, opts))
    {
        errprint("Can't set classify rules in driver.\n");
        return -1;
    }
    
    return 0;
}


int 
cli_fpga_clear_ruleset1(struct parser_params *ppar, int oc_no, int ruleno)
{
    unsigned long rule_num;
    rule1options_t opts;
    
    cli_db_lock(ppar);
    
    if(db_filter1_rule_exist(&ppar->db, (unsigned long)oc_no, ruleno))
    {
        if (db_get_filter1_rule_num(&ppar->db, (unsigned long)oc_no, &rule_num) < 0)
        {
            errprint("Can't get classify rule number.\n");
            cli_db_unlock(ppar);
            return -1;
        }
		
        rule_num--;
		
        if(db_set_filter1_rule_num(&ppar->db, (unsigned long)oc_no, rule_num) < 0)
        {
            errprint("Can't set classify rule number.\n");
            cli_db_unlock(ppar);
            return -1;
        }
    	
        if (db_delete_filter1_rule(&ppar->db, (unsigned long)oc_no, ruleno) < 0)
        {
            errprint("Can't clear classify rules from database.\n");
            cli_db_unlock(ppar);
            return -1;
        }
        
        {
    		// set this entry to be all '0', no packet should match this
        	memset(&opts, 0, sizeof(opts));
        	opts.id = ruleno;
        	opts.flag = 0x3f;
        	drv_set_ruleset1(oc_no, &opts);
    	}
    }
	
    cli_db_unlock(ppar);    
    
    
    return 0;
}

int 
cli_fpga_clear_ruleset2(struct parser_params *ppar, int oc_no, int ruleno)
{
    unsigned long rule_num;
    rule2options_t opts;
    
    cli_db_lock(ppar);
	
    if(db_filter2_rule_exist(&ppar->db, (unsigned long)oc_no, ruleno))
    {
        if (db_get_filter2_rule_num(&ppar->db, (unsigned long)oc_no, &rule_num) < 0)
        {
            errprint("Can't get classify rule number.\n");
            cli_db_unlock(ppar);
            return -1;
        }
		
        rule_num--;
		
        if(db_set_filter2_rule_num(&ppar->db, (unsigned long)oc_no, rule_num) < 0)
        {
            errprint("Can't set classify rule number.\n");
            cli_db_unlock(ppar);
            return -1;
        }
    	
        if (db_delete_filter2_rule(&ppar->db, (unsigned long)oc_no, ruleno) < 0)
        {
            errprint("Can't clear classify rules from database.\n");
            cli_db_unlock(ppar);
            return -1;
        }           
    	
        {
            // set this entry to be all '0', no packet should match this
            memset(&opts, 0, sizeof(opts));
            opts.id = ruleno;
            opts.flag = 0x3;
            drv_set_ruleset2(oc_no, &opts);
        }
    }
	
    cli_db_unlock(ppar);  
    
    
    return 0;
}

int
cli_get_filter_ruleset1(struct parser_params *ppar, int oc_no, unsigned long rule_id)
{   
    rule1options_t ruleopts;
    ruleopts.id = rule_id;
    
    cli_db_lock(ppar);
    
    if (db_filter1_rule_exist(&ppar->db, (unsigned long)oc_no, rule_id))
    {
        if (db_get_filter1_rule(&ppar->db, (unsigned long)oc_no, (rule1options_t *)&ruleopts))
        {
            errprint("Can't get classify rules from database.\n");
            cli_db_unlock(ppar);
            return -1;
        }   
        
        printf(PROMPT1("OC %d Classify ruleset1 %d\n"), oc_no, ruleopts.id);
        
        if (ruleopts.flag & RULE_OPT_PROTOCOL)
        {
      		switch(ruleopts.protocol)
            {
                case 0x06:                  
                    printf(PROMPT2("Classify rule protocol:", "%s\n"), "tcp");
                    break;
                case 0x11:                  
                    printf(PROMPT2("Classify rule protocol:", "%s\n"), "udp"); 
                    break;
                default:
                    printf(PROMPT2("Classify rule protocol:", "%d\n"), ruleopts.protocol); 
                    break;
            }
        }
        else
            printf(PROMPT2("Classify rule protocol:", "%s\n"), "any");
        
        if (ruleopts.flag & RULE_OPT_SRCIP)
        {
            printf(PROMPT2("Classify rule source ip:", "%d.%d.%d.%d\n"), (int)((ruleopts.sourceip>>24)&0xff), (int)((ruleopts.sourceip>>16)&0xff),
                (int)((ruleopts.sourceip>>8)&0xff), (int)(ruleopts.sourceip&0xff));
            printf(PROMPT2("Classify rule source ip mask:", "%d.%d.%d.%d\n"), (int)((ruleopts.sourceip_mask>>24)&0xff), (int)((ruleopts.sourceip_mask>>16)&0xff),
                (int)((ruleopts.sourceip_mask>>8)&0xff), (int)(ruleopts.sourceip_mask&0xff));                   
        }
        else
        {
            printf(PROMPT2("Classify rule source ip:", "%s\n"), "any");
            printf(PROMPT2("Classify rule source ip mask:", "%s\n"), "any");
        }
        
        if (ruleopts.flag & RULE_OPT_DESTIP)
        {
            printf(PROMPT2("Classify rule dest ip:", "%d.%d.%d.%d\n"), (int)((ruleopts.destip>>24)&0xff), (int)((ruleopts.destip>>16)&0xff),
                (int)((ruleopts.destip>>8)&0xff), (int)(ruleopts.destip&0xff));
            printf(PROMPT2("Classify rule dest ip mask:", "%d.%d.%d.%d\n"), (int)((ruleopts.destip_mask>>24)&0xff), (int)((ruleopts.destip_mask>>16)&0xff),
                (int)((ruleopts.destip_mask>>8)&0xff), (int)(ruleopts.destip_mask&0xff));                   
        }
        else
        {
            printf(PROMPT2("Classify rule dest ip:", "%s\n"), "any");
            printf(PROMPT2("Classify rule dest ip mask:", "%s\n"), "any");
        }
        
        if (ruleopts.flag & RULE_OPT_SRCL4PORT)
        {
            printf(PROMPT2("Classify rule source l4_port:", "%d\n"), ruleopts.sourcel4port);            
        }
        else
        {
            printf(PROMPT2("Classify rule source l4_port:", "%s\n"), "any");
        }
        
        if (ruleopts.flag & RULE_OPT_DESTL4PORT)
        {
            printf(PROMPT2("Classify rule dest l4_port:", "%d\n"), ruleopts.destl4port);            
        }
        else
        {
            printf(PROMPT2("Classify rule dest l4_port:", "%s\n"), "any");
        }   
        
    	if (ruleopts.flag & RULE_OPT_PACKETLEN)
    	{
      		printf(PROMPT2("Classify rule packet length range:", "%d\n"), ruleopts.pktlen_range);
    	}
   	 	else
    	{
      		printf(PROMPT2("Classify rule packet length range:", "%s\n"), "any");
    	}
		
    	printf("\n");
    }
    
    cli_db_unlock(ppar);
    
    
    return 0;
}

int
cli_get_filter_ruleset2(struct parser_params *ppar, int oc_no, unsigned long rule_id)
{   
    unsigned char word_fs, word_ls;
    unsigned char key[MAX_KEY_LEN];
    unsigned int i, j;
    rule2options_t ruleopts;
    
    ruleopts.id = rule_id;
    
    cli_db_lock(ppar);
    
    if (db_filter2_rule_exist(&ppar->db, (unsigned long)oc_no, rule_id))
    {
        if (db_get_filter2_rule(&ppar->db, (unsigned long)oc_no, (rule2options_t *)&ruleopts))
        {
            errprint("Can't get classify rules from database.\n");
            cli_db_unlock(ppar);
            return -1;
        }   
        
        printf(PROMPT1("OC %d Classify ruleset2 %d\n"), oc_no, ruleopts.id);
		
        if (ruleopts.flag & RULE_OPT_OFFSET)
            printf(PROMPT2("Classify rule offset:", "%d\n"), ruleopts.offset);
        else
            printf(PROMPT2("Classify rule offset:", "%d\n"), 0);
        
        if(ruleopts.flag & RULE_OPT_KEY)
        {
            for(i=0, j=0; i< MAX_KEY_NO; i++)
            {
                if ((ruleopts.key[i] == 0x0) && (ruleopts.mask[i] == 0xff))
                {
                    key[j] = 0;
                    break;
                }
                
                word_fs = (ruleopts.key[i] & 0xf0) >> 4;
                if (word_fs < 0xa)
                    word_fs += 0x30;
                else
                    word_fs += 0x37;
                key[j] = word_fs;
                j++;
                
                word_ls = (ruleopts.key[i] & 0xf);
                if (word_ls < 0xa)
                    word_ls += 0x30;
                else
                    word_ls += 0x37;
                key[j] = word_ls;
                j++;
            }         
            printf(PROMPT2("Classify rule key:", "%s\n"), key);
        }
    } // if
    
    cli_db_unlock(ppar);    
    
    return 0;
}

int
cli_set_forward_mode(struct parser_params *ppar, int oc_no, unsigned long ruleno1, unsigned long ruleno2, unsigned long forward_mode)
{
    unsigned long data;
    int i, j, addr;
    
    cli_db_lock(ppar);
    
    //update db
    if ((ruleno1 == MAX_RULE_NO) && (ruleno2 == MAX_RULE_NO))
    {
        for (i=0; i < MAX_RULE_NO; i++)
            for (j=0; j < MAX_RULE_NO; j++)
            {
        	  if(db_forward_mode_exist(&ppar->db, (unsigned long) oc_no, (unsigned long)i, (unsigned long)j))
                {
                    //case2: old(no default) -> new(default): delete
                    if (((forward_mode >> 12)&0xf) == FORWARD_DEF)
                        db_delete_forward_mode(&ppar->db, (unsigned long) oc_no, (unsigned long)i, (unsigned long)j);
                    //case3: old(non default) -> new(non default): change
                    else    
                        db_set_forward_mode(&ppar->db, (unsigned long) oc_no, (unsigned long)i, (unsigned long)j, forward_mode);
                }
                else
                {
                    //case0: old(default) -> new(default): unchange
                    //case1: old (default) -> new(non default): add
                    if (((forward_mode >> 12)&0xf) != FORWARD_DEF)
                        db_set_forward_mode(&ppar->db, (unsigned long) oc_no, (unsigned long)i, (unsigned long)j, forward_mode);
                }
            } // for
       }
  	else if (ruleno1 == MAX_RULE_NO)
  	{
        for (i=0; i < MAX_RULE_NO; i++)
        {       
            if(db_forward_mode_exist(&ppar->db, (unsigned long) oc_no, (unsigned long)i, ruleno2))
            {
                //case2: old(non hash) -> new(hash): delete
                if (((forward_mode >> 12)&0xf) == FORWARD_DEF)
                    db_delete_forward_mode(&ppar->db, (unsigned long) oc_no, (unsigned long)i, ruleno2);
                //case3: old(non hash) -> new(non hash): change
                else    
                    db_set_forward_mode(&ppar->db, (unsigned long) oc_no, (unsigned long)i, ruleno2, forward_mode);
            }
            else
            {
                //case0: old(hash) -> new(hash): unchange
                //case1: old (hash) -> new(non hash): add
                if (((forward_mode >> 12)&0xf) != FORWARD_DEF)
                    db_set_forward_mode(&ppar->db, (unsigned long) oc_no, (unsigned long)i, ruleno2, forward_mode);
            }
        }
    }
  	else if (ruleno2 == MAX_RULE_NO)
  	{
        for (j=0; j < MAX_RULE_NO; j++) 
        {
        	if(db_forward_mode_exist(&ppar->db, (unsigned long) oc_no, ruleno1, (unsigned long)j))
            {
                //case2: old(non hash) -> new(hash): delete
                if (((forward_mode >> 12)&0xf) == FORWARD_DEF)
                    db_delete_forward_mode(&ppar->db, (unsigned long) oc_no, ruleno1, (unsigned long)j);
                else    
                //case3: old(non hash) -> new(non hash): change
                    db_set_forward_mode(&ppar->db, (unsigned long) oc_no, ruleno1, (unsigned long)j, forward_mode);
            }
            else
            {
                //case0: old(hash) -> new(hash): unchange
                //case1: old (hash) -> new(non hash): add
                if (((forward_mode >> 12)&0xf) != FORWARD_DEF)
                    db_set_forward_mode(&ppar->db, (unsigned long) oc_no, ruleno1, (unsigned long)j, forward_mode);
            }
        }
  	} 
  	else
  	{
        if(db_forward_mode_exist(&ppar->db, (unsigned long) oc_no, ruleno1, ruleno2))
        {
            //case2: old(non hash) -> new(hash): delete
            if (((forward_mode >> 12)&0xf) == FORWARD_DEF)
                db_delete_forward_mode(&ppar->db, (unsigned long) oc_no, ruleno1, ruleno2);
            //case3: old(non hash) -> new(non hash): change
            else
                db_set_forward_mode(&ppar->db, (unsigned long) oc_no, ruleno1, ruleno2, forward_mode);
        }
        else
        {
            //case0: old(hash) -> new(hash): unchange
            //case1: old (hash) -> new(non hash): add
            if (((forward_mode >> 12)&0xf) != FORWARD_DEF)
                db_set_forward_mode(&ppar->db, (unsigned long) oc_no, ruleno1, ruleno2, forward_mode);
        }
  	} 
    
    cli_db_unlock(ppar);
    
    
    // update drv
    if ((ruleno1 == MAX_RULE_NO) && (ruleno2 == MAX_RULE_NO))
    {
        for (i=0; i < MAX_RULE_NO; i++)
      	     for (j =0; j < MAX_RULE_NO; j++)
            {
                addr = ((i << 6) | j) & 0xfff;
                data = ((forward_mode & 0xffff) << 16) | addr;
                drv_set_forward_mode(oc_no, data);  
            }
    }
    else if (ruleno1 == MAX_RULE_NO)
    {
        for (i=0; i < MAX_RULE_NO; i++)
        {       
            addr = ((i << 6) | ruleno2)&0xfff;
            data = ((forward_mode & 0xffff) << 16) | addr;
            drv_set_forward_mode(oc_no, data);  
    	}
    }
    else if (ruleno2 == MAX_RULE_NO)
    {
        for (j=0; j < MAX_RULE_NO; j++) 
        {
            addr = ((ruleno1 << 6) | j)&0xfff;
            data = ((forward_mode & 0xffff) << 16) | addr;
            drv_set_forward_mode(oc_no, data);
        }
    }
    else
    {
    	addr = ((ruleno1 << 6) | ruleno2)&0xfff;
        data = ((forward_mode & 0xffff) << 16) | addr;
        drv_set_forward_mode(oc_no, data);
    }
    
    
    return 0;
}

int
cli_get_forward_mode(struct parser_params *ppar, int oc_no, unsigned long ruleno1, unsigned long ruleno2)
{
    unsigned long forward_mode;
    int i=0, j=0;
    int vlanid, mode;
    int isvlan, issample, isdrop;
    
    
    cli_db_lock(ppar);
    
    if ((ruleno1 == MAX_RULE_NO) && (ruleno2 == MAX_RULE_NO))
    {
        for (i =0 ; i < MAX_RULE_NO; i++)
            for (j =0; j < MAX_RULE_NO; j++)
            {
            	if(db_forward_mode_exist(&ppar->db, (unsigned long) oc_no, (unsigned long)i, (unsigned long)j))
                    db_get_forward_mode(&ppar->db, (unsigned long) oc_no, (unsigned long)i, (unsigned long)j, &forward_mode);
                else
                    forward_mode = FORWARD_DEF;
                
                vlanid = forward_mode & 0x0fff;
                mode = (forward_mode >> 12) & 0xf;
                                
                isvlan   = (mode >> 0) & 0x1;
                issample = (mode >> 1) & 0x1;
                isdrop   = (mode >> 2) & 0x1;
                                
                if (isdrop)
                {
                    printf("     OC %d Ruleset1 %d Ruleset2 %d:", oc_no, i, j);
                    printf(PROMPT2("", "drop\n"));
                }
                else
                {
                    if (isvlan)
                    {
                        printf("     OC %d Ruleset1 %d Ruleset2 %d:", oc_no, i, j);
                        printf(PROMPT2("", "to vlan "));
                        printf("%d", vlanid);
                                
                        if (issample)
                        {
                            printf(" sample");
                        }   
			   printf("\n");			
                    }
                    else
                    {
                    	if (issample)
                        {
                            printf("     OC %d Ruleset1 %d Ruleset2 %d:", oc_no, i, j);
                            printf(PROMPT2("", "sample\n"));
                        }
                        else
                        {
                        	printf("     OC %d Ruleset1 %d Ruleset2 %d:", oc_no, i, j);
                            printf(PROMPT2("", "default (no vlan, no sample)\n"));
                        }   
                    }       
                }
            }
	}                       
    else if (ruleno1 == MAX_RULE_NO)
    {
        for (i =0 ; i < MAX_RULE_NO; i++)
        {
            if(db_forward_mode_exist(&ppar->db, (unsigned long) oc_no, (unsigned long)i, (unsigned long)ruleno2))
                db_get_forward_mode(&ppar->db, (unsigned long) oc_no, (unsigned long)i, (unsigned long)ruleno2, &forward_mode);
            else
                forward_mode = FORWARD_DEF;
            
            vlanid = forward_mode & 0x0fff;
            mode = (forward_mode >> 12) & 0xf;
            
            isvlan   = (mode >> 0) & 0x1;
            issample = (mode >> 1) & 0x1;
            isdrop   = (mode >> 2) & 0x1;
            
            if (isdrop)
            {
            	printf("     OC %d Ruleset1 %d Ruleset2 %d:", oc_no, i, (int)ruleno2);
                printf(PROMPT2("", "drop\n"));
            }
            else
            {
                if (isvlan)
                {
                    printf("     OC %d Ruleset1 %d Ruleset2 %d:", oc_no, i, (int)ruleno2);
                    printf(PROMPT2("", "to vlan "));
                    printf("%d", vlanid);
                                
                    if (issample)
                    {
                        printf(" sample");
                    }
		      printf("\n");		
                }
                else
                {
                    if (issample)
                    {
                        printf("     OC %d Ruleset1 %d Ruleset2 %d:", oc_no, i, (int)ruleno2);
                        printf(PROMPT2("", "sample\n"));
                    }
                    else
                    {
                        printf("     OC %d Ruleset1 %d Ruleset2 %d:", oc_no, i, (int)ruleno2);
                        printf(PROMPT2("", "default (no vlan, no sample)\n"));
                    }   
                }       
            }
        }
    }
    else if (ruleno2 == MAX_RULE_NO)
    {
        for (j =0 ; j < MAX_RULE_NO; j++)
      	{
        	if(db_forward_mode_exist(&ppar->db, (unsigned long) oc_no, (unsigned long)ruleno1, (unsigned long)j))
            	db_get_forward_mode(&ppar->db, (unsigned long) oc_no, (unsigned long)ruleno1, (unsigned long)j, &forward_mode);
            else
                forward_mode = (FORWARD_DEF << 12);
                
            vlanid = forward_mode & 0x0fff;
            mode = (forward_mode >> 12) & 0xf;
            
            isvlan   = (mode >> 0) & 0x1;
            issample = (mode >> 1) & 0x1;
            isdrop   = (mode >> 2) & 0x1;
            
            if (isdrop)
            {
                printf("     OC %d Ruleset1 %d Ruleset2 %d:", oc_no, (int)ruleno1, j);
                printf(PROMPT2("", "drop\n"));
            }
            else
            {
                if (isvlan)
                {
                    printf("     OC %d Ruleset1 %d Ruleset2 %d:", oc_no, (int)ruleno1, j);
                    printf(PROMPT2("", "to vlan "));
                    printf("%d", vlanid);
                                
                    if (issample)
                    {
                        printf(" sample");
                    }
			printf("\n");					
                }
                else
                {
                    if (issample)
                    {
                        printf("     OC %d Ruleset1 %d Ruleset2 %d:", oc_no, (int)ruleno1, j);
                        printf(PROMPT2("", "sample\n"));
                    }
                    else
                    {
                        printf("     OC %d Ruleset1 %d Ruleset2 %d:", oc_no, (int)ruleno1, j);
                        printf(PROMPT2("", "default (no vlan, no sample)\n"));
                    }   
                }       
            } // else
      	} // for
    }   
    else
    {
      	if(db_forward_mode_exist(&ppar->db, (unsigned long) oc_no, (unsigned long)ruleno1, (unsigned long)ruleno2))
        	db_get_forward_mode(&ppar->db, (unsigned long) oc_no, (unsigned long)ruleno1, (unsigned long)ruleno2, &forward_mode);
        else
            forward_mode = (FORWARD_DEF << 12);
            
        vlanid = forward_mode & 0x0fff;
        mode = (forward_mode >> 12) & 0xf;
        
        isvlan   = (mode >> 0) & 0x1;
        issample = (mode >> 1) & 0x1;
        isdrop   = (mode >> 2) & 0x1;
        
        if (isdrop)
        {
            printf("     OC %d Ruleset1 %d Ruleset2 %d:", oc_no, i, j);
            printf(PROMPT2("", "drop\n"));
        }
        else
        {
            if (isvlan)
            {
                printf("     OC %d Ruleset1 %d Ruleset2 %d:", oc_no, i, j);
                printf(PROMPT2("", "to vlan "));
                printf("%d", vlanid);
                                
                if (issample)
                {
                    printf(" sample");
                }
		printf("\n");				
            }
            else
            {
                if (issample)
                {
                    printf("     OC %d Ruleset1 %d Ruleset2 %d:", oc_no, i, j);
                    printf(PROMPT2("", "sample\n"));
                }
                else
                {
                    printf("     OC %d Ruleset1 %d Ruleset2 %d:", oc_no, i, j);
                    printf(PROMPT2("", "default (no vlan, no sample)\n"));
                }
            }
        }
    }// else
    
	
    cli_db_unlock(ppar);
    
    
    return 0;       
}


int 
cli_set_pktlen_range(struct parser_params *ppar, int oc_no, int range_no, int pktlen_min, int pktlen_max)
{
	cli_db_lock(ppar);

    /* write to database */
    if (db_set_pktlen_range(&ppar->db, oc_no, range_no, pktlen_min, pktlen_max) < 0)
    {
        errprint("Failed to set packet length range in database.\n");
        cli_db_unlock(ppar);
        return -1;
    }

    cli_db_unlock(ppar);
    
    drv_set_pktlen_range(oc_no, range_no, pktlen_min, pktlen_max);
    
    
    return 0;
}

int 
cli_get_pktlen_range(struct parser_params *ppar, int oc_no, int range_no)
{
	unsigned long pktlen_min, pktlen_max;
	
	cli_db_lock(ppar);

    /* write to database */
    if (db_get_pktlen_range(&ppar->db, oc_no, range_no, &pktlen_min, &pktlen_max) < 0)
    {
        errprint("Failed to get packet length range in database.\n");
        cli_db_unlock(ppar);
        return -1;
    }

    cli_db_unlock(ppar);
    
    
    printf(PROMPT1("OC %d Packet length range %d:"), oc_no, range_no);
    printf(PROMPT2("", "%d-"), (int)pktlen_min);
    printf("%d\n", (int)pktlen_max);
    
    
    return 0;
}


int 
cli_set_sample_rate(struct parser_params *ppar, int oc_no, int sample_rate)
{
    cli_db_lock(ppar);

    /* write to database */
    if (db_set_sample_rate(&ppar->db, oc_no, sample_rate) < 0)
    {
        errprint("Failed to set sample rate in database.\n");
        cli_db_unlock(ppar);
        return -1;
    }

    cli_db_unlock(ppar);
    
    
    // first oc 0
    drv_set_sample_rate(oc_no, sample_rate);
    
    
    return 0;
}


int 
cli_get_sample_rate(struct parser_params *ppar, int oc_no)
{
    unsigned long sample_rate;
	
	cli_db_lock(ppar);

    /* write to database */
    if (db_get_sample_rate(&ppar->db, oc_no, &sample_rate) < 0)
    {
        errprint("Failed to get sample rate in database.\n");
        cli_db_unlock(ppar);
        return -1;
    }
	
    cli_db_unlock(ppar);
    
    
    printf(PROMPT1("OC %d Packet sample rate:"), oc_no);
    printf(PROMPT2("", "%d\n"), (int)sample_rate);
    
    
    return 0;
}


int 
cli_set_eth_mac(struct parser_params *ppar, int oc_no, unsigned int src_dst, unsigned char mac[6])
{
    cli_db_lock(ppar);
	
    /* write to database */
    if (db_set_eth_mac(&ppar->db, oc_no, src_dst, mac) < 0)
    {
        errprint("Failed to set eth mac in database.\n");
        cli_db_unlock(ppar);
        return -1;
    }
	
    cli_db_unlock(ppar);
    
    
    drv_set_eth_mac(oc_no, src_dst, mac);
    
    
    return 0;
}

int
cli_get_eth_mac(struct parser_params *ppar, int oc_no)
{
    unsigned char src_mac[6], dst_mac[6];
    
    cli_db_lock(ppar);
	
    if (db_get_eth_mac(&ppar->db, oc_no, 0, src_mac))
    {
        errprint("Can't get ether source mac address from database.\n");
        cli_db_unlock(ppar);
        return -1;
    }
	
    if (db_get_eth_mac(&ppar->db, oc_no, 1, dst_mac))
    {
        errprint("Can't get ether dest mac address from database.\n");
        cli_db_unlock(ppar);
        return -1;      
    }
	
    cli_db_unlock(ppar);
	
    /* print header */
    printf("\n");
    printf("     ===================================================\n");
    printf("            Source MAC            Dest MAC\n");
    printf("     ===================================================\n");
	
    printf("     %02x:%02x:%02x:%02x:%02x:%02x   %02x:%02x:%02x:%02x:%02x:%02x\n",
            src_mac[0], src_mac[1], src_mac[2], src_mac[3], src_mac[4], src_mac[5],
            dst_mac[0], dst_mac[1], dst_mac[2], dst_mac[3], dst_mac[4], dst_mac[5]);
    
    printf("     ---------------------------------------------------\n\n");
	
	
    return 0;
}


int cli_set_gtp_dest_ports(struct parser_params *ppar, int oc_no, int gtpdestports)
{
     cli_db_lock(ppar);
	
    /* write to database */
    if (db_set_gtp_dest_ports(&ppar->db, oc_no, (unsigned long)gtpdestports) < 0)
    {
        errprint("Failed to set gtp dest ports in database.\n");
        cli_db_unlock(ppar);
        return -1;
    }
	
    cli_db_unlock(ppar);
    
    
    drv_set_gtp_dest_ports(oc_no, gtpdestports);
    
    
    return 0;
}

int cli_set_gtp_hash_mode(struct parser_params *ppar, int oc_no, int out_both)
{
    cli_db_lock(ppar);
	
    /* write to database */
    if (db_set_gtp_hash_mode(&ppar->db, oc_no, (unsigned long)out_both) < 0)
    {
        errprint("Failed to set eth mac in database.\n");
        cli_db_unlock(ppar);
        return -1;
    }
	
    cli_db_unlock(ppar);
    
    
    drv_set_gtp_hash_mode(oc_no, out_both);
    
    
    return 0;
}


int cli_get_gtp_settings(struct parser_params *ppar, int oc_no)
{
    unsigned long gtpdestports, out_both;
    
    cli_db_lock(ppar);
	
    if (db_get_gtp_dest_ports(&ppar->db, oc_no, &gtpdestports))
    {
        errprint("Can't get ether source mac address from database.\n");
        cli_db_unlock(ppar);
        return -1;
    }
	
    if (db_get_gtp_hash_mode(&ppar->db, oc_no, &out_both))
    {
        errprint("Can't get ether dest mac address from database.\n");
        cli_db_unlock(ppar);
        return -1;      
    }
	
    cli_db_unlock(ppar);
	
    /* print header */
    printf(PROMPT1("OC %d GTP dest ports:"), oc_no);
    printf(PROMPT2("", "%d\n"), (unsigned int)gtpdestports);
    printf(PROMPT1("OC %d GTP hash mode:"), oc_no);
    printf(PROMPT2("", "%s\n"), ((out_both == 0) ? "OUTER" : "BOTH"));	
	
	
    return 0;
}



/* functions not exported to user */ 
int
cli_fpga_reg_read(int oc_no, unsigned long reg_no)
{
  	unsigned int uData;
    
  	drv_fpga_reg_read(oc_no, (unsigned int) reg_no, &uData);

  	printf(PROMPT2("REG Data:", "0x%08x\n"), uData);
	
	
  	return 0;
}

int
cli_fpga_reg_write(int oc_no, unsigned long reg_no, unsigned long reg_data)
{
  	if (drv_fpga_reg_write(oc_no, (unsigned int)reg_no, (unsigned int)reg_data) != 0)
  	{ 
    	errprint("Error writting fpga register in parserops.c\n");
      	return -1;
  	}
	
	
  	return 0;
}

int
cli_get_fpga_stats(int oc_no)
{
    if (drv_get_fpga_stats(oc_no) != 0)
  	{ 
    	errprint("Error get fpga stats\n");
      	return -1;
  	}
    return 0;
}
int
cli_get_packet_stats()
{
    if (drv_get_packet_stats() != 0)
  	{ 
    	errprint("Error get fpga stats\n");
      	return -1;
  	}
	
    return 0;
}

int 
cli_get_framer_running_cfg(int oc_no)
{
	if (drv_get_framer_running_cfg(oc_no) != 0)
	{
	errprint("Error get framer stats\n");
      	return -1;
  	}
	
    return 0;   
}


int
cli_get_framer_stats(int oc_no)
{
    if (drv_get_framer_stats(oc_no) != 0)
  	{ 
    	errprint("Error get framer stats\n");
      	return -1;
  	}
	
    return 0;   
}

int
cli_palermo_get_stats(int oc_no)
{
    if (drv_get_palermo_stats(oc_no) != 0)
  	{ 
    	errprint("Error get framer stats\n");
      	return -1;
  	}
	
    return 0;   
}


int 
cli_fpga_set_flow_enable(struct parser_params *ppar, int oc_no, unsigned long enable_or_disable)
{
	cli_db_lock(ppar);

    /* write to database */
    if (db_set_flow_enable(&ppar->db, oc_no, enable_or_disable) < 0)
    {
        errprint("Failed to set flow function in database.\n");
        cli_db_unlock(ppar);
        return -1;
    }

    cli_db_unlock(ppar);
    
    
    drv_set_flow_enable(oc_no, enable_or_disable);
    
    
    return 0;
}

int 
cli_fpga_get_flow_enable(struct parser_params *ppar, int oc_no)
{
	unsigned long enable_or_disable;
	
	cli_db_lock(ppar);
	
    /* write to database */
    if (db_get_flow_enable(&ppar->db, oc_no, &enable_or_disable) < 0)
    {
        errprint("Failed to get flow function in database.\n");
        cli_db_unlock(ppar);
        return -1;
    }

    cli_db_unlock(ppar);
    
    printf(PROMPT1("OC %d flow function:"), oc_no);
    printf(PROMPT2("", "%s\n"), (enable_or_disable ? "enable" : "disable"));
   	
    return 0;
}

int 
cli_fpga_set_flow_mms(struct parser_params *ppar, int oc_no, unsigned long mms_size)
{
	cli_db_lock(ppar);

    /* write to database */
    if (db_set_flow_mms(&ppar->db, oc_no, mms_size) < 0)
    {
        errprint("Failed to set flow function in database.\n");
        cli_db_unlock(ppar);
        return -1;
    }
	
    cli_db_unlock(ppar);
    
    
    drv_set_flow_mms(oc_no, mms_size);
    
    
    return 0;
}

int 
cli_fpga_get_flow_mms(struct parser_params *ppar, int oc_no)
{
	unsigned long mms_size;
	
	cli_db_lock(ppar);
	
    /* write to database */
    if (db_get_flow_mms(&ppar->db, oc_no, &mms_size) < 0)
    {
        errprint("Failed to get flow function in database.\n");
        cli_db_unlock(ppar);
        return -1;
    }
	
    cli_db_unlock(ppar);
    
    printf(PROMPT1("OC %d flow tcp mms:"), oc_no);
    printf(PROMPT2("", "%d\n"), (int)mms_size);
   	
    return 0;
}


int 
cli_fpga_set_traffic_enable(struct parser_params *ppar, int oc_no, unsigned long enable_or_disable)
{
	cli_db_lock(ppar);

    /* write to database */
    if (db_set_traffic_enable(&ppar->db, oc_no, enable_or_disable) < 0)
    {
        errprint("Failed to set traffic enable in database.\n");
        cli_db_unlock(ppar);
        return -1;
    }

    cli_db_unlock(ppar);
    
    
    drv_set_traffic_enable(oc_no, enable_or_disable);
    
    
    return 0;
}

int 
cli_fpga_get_traffic_enable(struct parser_params *ppar, int oc_no)
{
	unsigned long enable_or_disable;
	
	cli_db_lock(ppar);
	
    /* write to database */
    if (db_get_traffic_enable(&ppar->db, oc_no, &enable_or_disable) < 0)
    {
        errprint("Failed to get traffic enable in database.\n");
        cli_db_unlock(ppar);
        return -1;
    }
	
    cli_db_unlock(ppar);
    
    printf(PROMPT1("OC %d traffic:"), oc_no);
    printf(PROMPT2("", "%s\n"), (enable_or_disable ? "enable" : "disable"));
   	
   	
    return 0;
}


// these functions are calling bcm sdk functions
// it's a lit bit complicated, as each time it will connect and close
// we will change it latter
int
cli_bcm_cmds(struct parser_params *ppar, unsigned char *cmds)
{
    // calling the real functions
    drv_set_bcm_cmds(&ppar->cs, cmds);
    
    return 0;   
}

int
cli_bcm_cmds_v2(struct parser_params *ppar, bcm_cmds_t *opts)
{
    unsigned char cmds[MAX_TTY_BUF_SIZE];

    //printf("inside cli_bcm_cmds_v2, cmds is: %s\n", opts->cmds);
    strcpy(cmds, opts->cmds);
    drv_set_bcm_cmds(&ppar->cs, cmds);
    
    return 0;
}


/***************************************************************************************/
/* BYPASS functions */
/***************************************************************************************/
int 
cli_bypass_get(struct parser_params *ppar)
{
    drv_bypass_get();
    
    return 0;
}

int 
cli_bypass_set(struct parser_params *ppar, int pbmp, unsigned long enable_or_disable)
{
    drv_bypass_set(&ppar->cs, pbmp, enable_or_disable);
    
    return 0;
}
