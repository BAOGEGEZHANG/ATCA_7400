/*
 *                          Copyright QiCe Tech, 2007-2010
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
#include <ctype.h>
#include "database.h"
#include "parserops.h"
#include "utils/errprint.h"
#include "db_client/db_client.h"




/*******************************************************************
 *
 * Description:     Function prototypes
 *
 *******************************************************************/
static int db_reset_filter_default_conf(DATABASE *pdb);
static int db_reset_forward_default_conf(DATABASE *pdb);
static int db_reset_global_default_conf(DATABASE *pdb);


/*******************************************************************
 *
 * Description:     Static (global) variables
 *
 *******************************************************************/


/******************************************************************
 *** Database schema
 ***
 *** <nnn>      Replace with a number. When used to find individual
 ***            instances of a specific entry (eg, Static.L2.<nnn>), it
 ***            is zero based.
 *** 
 *** Types
 *** Scal       Scalar value, int/unsigned...
 *** Str        String value
 *** Blob       Binary data
 *** 
 *** Key------------------------------  Type    Desription
 *** DB.Init                            Scal    If database has been initialized
 ***
 *** Port.Enable.Num                    Scal    Port enabled number
 *** Port.<nnn>.Enabled                 Scal    Port enabled or not
 *** Port.<nnn>.MaxFrameSize            Scal    Max frame size this port accepted
 *** Port.<nnn>.Padding                 Scal    Padding or CRC adding enabled or not
 *** Port.<nnn>.Dropbadframes           Scal    Drop bad frames or not
 ***
 *** Filter.Enabled                     Scal    Filter enabled or not
 *** Filter.No                          Scal    Number of filter rules
 *** Filter.<nnn>.Protocol              Scal    Filter Protocol value
 *** Filter.<nnn>.Srcip                 Scal    Source IP address
 *** Filter.<nnn>.Srcip_mask            Scal    Source IP address 
 *** Filter.<nnn>.Dstip                 Scal    Dest IP address
 *** Filter.<nnn>.Dstip_mask            Scal    Dest IP address 
 *** Filter.<nnn>.Srcl4port             Scal    Source l4 port 
 *** Filter.<nnn>.Dstl4port             Scal    Dest l4 port
 *** Filter.<nnn>.Flag                  Scal    Flag
 ***
 *** Forward.Mode.%d                    Scal    Forward mode
 ***
 *** Transfer.Mode                      Scal    Transfer mode
 ***
 *** OC.Line                            Scal    OC encapsulate 
 *** OC.Encap                           Scal    OC encapsulate
 *** OC.Sonnet_scramble                 Scal    OC Sonnet scramble enabled or not
 *** OC.Payload_scramble                Scal    OC Payload scramble enabled or not
 *** OC.CrcMode                         Scal    OC CRC mode
 ***
 *** Framer.Stats                       Blob    Statistics for Framer channel nnn
 *** Fpga.Stats                         Blob    Statistics for Fpga
 *** Mac.Stats                          Blob    Statistics for Mac
 *******************************************************************/

int
db_open(DATABASE *pdb)
{
    if (db_cl_open(pdb))
    {
        errprint("Can't open database.\n");
        return -1;
    }

    return 0;
}

int
db_init_config(DATABASE *pdb)
{
    int i, oc_no;
    unsigned char src_mac[6], dst_mac[6];
    
#ifdef BUILD_MC10P_FLOW    
    FILE *fp;
    confs_t cfg;
    char tmps[256];
    char *options, *keys;
    unsigned int value0, value1;
#endif
    
    /* Mark database as not initialized yet */
    if (db_cl_insert_ul(pdb, "DB.Init", 0))
    {
        errprint("Can't insert key '%s'.\n", "DB.Init");
        return -1;
    }
    
    /* create per filter entries */
    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no++)
    {
        sprintf(pdb->key_buf, "Filter1.%d.No", oc_no);
        if (db_cl_insert_ul(pdb, pdb->key_buf, FILTER_NO_DEF))
        {
            errprint("Can't insert key '%s'.\n", pdb->key_buf);
            return -1;
        }
        
	 sprintf(pdb->key_buf, "Filter2.%d.No", oc_no);
        if (db_cl_insert_ul(pdb, pdb->key_buf, FILTER_NO_DEF))
        {
            errprint("Can't insert key '%s'.\n", pdb->key_buf);
            return -1;
        }	
    }

   
    /* transfer related */
    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no++)
    {
        src_mac[0] = 0x00;
        src_mac[1] = 0x01;
        src_mac[2] = 0x2f;
        src_mac[3] = 0xff;
        src_mac[4] = 0x12;
        src_mac[5] = 0x00 | oc_no;
        sprintf(pdb->key_buf, "Src.MAC.%d", oc_no);
        if (db_cl_insert_blob(pdb, pdb->key_buf, src_mac, 6))
        {
            errprint("Can't insert key '%s'.\n", pdb->key_buf);
            return -1;
        }
        
        dst_mac[0] = 0x00;
        dst_mac[1] = 0x01;
        dst_mac[2] = 0x2f;
        dst_mac[3] = 0xff;
        dst_mac[4] = 0x13;
        dst_mac[5] = 0x50 | oc_no;
        sprintf(pdb->key_buf, "Dst.MAC.%d", oc_no);
        if (db_cl_insert_blob(pdb, pdb->key_buf, dst_mac, 6))
        {                                                                                                                       
            errprint("Can't insert key '%s'.\n", pdb->key_buf);
            return -1;
        }
    }		
    
    
    /* flow function */
    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no++)
    {
        sprintf(pdb->key_buf, "FPGA.Flow.%d", oc_no);
	 if (db_cl_insert_ul(pdb, pdb->key_buf, ENABLED))
        {
            errprint("Can't insert key '%s'.\n", pdb->key_buf);
            return -1;
        }
    }
    
    
    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no++)
    {
        sprintf(pdb->key_buf, "FPGA.Flow.MMS.%d", oc_no);
        if (db_cl_insert_ul(pdb, pdb->key_buf, TCP_MMS))
        {
            errprint("Can't insert key '%s'.\n", pdb->key_buf);
            return -1;
        }
    }
    
    
    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no++)
    {
        sprintf(pdb->key_buf, "FPGA.Traffic.%d", oc_no);
        if (db_cl_insert_ul(pdb, pdb->key_buf, ENABLED))
        {
            errprint("Can't insert key '%s'.\n", pdb->key_buf);
            return -1;
        }
    }
    
    
    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no++)
    	for (i = 0 ; i < 8; i++)
    	{
    		sprintf(pdb->key_buf, "PKTLEN.RANGE.MIN.%d.%d", oc_no, i);
    		if (db_cl_insert_ul(pdb, pdb->key_buf, PKTLEN_MIN))
    		{
        		errprint("Can't insert key '%s'.\n", pdb->key_buf);
    	    	return -1;
    		}
    		sprintf(pdb->key_buf, "PKTLEN.RANGE.MAX.%d.%d", oc_no, i);
    		if (db_cl_insert_ul(pdb, pdb->key_buf, PKTLEN_MAX))
    		{
        		errprint("Can't insert key '%s'.\n", pdb->key_buf);
        		return -1;
    		}
	}
    
    
    // default sample rate is 100%
    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no++)
    {
        sprintf(pdb->key_buf, "FPGA.SAMPLE.%d", oc_no);
        if (db_cl_insert_ul(pdb, pdb->key_buf, SAMPLE_RATE))
        {
            errprint("Can't insert key '%s'.\n", pdb->key_buf);
            return -1;
        }
    }
    
#ifdef BUILD_MC10P_FLOW    
    /* set oc parameters */
    if((fp = fopen(DRV_CONF_FILE,"r")) == NULL)
    {
        errprint("Can't save oc settings while writing to file.\n");
        return -1;
    }
    
    while(fgets(tmps, 256, fp))
    {
        options = strtok(tmps, " ");
	keys = strtok(NULL, " ");
	if ((!options) || (!keys))
        {
            break;
        }
	sscanf(keys, "%u,%u", &value0, &value1);
		
	if (strncmp("LINE_RATE",options,9)==0)
	{
            cfg.configs[0].line_rate = value0;
            cfg.configs[1].line_rate = value1;
	}
	else if (strncmp("SONET_MODE",options,10)==0)
	{
            cfg.configs[0].frame = value0;
            cfg.configs[1].frame = value1;
	}
	else if (strncmp("PPP_MODE",options,8)==0)
	{
            cfg.configs[0].encap = value0;
            cfg.configs[1].encap = value1;
	}
        else if (strncmp("SONET_SCRAMBLE",options,14)==0)
        {
            cfg.configs[0].framer_scramble = value0;
            cfg.configs[1].framer_scramble = value1;
	}		
        else if (strncmp("PAYLOAD_SCRAMBLE",options,16)==0)
        {
            cfg.configs[0].mapper_scramble = value0;
            cfg.configs[1].mapper_scramble = value1;
	}		
        else if (strncmp("CRC32_MODE",options,10)==0)
        {
            cfg.configs[0].fcs = value0;
            cfg.configs[1].fcs = value1;
	}		
        else if (strncmp("PASS_CRC",options,8)==0)
        {
            cfg.configs[0].pass_crc = value0;
            cfg.configs[1].pass_crc = value1;
	}
        else if (strncmp("MAX_PKT_SIZE",options,12)==0)
        {
            cfg.configs[0].max_pkt_size = value0;
            cfg.configs[1].max_pkt_size = value1;
	}
        else if (strncmp("MIN_PKT_SIZE",options,12)==0)
        {
            cfg.configs[0].min_pkt_size = value0;
            cfg.configs[1].min_pkt_size = value1;
	}
        else
        {
            printf("unsupported configs.\n");
    	}
    } // while
    
    if (fclose(fp))
    {
        errprint("Meet some problems when closing the drvconf.ini file!\n");
        return -1;
    }
	
	
    for (i=0; i < MAX_OC_NUM; i++)
    {
        sprintf(pdb->key_buf, "OC.LineRate.%d", i);
        if (db_cl_insert_ul(pdb, pdb->key_buf, cfg.configs[i].line_rate))
        {
            errprint("Can't insert key '%s'.\n", pdb->key_buf);
            return -1;
        }
        
        sprintf(pdb->key_buf, "OC.Line.%d", i);
        if (db_cl_insert_ul(pdb, pdb->key_buf, cfg.configs[i].frame))
        {
            errprint("Can't insert key '%s'.\n", pdb->key_buf);
            return -1;
        }
		
        sprintf(pdb->key_buf, "OC.Encap.%d", i);
        if (db_cl_insert_ul(pdb, pdb->key_buf, cfg.configs[i].encap))
        {
            errprint("Can't insert key '%s'.\n", pdb->key_buf);
            return -1;
        }
    	
        sprintf(pdb->key_buf, "OC.Sonnet_scramble.%d", i);
        if (db_cl_insert_ul(pdb, pdb->key_buf, cfg.configs[i].framer_scramble))
        {
            errprint("Can't insert key '%s'.\n", pdb->key_buf);
            return -1;
        }
    	
        sprintf(pdb->key_buf, "OC.Payload_scramble.%d", i);
        if (db_cl_insert_ul(pdb, pdb->key_buf, cfg.configs[i].mapper_scramble))
        {
            errprint("Can't insert key '%s'.\n", pdb->key_buf);
            return -1;
        }
    	
        sprintf(pdb->key_buf, "OC.CrcMode.%d", i);
        if (db_cl_insert_ul(pdb, pdb->key_buf, cfg.configs[i].fcs))
        {
            errprint("Can't insert key '%s'.\n", pdb->key_buf);
            return -1;
        }
        
        sprintf(pdb->key_buf, "OC.PassCrc.%d", i);
        if (db_cl_insert_ul(pdb, pdb->key_buf, cfg.configs[i].pass_crc))
        {
            errprint("Can't insert key '%s'.\n", pdb->key_buf);
            return -1;
        }
        
        sprintf(pdb->key_buf, "OC.MAXPKTSIZE.%d", i);
        if (db_cl_insert_ul(pdb, pdb->key_buf, cfg.configs[i].max_pkt_size))
        {
            errprint("Can't insert key '%s'.\n", pdb->key_buf);
            return -1;
        }
        
        sprintf(pdb->key_buf, "OC.MINPKTSIZE.%d", i);
        if (db_cl_insert_ul(pdb, pdb->key_buf, cfg.configs[i].min_pkt_size))
        {
            errprint("Can't insert key '%s'.\n", pdb->key_buf);
            return -1;
        }
    }   
#endif


    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no++)
    	for (i = 0 ; i < MAX_PKT_RANGE; i++)
    	{
    		sprintf(pdb->key_buf, "PKTLEN.RANGE.MIN.%d.%d", oc_no, i);
    		if (db_cl_insert_ul(pdb, pdb->key_buf, PKTLEN_MIN))
    		{
        		errprint("Can't insert key '%s'.\n", pdb->key_buf);
    	    		return -1;
    		}
    		sprintf(pdb->key_buf, "PKTLEN.RANGE.MAX.%d.%d", oc_no, i);
    		if (db_cl_insert_ul(pdb, pdb->key_buf, PKTLEN_MIN))
    		{
        		errprint("Can't insert key '%s'.\n", pdb->key_buf);
        		return -1;
    		}
	}


    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no++)
    {
    		sprintf(pdb->key_buf, "GTP.DestPorts.%d", oc_no);
    		if (db_cl_insert_ul(pdb, pdb->key_buf, 24))
    		{
        		errprint("Can't insert key '%s'.\n", pdb->key_buf);
    	    		return -1;
    		}
    		sprintf(pdb->key_buf, "GTP.HashMode.%d", oc_no);
    		if (db_cl_insert_ul(pdb, pdb->key_buf, 0x1))
    		{
        		errprint("Can't insert key '%s'.\n", pdb->key_buf);
        		return -1;
    		}
    }	
    
    return 0;
}

int 
db_close(DATABASE *pdb)
{
    if (db_cl_close(pdb) < 0)
    {
        errprint("Error closing db session.\n");
        return -1;
    }
    
    return 0;
}

int
db_dump_blob(DATABASE *pdb, struct hash_table_datum *entry)
{
    int i;
    int j;
    int breaks;
    unsigned char *p;
    unsigned char *q;

    printf("BLOB size: %d\n", entry->data_length);
    printf("      %40s  ", "");
    /* Do hex dump */
    breaks = 0;
    p = q = (char *)entry->data;
    j = 0;
    for (i = 0; i < entry->data_length; i++)
    {
        printf("%02x ", *p++);
        if (++breaks % 8 == 0)
        {
            printf("- |");
            do
            {
                if (isprint(*q))
                {
                    printf("%c", *q);
                }
                else
                {
                    printf(".");
                }
                q++;
                j++;
            } while ((j % 8 != 0) && (j < entry->data_length));
            if (j < entry->data_length)
            {
                printf("|\n      %40s  ", "");
            }
            else
            {
                printf("|\n");
            }
        }
    }
    while (i++ % 8)
    {
        printf("   ");
    }
    if (j < entry->data_length)
    {
        printf("- |");
        while (j < entry->data_length)
        {
            if (isprint(*q))
            {
                printf("%c", *q);
            }
            else
            {
                printf(".");
            }
            q++;
            j++;
        }
        printf("|\n");
    }
	
    return 0;
}

int
db_dump(DATABASE *pdb, char *pattern)
{
    struct hash_table_datum *entry;
    db_cl_search_state_t state;
    int keys = 0;
    int data_size = 0;
    int key_size = 0;
    int tot_size = 0;

    DB_CL_LOCK(pdb);
    
    entry = db_cl_get_first(pdb, pattern, &state);
    while(entry != NULL)
    {
        ++keys;
        data_size += entry->data_length;
        key_size += entry->key.size;
        tot_size += entry->data_length + entry->key.size + sizeof(struct hash_table_datum);
        printf("      %-40s: ", (char *)(entry->key.data));
        if (entry->data_length == sizeof(unsigned long))
        {
            printf("0x%08lx\n", *((unsigned long *)entry->data));
        }
        else
        {
            db_dump_blob(pdb, entry);
        }

        HASH_FREE(entry);
        entry = db_cl_get_next(pdb, pattern, &state);
    }
    printf("Number of keys: %d\n", keys);
    printf("Tot size: %d, Key size: %d, Data size: %d\n", tot_size, key_size, data_size);
    
    DB_CL_UNLOCK(pdb);
    
    return 0;
}


int
db_initialized(DATABASE *pdb)
{
    int val;
	
    sprintf(pdb->key_buf, "DB.Init");
    if (db_cl_get_ul(pdb, pdb->key_buf, (unsigned long *)&val))
    {
        return 0;
    }
	
    return val;
}


int
db_set_initialized(DATABASE *pdb)
{
    sprintf(pdb->key_buf, "DB.Init");
    if (db_initialized(pdb))
    {
        errprint("db_set_initialized called twice!\n");
        return -1;
    }
    
    if (db_cl_insert_ul(pdb, pdb->key_buf, 1))
    {
        return 0;
    }
	
    return 0;
}


int 
db_reset_default_config(DATABASE *pdb)
{
    if (db_reset_filter_default_conf(pdb) < 0)
    {
        errprint("Can't reset filter default config in database.\n");
        return -1;
    }
	
    if (db_reset_forward_default_conf(pdb) < 0)
    {
        errprint("Can't reset forward default config in database.\n");
        return -1;
    }
	
    if (db_reset_global_default_conf(pdb) < 0)
    {
        errprint("Can't reset forward default config in database.\n");
        return -1;
    }
	
    return 0;
}


static int 
db_reset_filter_default_conf(DATABASE *pdb)
{
    unsigned long oc_no;  //for framer 1 or 2
    unsigned long id;
    unsigned long id_matched;
    unsigned long number;
    
    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no++)
    {
        /* deal with ruleset1 */
        if(db_get_filter1_rule_num(pdb, oc_no, &number))
        {
            errprint("Can't get filter rule number in database.\n");
            return -1;
        }
    	
        for(id_matched = 0, id = 0; id_matched < number; id++)
        {
            if(db_filter1_rule_exist(pdb, oc_no, id))
            {
                id_matched ++;
            
                if(db_delete_filter1_rule(pdb, oc_no, id))
                {
                    errprint("Can't delete filter rule from database.\n");
                    return -1;
                }
            }
        }
    	
        if(db_set_filter1_rule_num(pdb, oc_no, (unsigned long)FILTER_NO_DEF))
        {
            errprint("Can't set filter rule number in database.\n");
            return -1;
        }

        /* deal with ruleset2 */
        if(db_get_filter2_rule_num(pdb, oc_no, &number))
        {
            errprint("Can't get filter rule number in database.\n");
            return -1;
        }
    	
        for(id_matched = 0, id = 0; id_matched < number; id++)
        {
            if(db_filter2_rule_exist(pdb, oc_no, id))
            {
                id_matched ++;
            	
                if(db_delete_filter2_rule(pdb, oc_no, id))
                {
                    errprint("Can't delete filter rule from database.\n");
                    return -1;
                }
            }
        }
    	
        if(db_set_filter2_rule_num(pdb, oc_no, (unsigned long)FILTER_NO_DEF))
        {
            errprint("Can't set filter rule number in database.\n");
            return -1;
        }
    }
	
    return 0;
}


static int 
db_reset_forward_default_conf(DATABASE *pdb)
{
    int i,j;
    unsigned long oc_no;
    
    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no++)
    {
        for (i = 0; i < MAX_RULE_NO; i++)
            for (j = 0; j < MAX_RULE_NO; j++)
            {
                if(db_forward_mode_exist(pdb, oc_no, (unsigned long)i, (unsigned long)j))
                    db_delete_forward_mode(pdb, oc_no, (unsigned long)i, (unsigned long)j);
            }
    }
    
    return 0;
}


static int 
db_reset_global_default_conf(DATABASE *pdb)
{
    int i;
    unsigned long oc_no;
    
    for (oc_no = 0; oc_no < MAX_OC_NUM; oc_no++)
    {    
       if (db_set_flow_enable(pdb, oc_no, (unsigned long)ENABLED))
       {
        	errprint("Can't set flow to default.\n");
		return -1;
       }

 	for (i = 0; i < 8 ; i++)
 	{
		if (db_set_pktlen_range(pdb, oc_no, i, (unsigned long)PKTLEN_MIN, (unsigned long)PKTLEN_MAX))
       	{
        		errprint("Can't set packet length range to default.\n");
			return -1;
        	}
 	}	
	
	if (db_set_flow_mms(pdb, oc_no, (unsigned long)TCP_MMS))
       {
        	errprint("Can't set flow mms default.\n");
		return -1;
       }	

       if (db_set_sample_rate(pdb, oc_no, (unsigned long)SAMPLE_RATE))
       {
        	errprint("Can't set packet sample rate default.\n");
		return -1;
       }	   

	for (i = 0; i < 12 ; i++)
 	{
		if (db_set_pktlen_range(pdb, oc_no, i, (unsigned long)PKTLEN_MIN, (unsigned long)PKTLEN_MAX))
       	{
        		errprint("Can't set packet length range to default.\n");
			return -1;
        	}
 	}
    	
       if (db_set_traffic_enable(pdb, oc_no, (unsigned long)ENABLED))
       {
        	errprint("Can't set traffic to default.\n");
		return -1;
       }

	if (db_set_gtp_dest_ports(pdb, oc_no, (unsigned long)24))
       {
        	errprint("Can't set traffic to default.\n");
		return -1;
       }
	
	if (db_set_gtp_hash_mode(pdb, oc_no, (unsigned long)0x1))
       {
        	errprint("Can't set traffic to default.\n");
		return -1;
       }
    }
    
    return 0;
}


/* filter functions */
int 
db_filter1_rule_exist(DATABASE *pdb, unsigned long oc_no, unsigned long ruleno)
{   
    db_cl_set_key_buf(pdb, "Filter1.%d.%d.Flag", oc_no, ruleno);
    return db_cl_key_exists(pdb, pdb->key_buf);         
}

int 
db_filter2_rule_exist(DATABASE *pdb, unsigned long oc_no, unsigned long ruleno)
{   
    db_cl_set_key_buf(pdb, "Filter2.%d.%d.Flag", oc_no, ruleno);
    return db_cl_key_exists(pdb, pdb->key_buf);         
}

int 
db_get_filter1_rule_num(DATABASE *pdb, unsigned long oc_no, unsigned long *rule_num)
{
    db_cl_set_key_buf(pdb, "Filter1.%d.No", oc_no);
    DB_CL_GET_UL(pdb, rule_num);
    
    return 0;
}

int 
db_get_filter2_rule_num(DATABASE *pdb, unsigned long oc_no, unsigned long *rule_num)
{
    db_cl_set_key_buf(pdb, "Filter2.%d.No", oc_no);
    DB_CL_GET_UL(pdb, rule_num);
    
    return 0;
}

int 
db_set_filter1_rule_num(DATABASE *pdb, unsigned long oc_no, unsigned long rule_num)
{
    db_cl_set_key_buf(pdb, "Filter1.%d.No", oc_no);
    DB_CL_INSERT_UL(pdb, rule_num);
    
    return 0;
}

int 
db_set_filter2_rule_num(DATABASE *pdb, unsigned long oc_no, unsigned long rule_num)
{
    db_cl_set_key_buf(pdb, "Filter2.%d.No", oc_no);
    DB_CL_INSERT_UL(pdb, rule_num);
    
    return 0;
}

int 
db_set_filter1_rule(DATABASE *pdb, unsigned long oc_no, rule1options_t *opts)
{
    db_cl_set_key_buf(pdb, "Filter1.%d.%d.Flag", oc_no, opts->id);
    DB_CL_INSERT_UL(pdb, opts->flag);

    if(opts->flag & RULE_OPT_PROTOCOL)
    {
        db_cl_set_key_buf(pdb, "Filter1.%d.%d.Protocol", oc_no, opts->id);
        DB_CL_INSERT_UL(pdb, opts->protocol);       
    }

    if(opts->flag & RULE_OPT_SRCIP)
    {
        db_cl_set_key_buf(pdb, "Filter1.%d.%d.Srcip", oc_no, opts->id);
        DB_CL_INSERT_UL(pdb, opts->sourceip);

        db_cl_set_key_buf(pdb, "Filter1.%d.%d.Srcip_mask", oc_no, opts->id);
        DB_CL_INSERT_UL(pdb, opts->sourceip_mask);      
    }

    if(opts->flag & RULE_OPT_DESTIP)
    {
        db_cl_set_key_buf(pdb, "Filter1.%d.%d.Dstip", oc_no, opts->id);
        DB_CL_INSERT_UL(pdb, opts->destip);

        db_cl_set_key_buf(pdb, "Filter1.%d.%d.Dstip_mask", oc_no, opts->id);
        DB_CL_INSERT_UL(pdb, opts->destip_mask);        
    }

    if(opts->flag & RULE_OPT_SRCL4PORT)
    {
        db_cl_set_key_buf(pdb, "Filter1.%d.%d.Srcl4port", oc_no, opts->id);
        DB_CL_INSERT_UL(pdb, opts->sourcel4port);
    }

    if(opts->flag & RULE_OPT_DESTL4PORT)
    {
        db_cl_set_key_buf(pdb, "Filter1.%d.%d.Dstl4port", oc_no, opts->id);
        DB_CL_INSERT_UL(pdb, opts->destl4port);
    } 

    if(opts->flag & RULE_OPT_PACKETLEN)
    {
        db_cl_set_key_buf(pdb, "Filter1.%d.%d.Packetlen_range", oc_no, opts->id);
        DB_CL_INSERT_UL(pdb, opts->pktlen_range);
    }

    return 0;
}

int 
db_set_filter2_rule(DATABASE *pdb, unsigned long oc_no, rule2options_t *opts)
{
    db_cl_set_key_buf(pdb, "Filter2.%d.%d.Flag", oc_no, opts->id);
    DB_CL_INSERT_UL(pdb, opts->flag);

    if(opts->flag & RULE_OPT_OFFSET)
    {
        db_cl_set_key_buf(pdb, "Filter2.%d.%d.Offset", oc_no, opts->id);
        DB_CL_INSERT_UL(pdb, opts->offset);
    }   

    if(opts->flag & RULE_OPT_KEY)
    {
        db_cl_set_key_buf(pdb, "Filter2.%d.%d.Key", oc_no, opts->id);
        DB_CL_INSERT_BLOB(pdb, opts->key, MAX_KEY_NO);

        db_cl_set_key_buf(pdb, "Filter2.%d.%d.Mask", oc_no, opts->id);
        DB_CL_INSERT_BLOB(pdb, opts->mask, MAX_KEY_NO);
    }           

    return 0;
}

int 
db_get_filter1_rule(DATABASE *pdb, unsigned long oc_no, rule1options_t *opts)
{
    unsigned long result;

    db_cl_set_key_buf(pdb, "Filter1.%d.%d.Flag", oc_no, opts->id);
    DB_CL_GET_UL(pdb, &result);
    opts->flag = result;

    if(opts->flag & RULE_OPT_PROTOCOL)
    {
        db_cl_set_key_buf(pdb, "Filter1.%d.%d.Protocol", oc_no, opts->id);
        DB_CL_GET_UL(pdb, &result); 
        opts->protocol = (unsigned char)result;
    }

    if(opts->flag & RULE_OPT_SRCIP)
    {
        db_cl_set_key_buf(pdb, "Filter1.%d.%d.Srcip", oc_no, opts->id);
        DB_CL_GET_UL(pdb, &result);
        opts->sourceip = result;

        db_cl_set_key_buf(pdb, "Filter1.%d.%d.Srcip_mask", oc_no, opts->id);
        DB_CL_GET_UL(pdb, &result);
        opts->sourceip_mask = result;
    }

    if(opts->flag & RULE_OPT_DESTIP)
    {
        db_cl_set_key_buf(pdb, "Filter1.%d.%d.Dstip", oc_no, opts->id);
        DB_CL_GET_UL(pdb, &result);
        opts->destip = result;

        db_cl_set_key_buf(pdb, "Filter1.%d.%d.Dstip_mask", oc_no, opts->id);
        DB_CL_GET_UL(pdb, &result);
        opts->destip_mask = result;
    }
	
    if(opts->flag & RULE_OPT_SRCL4PORT)
    {
        db_cl_set_key_buf(pdb, "Filter1.%d.%d.Srcl4port", oc_no, opts->id);
        DB_CL_GET_UL(pdb, &result);
        opts->sourcel4port = (unsigned short)result;
    }
	
    if(opts->flag & RULE_OPT_DESTL4PORT)
    {
        db_cl_set_key_buf(pdb, "Filter1.%d.%d.Dstl4port", oc_no, opts->id);
        DB_CL_GET_UL(pdb, &result);
        opts->destl4port = (unsigned short)result;
    }
	
    if(opts->flag & RULE_OPT_PACKETLEN)
    {
        db_cl_set_key_buf(pdb,"Filter1.%d.%d.Packetlen_range", oc_no, opts->id);
        DB_CL_GET_UL(pdb, &result);
        opts->pktlen_range = (unsigned char)result;
    }
	
    return 0;   
}

int 
db_get_filter2_rule(DATABASE *pdb, unsigned long oc_no, rule2options_t *opts)
{
    unsigned long result;

    db_cl_set_key_buf(pdb, "Filter2.%d.%d.Flag", oc_no, opts->id);
    DB_CL_GET_UL(pdb, &result);
    opts->flag = result;

    if(opts->flag & RULE_OPT_OFFSET)
    {
        db_cl_set_key_buf(pdb, "Filter2.%d.%d.Offset", oc_no, opts->id);
        DB_CL_GET_UL(pdb, &result); 
        opts->offset = result;
    }

    if(opts->flag & RULE_OPT_KEY)
    {
        db_cl_set_key_buf(pdb, "Filter2.%d.%d.Key", oc_no, opts->id);
        DB_CL_GET_BLOB(pdb, opts->key, MAX_KEY_NO);

        db_cl_set_key_buf(pdb, "Filter2.%d.%d.Mask", oc_no, opts->id);
        DB_CL_GET_BLOB(pdb, opts->mask, MAX_KEY_NO);
    }       

    return 0;
}

int 
db_delete_filter1_rule(DATABASE *pdb, unsigned long oc_no, unsigned long ruleno)
{
    unsigned long result;
    
    if(db_filter1_rule_exist(pdb, oc_no, ruleno))
    {
        db_cl_set_key_buf(pdb, "Filter1.%d.%d.Flag", oc_no, ruleno);
        DB_CL_GET_UL(pdb, &result);

        db_cl_set_key_buf(pdb, "Filter1.%d.%d.Flag", oc_no, ruleno);
        DB_CL_DELETE(pdb);

        if(result & RULE_OPT_PROTOCOL)
        {
            db_cl_set_key_buf(pdb, "Filter1.%d.%d.Protocol", oc_no, ruleno);
            DB_CL_DELETE(pdb);  
        }

        if(result & RULE_OPT_SRCIP)
        {
            db_cl_set_key_buf(pdb, "Filter1.%d.%d.Srcip", oc_no, ruleno);
            DB_CL_DELETE(pdb);

            db_cl_set_key_buf(pdb, "Filter1.%d.%d.Srcip_mask", oc_no, ruleno);
            DB_CL_DELETE(pdb);
        }

        if(result & RULE_OPT_DESTIP)
        {
            db_cl_set_key_buf(pdb, "Filter1.%d.%d.Dstip", oc_no, ruleno);
            DB_CL_DELETE(pdb);

            db_cl_set_key_buf(pdb, "Filter1.%d.%d.Dstip_mask", oc_no, ruleno);
            DB_CL_DELETE(pdb);
        }

        if(result & RULE_OPT_SRCL4PORT)
        {
            db_cl_set_key_buf(pdb, "Filter1.%d.%d.Srcl4port", oc_no, ruleno);
            DB_CL_DELETE(pdb);
        }

        if(result & RULE_OPT_DESTL4PORT)
        {
            db_cl_set_key_buf(pdb, "Filter1.%d.%d.Dstl4port", oc_no, ruleno);
            DB_CL_DELETE(pdb);
        }

        if(result & RULE_OPT_PACKETLEN)
        {
            db_cl_set_key_buf(pdb, "Filter1.%d.%d.Packetlen_range", oc_no, ruleno);
            DB_CL_DELETE(pdb);
        }
    }   

    return 0;
}

int 
db_delete_filter2_rule(DATABASE *pdb, unsigned long oc_no, unsigned long ruleno)
{
    unsigned long result;
    
    if(db_filter2_rule_exist(pdb, oc_no, ruleno))
    {
        db_cl_set_key_buf(pdb, "Filter2.%d.%d.Flag", oc_no, ruleno);
        DB_CL_GET_UL(pdb, &result);
		
        db_cl_set_key_buf(pdb, "Filter2.%d.%d.Flag", oc_no, ruleno);
        DB_CL_DELETE(pdb);
		
        if(result & RULE_OPT_OFFSET)
        {
            db_cl_set_key_buf(pdb, "Filter2.%d.%d.Offset", oc_no, ruleno);
            DB_CL_DELETE(pdb);
        }
		
        if(result & RULE_OPT_KEY)
        {
            db_cl_set_key_buf(pdb, "Filter2.%d.%d.Key", oc_no, ruleno);
            DB_CL_DELETE(pdb);

            db_cl_set_key_buf(pdb, "Filter2.%d.%d.Mask", oc_no, ruleno);
            DB_CL_DELETE(pdb);
        }
    }
	
    return 0;
}

/* forward functions */
int 
db_set_forward_mode(DATABASE *pdb, unsigned long oc_no, unsigned long ruleno1, unsigned long ruleno2, unsigned long forward_mode)
{
    db_cl_set_key_buf(pdb, "Forward.Mode.%d.%d.%d",oc_no, ruleno1, ruleno2);
    DB_CL_INSERT_UL(pdb, forward_mode);
    
    return 0;
}

int 
db_get_forward_mode(DATABASE *pdb, unsigned long oc_no, unsigned long ruleno1, unsigned long ruleno2, unsigned long *forward_mode)
{
    db_cl_set_key_buf(pdb, "Forward.Mode.%d.%d.%d", oc_no, ruleno1, ruleno2);
    DB_CL_GET_UL(pdb, forward_mode);
	
    return 0;
}

int
db_forward_mode_exist(DATABASE *pdb, unsigned long oc_no, unsigned long ruleno1, unsigned long ruleno2)
{
    db_cl_set_key_buf(pdb, "Forward.Mode.%d.%d.%d", oc_no, ruleno1, ruleno2);
    return db_cl_key_exists(pdb, pdb->key_buf); 
}

int 
db_delete_forward_mode(DATABASE *pdb, unsigned long oc_no, unsigned long ruleno1, unsigned long ruleno2)
{
    if(db_forward_mode_exist(pdb, oc_no, ruleno1, ruleno2))
    {
        db_cl_set_key_buf(pdb, "Forward.Mode.%d.%d.%d", oc_no, ruleno1, ruleno2);
        DB_CL_DELETE(pdb);
    }
	
    return 0;
}

/* transfer functions */
int
db_set_eth_mac(DATABASE *pdb, unsigned long oc_no, unsigned int src_dst, unsigned char mac[6])
{
    if (src_dst == 0)
    {
    	db_cl_set_key_buf(pdb, "Src.MAC.%d", oc_no);
        db_cl_insert_blob(pdb, pdb->key_buf, mac, 6);
    }
    else
    {
    	db_cl_set_key_buf(pdb, "Dst.MAC.%d", oc_no);
        db_cl_insert_blob(pdb, pdb->key_buf, mac, 6);
    }
    
    return 0;
}

int
db_get_eth_mac(DATABASE *pdb, unsigned long oc_no, unsigned int src_dst, unsigned char mac[6])
{
    if(src_dst == 0)
    {
    	db_cl_set_key_buf(pdb, "Src.MAC.%d", oc_no);
        db_cl_get_blob(pdb, pdb->key_buf, mac, 6);
    }
    else
    {
    	db_cl_set_key_buf(pdb, "Dst.MAC.%d", oc_no);
        db_cl_get_blob(pdb, pdb->key_buf, mac, 6);
	}
	
    return 0;
}


int
db_set_gtp_dest_ports(DATABASE *pdb, unsigned long oc_no, unsigned long gtpdestports)
{
    db_cl_set_key_buf(pdb, "GTP.DstPorts.%d", oc_no);
    DB_CL_INSERT_UL(pdb, gtpdestports);
    
    return 0;
}

int
db_get_gtp_dest_ports(DATABASE *pdb, unsigned long oc_no, unsigned long *gtpdestports)
{
    db_cl_set_key_buf(pdb, "GTP.DstPorts.%d", oc_no);
    DB_CL_GET_UL(pdb, gtpdestports);
	
    return 0;
}

int
db_set_gtp_hash_mode(DATABASE *pdb, unsigned long oc_no, unsigned long out_both)
{
    db_cl_set_key_buf(pdb, "GTP.HashMode.%d", oc_no);
    DB_CL_INSERT_UL(pdb, out_both);
    
    return 0;
}

int
db_get_gtp_hash_mode(DATABASE *pdb, unsigned long oc_no, unsigned long *out_both)
{
    db_cl_set_key_buf(pdb, "GTP.HashMode.%d", oc_no);
    DB_CL_GET_UL(pdb, out_both);
	
    return 0;
}


/* oc functions */
int 
db_set_oc_line_rate(DATABASE *pdb, unsigned long oc_no, unsigned long line_rate)
{
    db_cl_set_key_buf(pdb, "OC.LineRate.%d", oc_no);
    DB_CL_INSERT_UL(pdb, line_rate);
    
    return 0;   
}

int 
db_set_oc_line_mode(DATABASE *pdb, unsigned long oc_no, unsigned long line_mode)
{
    db_cl_set_key_buf(pdb, "OC.Line.%d", oc_no);
    DB_CL_INSERT_UL(pdb, line_mode);
    
    return 0;   
}

int 
db_set_oc_encap_mode(DATABASE *pdb, unsigned long oc_no, unsigned long encap_mode)
{
    db_cl_set_key_buf(pdb, "OC.Encap.%d", oc_no);
    DB_CL_INSERT_UL(pdb, encap_mode);
    
    return 0;   
}

int 
db_set_oc_sonnet_scramble_enable(DATABASE *pdb, unsigned long oc_no, unsigned long enable_or_disable)
{
    db_cl_set_key_buf(pdb, "OC.Sonnet_scramble.%d", oc_no);
    DB_CL_INSERT_UL(pdb, enable_or_disable);
    
    return 0;   
}

int 
db_set_oc_payload_scramble_enable(DATABASE *pdb, unsigned long oc_no, unsigned long enable_or_disable)
{
    db_cl_set_key_buf(pdb, "OC.Payload_scramble.%d", oc_no);
    DB_CL_INSERT_UL(pdb, enable_or_disable);
    
    return 0;       
}

int 
db_set_oc_crc_mode(DATABASE *pdb, unsigned long oc_no, unsigned long crc_mode)
{
    db_cl_set_key_buf(pdb, "OC.CrcMode.%d", oc_no);
    DB_CL_INSERT_UL(pdb, crc_mode);
    
    return 0;   
}

int 
db_set_oc_pass_crc_mode(DATABASE *pdb, unsigned long oc_no, unsigned long pass_crc)
{
    db_cl_set_key_buf(pdb, "OC.PassCrc.%d", oc_no);
    DB_CL_INSERT_UL(pdb, pass_crc);
    
    return 0;   
}


int 
db_set_oc_max_pkt_size(DATABASE *pdb, unsigned long oc_no, unsigned long max_pkt_size)
{
    db_cl_set_key_buf(pdb, "OC.MAXPKTSIZE.%d", oc_no);
    DB_CL_INSERT_UL(pdb, max_pkt_size);
    
    return 0;   
}

int 
db_set_oc_min_pkt_size(DATABASE *pdb, unsigned long oc_no, unsigned long min_pkt_size)
{
    db_cl_set_key_buf(pdb, "OC.MINPKTSIZE.%d", oc_no);
    DB_CL_INSERT_UL(pdb, min_pkt_size);
    
    return 0;   
}




int 
db_get_oc_line_rate(DATABASE *pdb, unsigned long oc_no, unsigned long *line_rate)
{
    db_cl_set_key_buf(pdb, "OC.LineRate.%d", oc_no);
    DB_CL_GET_UL(pdb, line_rate);
    
    return 0;   
}

int 
db_get_oc_line_mode(DATABASE *pdb, unsigned long oc_no, unsigned long *line_mode)
{
    db_cl_set_key_buf(pdb, "OC.Line.%d", oc_no);
    DB_CL_GET_UL(pdb, line_mode);
    
    return 0;   
}

int 
db_get_oc_encap_mode(DATABASE *pdb, unsigned long oc_no, unsigned long *encap_mode)
{
    db_cl_set_key_buf(pdb, "OC.Encap.%d", oc_no);
    DB_CL_GET_UL(pdb, encap_mode);
    
    return 0;   
}

int 
db_get_oc_sonnet_scramble_enable(DATABASE *pdb, unsigned long oc_no, unsigned long *enable_or_disable)
{
    db_cl_set_key_buf(pdb, "OC.Sonnet_scramble.%d", oc_no);
    DB_CL_GET_UL(pdb, enable_or_disable);
    
    return 0;   
}

int 
db_get_oc_payload_scramble_enable(DATABASE *pdb, unsigned long oc_no, unsigned long *enable_or_disable)
{
    db_cl_set_key_buf(pdb, "OC.Payload_scramble.%d", oc_no);
    DB_CL_GET_UL(pdb, enable_or_disable);
    
    return 0;   
}

int 
db_get_oc_crc_mode(DATABASE *pdb, unsigned long oc_no, unsigned long *crc_mode)
{
    db_cl_set_key_buf(pdb, "OC.CrcMode.%d", oc_no);
    DB_CL_GET_UL(pdb, crc_mode);
    
    return 0;   
}


int 
db_get_oc_pass_crc_mode(DATABASE *pdb, unsigned long oc_no, unsigned long *pass_crc)
{
    db_cl_set_key_buf(pdb, "OC.PassCrc.%d", oc_no);
    DB_CL_GET_UL(pdb, pass_crc);
    
    return 0;   
}


int 
db_get_oc_max_pkt_size(DATABASE *pdb, unsigned long oc_no, unsigned long *max_pkt_size)
{
    db_cl_set_key_buf(pdb, "OC.MAXPKTSIZE.%d", oc_no);
    DB_CL_GET_UL(pdb, max_pkt_size);
    
    return 0;   
}

int 
db_get_oc_min_pkt_size(DATABASE *pdb, unsigned long oc_no, unsigned long *min_pkt_size)
{
    db_cl_set_key_buf(pdb, "OC.MINPKTSIZE.%d", oc_no);
    DB_CL_GET_UL(pdb, min_pkt_size);
    
    return 0;   
}



int
db_set_pktlen_range(DATABASE *pdb, unsigned long oc_no, unsigned long range_no, unsigned long pktlen_min, unsigned long pktlen_max)
{
	db_cl_set_key_buf(pdb, "PKTLEN.RANGE.MIN.%d.%d", oc_no, range_no);
    DB_CL_INSERT_UL(pdb, pktlen_min);
    
    db_cl_set_key_buf(pdb, "PKTLEN.RANGE.MAX.%d.%d", oc_no, range_no);
    DB_CL_INSERT_UL(pdb, pktlen_max);
    
    return 0;
}

int 
db_get_pktlen_range(DATABASE *pdb, unsigned long oc_no, unsigned long range_no, unsigned long *pktlen_min, unsigned long *pktlen_max)
{
	db_cl_set_key_buf(pdb, "PKTLEN.RANGE.MIN.%d.%d", oc_no, range_no);
    DB_CL_GET_UL(pdb, pktlen_min);
    
    db_cl_set_key_buf(pdb, "PKTLEN.RANGE.MAX.%d.%d", oc_no, range_no);
    DB_CL_GET_UL(pdb, pktlen_max);
    
    return 0;
}

int 
db_set_sample_rate(DATABASE *pdb, unsigned long oc_no, unsigned long sample_rate)
{
	db_cl_set_key_buf(pdb, "FPGA.SAMPLE.%d", oc_no);
    DB_CL_INSERT_UL(pdb, sample_rate);
    
    return 0;
}

int 
db_get_sample_rate(DATABASE *pdb, unsigned long oc_no, unsigned long *sample_rate)
{
	db_cl_set_key_buf(pdb, "FPGA.SAMPLE.%d", oc_no);
    DB_CL_GET_UL(pdb, sample_rate);
    
    return 0;
}

int 
db_set_flow_enable(DATABASE *pdb, unsigned long oc_no, unsigned long enable_or_disable)
{
	db_cl_set_key_buf(pdb, "FPGA.Flow.%d", oc_no);
    DB_CL_INSERT_UL(pdb, enable_or_disable);
    
    return 0;
}

int 
db_get_flow_enable(DATABASE *pdb, unsigned long oc_no, unsigned long *enable_or_disable)
{
	db_cl_set_key_buf(pdb, "FPGA.Flow.%d", oc_no);
    DB_CL_GET_UL(pdb, enable_or_disable);
    
    return 0;
}

int 
db_set_flow_mms(DATABASE *pdb, unsigned long oc_no, unsigned long mms_size)
{
	db_cl_set_key_buf(pdb, "FPGA.Flow.MMS.%d", oc_no);
    DB_CL_INSERT_UL(pdb, mms_size);
    
    return 0;
}

int 
db_get_flow_mms(DATABASE *pdb, unsigned long oc_no, unsigned long *mms_size)
{
	db_cl_set_key_buf(pdb, "FPGA.Flow.MMS.%d", oc_no);
    DB_CL_GET_UL(pdb, mms_size);
    
    return 0;
}

int 
db_set_traffic_enable(DATABASE *pdb, unsigned long oc_no, unsigned long enable_or_disable)
{
	db_cl_set_key_buf(pdb, "FPGA.Traffic.%d", oc_no);
    DB_CL_INSERT_UL(pdb, enable_or_disable);
    
    return 0;
}

int 
db_get_traffic_enable(DATABASE *pdb, unsigned long oc_no, unsigned long *enable_or_disable)
{
	db_cl_set_key_buf(pdb, "FPGA.Traffic.%d", oc_no);
    DB_CL_GET_UL(pdb, enable_or_disable);
    
    return 0;
}

