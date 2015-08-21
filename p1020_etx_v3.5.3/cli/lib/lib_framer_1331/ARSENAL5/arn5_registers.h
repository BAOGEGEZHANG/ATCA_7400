/*
 * arn5_registers.h
 *
 * $Id$
 */
#ifndef _ARN5_REGISTERS_H_
#define _ARN5_REGISTERS_H_

#include "cs_types.h"


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * MicroIF::mpif
 */

#define ARN5_MPIF_CHIPIdLsb                      0x00000
#define ARN5_MPIF_CHIPIdLsb_dft                  0xf3e5
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 IdLsb                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 IdLsb                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_CHIPIdLsb_t;

#define ARN5_MPIF_CHIPIdMsb                      0x00001
#define ARN5_MPIF_CHIPIdMsb_dft                  0x1000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 IdMsb                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 IdMsb                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_CHIPIdMsb_t;

#define ARN5_MPIF_GlobalReset0                   0x00002
#define ARN5_MPIF_GlobalReset0_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Lif7HReset           :  1 ; /* bits 15:15 */
    cs_uint16 Lif6HReset           :  1 ; /* bits 14:14 */
    cs_uint16 Lif5HReset           :  1 ; /* bits 13:13 */
    cs_uint16 Lif4HReset           :  1 ; /* bits 12:12 */
    cs_uint16 Lif3HReset           :  1 ; /* bits 11:11 */
    cs_uint16 Lif2HReset           :  1 ; /* bits 10:10 */
    cs_uint16 Lif1HReset           :  1 ; /* bits 9:9 */
    cs_uint16 Lif0HReset           :  1 ; /* bits 8:8 */
    cs_uint16 Framer7HReset        :  1 ; /* bits 7:7 */
    cs_uint16 Framer6HReset        :  1 ; /* bits 6:6 */
    cs_uint16 Framer5HReset        :  1 ; /* bits 5:5 */
    cs_uint16 Framer4HReset        :  1 ; /* bits 4:4 */
    cs_uint16 Framer3HReset        :  1 ; /* bits 3:3 */
    cs_uint16 Framer2HReset        :  1 ; /* bits 2:2 */
    cs_uint16 Framer1HReset        :  1 ; /* bits 1:1 */
    cs_uint16 Framer0HReset        :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Framer0HReset        :  1 ; /* bits 0:0 */
    cs_uint16 Framer1HReset        :  1 ; /* bits 1:1 */
    cs_uint16 Framer2HReset        :  1 ; /* bits 2:2 */
    cs_uint16 Framer3HReset        :  1 ; /* bits 3:3 */
    cs_uint16 Framer4HReset        :  1 ; /* bits 4:4 */
    cs_uint16 Framer5HReset        :  1 ; /* bits 5:5 */
    cs_uint16 Framer6HReset        :  1 ; /* bits 6:6 */
    cs_uint16 Framer7HReset        :  1 ; /* bits 7:7 */
    cs_uint16 Lif0HReset           :  1 ; /* bits 8:8 */
    cs_uint16 Lif1HReset           :  1 ; /* bits 9:9 */
    cs_uint16 Lif2HReset           :  1 ; /* bits 10:10 */
    cs_uint16 Lif3HReset           :  1 ; /* bits 11:11 */
    cs_uint16 Lif4HReset           :  1 ; /* bits 12:12 */
    cs_uint16 Lif5HReset           :  1 ; /* bits 13:13 */
    cs_uint16 Lif6HReset           :  1 ; /* bits 14:14 */
    cs_uint16 Lif7HReset           :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GlobalReset0_t;

#define ARN5_MPIF_GlobalReset1                   0x00003
#define ARN5_MPIF_GlobalReset1_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 SPI4P2HReset         :  1 ; /* bits 8:8 */
    cs_uint16 Gige7HReset          :  1 ; /* bits 7:7 */
    cs_uint16 Gige6HReset          :  1 ; /* bits 6:6 */
    cs_uint16 Gige5HReset          :  1 ; /* bits 5:5 */
    cs_uint16 Gige4HReset          :  1 ; /* bits 4:4 */
    cs_uint16 Gige3HReset          :  1 ; /* bits 3:3 */
    cs_uint16 Gige2HReset          :  1 ; /* bits 2:2 */
    cs_uint16 Gige1HReset          :  1 ; /* bits 1:1 */
    cs_uint16 Gige0HReset          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Gige0HReset          :  1 ; /* bits 0:0 */
    cs_uint16 Gige1HReset          :  1 ; /* bits 1:1 */
    cs_uint16 Gige2HReset          :  1 ; /* bits 2:2 */
    cs_uint16 Gige3HReset          :  1 ; /* bits 3:3 */
    cs_uint16 Gige4HReset          :  1 ; /* bits 4:4 */
    cs_uint16 Gige5HReset          :  1 ; /* bits 5:5 */
    cs_uint16 Gige6HReset          :  1 ; /* bits 6:6 */
    cs_uint16 Gige7HReset          :  1 ; /* bits 7:7 */
    cs_uint16 SPI4P2HReset         :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GlobalReset1_t;

#define ARN5_MPIF_GlobalReset2                   0x00004
#define ARN5_MPIF_GlobalReset2_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 Pprbs7HReset         :  1 ; /* bits 7:7 */
    cs_uint16 Pprbs6HReset         :  1 ; /* bits 6:6 */
    cs_uint16 Pprbs5HReset         :  1 ; /* bits 5:5 */
    cs_uint16 Pprbs4HReset         :  1 ; /* bits 4:4 */
    cs_uint16 Pprbs3HReset         :  1 ; /* bits 3:3 */
    cs_uint16 Pprbs2HReset         :  1 ; /* bits 2:2 */
    cs_uint16 Pprbs1HReset         :  1 ; /* bits 1:1 */
    cs_uint16 Pprbs0HReset         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Pprbs0HReset         :  1 ; /* bits 0:0 */
    cs_uint16 Pprbs1HReset         :  1 ; /* bits 1:1 */
    cs_uint16 Pprbs2HReset         :  1 ; /* bits 2:2 */
    cs_uint16 Pprbs3HReset         :  1 ; /* bits 3:3 */
    cs_uint16 Pprbs4HReset         :  1 ; /* bits 4:4 */
    cs_uint16 Pprbs5HReset         :  1 ; /* bits 5:5 */
    cs_uint16 Pprbs6HReset         :  1 ; /* bits 6:6 */
    cs_uint16 Pprbs7HReset         :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GlobalReset2_t;

#define ARN5_MPIF_GlobalInt                      0x00005
#define ARN5_MPIF_GlobalInt_dft                  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 MPIFI                :  1 ; /* bits 10:10 */
    cs_uint16 GPIOI                :  1 ; /* bits 9:9 */
    cs_uint16 SPI4P2I              :  1 ; /* bits 8:8 */
    cs_uint16 Channel7I            :  1 ; /* bits 7:7 */
    cs_uint16 Channel6I            :  1 ; /* bits 6:6 */
    cs_uint16 Channel5I            :  1 ; /* bits 5:5 */
    cs_uint16 Channel4I            :  1 ; /* bits 4:4 */
    cs_uint16 Channel3I            :  1 ; /* bits 3:3 */
    cs_uint16 Channel2I            :  1 ; /* bits 2:2 */
    cs_uint16 Channel1I            :  1 ; /* bits 1:1 */
    cs_uint16 Channel0I            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Channel0I            :  1 ; /* bits 0:0 */
    cs_uint16 Channel1I            :  1 ; /* bits 1:1 */
    cs_uint16 Channel2I            :  1 ; /* bits 2:2 */
    cs_uint16 Channel3I            :  1 ; /* bits 3:3 */
    cs_uint16 Channel4I            :  1 ; /* bits 4:4 */
    cs_uint16 Channel5I            :  1 ; /* bits 5:5 */
    cs_uint16 Channel6I            :  1 ; /* bits 6:6 */
    cs_uint16 Channel7I            :  1 ; /* bits 7:7 */
    cs_uint16 SPI4P2I              :  1 ; /* bits 8:8 */
    cs_uint16 GPIOI                :  1 ; /* bits 9:9 */
    cs_uint16 MPIFI                :  1 ; /* bits 10:10 */
    cs_uint16 rsrvd1               :  5 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GlobalInt_t;

#define ARN5_MPIF_GlobalIntEnable                0x00006
#define ARN5_MPIF_GlobalIntEnable_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 MPIFE                :  1 ; /* bits 10:10 */
    cs_uint16 GPIOE                :  1 ; /* bits 9:9 */
    cs_uint16 SPI4P2E              :  1 ; /* bits 8:8 */
    cs_uint16 Channel7E            :  1 ; /* bits 7:7 */
    cs_uint16 Channel6E            :  1 ; /* bits 6:6 */
    cs_uint16 Channel5E            :  1 ; /* bits 5:5 */
    cs_uint16 Channel4E            :  1 ; /* bits 4:4 */
    cs_uint16 Channel3E            :  1 ; /* bits 3:3 */
    cs_uint16 Channel2E            :  1 ; /* bits 2:2 */
    cs_uint16 Channel1E            :  1 ; /* bits 1:1 */
    cs_uint16 Channel0E            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Channel0E            :  1 ; /* bits 0:0 */
    cs_uint16 Channel1E            :  1 ; /* bits 1:1 */
    cs_uint16 Channel2E            :  1 ; /* bits 2:2 */
    cs_uint16 Channel3E            :  1 ; /* bits 3:3 */
    cs_uint16 Channel4E            :  1 ; /* bits 4:4 */
    cs_uint16 Channel5E            :  1 ; /* bits 5:5 */
    cs_uint16 Channel6E            :  1 ; /* bits 6:6 */
    cs_uint16 Channel7E            :  1 ; /* bits 7:7 */
    cs_uint16 SPI4P2E              :  1 ; /* bits 8:8 */
    cs_uint16 GPIOE                :  1 ; /* bits 9:9 */
    cs_uint16 MPIFE                :  1 ; /* bits 10:10 */
    cs_uint16 rsrvd1               :  5 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GlobalIntEnable_t;

#define ARN5_MPIF_Channel0Int                    0x00007
#define ARN5_MPIF_Channel0Int_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 GIGE0I               :  1 ; /* bits 3:3 */
    cs_uint16 PPRBS0I              :  1 ; /* bits 2:2 */
    cs_uint16 LIF0I                :  1 ; /* bits 1:1 */
    cs_uint16 Framer0I             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Framer0I             :  1 ; /* bits 0:0 */
    cs_uint16 LIF0I                :  1 ; /* bits 1:1 */
    cs_uint16 PPRBS0I              :  1 ; /* bits 2:2 */
    cs_uint16 GIGE0I               :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_Channel0Int_t;

#define ARN5_MPIF_Channel0IntEnable              0x00008
#define ARN5_MPIF_Channel0IntEnable_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 GIGE0E               :  1 ; /* bits 3:3 */
    cs_uint16 PPRBS0E              :  1 ; /* bits 2:2 */
    cs_uint16 LIF0E                :  1 ; /* bits 1:1 */
    cs_uint16 Framer0E             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Framer0E             :  1 ; /* bits 0:0 */
    cs_uint16 LIF0E                :  1 ; /* bits 1:1 */
    cs_uint16 PPRBS0E              :  1 ; /* bits 2:2 */
    cs_uint16 GIGE0E               :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_Channel0IntEnable_t;

#define ARN5_MPIF_Channel1Int                    0x00009
#define ARN5_MPIF_Channel1Int_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 GIGE1I               :  1 ; /* bits 3:3 */
    cs_uint16 PPRBS1I              :  1 ; /* bits 2:2 */
    cs_uint16 LIF1I                :  1 ; /* bits 1:1 */
    cs_uint16 Framer1I             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Framer1I             :  1 ; /* bits 0:0 */
    cs_uint16 LIF1I                :  1 ; /* bits 1:1 */
    cs_uint16 PPRBS1I              :  1 ; /* bits 2:2 */
    cs_uint16 GIGE1I               :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_Channel1Int_t;

#define ARN5_MPIF_Channel1IntEnable              0x0000a
#define ARN5_MPIF_Channel1IntEnable_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 GIGE1E               :  1 ; /* bits 3:3 */
    cs_uint16 PPRBS1E              :  1 ; /* bits 2:2 */
    cs_uint16 LIF1E                :  1 ; /* bits 1:1 */
    cs_uint16 Framer1E             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Framer1E             :  1 ; /* bits 0:0 */
    cs_uint16 LIF1E                :  1 ; /* bits 1:1 */
    cs_uint16 PPRBS1E              :  1 ; /* bits 2:2 */
    cs_uint16 GIGE1E               :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_Channel1IntEnable_t;

#define ARN5_MPIF_Channel2Int                    0x0000b
#define ARN5_MPIF_Channel2Int_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 GIGE2I               :  1 ; /* bits 3:3 */
    cs_uint16 PPRBS2I              :  1 ; /* bits 2:2 */
    cs_uint16 LIF2I                :  1 ; /* bits 1:1 */
    cs_uint16 Framer2I             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Framer2I             :  1 ; /* bits 0:0 */
    cs_uint16 LIF2I                :  1 ; /* bits 1:1 */
    cs_uint16 PPRBS2I              :  1 ; /* bits 2:2 */
    cs_uint16 GIGE2I               :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_Channel2Int_t;

#define ARN5_MPIF_Channel2IntEnable              0x0000c
#define ARN5_MPIF_Channel2IntEnable_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 GIGE2E               :  1 ; /* bits 3:3 */
    cs_uint16 PPRBS2E              :  1 ; /* bits 2:2 */
    cs_uint16 LIF2E                :  1 ; /* bits 1:1 */
    cs_uint16 Framer2E             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Framer2E             :  1 ; /* bits 0:0 */
    cs_uint16 LIF2E                :  1 ; /* bits 1:1 */
    cs_uint16 PPRBS2E              :  1 ; /* bits 2:2 */
    cs_uint16 GIGE2E               :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_Channel2IntEnable_t;

#define ARN5_MPIF_Channel3Int                    0x0000d
#define ARN5_MPIF_Channel3Int_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 GIGE3I               :  1 ; /* bits 3:3 */
    cs_uint16 PPRBS3I              :  1 ; /* bits 2:2 */
    cs_uint16 LIF3I                :  1 ; /* bits 1:1 */
    cs_uint16 Framer3I             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Framer3I             :  1 ; /* bits 0:0 */
    cs_uint16 LIF3I                :  1 ; /* bits 1:1 */
    cs_uint16 PPRBS3I              :  1 ; /* bits 2:2 */
    cs_uint16 GIGE3I               :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_Channel3Int_t;

#define ARN5_MPIF_Channel3IntEnable              0x0000e
#define ARN5_MPIF_Channel3IntEnable_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 GIGE3E               :  1 ; /* bits 3:3 */
    cs_uint16 PPRBS3E              :  1 ; /* bits 2:2 */
    cs_uint16 LIF3E                :  1 ; /* bits 1:1 */
    cs_uint16 Framer3E             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Framer3E             :  1 ; /* bits 0:0 */
    cs_uint16 LIF3E                :  1 ; /* bits 1:1 */
    cs_uint16 PPRBS3E              :  1 ; /* bits 2:2 */
    cs_uint16 GIGE3E               :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_Channel3IntEnable_t;

#define ARN5_MPIF_Channel4Int                    0x0000f
#define ARN5_MPIF_Channel4Int_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 GIGE4I               :  1 ; /* bits 3:3 */
    cs_uint16 PPRBS4I              :  1 ; /* bits 2:2 */
    cs_uint16 LIF4I                :  1 ; /* bits 1:1 */
    cs_uint16 Framer4I             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Framer4I             :  1 ; /* bits 0:0 */
    cs_uint16 LIF4I                :  1 ; /* bits 1:1 */
    cs_uint16 PPRBS4I              :  1 ; /* bits 2:2 */
    cs_uint16 GIGE4I               :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_Channel4Int_t;

#define ARN5_MPIF_Channel4IntEnable              0x00010
#define ARN5_MPIF_Channel4IntEnable_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 GIGE4E               :  1 ; /* bits 3:3 */
    cs_uint16 PPRBS4E              :  1 ; /* bits 2:2 */
    cs_uint16 LIF4E                :  1 ; /* bits 1:1 */
    cs_uint16 Framer4E             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Framer4E             :  1 ; /* bits 0:0 */
    cs_uint16 LIF4E                :  1 ; /* bits 1:1 */
    cs_uint16 PPRBS4E              :  1 ; /* bits 2:2 */
    cs_uint16 GIGE4E               :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_Channel4IntEnable_t;

#define ARN5_MPIF_Channel5Int                    0x00011
#define ARN5_MPIF_Channel5Int_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 GIGE5I               :  1 ; /* bits 3:3 */
    cs_uint16 PPRBS5I              :  1 ; /* bits 2:2 */
    cs_uint16 LIF5I                :  1 ; /* bits 1:1 */
    cs_uint16 Framer5I             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Framer5I             :  1 ; /* bits 0:0 */
    cs_uint16 LIF5I                :  1 ; /* bits 1:1 */
    cs_uint16 PPRBS5I              :  1 ; /* bits 2:2 */
    cs_uint16 GIGE5I               :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_Channel5Int_t;

#define ARN5_MPIF_Channel5IntEnable              0x00012
#define ARN5_MPIF_Channel5IntEnable_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 GIGE5E               :  1 ; /* bits 3:3 */
    cs_uint16 PPRBS5E              :  1 ; /* bits 2:2 */
    cs_uint16 LIF5E                :  1 ; /* bits 1:1 */
    cs_uint16 Framer5E             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Framer5E             :  1 ; /* bits 0:0 */
    cs_uint16 LIF5E                :  1 ; /* bits 1:1 */
    cs_uint16 PPRBS5E              :  1 ; /* bits 2:2 */
    cs_uint16 GIGE5E               :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_Channel5IntEnable_t;

#define ARN5_MPIF_Channel6Int                    0x00013
#define ARN5_MPIF_Channel6Int_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 GIGE6I               :  1 ; /* bits 3:3 */
    cs_uint16 PPRBS6I              :  1 ; /* bits 2:2 */
    cs_uint16 LIF6I                :  1 ; /* bits 1:1 */
    cs_uint16 Framer6I             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Framer6I             :  1 ; /* bits 0:0 */
    cs_uint16 LIF6I                :  1 ; /* bits 1:1 */
    cs_uint16 PPRBS6I              :  1 ; /* bits 2:2 */
    cs_uint16 GIGE6I               :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_Channel6Int_t;

#define ARN5_MPIF_Channel6IntEnable              0x00014
#define ARN5_MPIF_Channel6IntEnable_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 GIGE6E               :  1 ; /* bits 3:3 */
    cs_uint16 PPRBS6E              :  1 ; /* bits 2:2 */
    cs_uint16 LIF6E                :  1 ; /* bits 1:1 */
    cs_uint16 Framer6E             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Framer6E             :  1 ; /* bits 0:0 */
    cs_uint16 LIF6E                :  1 ; /* bits 1:1 */
    cs_uint16 PPRBS6E              :  1 ; /* bits 2:2 */
    cs_uint16 GIGE6E               :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_Channel6IntEnable_t;

#define ARN5_MPIF_Channel7Int                    0x00015
#define ARN5_MPIF_Channel7Int_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 GIGE7I               :  1 ; /* bits 3:3 */
    cs_uint16 PPRBS7I              :  1 ; /* bits 2:2 */
    cs_uint16 LIF7I                :  1 ; /* bits 1:1 */
    cs_uint16 Framer7I             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Framer7I             :  1 ; /* bits 0:0 */
    cs_uint16 LIF7I                :  1 ; /* bits 1:1 */
    cs_uint16 PPRBS7I              :  1 ; /* bits 2:2 */
    cs_uint16 GIGE7I               :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_Channel7Int_t;

#define ARN5_MPIF_Channel7IntEnable              0x00016
#define ARN5_MPIF_Channel7IntEnable_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 GIGE7E               :  1 ; /* bits 3:3 */
    cs_uint16 PPRBS7E              :  1 ; /* bits 2:2 */
    cs_uint16 LIF7E                :  1 ; /* bits 1:1 */
    cs_uint16 Framer7E             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Framer7E             :  1 ; /* bits 0:0 */
    cs_uint16 LIF7E                :  1 ; /* bits 1:1 */
    cs_uint16 PPRBS7E              :  1 ; /* bits 2:2 */
    cs_uint16 GIGE7E               :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_Channel7IntEnable_t;

#define ARN5_MPIF_ChannelMode                    0x00017
#define ARN5_MPIF_ChannelMode_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 Ch7Mode              :  1 ; /* bits 7:7 */
    cs_uint16 Ch6Mode              :  1 ; /* bits 6:6 */
    cs_uint16 Ch5Mode              :  1 ; /* bits 5:5 */
    cs_uint16 Ch4Mode              :  1 ; /* bits 4:4 */
    cs_uint16 Ch3Mode              :  1 ; /* bits 3:3 */
    cs_uint16 Ch2Mode              :  1 ; /* bits 2:2 */
    cs_uint16 Ch1Mode              :  1 ; /* bits 1:1 */
    cs_uint16 Ch0Mode              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Ch0Mode              :  1 ; /* bits 0:0 */
    cs_uint16 Ch1Mode              :  1 ; /* bits 1:1 */
    cs_uint16 Ch2Mode              :  1 ; /* bits 2:2 */
    cs_uint16 Ch3Mode              :  1 ; /* bits 3:3 */
    cs_uint16 Ch4Mode              :  1 ; /* bits 4:4 */
    cs_uint16 Ch5Mode              :  1 ; /* bits 5:5 */
    cs_uint16 Ch6Mode              :  1 ; /* bits 6:6 */
    cs_uint16 Ch7Mode              :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_ChannelMode_t;
#define     ARN5_Ch7Mode_SONET                            0x00
#define     ARN5_Ch7Mode_GE                               0x01
#define     ARN5_Ch6Mode_SONET                            0x00
#define     ARN5_Ch6Mode_GE                               0x01
#define     ARN5_Ch5Mode_SONET                            0x00
#define     ARN5_Ch5Mode_GE                               0x01
#define     ARN5_Ch4Mode_SONET                            0x00
#define     ARN5_Ch4Mode_GE                               0x01
#define     ARN5_Ch3Mode_SONET                            0x00
#define     ARN5_Ch3Mode_GE                               0x01
#define     ARN5_Ch2Mode_SONET                            0x00
#define     ARN5_Ch2Mode_GE                               0x01
#define     ARN5_Ch1Mode_SONET                            0x00
#define     ARN5_Ch1Mode_GE                               0x01
#define     ARN5_Ch0Mode_SONET                            0x00
#define     ARN5_Ch0Mode_GE                               0x01

#define ARN5_MPIF_SONETInterface                 0x00018
#define ARN5_MPIF_SONETInterface_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Ch7SIType            :  2 ; /* bits 15:14 */
    cs_uint16 Ch6SIType            :  2 ; /* bits 13:12 */
    cs_uint16 Ch5SIType            :  2 ; /* bits 11:10 */
    cs_uint16 Ch4SIType            :  2 ; /* bits 9:8 */
    cs_uint16 Ch3SIType            :  2 ; /* bits 7:6 */
    cs_uint16 Ch2SIType            :  2 ; /* bits 5:4 */
    cs_uint16 Ch1SIType            :  2 ; /* bits 3:2 */
    cs_uint16 Ch0SIType            :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Ch0SIType            :  2 ; /* bits 1:0 */
    cs_uint16 Ch1SIType            :  2 ; /* bits 3:2 */
    cs_uint16 Ch2SIType            :  2 ; /* bits 5:4 */
    cs_uint16 Ch3SIType            :  2 ; /* bits 7:6 */
    cs_uint16 Ch4SIType            :  2 ; /* bits 9:8 */
    cs_uint16 Ch5SIType            :  2 ; /* bits 11:10 */
    cs_uint16 Ch6SIType            :  2 ; /* bits 13:12 */
    cs_uint16 Ch7SIType            :  2 ; /* bits 15:14 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_SONETInterface_t;
#define     ARN5_Ch7SIType_OC3                            0x00
#define     ARN5_Ch7SIType_OC12                           0x01
#define     ARN5_Ch7SIType_OC48                           0x02
#define     ARN5_Ch7SIType_Undef                          0x03
#define     ARN5_Ch6SIType_OC3                            0x00
#define     ARN5_Ch6SIType_OC12                           0x01
#define     ARN5_Ch6SIType_OC48                           0x02
#define     ARN5_Ch6SIType_Undef                          0x03
#define     ARN5_Ch5SIType_OC3                            0x00
#define     ARN5_Ch5SIType_OC12                           0x01
#define     ARN5_Ch5SIType_OC48                           0x02
#define     ARN5_Ch5SIType_Undef                          0x03
#define     ARN5_Ch4SIType_OC3                            0x00
#define     ARN5_Ch4SIType_OC12                           0x01
#define     ARN5_Ch4SIType_OC48                           0x02
#define     ARN5_Ch4SIType_Undef                          0x03
#define     ARN5_Ch3SIType_OC3                            0x00
#define     ARN5_Ch3SIType_OC12                           0x01
#define     ARN5_Ch3SIType_OC48                           0x02
#define     ARN5_Ch3SIType_Undef                          0x03
#define     ARN5_Ch2SIType_OC3                            0x00
#define     ARN5_Ch2SIType_OC12                           0x01
#define     ARN5_Ch2SIType_OC48                           0x02
#define     ARN5_Ch2SIType_Undef                          0x03
#define     ARN5_Ch1SIType_OC3                            0x00
#define     ARN5_Ch1SIType_OC12                           0x01
#define     ARN5_Ch1SIType_OC48                           0x02
#define     ARN5_Ch1SIType_Undef                          0x03
#define     ARN5_Ch0SIType_OC3                            0x00
#define     ARN5_Ch0SIType_OC12                           0x01
#define     ARN5_Ch0SIType_OC48                           0x02
#define     ARN5_Ch0SIType_Undef                          0x03

#define ARN5_MPIF_PowerdownControl               0x00019
#define ARN5_MPIF_PowerdownControl_dft           0x00ff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 PwrDwn_7             :  1 ; /* bits 7:7 */
    cs_uint16 PwrDwn_6             :  1 ; /* bits 6:6 */
    cs_uint16 PwrDwn_5             :  1 ; /* bits 5:5 */
    cs_uint16 PwrDwn_4             :  1 ; /* bits 4:4 */
    cs_uint16 PwrDwn_3             :  1 ; /* bits 3:3 */
    cs_uint16 PwrDwn_2             :  1 ; /* bits 2:2 */
    cs_uint16 PwrDwn_1             :  1 ; /* bits 1:1 */
    cs_uint16 PwrDwn_0             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PwrDwn_0             :  1 ; /* bits 0:0 */
    cs_uint16 PwrDwn_1             :  1 ; /* bits 1:1 */
    cs_uint16 PwrDwn_2             :  1 ; /* bits 2:2 */
    cs_uint16 PwrDwn_3             :  1 ; /* bits 3:3 */
    cs_uint16 PwrDwn_4             :  1 ; /* bits 4:4 */
    cs_uint16 PwrDwn_5             :  1 ; /* bits 5:5 */
    cs_uint16 PwrDwn_6             :  1 ; /* bits 6:6 */
    cs_uint16 PwrDwn_7             :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_PowerdownControl_t;

#define ARN5_MPIF_SysClk400Phase                 0x0001a
#define ARN5_MPIF_SysClk400Phase_dft             0x000b
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 PHASE                :  4 ; /* bits 15:12 */
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 CKDIV2_EN            :  1 ; /* bits 4:4 */
    cs_uint16 CKDIV4_EN            :  1 ; /* bits 3:3 */
    cs_uint16 CKDIV8_EN            :  1 ; /* bits 2:2 */
    cs_uint16 CKDIV16_EN           :  1 ; /* bits 1:1 */
    cs_uint16 CKDIV2_ADJ_EN        :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 CKDIV2_ADJ_EN        :  1 ; /* bits 0:0 */
    cs_uint16 CKDIV16_EN           :  1 ; /* bits 1:1 */
    cs_uint16 CKDIV8_EN            :  1 ; /* bits 2:2 */
    cs_uint16 CKDIV4_EN            :  1 ; /* bits 3:3 */
    cs_uint16 CKDIV2_EN            :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 PHASE                :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_SysClk400Phase_t;

#define ARN5_MPIF_SyncClk                        0x0001b
#define ARN5_MPIF_SyncClk_dft                    0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  7 ;
    cs_uint16 SQUELCH              :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 CHANNEL              :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 CHANNEL              :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 SQUELCH              :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_SyncClk_t;
#define     ARN5_CHANNEL_CHAN0                            0x00
#define     ARN5_CHANNEL_CHAN1                            0x01
#define     ARN5_CHANNEL_CHAN2                            0x02
#define     ARN5_CHANNEL_CHAN3                            0x03
#define     ARN5_CHANNEL_CHAN4                            0x04
#define     ARN5_CHANNEL_CHAN5                            0x05
#define     ARN5_CHANNEL_CHAN6                            0x06
#define     ARN5_CHANNEL_CHAN7                            0x07

#define ARN5_MPIF_SMHControl0                    0x0001c
#define ARN5_MPIF_SMHControl0_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  2 ;
    cs_uint16 TxEn                 :  1 ; /* bits 13:13 */
    cs_uint16 TxType               :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 TxChID               :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 RxEn                 :  1 ; /* bits 5:5 */
    cs_uint16 RxType               :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 RxChID               :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RxChID               :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 RxType               :  1 ; /* bits 4:4 */
    cs_uint16 RxEn                 :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 TxChID               :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 TxType               :  1 ; /* bits 12:12 */
    cs_uint16 TxEn                 :  1 ; /* bits 13:13 */
    cs_uint16 rsrvd4               :  2 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_SMHControl0_t;

#define ARN5_MPIF_SMHControl1                    0x0001d
#define ARN5_MPIF_SMHControl1_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  2 ;
    cs_uint16 TxEn                 :  1 ; /* bits 13:13 */
    cs_uint16 TxType               :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 TxChID               :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 RxEn                 :  1 ; /* bits 5:5 */
    cs_uint16 RxType               :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 RxChID               :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RxChID               :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 RxType               :  1 ; /* bits 4:4 */
    cs_uint16 RxEn                 :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 TxChID               :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 TxType               :  1 ; /* bits 12:12 */
    cs_uint16 TxEn                 :  1 ; /* bits 13:13 */
    cs_uint16 rsrvd4               :  2 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_SMHControl1_t;

#define ARN5_MPIF_GPIODirection0                 0x0001e
#define ARN5_MPIF_GPIODirection0_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 DIR                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 DIR                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GPIODirection0_t;
#define     ARN5_DIR_DIRi                                 0x00
#define     ARN5_DIR_DIRo                                 0x01

#define ARN5_MPIF_GPIODirection1                 0x0001f
#define ARN5_MPIF_GPIODirection1_dft             0x00ff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 DIR                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 DIR                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GPIODirection1_t;

#define ARN5_MPIF_GPIOAlarmControl               0x00020
#define ARN5_MPIF_GPIOAlarmControl_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 Alarm_E              :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Alarm_E              :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GPIOAlarmControl_t;
#define     ARN5_Alarm_E_GPIO_output                      0x00
#define     ARN5_Alarm_E_ALARM                            0x01

#define ARN5_MPIF_GPIOFmroControl                0x00021
#define ARN5_MPIF_GPIOFmroControl_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 10 ;
    cs_uint16 FMRO_E               :  1 ; /* bits 5:5 */
    cs_uint16 FMRO_SEL             :  1 ; /* bits 4:4 */
    cs_uint16 FMRO_FRSEL           :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FMRO_FRSEL           :  4 ; /* bits 3:0 */
    cs_uint16 FMRO_SEL             :  1 ; /* bits 4:4 */
    cs_uint16 FMRO_E               :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd1               : 10 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GPIOFmroControl_t;
#define     ARN5_FMRO_E_OTHER                             0x00
#define     ARN5_FMRO_E_FMRO                              0x01
#define     ARN5_FMRO_SEL_Br                              0x00
#define     ARN5_FMRO_SEL_FrGe                            0x01
#define     ARN5_FMRO_FRSEL_Fr0                           0x00
#define     ARN5_FMRO_FRSEL_Fr1                           0x01
#define     ARN5_FMRO_FRSEL_Fr2                           0x02
#define     ARN5_FMRO_FRSEL_Fr3                           0x03
#define     ARN5_FMRO_FRSEL_Fr4                           0x04
#define     ARN5_FMRO_FRSEL_Fr5                           0x05
#define     ARN5_FMRO_FRSEL_Fr6                           0x06
#define     ARN5_FMRO_FRSEL_Fr7                           0x07
#define     ARN5_FMRO_FRSEL_Ge0                           0x08
#define     ARN5_FMRO_FRSEL_Ge1                           0x09
#define     ARN5_FMRO_FRSEL_Ge2                           0x0a
#define     ARN5_FMRO_FRSEL_Ge3                           0x0b
#define     ARN5_FMRO_FRSEL_Ge4                           0x0c
#define     ARN5_FMRO_FRSEL_Ge5                           0x0d
#define     ARN5_FMRO_FRSEL_Ge6                           0x0e
#define     ARN5_FMRO_FRSEL_Ge7                           0x0f

#define ARN5_MPIF_GPIOOutput0                    0x00022
#define ARN5_MPIF_GPIOOutput0_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 O                    :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 O                    :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GPIOOutput0_t;

#define ARN5_MPIF_GPIOOutput1                    0x00023
#define ARN5_MPIF_GPIOOutput1_dft                0x00ff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 O                    :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 O                    :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GPIOOutput1_t;

#define ARN5_MPIF_GPIOSample0                    0x00024
#define ARN5_MPIF_GPIOSample0_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 I                    :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 I                    :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GPIOSample0_t;

#define ARN5_MPIF_GPIOSample1                    0x00025
#define ARN5_MPIF_GPIOSample1_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 I                    :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 I                    :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GPIOSample1_t;

#define ARN5_MPIF_GPIORInt0                      0x00026
#define ARN5_MPIF_GPIORInt0_dft                  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 RI0                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RI0                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GPIORInt0_t;

#define ARN5_MPIF_GPIORInt1                      0x00027
#define ARN5_MPIF_GPIORInt1_dft                  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 RI1                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RI1                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GPIORInt1_t;

#define ARN5_MPIF_GPIOFInt0                      0x00028
#define ARN5_MPIF_GPIOFInt0_dft                  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 FI0                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FI0                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GPIOFInt0_t;

#define ARN5_MPIF_GPIOFInt1                      0x00029
#define ARN5_MPIF_GPIOFInt1_dft                  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 FI1                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FI1                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GPIOFInt1_t;

#define ARN5_MPIF_GPIORMask0                     0x0002a
#define ARN5_MPIF_GPIORMask0_dft                 0x00ff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 RM0                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RM0                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GPIORMask0_t;

#define ARN5_MPIF_GPIORMask1                     0x0002b
#define ARN5_MPIF_GPIORMask1_dft                 0x00ff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 RM1                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RM1                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GPIORMask1_t;

#define ARN5_MPIF_GPIOFMask0                     0x0002c
#define ARN5_MPIF_GPIOFMask0_dft                 0x00ff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 FM0                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FM0                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GPIOFMask0_t;

#define ARN5_MPIF_GPIOFMask1                     0x0002d
#define ARN5_MPIF_GPIOFMask1_dft                 0x00ff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 FM1                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FM1                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_GPIOFMask1_t;

#define ARN5_MPIF_BISTCONTROL                    0x0002e
#define ARN5_MPIF_BISTCONTROL_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 START_FIFO_BIST      :  1 ; /* bits 15:15 */
    cs_uint16 START_PROD_BIST      :  1 ; /* bits 14:14 */
    cs_uint16 CANCEL_AUTOBIST      :  1 ; /* bits 13:13 */
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 REPAIR_DONE          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 REPAIR_DONE          :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 CANCEL_AUTOBIST      :  1 ; /* bits 13:13 */
    cs_uint16 START_PROD_BIST      :  1 ; /* bits 14:14 */
    cs_uint16 START_FIFO_BIST      :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_BISTCONTROL_t;

#define ARN5_MPIF_BISTSTATUS0                    0x0002f
#define ARN5_MPIF_BISTSTATUS0_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 FRAMER_RX_7          :  2 ; /* bits 15:14 */
    cs_uint16 FRAMER_RX_6          :  2 ; /* bits 13:12 */
    cs_uint16 FRAMER_RX_5          :  2 ; /* bits 11:10 */
    cs_uint16 FRAMER_RX_4          :  2 ; /* bits 9:8 */
    cs_uint16 FRAMER_RX_3          :  2 ; /* bits 7:6 */
    cs_uint16 FRAMER_RX_2          :  2 ; /* bits 5:4 */
    cs_uint16 FRAMER_RX_1          :  2 ; /* bits 3:2 */
    cs_uint16 FRAMER_RX_0          :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FRAMER_RX_0          :  2 ; /* bits 1:0 */
    cs_uint16 FRAMER_RX_1          :  2 ; /* bits 3:2 */
    cs_uint16 FRAMER_RX_2          :  2 ; /* bits 5:4 */
    cs_uint16 FRAMER_RX_3          :  2 ; /* bits 7:6 */
    cs_uint16 FRAMER_RX_4          :  2 ; /* bits 9:8 */
    cs_uint16 FRAMER_RX_5          :  2 ; /* bits 11:10 */
    cs_uint16 FRAMER_RX_6          :  2 ; /* bits 13:12 */
    cs_uint16 FRAMER_RX_7          :  2 ; /* bits 15:14 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_BISTSTATUS0_t;

#define ARN5_MPIF_BISTSTATUS1                    0x00030
#define ARN5_MPIF_BISTSTATUS1_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 FRAMER_TX_7          :  2 ; /* bits 15:14 */
    cs_uint16 FRAMER_TX_6          :  2 ; /* bits 13:12 */
    cs_uint16 FRAMER_TX_5          :  2 ; /* bits 11:10 */
    cs_uint16 FRAMER_TX_4          :  2 ; /* bits 9:8 */
    cs_uint16 FRAMER_TX_3          :  2 ; /* bits 7:6 */
    cs_uint16 FRAMER_TX_2          :  2 ; /* bits 5:4 */
    cs_uint16 FRAMER_TX_1          :  2 ; /* bits 3:2 */
    cs_uint16 FRAMER_TX_0          :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FRAMER_TX_0          :  2 ; /* bits 1:0 */
    cs_uint16 FRAMER_TX_1          :  2 ; /* bits 3:2 */
    cs_uint16 FRAMER_TX_2          :  2 ; /* bits 5:4 */
    cs_uint16 FRAMER_TX_3          :  2 ; /* bits 7:6 */
    cs_uint16 FRAMER_TX_4          :  2 ; /* bits 9:8 */
    cs_uint16 FRAMER_TX_5          :  2 ; /* bits 11:10 */
    cs_uint16 FRAMER_TX_6          :  2 ; /* bits 13:12 */
    cs_uint16 FRAMER_TX_7          :  2 ; /* bits 15:14 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_BISTSTATUS1_t;

#define ARN5_MPIF_BISTSTATUS3                    0x00031
#define ARN5_MPIF_BISTSTATUS3_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 GE_TX_7              :  2 ; /* bits 15:14 */
    cs_uint16 GE_TX_6              :  2 ; /* bits 13:12 */
    cs_uint16 GE_TX_5              :  2 ; /* bits 11:10 */
    cs_uint16 GE_TX_4              :  2 ; /* bits 9:8 */
    cs_uint16 GE_TX_3              :  2 ; /* bits 7:6 */
    cs_uint16 GE_TX_2              :  2 ; /* bits 5:4 */
    cs_uint16 GE_TX_1              :  2 ; /* bits 3:2 */
    cs_uint16 GE_TX_0              :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 GE_TX_0              :  2 ; /* bits 1:0 */
    cs_uint16 GE_TX_1              :  2 ; /* bits 3:2 */
    cs_uint16 GE_TX_2              :  2 ; /* bits 5:4 */
    cs_uint16 GE_TX_3              :  2 ; /* bits 7:6 */
    cs_uint16 GE_TX_4              :  2 ; /* bits 9:8 */
    cs_uint16 GE_TX_5              :  2 ; /* bits 11:10 */
    cs_uint16 GE_TX_6              :  2 ; /* bits 13:12 */
    cs_uint16 GE_TX_7              :  2 ; /* bits 15:14 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_BISTSTATUS3_t;

#define ARN5_MPIF_BISTSTATUS4                    0x00032
#define ARN5_MPIF_BISTSTATUS4_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 BRIDGE_RX            :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 BRIDGE_RX            :  2 ; /* bits 1:0 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_BISTSTATUS4_t;

#define ARN5_MPIF_BISTSTATUS5                    0x00033
#define ARN5_MPIF_BISTSTATUS5_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 STAR_BIST_DONE       :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd2               :  6 ;
    cs_uint16 STAR_BIST_BRIDGE     :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  8 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 STAR_BIST_BRIDGE     :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  6 ;
    cs_uint16 STAR_BIST_DONE       :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_BISTSTATUS5_t;

#define ARN5_MPIF_BISTSTARCTL                    0x00034
#define ARN5_MPIF_BISTSTARCTL_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 ACTIVATE             :  1 ; /* bits 15:15 */
    cs_uint16 CODE                 :  3 ; /* bits 14:12 */
    cs_uint16 NUM_SHIFT            :  8 ; /* bits 11:4 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 MEMENABLE            :  1 ; /* bits 1:1 */
    cs_uint16 RET                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RET                  :  1 ; /* bits 0:0 */
    cs_uint16 MEMENABLE            :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 NUM_SHIFT            :  8 ; /* bits 11:4 */
    cs_uint16 CODE                 :  3 ; /* bits 14:12 */
    cs_uint16 ACTIVATE             :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_BISTSTARCTL_t;
#define     ARN5_CODE_STAR_CAP                            0x00
#define     ARN5_CODE_STAR_UP                             0x01
#define     ARN5_CODE_STAR_SI                             0x02
#define     ARN5_CODE_STAR_SO                             0x03
#define     ARN5_CODE_STAR_CAPDR                          0x04
#define     ARN5_CODE_RSVD_UPDR                           0x05
#define     ARN5_CODE_RSVD_SIDR                           0x06
#define     ARN5_CODE_RSVD_SODR                           0x07

#define ARN5_MPIF_BISTSTARDIN                    0x00035
#define ARN5_MPIF_BISTSTARDIN_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 WIR_data_in          : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 WIR_data_in          : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_BISTSTARDIN_t;

#define ARN5_MPIF_BISTSTARDOUT                   0x00036
#define ARN5_MPIF_BISTSTARDOUT_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 WIR_data_out         : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 WIR_data_out         : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_BISTSTARDOUT_t;

#define ARN5_MPIF_DebugClk                       0x00037
#define ARN5_MPIF_DebugClk_dft                   0x007f
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 FMRCLK_E             :  1 ; /* bits 7:7 */
    cs_uint16 clksel               :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 clksel               :  7 ; /* bits 6:0 */
    cs_uint16 FMRCLK_E             :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_DebugClk_t;
#define     ARN5_clksel_mpif_clk                          0x40
#define     ARN5_clksel_clk_50                            0x48
#define     ARN5_clksel_clk_200_a                         0x50
#define     ARN5_clksel_clk_200_b                         0x51
#define     ARN5_clksel_clk_200_c                         0x52
#define     ARN5_clksel_Bridge_fmrclk_a                   0x58
#define     ARN5_clksel_Bridge_fmrclk_b                   0x59
#define     ARN5_clksel_Bridge_fmrclk_c                   0x5a
#define     ARN5_clksel_Process_Monitor_a                 0x60
#define     ARN5_clksel_Process_Monitor_b                 0x61
#define     ARN5_clksel_Process_Monitor_c                 0x62
#define     ARN5_clksel_Low_Logic_Level                   0x7f
#define     ARN5_clksel_LIF_0_fmrclk_a                    0x00
#define     ARN5_clksel_LIF_0_fmrclk_b                    0x01
#define     ARN5_clksel_LIF_0_fmrclk_c                    0x02
#define     ARN5_clksel_GigE_0_fmrclk_a                   0x04
#define     ARN5_clksel_GigE_0_fmrclk_b                   0x05
#define     ARN5_clksel_GigE_0_fmrclk_c                   0x06
#define     ARN5_clksel_LIF_1_fmrclk_a                    0x08
#define     ARN5_clksel_LIF_1_fmrclk_b                    0x09
#define     ARN5_clksel_LIF_1_fmrclk_c                    0x0a
#define     ARN5_clksel_GigE_1_fmrclk_a                   0x0c
#define     ARN5_clksel_GigE_1_fmrclk_b                   0x0d
#define     ARN5_clksel_GigE_1_fmrclk_c                   0x0e
#define     ARN5_clksel_LIF_2_fmrclk_a                    0x10
#define     ARN5_clksel_LIF_2_fmrclk_b                    0x11
#define     ARN5_clksel_LIF_2_fmrclk_c                    0x12
#define     ARN5_clksel_GigE_2_fmrclk_a                   0x14
#define     ARN5_clksel_GigE_2_fmrclk_b                   0x15
#define     ARN5_clksel_GigE_2_fmrclk_c                   0x16
#define     ARN5_clksel_LIF_3_fmrclk_a                    0x18
#define     ARN5_clksel_LIF_3_fmrclk_b                    0x19
#define     ARN5_clksel_LIF_3_fmrclk_c                    0x1a
#define     ARN5_clksel_GigE_3_fmrclk_a                   0x1c
#define     ARN5_clksel_GigE_3_fmrclk_b                   0x1d
#define     ARN5_clksel_GigE_3_fmrclk_c                   0x1e
#define     ARN5_clksel_LIF_4_fmrclk_a                    0x20
#define     ARN5_clksel_LIF_4_fmrclk_b                    0x21
#define     ARN5_clksel_LIF_4_fmrclk_c                    0x22
#define     ARN5_clksel_GigE_4_fmrclk_a                   0x24
#define     ARN5_clksel_GigE_4_fmrclk_b                   0x25
#define     ARN5_clksel_GigE_4_fmrclk_c                   0x26
#define     ARN5_clksel_LIF_5_fmrclk_a                    0x28
#define     ARN5_clksel_LIF_5_fmrclk_b                    0x29
#define     ARN5_clksel_LIF_5_fmrclk_c                    0x2a
#define     ARN5_clksel_GigE_5_fmrclk_a                   0x2c
#define     ARN5_clksel_GigE_5_fmrclk_b                   0x2d
#define     ARN5_clksel_GigE_5_fmrclk_c                   0x2e
#define     ARN5_clksel_LIF_6_fmrclk_a                    0x30
#define     ARN5_clksel_LIF_6_fmrclk_b                    0x31
#define     ARN5_clksel_LIF_6_fmrclk_c                    0x32
#define     ARN5_clksel_GigE_6_fmrclk_a                   0x34
#define     ARN5_clksel_GigE_6_fmrclk_b                   0x35
#define     ARN5_clksel_GigE_6_fmrclk_c                   0x36
#define     ARN5_clksel_LIF_7_fmrclk_a                    0x38
#define     ARN5_clksel_LIF_7_fmrclk_b                    0x39
#define     ARN5_clksel_LIF_7_fmrclk_c                    0x3a
#define     ARN5_clksel_GigE_7_fmrclk_a                   0x3c
#define     ARN5_clksel_GigE_7_fmrclk_b                   0x3d
#define     ARN5_clksel_GigE_7_fmrclk_c                   0x3e

#define ARN5_MPIF_SysClkPLLControl               0x00038
#define ARN5_MPIF_SysClkPLLControl_dft           0x8010
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 FREQTUNE             :  4 ; /* bits 15:12 */
    cs_uint16 TCKI_EN              :  1 ; /* bits 11:11 */
    cs_uint16 TCKO_EN              :  1 ; /* bits 10:10 */
    cs_uint16 TCKO_SEL             :  3 ; /* bits 9:7 */
    cs_uint16 CHPI50UA_EN          :  1 ; /* bits 6:6 */
    cs_uint16 PWRDWN               :  1 ; /* bits 5:5 */
    cs_uint16 INT_FILTER_EN        :  1 ; /* bits 4:4 */
    cs_uint16 BYPASS_LOOP_RES      :  1 ; /* bits 3:3 */
    cs_uint16 FILTPN_EN            :  1 ; /* bits 2:2 */
    cs_uint16 SPARE                :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SPARE                :  2 ; /* bits 1:0 */
    cs_uint16 FILTPN_EN            :  1 ; /* bits 2:2 */
    cs_uint16 BYPASS_LOOP_RES      :  1 ; /* bits 3:3 */
    cs_uint16 INT_FILTER_EN        :  1 ; /* bits 4:4 */
    cs_uint16 PWRDWN               :  1 ; /* bits 5:5 */
    cs_uint16 CHPI50UA_EN          :  1 ; /* bits 6:6 */
    cs_uint16 TCKO_SEL             :  3 ; /* bits 9:7 */
    cs_uint16 TCKO_EN              :  1 ; /* bits 10:10 */
    cs_uint16 TCKI_EN              :  1 ; /* bits 11:11 */
    cs_uint16 FREQTUNE             :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_SysClkPLLControl_t;
#define     ARN5_TCKO_SEL_CKOUT_PH1                       0x00
#define     ARN5_TCKO_SEL_CKOUT_DIV2                      0x04
#define     ARN5_TCKO_SEL_CKOUT_DIV4                      0x05
#define     ARN5_TCKO_SEL_CKOUT_DIV8                      0x06
#define     ARN5_TCKO_SEL_CKOUT_DIV16                     0x07

#define ARN5_MPIF_PMONControl                    0x00039
#define ARN5_MPIF_PMONControl_dft                0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 pmon_en              :  1 ; /* bits 8:8 */
    cs_uint16 m7_C_en              :  1 ; /* bits 7:7 */
    cs_uint16 m6_C_en              :  1 ; /* bits 6:6 */
    cs_uint16 m5_C_en              :  1 ; /* bits 5:5 */
    cs_uint16 m4_C_en              :  1 ; /* bits 4:4 */
    cs_uint16 m3_C_en              :  1 ; /* bits 3:3 */
    cs_uint16 m2_C_en              :  1 ; /* bits 2:2 */
    cs_uint16 m1_C_en              :  1 ; /* bits 1:1 */
    cs_uint16 vco_highC_en         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 vco_highC_en         :  1 ; /* bits 0:0 */
    cs_uint16 m1_C_en              :  1 ; /* bits 1:1 */
    cs_uint16 m2_C_en              :  1 ; /* bits 2:2 */
    cs_uint16 m3_C_en              :  1 ; /* bits 3:3 */
    cs_uint16 m4_C_en              :  1 ; /* bits 4:4 */
    cs_uint16 m5_C_en              :  1 ; /* bits 5:5 */
    cs_uint16 m6_C_en              :  1 ; /* bits 6:6 */
    cs_uint16 m7_C_en              :  1 ; /* bits 7:7 */
    cs_uint16 pmon_en              :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_PMONControl_t;
#define     ARN5_m7_C_en_DIS                              0x00
#define     ARN5_m7_C_en_EN                               0x01
#define     ARN5_m6_C_en_DIS                              0x00
#define     ARN5_m6_C_en_EN                               0x01
#define     ARN5_m5_C_en_DIS                              0x00
#define     ARN5_m5_C_en_EN                               0x01
#define     ARN5_m4_C_en_DIS                              0x00
#define     ARN5_m4_C_en_EN                               0x01
#define     ARN5_m3_C_en_DIS                              0x00
#define     ARN5_m3_C_en_EN                               0x01
#define     ARN5_m2_C_en_DIS                              0x00
#define     ARN5_m2_C_en_EN                               0x01
#define     ARN5_m1_C_en_DIS                              0x00
#define     ARN5_m1_C_en_EN                               0x01
#define     ARN5_vco_highC_en_GND                         0x00
#define     ARN5_vco_highC_en_MET                         0x01

#define ARN5_MPIF_MPIF_INTERRUPT                 0x0003a
#define ARN5_MPIF_MPIF_INTERRUPT_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 BUSERRI              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 BUSERRI              :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_MPIF_INTERRUPT_t;

#define ARN5_MPIF_MPIF_INTENABLE                 0x0003b
#define ARN5_MPIF_MPIF_INTENABLE_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 BUSERRE              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 BUSERRE              :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_MPIF_INTENABLE_t;

#define ARN5_MPIF_TIMEOUT_ADDRESS1               0x0003c
#define ARN5_MPIF_TIMEOUT_ADDRESS1_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 address1             :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 address1             :  4 ; /* bits 3:0 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_TIMEOUT_ADDRESS1_t;

#define ARN5_MPIF_TIMEOUT_ADDRESS0               0x0003d
#define ARN5_MPIF_TIMEOUT_ADDRESS0_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 address0             : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 address0             : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_TIMEOUT_ADDRESS0_t;

#define ARN5_MPIF_TIMEOUT_TRANSACTION            0x0003e
#define ARN5_MPIF_TIMEOUT_TRANSACTION_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 rwn                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rwn                  :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_TIMEOUT_TRANSACTION_t;

#define ARN5_MPIF_PREV_ADDRESS1                  0x0003f
#define ARN5_MPIF_PREV_ADDRESS1_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 address1             :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 address1             :  4 ; /* bits 3:0 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_PREV_ADDRESS1_t;

#define ARN5_MPIF_PREV_ADDRESS0                  0x00040
#define ARN5_MPIF_PREV_ADDRESS0_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 address0             : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 address0             : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_PREV_ADDRESS0_t;

#define ARN5_MPIF_PREV_TRANSACTION               0x00041
#define ARN5_MPIF_PREV_TRANSACTION_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 rwn                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rwn                  :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_PREV_TRANSACTION_t;

#define ARN5_MPIF_PREV_DATA                      0x00042
#define ARN5_MPIF_PREV_DATA_dft                  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 data                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 data                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_PREV_DATA_t;

#define ARN5_MPIF_clk_count_ctrl                 0x00043
#define ARN5_MPIF_clk_count_ctrl_dft             0x0110
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd3               :  4 ;
    cs_uint16 spare                :  4 ; /* bits 11:8 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 free_run             :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 go                   :  1 ; /* bits 1:1 */
    cs_uint16 reset                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 reset                :  1 ; /* bits 0:0 */
    cs_uint16 go                   :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 free_run             :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 spare                :  4 ; /* bits 11:8 */
    cs_uint16 rsrvd3               :  4 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_clk_count_ctrl_t;

#define ARN5_MPIF_mpifclk_count_duration         0x00044
#define ARN5_MPIF_mpifclk_count_duration_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_mpifclk_count_duration_t;

#define ARN5_MPIF_clk_count_status               0x00045
#define ARN5_MPIF_clk_count_status_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 ovrflw               :  1 ; /* bits 3:3 */
    cs_uint16 cmp                  :  2 ; /* bits 2:1 */
    cs_uint16 done                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 done                 :  1 ; /* bits 0:0 */
    cs_uint16 cmp                  :  2 ; /* bits 2:1 */
    cs_uint16 ovrflw               :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_clk_count_status_t;
#define     ARN5_cmp_INIT                                 0x00
#define     ARN5_cmp_LT                                   0x01
#define     ARN5_cmp_BTWN                                 0x02
#define     ARN5_cmp_GT                                   0x03

#define ARN5_MPIF_clk_cycle_count1               0x00046
#define ARN5_MPIF_clk_cycle_count1_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count1               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count1               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_clk_cycle_count1_t;

#define ARN5_MPIF_clk_cycle_count0               0x00047
#define ARN5_MPIF_clk_cycle_count0_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count0               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count0               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_clk_cycle_count0_t;

#define ARN5_MPIF_clk_cycle_min1                 0x00048
#define ARN5_MPIF_clk_cycle_min1_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value1               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value1               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_clk_cycle_min1_t;

#define ARN5_MPIF_clk_cycle_min0                 0x00049
#define ARN5_MPIF_clk_cycle_min0_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value0               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value0               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_clk_cycle_min0_t;

#define ARN5_MPIF_clk_cycle_max1                 0x0004a
#define ARN5_MPIF_clk_cycle_max1_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value1               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value1               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_clk_cycle_max1_t;

#define ARN5_MPIF_clk_cycle_max0                 0x0004b
#define ARN5_MPIF_clk_cycle_max0_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value0               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value0               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_MPIF_clk_cycle_max0_t;


/******************************************************************
 *
 * Block declaration.
 *
 ******************************************************************/

/* MicroIF */

typedef struct {
  ARN5_MPIF_CHIPIdLsb_t                    CHIPIdLsb;                     /* addr: 0x0000 */
  ARN5_MPIF_CHIPIdMsb_t                    CHIPIdMsb;                     /* addr: 0x0001 */
  ARN5_MPIF_GlobalReset0_t                 GlobalReset0;                  /* addr: 0x0002 */
  ARN5_MPIF_GlobalReset1_t                 GlobalReset1;                  /* addr: 0x0003 */
  ARN5_MPIF_GlobalReset2_t                 GlobalReset2;                  /* addr: 0x0004 */
  ARN5_MPIF_GlobalInt_t                    GlobalInt;                     /* addr: 0x0005 */
  ARN5_MPIF_GlobalIntEnable_t              GlobalIntEnable;               /* addr: 0x0006 */
  ARN5_MPIF_Channel0Int_t                  Channel0Int;                   /* addr: 0x0007 */
  ARN5_MPIF_Channel0IntEnable_t            Channel0IntEnable;             /* addr: 0x0008 */
  ARN5_MPIF_Channel1Int_t                  Channel1Int;                   /* addr: 0x0009 */
  ARN5_MPIF_Channel1IntEnable_t            Channel1IntEnable;             /* addr: 0x000a */
  ARN5_MPIF_Channel2Int_t                  Channel2Int;                   /* addr: 0x000b */
  ARN5_MPIF_Channel2IntEnable_t            Channel2IntEnable;             /* addr: 0x000c */
  ARN5_MPIF_Channel3Int_t                  Channel3Int;                   /* addr: 0x000d */
  ARN5_MPIF_Channel3IntEnable_t            Channel3IntEnable;             /* addr: 0x000e */
  ARN5_MPIF_Channel4Int_t                  Channel4Int;                   /* addr: 0x000f */
  ARN5_MPIF_Channel4IntEnable_t            Channel4IntEnable;             /* addr: 0x0010 */
  ARN5_MPIF_Channel5Int_t                  Channel5Int;                   /* addr: 0x0011 */
  ARN5_MPIF_Channel5IntEnable_t            Channel5IntEnable;             /* addr: 0x0012 */
  ARN5_MPIF_Channel6Int_t                  Channel6Int;                   /* addr: 0x0013 */
  ARN5_MPIF_Channel6IntEnable_t            Channel6IntEnable;             /* addr: 0x0014 */
  ARN5_MPIF_Channel7Int_t                  Channel7Int;                   /* addr: 0x0015 */
  ARN5_MPIF_Channel7IntEnable_t            Channel7IntEnable;             /* addr: 0x0016 */
  ARN5_MPIF_ChannelMode_t                  ChannelMode;                   /* addr: 0x0017 */
  ARN5_MPIF_SONETInterface_t               SONETInterface;                /* addr: 0x0018 */
  ARN5_MPIF_PowerdownControl_t             PowerdownControl;              /* addr: 0x0019 */
  ARN5_MPIF_SysClk400Phase_t               SysClk400Phase;                /* addr: 0x001a */
  ARN5_MPIF_SyncClk_t                      SyncClk;                       /* addr: 0x001b */
  ARN5_MPIF_SMHControl0_t                  SMHControl0;                   /* addr: 0x001c */
  ARN5_MPIF_SMHControl1_t                  SMHControl1;                   /* addr: 0x001d */
  ARN5_MPIF_GPIODirection0_t               GPIODirection0;                /* addr: 0x001e */
  ARN5_MPIF_GPIODirection1_t               GPIODirection1;                /* addr: 0x001f */
  ARN5_MPIF_GPIOAlarmControl_t             GPIOAlarmControl;              /* addr: 0x0020 */
  ARN5_MPIF_GPIOFmroControl_t              GPIOFmroControl;               /* addr: 0x0021 */
  ARN5_MPIF_GPIOOutput0_t                  GPIOOutput0;                   /* addr: 0x0022 */
  ARN5_MPIF_GPIOOutput1_t                  GPIOOutput1;                   /* addr: 0x0023 */
  ARN5_MPIF_GPIOSample0_t                  GPIOSample0;                   /* addr: 0x0024 */
  ARN5_MPIF_GPIOSample1_t                  GPIOSample1;                   /* addr: 0x0025 */
  ARN5_MPIF_GPIORInt0_t                    GPIORInt0;                     /* addr: 0x0026 */
  ARN5_MPIF_GPIORInt1_t                    GPIORInt1;                     /* addr: 0x0027 */
  ARN5_MPIF_GPIOFInt0_t                    GPIOFInt0;                     /* addr: 0x0028 */
  ARN5_MPIF_GPIOFInt1_t                    GPIOFInt1;                     /* addr: 0x0029 */
  ARN5_MPIF_GPIORMask0_t                   GPIORMask0;                    /* addr: 0x002a */
  ARN5_MPIF_GPIORMask1_t                   GPIORMask1;                    /* addr: 0x002b */
  ARN5_MPIF_GPIOFMask0_t                   GPIOFMask0;                    /* addr: 0x002c */
  ARN5_MPIF_GPIOFMask1_t                   GPIOFMask1;                    /* addr: 0x002d */
  ARN5_MPIF_BISTCONTROL_t                  BISTCONTROL;                   /* addr: 0x002e */
  ARN5_MPIF_BISTSTATUS0_t                  BISTSTATUS0;                   /* addr: 0x002f */
  ARN5_MPIF_BISTSTATUS1_t                  BISTSTATUS1;                   /* addr: 0x0030 */
  ARN5_MPIF_BISTSTATUS3_t                  BISTSTATUS3;                   /* addr: 0x0031 */
  ARN5_MPIF_BISTSTATUS4_t                  BISTSTATUS4;                   /* addr: 0x0032 */
  ARN5_MPIF_BISTSTATUS5_t                  BISTSTATUS5;                   /* addr: 0x0033 */
  ARN5_MPIF_BISTSTARCTL_t                  BISTSTARCTL;                   /* addr: 0x0034 */
  ARN5_MPIF_BISTSTARDIN_t                  BISTSTARDIN;                   /* addr: 0x0035 */
  ARN5_MPIF_BISTSTARDOUT_t                 BISTSTARDOUT;                  /* addr: 0x0036 */
  ARN5_MPIF_DebugClk_t                     DebugClk;                      /* addr: 0x0037 */
  ARN5_MPIF_SysClkPLLControl_t             SysClkPLLControl;              /* addr: 0x0038 */
  ARN5_MPIF_PMONControl_t                  PMONControl;                   /* addr: 0x0039 */
  ARN5_MPIF_MPIF_INTERRUPT_t               MPIF_INTERRUPT;                /* addr: 0x003a */
  ARN5_MPIF_MPIF_INTENABLE_t               MPIF_INTENABLE;                /* addr: 0x003b */
  ARN5_MPIF_TIMEOUT_ADDRESS1_t             TIMEOUT_ADDRESS1;              /* addr: 0x003c */
  ARN5_MPIF_TIMEOUT_ADDRESS0_t             TIMEOUT_ADDRESS0;              /* addr: 0x003d */
  ARN5_MPIF_TIMEOUT_TRANSACTION_t          TIMEOUT_TRANSACTION;           /* addr: 0x003e */
  ARN5_MPIF_PREV_ADDRESS1_t                PREV_ADDRESS1;                 /* addr: 0x003f */
  ARN5_MPIF_PREV_ADDRESS0_t                PREV_ADDRESS0;                 /* addr: 0x0040 */
  ARN5_MPIF_PREV_TRANSACTION_t             PREV_TRANSACTION;              /* addr: 0x0041 */
  ARN5_MPIF_PREV_DATA_t                    PREV_DATA;                     /* addr: 0x0042 */
  ARN5_MPIF_clk_count_ctrl_t               clk_count_ctrl;                /* addr: 0x0043 */
  ARN5_MPIF_mpifclk_count_duration_t       mpifclk_count_duration;        /* addr: 0x0044 */
  ARN5_MPIF_clk_count_status_t             clk_count_status;              /* addr: 0x0045 */
  ARN5_MPIF_clk_cycle_count1_t             clk_cycle_count1;              /* addr: 0x0046 */
  ARN5_MPIF_clk_cycle_count0_t             clk_cycle_count0;              /* addr: 0x0047 */
  ARN5_MPIF_clk_cycle_min1_t               clk_cycle_min1;                /* addr: 0x0048 */
  ARN5_MPIF_clk_cycle_min0_t               clk_cycle_min0;                /* addr: 0x0049 */
  ARN5_MPIF_clk_cycle_max1_t               clk_cycle_max1;                /* addr: 0x004a */
  ARN5_MPIF_clk_cycle_max0_t               clk_cycle_max0;                /* addr: 0x004b */
} ARN5_MicroIF_t;

/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * SPI4P2::core
 */

#define ARN5_SPI4P2_CORE_status                  0x08000
#define ARN5_SPI4P2_CORE_status_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd3               :  2 ;
    cs_uint16 spi4p2rxstate        :  2 ; /* bits 13:12 */
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 spi4p2txstate        :  2 ; /* bits 9:8 */
    cs_uint16 rsrvd1               :  8 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 spi4p2txstate        :  2 ; /* bits 9:8 */
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 spi4p2rxstate        :  2 ; /* bits 13:12 */
    cs_uint16 rsrvd3               :  2 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_CORE_status_t;
#define     ARN5_spi4p2rxstate_INIT                       0x00
#define     ARN5_spi4p2rxstate_OUT_OF_SYNC                0x01
#define     ARN5_spi4p2rxstate_SYNC                       0x02
#define     ARN5_spi4p2rxstate_reserved                   0x03
#define     ARN5_spi4p2txstate_INIT                       0x00
#define     ARN5_spi4p2txstate_OUT_OF_SYNC                0x01
#define     ARN5_spi4p2txstate_SYNC                       0x02
#define     ARN5_spi4p2txstate_reserved                   0x03

#define ARN5_SPI4P2_CORE_control                 0x08001
#define ARN5_SPI4P2_CORE_control_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 12 ;
    cs_uint16 rxsynreset           :  1 ; /* bits 3:3 */
    cs_uint16 txsynreset           :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 reset                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 reset                :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 txsynreset           :  1 ; /* bits 2:2 */
    cs_uint16 rxsynreset           :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd2               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_CORE_control_t;

#define ARN5_SPI4P2_CORE_scratch                 0x08002
#define ARN5_SPI4P2_CORE_scratch_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 scratch              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 scratch              : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_CORE_scratch_t;

#define ARN5_SPI4P2_CORE_intrpt                  0x08003
#define ARN5_SPI4P2_CORE_intrpt_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd3               :  4 ;
    cs_uint16 rxif_intrpt          :  1 ; /* bits 11:11 */
    cs_uint16 rx_rprif_intrpt_vec  :  1 ; /* bits 10:10 */
    cs_uint16 rxint_intrpt_vec     :  1 ; /* bits 9:9 */
    cs_uint16 rxint_rxintrpt       :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 membist_intrpt       :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 tx_rprif_afifo_intrpt :  1 ; /* bits 2:2 */
    cs_uint16 txint_intrpt_vec     :  1 ; /* bits 1:1 */
    cs_uint16 txif_txintrpt        :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 txif_txintrpt        :  1 ; /* bits 0:0 */
    cs_uint16 txint_intrpt_vec     :  1 ; /* bits 1:1 */
    cs_uint16 tx_rprif_afifo_intrpt :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 membist_intrpt       :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 rxint_rxintrpt       :  1 ; /* bits 8:8 */
    cs_uint16 rxint_intrpt_vec     :  1 ; /* bits 9:9 */
    cs_uint16 rx_rprif_intrpt_vec  :  1 ; /* bits 10:10 */
    cs_uint16 rxif_intrpt          :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd3               :  4 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_CORE_intrpt_t;

#define ARN5_SPI4P2_CORE_intrpten                0x08004
#define ARN5_SPI4P2_CORE_intrpten_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd3               :  4 ;
    cs_uint16 int_en11             :  1 ; /* bits 11:11 */
    cs_uint16 int_en10             :  1 ; /* bits 10:10 */
    cs_uint16 int_en9              :  1 ; /* bits 9:9 */
    cs_uint16 int_en8              :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 int_en6              :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 int_en2              :  1 ; /* bits 2:2 */
    cs_uint16 int_en1              :  1 ; /* bits 1:1 */
    cs_uint16 int_en0              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 int_en0              :  1 ; /* bits 0:0 */
    cs_uint16 int_en1              :  1 ; /* bits 1:1 */
    cs_uint16 int_en2              :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 int_en6              :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 int_en8              :  1 ; /* bits 8:8 */
    cs_uint16 int_en9              :  1 ; /* bits 9:9 */
    cs_uint16 int_en10             :  1 ; /* bits 10:10 */
    cs_uint16 int_en11             :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd3               :  4 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_CORE_intrpten_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * SPI4P2::txif
 */

#define ARN5_SPI4P2_TXIF_dynalign                0x08100
#define ARN5_SPI4P2_TXIF_dynalign_dft            0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 enable               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 enable               :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_dynalign_t;

#define ARN5_SPI4P2_TXIF_numdip4err              0x08101
#define ARN5_SPI4P2_TXIF_numdip4err_dft          0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 value                :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  4 ; /* bits 3:0 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_numdip4err_t;

#define ARN5_SPI4P2_TXIF_numdip4match            0x08102
#define ARN5_SPI4P2_TXIF_numdip4match_dft        0x0c80
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_numdip4match_t;

#define ARN5_SPI4P2_TXIF_txbuf_almostfull        0x08103
#define ARN5_SPI4P2_TXIF_txbuf_almostfull_dft    0x001f
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 value                :  9 ; /* bits 8:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  9 ; /* bits 8:0 */
    cs_uint16 rsrvd1               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txbuf_almostfull_t;

#define ARN5_SPI4P2_TXIF_spi42ctl                0x08104
#define ARN5_SPI4P2_TXIF_spi42ctl_dft            0x0030
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  7 ;
    cs_uint16 tx_trn_seq_strict_mode :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 disable_sop_dist_err_close_all_pkt :  1 ; /* bits 5:5 */
    cs_uint16 disable_burst_len_err_close_all_pkt :  1 ; /* bits 4:4 */
    cs_uint16 disable_rsvd_word_close_all_pkt :  1 ; /* bits 3:3 */
    cs_uint16 ignore_sop_distance_deletion :  1 ; /* bits 2:2 */
    cs_uint16 disable_seq_err_close_all_pkt :  1 ; /* bits 1:1 */
    cs_uint16 disable_dip4_close_all_pkt :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 disable_dip4_close_all_pkt :  1 ; /* bits 0:0 */
    cs_uint16 disable_seq_err_close_all_pkt :  1 ; /* bits 1:1 */
    cs_uint16 ignore_sop_distance_deletion :  1 ; /* bits 2:2 */
    cs_uint16 disable_rsvd_word_close_all_pkt :  1 ; /* bits 3:3 */
    cs_uint16 disable_burst_len_err_close_all_pkt :  1 ; /* bits 4:4 */
    cs_uint16 disable_sop_dist_err_close_all_pkt :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 tx_trn_seq_strict_mode :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_spi42ctl_t;

#define ARN5_SPI4P2_TXIF_spi3ctl                 0x08105
#define ARN5_SPI4P2_TXIF_spi3ctl_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  5 ;
    cs_uint16 rx2tx                :  1 ; /* bits 10:10 */
    cs_uint16 tx2rx                :  1 ; /* bits 9:9 */
    cs_uint16 phylink_flip         :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 stpa_enable          :  1 ; /* bits 2:2 */
    cs_uint16 ptpa_enable          :  1 ; /* bits 1:1 */
    cs_uint16 Eightbmode           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Eightbmode           :  1 ; /* bits 0:0 */
    cs_uint16 ptpa_enable          :  1 ; /* bits 1:1 */
    cs_uint16 stpa_enable          :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 phylink_flip         :  1 ; /* bits 8:8 */
    cs_uint16 tx2rx                :  1 ; /* bits 9:9 */
    cs_uint16 rx2tx                :  1 ; /* bits 10:10 */
    cs_uint16 rsrvd2               :  5 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_spi3ctl_t;

#define ARN5_SPI4P2_TXIF_fifo_eccerr_ctrl        0x08106
#define ARN5_SPI4P2_TXIF_fifo_eccerr_ctrl_dft    0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 txbuf_const_err_gen  :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 txbuf_err_gen        :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 txbuf_err_gen        :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 txbuf_const_err_gen  :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_fifo_eccerr_ctrl_t;
#define     ARN5_txbuf_const_err_gen_SINGLE               0x00
#define     ARN5_txbuf_const_err_gen_CONST                0x01

#define ARN5_SPI4P2_TXIF_fifo_eccfix_ctrl        0x08107
#define ARN5_SPI4P2_TXIF_fifo_eccfix_ctrl_dft    0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 txbuf_const_fix_gen  :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 txbuf_fix_gen        :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 txbuf_fix_gen        :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 txbuf_const_fix_gen  :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_fifo_eccfix_ctrl_t;
#define     ARN5_txbuf_const_fix_gen_SINGLE               0x00
#define     ARN5_txbuf_const_fix_gen_CONST                0x01

#define ARN5_SPI4P2_TXIF_txintrpt                0x08108
#define ARN5_SPI4P2_TXIF_txintrpt_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 txbuf_fifo_ovrflw    :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd2               :  4 ;
    cs_uint16 txspi3_sx_err        :  1 ; /* bits 10:10 */
    cs_uint16 txspi3_adr_err       :  1 ; /* bits 9:9 */
    cs_uint16 txspi3_perr          :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 txrsvctrlwderr       :  1 ; /* bits 5:5 */
    cs_uint16 txseqerr             :  1 ; /* bits 4:4 */
    cs_uint16 txsopdisterr         :  1 ; /* bits 3:3 */
    cs_uint16 txburstsizeerr       :  1 ; /* bits 2:2 */
    cs_uint16 dip4err              :  1 ; /* bits 1:1 */
    cs_uint16 txlosyn              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 txlosyn              :  1 ; /* bits 0:0 */
    cs_uint16 dip4err              :  1 ; /* bits 1:1 */
    cs_uint16 txburstsizeerr       :  1 ; /* bits 2:2 */
    cs_uint16 txsopdisterr         :  1 ; /* bits 3:3 */
    cs_uint16 txseqerr             :  1 ; /* bits 4:4 */
    cs_uint16 txrsvctrlwderr       :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 txspi3_perr          :  1 ; /* bits 8:8 */
    cs_uint16 txspi3_adr_err       :  1 ; /* bits 9:9 */
    cs_uint16 txspi3_sx_err        :  1 ; /* bits 10:10 */
    cs_uint16 rsrvd2               :  4 ;
    cs_uint16 txbuf_fifo_ovrflw    :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txintrpt_t;

#define ARN5_SPI4P2_TXIF_txintrpten              0x08109
#define ARN5_SPI4P2_TXIF_txintrpten_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 txbuf_fifo_ovrflw_en :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd2               :  4 ;
    cs_uint16 txspi3_sx_err_en     :  1 ; /* bits 10:10 */
    cs_uint16 txspi3_adr_err_en    :  1 ; /* bits 9:9 */
    cs_uint16 txspi3_perr_en       :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 txrsvctrlwderr_en    :  1 ; /* bits 5:5 */
    cs_uint16 txseqerr_en          :  1 ; /* bits 4:4 */
    cs_uint16 txsopdisterr_en      :  1 ; /* bits 3:3 */
    cs_uint16 txburstsizeerr_en    :  1 ; /* bits 2:2 */
    cs_uint16 dip4err_en           :  1 ; /* bits 1:1 */
    cs_uint16 txlosyn_en           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 txlosyn_en           :  1 ; /* bits 0:0 */
    cs_uint16 dip4err_en           :  1 ; /* bits 1:1 */
    cs_uint16 txburstsizeerr_en    :  1 ; /* bits 2:2 */
    cs_uint16 txsopdisterr_en      :  1 ; /* bits 3:3 */
    cs_uint16 txseqerr_en          :  1 ; /* bits 4:4 */
    cs_uint16 txrsvctrlwderr_en    :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 txspi3_perr_en       :  1 ; /* bits 8:8 */
    cs_uint16 txspi3_adr_err_en    :  1 ; /* bits 9:9 */
    cs_uint16 txspi3_sx_err_en     :  1 ; /* bits 10:10 */
    cs_uint16 rsrvd2               :  4 ;
    cs_uint16 txbuf_fifo_ovrflw_en :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txintrpten_t;

#define ARN5_SPI4P2_TXIF_txloscnt                0x0810a
#define ARN5_SPI4P2_TXIF_txloscnt_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txloscnt_t;

#define ARN5_SPI4P2_TXIF_dip4errcnt              0x0810b
#define ARN5_SPI4P2_TXIF_dip4errcnt_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_dip4errcnt_t;

#define ARN5_SPI4P2_TXIF_txburstsizeerr          0x0810c
#define ARN5_SPI4P2_TXIF_txburstsizeerr_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txburstsizeerr_t;

#define ARN5_SPI4P2_TXIF_txsopdisterr            0x0810d
#define ARN5_SPI4P2_TXIF_txsopdisterr_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txsopdisterr_t;

#define ARN5_SPI4P2_TXIF_txseqerr                0x0810e
#define ARN5_SPI4P2_TXIF_txseqerr_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txseqerr_t;

#define ARN5_SPI4P2_TXIF_txrsvctrlwderr          0x0810f
#define ARN5_SPI4P2_TXIF_txrsvctrlwderr_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txrsvctrlwderr_t;

#define ARN5_SPI4P2_TXIF_txdskwoverrideen0       0x08110
#define ARN5_SPI4P2_TXIF_txdskwoverrideen0_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 en                   : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 en                   : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txdskwoverrideen0_t;

#define ARN5_SPI4P2_TXIF_txdskwoverrideen1       0x08111
#define ARN5_SPI4P2_TXIF_txdskwoverrideen1_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 en                   :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 en                   :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txdskwoverrideen1_t;

#define ARN5_SPI4P2_TXIF_txdskwoverride0         0x08112
#define ARN5_SPI4P2_TXIF_txdskwoverride0_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 ddsk3                :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 ddsk2                :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 ddsk1                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 ddsk0                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ddsk0                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 ddsk1                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 ddsk2                :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 ddsk3                :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txdskwoverride0_t;

#define ARN5_SPI4P2_TXIF_txdskwoverride1         0x08113
#define ARN5_SPI4P2_TXIF_txdskwoverride1_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 ddsk7                :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 ddsk6                :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 ddsk5                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 ddsk4                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ddsk4                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 ddsk5                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 ddsk6                :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 ddsk7                :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txdskwoverride1_t;

#define ARN5_SPI4P2_TXIF_txdskwoverride2         0x08114
#define ARN5_SPI4P2_TXIF_txdskwoverride2_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 ddsk11               :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 ddsk10               :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 ddsk9                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 ddsk8                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ddsk8                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 ddsk9                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 ddsk10               :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 ddsk11               :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txdskwoverride2_t;

#define ARN5_SPI4P2_TXIF_txdskwoverride3         0x08115
#define ARN5_SPI4P2_TXIF_txdskwoverride3_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 ddsk15               :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 ddsk14               :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 ddsk13               :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 ddsk12               :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ddsk12               :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 ddsk13               :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 ddsk14               :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 ddsk15               :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txdskwoverride3_t;

#define ARN5_SPI4P2_TXIF_txdskwoverride4         0x08116
#define ARN5_SPI4P2_TXIF_txdskwoverride4_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 cdsk0                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 cdsk0                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txdskwoverride4_t;

#define ARN5_SPI4P2_TXIF_txdskw0                 0x08117
#define ARN5_SPI4P2_TXIF_txdskw0_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 dsk3                 :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 dsk2                 :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 dsk1                 :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 dsk0                 :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 dsk0                 :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 dsk1                 :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 dsk2                 :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 dsk3                 :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txdskw0_t;

#define ARN5_SPI4P2_TXIF_txdskw1                 0x08118
#define ARN5_SPI4P2_TXIF_txdskw1_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 dsk7                 :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 dsk6                 :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 dsk5                 :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 dsk4                 :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 dsk4                 :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 dsk5                 :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 dsk6                 :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 dsk7                 :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txdskw1_t;

#define ARN5_SPI4P2_TXIF_txdskw2                 0x08119
#define ARN5_SPI4P2_TXIF_txdskw2_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 dsk11                :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 dsk10                :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 dsk9                 :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 dsk8                 :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 dsk8                 :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 dsk9                 :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 dsk10                :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 dsk11                :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txdskw2_t;

#define ARN5_SPI4P2_TXIF_txdskw3                 0x0811a
#define ARN5_SPI4P2_TXIF_txdskw3_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 dsk15                :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 dsk14                :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 dsk13                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 dsk12                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 dsk12                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 dsk13                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 dsk14                :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 dsk15                :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txdskw3_t;

#define ARN5_SPI4P2_TXIF_txdskw4                 0x0811b
#define ARN5_SPI4P2_TXIF_txdskw4_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 csk0                 :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 csk0                 :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txdskw4_t;

#define ARN5_SPI4P2_TXIF_txdrctrl                0x0811c
#define ARN5_SPI4P2_TXIF_txdrctrl_dft            0x5c01
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 txhysteresis         :  4 ; /* bits 15:12 */
    cs_uint16 txmapsel             :  1 ; /* bits 11:11 */
    cs_uint16 txwdenable           :  1 ; /* bits 10:10 */
    cs_uint16 txregcontrol         : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 txregcontrol         : 10 ; /* bits 9:0 */
    cs_uint16 txwdenable           :  1 ; /* bits 10:10 */
    cs_uint16 txmapsel             :  1 ; /* bits 11:11 */
    cs_uint16 txhysteresis         :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txdrctrl_t;
#define     ARN5_txhysteresis_32X                         0x00
#define     ARN5_txhysteresis_17X                         0x01
#define     ARN5_txhysteresis_8X                          0x02
#define     ARN5_txhysteresis_4X                          0x03
#define     ARN5_txhysteresis_2X                          0x04
#define     ARN5_txhysteresis_1X                          0x05

#define ARN5_SPI4P2_TXIF_txoverride1             0x0811d
#define ARN5_SPI4P2_TXIF_txoverride1_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 tdat3                :  4 ; /* bits 15:12 */
    cs_uint16 tdat2                :  4 ; /* bits 11:8 */
    cs_uint16 tdat1                :  4 ; /* bits 7:4 */
    cs_uint16 tdat0                :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 tdat0                :  4 ; /* bits 3:0 */
    cs_uint16 tdat1                :  4 ; /* bits 7:4 */
    cs_uint16 tdat2                :  4 ; /* bits 11:8 */
    cs_uint16 tdat3                :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txoverride1_t;

#define ARN5_SPI4P2_TXIF_txoverride2             0x0811e
#define ARN5_SPI4P2_TXIF_txoverride2_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 tdat7                :  4 ; /* bits 15:12 */
    cs_uint16 tdat6                :  4 ; /* bits 11:8 */
    cs_uint16 tdat5                :  4 ; /* bits 7:4 */
    cs_uint16 tdat4                :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 tdat4                :  4 ; /* bits 3:0 */
    cs_uint16 tdat5                :  4 ; /* bits 7:4 */
    cs_uint16 tdat6                :  4 ; /* bits 11:8 */
    cs_uint16 tdat7                :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txoverride2_t;

#define ARN5_SPI4P2_TXIF_txoverride3             0x0811f
#define ARN5_SPI4P2_TXIF_txoverride3_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 tdat11               :  4 ; /* bits 15:12 */
    cs_uint16 tdat10               :  4 ; /* bits 11:8 */
    cs_uint16 tdat9                :  4 ; /* bits 7:4 */
    cs_uint16 tdat8                :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 tdat8                :  4 ; /* bits 3:0 */
    cs_uint16 tdat9                :  4 ; /* bits 7:4 */
    cs_uint16 tdat10               :  4 ; /* bits 11:8 */
    cs_uint16 tdat11               :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txoverride3_t;

#define ARN5_SPI4P2_TXIF_txoverride4             0x08120
#define ARN5_SPI4P2_TXIF_txoverride4_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 tdat15               :  4 ; /* bits 15:12 */
    cs_uint16 tdat14               :  4 ; /* bits 11:8 */
    cs_uint16 tdat13               :  4 ; /* bits 7:4 */
    cs_uint16 tdat12               :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 tdat12               :  4 ; /* bits 3:0 */
    cs_uint16 tdat13               :  4 ; /* bits 7:4 */
    cs_uint16 tdat14               :  4 ; /* bits 11:8 */
    cs_uint16 tdat15               :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txoverride4_t;

#define ARN5_SPI4P2_TXIF_txoverride5             0x08121
#define ARN5_SPI4P2_TXIF_txoverride5_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 sparein              :  4 ; /* bits 7:4 */
    cs_uint16 tctl                 :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 tctl                 :  4 ; /* bits 3:0 */
    cs_uint16 sparein              :  4 ; /* bits 7:4 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txoverride5_t;

#define ARN5_SPI4P2_TXIF_txfreeze1               0x08122
#define ARN5_SPI4P2_TXIF_txfreeze1_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 tdat15               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 tdat15               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txfreeze1_t;

#define ARN5_SPI4P2_TXIF_txfreeze2               0x08123
#define ARN5_SPI4P2_TXIF_txfreeze2_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 spare                :  1 ; /* bits 1:1 */
    cs_uint16 ctl                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ctl                  :  1 ; /* bits 0:0 */
    cs_uint16 spare                :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txfreeze2_t;

#define ARN5_SPI4P2_TXIF_txspare                 0x08124
#define ARN5_SPI4P2_TXIF_txspare_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 spare                :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 spare                :  4 ; /* bits 3:0 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txspare_t;

#define ARN5_SPI4P2_TXIF_txcurphase1             0x08125
#define ARN5_SPI4P2_TXIF_txcurphase1_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 tdat3                :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 tdat2                :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 tdat1                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 tdat0                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 tdat0                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 tdat1                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 tdat2                :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 tdat3                :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txcurphase1_t;

#define ARN5_SPI4P2_TXIF_txcurphase2             0x08126
#define ARN5_SPI4P2_TXIF_txcurphase2_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 tdat7                :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 tdat6                :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 tdat5                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 tdat4                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 tdat4                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 tdat5                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 tdat6                :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 tdat7                :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txcurphase2_t;

#define ARN5_SPI4P2_TXIF_txcurphase3             0x08127
#define ARN5_SPI4P2_TXIF_txcurphase3_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 tdat11               :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 tdat10               :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 tdat9                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 tdat8                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 tdat8                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 tdat9                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 tdat10               :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 tdat11               :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txcurphase3_t;

#define ARN5_SPI4P2_TXIF_txcurphase4             0x08128
#define ARN5_SPI4P2_TXIF_txcurphase4_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 tdat15               :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 tdat14               :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 tdat13               :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 tdat12               :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 tdat12               :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 tdat13               :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 tdat14               :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 tdat15               :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txcurphase4_t;

#define ARN5_SPI4P2_TXIF_txcurphase5             0x08129
#define ARN5_SPI4P2_TXIF_txcurphase5_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  9 ;
    cs_uint16 sparein              :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 tctl                 :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 tctl                 :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 sparein              :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd2               :  9 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_txcurphase5_t;

#define ARN5_SPI4P2_TXIF_biststart               0x0812a
#define ARN5_SPI4P2_TXIF_biststart_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 start                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 start                :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_biststart_t;

#define ARN5_SPI4P2_TXIF_bistenable0             0x0812b
#define ARN5_SPI4P2_TXIF_bistenable0_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 dat                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 dat                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bistenable0_t;

#define ARN5_SPI4P2_TXIF_bistenable1             0x0812c
#define ARN5_SPI4P2_TXIF_bistenable1_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 dclk                 :  1 ; /* bits 1:1 */
    cs_uint16 ctl                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ctl                  :  1 ; /* bits 0:0 */
    cs_uint16 dclk                 :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bistenable1_t;

#define ARN5_SPI4P2_TXIF_bistdone0               0x0812d
#define ARN5_SPI4P2_TXIF_bistdone0_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 dat                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 dat                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bistdone0_t;

#define ARN5_SPI4P2_TXIF_bistdone1               0x0812e
#define ARN5_SPI4P2_TXIF_bistdone1_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 dclk                 :  1 ; /* bits 1:1 */
    cs_uint16 ctl                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ctl                  :  1 ; /* bits 0:0 */
    cs_uint16 dclk                 :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bistdone1_t;

#define ARN5_SPI4P2_TXIF_bistpass0               0x0812f
#define ARN5_SPI4P2_TXIF_bistpass0_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 dat                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 dat                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bistpass0_t;

#define ARN5_SPI4P2_TXIF_bistpass1               0x08130
#define ARN5_SPI4P2_TXIF_bistpass1_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 spare                :  1 ; /* bits 1:1 */
    cs_uint16 ctl                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ctl                  :  1 ; /* bits 0:0 */
    cs_uint16 spare                :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bistpass1_t;

#define ARN5_SPI4P2_TXIF_bistphaseoverride0      0x08131
#define ARN5_SPI4P2_TXIF_bistphaseoverride0_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 dat                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 dat                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bistphaseoverride0_t;

#define ARN5_SPI4P2_TXIF_bistphaseoverride1      0x08132
#define ARN5_SPI4P2_TXIF_bistphaseoverride1_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 dclk                 :  1 ; /* bits 1:1 */
    cs_uint16 ctl                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ctl                  :  1 ; /* bits 0:0 */
    cs_uint16 dclk                 :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bistphaseoverride1_t;

#define ARN5_SPI4P2_TXIF_bisttesttime            0x08133
#define ARN5_SPI4P2_TXIF_bisttesttime_dft        0xf000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 testtime             : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 testtime             : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bisttesttime_t;

#define ARN5_SPI4P2_TXIF_bistsettletime          0x08134
#define ARN5_SPI4P2_TXIF_bistsettletime_dft      0x0200
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 settletime           : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 settletime           : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bistsettletime_t;

#define ARN5_SPI4P2_TXIF_bistpattern0            0x08135
#define ARN5_SPI4P2_TXIF_bistpattern0_dft        0xb5a0
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 pattern              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pattern              : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bistpattern0_t;

#define ARN5_SPI4P2_TXIF_bistpattern1            0x08136
#define ARN5_SPI4P2_TXIF_bistpattern1_dft        0xfd37
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 pattern              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pattern              : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bistpattern1_t;

#define ARN5_SPI4P2_TXIF_bistpattern2            0x08137
#define ARN5_SPI4P2_TXIF_bistpattern2_dft        0x8264
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 pattern              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pattern              : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bistpattern2_t;

#define ARN5_SPI4P2_TXIF_bistpattern3            0x08138
#define ARN5_SPI4P2_TXIF_bistpattern3_dft        0xded6
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 pattern              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pattern              : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bistpattern3_t;

#define ARN5_SPI4P2_TXIF_bistpattern4            0x08139
#define ARN5_SPI4P2_TXIF_bistpattern4_dft        0x93f4
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 pattern              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pattern              : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bistpattern4_t;

#define ARN5_SPI4P2_TXIF_bistpattern5            0x0813a
#define ARN5_SPI4P2_TXIF_bistpattern5_dft        0x5a09
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 pattern              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pattern              : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bistpattern5_t;

#define ARN5_SPI4P2_TXIF_bisterror0              0x0813b
#define ARN5_SPI4P2_TXIF_bisterror0_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 error2               :  5 ; /* bits 14:10 */
    cs_uint16 error1               :  5 ; /* bits 9:5 */
    cs_uint16 error0               :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 error0               :  5 ; /* bits 4:0 */
    cs_uint16 error1               :  5 ; /* bits 9:5 */
    cs_uint16 error2               :  5 ; /* bits 14:10 */
    cs_uint16 rsrvd1               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bisterror0_t;

#define ARN5_SPI4P2_TXIF_bisterror1              0x0813c
#define ARN5_SPI4P2_TXIF_bisterror1_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 error5               :  5 ; /* bits 14:10 */
    cs_uint16 error4               :  5 ; /* bits 9:5 */
    cs_uint16 error3               :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 error3               :  5 ; /* bits 4:0 */
    cs_uint16 error4               :  5 ; /* bits 9:5 */
    cs_uint16 error5               :  5 ; /* bits 14:10 */
    cs_uint16 rsrvd1               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bisterror1_t;

#define ARN5_SPI4P2_TXIF_bisterror2              0x0813d
#define ARN5_SPI4P2_TXIF_bisterror2_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 error8               :  5 ; /* bits 14:10 */
    cs_uint16 error7               :  5 ; /* bits 9:5 */
    cs_uint16 error6               :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 error6               :  5 ; /* bits 4:0 */
    cs_uint16 error7               :  5 ; /* bits 9:5 */
    cs_uint16 error8               :  5 ; /* bits 14:10 */
    cs_uint16 rsrvd1               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bisterror2_t;

#define ARN5_SPI4P2_TXIF_bisterror3              0x0813e
#define ARN5_SPI4P2_TXIF_bisterror3_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 error11              :  5 ; /* bits 14:10 */
    cs_uint16 error10              :  5 ; /* bits 9:5 */
    cs_uint16 error9               :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 error9               :  5 ; /* bits 4:0 */
    cs_uint16 error10              :  5 ; /* bits 9:5 */
    cs_uint16 error11              :  5 ; /* bits 14:10 */
    cs_uint16 rsrvd1               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bisterror3_t;

#define ARN5_SPI4P2_TXIF_bisterror4              0x0813f
#define ARN5_SPI4P2_TXIF_bisterror4_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 error14              :  5 ; /* bits 14:10 */
    cs_uint16 error13              :  5 ; /* bits 9:5 */
    cs_uint16 error12              :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 error12              :  5 ; /* bits 4:0 */
    cs_uint16 error13              :  5 ; /* bits 9:5 */
    cs_uint16 error14              :  5 ; /* bits 14:10 */
    cs_uint16 rsrvd1               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bisterror4_t;

#define ARN5_SPI4P2_TXIF_bisterror5              0x08140
#define ARN5_SPI4P2_TXIF_bisterror5_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 error17              :  5 ; /* bits 14:10 */
    cs_uint16 error16              :  5 ; /* bits 9:5 */
    cs_uint16 error15              :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 error15              :  5 ; /* bits 4:0 */
    cs_uint16 error16              :  5 ; /* bits 9:5 */
    cs_uint16 error17              :  5 ; /* bits 14:10 */
    cs_uint16 rsrvd1               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXIF_bisterror5_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * SPI4P2::rxif
 */

#define ARN5_SPI4P2_RXIF_dip4errctrl             0x08200
#define ARN5_SPI4P2_RXIF_dip4errctrl_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 conterr              :  1 ; /* bits 1:1 */
    cs_uint16 snglerr              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 snglerr              :  1 ; /* bits 0:0 */
    cs_uint16 conterr              :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXIF_dip4errctrl_t;

#define ARN5_SPI4P2_RXIF_spi3ctl                 0x08201
#define ARN5_SPI4P2_RXIF_spi3ctl_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  5 ;
    cs_uint16 rx2tx                :  1 ; /* bits 10:10 */
    cs_uint16 tx2rx                :  1 ; /* bits 9:9 */
    cs_uint16 phylink_flip         :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 npause               :  2 ; /* bits 4:3 */
    cs_uint16 stpa_enable          :  1 ; /* bits 2:2 */
    cs_uint16 ptpa_enable          :  1 ; /* bits 1:1 */
    cs_uint16 Eightbmode           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Eightbmode           :  1 ; /* bits 0:0 */
    cs_uint16 ptpa_enable          :  1 ; /* bits 1:1 */
    cs_uint16 stpa_enable          :  1 ; /* bits 2:2 */
    cs_uint16 npause               :  2 ; /* bits 4:3 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 phylink_flip         :  1 ; /* bits 8:8 */
    cs_uint16 tx2rx                :  1 ; /* bits 9:9 */
    cs_uint16 rx2tx                :  1 ; /* bits 10:10 */
    cs_uint16 rsrvd2               :  5 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXIF_spi3ctl_t;

#define ARN5_SPI4P2_RXIF_intrpt                  0x08202
#define ARN5_SPI4P2_RXIF_intrpt_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 Rxspi42_perr         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Rxspi42_perr         :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXIF_intrpt_t;

#define ARN5_SPI4P2_RXIF_intrpten                0x08203
#define ARN5_SPI4P2_RXIF_intrpten_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 Rxspi42_perr_en      :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Rxspi42_perr_en      :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXIF_intrpten_t;

#define ARN5_SPI4P2_RXIF_rxspi42_perrcnt         0x08204
#define ARN5_SPI4P2_RXIF_rxspi42_perrcnt_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXIF_rxspi42_perrcnt_t;

#define ARN5_SPI4P2_RXIF_rxphaseadj1             0x08205
#define ARN5_SPI4P2_RXIF_rxphaseadj1_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 rdat3                :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 rdat2                :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 rdat1                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 rdat0                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rdat0                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 rdat1                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 rdat2                :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 rdat3                :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXIF_rxphaseadj1_t;
#define     ARN5_rdat3_PHASE0                             0x00
#define     ARN5_rdat3_PHASE1                             0x01
#define     ARN5_rdat3_PHASE2                             0x03
#define     ARN5_rdat3_PHASE3                             0x02
#define     ARN5_rdat3_PHASE4                             0x06
#define     ARN5_rdat3_PHASE5                             0x07
#define     ARN5_rdat3_PHASE6                             0x05
#define     ARN5_rdat3_PHASE7                             0x04
#define     ARN5_rdat2_PHASE0                             0x00
#define     ARN5_rdat2_PHASE1                             0x01
#define     ARN5_rdat2_PHASE2                             0x03
#define     ARN5_rdat2_PHASE3                             0x02
#define     ARN5_rdat2_PHASE4                             0x06
#define     ARN5_rdat2_PHASE5                             0x07
#define     ARN5_rdat2_PHASE6                             0x05
#define     ARN5_rdat2_PHASE7                             0x04
#define     ARN5_rdat1_PHASE0                             0x00
#define     ARN5_rdat1_PHASE1                             0x01
#define     ARN5_rdat1_PHASE2                             0x03
#define     ARN5_rdat1_PHASE3                             0x02
#define     ARN5_rdat1_PHASE4                             0x06
#define     ARN5_rdat1_PHASE5                             0x07
#define     ARN5_rdat1_PHASE6                             0x05
#define     ARN5_rdat1_PHASE7                             0x04
#define     ARN5_rdat0_PHASE0                             0x00
#define     ARN5_rdat0_PHASE1                             0x01
#define     ARN5_rdat0_PHASE2                             0x03
#define     ARN5_rdat0_PHASE3                             0x02
#define     ARN5_rdat0_PHASE4                             0x06
#define     ARN5_rdat0_PHASE5                             0x07
#define     ARN5_rdat0_PHASE6                             0x05
#define     ARN5_rdat0_PHASE7                             0x04

#define ARN5_SPI4P2_RXIF_rxphaseadj2             0x08206
#define ARN5_SPI4P2_RXIF_rxphaseadj2_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 rdat7                :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 rdat6                :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 rdat5                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 rdat4                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rdat4                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 rdat5                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 rdat6                :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 rdat7                :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXIF_rxphaseadj2_t;
#define     ARN5_rdat7_PHASE0                             0x00
#define     ARN5_rdat7_PHASE1                             0x01
#define     ARN5_rdat7_PHASE2                             0x03
#define     ARN5_rdat7_PHASE3                             0x02
#define     ARN5_rdat7_PHASE4                             0x06
#define     ARN5_rdat7_PHASE5                             0x07
#define     ARN5_rdat7_PHASE6                             0x05
#define     ARN5_rdat7_PHASE7                             0x04
#define     ARN5_rdat6_PHASE0                             0x00
#define     ARN5_rdat6_PHASE1                             0x01
#define     ARN5_rdat6_PHASE2                             0x03
#define     ARN5_rdat6_PHASE3                             0x02
#define     ARN5_rdat6_PHASE4                             0x06
#define     ARN5_rdat6_PHASE5                             0x07
#define     ARN5_rdat6_PHASE6                             0x05
#define     ARN5_rdat6_PHASE7                             0x04
#define     ARN5_rdat5_PHASE0                             0x00
#define     ARN5_rdat5_PHASE1                             0x01
#define     ARN5_rdat5_PHASE2                             0x03
#define     ARN5_rdat5_PHASE3                             0x02
#define     ARN5_rdat5_PHASE4                             0x06
#define     ARN5_rdat5_PHASE5                             0x07
#define     ARN5_rdat5_PHASE6                             0x05
#define     ARN5_rdat5_PHASE7                             0x04
#define     ARN5_rdat4_PHASE0                             0x00
#define     ARN5_rdat4_PHASE1                             0x01
#define     ARN5_rdat4_PHASE2                             0x03
#define     ARN5_rdat4_PHASE3                             0x02
#define     ARN5_rdat4_PHASE4                             0x06
#define     ARN5_rdat4_PHASE5                             0x07
#define     ARN5_rdat4_PHASE6                             0x05
#define     ARN5_rdat4_PHASE7                             0x04

#define ARN5_SPI4P2_RXIF_rxphaseadj3             0x08207
#define ARN5_SPI4P2_RXIF_rxphaseadj3_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 rdat11               :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 rdat10               :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 rdat9                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 rdat8                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rdat8                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 rdat9                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 rdat10               :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 rdat11               :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXIF_rxphaseadj3_t;
#define     ARN5_rdat11_PHASE0                            0x00
#define     ARN5_rdat11_PHASE1                            0x01
#define     ARN5_rdat11_PHASE2                            0x03
#define     ARN5_rdat11_PHASE3                            0x02
#define     ARN5_rdat11_PHASE4                            0x06
#define     ARN5_rdat11_PHASE5                            0x07
#define     ARN5_rdat11_PHASE6                            0x05
#define     ARN5_rdat11_PHASE7                            0x04
#define     ARN5_rdat10_PHASE0                            0x00
#define     ARN5_rdat10_PHASE1                            0x01
#define     ARN5_rdat10_PHASE2                            0x03
#define     ARN5_rdat10_PHASE3                            0x02
#define     ARN5_rdat10_PHASE4                            0x06
#define     ARN5_rdat10_PHASE5                            0x07
#define     ARN5_rdat10_PHASE6                            0x05
#define     ARN5_rdat10_PHASE7                            0x04
#define     ARN5_rdat9_PHASE0                             0x00
#define     ARN5_rdat9_PHASE1                             0x01
#define     ARN5_rdat9_PHASE2                             0x03
#define     ARN5_rdat9_PHASE3                             0x02
#define     ARN5_rdat9_PHASE4                             0x06
#define     ARN5_rdat9_PHASE5                             0x07
#define     ARN5_rdat9_PHASE6                             0x05
#define     ARN5_rdat9_PHASE7                             0x04
#define     ARN5_rdat8_PHASE0                             0x00
#define     ARN5_rdat8_PHASE1                             0x01
#define     ARN5_rdat8_PHASE2                             0x03
#define     ARN5_rdat8_PHASE3                             0x02
#define     ARN5_rdat8_PHASE4                             0x06
#define     ARN5_rdat8_PHASE5                             0x07
#define     ARN5_rdat8_PHASE6                             0x05
#define     ARN5_rdat8_PHASE7                             0x04

#define ARN5_SPI4P2_RXIF_rxphaseadj4             0x08208
#define ARN5_SPI4P2_RXIF_rxphaseadj4_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 rdat15               :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 rdat14               :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 rdat13               :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 rdat12               :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rdat12               :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 rdat13               :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 rdat14               :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 rdat15               :  3 ; /* bits 14:12 */
    cs_uint16 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXIF_rxphaseadj4_t;
#define     ARN5_rdat15_PHASE0                            0x00
#define     ARN5_rdat15_PHASE1                            0x01
#define     ARN5_rdat15_PHASE2                            0x03
#define     ARN5_rdat15_PHASE3                            0x02
#define     ARN5_rdat15_PHASE4                            0x06
#define     ARN5_rdat15_PHASE5                            0x07
#define     ARN5_rdat15_PHASE6                            0x05
#define     ARN5_rdat15_PHASE7                            0x04
#define     ARN5_rdat14_PHASE0                            0x00
#define     ARN5_rdat14_PHASE1                            0x01
#define     ARN5_rdat14_PHASE2                            0x03
#define     ARN5_rdat14_PHASE3                            0x02
#define     ARN5_rdat14_PHASE4                            0x06
#define     ARN5_rdat14_PHASE5                            0x07
#define     ARN5_rdat14_PHASE6                            0x05
#define     ARN5_rdat14_PHASE7                            0x04
#define     ARN5_rdat13_PHASE0                            0x00
#define     ARN5_rdat13_PHASE1                            0x01
#define     ARN5_rdat13_PHASE2                            0x03
#define     ARN5_rdat13_PHASE3                            0x02
#define     ARN5_rdat13_PHASE4                            0x06
#define     ARN5_rdat13_PHASE5                            0x07
#define     ARN5_rdat13_PHASE6                            0x05
#define     ARN5_rdat13_PHASE7                            0x04
#define     ARN5_rdat12_PHASE0                            0x00
#define     ARN5_rdat12_PHASE1                            0x01
#define     ARN5_rdat12_PHASE2                            0x03
#define     ARN5_rdat12_PHASE3                            0x02
#define     ARN5_rdat12_PHASE4                            0x06
#define     ARN5_rdat12_PHASE5                            0x07
#define     ARN5_rdat12_PHASE6                            0x05
#define     ARN5_rdat12_PHASE7                            0x04

#define ARN5_SPI4P2_RXIF_rxphaseadj5             0x08209
#define ARN5_SPI4P2_RXIF_rxphaseadj5_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  9 ;
    cs_uint16 rdclk                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 rctl                 :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rctl                 :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 rdclk                :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd2               :  9 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXIF_rxphaseadj5_t;
#define     ARN5_rdclk_PHASE0                             0x00
#define     ARN5_rdclk_PHASE1                             0x01
#define     ARN5_rdclk_PHASE2                             0x03
#define     ARN5_rdclk_PHASE3                             0x02
#define     ARN5_rdclk_PHASE4                             0x06
#define     ARN5_rdclk_PHASE5                             0x07
#define     ARN5_rdclk_PHASE6                             0x05
#define     ARN5_rdclk_PHASE7                             0x04
#define     ARN5_rctl_PHASE0                              0x00
#define     ARN5_rctl_PHASE1                              0x01
#define     ARN5_rctl_PHASE2                              0x03
#define     ARN5_rctl_PHASE3                              0x02
#define     ARN5_rctl_PHASE4                              0x06
#define     ARN5_rctl_PHASE5                              0x07
#define     ARN5_rctl_PHASE6                              0x05
#define     ARN5_rctl_PHASE7                              0x04


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * SPI4P2::mif
 */

#define ARN5_SPI4P2_MIF_rxpllcontrol1            0x08300
#define ARN5_SPI4P2_MIF_rxpllcontrol1_dft        0x4010
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 freqtune             :  4 ; /* bits 15:12 */
    cs_uint16 tcko_en              :  1 ; /* bits 11:11 */
    cs_uint16 tcko_sel             :  3 ; /* bits 10:8 */
    cs_uint16 tcki_en              :  1 ; /* bits 7:7 */
    cs_uint16 chpi50ua_en          :  1 ; /* bits 6:6 */
    cs_uint16 pwrdwn               :  1 ; /* bits 5:5 */
    cs_uint16 int_filter_en        :  1 ; /* bits 4:4 */
    cs_uint16 multsel              :  2 ; /* bits 3:2 */
    cs_uint16 spare_reg            :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 spare_reg            :  2 ; /* bits 1:0 */
    cs_uint16 multsel              :  2 ; /* bits 3:2 */
    cs_uint16 int_filter_en        :  1 ; /* bits 4:4 */
    cs_uint16 pwrdwn               :  1 ; /* bits 5:5 */
    cs_uint16 chpi50ua_en          :  1 ; /* bits 6:6 */
    cs_uint16 tcki_en              :  1 ; /* bits 7:7 */
    cs_uint16 tcko_sel             :  3 ; /* bits 10:8 */
    cs_uint16 tcko_en              :  1 ; /* bits 11:11 */
    cs_uint16 freqtune             :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_MIF_rxpllcontrol1_t;
#define     ARN5_tcko_sel_DIV1                            0x00
#define     ARN5_tcko_sel_DIV2                            0x04
#define     ARN5_tcko_sel_DIV4                            0x05
#define     ARN5_tcko_sel_DIV8                            0x06
#define     ARN5_tcko_sel_DIV16                           0x07
#define     ARN5_chpi50ua_en_low                          0x00
#define     ARN5_chpi50ua_en_high                         0x01
#define     ARN5_multsel_SPI3                             0x03
#define     ARN5_multsel_DIV8                             0x02
#define     ARN5_multsel_DIV4                             0x01
#define     ARN5_multsel_DIV2                             0x00

#define ARN5_SPI4P2_MIF_rxpllcontrol2            0x08301
#define ARN5_SPI4P2_MIF_rxpllcontrol2_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  8 ;
    cs_uint16 ckout_div_en         :  4 ; /* bits 7:4 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 filtpn_en            :  1 ; /* bits 1:1 */
    cs_uint16 bypass_loop_res      :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bypass_loop_res      :  1 ; /* bits 0:0 */
    cs_uint16 filtpn_en            :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 ckout_div_en         :  4 ; /* bits 7:4 */
    cs_uint16 rsrvd2               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_MIF_rxpllcontrol2_t;
#define     ARN5_ckout_div_en_DIV16                       0x08
#define     ARN5_ckout_div_en_DIV8                        0x04
#define     ARN5_ckout_div_en_DIV4                        0x02
#define     ARN5_ckout_div_en_DIV2                        0x01
#define     ARN5_ckout_div_en_DISABLE                     0x00

#define ARN5_SPI4P2_MIF_txpllcontrol1            0x08302
#define ARN5_SPI4P2_MIF_txpllcontrol1_dft        0x4010
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 freqtune             :  4 ; /* bits 15:12 */
    cs_uint16 tcko_en              :  1 ; /* bits 11:11 */
    cs_uint16 tcko_sel             :  3 ; /* bits 10:8 */
    cs_uint16 tcki_en              :  1 ; /* bits 7:7 */
    cs_uint16 chpi50ua_en          :  1 ; /* bits 6:6 */
    cs_uint16 pwrdwn               :  1 ; /* bits 5:5 */
    cs_uint16 int_filter_en        :  1 ; /* bits 4:4 */
    cs_uint16 multsel              :  2 ; /* bits 3:2 */
    cs_uint16 spare_reg            :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 spare_reg            :  2 ; /* bits 1:0 */
    cs_uint16 multsel              :  2 ; /* bits 3:2 */
    cs_uint16 int_filter_en        :  1 ; /* bits 4:4 */
    cs_uint16 pwrdwn               :  1 ; /* bits 5:5 */
    cs_uint16 chpi50ua_en          :  1 ; /* bits 6:6 */
    cs_uint16 tcki_en              :  1 ; /* bits 7:7 */
    cs_uint16 tcko_sel             :  3 ; /* bits 10:8 */
    cs_uint16 tcko_en              :  1 ; /* bits 11:11 */
    cs_uint16 freqtune             :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_MIF_txpllcontrol1_t;

#define ARN5_SPI4P2_MIF_txpllcontrol2            0x08303
#define ARN5_SPI4P2_MIF_txpllcontrol2_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  8 ;
    cs_uint16 ckout_div_en         :  4 ; /* bits 7:4 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 filtpn_en            :  1 ; /* bits 1:1 */
    cs_uint16 bypass_loop_res      :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bypass_loop_res      :  1 ; /* bits 0:0 */
    cs_uint16 filtpn_en            :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 ckout_div_en         :  4 ; /* bits 7:4 */
    cs_uint16 rsrvd2               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_MIF_txpllcontrol2_t;

#define ARN5_SPI4P2_MIF_loopbacks                0x08304
#define ARN5_SPI4P2_MIF_loopbacks_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 lb                   :  1 ; /* bits 2:2 */
    cs_uint16 bertlb               :  1 ; /* bits 1:1 */
    cs_uint16 bistmode             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bistmode             :  1 ; /* bits 0:0 */
    cs_uint16 bertlb               :  1 ; /* bits 1:1 */
    cs_uint16 lb                   :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_MIF_loopbacks_t;

#define ARN5_SPI4P2_MIF_mode                     0x08305
#define ARN5_SPI4P2_MIF_mode_dft                 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  7 ;
    cs_uint16 quarterrate_override :  1 ; /* bits 8:8 */
    cs_uint16 rsclk_invert         :  1 ; /* bits 7:7 */
    cs_uint16 tsclk_invert         :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 quarterrate_value    :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 quarterrate_value    :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 tsclk_invert         :  1 ; /* bits 6:6 */
    cs_uint16 rsclk_invert         :  1 ; /* bits 7:7 */
    cs_uint16 quarterrate_override :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_MIF_mode_t;

#define ARN5_SPI4P2_MIF_status                   0x08306
#define ARN5_SPI4P2_MIF_status_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 spi3mode             :  1 ; /* bits 2:2 */
    cs_uint16 lvdsstat             :  1 ; /* bits 1:1 */
    cs_uint16 quarterrate          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 quarterrate          :  1 ; /* bits 0:0 */
    cs_uint16 lvdsstat             :  1 ; /* bits 1:1 */
    cs_uint16 spi3mode             :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_MIF_status_t;

#define ARN5_SPI4P2_MIF_fmro_sel                 0x08307
#define ARN5_SPI4P2_MIF_fmro_sel_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 sel                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 sel                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_MIF_fmro_sel_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * SPI4P2::txcal
 */

#define ARN5_SPI4P2_TXCAL_ent_0                  0x08400
#define ARN5_SPI4P2_TXCAL_ent_0_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_0_t;

#define ARN5_SPI4P2_TXCAL_ent_1                  0x08401
#define ARN5_SPI4P2_TXCAL_ent_1_dft              0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_1_t;

#define ARN5_SPI4P2_TXCAL_ent_2                  0x08402
#define ARN5_SPI4P2_TXCAL_ent_2_dft              0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_2_t;

#define ARN5_SPI4P2_TXCAL_ent_3                  0x08403
#define ARN5_SPI4P2_TXCAL_ent_3_dft              0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_3_t;

#define ARN5_SPI4P2_TXCAL_ent_4                  0x08404
#define ARN5_SPI4P2_TXCAL_ent_4_dft              0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_4_t;

#define ARN5_SPI4P2_TXCAL_ent_5                  0x08405
#define ARN5_SPI4P2_TXCAL_ent_5_dft              0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_5_t;

#define ARN5_SPI4P2_TXCAL_ent_6                  0x08406
#define ARN5_SPI4P2_TXCAL_ent_6_dft              0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_6_t;

#define ARN5_SPI4P2_TXCAL_ent_7                  0x08407
#define ARN5_SPI4P2_TXCAL_ent_7_dft              0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_7_t;

#define ARN5_SPI4P2_TXCAL_ent_8                  0x08408
#define ARN5_SPI4P2_TXCAL_ent_8_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_8_t;

#define ARN5_SPI4P2_TXCAL_ent_9                  0x08409
#define ARN5_SPI4P2_TXCAL_ent_9_dft              0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_9_t;

#define ARN5_SPI4P2_TXCAL_ent_10                 0x0840a
#define ARN5_SPI4P2_TXCAL_ent_10_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_10_t;

#define ARN5_SPI4P2_TXCAL_ent_11                 0x0840b
#define ARN5_SPI4P2_TXCAL_ent_11_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_11_t;

#define ARN5_SPI4P2_TXCAL_ent_12                 0x0840c
#define ARN5_SPI4P2_TXCAL_ent_12_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_12_t;

#define ARN5_SPI4P2_TXCAL_ent_13                 0x0840d
#define ARN5_SPI4P2_TXCAL_ent_13_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_13_t;

#define ARN5_SPI4P2_TXCAL_ent_14                 0x0840e
#define ARN5_SPI4P2_TXCAL_ent_14_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_14_t;

#define ARN5_SPI4P2_TXCAL_ent_15                 0x0840f
#define ARN5_SPI4P2_TXCAL_ent_15_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_15_t;

#define ARN5_SPI4P2_TXCAL_ent_16                 0x08410
#define ARN5_SPI4P2_TXCAL_ent_16_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_16_t;

#define ARN5_SPI4P2_TXCAL_ent_17                 0x08411
#define ARN5_SPI4P2_TXCAL_ent_17_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_17_t;

#define ARN5_SPI4P2_TXCAL_ent_18                 0x08412
#define ARN5_SPI4P2_TXCAL_ent_18_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_18_t;

#define ARN5_SPI4P2_TXCAL_ent_19                 0x08413
#define ARN5_SPI4P2_TXCAL_ent_19_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_19_t;

#define ARN5_SPI4P2_TXCAL_ent_20                 0x08414
#define ARN5_SPI4P2_TXCAL_ent_20_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_20_t;

#define ARN5_SPI4P2_TXCAL_ent_21                 0x08415
#define ARN5_SPI4P2_TXCAL_ent_21_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_21_t;

#define ARN5_SPI4P2_TXCAL_ent_22                 0x08416
#define ARN5_SPI4P2_TXCAL_ent_22_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_22_t;

#define ARN5_SPI4P2_TXCAL_ent_23                 0x08417
#define ARN5_SPI4P2_TXCAL_ent_23_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_23_t;

#define ARN5_SPI4P2_TXCAL_ent_24                 0x08418
#define ARN5_SPI4P2_TXCAL_ent_24_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_24_t;

#define ARN5_SPI4P2_TXCAL_ent_25                 0x08419
#define ARN5_SPI4P2_TXCAL_ent_25_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_25_t;

#define ARN5_SPI4P2_TXCAL_ent_26                 0x0841a
#define ARN5_SPI4P2_TXCAL_ent_26_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_26_t;

#define ARN5_SPI4P2_TXCAL_ent_27                 0x0841b
#define ARN5_SPI4P2_TXCAL_ent_27_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_27_t;

#define ARN5_SPI4P2_TXCAL_ent_28                 0x0841c
#define ARN5_SPI4P2_TXCAL_ent_28_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_28_t;

#define ARN5_SPI4P2_TXCAL_ent_29                 0x0841d
#define ARN5_SPI4P2_TXCAL_ent_29_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_29_t;

#define ARN5_SPI4P2_TXCAL_ent_30                 0x0841e
#define ARN5_SPI4P2_TXCAL_ent_30_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_30_t;

#define ARN5_SPI4P2_TXCAL_ent_31                 0x0841f
#define ARN5_SPI4P2_TXCAL_ent_31_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_31_t;

#define ARN5_SPI4P2_TXCAL_ent_32                 0x08420
#define ARN5_SPI4P2_TXCAL_ent_32_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_32_t;

#define ARN5_SPI4P2_TXCAL_ent_33                 0x08421
#define ARN5_SPI4P2_TXCAL_ent_33_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_33_t;

#define ARN5_SPI4P2_TXCAL_ent_34                 0x08422
#define ARN5_SPI4P2_TXCAL_ent_34_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_34_t;

#define ARN5_SPI4P2_TXCAL_ent_35                 0x08423
#define ARN5_SPI4P2_TXCAL_ent_35_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_35_t;

#define ARN5_SPI4P2_TXCAL_ent_36                 0x08424
#define ARN5_SPI4P2_TXCAL_ent_36_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_36_t;

#define ARN5_SPI4P2_TXCAL_ent_37                 0x08425
#define ARN5_SPI4P2_TXCAL_ent_37_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_37_t;

#define ARN5_SPI4P2_TXCAL_ent_38                 0x08426
#define ARN5_SPI4P2_TXCAL_ent_38_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_38_t;

#define ARN5_SPI4P2_TXCAL_ent_39                 0x08427
#define ARN5_SPI4P2_TXCAL_ent_39_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_39_t;

#define ARN5_SPI4P2_TXCAL_ent_40                 0x08428
#define ARN5_SPI4P2_TXCAL_ent_40_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_40_t;

#define ARN5_SPI4P2_TXCAL_ent_41                 0x08429
#define ARN5_SPI4P2_TXCAL_ent_41_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_41_t;

#define ARN5_SPI4P2_TXCAL_ent_42                 0x0842a
#define ARN5_SPI4P2_TXCAL_ent_42_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_42_t;

#define ARN5_SPI4P2_TXCAL_ent_43                 0x0842b
#define ARN5_SPI4P2_TXCAL_ent_43_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_43_t;

#define ARN5_SPI4P2_TXCAL_ent_44                 0x0842c
#define ARN5_SPI4P2_TXCAL_ent_44_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_44_t;

#define ARN5_SPI4P2_TXCAL_ent_45                 0x0842d
#define ARN5_SPI4P2_TXCAL_ent_45_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_45_t;

#define ARN5_SPI4P2_TXCAL_ent_46                 0x0842e
#define ARN5_SPI4P2_TXCAL_ent_46_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_46_t;

#define ARN5_SPI4P2_TXCAL_ent_47                 0x0842f
#define ARN5_SPI4P2_TXCAL_ent_47_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_47_t;

#define ARN5_SPI4P2_TXCAL_ent_48                 0x08430
#define ARN5_SPI4P2_TXCAL_ent_48_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_48_t;

#define ARN5_SPI4P2_TXCAL_ent_49                 0x08431
#define ARN5_SPI4P2_TXCAL_ent_49_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_49_t;

#define ARN5_SPI4P2_TXCAL_ent_50                 0x08432
#define ARN5_SPI4P2_TXCAL_ent_50_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_50_t;

#define ARN5_SPI4P2_TXCAL_ent_51                 0x08433
#define ARN5_SPI4P2_TXCAL_ent_51_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_51_t;

#define ARN5_SPI4P2_TXCAL_ent_52                 0x08434
#define ARN5_SPI4P2_TXCAL_ent_52_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_52_t;

#define ARN5_SPI4P2_TXCAL_ent_53                 0x08435
#define ARN5_SPI4P2_TXCAL_ent_53_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_53_t;

#define ARN5_SPI4P2_TXCAL_ent_54                 0x08436
#define ARN5_SPI4P2_TXCAL_ent_54_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_54_t;

#define ARN5_SPI4P2_TXCAL_ent_55                 0x08437
#define ARN5_SPI4P2_TXCAL_ent_55_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_55_t;

#define ARN5_SPI4P2_TXCAL_ent_56                 0x08438
#define ARN5_SPI4P2_TXCAL_ent_56_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_56_t;

#define ARN5_SPI4P2_TXCAL_ent_57                 0x08439
#define ARN5_SPI4P2_TXCAL_ent_57_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_57_t;

#define ARN5_SPI4P2_TXCAL_ent_58                 0x0843a
#define ARN5_SPI4P2_TXCAL_ent_58_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_58_t;

#define ARN5_SPI4P2_TXCAL_ent_59                 0x0843b
#define ARN5_SPI4P2_TXCAL_ent_59_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_59_t;

#define ARN5_SPI4P2_TXCAL_ent_60                 0x0843c
#define ARN5_SPI4P2_TXCAL_ent_60_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_60_t;

#define ARN5_SPI4P2_TXCAL_ent_61                 0x0843d
#define ARN5_SPI4P2_TXCAL_ent_61_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_61_t;

#define ARN5_SPI4P2_TXCAL_ent_62                 0x0843e
#define ARN5_SPI4P2_TXCAL_ent_62_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_62_t;

#define ARN5_SPI4P2_TXCAL_ent_63                 0x0843f
#define ARN5_SPI4P2_TXCAL_ent_63_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_63_t;

#define ARN5_SPI4P2_TXCAL_ent_64                 0x08440
#define ARN5_SPI4P2_TXCAL_ent_64_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_64_t;

#define ARN5_SPI4P2_TXCAL_ent_65                 0x08441
#define ARN5_SPI4P2_TXCAL_ent_65_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_65_t;

#define ARN5_SPI4P2_TXCAL_ent_66                 0x08442
#define ARN5_SPI4P2_TXCAL_ent_66_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_66_t;

#define ARN5_SPI4P2_TXCAL_ent_67                 0x08443
#define ARN5_SPI4P2_TXCAL_ent_67_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_67_t;

#define ARN5_SPI4P2_TXCAL_ent_68                 0x08444
#define ARN5_SPI4P2_TXCAL_ent_68_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_68_t;

#define ARN5_SPI4P2_TXCAL_ent_69                 0x08445
#define ARN5_SPI4P2_TXCAL_ent_69_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_69_t;

#define ARN5_SPI4P2_TXCAL_ent_70                 0x08446
#define ARN5_SPI4P2_TXCAL_ent_70_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_70_t;

#define ARN5_SPI4P2_TXCAL_ent_71                 0x08447
#define ARN5_SPI4P2_TXCAL_ent_71_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_71_t;

#define ARN5_SPI4P2_TXCAL_ent_72                 0x08448
#define ARN5_SPI4P2_TXCAL_ent_72_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_72_t;

#define ARN5_SPI4P2_TXCAL_ent_73                 0x08449
#define ARN5_SPI4P2_TXCAL_ent_73_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_73_t;

#define ARN5_SPI4P2_TXCAL_ent_74                 0x0844a
#define ARN5_SPI4P2_TXCAL_ent_74_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_74_t;

#define ARN5_SPI4P2_TXCAL_ent_75                 0x0844b
#define ARN5_SPI4P2_TXCAL_ent_75_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_75_t;

#define ARN5_SPI4P2_TXCAL_ent_76                 0x0844c
#define ARN5_SPI4P2_TXCAL_ent_76_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_76_t;

#define ARN5_SPI4P2_TXCAL_ent_77                 0x0844d
#define ARN5_SPI4P2_TXCAL_ent_77_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_77_t;

#define ARN5_SPI4P2_TXCAL_ent_78                 0x0844e
#define ARN5_SPI4P2_TXCAL_ent_78_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_78_t;

#define ARN5_SPI4P2_TXCAL_ent_79                 0x0844f
#define ARN5_SPI4P2_TXCAL_ent_79_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_79_t;

#define ARN5_SPI4P2_TXCAL_ent_80                 0x08450
#define ARN5_SPI4P2_TXCAL_ent_80_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_80_t;

#define ARN5_SPI4P2_TXCAL_ent_81                 0x08451
#define ARN5_SPI4P2_TXCAL_ent_81_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_81_t;

#define ARN5_SPI4P2_TXCAL_ent_82                 0x08452
#define ARN5_SPI4P2_TXCAL_ent_82_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_82_t;

#define ARN5_SPI4P2_TXCAL_ent_83                 0x08453
#define ARN5_SPI4P2_TXCAL_ent_83_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_83_t;

#define ARN5_SPI4P2_TXCAL_ent_84                 0x08454
#define ARN5_SPI4P2_TXCAL_ent_84_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_84_t;

#define ARN5_SPI4P2_TXCAL_ent_85                 0x08455
#define ARN5_SPI4P2_TXCAL_ent_85_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_85_t;

#define ARN5_SPI4P2_TXCAL_ent_86                 0x08456
#define ARN5_SPI4P2_TXCAL_ent_86_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_86_t;

#define ARN5_SPI4P2_TXCAL_ent_87                 0x08457
#define ARN5_SPI4P2_TXCAL_ent_87_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_87_t;

#define ARN5_SPI4P2_TXCAL_ent_88                 0x08458
#define ARN5_SPI4P2_TXCAL_ent_88_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_88_t;

#define ARN5_SPI4P2_TXCAL_ent_89                 0x08459
#define ARN5_SPI4P2_TXCAL_ent_89_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_89_t;

#define ARN5_SPI4P2_TXCAL_ent_90                 0x0845a
#define ARN5_SPI4P2_TXCAL_ent_90_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_90_t;

#define ARN5_SPI4P2_TXCAL_ent_91                 0x0845b
#define ARN5_SPI4P2_TXCAL_ent_91_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_91_t;

#define ARN5_SPI4P2_TXCAL_ent_92                 0x0845c
#define ARN5_SPI4P2_TXCAL_ent_92_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_92_t;

#define ARN5_SPI4P2_TXCAL_ent_93                 0x0845d
#define ARN5_SPI4P2_TXCAL_ent_93_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_93_t;

#define ARN5_SPI4P2_TXCAL_ent_94                 0x0845e
#define ARN5_SPI4P2_TXCAL_ent_94_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_94_t;

#define ARN5_SPI4P2_TXCAL_ent_95                 0x0845f
#define ARN5_SPI4P2_TXCAL_ent_95_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_ent_95_t;

#define ARN5_SPI4P2_TXCAL_phy_ch_num_lch0        0x08460
#define ARN5_SPI4P2_TXCAL_phy_ch_num_lch0_dft    0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 value                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_phy_ch_num_lch0_t;

#define ARN5_SPI4P2_TXCAL_phy_ch_num_lch1        0x08461
#define ARN5_SPI4P2_TXCAL_phy_ch_num_lch1_dft    0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 value                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_phy_ch_num_lch1_t;

#define ARN5_SPI4P2_TXCAL_phy_ch_num_lch2        0x08462
#define ARN5_SPI4P2_TXCAL_phy_ch_num_lch2_dft    0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 value                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_phy_ch_num_lch2_t;

#define ARN5_SPI4P2_TXCAL_phy_ch_num_lch3        0x08463
#define ARN5_SPI4P2_TXCAL_phy_ch_num_lch3_dft    0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 value                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_phy_ch_num_lch3_t;

#define ARN5_SPI4P2_TXCAL_phy_ch_num_lch4        0x08464
#define ARN5_SPI4P2_TXCAL_phy_ch_num_lch4_dft    0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 value                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_phy_ch_num_lch4_t;

#define ARN5_SPI4P2_TXCAL_phy_ch_num_lch5        0x08465
#define ARN5_SPI4P2_TXCAL_phy_ch_num_lch5_dft    0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 value                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_phy_ch_num_lch5_t;

#define ARN5_SPI4P2_TXCAL_phy_ch_num_lch6        0x08466
#define ARN5_SPI4P2_TXCAL_phy_ch_num_lch6_dft    0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 value                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_phy_ch_num_lch6_t;

#define ARN5_SPI4P2_TXCAL_phy_ch_num_lch7        0x08467
#define ARN5_SPI4P2_TXCAL_phy_ch_num_lch7_dft    0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 value                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_phy_ch_num_lch7_t;

#define ARN5_SPI4P2_TXCAL_cal_len                0x08468
#define ARN5_SPI4P2_TXCAL_cal_len_dft            0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  9 ;
    cs_uint16 value                :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  7 ; /* bits 6:0 */
    cs_uint16 rsrvd1               :  9 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_cal_len_t;

#define ARN5_SPI4P2_TXCAL_cal_m                  0x08469
#define ARN5_SPI4P2_TXCAL_cal_m_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCAL_cal_m_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * SPI4P2::rxcal
 */

#define ARN5_SPI4P2_RXCAL_ent_0                  0x08500
#define ARN5_SPI4P2_RXCAL_ent_0_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_0_t;

#define ARN5_SPI4P2_RXCAL_ent_1                  0x08501
#define ARN5_SPI4P2_RXCAL_ent_1_dft              0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_1_t;

#define ARN5_SPI4P2_RXCAL_ent_2                  0x08502
#define ARN5_SPI4P2_RXCAL_ent_2_dft              0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_2_t;

#define ARN5_SPI4P2_RXCAL_ent_3                  0x08503
#define ARN5_SPI4P2_RXCAL_ent_3_dft              0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_3_t;

#define ARN5_SPI4P2_RXCAL_ent_4                  0x08504
#define ARN5_SPI4P2_RXCAL_ent_4_dft              0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_4_t;

#define ARN5_SPI4P2_RXCAL_ent_5                  0x08505
#define ARN5_SPI4P2_RXCAL_ent_5_dft              0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_5_t;

#define ARN5_SPI4P2_RXCAL_ent_6                  0x08506
#define ARN5_SPI4P2_RXCAL_ent_6_dft              0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_6_t;

#define ARN5_SPI4P2_RXCAL_ent_7                  0x08507
#define ARN5_SPI4P2_RXCAL_ent_7_dft              0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_7_t;

#define ARN5_SPI4P2_RXCAL_ent_8                  0x08508
#define ARN5_SPI4P2_RXCAL_ent_8_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_8_t;

#define ARN5_SPI4P2_RXCAL_ent_9                  0x08509
#define ARN5_SPI4P2_RXCAL_ent_9_dft              0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_9_t;

#define ARN5_SPI4P2_RXCAL_ent_10                 0x0850a
#define ARN5_SPI4P2_RXCAL_ent_10_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_10_t;

#define ARN5_SPI4P2_RXCAL_ent_11                 0x0850b
#define ARN5_SPI4P2_RXCAL_ent_11_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_11_t;

#define ARN5_SPI4P2_RXCAL_ent_12                 0x0850c
#define ARN5_SPI4P2_RXCAL_ent_12_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_12_t;

#define ARN5_SPI4P2_RXCAL_ent_13                 0x0850d
#define ARN5_SPI4P2_RXCAL_ent_13_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_13_t;

#define ARN5_SPI4P2_RXCAL_ent_14                 0x0850e
#define ARN5_SPI4P2_RXCAL_ent_14_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_14_t;

#define ARN5_SPI4P2_RXCAL_ent_15                 0x0850f
#define ARN5_SPI4P2_RXCAL_ent_15_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_15_t;

#define ARN5_SPI4P2_RXCAL_ent_16                 0x08510
#define ARN5_SPI4P2_RXCAL_ent_16_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_16_t;

#define ARN5_SPI4P2_RXCAL_ent_17                 0x08511
#define ARN5_SPI4P2_RXCAL_ent_17_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_17_t;

#define ARN5_SPI4P2_RXCAL_ent_18                 0x08512
#define ARN5_SPI4P2_RXCAL_ent_18_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_18_t;

#define ARN5_SPI4P2_RXCAL_ent_19                 0x08513
#define ARN5_SPI4P2_RXCAL_ent_19_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_19_t;

#define ARN5_SPI4P2_RXCAL_ent_20                 0x08514
#define ARN5_SPI4P2_RXCAL_ent_20_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_20_t;

#define ARN5_SPI4P2_RXCAL_ent_21                 0x08515
#define ARN5_SPI4P2_RXCAL_ent_21_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_21_t;

#define ARN5_SPI4P2_RXCAL_ent_22                 0x08516
#define ARN5_SPI4P2_RXCAL_ent_22_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_22_t;

#define ARN5_SPI4P2_RXCAL_ent_23                 0x08517
#define ARN5_SPI4P2_RXCAL_ent_23_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_23_t;

#define ARN5_SPI4P2_RXCAL_ent_24                 0x08518
#define ARN5_SPI4P2_RXCAL_ent_24_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_24_t;

#define ARN5_SPI4P2_RXCAL_ent_25                 0x08519
#define ARN5_SPI4P2_RXCAL_ent_25_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_25_t;

#define ARN5_SPI4P2_RXCAL_ent_26                 0x0851a
#define ARN5_SPI4P2_RXCAL_ent_26_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_26_t;

#define ARN5_SPI4P2_RXCAL_ent_27                 0x0851b
#define ARN5_SPI4P2_RXCAL_ent_27_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_27_t;

#define ARN5_SPI4P2_RXCAL_ent_28                 0x0851c
#define ARN5_SPI4P2_RXCAL_ent_28_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_28_t;

#define ARN5_SPI4P2_RXCAL_ent_29                 0x0851d
#define ARN5_SPI4P2_RXCAL_ent_29_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_29_t;

#define ARN5_SPI4P2_RXCAL_ent_30                 0x0851e
#define ARN5_SPI4P2_RXCAL_ent_30_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_30_t;

#define ARN5_SPI4P2_RXCAL_ent_31                 0x0851f
#define ARN5_SPI4P2_RXCAL_ent_31_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_31_t;

#define ARN5_SPI4P2_RXCAL_ent_32                 0x08520
#define ARN5_SPI4P2_RXCAL_ent_32_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_32_t;

#define ARN5_SPI4P2_RXCAL_ent_33                 0x08521
#define ARN5_SPI4P2_RXCAL_ent_33_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_33_t;

#define ARN5_SPI4P2_RXCAL_ent_34                 0x08522
#define ARN5_SPI4P2_RXCAL_ent_34_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_34_t;

#define ARN5_SPI4P2_RXCAL_ent_35                 0x08523
#define ARN5_SPI4P2_RXCAL_ent_35_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_35_t;

#define ARN5_SPI4P2_RXCAL_ent_36                 0x08524
#define ARN5_SPI4P2_RXCAL_ent_36_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_36_t;

#define ARN5_SPI4P2_RXCAL_ent_37                 0x08525
#define ARN5_SPI4P2_RXCAL_ent_37_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_37_t;

#define ARN5_SPI4P2_RXCAL_ent_38                 0x08526
#define ARN5_SPI4P2_RXCAL_ent_38_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_38_t;

#define ARN5_SPI4P2_RXCAL_ent_39                 0x08527
#define ARN5_SPI4P2_RXCAL_ent_39_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_39_t;

#define ARN5_SPI4P2_RXCAL_ent_40                 0x08528
#define ARN5_SPI4P2_RXCAL_ent_40_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_40_t;

#define ARN5_SPI4P2_RXCAL_ent_41                 0x08529
#define ARN5_SPI4P2_RXCAL_ent_41_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_41_t;

#define ARN5_SPI4P2_RXCAL_ent_42                 0x0852a
#define ARN5_SPI4P2_RXCAL_ent_42_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_42_t;

#define ARN5_SPI4P2_RXCAL_ent_43                 0x0852b
#define ARN5_SPI4P2_RXCAL_ent_43_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_43_t;

#define ARN5_SPI4P2_RXCAL_ent_44                 0x0852c
#define ARN5_SPI4P2_RXCAL_ent_44_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_44_t;

#define ARN5_SPI4P2_RXCAL_ent_45                 0x0852d
#define ARN5_SPI4P2_RXCAL_ent_45_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_45_t;

#define ARN5_SPI4P2_RXCAL_ent_46                 0x0852e
#define ARN5_SPI4P2_RXCAL_ent_46_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_46_t;

#define ARN5_SPI4P2_RXCAL_ent_47                 0x0852f
#define ARN5_SPI4P2_RXCAL_ent_47_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_47_t;

#define ARN5_SPI4P2_RXCAL_ent_48                 0x08530
#define ARN5_SPI4P2_RXCAL_ent_48_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_48_t;

#define ARN5_SPI4P2_RXCAL_ent_49                 0x08531
#define ARN5_SPI4P2_RXCAL_ent_49_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_49_t;

#define ARN5_SPI4P2_RXCAL_ent_50                 0x08532
#define ARN5_SPI4P2_RXCAL_ent_50_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_50_t;

#define ARN5_SPI4P2_RXCAL_ent_51                 0x08533
#define ARN5_SPI4P2_RXCAL_ent_51_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_51_t;

#define ARN5_SPI4P2_RXCAL_ent_52                 0x08534
#define ARN5_SPI4P2_RXCAL_ent_52_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_52_t;

#define ARN5_SPI4P2_RXCAL_ent_53                 0x08535
#define ARN5_SPI4P2_RXCAL_ent_53_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_53_t;

#define ARN5_SPI4P2_RXCAL_ent_54                 0x08536
#define ARN5_SPI4P2_RXCAL_ent_54_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_54_t;

#define ARN5_SPI4P2_RXCAL_ent_55                 0x08537
#define ARN5_SPI4P2_RXCAL_ent_55_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_55_t;

#define ARN5_SPI4P2_RXCAL_ent_56                 0x08538
#define ARN5_SPI4P2_RXCAL_ent_56_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_56_t;

#define ARN5_SPI4P2_RXCAL_ent_57                 0x08539
#define ARN5_SPI4P2_RXCAL_ent_57_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_57_t;

#define ARN5_SPI4P2_RXCAL_ent_58                 0x0853a
#define ARN5_SPI4P2_RXCAL_ent_58_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_58_t;

#define ARN5_SPI4P2_RXCAL_ent_59                 0x0853b
#define ARN5_SPI4P2_RXCAL_ent_59_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_59_t;

#define ARN5_SPI4P2_RXCAL_ent_60                 0x0853c
#define ARN5_SPI4P2_RXCAL_ent_60_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_60_t;

#define ARN5_SPI4P2_RXCAL_ent_61                 0x0853d
#define ARN5_SPI4P2_RXCAL_ent_61_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_61_t;

#define ARN5_SPI4P2_RXCAL_ent_62                 0x0853e
#define ARN5_SPI4P2_RXCAL_ent_62_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_62_t;

#define ARN5_SPI4P2_RXCAL_ent_63                 0x0853f
#define ARN5_SPI4P2_RXCAL_ent_63_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_63_t;

#define ARN5_SPI4P2_RXCAL_ent_64                 0x08540
#define ARN5_SPI4P2_RXCAL_ent_64_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_64_t;

#define ARN5_SPI4P2_RXCAL_ent_65                 0x08541
#define ARN5_SPI4P2_RXCAL_ent_65_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_65_t;

#define ARN5_SPI4P2_RXCAL_ent_66                 0x08542
#define ARN5_SPI4P2_RXCAL_ent_66_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_66_t;

#define ARN5_SPI4P2_RXCAL_ent_67                 0x08543
#define ARN5_SPI4P2_RXCAL_ent_67_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_67_t;

#define ARN5_SPI4P2_RXCAL_ent_68                 0x08544
#define ARN5_SPI4P2_RXCAL_ent_68_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_68_t;

#define ARN5_SPI4P2_RXCAL_ent_69                 0x08545
#define ARN5_SPI4P2_RXCAL_ent_69_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_69_t;

#define ARN5_SPI4P2_RXCAL_ent_70                 0x08546
#define ARN5_SPI4P2_RXCAL_ent_70_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_70_t;

#define ARN5_SPI4P2_RXCAL_ent_71                 0x08547
#define ARN5_SPI4P2_RXCAL_ent_71_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_71_t;

#define ARN5_SPI4P2_RXCAL_ent_72                 0x08548
#define ARN5_SPI4P2_RXCAL_ent_72_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_72_t;

#define ARN5_SPI4P2_RXCAL_ent_73                 0x08549
#define ARN5_SPI4P2_RXCAL_ent_73_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_73_t;

#define ARN5_SPI4P2_RXCAL_ent_74                 0x0854a
#define ARN5_SPI4P2_RXCAL_ent_74_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_74_t;

#define ARN5_SPI4P2_RXCAL_ent_75                 0x0854b
#define ARN5_SPI4P2_RXCAL_ent_75_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_75_t;

#define ARN5_SPI4P2_RXCAL_ent_76                 0x0854c
#define ARN5_SPI4P2_RXCAL_ent_76_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_76_t;

#define ARN5_SPI4P2_RXCAL_ent_77                 0x0854d
#define ARN5_SPI4P2_RXCAL_ent_77_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_77_t;

#define ARN5_SPI4P2_RXCAL_ent_78                 0x0854e
#define ARN5_SPI4P2_RXCAL_ent_78_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_78_t;

#define ARN5_SPI4P2_RXCAL_ent_79                 0x0854f
#define ARN5_SPI4P2_RXCAL_ent_79_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_79_t;

#define ARN5_SPI4P2_RXCAL_ent_80                 0x08550
#define ARN5_SPI4P2_RXCAL_ent_80_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_80_t;

#define ARN5_SPI4P2_RXCAL_ent_81                 0x08551
#define ARN5_SPI4P2_RXCAL_ent_81_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_81_t;

#define ARN5_SPI4P2_RXCAL_ent_82                 0x08552
#define ARN5_SPI4P2_RXCAL_ent_82_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_82_t;

#define ARN5_SPI4P2_RXCAL_ent_83                 0x08553
#define ARN5_SPI4P2_RXCAL_ent_83_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_83_t;

#define ARN5_SPI4P2_RXCAL_ent_84                 0x08554
#define ARN5_SPI4P2_RXCAL_ent_84_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_84_t;

#define ARN5_SPI4P2_RXCAL_ent_85                 0x08555
#define ARN5_SPI4P2_RXCAL_ent_85_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_85_t;

#define ARN5_SPI4P2_RXCAL_ent_86                 0x08556
#define ARN5_SPI4P2_RXCAL_ent_86_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_86_t;

#define ARN5_SPI4P2_RXCAL_ent_87                 0x08557
#define ARN5_SPI4P2_RXCAL_ent_87_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_87_t;

#define ARN5_SPI4P2_RXCAL_ent_88                 0x08558
#define ARN5_SPI4P2_RXCAL_ent_88_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_88_t;

#define ARN5_SPI4P2_RXCAL_ent_89                 0x08559
#define ARN5_SPI4P2_RXCAL_ent_89_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_89_t;

#define ARN5_SPI4P2_RXCAL_ent_90                 0x0855a
#define ARN5_SPI4P2_RXCAL_ent_90_dft             0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_90_t;

#define ARN5_SPI4P2_RXCAL_ent_91                 0x0855b
#define ARN5_SPI4P2_RXCAL_ent_91_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_91_t;

#define ARN5_SPI4P2_RXCAL_ent_92                 0x0855c
#define ARN5_SPI4P2_RXCAL_ent_92_dft             0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_92_t;

#define ARN5_SPI4P2_RXCAL_ent_93                 0x0855d
#define ARN5_SPI4P2_RXCAL_ent_93_dft             0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_93_t;

#define ARN5_SPI4P2_RXCAL_ent_94                 0x0855e
#define ARN5_SPI4P2_RXCAL_ent_94_dft             0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_94_t;

#define ARN5_SPI4P2_RXCAL_ent_95                 0x0855f
#define ARN5_SPI4P2_RXCAL_ent_95_dft             0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent                  :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_ent_95_t;

#define ARN5_SPI4P2_RXCAL_lch_num_pch0           0x08560
#define ARN5_SPI4P2_RXCAL_lch_num_pch0_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 value                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_lch_num_pch0_t;

#define ARN5_SPI4P2_RXCAL_lch_num_pch1           0x08561
#define ARN5_SPI4P2_RXCAL_lch_num_pch1_dft       0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 value                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_lch_num_pch1_t;

#define ARN5_SPI4P2_RXCAL_lch_num_pch2           0x08562
#define ARN5_SPI4P2_RXCAL_lch_num_pch2_dft       0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 value                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_lch_num_pch2_t;

#define ARN5_SPI4P2_RXCAL_lch_num_pch3           0x08563
#define ARN5_SPI4P2_RXCAL_lch_num_pch3_dft       0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 value                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_lch_num_pch3_t;

#define ARN5_SPI4P2_RXCAL_lch_num_pch4           0x08564
#define ARN5_SPI4P2_RXCAL_lch_num_pch4_dft       0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 value                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_lch_num_pch4_t;

#define ARN5_SPI4P2_RXCAL_lch_num_pch5           0x08565
#define ARN5_SPI4P2_RXCAL_lch_num_pch5_dft       0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 value                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_lch_num_pch5_t;

#define ARN5_SPI4P2_RXCAL_lch_num_pch6           0x08566
#define ARN5_SPI4P2_RXCAL_lch_num_pch6_dft       0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 value                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_lch_num_pch6_t;

#define ARN5_SPI4P2_RXCAL_lch_num_pch7           0x08567
#define ARN5_SPI4P2_RXCAL_lch_num_pch7_dft       0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 value                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_lch_num_pch7_t;

#define ARN5_SPI4P2_RXCAL_cal_len                0x08568
#define ARN5_SPI4P2_RXCAL_cal_len_dft            0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  9 ;
    cs_uint16 value                :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  7 ; /* bits 6:0 */
    cs_uint16 rsrvd1               :  9 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_cal_len_t;

#define ARN5_SPI4P2_RXCAL_sched_cal_len          0x08569
#define ARN5_SPI4P2_RXCAL_sched_cal_len_dft      0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  9 ;
    cs_uint16 value                :  7 ; /* bits 6:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  7 ; /* bits 6:0 */
    cs_uint16 rsrvd1               :  9 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_sched_cal_len_t;

#define ARN5_SPI4P2_RXCAL_cal_m                  0x0856a
#define ARN5_SPI4P2_RXCAL_cal_m_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCAL_cal_m_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * SPI4P2::txcore
 */

#define ARN5_SPI4P2_TXCORE_ch_min_words_reqd_for_tx 0x08600
#define ARN5_SPI4P2_TXCORE_ch_min_words_reqd_for_tx_dft 0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 start                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 start                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCORE_ch_min_words_reqd_for_tx_t;

#define ARN5_SPI4P2_TXCORE_mem_fifo_low          0x08601
#define ARN5_SPI4P2_TXCORE_mem_fifo_low_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 threshold            :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCORE_mem_fifo_low_t;

#define ARN5_SPI4P2_TXCORE_mem_full              0x08602
#define ARN5_SPI4P2_TXCORE_mem_full_dft          0x0008
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 threshold            : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCORE_mem_full_t;

#define ARN5_SPI4P2_TXCORE_q3_fifo_high          0x08603
#define ARN5_SPI4P2_TXCORE_q3_fifo_high_dft      0x0014
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 threshold            :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCORE_q3_fifo_high_t;

#define ARN5_SPI4P2_TXCORE_mem_fifo              0x08604
#define ARN5_SPI4P2_TXCORE_mem_fifo_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 count                :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCORE_mem_fifo_t;

#define ARN5_SPI4P2_TXCORE_spi3_single_fifo_full 0x08605
#define ARN5_SPI4P2_TXCORE_spi3_single_fifo_full_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 stall_mode           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 stall_mode           :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCORE_spi3_single_fifo_full_t;

#define ARN5_SPI4P2_TXCORE_pm_ctrl               0x08606
#define ARN5_SPI4P2_TXCORE_pm_ctrl_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 sel                  :  4 ; /* bits 15:12 */
    cs_uint16 wrn                  :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 addr                 :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr                 :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 wrn                  :  1 ; /* bits 11:11 */
    cs_uint16 sel                  :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCORE_pm_ctrl_t;

#define ARN5_SPI4P2_TXCORE_pm_data6              0x08607
#define ARN5_SPI4P2_TXCORE_pm_data6_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value6               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value6               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCORE_pm_data6_t;

#define ARN5_SPI4P2_TXCORE_pm_data5              0x08608
#define ARN5_SPI4P2_TXCORE_pm_data5_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value5               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value5               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCORE_pm_data5_t;

#define ARN5_SPI4P2_TXCORE_pm_data4              0x08609
#define ARN5_SPI4P2_TXCORE_pm_data4_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value4               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value4               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCORE_pm_data4_t;

#define ARN5_SPI4P2_TXCORE_pm_data3              0x0860a
#define ARN5_SPI4P2_TXCORE_pm_data3_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value3               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value3               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCORE_pm_data3_t;

#define ARN5_SPI4P2_TXCORE_pm_data2              0x0860b
#define ARN5_SPI4P2_TXCORE_pm_data2_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value2               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value2               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCORE_pm_data2_t;

#define ARN5_SPI4P2_TXCORE_pm_data1              0x0860c
#define ARN5_SPI4P2_TXCORE_pm_data1_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value1               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value1               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCORE_pm_data1_t;

#define ARN5_SPI4P2_TXCORE_pm_data0              0x0860d
#define ARN5_SPI4P2_TXCORE_pm_data0_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value0               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value0               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_TXCORE_pm_data0_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * SPI4P2::rxcore
 */

#define ARN5_SPI4P2_RXCORE_spi42_hysteresis      0x08700
#define ARN5_SPI4P2_RXCORE_spi42_hysteresis_dft  0x0009
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 value                :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_spi42_hysteresis_t;

#define ARN5_SPI4P2_RXCORE_spi42_spi3_threshold  0x08701
#define ARN5_SPI4P2_RXCORE_spi42_spi3_threshold_dft 0x0432
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 spi3                 :  6 ; /* bits 13:8 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 spi42                :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 spi42                :  6 ; /* bits 5:0 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 spi3                 :  6 ; /* bits 13:8 */
    cs_uint16 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_spi42_spi3_threshold_t;

#define ARN5_SPI4P2_RXCORE_chclose               0x08702
#define ARN5_SPI4P2_RXCORE_chclose_dft           0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 pcol                 :  1 ; /* bits 1:1 */
    cs_uint16 enable               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 enable               :  1 ; /* bits 0:0 */
    cs_uint16 pcol                 :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_chclose_t;

#define ARN5_SPI4P2_RXCORE_numdip2err            0x08703
#define ARN5_SPI4P2_RXCORE_numdip2err_dft        0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 value                :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  4 ; /* bits 3:0 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_numdip2err_t;

#define ARN5_SPI4P2_RXCORE_numdip2match          0x08704
#define ARN5_SPI4P2_RXCORE_numdip2match_dft      0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 value                :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  4 ; /* bits 3:0 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_numdip2match_t;

#define ARN5_SPI4P2_RXCORE_alpha                 0x08705
#define ARN5_SPI4P2_RXCORE_alpha_dft             0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 num                  :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 num                  :  4 ; /* bits 3:0 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_alpha_t;

#define ARN5_SPI4P2_RXCORE_datamaxt              0x08706
#define ARN5_SPI4P2_RXCORE_datamaxt_dft          0x03ff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_datamaxt_t;

#define ARN5_SPI4P2_RXCORE_burst                 0x08707
#define ARN5_SPI4P2_RXCORE_burst_dft             0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 size                 :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 size                 :  4 ; /* bits 3:0 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_burst_t;

#define ARN5_SPI4P2_RXCORE_maxburst1             0x08708
#define ARN5_SPI4P2_RXCORE_maxburst1_dft         0x000c
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 value                : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                : 10 ; /* bits 9:0 */
    cs_uint16 rsrvd1               :  6 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_maxburst1_t;

#define ARN5_SPI4P2_RXCORE_maxburst2             0x08709
#define ARN5_SPI4P2_RXCORE_maxburst2_dft         0x0008
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 value                : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                : 10 ; /* bits 9:0 */
    cs_uint16 rsrvd1               :  6 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_maxburst2_t;

#define ARN5_SPI4P2_RXCORE_bbenable              0x0870a
#define ARN5_SPI4P2_RXCORE_bbenable_dft          0x003f
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 spi3_spi42_qtrrate   :  4 ; /* bits 7:4 */
    cs_uint16 spi42                :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 spi42                :  4 ; /* bits 3:0 */
    cs_uint16 spi3_spi42_qtrrate   :  4 ; /* bits 7:4 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_bbenable_t;

#define ARN5_SPI4P2_RXCORE_abs_full              0x0870b
#define ARN5_SPI4P2_RXCORE_abs_full_dft          0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 threshold            : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            : 11 ; /* bits 10:0 */
    cs_uint16 rsrvd1               :  5 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_abs_full_t;

#define ARN5_SPI4P2_RXCORE_almost_full_0         0x0870c
#define ARN5_SPI4P2_RXCORE_almost_full_0_dft     0x0060
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_almost_full_0_t;

#define ARN5_SPI4P2_RXCORE_almost_empty_0        0x0870d
#define ARN5_SPI4P2_RXCORE_almost_empty_0_dft    0x0040
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_almost_empty_0_t;

#define ARN5_SPI4P2_RXCORE_hysteresis_0          0x0870e
#define ARN5_SPI4P2_RXCORE_hysteresis_0_dft      0x0010
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_hysteresis_0_t;

#define ARN5_SPI4P2_RXCORE_almost_full_1         0x0870f
#define ARN5_SPI4P2_RXCORE_almost_full_1_dft     0x0060
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_almost_full_1_t;

#define ARN5_SPI4P2_RXCORE_almost_empty_1        0x08710
#define ARN5_SPI4P2_RXCORE_almost_empty_1_dft    0x0040
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_almost_empty_1_t;

#define ARN5_SPI4P2_RXCORE_hysteresis_1          0x08711
#define ARN5_SPI4P2_RXCORE_hysteresis_1_dft      0x0010
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_hysteresis_1_t;

#define ARN5_SPI4P2_RXCORE_almost_full_2         0x08712
#define ARN5_SPI4P2_RXCORE_almost_full_2_dft     0x0060
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_almost_full_2_t;

#define ARN5_SPI4P2_RXCORE_almost_empty_2        0x08713
#define ARN5_SPI4P2_RXCORE_almost_empty_2_dft    0x0040
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_almost_empty_2_t;

#define ARN5_SPI4P2_RXCORE_hysteresis_2          0x08714
#define ARN5_SPI4P2_RXCORE_hysteresis_2_dft      0x0010
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_hysteresis_2_t;

#define ARN5_SPI4P2_RXCORE_almost_full_3         0x08715
#define ARN5_SPI4P2_RXCORE_almost_full_3_dft     0x0060
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_almost_full_3_t;

#define ARN5_SPI4P2_RXCORE_almost_empty_3        0x08716
#define ARN5_SPI4P2_RXCORE_almost_empty_3_dft    0x0040
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_almost_empty_3_t;

#define ARN5_SPI4P2_RXCORE_hysteresis_3          0x08717
#define ARN5_SPI4P2_RXCORE_hysteresis_3_dft      0x0010
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_hysteresis_3_t;

#define ARN5_SPI4P2_RXCORE_almost_full_4         0x08718
#define ARN5_SPI4P2_RXCORE_almost_full_4_dft     0x0060
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_almost_full_4_t;

#define ARN5_SPI4P2_RXCORE_almost_empty_4        0x08719
#define ARN5_SPI4P2_RXCORE_almost_empty_4_dft    0x0040
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_almost_empty_4_t;

#define ARN5_SPI4P2_RXCORE_hysteresis_4          0x0871a
#define ARN5_SPI4P2_RXCORE_hysteresis_4_dft      0x0010
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_hysteresis_4_t;

#define ARN5_SPI4P2_RXCORE_almost_full_5         0x0871b
#define ARN5_SPI4P2_RXCORE_almost_full_5_dft     0x0060
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_almost_full_5_t;

#define ARN5_SPI4P2_RXCORE_almost_empty_5        0x0871c
#define ARN5_SPI4P2_RXCORE_almost_empty_5_dft    0x0040
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_almost_empty_5_t;

#define ARN5_SPI4P2_RXCORE_hysteresis_5          0x0871d
#define ARN5_SPI4P2_RXCORE_hysteresis_5_dft      0x0010
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_hysteresis_5_t;

#define ARN5_SPI4P2_RXCORE_almost_full_6         0x0871e
#define ARN5_SPI4P2_RXCORE_almost_full_6_dft     0x0060
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_almost_full_6_t;

#define ARN5_SPI4P2_RXCORE_almost_empty_6        0x0871f
#define ARN5_SPI4P2_RXCORE_almost_empty_6_dft    0x0040
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_almost_empty_6_t;

#define ARN5_SPI4P2_RXCORE_hysteresis_6          0x08720
#define ARN5_SPI4P2_RXCORE_hysteresis_6_dft      0x0010
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_hysteresis_6_t;

#define ARN5_SPI4P2_RXCORE_almost_full_7         0x08721
#define ARN5_SPI4P2_RXCORE_almost_full_7_dft     0x0060
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_almost_full_7_t;

#define ARN5_SPI4P2_RXCORE_almost_empty_7        0x08722
#define ARN5_SPI4P2_RXCORE_almost_empty_7_dft    0x0040
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_almost_empty_7_t;

#define ARN5_SPI4P2_RXCORE_hysteresis_7          0x08723
#define ARN5_SPI4P2_RXCORE_hysteresis_7_dft      0x0010
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_hysteresis_7_t;

#define ARN5_SPI4P2_RXCORE_pm_ctrl               0x08724
#define ARN5_SPI4P2_RXCORE_pm_ctrl_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 sel                  :  4 ; /* bits 15:12 */
    cs_uint16 wrn                  :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 addr                 :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr                 :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 wrn                  :  1 ; /* bits 11:11 */
    cs_uint16 sel                  :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_pm_ctrl_t;

#define ARN5_SPI4P2_RXCORE_pm_data6              0x08725
#define ARN5_SPI4P2_RXCORE_pm_data6_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value6               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value6               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_pm_data6_t;

#define ARN5_SPI4P2_RXCORE_pm_data5              0x08726
#define ARN5_SPI4P2_RXCORE_pm_data5_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value5               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value5               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_pm_data5_t;

#define ARN5_SPI4P2_RXCORE_pm_data4              0x08727
#define ARN5_SPI4P2_RXCORE_pm_data4_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value4               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value4               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_pm_data4_t;

#define ARN5_SPI4P2_RXCORE_pm_data3              0x08728
#define ARN5_SPI4P2_RXCORE_pm_data3_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value3               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value3               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_pm_data3_t;

#define ARN5_SPI4P2_RXCORE_pm_data2              0x08729
#define ARN5_SPI4P2_RXCORE_pm_data2_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value2               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value2               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_pm_data2_t;

#define ARN5_SPI4P2_RXCORE_pm_data1              0x0872a
#define ARN5_SPI4P2_RXCORE_pm_data1_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value1               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value1               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_pm_data1_t;

#define ARN5_SPI4P2_RXCORE_pm_data0              0x0872b
#define ARN5_SPI4P2_RXCORE_pm_data0_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 value0               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value0               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2_RXCORE_pm_data0_t;


/******************************************************************
 *
 * Block declaration.
 *
 ******************************************************************/

/* SPI4P2 */

typedef struct {
  ARN5_SPI4P2_CORE_status_t                core_status;                   /* addr: 0x0000 */
  ARN5_SPI4P2_CORE_control_t               control;                       /* addr: 0x0001 */
  ARN5_SPI4P2_CORE_scratch_t               scratch;                       /* addr: 0x0002 */
  ARN5_SPI4P2_CORE_intrpt_t                core_intrpt;                   /* addr: 0x0003 */
  ARN5_SPI4P2_CORE_intrpten_t              core_intrpten;                 /* addr: 0x0004 */
  cs_uint16                                rsrvd1[251];
  ARN5_SPI4P2_TXIF_dynalign_t              dynalign;                      /* addr: 0x0100 */
  ARN5_SPI4P2_TXIF_numdip4err_t            numdip4err;                    /* addr: 0x0101 */
  ARN5_SPI4P2_TXIF_numdip4match_t          numdip4match;                  /* addr: 0x0102 */
  ARN5_SPI4P2_TXIF_txbuf_almostfull_t      txbuf_almostfull;              /* addr: 0x0103 */
  ARN5_SPI4P2_TXIF_spi42ctl_t              spi42ctl;                      /* addr: 0x0104 */
  ARN5_SPI4P2_TXIF_spi3ctl_t               txif_spi3ctl;                  /* addr: 0x0105 */
  ARN5_SPI4P2_TXIF_fifo_eccerr_ctrl_t      fifo_eccerr_ctrl;              /* addr: 0x0106 */
  ARN5_SPI4P2_TXIF_fifo_eccfix_ctrl_t      fifo_eccfix_ctrl;              /* addr: 0x0107 */
  ARN5_SPI4P2_TXIF_txintrpt_t              txintrpt;                      /* addr: 0x0108 */
  ARN5_SPI4P2_TXIF_txintrpten_t            txintrpten;                    /* addr: 0x0109 */
  ARN5_SPI4P2_TXIF_txloscnt_t              txloscnt;                      /* addr: 0x010a */
  ARN5_SPI4P2_TXIF_dip4errcnt_t            dip4errcnt;                    /* addr: 0x010b */
  ARN5_SPI4P2_TXIF_txburstsizeerr_t        txburstsizeerr;                /* addr: 0x010c */
  ARN5_SPI4P2_TXIF_txsopdisterr_t          txsopdisterr;                  /* addr: 0x010d */
  ARN5_SPI4P2_TXIF_txseqerr_t              txseqerr;                      /* addr: 0x010e */
  ARN5_SPI4P2_TXIF_txrsvctrlwderr_t        txrsvctrlwderr;                /* addr: 0x010f */
  ARN5_SPI4P2_TXIF_txdskwoverrideen0_t     txdskwoverrideen0;             /* addr: 0x0110 */
  ARN5_SPI4P2_TXIF_txdskwoverrideen1_t     txdskwoverrideen1;             /* addr: 0x0111 */
  ARN5_SPI4P2_TXIF_txdskwoverride0_t       txdskwoverride0;               /* addr: 0x0112 */
  ARN5_SPI4P2_TXIF_txdskwoverride1_t       txdskwoverride1;               /* addr: 0x0113 */
  ARN5_SPI4P2_TXIF_txdskwoverride2_t       txdskwoverride2;               /* addr: 0x0114 */
  ARN5_SPI4P2_TXIF_txdskwoverride3_t       txdskwoverride3;               /* addr: 0x0115 */
  ARN5_SPI4P2_TXIF_txdskwoverride4_t       txdskwoverride4;               /* addr: 0x0116 */
  ARN5_SPI4P2_TXIF_txdskw0_t               txdskw0;                       /* addr: 0x0117 */
  ARN5_SPI4P2_TXIF_txdskw1_t               txdskw1;                       /* addr: 0x0118 */
  ARN5_SPI4P2_TXIF_txdskw2_t               txdskw2;                       /* addr: 0x0119 */
  ARN5_SPI4P2_TXIF_txdskw3_t               txdskw3;                       /* addr: 0x011a */
  ARN5_SPI4P2_TXIF_txdskw4_t               txdskw4;                       /* addr: 0x011b */
  ARN5_SPI4P2_TXIF_txdrctrl_t              txdrctrl;                      /* addr: 0x011c */
  ARN5_SPI4P2_TXIF_txoverride1_t           txoverride1;                   /* addr: 0x011d */
  ARN5_SPI4P2_TXIF_txoverride2_t           txoverride2;                   /* addr: 0x011e */
  ARN5_SPI4P2_TXIF_txoverride3_t           txoverride3;                   /* addr: 0x011f */
  ARN5_SPI4P2_TXIF_txoverride4_t           txoverride4;                   /* addr: 0x0120 */
  ARN5_SPI4P2_TXIF_txoverride5_t           txoverride5;                   /* addr: 0x0121 */
  ARN5_SPI4P2_TXIF_txfreeze1_t             txfreeze1;                     /* addr: 0x0122 */
  ARN5_SPI4P2_TXIF_txfreeze2_t             txfreeze2;                     /* addr: 0x0123 */
  ARN5_SPI4P2_TXIF_txspare_t               txspare;                       /* addr: 0x0124 */
  ARN5_SPI4P2_TXIF_txcurphase1_t           txcurphase1;                   /* addr: 0x0125 */
  ARN5_SPI4P2_TXIF_txcurphase2_t           txcurphase2;                   /* addr: 0x0126 */
  ARN5_SPI4P2_TXIF_txcurphase3_t           txcurphase3;                   /* addr: 0x0127 */
  ARN5_SPI4P2_TXIF_txcurphase4_t           txcurphase4;                   /* addr: 0x0128 */
  ARN5_SPI4P2_TXIF_txcurphase5_t           txcurphase5;                   /* addr: 0x0129 */
  ARN5_SPI4P2_TXIF_biststart_t             biststart;                     /* addr: 0x012a */
  ARN5_SPI4P2_TXIF_bistenable0_t           bistenable0;                   /* addr: 0x012b */
  ARN5_SPI4P2_TXIF_bistenable1_t           bistenable1;                   /* addr: 0x012c */
  ARN5_SPI4P2_TXIF_bistdone0_t             bistdone0;                     /* addr: 0x012d */
  ARN5_SPI4P2_TXIF_bistdone1_t             bistdone1;                     /* addr: 0x012e */
  ARN5_SPI4P2_TXIF_bistpass0_t             bistpass0;                     /* addr: 0x012f */
  ARN5_SPI4P2_TXIF_bistpass1_t             bistpass1;                     /* addr: 0x0130 */
  ARN5_SPI4P2_TXIF_bistphaseoverride0_t    bistphaseoverride0;            /* addr: 0x0131 */
  ARN5_SPI4P2_TXIF_bistphaseoverride1_t    bistphaseoverride1;            /* addr: 0x0132 */
  ARN5_SPI4P2_TXIF_bisttesttime_t          bisttesttime;                  /* addr: 0x0133 */
  ARN5_SPI4P2_TXIF_bistsettletime_t        bistsettletime;                /* addr: 0x0134 */
  ARN5_SPI4P2_TXIF_bistpattern0_t          bistpattern0;                  /* addr: 0x0135 */
  ARN5_SPI4P2_TXIF_bistpattern1_t          bistpattern1;                  /* addr: 0x0136 */
  ARN5_SPI4P2_TXIF_bistpattern2_t          bistpattern2;                  /* addr: 0x0137 */
  ARN5_SPI4P2_TXIF_bistpattern3_t          bistpattern3;                  /* addr: 0x0138 */
  ARN5_SPI4P2_TXIF_bistpattern4_t          bistpattern4;                  /* addr: 0x0139 */
  ARN5_SPI4P2_TXIF_bistpattern5_t          bistpattern5;                  /* addr: 0x013a */
  ARN5_SPI4P2_TXIF_bisterror0_t            bisterror0;                    /* addr: 0x013b */
  ARN5_SPI4P2_TXIF_bisterror1_t            bisterror1;                    /* addr: 0x013c */
  ARN5_SPI4P2_TXIF_bisterror2_t            bisterror2;                    /* addr: 0x013d */
  ARN5_SPI4P2_TXIF_bisterror3_t            bisterror3;                    /* addr: 0x013e */
  ARN5_SPI4P2_TXIF_bisterror4_t            bisterror4;                    /* addr: 0x013f */
  ARN5_SPI4P2_TXIF_bisterror5_t            bisterror5;                    /* addr: 0x0140 */
  cs_uint16                                rsrvd2[191];
  ARN5_SPI4P2_RXIF_dip4errctrl_t           dip4errctrl;                   /* addr: 0x0200 */
  ARN5_SPI4P2_RXIF_spi3ctl_t               rxif_spi3ctl;                  /* addr: 0x0201 */
  ARN5_SPI4P2_RXIF_intrpt_t                rxif_intrpt;                   /* addr: 0x0202 */
  ARN5_SPI4P2_RXIF_intrpten_t              rxif_intrpten;                 /* addr: 0x0203 */
  ARN5_SPI4P2_RXIF_rxspi42_perrcnt_t       rxspi42_perrcnt;               /* addr: 0x0204 */
  ARN5_SPI4P2_RXIF_rxphaseadj1_t           rxphaseadj1;                   /* addr: 0x0205 */
  ARN5_SPI4P2_RXIF_rxphaseadj2_t           rxphaseadj2;                   /* addr: 0x0206 */
  ARN5_SPI4P2_RXIF_rxphaseadj3_t           rxphaseadj3;                   /* addr: 0x0207 */
  ARN5_SPI4P2_RXIF_rxphaseadj4_t           rxphaseadj4;                   /* addr: 0x0208 */
  ARN5_SPI4P2_RXIF_rxphaseadj5_t           rxphaseadj5;                   /* addr: 0x0209 */
  cs_uint16                                rsrvd3[246];
  ARN5_SPI4P2_MIF_rxpllcontrol1_t          rxpllcontrol1;                 /* addr: 0x0300 */
  ARN5_SPI4P2_MIF_rxpllcontrol2_t          rxpllcontrol2;                 /* addr: 0x0301 */
  ARN5_SPI4P2_MIF_txpllcontrol1_t          txpllcontrol1;                 /* addr: 0x0302 */
  ARN5_SPI4P2_MIF_txpllcontrol2_t          txpllcontrol2;                 /* addr: 0x0303 */
  ARN5_SPI4P2_MIF_loopbacks_t              loopbacks;                     /* addr: 0x0304 */
  ARN5_SPI4P2_MIF_mode_t                   mode;                          /* addr: 0x0305 */
  ARN5_SPI4P2_MIF_status_t                 mif_status;                    /* addr: 0x0306 */
  ARN5_SPI4P2_MIF_fmro_sel_t               fmro_sel;                      /* addr: 0x0307 */
  cs_uint16                                rsrvd4[248];
  ARN5_SPI4P2_TXCAL_ent_0_t                txcal_ent_0;                   /* addr: 0x0400 */
  ARN5_SPI4P2_TXCAL_ent_1_t                txcal_ent_1;                   /* addr: 0x0401 */
  ARN5_SPI4P2_TXCAL_ent_2_t                txcal_ent_2;                   /* addr: 0x0402 */
  ARN5_SPI4P2_TXCAL_ent_3_t                txcal_ent_3;                   /* addr: 0x0403 */
  ARN5_SPI4P2_TXCAL_ent_4_t                txcal_ent_4;                   /* addr: 0x0404 */
  ARN5_SPI4P2_TXCAL_ent_5_t                txcal_ent_5;                   /* addr: 0x0405 */
  ARN5_SPI4P2_TXCAL_ent_6_t                txcal_ent_6;                   /* addr: 0x0406 */
  ARN5_SPI4P2_TXCAL_ent_7_t                txcal_ent_7;                   /* addr: 0x0407 */
  ARN5_SPI4P2_TXCAL_ent_8_t                txcal_ent_8;                   /* addr: 0x0408 */
  ARN5_SPI4P2_TXCAL_ent_9_t                txcal_ent_9;                   /* addr: 0x0409 */
  ARN5_SPI4P2_TXCAL_ent_10_t               txcal_ent_10;                  /* addr: 0x040a */
  ARN5_SPI4P2_TXCAL_ent_11_t               txcal_ent_11;                  /* addr: 0x040b */
  ARN5_SPI4P2_TXCAL_ent_12_t               txcal_ent_12;                  /* addr: 0x040c */
  ARN5_SPI4P2_TXCAL_ent_13_t               txcal_ent_13;                  /* addr: 0x040d */
  ARN5_SPI4P2_TXCAL_ent_14_t               txcal_ent_14;                  /* addr: 0x040e */
  ARN5_SPI4P2_TXCAL_ent_15_t               txcal_ent_15;                  /* addr: 0x040f */
  ARN5_SPI4P2_TXCAL_ent_16_t               txcal_ent_16;                  /* addr: 0x0410 */
  ARN5_SPI4P2_TXCAL_ent_17_t               txcal_ent_17;                  /* addr: 0x0411 */
  ARN5_SPI4P2_TXCAL_ent_18_t               txcal_ent_18;                  /* addr: 0x0412 */
  ARN5_SPI4P2_TXCAL_ent_19_t               txcal_ent_19;                  /* addr: 0x0413 */
  ARN5_SPI4P2_TXCAL_ent_20_t               txcal_ent_20;                  /* addr: 0x0414 */
  ARN5_SPI4P2_TXCAL_ent_21_t               txcal_ent_21;                  /* addr: 0x0415 */
  ARN5_SPI4P2_TXCAL_ent_22_t               txcal_ent_22;                  /* addr: 0x0416 */
  ARN5_SPI4P2_TXCAL_ent_23_t               txcal_ent_23;                  /* addr: 0x0417 */
  ARN5_SPI4P2_TXCAL_ent_24_t               txcal_ent_24;                  /* addr: 0x0418 */
  ARN5_SPI4P2_TXCAL_ent_25_t               txcal_ent_25;                  /* addr: 0x0419 */
  ARN5_SPI4P2_TXCAL_ent_26_t               txcal_ent_26;                  /* addr: 0x041a */
  ARN5_SPI4P2_TXCAL_ent_27_t               txcal_ent_27;                  /* addr: 0x041b */
  ARN5_SPI4P2_TXCAL_ent_28_t               txcal_ent_28;                  /* addr: 0x041c */
  ARN5_SPI4P2_TXCAL_ent_29_t               txcal_ent_29;                  /* addr: 0x041d */
  ARN5_SPI4P2_TXCAL_ent_30_t               txcal_ent_30;                  /* addr: 0x041e */
  ARN5_SPI4P2_TXCAL_ent_31_t               txcal_ent_31;                  /* addr: 0x041f */
  ARN5_SPI4P2_TXCAL_ent_32_t               txcal_ent_32;                  /* addr: 0x0420 */
  ARN5_SPI4P2_TXCAL_ent_33_t               txcal_ent_33;                  /* addr: 0x0421 */
  ARN5_SPI4P2_TXCAL_ent_34_t               txcal_ent_34;                  /* addr: 0x0422 */
  ARN5_SPI4P2_TXCAL_ent_35_t               txcal_ent_35;                  /* addr: 0x0423 */
  ARN5_SPI4P2_TXCAL_ent_36_t               txcal_ent_36;                  /* addr: 0x0424 */
  ARN5_SPI4P2_TXCAL_ent_37_t               txcal_ent_37;                  /* addr: 0x0425 */
  ARN5_SPI4P2_TXCAL_ent_38_t               txcal_ent_38;                  /* addr: 0x0426 */
  ARN5_SPI4P2_TXCAL_ent_39_t               txcal_ent_39;                  /* addr: 0x0427 */
  ARN5_SPI4P2_TXCAL_ent_40_t               txcal_ent_40;                  /* addr: 0x0428 */
  ARN5_SPI4P2_TXCAL_ent_41_t               txcal_ent_41;                  /* addr: 0x0429 */
  ARN5_SPI4P2_TXCAL_ent_42_t               txcal_ent_42;                  /* addr: 0x042a */
  ARN5_SPI4P2_TXCAL_ent_43_t               txcal_ent_43;                  /* addr: 0x042b */
  ARN5_SPI4P2_TXCAL_ent_44_t               txcal_ent_44;                  /* addr: 0x042c */
  ARN5_SPI4P2_TXCAL_ent_45_t               txcal_ent_45;                  /* addr: 0x042d */
  ARN5_SPI4P2_TXCAL_ent_46_t               txcal_ent_46;                  /* addr: 0x042e */
  ARN5_SPI4P2_TXCAL_ent_47_t               txcal_ent_47;                  /* addr: 0x042f */
  ARN5_SPI4P2_TXCAL_ent_48_t               txcal_ent_48;                  /* addr: 0x0430 */
  ARN5_SPI4P2_TXCAL_ent_49_t               txcal_ent_49;                  /* addr: 0x0431 */
  ARN5_SPI4P2_TXCAL_ent_50_t               txcal_ent_50;                  /* addr: 0x0432 */
  ARN5_SPI4P2_TXCAL_ent_51_t               txcal_ent_51;                  /* addr: 0x0433 */
  ARN5_SPI4P2_TXCAL_ent_52_t               txcal_ent_52;                  /* addr: 0x0434 */
  ARN5_SPI4P2_TXCAL_ent_53_t               txcal_ent_53;                  /* addr: 0x0435 */
  ARN5_SPI4P2_TXCAL_ent_54_t               txcal_ent_54;                  /* addr: 0x0436 */
  ARN5_SPI4P2_TXCAL_ent_55_t               txcal_ent_55;                  /* addr: 0x0437 */
  ARN5_SPI4P2_TXCAL_ent_56_t               txcal_ent_56;                  /* addr: 0x0438 */
  ARN5_SPI4P2_TXCAL_ent_57_t               txcal_ent_57;                  /* addr: 0x0439 */
  ARN5_SPI4P2_TXCAL_ent_58_t               txcal_ent_58;                  /* addr: 0x043a */
  ARN5_SPI4P2_TXCAL_ent_59_t               txcal_ent_59;                  /* addr: 0x043b */
  ARN5_SPI4P2_TXCAL_ent_60_t               txcal_ent_60;                  /* addr: 0x043c */
  ARN5_SPI4P2_TXCAL_ent_61_t               txcal_ent_61;                  /* addr: 0x043d */
  ARN5_SPI4P2_TXCAL_ent_62_t               txcal_ent_62;                  /* addr: 0x043e */
  ARN5_SPI4P2_TXCAL_ent_63_t               txcal_ent_63;                  /* addr: 0x043f */
  ARN5_SPI4P2_TXCAL_ent_64_t               txcal_ent_64;                  /* addr: 0x0440 */
  ARN5_SPI4P2_TXCAL_ent_65_t               txcal_ent_65;                  /* addr: 0x0441 */
  ARN5_SPI4P2_TXCAL_ent_66_t               txcal_ent_66;                  /* addr: 0x0442 */
  ARN5_SPI4P2_TXCAL_ent_67_t               txcal_ent_67;                  /* addr: 0x0443 */
  ARN5_SPI4P2_TXCAL_ent_68_t               txcal_ent_68;                  /* addr: 0x0444 */
  ARN5_SPI4P2_TXCAL_ent_69_t               txcal_ent_69;                  /* addr: 0x0445 */
  ARN5_SPI4P2_TXCAL_ent_70_t               txcal_ent_70;                  /* addr: 0x0446 */
  ARN5_SPI4P2_TXCAL_ent_71_t               txcal_ent_71;                  /* addr: 0x0447 */
  ARN5_SPI4P2_TXCAL_ent_72_t               txcal_ent_72;                  /* addr: 0x0448 */
  ARN5_SPI4P2_TXCAL_ent_73_t               txcal_ent_73;                  /* addr: 0x0449 */
  ARN5_SPI4P2_TXCAL_ent_74_t               txcal_ent_74;                  /* addr: 0x044a */
  ARN5_SPI4P2_TXCAL_ent_75_t               txcal_ent_75;                  /* addr: 0x044b */
  ARN5_SPI4P2_TXCAL_ent_76_t               txcal_ent_76;                  /* addr: 0x044c */
  ARN5_SPI4P2_TXCAL_ent_77_t               txcal_ent_77;                  /* addr: 0x044d */
  ARN5_SPI4P2_TXCAL_ent_78_t               txcal_ent_78;                  /* addr: 0x044e */
  ARN5_SPI4P2_TXCAL_ent_79_t               txcal_ent_79;                  /* addr: 0x044f */
  ARN5_SPI4P2_TXCAL_ent_80_t               txcal_ent_80;                  /* addr: 0x0450 */
  ARN5_SPI4P2_TXCAL_ent_81_t               txcal_ent_81;                  /* addr: 0x0451 */
  ARN5_SPI4P2_TXCAL_ent_82_t               txcal_ent_82;                  /* addr: 0x0452 */
  ARN5_SPI4P2_TXCAL_ent_83_t               txcal_ent_83;                  /* addr: 0x0453 */
  ARN5_SPI4P2_TXCAL_ent_84_t               txcal_ent_84;                  /* addr: 0x0454 */
  ARN5_SPI4P2_TXCAL_ent_85_t               txcal_ent_85;                  /* addr: 0x0455 */
  ARN5_SPI4P2_TXCAL_ent_86_t               txcal_ent_86;                  /* addr: 0x0456 */
  ARN5_SPI4P2_TXCAL_ent_87_t               txcal_ent_87;                  /* addr: 0x0457 */
  ARN5_SPI4P2_TXCAL_ent_88_t               txcal_ent_88;                  /* addr: 0x0458 */
  ARN5_SPI4P2_TXCAL_ent_89_t               txcal_ent_89;                  /* addr: 0x0459 */
  ARN5_SPI4P2_TXCAL_ent_90_t               txcal_ent_90;                  /* addr: 0x045a */
  ARN5_SPI4P2_TXCAL_ent_91_t               txcal_ent_91;                  /* addr: 0x045b */
  ARN5_SPI4P2_TXCAL_ent_92_t               txcal_ent_92;                  /* addr: 0x045c */
  ARN5_SPI4P2_TXCAL_ent_93_t               txcal_ent_93;                  /* addr: 0x045d */
  ARN5_SPI4P2_TXCAL_ent_94_t               txcal_ent_94;                  /* addr: 0x045e */
  ARN5_SPI4P2_TXCAL_ent_95_t               txcal_ent_95;                  /* addr: 0x045f */
  ARN5_SPI4P2_TXCAL_phy_ch_num_lch0_t      phy_ch_num_lch0;               /* addr: 0x0460 */
  ARN5_SPI4P2_TXCAL_phy_ch_num_lch1_t      phy_ch_num_lch1;               /* addr: 0x0461 */
  ARN5_SPI4P2_TXCAL_phy_ch_num_lch2_t      phy_ch_num_lch2;               /* addr: 0x0462 */
  ARN5_SPI4P2_TXCAL_phy_ch_num_lch3_t      phy_ch_num_lch3;               /* addr: 0x0463 */
  ARN5_SPI4P2_TXCAL_phy_ch_num_lch4_t      phy_ch_num_lch4;               /* addr: 0x0464 */
  ARN5_SPI4P2_TXCAL_phy_ch_num_lch5_t      phy_ch_num_lch5;               /* addr: 0x0465 */
  ARN5_SPI4P2_TXCAL_phy_ch_num_lch6_t      phy_ch_num_lch6;               /* addr: 0x0466 */
  ARN5_SPI4P2_TXCAL_phy_ch_num_lch7_t      phy_ch_num_lch7;               /* addr: 0x0467 */
  ARN5_SPI4P2_TXCAL_cal_len_t              txcal_cal_len;                 /* addr: 0x0468 */
  ARN5_SPI4P2_TXCAL_cal_m_t                txcal_cal_m;                   /* addr: 0x0469 */
  cs_uint16                                rsrvd5[150];
  ARN5_SPI4P2_RXCAL_ent_0_t                rxcal_ent_0;                   /* addr: 0x0500 */
  ARN5_SPI4P2_RXCAL_ent_1_t                rxcal_ent_1;                   /* addr: 0x0501 */
  ARN5_SPI4P2_RXCAL_ent_2_t                rxcal_ent_2;                   /* addr: 0x0502 */
  ARN5_SPI4P2_RXCAL_ent_3_t                rxcal_ent_3;                   /* addr: 0x0503 */
  ARN5_SPI4P2_RXCAL_ent_4_t                rxcal_ent_4;                   /* addr: 0x0504 */
  ARN5_SPI4P2_RXCAL_ent_5_t                rxcal_ent_5;                   /* addr: 0x0505 */
  ARN5_SPI4P2_RXCAL_ent_6_t                rxcal_ent_6;                   /* addr: 0x0506 */
  ARN5_SPI4P2_RXCAL_ent_7_t                rxcal_ent_7;                   /* addr: 0x0507 */
  ARN5_SPI4P2_RXCAL_ent_8_t                rxcal_ent_8;                   /* addr: 0x0508 */
  ARN5_SPI4P2_RXCAL_ent_9_t                rxcal_ent_9;                   /* addr: 0x0509 */
  ARN5_SPI4P2_RXCAL_ent_10_t               rxcal_ent_10;                  /* addr: 0x050a */
  ARN5_SPI4P2_RXCAL_ent_11_t               rxcal_ent_11;                  /* addr: 0x050b */
  ARN5_SPI4P2_RXCAL_ent_12_t               rxcal_ent_12;                  /* addr: 0x050c */
  ARN5_SPI4P2_RXCAL_ent_13_t               rxcal_ent_13;                  /* addr: 0x050d */
  ARN5_SPI4P2_RXCAL_ent_14_t               rxcal_ent_14;                  /* addr: 0x050e */
  ARN5_SPI4P2_RXCAL_ent_15_t               rxcal_ent_15;                  /* addr: 0x050f */
  ARN5_SPI4P2_RXCAL_ent_16_t               rxcal_ent_16;                  /* addr: 0x0510 */
  ARN5_SPI4P2_RXCAL_ent_17_t               rxcal_ent_17;                  /* addr: 0x0511 */
  ARN5_SPI4P2_RXCAL_ent_18_t               rxcal_ent_18;                  /* addr: 0x0512 */
  ARN5_SPI4P2_RXCAL_ent_19_t               rxcal_ent_19;                  /* addr: 0x0513 */
  ARN5_SPI4P2_RXCAL_ent_20_t               rxcal_ent_20;                  /* addr: 0x0514 */
  ARN5_SPI4P2_RXCAL_ent_21_t               rxcal_ent_21;                  /* addr: 0x0515 */
  ARN5_SPI4P2_RXCAL_ent_22_t               rxcal_ent_22;                  /* addr: 0x0516 */
  ARN5_SPI4P2_RXCAL_ent_23_t               rxcal_ent_23;                  /* addr: 0x0517 */
  ARN5_SPI4P2_RXCAL_ent_24_t               rxcal_ent_24;                  /* addr: 0x0518 */
  ARN5_SPI4P2_RXCAL_ent_25_t               rxcal_ent_25;                  /* addr: 0x0519 */
  ARN5_SPI4P2_RXCAL_ent_26_t               rxcal_ent_26;                  /* addr: 0x051a */
  ARN5_SPI4P2_RXCAL_ent_27_t               rxcal_ent_27;                  /* addr: 0x051b */
  ARN5_SPI4P2_RXCAL_ent_28_t               rxcal_ent_28;                  /* addr: 0x051c */
  ARN5_SPI4P2_RXCAL_ent_29_t               rxcal_ent_29;                  /* addr: 0x051d */
  ARN5_SPI4P2_RXCAL_ent_30_t               rxcal_ent_30;                  /* addr: 0x051e */
  ARN5_SPI4P2_RXCAL_ent_31_t               rxcal_ent_31;                  /* addr: 0x051f */
  ARN5_SPI4P2_RXCAL_ent_32_t               rxcal_ent_32;                  /* addr: 0x0520 */
  ARN5_SPI4P2_RXCAL_ent_33_t               rxcal_ent_33;                  /* addr: 0x0521 */
  ARN5_SPI4P2_RXCAL_ent_34_t               rxcal_ent_34;                  /* addr: 0x0522 */
  ARN5_SPI4P2_RXCAL_ent_35_t               rxcal_ent_35;                  /* addr: 0x0523 */
  ARN5_SPI4P2_RXCAL_ent_36_t               rxcal_ent_36;                  /* addr: 0x0524 */
  ARN5_SPI4P2_RXCAL_ent_37_t               rxcal_ent_37;                  /* addr: 0x0525 */
  ARN5_SPI4P2_RXCAL_ent_38_t               rxcal_ent_38;                  /* addr: 0x0526 */
  ARN5_SPI4P2_RXCAL_ent_39_t               rxcal_ent_39;                  /* addr: 0x0527 */
  ARN5_SPI4P2_RXCAL_ent_40_t               rxcal_ent_40;                  /* addr: 0x0528 */
  ARN5_SPI4P2_RXCAL_ent_41_t               rxcal_ent_41;                  /* addr: 0x0529 */
  ARN5_SPI4P2_RXCAL_ent_42_t               rxcal_ent_42;                  /* addr: 0x052a */
  ARN5_SPI4P2_RXCAL_ent_43_t               rxcal_ent_43;                  /* addr: 0x052b */
  ARN5_SPI4P2_RXCAL_ent_44_t               rxcal_ent_44;                  /* addr: 0x052c */
  ARN5_SPI4P2_RXCAL_ent_45_t               rxcal_ent_45;                  /* addr: 0x052d */
  ARN5_SPI4P2_RXCAL_ent_46_t               rxcal_ent_46;                  /* addr: 0x052e */
  ARN5_SPI4P2_RXCAL_ent_47_t               rxcal_ent_47;                  /* addr: 0x052f */
  ARN5_SPI4P2_RXCAL_ent_48_t               rxcal_ent_48;                  /* addr: 0x0530 */
  ARN5_SPI4P2_RXCAL_ent_49_t               rxcal_ent_49;                  /* addr: 0x0531 */
  ARN5_SPI4P2_RXCAL_ent_50_t               rxcal_ent_50;                  /* addr: 0x0532 */
  ARN5_SPI4P2_RXCAL_ent_51_t               rxcal_ent_51;                  /* addr: 0x0533 */
  ARN5_SPI4P2_RXCAL_ent_52_t               rxcal_ent_52;                  /* addr: 0x0534 */
  ARN5_SPI4P2_RXCAL_ent_53_t               rxcal_ent_53;                  /* addr: 0x0535 */
  ARN5_SPI4P2_RXCAL_ent_54_t               rxcal_ent_54;                  /* addr: 0x0536 */
  ARN5_SPI4P2_RXCAL_ent_55_t               rxcal_ent_55;                  /* addr: 0x0537 */
  ARN5_SPI4P2_RXCAL_ent_56_t               rxcal_ent_56;                  /* addr: 0x0538 */
  ARN5_SPI4P2_RXCAL_ent_57_t               rxcal_ent_57;                  /* addr: 0x0539 */
  ARN5_SPI4P2_RXCAL_ent_58_t               rxcal_ent_58;                  /* addr: 0x053a */
  ARN5_SPI4P2_RXCAL_ent_59_t               rxcal_ent_59;                  /* addr: 0x053b */
  ARN5_SPI4P2_RXCAL_ent_60_t               rxcal_ent_60;                  /* addr: 0x053c */
  ARN5_SPI4P2_RXCAL_ent_61_t               rxcal_ent_61;                  /* addr: 0x053d */
  ARN5_SPI4P2_RXCAL_ent_62_t               rxcal_ent_62;                  /* addr: 0x053e */
  ARN5_SPI4P2_RXCAL_ent_63_t               rxcal_ent_63;                  /* addr: 0x053f */
  ARN5_SPI4P2_RXCAL_ent_64_t               rxcal_ent_64;                  /* addr: 0x0540 */
  ARN5_SPI4P2_RXCAL_ent_65_t               rxcal_ent_65;                  /* addr: 0x0541 */
  ARN5_SPI4P2_RXCAL_ent_66_t               rxcal_ent_66;                  /* addr: 0x0542 */
  ARN5_SPI4P2_RXCAL_ent_67_t               rxcal_ent_67;                  /* addr: 0x0543 */
  ARN5_SPI4P2_RXCAL_ent_68_t               rxcal_ent_68;                  /* addr: 0x0544 */
  ARN5_SPI4P2_RXCAL_ent_69_t               rxcal_ent_69;                  /* addr: 0x0545 */
  ARN5_SPI4P2_RXCAL_ent_70_t               rxcal_ent_70;                  /* addr: 0x0546 */
  ARN5_SPI4P2_RXCAL_ent_71_t               rxcal_ent_71;                  /* addr: 0x0547 */
  ARN5_SPI4P2_RXCAL_ent_72_t               rxcal_ent_72;                  /* addr: 0x0548 */
  ARN5_SPI4P2_RXCAL_ent_73_t               rxcal_ent_73;                  /* addr: 0x0549 */
  ARN5_SPI4P2_RXCAL_ent_74_t               rxcal_ent_74;                  /* addr: 0x054a */
  ARN5_SPI4P2_RXCAL_ent_75_t               rxcal_ent_75;                  /* addr: 0x054b */
  ARN5_SPI4P2_RXCAL_ent_76_t               rxcal_ent_76;                  /* addr: 0x054c */
  ARN5_SPI4P2_RXCAL_ent_77_t               rxcal_ent_77;                  /* addr: 0x054d */
  ARN5_SPI4P2_RXCAL_ent_78_t               rxcal_ent_78;                  /* addr: 0x054e */
  ARN5_SPI4P2_RXCAL_ent_79_t               rxcal_ent_79;                  /* addr: 0x054f */
  ARN5_SPI4P2_RXCAL_ent_80_t               rxcal_ent_80;                  /* addr: 0x0550 */
  ARN5_SPI4P2_RXCAL_ent_81_t               rxcal_ent_81;                  /* addr: 0x0551 */
  ARN5_SPI4P2_RXCAL_ent_82_t               rxcal_ent_82;                  /* addr: 0x0552 */
  ARN5_SPI4P2_RXCAL_ent_83_t               rxcal_ent_83;                  /* addr: 0x0553 */
  ARN5_SPI4P2_RXCAL_ent_84_t               rxcal_ent_84;                  /* addr: 0x0554 */
  ARN5_SPI4P2_RXCAL_ent_85_t               rxcal_ent_85;                  /* addr: 0x0555 */
  ARN5_SPI4P2_RXCAL_ent_86_t               rxcal_ent_86;                  /* addr: 0x0556 */
  ARN5_SPI4P2_RXCAL_ent_87_t               rxcal_ent_87;                  /* addr: 0x0557 */
  ARN5_SPI4P2_RXCAL_ent_88_t               rxcal_ent_88;                  /* addr: 0x0558 */
  ARN5_SPI4P2_RXCAL_ent_89_t               rxcal_ent_89;                  /* addr: 0x0559 */
  ARN5_SPI4P2_RXCAL_ent_90_t               rxcal_ent_90;                  /* addr: 0x055a */
  ARN5_SPI4P2_RXCAL_ent_91_t               rxcal_ent_91;                  /* addr: 0x055b */
  ARN5_SPI4P2_RXCAL_ent_92_t               rxcal_ent_92;                  /* addr: 0x055c */
  ARN5_SPI4P2_RXCAL_ent_93_t               rxcal_ent_93;                  /* addr: 0x055d */
  ARN5_SPI4P2_RXCAL_ent_94_t               rxcal_ent_94;                  /* addr: 0x055e */
  ARN5_SPI4P2_RXCAL_ent_95_t               rxcal_ent_95;                  /* addr: 0x055f */
  ARN5_SPI4P2_RXCAL_lch_num_pch0_t         lch_num_pch0;                  /* addr: 0x0560 */
  ARN5_SPI4P2_RXCAL_lch_num_pch1_t         lch_num_pch1;                  /* addr: 0x0561 */
  ARN5_SPI4P2_RXCAL_lch_num_pch2_t         lch_num_pch2;                  /* addr: 0x0562 */
  ARN5_SPI4P2_RXCAL_lch_num_pch3_t         lch_num_pch3;                  /* addr: 0x0563 */
  ARN5_SPI4P2_RXCAL_lch_num_pch4_t         lch_num_pch4;                  /* addr: 0x0564 */
  ARN5_SPI4P2_RXCAL_lch_num_pch5_t         lch_num_pch5;                  /* addr: 0x0565 */
  ARN5_SPI4P2_RXCAL_lch_num_pch6_t         lch_num_pch6;                  /* addr: 0x0566 */
  ARN5_SPI4P2_RXCAL_lch_num_pch7_t         lch_num_pch7;                  /* addr: 0x0567 */
  ARN5_SPI4P2_RXCAL_cal_len_t              rxcal_cal_len;                 /* addr: 0x0568 */
  ARN5_SPI4P2_RXCAL_sched_cal_len_t        sched_cal_len;                 /* addr: 0x0569 */
  ARN5_SPI4P2_RXCAL_cal_m_t                rxcal_cal_m;                   /* addr: 0x056a */
  cs_uint16                                rsrvd6[149];
  ARN5_SPI4P2_TXCORE_ch_min_words_reqd_for_tx_t ch_min_words_reqd_for_tx;      /* addr: 0x0600 */
  ARN5_SPI4P2_TXCORE_mem_fifo_low_t        mem_fifo_low;                  /* addr: 0x0601 */
  ARN5_SPI4P2_TXCORE_mem_full_t            mem_full;                      /* addr: 0x0602 */
  ARN5_SPI4P2_TXCORE_q3_fifo_high_t        q3_fifo_high;                  /* addr: 0x0603 */
  ARN5_SPI4P2_TXCORE_mem_fifo_t            mem_fifo;                      /* addr: 0x0604 */
  ARN5_SPI4P2_TXCORE_spi3_single_fifo_full_t spi3_single_fifo_full;         /* addr: 0x0605 */
  ARN5_SPI4P2_TXCORE_pm_ctrl_t             txcore_pm_ctrl;                /* addr: 0x0606 */
  ARN5_SPI4P2_TXCORE_pm_data6_t            txcore_pm_data6;               /* addr: 0x0607 */
  ARN5_SPI4P2_TXCORE_pm_data5_t            txcore_pm_data5;               /* addr: 0x0608 */
  ARN5_SPI4P2_TXCORE_pm_data4_t            txcore_pm_data4;               /* addr: 0x0609 */
  ARN5_SPI4P2_TXCORE_pm_data3_t            txcore_pm_data3;               /* addr: 0x060a */
  ARN5_SPI4P2_TXCORE_pm_data2_t            txcore_pm_data2;               /* addr: 0x060b */
  ARN5_SPI4P2_TXCORE_pm_data1_t            txcore_pm_data1;               /* addr: 0x060c */
  ARN5_SPI4P2_TXCORE_pm_data0_t            txcore_pm_data0;               /* addr: 0x060d */
  cs_uint16                                rsrvd7[242];
  ARN5_SPI4P2_RXCORE_spi42_hysteresis_t    spi42_hysteresis;              /* addr: 0x0700 */
  ARN5_SPI4P2_RXCORE_spi42_spi3_threshold_t spi42_spi3_threshold;          /* addr: 0x0701 */
  ARN5_SPI4P2_RXCORE_chclose_t             chclose;                       /* addr: 0x0702 */
  ARN5_SPI4P2_RXCORE_numdip2err_t          numdip2err;                    /* addr: 0x0703 */
  ARN5_SPI4P2_RXCORE_numdip2match_t        numdip2match;                  /* addr: 0x0704 */
  ARN5_SPI4P2_RXCORE_alpha_t               alpha;                         /* addr: 0x0705 */
  ARN5_SPI4P2_RXCORE_datamaxt_t            datamaxt;                      /* addr: 0x0706 */
  ARN5_SPI4P2_RXCORE_burst_t               burst;                         /* addr: 0x0707 */
  ARN5_SPI4P2_RXCORE_maxburst1_t           maxburst1;                     /* addr: 0x0708 */
  ARN5_SPI4P2_RXCORE_maxburst2_t           maxburst2;                     /* addr: 0x0709 */
  ARN5_SPI4P2_RXCORE_bbenable_t            bbenable;                      /* addr: 0x070a */
  ARN5_SPI4P2_RXCORE_abs_full_t            abs_full;                      /* addr: 0x070b */
  ARN5_SPI4P2_RXCORE_almost_full_0_t       almost_full_0;                 /* addr: 0x070c */
  ARN5_SPI4P2_RXCORE_almost_empty_0_t      almost_empty_0;                /* addr: 0x070d */
  ARN5_SPI4P2_RXCORE_hysteresis_0_t        hysteresis_0;                  /* addr: 0x070e */
  ARN5_SPI4P2_RXCORE_almost_full_1_t       almost_full_1;                 /* addr: 0x070f */
  ARN5_SPI4P2_RXCORE_almost_empty_1_t      almost_empty_1;                /* addr: 0x0710 */
  ARN5_SPI4P2_RXCORE_hysteresis_1_t        hysteresis_1;                  /* addr: 0x0711 */
  ARN5_SPI4P2_RXCORE_almost_full_2_t       almost_full_2;                 /* addr: 0x0712 */
  ARN5_SPI4P2_RXCORE_almost_empty_2_t      almost_empty_2;                /* addr: 0x0713 */
  ARN5_SPI4P2_RXCORE_hysteresis_2_t        hysteresis_2;                  /* addr: 0x0714 */
  ARN5_SPI4P2_RXCORE_almost_full_3_t       almost_full_3;                 /* addr: 0x0715 */
  ARN5_SPI4P2_RXCORE_almost_empty_3_t      almost_empty_3;                /* addr: 0x0716 */
  ARN5_SPI4P2_RXCORE_hysteresis_3_t        hysteresis_3;                  /* addr: 0x0717 */
  ARN5_SPI4P2_RXCORE_almost_full_4_t       almost_full_4;                 /* addr: 0x0718 */
  ARN5_SPI4P2_RXCORE_almost_empty_4_t      almost_empty_4;                /* addr: 0x0719 */
  ARN5_SPI4P2_RXCORE_hysteresis_4_t        hysteresis_4;                  /* addr: 0x071a */
  ARN5_SPI4P2_RXCORE_almost_full_5_t       almost_full_5;                 /* addr: 0x071b */
  ARN5_SPI4P2_RXCORE_almost_empty_5_t      almost_empty_5;                /* addr: 0x071c */
  ARN5_SPI4P2_RXCORE_hysteresis_5_t        hysteresis_5;                  /* addr: 0x071d */
  ARN5_SPI4P2_RXCORE_almost_full_6_t       almost_full_6;                 /* addr: 0x071e */
  ARN5_SPI4P2_RXCORE_almost_empty_6_t      almost_empty_6;                /* addr: 0x071f */
  ARN5_SPI4P2_RXCORE_hysteresis_6_t        hysteresis_6;                  /* addr: 0x0720 */
  ARN5_SPI4P2_RXCORE_almost_full_7_t       almost_full_7;                 /* addr: 0x0721 */
  ARN5_SPI4P2_RXCORE_almost_empty_7_t      almost_empty_7;                /* addr: 0x0722 */
  ARN5_SPI4P2_RXCORE_hysteresis_7_t        hysteresis_7;                  /* addr: 0x0723 */
  ARN5_SPI4P2_RXCORE_pm_ctrl_t             rxcore_pm_ctrl;                /* addr: 0x0724 */
  ARN5_SPI4P2_RXCORE_pm_data6_t            rxcore_pm_data6;               /* addr: 0x0725 */
  ARN5_SPI4P2_RXCORE_pm_data5_t            rxcore_pm_data5;               /* addr: 0x0726 */
  ARN5_SPI4P2_RXCORE_pm_data4_t            rxcore_pm_data4;               /* addr: 0x0727 */
  ARN5_SPI4P2_RXCORE_pm_data3_t            rxcore_pm_data3;               /* addr: 0x0728 */
  ARN5_SPI4P2_RXCORE_pm_data2_t            rxcore_pm_data2;               /* addr: 0x0729 */
  ARN5_SPI4P2_RXCORE_pm_data1_t            rxcore_pm_data1;               /* addr: 0x072a */
  ARN5_SPI4P2_RXCORE_pm_data0_t            rxcore_pm_data0;               /* addr: 0x072b */
} ARN5_SPI4P2_t;

/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * SPI4P2a::tx_rprif
 */

#define ARN5_SPI4P2A_TX_RPRIF_afifo_intrpt       0x0a000
#define ARN5_SPI4P2A_TX_RPRIF_afifo_intrpt_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 afifo7_perr          :  1 ; /* bits 7:7 */
    cs_uint16 afifo6_perr          :  1 ; /* bits 6:6 */
    cs_uint16 afifo5_perr          :  1 ; /* bits 5:5 */
    cs_uint16 afifo4_perr          :  1 ; /* bits 4:4 */
    cs_uint16 afifo3_perr          :  1 ; /* bits 3:3 */
    cs_uint16 afifo2_perr          :  1 ; /* bits 2:2 */
    cs_uint16 afifo1_perr          :  1 ; /* bits 1:1 */
    cs_uint16 afifo0_perr          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 afifo0_perr          :  1 ; /* bits 0:0 */
    cs_uint16 afifo1_perr          :  1 ; /* bits 1:1 */
    cs_uint16 afifo2_perr          :  1 ; /* bits 2:2 */
    cs_uint16 afifo3_perr          :  1 ; /* bits 3:3 */
    cs_uint16 afifo4_perr          :  1 ; /* bits 4:4 */
    cs_uint16 afifo5_perr          :  1 ; /* bits 5:5 */
    cs_uint16 afifo6_perr          :  1 ; /* bits 6:6 */
    cs_uint16 afifo7_perr          :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TX_RPRIF_afifo_intrpt_t;

#define ARN5_SPI4P2A_TX_RPRIF_afifo_intrpten     0x0a001
#define ARN5_SPI4P2A_TX_RPRIF_afifo_intrpten_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 afifo7_perr_en       :  1 ; /* bits 7:7 */
    cs_uint16 afifo6_perr_en       :  1 ; /* bits 6:6 */
    cs_uint16 afifo5_perr_en       :  1 ; /* bits 5:5 */
    cs_uint16 afifo4_perr_en       :  1 ; /* bits 4:4 */
    cs_uint16 afifo3_perr_en       :  1 ; /* bits 3:3 */
    cs_uint16 afifo2_perr_en       :  1 ; /* bits 2:2 */
    cs_uint16 afifo1_perr_en       :  1 ; /* bits 1:1 */
    cs_uint16 afifo0_perr_en       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 afifo0_perr_en       :  1 ; /* bits 0:0 */
    cs_uint16 afifo1_perr_en       :  1 ; /* bits 1:1 */
    cs_uint16 afifo2_perr_en       :  1 ; /* bits 2:2 */
    cs_uint16 afifo3_perr_en       :  1 ; /* bits 3:3 */
    cs_uint16 afifo4_perr_en       :  1 ; /* bits 4:4 */
    cs_uint16 afifo5_perr_en       :  1 ; /* bits 5:5 */
    cs_uint16 afifo6_perr_en       :  1 ; /* bits 6:6 */
    cs_uint16 afifo7_perr_en       :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TX_RPRIF_afifo_intrpten_t;

#define ARN5_SPI4P2A_TX_RPRIF_intrpt_cntr_cfg0   0x0a002
#define ARN5_SPI4P2A_TX_RPRIF_intrpt_cntr_cfg0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 reg_sel              :  4 ; /* bits 7:4 */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
    cs_uint16 reg_sel              :  4 ; /* bits 7:4 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TX_RPRIF_intrpt_cntr_cfg0_t;
#define     ARN5_reg_sel_AFIFO_INTRPT                     0x00

#define ARN5_SPI4P2A_TX_RPRIF_intrpt_cntr0       0x0a003
#define ARN5_SPI4P2A_TX_RPRIF_intrpt_cntr0_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TX_RPRIF_intrpt_cntr0_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * SPI4P2a::rx_rprif
 */

#define ARN5_SPI4P2A_RX_RPRIF_dropch             0x0a100
#define ARN5_SPI4P2A_RX_RPRIF_dropch_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RX_RPRIF_dropch_t;

#define ARN5_SPI4P2A_RX_RPRIF_disable_filter_bad_pkts 0x0a101
#define ARN5_SPI4P2A_RX_RPRIF_disable_filter_bad_pkts_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 value                :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RX_RPRIF_disable_filter_bad_pkts_t;

#define ARN5_SPI4P2A_RX_RPRIF_intrpt_vec         0x0a102
#define ARN5_SPI4P2A_RX_RPRIF_intrpt_vec_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 spi4p1_intrpt        :  1 ; /* bits 2:2 */
    cs_uint16 misssop_intrpt       :  1 ; /* bits 1:1 */
    cs_uint16 misseop_intrpt       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 misseop_intrpt       :  1 ; /* bits 0:0 */
    cs_uint16 misssop_intrpt       :  1 ; /* bits 1:1 */
    cs_uint16 spi4p1_intrpt        :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RX_RPRIF_intrpt_vec_t;

#define ARN5_SPI4P2A_RX_RPRIF_intrpt_vec_en      0x0a103
#define ARN5_SPI4P2A_RX_RPRIF_intrpt_vec_en_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 int_en2              :  1 ; /* bits 2:2 */
    cs_uint16 int_en1              :  1 ; /* bits 1:1 */
    cs_uint16 int_en0              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 int_en0              :  1 ; /* bits 0:0 */
    cs_uint16 int_en1              :  1 ; /* bits 1:1 */
    cs_uint16 int_en2              :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RX_RPRIF_intrpt_vec_en_t;

#define ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt     0x0a104
#define ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_t;

#define ARN5_SPI4P2A_RX_RPRIF_misseop_intrpten   0x0a105
#define ARN5_SPI4P2A_RX_RPRIF_misseop_intrpten_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RX_RPRIF_misseop_intrpten_t;

#define ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt     0x0a106
#define ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_t;

#define ARN5_SPI4P2A_RX_RPRIF_misssop_intrpten   0x0a107
#define ARN5_SPI4P2A_RX_RPRIF_misssop_intrpten_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RX_RPRIF_misssop_intrpten_t;

#define ARN5_SPI4P2A_RX_RPRIF_spi4p1_intrpt      0x0a108
#define ARN5_SPI4P2A_RX_RPRIF_spi4p1_intrpt_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 perr7                :  1 ; /* bits 7:7 */
    cs_uint16 perr6                :  1 ; /* bits 6:6 */
    cs_uint16 perr5                :  1 ; /* bits 5:5 */
    cs_uint16 perr4                :  1 ; /* bits 4:4 */
    cs_uint16 perr3                :  1 ; /* bits 3:3 */
    cs_uint16 perr2                :  1 ; /* bits 2:2 */
    cs_uint16 perr1                :  1 ; /* bits 1:1 */
    cs_uint16 perr0                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 perr0                :  1 ; /* bits 0:0 */
    cs_uint16 perr1                :  1 ; /* bits 1:1 */
    cs_uint16 perr2                :  1 ; /* bits 2:2 */
    cs_uint16 perr3                :  1 ; /* bits 3:3 */
    cs_uint16 perr4                :  1 ; /* bits 4:4 */
    cs_uint16 perr5                :  1 ; /* bits 5:5 */
    cs_uint16 perr6                :  1 ; /* bits 6:6 */
    cs_uint16 perr7                :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RX_RPRIF_spi4p1_intrpt_t;

#define ARN5_SPI4P2A_RX_RPRIF_spi4p1_intrpten    0x0a109
#define ARN5_SPI4P2A_RX_RPRIF_spi4p1_intrpten_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 perr7_en             :  1 ; /* bits 7:7 */
    cs_uint16 perr6_en             :  1 ; /* bits 6:6 */
    cs_uint16 perr5_en             :  1 ; /* bits 5:5 */
    cs_uint16 perr4_en             :  1 ; /* bits 4:4 */
    cs_uint16 perr3_en             :  1 ; /* bits 3:3 */
    cs_uint16 perr2_en             :  1 ; /* bits 2:2 */
    cs_uint16 perr1_en             :  1 ; /* bits 1:1 */
    cs_uint16 perr0_en             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 perr0_en             :  1 ; /* bits 0:0 */
    cs_uint16 perr1_en             :  1 ; /* bits 1:1 */
    cs_uint16 perr2_en             :  1 ; /* bits 2:2 */
    cs_uint16 perr3_en             :  1 ; /* bits 3:3 */
    cs_uint16 perr4_en             :  1 ; /* bits 4:4 */
    cs_uint16 perr5_en             :  1 ; /* bits 5:5 */
    cs_uint16 perr6_en             :  1 ; /* bits 6:6 */
    cs_uint16 perr7_en             :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RX_RPRIF_spi4p1_intrpten_t;

#define ARN5_SPI4P2A_RX_RPRIF_intrpt_cntr_cfg0   0x0a10a
#define ARN5_SPI4P2A_RX_RPRIF_intrpt_cntr_cfg0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 reg_sel              :  4 ; /* bits 7:4 */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
    cs_uint16 reg_sel              :  4 ; /* bits 7:4 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RX_RPRIF_intrpt_cntr_cfg0_t;
#define     ARN5_reg_sel_MISSEOP_INTRPT                   0x00
#define     ARN5_reg_sel_MISSSOP_INTRPT                   0x00
#define     ARN5_reg_sel_SPI4P1_INTRPT                    0x00

#define ARN5_SPI4P2A_RX_RPRIF_intrpt_cntr0       0x0a10b
#define ARN5_SPI4P2A_RX_RPRIF_intrpt_cntr0_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RX_RPRIF_intrpt_cntr0_t;

#define ARN5_SPI4P2A_RX_RPRIF_intrpt_cntr_cfg1   0x0a10c
#define ARN5_SPI4P2A_RX_RPRIF_intrpt_cntr_cfg1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 reg_sel              :  4 ; /* bits 7:4 */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
    cs_uint16 reg_sel              :  4 ; /* bits 7:4 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RX_RPRIF_intrpt_cntr_cfg1_t;

#define ARN5_SPI4P2A_RX_RPRIF_intrpt_cntr1       0x0a10d
#define ARN5_SPI4P2A_RX_RPRIF_intrpt_cntr1_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RX_RPRIF_intrpt_cntr1_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * SPI4P2a::txmem
 */

#define ARN5_SPI4P2A_TXMEM_mem_lo_0              0x0a200
#define ARN5_SPI4P2A_TXMEM_mem_lo_0_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXMEM_mem_lo_0_t;

#define ARN5_SPI4P2A_TXMEM_mem_lo_1              0x0a201
#define ARN5_SPI4P2A_TXMEM_mem_lo_1_dft          0x0380
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXMEM_mem_lo_1_t;

#define ARN5_SPI4P2A_TXMEM_mem_lo_2              0x0a202
#define ARN5_SPI4P2A_TXMEM_mem_lo_2_dft          0x0700
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXMEM_mem_lo_2_t;

#define ARN5_SPI4P2A_TXMEM_mem_lo_3              0x0a203
#define ARN5_SPI4P2A_TXMEM_mem_lo_3_dft          0x0a80
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXMEM_mem_lo_3_t;

#define ARN5_SPI4P2A_TXMEM_mem_lo_4              0x0a204
#define ARN5_SPI4P2A_TXMEM_mem_lo_4_dft          0x0e00
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXMEM_mem_lo_4_t;

#define ARN5_SPI4P2A_TXMEM_mem_lo_5              0x0a205
#define ARN5_SPI4P2A_TXMEM_mem_lo_5_dft          0x1180
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXMEM_mem_lo_5_t;

#define ARN5_SPI4P2A_TXMEM_mem_lo_6              0x0a206
#define ARN5_SPI4P2A_TXMEM_mem_lo_6_dft          0x1500
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXMEM_mem_lo_6_t;

#define ARN5_SPI4P2A_TXMEM_mem_lo_7              0x0a207
#define ARN5_SPI4P2A_TXMEM_mem_lo_7_dft          0x1880
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXMEM_mem_lo_7_t;

#define ARN5_SPI4P2A_TXMEM_mem_hi_0              0x0a208
#define ARN5_SPI4P2A_TXMEM_mem_hi_0_dft          0x037f
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXMEM_mem_hi_0_t;

#define ARN5_SPI4P2A_TXMEM_mem_hi_1              0x0a209
#define ARN5_SPI4P2A_TXMEM_mem_hi_1_dft          0x06ff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXMEM_mem_hi_1_t;

#define ARN5_SPI4P2A_TXMEM_mem_hi_2              0x0a20a
#define ARN5_SPI4P2A_TXMEM_mem_hi_2_dft          0x0a7f
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXMEM_mem_hi_2_t;

#define ARN5_SPI4P2A_TXMEM_mem_hi_3              0x0a20b
#define ARN5_SPI4P2A_TXMEM_mem_hi_3_dft          0x0dff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXMEM_mem_hi_3_t;

#define ARN5_SPI4P2A_TXMEM_mem_hi_4              0x0a20c
#define ARN5_SPI4P2A_TXMEM_mem_hi_4_dft          0x117f
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXMEM_mem_hi_4_t;

#define ARN5_SPI4P2A_TXMEM_mem_hi_5              0x0a20d
#define ARN5_SPI4P2A_TXMEM_mem_hi_5_dft          0x14ff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXMEM_mem_hi_5_t;

#define ARN5_SPI4P2A_TXMEM_mem_hi_6              0x0a20e
#define ARN5_SPI4P2A_TXMEM_mem_hi_6_dft          0x187f
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXMEM_mem_hi_6_t;

#define ARN5_SPI4P2A_TXMEM_mem_hi_7              0x0a20f
#define ARN5_SPI4P2A_TXMEM_mem_hi_7_dft          0x1bff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXMEM_mem_hi_7_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * SPI4P2a::txint
 */

#define ARN5_SPI4P2A_TXINT_dip2errctrl           0x0a300
#define ARN5_SPI4P2A_TXINT_dip2errctrl_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 conterr              :  1 ; /* bits 1:1 */
    cs_uint16 snglerr              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 snglerr              :  1 ; /* bits 0:0 */
    cs_uint16 conterr              :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_dip2errctrl_t;

#define ARN5_SPI4P2A_TXINT_fifo_eccerr_ctrl      0x0a301
#define ARN5_SPI4P2A_TXINT_fifo_eccerr_ctrl_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 const_err_gen        :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 txmem_err_gen        :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 txmem_err_gen        :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 const_err_gen        :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_fifo_eccerr_ctrl_t;
#define     ARN5_const_err_gen_SINGLE                     0x00
#define     ARN5_const_err_gen_CONST                      0x01

#define ARN5_SPI4P2A_TXINT_fifo_eccfix_ctrl      0x0a302
#define ARN5_SPI4P2A_TXINT_fifo_eccfix_ctrl_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 const_fix_gen        :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 txmem_fix_gen        :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 txmem_fix_gen        :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 const_fix_gen        :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_fifo_eccfix_ctrl_t;
#define     ARN5_const_fix_gen_SINGLE                     0x00
#define     ARN5_const_fix_gen_CONST                      0x01

#define ARN5_SPI4P2A_TXINT_perr_ctrl             0x0a303
#define ARN5_SPI4P2A_TXINT_perr_ctrl_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 const_perr_gen       :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 pm_perr_gen          :  1 ; /* bits 8:8 */
    cs_uint16 afifo7_perr_gen      :  1 ; /* bits 7:7 */
    cs_uint16 afifo6_perr_gen      :  1 ; /* bits 6:6 */
    cs_uint16 afifo5_perr_gen      :  1 ; /* bits 5:5 */
    cs_uint16 afifo4_perr_gen      :  1 ; /* bits 4:4 */
    cs_uint16 afifo3_perr_gen      :  1 ; /* bits 3:3 */
    cs_uint16 afifo2_perr_gen      :  1 ; /* bits 2:2 */
    cs_uint16 afifo1_perr_gen      :  1 ; /* bits 1:1 */
    cs_uint16 afifo0_perr_gen      :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 afifo0_perr_gen      :  1 ; /* bits 0:0 */
    cs_uint16 afifo1_perr_gen      :  1 ; /* bits 1:1 */
    cs_uint16 afifo2_perr_gen      :  1 ; /* bits 2:2 */
    cs_uint16 afifo3_perr_gen      :  1 ; /* bits 3:3 */
    cs_uint16 afifo4_perr_gen      :  1 ; /* bits 4:4 */
    cs_uint16 afifo5_perr_gen      :  1 ; /* bits 5:5 */
    cs_uint16 afifo6_perr_gen      :  1 ; /* bits 6:6 */
    cs_uint16 afifo7_perr_gen      :  1 ; /* bits 7:7 */
    cs_uint16 pm_perr_gen          :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 const_perr_gen       :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_perr_ctrl_t;
#define     ARN5_const_perr_gen_SINGLE                    0x00
#define     ARN5_const_perr_gen_CONST                     0x01

#define ARN5_SPI4P2A_TXINT_intrpt_vec            0x0a304
#define ARN5_SPI4P2A_TXINT_intrpt_vec_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 sop_eop_err_intrpt0  :  1 ; /* bits 12:12 */
    cs_uint16 max_pkt_err_intrpt   :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd1               :  4 ;
    cs_uint16 perr_intrpt          :  1 ; /* bits 6:6 */
    cs_uint16 fifo_eccfix_intrpt   :  1 ; /* bits 5:5 */
    cs_uint16 fifo_eccerr_intrpt   :  1 ; /* bits 4:4 */
    cs_uint16 fifo_drop_pkt_intrpt :  1 ; /* bits 3:3 */
    cs_uint16 fifo_abort_pkt_intrpt :  1 ; /* bits 2:2 */
    cs_uint16 fifo_ovrflw_intrpt   :  1 ; /* bits 1:1 */
    cs_uint16 intrpt               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 intrpt               :  1 ; /* bits 0:0 */
    cs_uint16 fifo_ovrflw_intrpt   :  1 ; /* bits 1:1 */
    cs_uint16 fifo_abort_pkt_intrpt :  1 ; /* bits 2:2 */
    cs_uint16 fifo_drop_pkt_intrpt :  1 ; /* bits 3:3 */
    cs_uint16 fifo_eccerr_intrpt   :  1 ; /* bits 4:4 */
    cs_uint16 fifo_eccfix_intrpt   :  1 ; /* bits 5:5 */
    cs_uint16 perr_intrpt          :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd1               :  4 ;
    cs_uint16 max_pkt_err_intrpt   :  1 ; /* bits 11:11 */
    cs_uint16 sop_eop_err_intrpt0  :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd2               :  3 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_intrpt_vec_t;

#define ARN5_SPI4P2A_TXINT_intrpt_vec_en         0x0a305
#define ARN5_SPI4P2A_TXINT_intrpt_vec_en_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 int_en12             :  1 ; /* bits 12:12 */
    cs_uint16 int_en11             :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 int_en7              :  1 ; /* bits 7:7 */
    cs_uint16 int_en6              :  1 ; /* bits 6:6 */
    cs_uint16 int_en5              :  1 ; /* bits 5:5 */
    cs_uint16 int_en4              :  1 ; /* bits 4:4 */
    cs_uint16 int_en3              :  1 ; /* bits 3:3 */
    cs_uint16 int_en2              :  1 ; /* bits 2:2 */
    cs_uint16 int_en1              :  1 ; /* bits 1:1 */
    cs_uint16 int_en0              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 int_en0              :  1 ; /* bits 0:0 */
    cs_uint16 int_en1              :  1 ; /* bits 1:1 */
    cs_uint16 int_en2              :  1 ; /* bits 2:2 */
    cs_uint16 int_en3              :  1 ; /* bits 3:3 */
    cs_uint16 int_en4              :  1 ; /* bits 4:4 */
    cs_uint16 int_en5              :  1 ; /* bits 5:5 */
    cs_uint16 int_en6              :  1 ; /* bits 6:6 */
    cs_uint16 int_en7              :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 int_en11             :  1 ; /* bits 11:11 */
    cs_uint16 int_en12             :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd2               :  3 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_intrpt_vec_en_t;

#define ARN5_SPI4P2A_TXINT_intrpt                0x0a306
#define ARN5_SPI4P2A_TXINT_intrpt_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 q3_fifo_ovf          :  1 ; /* bits 2:2 */
    cs_uint16 q3_fifo_und          :  1 ; /* bits 1:1 */
    cs_uint16 addrerr              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addrerr              :  1 ; /* bits 0:0 */
    cs_uint16 q3_fifo_und          :  1 ; /* bits 1:1 */
    cs_uint16 q3_fifo_ovf          :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_intrpt_t;

#define ARN5_SPI4P2A_TXINT_intrpten              0x0a307
#define ARN5_SPI4P2A_TXINT_intrpten_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 q3_fifo_ovf_en       :  1 ; /* bits 2:2 */
    cs_uint16 q3_fifo_und_en       :  1 ; /* bits 1:1 */
    cs_uint16 addrerr_en           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 addrerr_en           :  1 ; /* bits 0:0 */
    cs_uint16 q3_fifo_und_en       :  1 ; /* bits 1:1 */
    cs_uint16 q3_fifo_ovf_en       :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_intrpten_t;

#define ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt    0x0a308
#define ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_t;

#define ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpten  0x0a309
#define ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpten_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpten_t;

#define ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt 0x0a30a
#define ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_t;

#define ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpten 0x0a30b
#define ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpten_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpten_t;

#define ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt  0x0a30c
#define ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_t;

#define ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpten 0x0a30d
#define ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpten_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpten_t;

#define ARN5_SPI4P2A_TXINT_fifo_eccerr_intrpt    0x0a30e
#define ARN5_SPI4P2A_TXINT_fifo_eccerr_intrpt_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 txbuf_err            :  1 ; /* bits 1:1 */
    cs_uint16 txmem_err            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 txmem_err            :  1 ; /* bits 0:0 */
    cs_uint16 txbuf_err            :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_fifo_eccerr_intrpt_t;

#define ARN5_SPI4P2A_TXINT_fifo_eccerr_intrpten  0x0a30f
#define ARN5_SPI4P2A_TXINT_fifo_eccerr_intrpten_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 txbuf_err_en         :  1 ; /* bits 1:1 */
    cs_uint16 txmem_err_en         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 txmem_err_en         :  1 ; /* bits 0:0 */
    cs_uint16 txbuf_err_en         :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_fifo_eccerr_intrpten_t;

#define ARN5_SPI4P2A_TXINT_fifo_eccfix_intrpt    0x0a310
#define ARN5_SPI4P2A_TXINT_fifo_eccfix_intrpt_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 txbuf_fix            :  1 ; /* bits 1:1 */
    cs_uint16 txmem_fix            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 txmem_fix            :  1 ; /* bits 0:0 */
    cs_uint16 txbuf_fix            :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_fifo_eccfix_intrpt_t;

#define ARN5_SPI4P2A_TXINT_fifo_eccfix_intrpten  0x0a311
#define ARN5_SPI4P2A_TXINT_fifo_eccfix_intrpten_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 txbuf_fix_en         :  1 ; /* bits 1:1 */
    cs_uint16 txmem_fix_en         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 txmem_fix_en         :  1 ; /* bits 0:0 */
    cs_uint16 txbuf_fix_en         :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_fifo_eccfix_intrpten_t;

#define ARN5_SPI4P2A_TXINT_perr_intrpt           0x0a312
#define ARN5_SPI4P2A_TXINT_perr_intrpt_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  7 ;
    cs_uint16 pm_perr              :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  8 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 pm_perr              :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_perr_intrpt_t;

#define ARN5_SPI4P2A_TXINT_perr_intrpten         0x0a313
#define ARN5_SPI4P2A_TXINT_perr_intrpten_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  7 ;
    cs_uint16 pm_perr_en           :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  8 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 pm_perr_en           :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_perr_intrpten_t;

#define ARN5_SPI4P2A_TXINT_max_pkt_err_intrpt    0x0a314
#define ARN5_SPI4P2A_TXINT_max_pkt_err_intrpt_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_max_pkt_err_intrpt_t;

#define ARN5_SPI4P2A_TXINT_max_pkt_err_intrpten  0x0a315
#define ARN5_SPI4P2A_TXINT_max_pkt_err_intrpten_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_max_pkt_err_intrpten_t;

#define ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt    0x0a316
#define ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt_t;

#define ARN5_SPI4P2A_TXINT_sop_eop_err_intrpten  0x0a317
#define ARN5_SPI4P2A_TXINT_sop_eop_err_intrpten_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 vec                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_sop_eop_err_intrpten_t;

#define ARN5_SPI4P2A_TXINT_intrpt_cntr_cfg0      0x0a318
#define ARN5_SPI4P2A_TXINT_intrpt_cntr_cfg0_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 reg_sel              :  4 ; /* bits 7:4 */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
    cs_uint16 reg_sel              :  4 ; /* bits 7:4 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_intrpt_cntr_cfg0_t;
#define     ARN5_reg_sel_INTRPT                           0x00
#define     ARN5_reg_sel_FIFO_OVRFLW_INTRPT               0x00
#define     ARN5_reg_sel_FIFO_ABORT_PKT_INTRPT            0x00
#define     ARN5_reg_sel_FIFO_DROP_PKT_INTRPT             0x00
#define     ARN5_reg_sel_FIFO_ECCERR_INTRPT               0x00
#define     ARN5_reg_sel_FIFO_ECCFIX_INTRPT               0x00
#define     ARN5_reg_sel_PERR_INTRPT                      0x00
#define     ARN5_reg_sel_MAX_PKT_ERR_INTRPT               0x00
#define     ARN5_reg_sel_SOP_EOP_ERR_INTRPT0              0x00

#define ARN5_SPI4P2A_TXINT_intrpt_cntr0          0x0a319
#define ARN5_SPI4P2A_TXINT_intrpt_cntr0_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_intrpt_cntr0_t;

#define ARN5_SPI4P2A_TXINT_intrpt_cntr_cfg1      0x0a31a
#define ARN5_SPI4P2A_TXINT_intrpt_cntr_cfg1_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 reg_sel              :  4 ; /* bits 7:4 */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
    cs_uint16 reg_sel              :  4 ; /* bits 7:4 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_intrpt_cntr_cfg1_t;

#define ARN5_SPI4P2A_TXINT_intrpt_cntr1          0x0a31b
#define ARN5_SPI4P2A_TXINT_intrpt_cntr1_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_intrpt_cntr1_t;

#define ARN5_SPI4P2A_TXINT_intrpt_cntr_cfg2      0x0a31c
#define ARN5_SPI4P2A_TXINT_intrpt_cntr_cfg2_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 reg_sel              :  4 ; /* bits 7:4 */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
    cs_uint16 reg_sel              :  4 ; /* bits 7:4 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_intrpt_cntr_cfg2_t;

#define ARN5_SPI4P2A_TXINT_intrpt_cntr2          0x0a31d
#define ARN5_SPI4P2A_TXINT_intrpt_cntr2_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_intrpt_cntr2_t;

#define ARN5_SPI4P2A_TXINT_intrpt_cntr_cfg3      0x0a31e
#define ARN5_SPI4P2A_TXINT_intrpt_cntr_cfg3_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 reg_sel              :  4 ; /* bits 7:4 */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
    cs_uint16 reg_sel              :  4 ; /* bits 7:4 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_intrpt_cntr_cfg3_t;

#define ARN5_SPI4P2A_TXINT_intrpt_cntr3          0x0a31f
#define ARN5_SPI4P2A_TXINT_intrpt_cntr3_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXINT_intrpt_cntr3_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * SPI4P2a::txch[0..7]
 */

#define ARN5_SPI4P2A_TXCH_STRIDE                 16
#define ARN5_SPI4P2A_TXCH_COUNT                  8
#define ARN5_SPI4P2A_TXCH_almost_full            0x0a400
#define ARN5_SPI4P2A_TXCH_almost_full_dft        0x01c0
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 threshold            : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXCH_almost_full_t;

#define ARN5_SPI4P2A_TXCH_almost_empty           0x0a401
#define ARN5_SPI4P2A_TXCH_almost_empty_dft       0x00e0
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 threshold            : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 threshold            : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXCH_almost_empty_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * SPI4P2a::rxint
 */

#define ARN5_SPI4P2A_RXINT_fifo_eccerr_ctrl      0x0a500
#define ARN5_SPI4P2A_RXINT_fifo_eccerr_ctrl_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 const_err_gen        :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 pf7_err_gen          :  1 ; /* bits 7:7 */
    cs_uint16 pf6_err_gen          :  1 ; /* bits 6:6 */
    cs_uint16 pf5_err_gen          :  1 ; /* bits 5:5 */
    cs_uint16 pf4_err_gen          :  1 ; /* bits 4:4 */
    cs_uint16 pf3_err_gen          :  1 ; /* bits 3:3 */
    cs_uint16 pf2_err_gen          :  1 ; /* bits 2:2 */
    cs_uint16 pf1_err_gen          :  1 ; /* bits 1:1 */
    cs_uint16 pf0_err_gen          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pf0_err_gen          :  1 ; /* bits 0:0 */
    cs_uint16 pf1_err_gen          :  1 ; /* bits 1:1 */
    cs_uint16 pf2_err_gen          :  1 ; /* bits 2:2 */
    cs_uint16 pf3_err_gen          :  1 ; /* bits 3:3 */
    cs_uint16 pf4_err_gen          :  1 ; /* bits 4:4 */
    cs_uint16 pf5_err_gen          :  1 ; /* bits 5:5 */
    cs_uint16 pf6_err_gen          :  1 ; /* bits 6:6 */
    cs_uint16 pf7_err_gen          :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 const_err_gen        :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_fifo_eccerr_ctrl_t;

#define ARN5_SPI4P2A_RXINT_fifo_eccfix_ctrl      0x0a501
#define ARN5_SPI4P2A_RXINT_fifo_eccfix_ctrl_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 const_fix_gen        :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 pf7_fix_gen          :  1 ; /* bits 7:7 */
    cs_uint16 pf6_fix_gen          :  1 ; /* bits 6:6 */
    cs_uint16 pf5_fix_gen          :  1 ; /* bits 5:5 */
    cs_uint16 pf4_fix_gen          :  1 ; /* bits 4:4 */
    cs_uint16 pf3_fix_gen          :  1 ; /* bits 3:3 */
    cs_uint16 pf2_fix_gen          :  1 ; /* bits 2:2 */
    cs_uint16 pf1_fix_gen          :  1 ; /* bits 1:1 */
    cs_uint16 pf0_fix_gen          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pf0_fix_gen          :  1 ; /* bits 0:0 */
    cs_uint16 pf1_fix_gen          :  1 ; /* bits 1:1 */
    cs_uint16 pf2_fix_gen          :  1 ; /* bits 2:2 */
    cs_uint16 pf3_fix_gen          :  1 ; /* bits 3:3 */
    cs_uint16 pf4_fix_gen          :  1 ; /* bits 4:4 */
    cs_uint16 pf5_fix_gen          :  1 ; /* bits 5:5 */
    cs_uint16 pf6_fix_gen          :  1 ; /* bits 6:6 */
    cs_uint16 pf7_fix_gen          :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 const_fix_gen        :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_fifo_eccfix_ctrl_t;

#define ARN5_SPI4P2A_RXINT_perr_ctrl             0x0a502
#define ARN5_SPI4P2A_RXINT_perr_ctrl_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 const_perr_gen       :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd2               :  5 ;
    cs_uint16 sp_perr_gen          :  1 ; /* bits 9:9 */
    cs_uint16 pm_perr_gen          :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  4 ;
    cs_uint16 bb3_perr_gen         :  1 ; /* bits 3:3 */
    cs_uint16 bb2_perr_gen         :  1 ; /* bits 2:2 */
    cs_uint16 bb1_perr_gen         :  1 ; /* bits 1:1 */
    cs_uint16 bb0_perr_gen         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bb0_perr_gen         :  1 ; /* bits 0:0 */
    cs_uint16 bb1_perr_gen         :  1 ; /* bits 1:1 */
    cs_uint16 bb2_perr_gen         :  1 ; /* bits 2:2 */
    cs_uint16 bb3_perr_gen         :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               :  4 ;
    cs_uint16 pm_perr_gen          :  1 ; /* bits 8:8 */
    cs_uint16 sp_perr_gen          :  1 ; /* bits 9:9 */
    cs_uint16 rsrvd2               :  5 ;
    cs_uint16 const_perr_gen       :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_perr_ctrl_t;

#define ARN5_SPI4P2A_RXINT_rxintrpt              0x0a503
#define ARN5_SPI4P2A_RXINT_rxintrpt_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 dip2err              :  1 ; /* bits 1:1 */
    cs_uint16 rxlosyn              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rxlosyn              :  1 ; /* bits 0:0 */
    cs_uint16 dip2err              :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_rxintrpt_t;

#define ARN5_SPI4P2A_RXINT_rxintrpten            0x0a504
#define ARN5_SPI4P2A_RXINT_rxintrpten_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 dip2err_en           :  1 ; /* bits 1:1 */
    cs_uint16 rxlosyn_en           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rxlosyn_en           :  1 ; /* bits 0:0 */
    cs_uint16 dip2err_en           :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_rxintrpten_t;

#define ARN5_SPI4P2A_RXINT_rxloscnt              0x0a505
#define ARN5_SPI4P2A_RXINT_rxloscnt_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_rxloscnt_t;

#define ARN5_SPI4P2A_RXINT_dip2errcnt            0x0a506
#define ARN5_SPI4P2A_RXINT_dip2errcnt_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_dip2errcnt_t;

#define ARN5_SPI4P2A_RXINT_intrpt_vec            0x0a507
#define ARN5_SPI4P2A_RXINT_intrpt_vec_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 perr_intrpt          :  1 ; /* bits 3:3 */
    cs_uint16 fifo_eccfix_intrpt   :  1 ; /* bits 2:2 */
    cs_uint16 fifo_eccerr_intrpt   :  1 ; /* bits 1:1 */
    cs_uint16 fifo_ovrflw_intrpt   :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 fifo_ovrflw_intrpt   :  1 ; /* bits 0:0 */
    cs_uint16 fifo_eccerr_intrpt   :  1 ; /* bits 1:1 */
    cs_uint16 fifo_eccfix_intrpt   :  1 ; /* bits 2:2 */
    cs_uint16 perr_intrpt          :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_intrpt_vec_t;

#define ARN5_SPI4P2A_RXINT_intrpt_vec_en         0x0a508
#define ARN5_SPI4P2A_RXINT_intrpt_vec_en_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 int_en3              :  1 ; /* bits 3:3 */
    cs_uint16 int_en2              :  1 ; /* bits 2:2 */
    cs_uint16 int_en1              :  1 ; /* bits 1:1 */
    cs_uint16 int_en0              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 int_en0              :  1 ; /* bits 0:0 */
    cs_uint16 int_en1              :  1 ; /* bits 1:1 */
    cs_uint16 int_en2              :  1 ; /* bits 2:2 */
    cs_uint16 int_en3              :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_intrpt_vec_en_t;

#define ARN5_SPI4P2A_RXINT_fifo_ovrflw_intrpt    0x0a509
#define ARN5_SPI4P2A_RXINT_fifo_ovrflw_intrpt_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 pf7_overrun          :  1 ; /* bits 7:7 */
    cs_uint16 pf6_overrun          :  1 ; /* bits 6:6 */
    cs_uint16 pf5_overrun          :  1 ; /* bits 5:5 */
    cs_uint16 pf4_overrun          :  1 ; /* bits 4:4 */
    cs_uint16 pf3_overrun          :  1 ; /* bits 3:3 */
    cs_uint16 pf2_overrun          :  1 ; /* bits 2:2 */
    cs_uint16 pf1_overrun          :  1 ; /* bits 1:1 */
    cs_uint16 pf0_overrun          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pf0_overrun          :  1 ; /* bits 0:0 */
    cs_uint16 pf1_overrun          :  1 ; /* bits 1:1 */
    cs_uint16 pf2_overrun          :  1 ; /* bits 2:2 */
    cs_uint16 pf3_overrun          :  1 ; /* bits 3:3 */
    cs_uint16 pf4_overrun          :  1 ; /* bits 4:4 */
    cs_uint16 pf5_overrun          :  1 ; /* bits 5:5 */
    cs_uint16 pf6_overrun          :  1 ; /* bits 6:6 */
    cs_uint16 pf7_overrun          :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_fifo_ovrflw_intrpt_t;

#define ARN5_SPI4P2A_RXINT_fifo_ovrflw_intrpten  0x0a50a
#define ARN5_SPI4P2A_RXINT_fifo_ovrflw_intrpten_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 pf7_overrun_en       :  1 ; /* bits 7:7 */
    cs_uint16 pf6_overrun_en       :  1 ; /* bits 6:6 */
    cs_uint16 pf5_overrun_en       :  1 ; /* bits 5:5 */
    cs_uint16 pf4_overrun_en       :  1 ; /* bits 4:4 */
    cs_uint16 pf3_overrun_en       :  1 ; /* bits 3:3 */
    cs_uint16 pf2_overrun_en       :  1 ; /* bits 2:2 */
    cs_uint16 pf1_overrun_en       :  1 ; /* bits 1:1 */
    cs_uint16 pf0_overrun_en       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pf0_overrun_en       :  1 ; /* bits 0:0 */
    cs_uint16 pf1_overrun_en       :  1 ; /* bits 1:1 */
    cs_uint16 pf2_overrun_en       :  1 ; /* bits 2:2 */
    cs_uint16 pf3_overrun_en       :  1 ; /* bits 3:3 */
    cs_uint16 pf4_overrun_en       :  1 ; /* bits 4:4 */
    cs_uint16 pf5_overrun_en       :  1 ; /* bits 5:5 */
    cs_uint16 pf6_overrun_en       :  1 ; /* bits 6:6 */
    cs_uint16 pf7_overrun_en       :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_fifo_ovrflw_intrpten_t;

#define ARN5_SPI4P2A_RXINT_fifo_eccerr_intrpt    0x0a50b
#define ARN5_SPI4P2A_RXINT_fifo_eccerr_intrpt_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 pf7_err              :  1 ; /* bits 7:7 */
    cs_uint16 pf6_err              :  1 ; /* bits 6:6 */
    cs_uint16 pf5_err              :  1 ; /* bits 5:5 */
    cs_uint16 pf4_err              :  1 ; /* bits 4:4 */
    cs_uint16 pf3_err              :  1 ; /* bits 3:3 */
    cs_uint16 pf2_err              :  1 ; /* bits 2:2 */
    cs_uint16 pf1_err              :  1 ; /* bits 1:1 */
    cs_uint16 pf0_err              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pf0_err              :  1 ; /* bits 0:0 */
    cs_uint16 pf1_err              :  1 ; /* bits 1:1 */
    cs_uint16 pf2_err              :  1 ; /* bits 2:2 */
    cs_uint16 pf3_err              :  1 ; /* bits 3:3 */
    cs_uint16 pf4_err              :  1 ; /* bits 4:4 */
    cs_uint16 pf5_err              :  1 ; /* bits 5:5 */
    cs_uint16 pf6_err              :  1 ; /* bits 6:6 */
    cs_uint16 pf7_err              :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_fifo_eccerr_intrpt_t;

#define ARN5_SPI4P2A_RXINT_fifo_eccerr_intrpten  0x0a50c
#define ARN5_SPI4P2A_RXINT_fifo_eccerr_intrpten_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 pf7_err_en           :  1 ; /* bits 7:7 */
    cs_uint16 pf6_err_en           :  1 ; /* bits 6:6 */
    cs_uint16 pf5_err_en           :  1 ; /* bits 5:5 */
    cs_uint16 pf4_err_en           :  1 ; /* bits 4:4 */
    cs_uint16 pf3_err_en           :  1 ; /* bits 3:3 */
    cs_uint16 pf2_err_en           :  1 ; /* bits 2:2 */
    cs_uint16 pf1_err_en           :  1 ; /* bits 1:1 */
    cs_uint16 pf0_err_en           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pf0_err_en           :  1 ; /* bits 0:0 */
    cs_uint16 pf1_err_en           :  1 ; /* bits 1:1 */
    cs_uint16 pf2_err_en           :  1 ; /* bits 2:2 */
    cs_uint16 pf3_err_en           :  1 ; /* bits 3:3 */
    cs_uint16 pf4_err_en           :  1 ; /* bits 4:4 */
    cs_uint16 pf5_err_en           :  1 ; /* bits 5:5 */
    cs_uint16 pf6_err_en           :  1 ; /* bits 6:6 */
    cs_uint16 pf7_err_en           :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_fifo_eccerr_intrpten_t;

#define ARN5_SPI4P2A_RXINT_fifo_eccfix_intrpt    0x0a50d
#define ARN5_SPI4P2A_RXINT_fifo_eccfix_intrpt_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 pf7_fix              :  1 ; /* bits 7:7 */
    cs_uint16 pf6_fix              :  1 ; /* bits 6:6 */
    cs_uint16 pf5_fix              :  1 ; /* bits 5:5 */
    cs_uint16 pf4_fix              :  1 ; /* bits 4:4 */
    cs_uint16 pf3_fix              :  1 ; /* bits 3:3 */
    cs_uint16 pf2_fix              :  1 ; /* bits 2:2 */
    cs_uint16 pf1_fix              :  1 ; /* bits 1:1 */
    cs_uint16 pf0_fix              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pf0_fix              :  1 ; /* bits 0:0 */
    cs_uint16 pf1_fix              :  1 ; /* bits 1:1 */
    cs_uint16 pf2_fix              :  1 ; /* bits 2:2 */
    cs_uint16 pf3_fix              :  1 ; /* bits 3:3 */
    cs_uint16 pf4_fix              :  1 ; /* bits 4:4 */
    cs_uint16 pf5_fix              :  1 ; /* bits 5:5 */
    cs_uint16 pf6_fix              :  1 ; /* bits 6:6 */
    cs_uint16 pf7_fix              :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_fifo_eccfix_intrpt_t;

#define ARN5_SPI4P2A_RXINT_fifo_eccfix_intrpten  0x0a50e
#define ARN5_SPI4P2A_RXINT_fifo_eccfix_intrpten_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 pf7_fix_en           :  1 ; /* bits 7:7 */
    cs_uint16 pf6_fix_en           :  1 ; /* bits 6:6 */
    cs_uint16 pf5_fix_en           :  1 ; /* bits 5:5 */
    cs_uint16 pf4_fix_en           :  1 ; /* bits 4:4 */
    cs_uint16 pf3_fix_en           :  1 ; /* bits 3:3 */
    cs_uint16 pf2_fix_en           :  1 ; /* bits 2:2 */
    cs_uint16 pf1_fix_en           :  1 ; /* bits 1:1 */
    cs_uint16 pf0_fix_en           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 pf0_fix_en           :  1 ; /* bits 0:0 */
    cs_uint16 pf1_fix_en           :  1 ; /* bits 1:1 */
    cs_uint16 pf2_fix_en           :  1 ; /* bits 2:2 */
    cs_uint16 pf3_fix_en           :  1 ; /* bits 3:3 */
    cs_uint16 pf4_fix_en           :  1 ; /* bits 4:4 */
    cs_uint16 pf5_fix_en           :  1 ; /* bits 5:5 */
    cs_uint16 pf6_fix_en           :  1 ; /* bits 6:6 */
    cs_uint16 pf7_fix_en           :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_fifo_eccfix_intrpten_t;

#define ARN5_SPI4P2A_RXINT_perr_intrpt           0x0a50f
#define ARN5_SPI4P2A_RXINT_perr_intrpt_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  7 ;
    cs_uint16 pm_perr              :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  4 ;
    cs_uint16 bb3_perr             :  1 ; /* bits 3:3 */
    cs_uint16 bb2_perr             :  1 ; /* bits 2:2 */
    cs_uint16 bb1_perr             :  1 ; /* bits 1:1 */
    cs_uint16 bb0_perr             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bb0_perr             :  1 ; /* bits 0:0 */
    cs_uint16 bb1_perr             :  1 ; /* bits 1:1 */
    cs_uint16 bb2_perr             :  1 ; /* bits 2:2 */
    cs_uint16 bb3_perr             :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               :  4 ;
    cs_uint16 pm_perr              :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_perr_intrpt_t;

#define ARN5_SPI4P2A_RXINT_perr_intrpten         0x0a510
#define ARN5_SPI4P2A_RXINT_perr_intrpten_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  7 ;
    cs_uint16 pm_perr_en           :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  4 ;
    cs_uint16 bb3_perr_en          :  1 ; /* bits 3:3 */
    cs_uint16 bb2_perr_en          :  1 ; /* bits 2:2 */
    cs_uint16 bb1_perr_en          :  1 ; /* bits 1:1 */
    cs_uint16 bb0_perr_en          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bb0_perr_en          :  1 ; /* bits 0:0 */
    cs_uint16 bb1_perr_en          :  1 ; /* bits 1:1 */
    cs_uint16 bb2_perr_en          :  1 ; /* bits 2:2 */
    cs_uint16 bb3_perr_en          :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               :  4 ;
    cs_uint16 pm_perr_en           :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_perr_intrpten_t;

#define ARN5_SPI4P2A_RXINT_intrpt_cntr_cfg0      0x0a511
#define ARN5_SPI4P2A_RXINT_intrpt_cntr_cfg0_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 reg_sel              :  5 ; /* bits 8:4 */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
    cs_uint16 reg_sel              :  5 ; /* bits 8:4 */
    cs_uint16 rsrvd1               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_intrpt_cntr_cfg0_t;
#define     ARN5_reg_sel_FIFO_OVRFLW_INTRRPT              0x00
#define     ARN5_reg_sel_FIFO_ECCERR_INTRRPT              0x00
#define     ARN5_reg_sel_FIFO_ECCFIX_INTRRPT              0x00
#define     ARN5_reg_sel_PERR_INTRRPT                     0x00

#define ARN5_SPI4P2A_RXINT_intrpt_cntr0          0x0a512
#define ARN5_SPI4P2A_RXINT_intrpt_cntr0_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_intrpt_cntr0_t;

#define ARN5_SPI4P2A_RXINT_intrpt_cntr_cfg1      0x0a513
#define ARN5_SPI4P2A_RXINT_intrpt_cntr_cfg1_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 reg_sel              :  5 ; /* bits 8:4 */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
    cs_uint16 reg_sel              :  5 ; /* bits 8:4 */
    cs_uint16 rsrvd1               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_intrpt_cntr_cfg1_t;

#define ARN5_SPI4P2A_RXINT_intrpt_cntr1          0x0a514
#define ARN5_SPI4P2A_RXINT_intrpt_cntr1_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_intrpt_cntr1_t;

#define ARN5_SPI4P2A_RXINT_intrpt_cntr_cfg2      0x0a515
#define ARN5_SPI4P2A_RXINT_intrpt_cntr_cfg2_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 reg_sel              :  5 ; /* bits 8:4 */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
    cs_uint16 reg_sel              :  5 ; /* bits 8:4 */
    cs_uint16 rsrvd1               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_intrpt_cntr_cfg2_t;

#define ARN5_SPI4P2A_RXINT_intrpt_cntr2          0x0a516
#define ARN5_SPI4P2A_RXINT_intrpt_cntr2_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_intrpt_cntr2_t;

#define ARN5_SPI4P2A_RXINT_intrpt_cntr_cfg3      0x0a517
#define ARN5_SPI4P2A_RXINT_intrpt_cntr_cfg3_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 reg_sel              :  5 ; /* bits 8:4 */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bit_sel              :  4 ; /* bits 3:0 */
    cs_uint16 reg_sel              :  5 ; /* bits 8:4 */
    cs_uint16 rsrvd1               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_intrpt_cntr_cfg3_t;

#define ARN5_SPI4P2A_RXINT_intrpt_cntr3          0x0a518
#define ARN5_SPI4P2A_RXINT_intrpt_cntr3_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_RXINT_intrpt_cntr3_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * SPI4P2a::membist
 */

#define ARN5_SPI4P2A_MEMBIST_mb_done             0x0aa00
#define ARN5_SPI4P2A_MEMBIST_mb_done_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 bistdone             : 15 ; /* bits 14:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bistdone             : 15 ; /* bits 14:0 */
    cs_uint16 rsrvd1               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_MEMBIST_mb_done_t;

#define ARN5_SPI4P2A_MEMBIST_mb_fail             0x0aa01
#define ARN5_SPI4P2A_MEMBIST_mb_fail_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 bistfail             : 15 ; /* bits 14:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bistfail             : 15 ; /* bits 14:0 */
    cs_uint16 rsrvd1               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_MEMBIST_mb_fail_t;

#define ARN5_SPI4P2A_MEMBIST_sb_strready         0x0aa02
#define ARN5_SPI4P2A_MEMBIST_sb_strready_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 strready             : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 strready             : 10 ; /* bits 9:0 */
    cs_uint16 rsrvd1               :  6 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_MEMBIST_sb_strready_t;

#define ARN5_SPI4P2A_MEMBIST_sb_birafail         0x0aa03
#define ARN5_SPI4P2A_MEMBIST_sb_birafail_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 birafail             : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 birafail             : 10 ; /* bits 9:0 */
    cs_uint16 rsrvd1               :  6 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_MEMBIST_sb_birafail_t;

#define ARN5_SPI4P2A_MEMBIST_sb_bisterror        0x0aa04
#define ARN5_SPI4P2A_MEMBIST_sb_bisterror_dft    0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 bisterror            : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 bisterror            : 10 ; /* bits 9:0 */
    cs_uint16 rsrvd1               :  6 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_MEMBIST_sb_bisterror_t;

#define ARN5_SPI4P2A_MEMBIST_intrpt              0x0aa05
#define ARN5_SPI4P2A_MEMBIST_intrpt_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 sb_bist_int          :  1 ; /* bits 1:1 */
    cs_uint16 mb_bist_int          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 mb_bist_int          :  1 ; /* bits 0:0 */
    cs_uint16 sb_bist_int          :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_MEMBIST_intrpt_t;

#define ARN5_SPI4P2A_MEMBIST_intrpten            0x0aa06
#define ARN5_SPI4P2A_MEMBIST_intrpten_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 sb_bist_int_en       :  1 ; /* bits 1:1 */
    cs_uint16 mb_bist_int_en       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 mb_bist_int_en       :  1 ; /* bits 0:0 */
    cs_uint16 sb_bist_int_en       :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_MEMBIST_intrpten_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * SPI4P2a::txcfg
 */

#define ARN5_SPI4P2A_TXCFG_phy_ch0               0x0ac00
#define ARN5_SPI4P2A_TXCFG_phy_ch0_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 12 ;
    cs_uint16 gfp_pli              :  1 ; /* bits 3:3 */
    cs_uint16 store_fwd            :  1 ; /* bits 2:2 */
    cs_uint16 drop                 :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 drop                 :  1 ; /* bits 1:1 */
    cs_uint16 store_fwd            :  1 ; /* bits 2:2 */
    cs_uint16 gfp_pli              :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd2               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXCFG_phy_ch0_t;

#define ARN5_SPI4P2A_TXCFG_phy_ch1               0x0ac01
#define ARN5_SPI4P2A_TXCFG_phy_ch1_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 12 ;
    cs_uint16 gfp_pli              :  1 ; /* bits 3:3 */
    cs_uint16 store_fwd            :  1 ; /* bits 2:2 */
    cs_uint16 drop                 :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 drop                 :  1 ; /* bits 1:1 */
    cs_uint16 store_fwd            :  1 ; /* bits 2:2 */
    cs_uint16 gfp_pli              :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd2               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXCFG_phy_ch1_t;

#define ARN5_SPI4P2A_TXCFG_phy_ch2               0x0ac02
#define ARN5_SPI4P2A_TXCFG_phy_ch2_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 12 ;
    cs_uint16 gfp_pli              :  1 ; /* bits 3:3 */
    cs_uint16 store_fwd            :  1 ; /* bits 2:2 */
    cs_uint16 drop                 :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 drop                 :  1 ; /* bits 1:1 */
    cs_uint16 store_fwd            :  1 ; /* bits 2:2 */
    cs_uint16 gfp_pli              :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd2               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXCFG_phy_ch2_t;

#define ARN5_SPI4P2A_TXCFG_phy_ch3               0x0ac03
#define ARN5_SPI4P2A_TXCFG_phy_ch3_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 12 ;
    cs_uint16 gfp_pli              :  1 ; /* bits 3:3 */
    cs_uint16 store_fwd            :  1 ; /* bits 2:2 */
    cs_uint16 drop                 :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 drop                 :  1 ; /* bits 1:1 */
    cs_uint16 store_fwd            :  1 ; /* bits 2:2 */
    cs_uint16 gfp_pli              :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd2               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXCFG_phy_ch3_t;

#define ARN5_SPI4P2A_TXCFG_phy_ch4               0x0ac04
#define ARN5_SPI4P2A_TXCFG_phy_ch4_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 12 ;
    cs_uint16 gfp_pli              :  1 ; /* bits 3:3 */
    cs_uint16 store_fwd            :  1 ; /* bits 2:2 */
    cs_uint16 drop                 :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 drop                 :  1 ; /* bits 1:1 */
    cs_uint16 store_fwd            :  1 ; /* bits 2:2 */
    cs_uint16 gfp_pli              :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd2               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXCFG_phy_ch4_t;

#define ARN5_SPI4P2A_TXCFG_phy_ch5               0x0ac05
#define ARN5_SPI4P2A_TXCFG_phy_ch5_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 12 ;
    cs_uint16 gfp_pli              :  1 ; /* bits 3:3 */
    cs_uint16 store_fwd            :  1 ; /* bits 2:2 */
    cs_uint16 drop                 :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 drop                 :  1 ; /* bits 1:1 */
    cs_uint16 store_fwd            :  1 ; /* bits 2:2 */
    cs_uint16 gfp_pli              :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd2               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXCFG_phy_ch5_t;

#define ARN5_SPI4P2A_TXCFG_phy_ch6               0x0ac06
#define ARN5_SPI4P2A_TXCFG_phy_ch6_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 12 ;
    cs_uint16 gfp_pli              :  1 ; /* bits 3:3 */
    cs_uint16 store_fwd            :  1 ; /* bits 2:2 */
    cs_uint16 drop                 :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 drop                 :  1 ; /* bits 1:1 */
    cs_uint16 store_fwd            :  1 ; /* bits 2:2 */
    cs_uint16 gfp_pli              :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd2               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXCFG_phy_ch6_t;

#define ARN5_SPI4P2A_TXCFG_phy_ch7               0x0ac07
#define ARN5_SPI4P2A_TXCFG_phy_ch7_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 12 ;
    cs_uint16 gfp_pli              :  1 ; /* bits 3:3 */
    cs_uint16 store_fwd            :  1 ; /* bits 2:2 */
    cs_uint16 drop                 :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 drop                 :  1 ; /* bits 1:1 */
    cs_uint16 store_fwd            :  1 ; /* bits 2:2 */
    cs_uint16 gfp_pli              :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd2               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXCFG_phy_ch7_t;

#define ARN5_SPI4P2A_TXCFG_tx_sched_changed      0x0ac08
#define ARN5_SPI4P2A_TXCFG_tx_sched_changed_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 flag                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 flag                 :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXCFG_tx_sched_changed_t;

#define ARN5_SPI4P2A_TXCFG_tx_sched_0            0x0ac09
#define ARN5_SPI4P2A_TXCFG_tx_sched_0_dft        0x0123
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 ent_0                :  4 ; /* bits 15:12 */
    cs_uint16 ent_1                :  4 ; /* bits 11:8 */
    cs_uint16 ent_2                :  4 ; /* bits 7:4 */
    cs_uint16 ent_3                :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent_3                :  4 ; /* bits 3:0 */
    cs_uint16 ent_2                :  4 ; /* bits 7:4 */
    cs_uint16 ent_1                :  4 ; /* bits 11:8 */
    cs_uint16 ent_0                :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXCFG_tx_sched_0_t;

#define ARN5_SPI4P2A_TXCFG_tx_sched_1            0x0ac0a
#define ARN5_SPI4P2A_TXCFG_tx_sched_1_dft        0x4567
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 ent_4                :  4 ; /* bits 15:12 */
    cs_uint16 ent_5                :  4 ; /* bits 11:8 */
    cs_uint16 ent_6                :  4 ; /* bits 7:4 */
    cs_uint16 ent_7                :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent_7                :  4 ; /* bits 3:0 */
    cs_uint16 ent_6                :  4 ; /* bits 7:4 */
    cs_uint16 ent_5                :  4 ; /* bits 11:8 */
    cs_uint16 ent_4                :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXCFG_tx_sched_1_t;

#define ARN5_SPI4P2A_TXCFG_tx_sched_2            0x0ac0b
#define ARN5_SPI4P2A_TXCFG_tx_sched_2_dft        0x0123
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 ent_8                :  4 ; /* bits 15:12 */
    cs_uint16 ent_9                :  4 ; /* bits 11:8 */
    cs_uint16 ent_10               :  4 ; /* bits 7:4 */
    cs_uint16 ent_11               :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent_11               :  4 ; /* bits 3:0 */
    cs_uint16 ent_10               :  4 ; /* bits 7:4 */
    cs_uint16 ent_9                :  4 ; /* bits 11:8 */
    cs_uint16 ent_8                :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXCFG_tx_sched_2_t;

#define ARN5_SPI4P2A_TXCFG_tx_sched_3            0x0ac0c
#define ARN5_SPI4P2A_TXCFG_tx_sched_3_dft        0x4567
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 ent_12               :  4 ; /* bits 15:12 */
    cs_uint16 ent_13               :  4 ; /* bits 11:8 */
    cs_uint16 ent_14               :  4 ; /* bits 7:4 */
    cs_uint16 ent_15               :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ent_15               :  4 ; /* bits 3:0 */
    cs_uint16 ent_14               :  4 ; /* bits 7:4 */
    cs_uint16 ent_13               :  4 ; /* bits 11:8 */
    cs_uint16 ent_12               :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_SPI4P2A_TXCFG_tx_sched_3_t;


/******************************************************************
 *
 * Block declaration.
 *
 ******************************************************************/

/* SPI4P2a */

typedef struct {
  ARN5_SPI4P2A_TX_RPRIF_afifo_intrpt_t     afifo_intrpt;                  /* addr: 0x0000 */
  ARN5_SPI4P2A_TX_RPRIF_afifo_intrpten_t   afifo_intrpten;                /* addr: 0x0001 */
  ARN5_SPI4P2A_TX_RPRIF_intrpt_cntr_cfg0_t tx_rprif_intrpt_cntr_cfg0;     /* addr: 0x0002 */
  ARN5_SPI4P2A_TX_RPRIF_intrpt_cntr0_t     tx_rprif_intrpt_cntr0;         /* addr: 0x0003 */
  cs_uint16                                rsrvd1[252];
  ARN5_SPI4P2A_RX_RPRIF_dropch_t           dropch;                        /* addr: 0x0100 */
  ARN5_SPI4P2A_RX_RPRIF_disable_filter_bad_pkts_t disable_filter_bad_pkts;       /* addr: 0x0101 */
  ARN5_SPI4P2A_RX_RPRIF_intrpt_vec_t       rx_rprif_intrpt_vec;           /* addr: 0x0102 */
  ARN5_SPI4P2A_RX_RPRIF_intrpt_vec_en_t    rx_rprif_intrpt_vec_en;        /* addr: 0x0103 */
  ARN5_SPI4P2A_RX_RPRIF_misseop_intrpt_t   misseop_intrpt;                /* addr: 0x0104 */
  ARN5_SPI4P2A_RX_RPRIF_misseop_intrpten_t misseop_intrpten;              /* addr: 0x0105 */
  ARN5_SPI4P2A_RX_RPRIF_misssop_intrpt_t   misssop_intrpt;                /* addr: 0x0106 */
  ARN5_SPI4P2A_RX_RPRIF_misssop_intrpten_t misssop_intrpten;              /* addr: 0x0107 */
  ARN5_SPI4P2A_RX_RPRIF_spi4p1_intrpt_t    spi4p1_intrpt;                 /* addr: 0x0108 */
  ARN5_SPI4P2A_RX_RPRIF_spi4p1_intrpten_t  spi4p1_intrpten;               /* addr: 0x0109 */
  ARN5_SPI4P2A_RX_RPRIF_intrpt_cntr_cfg0_t rx_rprif_intrpt_cntr_cfg0;     /* addr: 0x010a */
  ARN5_SPI4P2A_RX_RPRIF_intrpt_cntr0_t     rx_rprif_intrpt_cntr0;         /* addr: 0x010b */
  ARN5_SPI4P2A_RX_RPRIF_intrpt_cntr_cfg1_t rx_rprif_intrpt_cntr_cfg1;     /* addr: 0x010c */
  ARN5_SPI4P2A_RX_RPRIF_intrpt_cntr1_t     rx_rprif_intrpt_cntr1;         /* addr: 0x010d */
  cs_uint16                                rsrvd2[242];
  ARN5_SPI4P2A_TXMEM_mem_lo_0_t            mem_lo_0;                      /* addr: 0x0200 */
  ARN5_SPI4P2A_TXMEM_mem_lo_1_t            mem_lo_1;                      /* addr: 0x0201 */
  ARN5_SPI4P2A_TXMEM_mem_lo_2_t            mem_lo_2;                      /* addr: 0x0202 */
  ARN5_SPI4P2A_TXMEM_mem_lo_3_t            mem_lo_3;                      /* addr: 0x0203 */
  ARN5_SPI4P2A_TXMEM_mem_lo_4_t            mem_lo_4;                      /* addr: 0x0204 */
  ARN5_SPI4P2A_TXMEM_mem_lo_5_t            mem_lo_5;                      /* addr: 0x0205 */
  ARN5_SPI4P2A_TXMEM_mem_lo_6_t            mem_lo_6;                      /* addr: 0x0206 */
  ARN5_SPI4P2A_TXMEM_mem_lo_7_t            mem_lo_7;                      /* addr: 0x0207 */
  ARN5_SPI4P2A_TXMEM_mem_hi_0_t            mem_hi_0;                      /* addr: 0x0208 */
  ARN5_SPI4P2A_TXMEM_mem_hi_1_t            mem_hi_1;                      /* addr: 0x0209 */
  ARN5_SPI4P2A_TXMEM_mem_hi_2_t            mem_hi_2;                      /* addr: 0x020a */
  ARN5_SPI4P2A_TXMEM_mem_hi_3_t            mem_hi_3;                      /* addr: 0x020b */
  ARN5_SPI4P2A_TXMEM_mem_hi_4_t            mem_hi_4;                      /* addr: 0x020c */
  ARN5_SPI4P2A_TXMEM_mem_hi_5_t            mem_hi_5;                      /* addr: 0x020d */
  ARN5_SPI4P2A_TXMEM_mem_hi_6_t            mem_hi_6;                      /* addr: 0x020e */
  ARN5_SPI4P2A_TXMEM_mem_hi_7_t            mem_hi_7;                      /* addr: 0x020f */
  cs_uint16                                rsrvd3[240];
  ARN5_SPI4P2A_TXINT_dip2errctrl_t         dip2errctrl;                   /* addr: 0x0300 */
  ARN5_SPI4P2A_TXINT_fifo_eccerr_ctrl_t    txint_fifo_eccerr_ctrl;        /* addr: 0x0301 */
  ARN5_SPI4P2A_TXINT_fifo_eccfix_ctrl_t    txint_fifo_eccfix_ctrl;        /* addr: 0x0302 */
  ARN5_SPI4P2A_TXINT_perr_ctrl_t           txint_perr_ctrl;               /* addr: 0x0303 */
  ARN5_SPI4P2A_TXINT_intrpt_vec_t          txint_intrpt_vec;              /* addr: 0x0304 */
  ARN5_SPI4P2A_TXINT_intrpt_vec_en_t       txint_intrpt_vec_en;           /* addr: 0x0305 */
  ARN5_SPI4P2A_TXINT_intrpt_t              txint_intrpt;                  /* addr: 0x0306 */
  ARN5_SPI4P2A_TXINT_intrpten_t            txint_intrpten;                /* addr: 0x0307 */
  ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpt_t  txint_fifo_ovrflw_intrpt;      /* addr: 0x0308 */
  ARN5_SPI4P2A_TXINT_fifo_ovrflw_intrpten_t txint_fifo_ovrflw_intrpten;    /* addr: 0x0309 */
  ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpt_t fifo_abort_pkt_intrpt;         /* addr: 0x030a */
  ARN5_SPI4P2A_TXINT_fifo_abort_pkt_intrpten_t fifo_abort_pkt_intrpten;       /* addr: 0x030b */
  ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpt_t fifo_drop_pkt_intrpt;          /* addr: 0x030c */
  ARN5_SPI4P2A_TXINT_fifo_drop_pkt_intrpten_t fifo_drop_pkt_intrpten;        /* addr: 0x030d */
  ARN5_SPI4P2A_TXINT_fifo_eccerr_intrpt_t  txint_fifo_eccerr_intrpt;      /* addr: 0x030e */
  ARN5_SPI4P2A_TXINT_fifo_eccerr_intrpten_t txint_fifo_eccerr_intrpten;    /* addr: 0x030f */
  ARN5_SPI4P2A_TXINT_fifo_eccfix_intrpt_t  txint_fifo_eccfix_intrpt;      /* addr: 0x0310 */
  ARN5_SPI4P2A_TXINT_fifo_eccfix_intrpten_t txint_fifo_eccfix_intrpten;    /* addr: 0x0311 */
  ARN5_SPI4P2A_TXINT_perr_intrpt_t         txint_perr_intrpt;             /* addr: 0x0312 */
  ARN5_SPI4P2A_TXINT_perr_intrpten_t       txint_perr_intrpten;           /* addr: 0x0313 */
  ARN5_SPI4P2A_TXINT_max_pkt_err_intrpt_t  max_pkt_err_intrpt;            /* addr: 0x0314 */
  ARN5_SPI4P2A_TXINT_max_pkt_err_intrpten_t max_pkt_err_intrpten;          /* addr: 0x0315 */
  ARN5_SPI4P2A_TXINT_sop_eop_err_intrpt_t  sop_eop_err_intrpt;            /* addr: 0x0316 */
  ARN5_SPI4P2A_TXINT_sop_eop_err_intrpten_t sop_eop_err_intrpten;          /* addr: 0x0317 */
  ARN5_SPI4P2A_TXINT_intrpt_cntr_cfg0_t    txint_intrpt_cntr_cfg0;        /* addr: 0x0318 */
  ARN5_SPI4P2A_TXINT_intrpt_cntr0_t        txint_intrpt_cntr0;            /* addr: 0x0319 */
  ARN5_SPI4P2A_TXINT_intrpt_cntr_cfg1_t    txint_intrpt_cntr_cfg1;        /* addr: 0x031a */
  ARN5_SPI4P2A_TXINT_intrpt_cntr1_t        txint_intrpt_cntr1;            /* addr: 0x031b */
  ARN5_SPI4P2A_TXINT_intrpt_cntr_cfg2_t    txint_intrpt_cntr_cfg2;        /* addr: 0x031c */
  ARN5_SPI4P2A_TXINT_intrpt_cntr2_t        txint_intrpt_cntr2;            /* addr: 0x031d */
  ARN5_SPI4P2A_TXINT_intrpt_cntr_cfg3_t    txint_intrpt_cntr_cfg3;        /* addr: 0x031e */
  ARN5_SPI4P2A_TXINT_intrpt_cntr3_t        txint_intrpt_cntr3;            /* addr: 0x031f */
  cs_uint16                                rsrvd4[224];
  ARN5_SPI4P2A_TXCH_almost_full_t          almost_full;                   /* addr: 0x0400 */
  ARN5_SPI4P2A_TXCH_almost_empty_t         almost_empty;                  /* addr: 0x0401 */
  cs_uint16                                rsrvd5[254];
  ARN5_SPI4P2A_RXINT_fifo_eccerr_ctrl_t    rxint_fifo_eccerr_ctrl;        /* addr: 0x0500 */
  ARN5_SPI4P2A_RXINT_fifo_eccfix_ctrl_t    rxint_fifo_eccfix_ctrl;        /* addr: 0x0501 */
  ARN5_SPI4P2A_RXINT_perr_ctrl_t           rxint_perr_ctrl;               /* addr: 0x0502 */
  ARN5_SPI4P2A_RXINT_rxintrpt_t            rxintrpt;                      /* addr: 0x0503 */
  ARN5_SPI4P2A_RXINT_rxintrpten_t          rxintrpten;                    /* addr: 0x0504 */
  ARN5_SPI4P2A_RXINT_rxloscnt_t            rxloscnt;                      /* addr: 0x0505 */
  ARN5_SPI4P2A_RXINT_dip2errcnt_t          dip2errcnt;                    /* addr: 0x0506 */
  ARN5_SPI4P2A_RXINT_intrpt_vec_t          rxint_intrpt_vec;              /* addr: 0x0507 */
  ARN5_SPI4P2A_RXINT_intrpt_vec_en_t       rxint_intrpt_vec_en;           /* addr: 0x0508 */
  ARN5_SPI4P2A_RXINT_fifo_ovrflw_intrpt_t  rxint_fifo_ovrflw_intrpt;      /* addr: 0x0509 */
  ARN5_SPI4P2A_RXINT_fifo_ovrflw_intrpten_t rxint_fifo_ovrflw_intrpten;    /* addr: 0x050a */
  ARN5_SPI4P2A_RXINT_fifo_eccerr_intrpt_t  rxint_fifo_eccerr_intrpt;      /* addr: 0x050b */
  ARN5_SPI4P2A_RXINT_fifo_eccerr_intrpten_t rxint_fifo_eccerr_intrpten;    /* addr: 0x050c */
  ARN5_SPI4P2A_RXINT_fifo_eccfix_intrpt_t  rxint_fifo_eccfix_intrpt;      /* addr: 0x050d */
  ARN5_SPI4P2A_RXINT_fifo_eccfix_intrpten_t rxint_fifo_eccfix_intrpten;    /* addr: 0x050e */
  ARN5_SPI4P2A_RXINT_perr_intrpt_t         rxint_perr_intrpt;             /* addr: 0x050f */
  ARN5_SPI4P2A_RXINT_perr_intrpten_t       rxint_perr_intrpten;           /* addr: 0x0510 */
  ARN5_SPI4P2A_RXINT_intrpt_cntr_cfg0_t    rxint_intrpt_cntr_cfg0;        /* addr: 0x0511 */
  ARN5_SPI4P2A_RXINT_intrpt_cntr0_t        rxint_intrpt_cntr0;            /* addr: 0x0512 */
  ARN5_SPI4P2A_RXINT_intrpt_cntr_cfg1_t    rxint_intrpt_cntr_cfg1;        /* addr: 0x0513 */
  ARN5_SPI4P2A_RXINT_intrpt_cntr1_t        rxint_intrpt_cntr1;            /* addr: 0x0514 */
  ARN5_SPI4P2A_RXINT_intrpt_cntr_cfg2_t    rxint_intrpt_cntr_cfg2;        /* addr: 0x0515 */
  ARN5_SPI4P2A_RXINT_intrpt_cntr2_t        rxint_intrpt_cntr2;            /* addr: 0x0516 */
  ARN5_SPI4P2A_RXINT_intrpt_cntr_cfg3_t    rxint_intrpt_cntr_cfg3;        /* addr: 0x0517 */
  ARN5_SPI4P2A_RXINT_intrpt_cntr3_t        rxint_intrpt_cntr3;            /* addr: 0x0518 */
  cs_uint16                                rsrvd6[1255];
  ARN5_SPI4P2A_MEMBIST_mb_done_t           mb_done;                       /* addr: 0x0a00 */
  ARN5_SPI4P2A_MEMBIST_mb_fail_t           mb_fail;                       /* addr: 0x0a01 */
  ARN5_SPI4P2A_MEMBIST_sb_strready_t       sb_strready;                   /* addr: 0x0a02 */
  ARN5_SPI4P2A_MEMBIST_sb_birafail_t       sb_birafail;                   /* addr: 0x0a03 */
  ARN5_SPI4P2A_MEMBIST_sb_bisterror_t      sb_bisterror;                  /* addr: 0x0a04 */
  ARN5_SPI4P2A_MEMBIST_intrpt_t            membist_intrpt;                /* addr: 0x0a05 */
  ARN5_SPI4P2A_MEMBIST_intrpten_t          membist_intrpten;              /* addr: 0x0a06 */
  cs_uint16                                rsrvd7[505];
  ARN5_SPI4P2A_TXCFG_phy_ch0_t             phy_ch0;                       /* addr: 0x0c00 */
  ARN5_SPI4P2A_TXCFG_phy_ch1_t             phy_ch1;                       /* addr: 0x0c01 */
  ARN5_SPI4P2A_TXCFG_phy_ch2_t             phy_ch2;                       /* addr: 0x0c02 */
  ARN5_SPI4P2A_TXCFG_phy_ch3_t             phy_ch3;                       /* addr: 0x0c03 */
  ARN5_SPI4P2A_TXCFG_phy_ch4_t             phy_ch4;                       /* addr: 0x0c04 */
  ARN5_SPI4P2A_TXCFG_phy_ch5_t             phy_ch5;                       /* addr: 0x0c05 */
  ARN5_SPI4P2A_TXCFG_phy_ch6_t             phy_ch6;                       /* addr: 0x0c06 */
  ARN5_SPI4P2A_TXCFG_phy_ch7_t             phy_ch7;                       /* addr: 0x0c07 */
  ARN5_SPI4P2A_TXCFG_tx_sched_changed_t    tx_sched_changed;              /* addr: 0x0c08 */
  ARN5_SPI4P2A_TXCFG_tx_sched_0_t          tx_sched_0;                    /* addr: 0x0c09 */
  ARN5_SPI4P2A_TXCFG_tx_sched_1_t          tx_sched_1;                    /* addr: 0x0c0a */
  ARN5_SPI4P2A_TXCFG_tx_sched_2_t          tx_sched_2;                    /* addr: 0x0c0b */
  ARN5_SPI4P2A_TXCFG_tx_sched_3_t          tx_sched_3;                    /* addr: 0x0c0c */
} ARN5_SPI4P2a_t;

/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * LIF::LIF
 */

#define ARN5_LIF_RESET                           0x12000
#define ARN5_LIF_RESET_dft                       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 RX_RST               :  1 ; /* bits 1:1 */
    cs_uint16 TX_RST               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TX_RST               :  1 ; /* bits 0:0 */
    cs_uint16 RX_RST               :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_RESET_t;

#define ARN5_LIF_STATUS                          0x12001
#define ARN5_LIF_STATUS_dft                      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 PRBS_RX_EFs          :  1 ; /* bits 4:4 */
    cs_uint16 PRBS_SYNCs           :  1 ; /* bits 3:3 */
    cs_uint16 NO_DATAs             :  1 ; /* bits 2:2 */
    cs_uint16 IN_LOCKs             :  1 ; /* bits 1:1 */
    cs_uint16 SRX_LOCKDETIs        :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_LOCKDETIs        :  1 ; /* bits 0:0 */
    cs_uint16 IN_LOCKs             :  1 ; /* bits 1:1 */
    cs_uint16 NO_DATAs             :  1 ; /* bits 2:2 */
    cs_uint16 PRBS_SYNCs           :  1 ; /* bits 3:3 */
    cs_uint16 PRBS_RX_EFs          :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_STATUS_t;

#define ARN5_LIF_MASK                            0x12002
#define ARN5_LIF_MASK_dft                        0x001f
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 PRBS_RX_EFm          :  1 ; /* bits 4:4 */
    cs_uint16 PRBS_SYNCm           :  1 ; /* bits 3:3 */
    cs_uint16 NO_DATAm             :  1 ; /* bits 2:2 */
    cs_uint16 IN_LOCKm             :  1 ; /* bits 1:1 */
    cs_uint16 SRX_LOCKDETIm        :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_LOCKDETIm        :  1 ; /* bits 0:0 */
    cs_uint16 IN_LOCKm             :  1 ; /* bits 1:1 */
    cs_uint16 NO_DATAm             :  1 ; /* bits 2:2 */
    cs_uint16 PRBS_SYNCm           :  1 ; /* bits 3:3 */
    cs_uint16 PRBS_RX_EFm          :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_MASK_t;

#define ARN5_LIF_INTERRUPT                       0x12003
#define ARN5_LIF_INTERRUPT_dft                   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 PRBS_RX_EFi          :  1 ; /* bits 4:4 */
    cs_uint16 PRBS_SYNCi           :  1 ; /* bits 3:3 */
    cs_uint16 NO_DATAi             :  1 ; /* bits 2:2 */
    cs_uint16 IN_LOCKi             :  1 ; /* bits 1:1 */
    cs_uint16 SRX_LOCKDETIi        :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_LOCKDETIi        :  1 ; /* bits 0:0 */
    cs_uint16 IN_LOCKi             :  1 ; /* bits 1:1 */
    cs_uint16 NO_DATAi             :  1 ; /* bits 2:2 */
    cs_uint16 PRBS_SYNCi           :  1 ; /* bits 3:3 */
    cs_uint16 PRBS_RX_EFi          :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_INTERRUPT_t;

#define ARN5_LIF_GP_PROV                         0x12004
#define ARN5_LIF_GP_PROV_dft                     0x001f
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 RX_UP_NIBBLE_FLIP    :  1 ; /* bits 15:15 */
    cs_uint16 RX_LO_NIBBLE_FLIP    :  1 ; /* bits 14:14 */
    cs_uint16 RX_WORD_FLIP         :  1 ; /* bits 13:13 */
    cs_uint16 RX_DATA_INV          :  1 ; /* bits 12:12 */
    cs_uint16 TX_UP_NIBBLE_FLIP    :  1 ; /* bits 11:11 */
    cs_uint16 TX_LO_NIBBLE_FLIP    :  1 ; /* bits 10:10 */
    cs_uint16 TX_WORD_FLIP         :  1 ; /* bits 9:9 */
    cs_uint16 TX_DATA_INV          :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 RX_EVEN_PARITY       :  1 ; /* bits 4:4 */
    cs_uint16 LOS_SQELCH_E         :  1 ; /* bits 3:3 */
    cs_uint16 LOF_SQELCH_E         :  1 ; /* bits 2:2 */
    cs_uint16 AISL_SQELCH_E        :  1 ; /* bits 1:1 */
    cs_uint16 NO_LOCK_SQELCH_E     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 NO_LOCK_SQELCH_E     :  1 ; /* bits 0:0 */
    cs_uint16 AISL_SQELCH_E        :  1 ; /* bits 1:1 */
    cs_uint16 LOF_SQELCH_E         :  1 ; /* bits 2:2 */
    cs_uint16 LOS_SQELCH_E         :  1 ; /* bits 3:3 */
    cs_uint16 RX_EVEN_PARITY       :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 TX_DATA_INV          :  1 ; /* bits 8:8 */
    cs_uint16 TX_WORD_FLIP         :  1 ; /* bits 9:9 */
    cs_uint16 TX_LO_NIBBLE_FLIP    :  1 ; /* bits 10:10 */
    cs_uint16 TX_UP_NIBBLE_FLIP    :  1 ; /* bits 11:11 */
    cs_uint16 RX_DATA_INV          :  1 ; /* bits 12:12 */
    cs_uint16 RX_WORD_FLIP         :  1 ; /* bits 13:13 */
    cs_uint16 RX_LO_NIBBLE_FLIP    :  1 ; /* bits 14:14 */
    cs_uint16 RX_UP_NIBBLE_FLIP    :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_GP_PROV_t;

#define ARN5_LIF_GP_CTRL                         0x12005
#define ARN5_LIF_GP_CTRL_dft                     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 10 ;
    cs_uint16 STX_CKREF_SEL        :  1 ; /* bits 5:5 */
    cs_uint16 STX_LPTIME_E         :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 LINE_LOOP_BK         :  1 ; /* bits 2:2 */
    cs_uint16 FE_TERM_LP_BK        :  1 ; /* bits 1:1 */
    cs_uint16 TERM_LP_BK           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TERM_LP_BK           :  1 ; /* bits 0:0 */
    cs_uint16 FE_TERM_LP_BK        :  1 ; /* bits 1:1 */
    cs_uint16 LINE_LOOP_BK         :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 STX_LPTIME_E         :  1 ; /* bits 4:4 */
    cs_uint16 STX_CKREF_SEL        :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd2               : 10 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_GP_CTRL_t;

#define ARN5_LIF_PRBS_PROV                       0x12006
#define ARN5_LIF_PRBS_PROV_dft                   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 PRBS_RX_ENABLE       :  1 ; /* bits 7:7 */
    cs_uint16 PRBS_RX_INVERT       :  1 ; /* bits 6:6 */
    cs_uint16 PRBS_RX_SEL          :  2 ; /* bits 5:4 */
    cs_uint16 PRBS_TX_ENABLE       :  1 ; /* bits 3:3 */
    cs_uint16 PRBS_TX_INVERT       :  1 ; /* bits 2:2 */
    cs_uint16 PRBS_TX_SEL          :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PRBS_TX_SEL          :  2 ; /* bits 1:0 */
    cs_uint16 PRBS_TX_INVERT       :  1 ; /* bits 2:2 */
    cs_uint16 PRBS_TX_ENABLE       :  1 ; /* bits 3:3 */
    cs_uint16 PRBS_RX_SEL          :  2 ; /* bits 5:4 */
    cs_uint16 PRBS_RX_INVERT       :  1 ; /* bits 6:6 */
    cs_uint16 PRBS_RX_ENABLE       :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_PRBS_PROV_t;
#define     ARN5_PRBS_RX_ENABLE_PRBS_RX_DIS               0x00
#define     ARN5_PRBS_RX_ENABLE_PRBS_RX_E                 0x01
#define     ARN5_PRBS_RX_SEL_PRBS_Rx_2exp31               0x00
#define     ARN5_PRBS_RX_SEL_PRBS_Rx_2exp23               0x01
#define     ARN5_PRBS_RX_SEL_PRBS_Rx_2exp15               0x02
#define     ARN5_PRBS_RX_SEL_PRBS_Rx_2exp7                0x03
#define     ARN5_PRBS_TX_ENABLE_PRBS_TX_DIS               0x00
#define     ARN5_PRBS_TX_ENABLE_PRBS_TX_E                 0x01
#define     ARN5_PRBS_TX_SEL_PRBS_Tx_2exp31               0x00
#define     ARN5_PRBS_TX_SEL_PRBS_Tx_2exp23               0x01
#define     ARN5_PRBS_TX_SEL_PRBS_Tx_2exp15               0x02
#define     ARN5_PRBS_TX_SEL_PRBS_Tx_2exp7                0x03

#define ARN5_LIF_PRBS_CTRL                       0x12007
#define ARN5_LIF_PRBS_CTRL_dft                   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 PRBS_RX_LOAD         :  1 ; /* bits 4:4 */
    cs_uint16 PRBS_WORD_ERR_INJ    :  1 ; /* bits 3:3 */
    cs_uint16 PRBS_BIT_ERR_INJ     :  1 ; /* bits 2:2 */
    cs_uint16 PRBS_WORD_ERR_INJ_ONCE :  1 ; /* bits 1:1 */
    cs_uint16 PRBS_BIT_ERR_INJ_ONCE :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PRBS_BIT_ERR_INJ_ONCE :  1 ; /* bits 0:0 */
    cs_uint16 PRBS_WORD_ERR_INJ_ONCE :  1 ; /* bits 1:1 */
    cs_uint16 PRBS_BIT_ERR_INJ     :  1 ; /* bits 2:2 */
    cs_uint16 PRBS_WORD_ERR_INJ    :  1 ; /* bits 3:3 */
    cs_uint16 PRBS_RX_LOAD         :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_PRBS_CTRL_t;

#define ARN5_LIF_PRBS_CNT_MSB                    0x12008
#define ARN5_LIF_PRBS_CNT_MSB_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 PRBS_ERR_CNT_MSB     : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PRBS_ERR_CNT_MSB     : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_PRBS_CNT_MSB_t;

#define ARN5_LIF_PRBS_CNT_LSB                    0x12009
#define ARN5_LIF_PRBS_CNT_LSB_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 PRBS_ERR_CNT_LSB     : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PRBS_ERR_CNT_LSB     : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_PRBS_CNT_LSB_t;

#define ARN5_LIF_RX_LOCK_OFFSET                  0x1200a
#define ARN5_LIF_RX_LOCK_OFFSET_dft              0x0021
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 LOCK_OFF             :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOCK_OFF             :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_RX_LOCK_OFFSET_t;

#define ARN5_LIF_RX_LOCK_THRESHOLD               0x1200b
#define ARN5_LIF_RX_LOCK_THRESHOLD_dft           0x0011
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 LOCK_THR             :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOCK_THR             :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_RX_LOCK_THRESHOLD_t;

#define ARN5_LIF_RX_LOCK_RESOL                   0x1200c
#define ARN5_LIF_RX_LOCK_RESOL_dft               0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 LOCK_RES             :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOCK_RES             :  2 ; /* bits 1:0 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_RX_LOCK_RESOL_t;
#define     ARN5_LOCK_RES_LOW_RES                         0x00
#define     ARN5_LOCK_RES_MED_RES                         0x01
#define     ARN5_LOCK_RES_HIGH_RES                        0x02
#define     ARN5_LOCK_RES_VERYHIGH_RES                    0x03

#define ARN5_LIF_ESTORE_RESYNC                   0x1200f
#define ARN5_LIF_ESTORE_RESYNC_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 RX_DATA_RESYNC       :  1 ; /* bits 3:3 */
    cs_uint16 RX_LPBK_RESYNC       :  1 ; /* bits 2:2 */
    cs_uint16 TX_DATA_RESYNC       :  1 ; /* bits 1:1 */
    cs_uint16 TX_LPBK_RESYNC       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TX_LPBK_RESYNC       :  1 ; /* bits 0:0 */
    cs_uint16 TX_DATA_RESYNC       :  1 ; /* bits 1:1 */
    cs_uint16 RX_LPBK_RESYNC       :  1 ; /* bits 2:2 */
    cs_uint16 RX_DATA_RESYNC       :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_ESTORE_RESYNC_t;

#define ARN5_LIF_CLOCK_ENABLE                    0x12010
#define ARN5_LIF_CLOCK_ENABLE_dft                0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 RX_CLK_E             :  1 ; /* bits 2:2 */
    cs_uint16 TX_CLK_E             :  1 ; /* bits 1:1 */
    cs_uint16 LD_CLK_E             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LD_CLK_E             :  1 ; /* bits 0:0 */
    cs_uint16 TX_CLK_E             :  1 ; /* bits 1:1 */
    cs_uint16 RX_CLK_E             :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_CLOCK_ENABLE_t;

#define ARN5_LIF_TIMING_CTRL                     0x12011
#define ARN5_LIF_TIMING_CTRL_dft                 0x0035
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 SRX_DIV_SEL          :  5 ; /* bits 8:4 */
    cs_uint16 STX_CKREF_DIV1       :  2 ; /* bits 3:2 */
    cs_uint16 STX_CKREF_DIV2       :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 STX_CKREF_DIV2       :  2 ; /* bits 1:0 */
    cs_uint16 STX_CKREF_DIV1       :  2 ; /* bits 3:2 */
    cs_uint16 SRX_DIV_SEL          :  5 ; /* bits 8:4 */
    cs_uint16 rsrvd1               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_TIMING_CTRL_t;
#define     ARN5_SRX_DIV_SEL_Rx_622_MHz                   0x00
#define     ARN5_SRX_DIV_SEL_Rx_311_MHz                   0x01
#define     ARN5_SRX_DIV_SEL_Rx_155_MHz                   0x02
#define     ARN5_SRX_DIV_SEL_Rx_78_MHz                    0x03
#define     ARN5_STX_CKREF_DIV2_STX_155REF_LOCAL          0x05

#define ARN5_LIF_CLKOUT_CTRL                     0x12012
#define ARN5_LIF_CLKOUT_CTRL_dft                 0x2013
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 STX_CLKOUT_E         :  1 ; /* bits 14:14 */
    cs_uint16 STX_CLKOUT_SEL       :  2 ; /* bits 13:12 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 SRX_CLKOUT_E         :  1 ; /* bits 8:8 */
    cs_uint16 SRX_CLKOUT_SEL       :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_CLKOUT_SEL       :  8 ; /* bits 7:0 */
    cs_uint16 SRX_CLKOUT_E         :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 STX_CLKOUT_SEL       :  2 ; /* bits 13:12 */
    cs_uint16 STX_CLKOUT_E         :  1 ; /* bits 14:14 */
    cs_uint16 rsrvd2               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_CLKOUT_CTRL_t;
#define     ARN5_STX_CLKOUT_SEL_STX_CKO_STX_CLK           0x00
#define     ARN5_STX_CLKOUT_SEL_STX_CKO_STX_PILOT         0x01
#define     ARN5_STX_CLKOUT_SEL_STX_CKO_155_52_MHz        0x02
#define     ARN5_STX_CLKOUT_SEL_STX_CKO_High_Speed        0x03
#define     ARN5_SRX_CLKOUT_SEL_RX_CKO_622_MHz            0x03
#define     ARN5_SRX_CLKOUT_SEL_RX_CKO_311_MHz            0x0b
#define     ARN5_SRX_CLKOUT_SEL_RX_CKO_155_MHz            0x13
#define     ARN5_SRX_CLKOUT_SEL_RX_CKO_78_MHz             0x1b
#define     ARN5_SRX_CLKOUT_SEL_RX_CKO_39_MHz             0x33
#define     ARN5_SRX_CLKOUT_SEL_RX_CKO_SRX_CLK            0x3b
#define     ARN5_SRX_CLKOUT_SEL_RX_CKO_2_5_GHz            0x3d
#define     ARN5_SRX_CLKOUT_SEL_RX_CKO_data               0x3e

#define ARN5_LIF_RX_DATA_CTRL                    0x12013
#define ARN5_LIF_RX_DATA_CTRL_dft                0x4010
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 SRX_HI_INPUT         :  1 ; /* bits 14:14 */
    cs_uint16 SRX_EQADJ            :  2 ; /* bits 13:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 SRX_DATADL           :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 SRX_DATA_LOCK        :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 SRX_FORCE_LOCK       :  1 ; /* bits 1:1 */
    cs_uint16 SRX_PD_MODE          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_PD_MODE          :  1 ; /* bits 0:0 */
    cs_uint16 SRX_FORCE_LOCK       :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 SRX_DATA_LOCK        :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 SRX_DATADL           :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 SRX_EQADJ            :  2 ; /* bits 13:12 */
    cs_uint16 SRX_HI_INPUT         :  1 ; /* bits 14:14 */
    cs_uint16 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_RX_DATA_CTRL_t;

#define ARN5_LIF_RX_CHARGE_PUMP                  0x12014
#define ARN5_LIF_RX_CHARGE_PUMP_dft              0x0f2f
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 SRX_PH_SEL           :  6 ; /* bits 13:8 */
    cs_uint16 SRX_CPCURR           :  4 ; /* bits 7:4 */
    cs_uint16 SRX_CPOFF            :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_CPOFF            :  4 ; /* bits 3:0 */
    cs_uint16 SRX_CPCURR           :  4 ; /* bits 7:4 */
    cs_uint16 SRX_PH_SEL           :  6 ; /* bits 13:8 */
    cs_uint16 rsrvd1               :  2 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_RX_CHARGE_PUMP_t;

#define ARN5_LIF_RX_LOOP_FILTER                  0x12015
#define ARN5_LIF_RX_LOOP_FILTER_dft              0x00a3
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 SRX_REXT_TUNE        :  1 ; /* bits 7:7 */
    cs_uint16 SRX_VCO_FILT_E       :  1 ; /* bits 6:6 */
    cs_uint16 SRX_FILTINT          :  1 ; /* bits 5:5 */
    cs_uint16 SRX_FILT_E           :  1 ; /* bits 4:4 */
    cs_uint16 SRX_FILTR            :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_FILTR            :  4 ; /* bits 3:0 */
    cs_uint16 SRX_FILT_E           :  1 ; /* bits 4:4 */
    cs_uint16 SRX_FILTINT          :  1 ; /* bits 5:5 */
    cs_uint16 SRX_VCO_FILT_E       :  1 ; /* bits 6:6 */
    cs_uint16 SRX_REXT_TUNE        :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_RX_LOOP_FILTER_t;

#define ARN5_LIF_SERDES_CONFIGURATION            0x12016
#define ARN5_LIF_SERDES_CONFIGURATION_dft        0x5070
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 STX_FREQ_ADJ         :  2 ; /* bits 15:14 */
    cs_uint16 SRX_FREQ_ADJ         :  2 ; /* bits 13:12 */
    cs_uint16 MODE_OVER_RIDE       :  1 ; /* bits 11:11 */
    cs_uint16 STX_OC12_E           :  1 ; /* bits 10:10 */
    cs_uint16 STX_OC3_E            :  1 ; /* bits 9:9 */
    cs_uint16 STX_1GE_E            :  1 ; /* bits 8:8 */
    cs_uint16 STX_E_CKREF_SEL      :  1 ; /* bits 7:7 */
    cs_uint16 SRX_CLK_E            :  1 ; /* bits 6:6 */
    cs_uint16 SRX_CLK_INV          :  1 ; /* bits 5:5 */
    cs_uint16 STX_CLK_E            :  1 ; /* bits 4:4 */
    cs_uint16 PD_OR                :  1 ; /* bits 3:3 */
    cs_uint16 SRX_AMP_PD           :  1 ; /* bits 2:2 */
    cs_uint16 SRX_PD               :  1 ; /* bits 1:1 */
    cs_uint16 STX_PD               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 STX_PD               :  1 ; /* bits 0:0 */
    cs_uint16 SRX_PD               :  1 ; /* bits 1:1 */
    cs_uint16 SRX_AMP_PD           :  1 ; /* bits 2:2 */
    cs_uint16 PD_OR                :  1 ; /* bits 3:3 */
    cs_uint16 STX_CLK_E            :  1 ; /* bits 4:4 */
    cs_uint16 SRX_CLK_INV          :  1 ; /* bits 5:5 */
    cs_uint16 SRX_CLK_E            :  1 ; /* bits 6:6 */
    cs_uint16 STX_E_CKREF_SEL      :  1 ; /* bits 7:7 */
    cs_uint16 STX_1GE_E            :  1 ; /* bits 8:8 */
    cs_uint16 STX_OC3_E            :  1 ; /* bits 9:9 */
    cs_uint16 STX_OC12_E           :  1 ; /* bits 10:10 */
    cs_uint16 MODE_OVER_RIDE       :  1 ; /* bits 11:11 */
    cs_uint16 SRX_FREQ_ADJ         :  2 ; /* bits 13:12 */
    cs_uint16 STX_FREQ_ADJ         :  2 ; /* bits 15:14 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_SERDES_CONFIGURATION_t;

#define ARN5_LIF_RX_VCO_TUNE1                    0x12017
#define ARN5_LIF_RX_VCO_TUNE1_dft                0x000b
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 SRX_AMP_PD_OVRIDE    :  2 ; /* bits 9:8 */
    cs_uint16 SRX_VCO_INIT         :  2 ; /* bits 7:6 */
    cs_uint16 SRX_SHORT_OVRIDE     :  2 ; /* bits 5:4 */
    cs_uint16 SRX_FORCE_FREQ       :  1 ; /* bits 3:3 */
    cs_uint16 SRX_FREQ_EXT         :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_FREQ_EXT         :  3 ; /* bits 2:0 */
    cs_uint16 SRX_FORCE_FREQ       :  1 ; /* bits 3:3 */
    cs_uint16 SRX_SHORT_OVRIDE     :  2 ; /* bits 5:4 */
    cs_uint16 SRX_VCO_INIT         :  2 ; /* bits 7:6 */
    cs_uint16 SRX_AMP_PD_OVRIDE    :  2 ; /* bits 9:8 */
    cs_uint16 rsrvd1               :  6 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_RX_VCO_TUNE1_t;
#define     ARN5_SRX_AMP_PD_OVRIDE_SRX_AMP_PD__ALG        0x00
#define     ARN5_SRX_AMP_PD_OVRIDE_SRX_AMP_PD_FRC_0       0x02
#define     ARN5_SRX_AMP_PD_OVRIDE_SRX_AMP_PD_FRC_1       0x03
#define     ARN5_SRX_SHORT_OVRIDE_SRX_SHORT_ALG           0x00
#define     ARN5_SRX_SHORT_OVRIDE_SRX_SHORT_FRC_0         0x02
#define     ARN5_SRX_SHORT_OVRIDE_SRX_SHORT_FRC_1         0x03
#define     ARN5_SRX_FORCE_FREQ_FORCE_FREQ_EXT            0x00
#define     ARN5_SRX_FORCE_FREQ_FORCE_FREQ_ALG            0x01

#define ARN5_LIF_TEST_CTRL                       0x12018
#define ARN5_LIF_TEST_CTRL_dft                   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 10 ;
    cs_uint16 FMRCLK_SEL           :  4 ; /* bits 5:2 */
    cs_uint16 SRX_TCKI_E           :  1 ; /* bits 1:1 */
    cs_uint16 STX_TCKI_E           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 STX_TCKI_E           :  1 ; /* bits 0:0 */
    cs_uint16 SRX_TCKI_E           :  1 ; /* bits 1:1 */
    cs_uint16 FMRCLK_SEL           :  4 ; /* bits 5:2 */
    cs_uint16 rsrvd1               : 10 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_TEST_CTRL_t;
#define     ARN5_FMRCLK_SEL_DISABLE                       0x00
#define     ARN5_FMRCLK_SEL_MPIF_CLK_SEL                  0x08
#define     ARN5_FMRCLK_SEL_50MHZ_CLK_SEL                 0x09
#define     ARN5_FMRCLK_SEL_SRX_CLK_SEL                   0x0a
#define     ARN5_FMRCLK_SEL_STX_CLK_SEL                   0x0b
#define     ARN5_FMRCLK_SEL_200MHZ_CLK_SEL                0x0c

#define ARN5_LIF_TX_OUTPUT_CTRL                  0x12019
#define ARN5_LIF_TX_OUTPUT_CTRL_dft              0x0047
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  6 ;
    cs_uint16 STX_Force_Output     :  2 ; /* bits 9:8 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 STX_LEVEL            :  3 ; /* bits 6:4 */
    cs_uint16 STX_PEAK             :  2 ; /* bits 3:2 */
    cs_uint16 STX_AC_COUPLE        :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 STX_AC_COUPLE        :  2 ; /* bits 1:0 */
    cs_uint16 STX_PEAK             :  2 ; /* bits 3:2 */
    cs_uint16 STX_LEVEL            :  3 ; /* bits 6:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 STX_Force_Output     :  2 ; /* bits 9:8 */
    cs_uint16 rsrvd2               :  6 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_TX_OUTPUT_CTRL_t;
#define     ARN5_STX_Force_Output_STX_Out_Active          0x00
#define     ARN5_STX_Force_Output_STX_Out_Low             0x01
#define     ARN5_STX_Force_Output_STX_Out_Higha           0x02
#define     ARN5_STX_Force_Output_STX_Out_Highb           0x03
#define     ARN5_STX_AC_COUPLE_STX_BIAS_OFF               0x00
#define     ARN5_STX_AC_COUPLE_STX_BIAS_LOW               0x01
#define     ARN5_STX_AC_COUPLE_STX_BIAS_RSVD              0x02
#define     ARN5_STX_AC_COUPLE_STX_BIAS_HIGH              0x03

#define ARN5_LIF_TX_PLL_CTRL                     0x1201a
#define ARN5_LIF_TX_PLL_CTRL_dft                 0x0031
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 10 ;
    cs_uint16 STX_IVCOE            :  2 ; /* bits 5:4 */
    cs_uint16 STX_RES_E            :  3 ; /* bits 3:1 */
    cs_uint16 STX_I400UA_E         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 STX_I400UA_E         :  1 ; /* bits 0:0 */
    cs_uint16 STX_RES_E            :  3 ; /* bits 3:1 */
    cs_uint16 STX_IVCOE            :  2 ; /* bits 5:4 */
    cs_uint16 rsrvd1               : 10 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_TX_PLL_CTRL_t;
#define     ARN5_STX_IVCOE_STX_IVCO_0                     0x00
#define     ARN5_STX_IVCOE_STX_IVCO_1                     0x01
#define     ARN5_STX_IVCOE_STX_IVCO_2                     0x02
#define     ARN5_STX_IVCOE_STX_IVCO_3                     0x03
#define     ARN5_STX_RES_E_STX_RES_1_k_ohm                0x00
#define     ARN5_STX_RES_E_STX_RES_500_ohm                0x01
#define     ARN5_STX_RES_E_STX_RES_250_ohm                0x03
#define     ARN5_STX_RES_E_STX_RES_80_ohm                 0x07

#define ARN5_LIF_VCO_FREQ                        0x1201b
#define ARN5_LIF_VCO_FREQ_dft                    0x0801
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  4 ;
    cs_uint16 SRX_FREQ_RING        :  4 ; /* bits 11:8 */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 GEN_SPARE            :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 GEN_SPARE            :  2 ; /* bits 1:0 */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 SRX_FREQ_RING        :  4 ; /* bits 11:8 */
    cs_uint16 rsrvd2               :  4 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_VCO_FREQ_t;

#define ARN5_LIF_SPARE                           0x1201c
#define ARN5_LIF_SPARE_dft                       0xf0f0
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SRX_SPARE            :  8 ; /* bits 15:8 */
    cs_uint16 STX_SPARE            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 STX_SPARE            :  8 ; /* bits 7:0 */
    cs_uint16 SRX_SPARE            :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_SPARE_t;

#define ARN5_LIF_TX_VCO_TUNE                     0x1201d
#define ARN5_LIF_TX_VCO_TUNE_dft                 0x350b
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 STX_FREQ_THRES       :  8 ; /* bits 15:8 */
    cs_uint16 STX_VCO_INIT         :  2 ; /* bits 7:6 */
    cs_uint16 STX_SHORT_OVRIDE     :  2 ; /* bits 5:4 */
    cs_uint16 STX_FORCE_FREQ       :  1 ; /* bits 3:3 */
    cs_uint16 STX_FREQ_EXT         :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 STX_FREQ_EXT         :  3 ; /* bits 2:0 */
    cs_uint16 STX_FORCE_FREQ       :  1 ; /* bits 3:3 */
    cs_uint16 STX_SHORT_OVRIDE     :  2 ; /* bits 5:4 */
    cs_uint16 STX_VCO_INIT         :  2 ; /* bits 7:6 */
    cs_uint16 STX_FREQ_THRES       :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_TX_VCO_TUNE_t;
#define     ARN5_STX_SHORT_OVRIDE_STX_SHORT_ALG           0x00
#define     ARN5_STX_SHORT_OVRIDE_STX_SHORT_FRC_0         0x02
#define     ARN5_STX_SHORT_OVRIDE_STX_SHORT_FRC_1         0x03
#define     ARN5_STX_FORCE_FREQ_FORCE_FREQ_EXT            0x00
#define     ARN5_STX_FORCE_FREQ_FORCE_FREQ_ALG            0x01

#define ARN5_LIF_STX_VCO_STATUS                  0x1201e
#define ARN5_LIF_STX_VCO_STATUS_dft              0x0303
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  2 ;
    cs_uint16 SRX_AMP_PD           :  1 ; /* bits 13:13 */
    cs_uint16 SRX_SHORT_FILTPN     :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 SRX_FREQ_LC          :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 STX_SHORT_FILTPN     :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 STX_FREQ_LC          :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 STX_FREQ_LC          :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 STX_SHORT_FILTPN     :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 SRX_FREQ_LC          :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 SRX_SHORT_FILTPN     :  1 ; /* bits 12:12 */
    cs_uint16 SRX_AMP_PD           :  1 ; /* bits 13:13 */
    cs_uint16 rsrvd4               :  2 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_STX_VCO_STATUS_t;

#define ARN5_LIF_RX_VCO_TUNE2                    0x1201f
#define ARN5_LIF_RX_VCO_TUNE2_dft                0x0735
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  4 ;
    cs_uint16 SRX_LCEN             :  1 ; /* bits 11:11 */
    cs_uint16 SRX_ILCVCOEN         :  3 ; /* bits 10:8 */
    cs_uint16 SRX_FREQ_THRES       :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SRX_FREQ_THRES       :  8 ; /* bits 7:0 */
    cs_uint16 SRX_ILCVCOEN         :  3 ; /* bits 10:8 */
    cs_uint16 SRX_LCEN             :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd1               :  4 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_LIF_RX_VCO_TUNE2_t;
#define     ARN5_SRX_ILCVCOEN_SRX_ILCVCO_0                0x00
#define     ARN5_SRX_ILCVCOEN_SRX_ILCVCO_1                0x01
#define     ARN5_SRX_ILCVCOEN_SRX_ILCVCO_2                0x02
#define     ARN5_SRX_ILCVCOEN_SRX_ILCVCO_4                0x04
#define     ARN5_SRX_ILCVCOEN_SRX_ILCVCO_3                0x03
#define     ARN5_SRX_ILCVCOEN_SRX_ILCVCO_5                0x05
#define     ARN5_SRX_ILCVCOEN_SRX_ILCVCO_6                0x06
#define     ARN5_SRX_ILCVCOEN_SRX_ILCVCO_7                0x07


/******************************************************************
 *
 * Block declaration.
 *
 ******************************************************************/

/* LIF */

#define ARN5_LIF_STRIDE                          131072
#define ARN5_LIF_COUNT                           8
typedef struct {
  ARN5_LIF_RESET_t                         RESET;                         /* addr: 0x0000 */
  ARN5_LIF_STATUS_t                        STATUS;                        /* addr: 0x0001 */
  ARN5_LIF_MASK_t                          MASK;                          /* addr: 0x0002 */
  ARN5_LIF_INTERRUPT_t                     INTERRUPT;                     /* addr: 0x0003 */
  ARN5_LIF_GP_PROV_t                       GP_PROV;                       /* addr: 0x0004 */
  ARN5_LIF_GP_CTRL_t                       GP_CTRL;                       /* addr: 0x0005 */
  ARN5_LIF_PRBS_PROV_t                     PRBS_PROV;                     /* addr: 0x0006 */
  ARN5_LIF_PRBS_CTRL_t                     PRBS_CTRL;                     /* addr: 0x0007 */
  ARN5_LIF_PRBS_CNT_MSB_t                  PRBS_CNT_MSB;                  /* addr: 0x0008 */
  ARN5_LIF_PRBS_CNT_LSB_t                  PRBS_CNT_LSB;                  /* addr: 0x0009 */
  ARN5_LIF_RX_LOCK_OFFSET_t                RX_LOCK_OFFSET;                /* addr: 0x000a */
  ARN5_LIF_RX_LOCK_THRESHOLD_t             RX_LOCK_THRESHOLD;             /* addr: 0x000b */
  ARN5_LIF_RX_LOCK_RESOL_t                 RX_LOCK_RESOL;                 /* addr: 0x000c */
  cs_uint16                                rsrvd1[2];
  ARN5_LIF_ESTORE_RESYNC_t                 ESTORE_RESYNC;                 /* addr: 0x000f */
  ARN5_LIF_CLOCK_ENABLE_t                  CLOCK_ENABLE;                  /* addr: 0x0010 */
  ARN5_LIF_TIMING_CTRL_t                   TIMING_CTRL;                   /* addr: 0x0011 */
  ARN5_LIF_CLKOUT_CTRL_t                   CLKOUT_CTRL;                   /* addr: 0x0012 */
  ARN5_LIF_RX_DATA_CTRL_t                  RX_DATA_CTRL;                  /* addr: 0x0013 */
  ARN5_LIF_RX_CHARGE_PUMP_t                RX_CHARGE_PUMP;                /* addr: 0x0014 */
  ARN5_LIF_RX_LOOP_FILTER_t                RX_LOOP_FILTER;                /* addr: 0x0015 */
  ARN5_LIF_SERDES_CONFIGURATION_t          SERDES_CONFIGURATION;          /* addr: 0x0016 */
  ARN5_LIF_RX_VCO_TUNE1_t                  RX_VCO_TUNE1;                  /* addr: 0x0017 */
  ARN5_LIF_TEST_CTRL_t                     TEST_CTRL;                     /* addr: 0x0018 */
  ARN5_LIF_TX_OUTPUT_CTRL_t                TX_OUTPUT_CTRL;                /* addr: 0x0019 */
  ARN5_LIF_TX_PLL_CTRL_t                   TX_PLL_CTRL;                   /* addr: 0x001a */
  ARN5_LIF_VCO_FREQ_t                      VCO_FREQ;                      /* addr: 0x001b */
  ARN5_LIF_SPARE_t                         SPARE;                         /* addr: 0x001c */
  ARN5_LIF_TX_VCO_TUNE_t                   TX_VCO_TUNE;                   /* addr: 0x001d */
  ARN5_LIF_STX_VCO_STATUS_t                STX_VCO_STATUS;                /* addr: 0x001e */
  ARN5_LIF_RX_VCO_TUNE2_t                  RX_VCO_TUNE2;                  /* addr: 0x001f */
} ARN5_LIF_t;

/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * PPRBS::pprbs
 */

#define ARN5_PPRBS_GenCntl0                      0x14000
#define ARN5_PPRBS_GenCntl0_dft                  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 err                  :  1 ; /* bits 15:15 */
    cs_uint16 atm                  :  1 ; /* bits 14:14 */
    cs_uint16 multicast            :  1 ; /* bits 13:13 */
    cs_uint16 ignore_bp            :  1 ; /* bits 12:12 */
    cs_uint16 fixed_format         :  1 ; /* bits 11:11 */
    cs_uint16 sz_val               :  3 ; /* bits 10:8 */
    cs_uint16 chan                 :  2 ; /* bits 7:6 */
    cs_uint16 sz_disable           :  1 ; /* bits 5:5 */
    cs_uint16 rewrite              :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 sys_loopbk           :  1 ; /* bits 1:1 */
    cs_uint16 enable               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 enable               :  1 ; /* bits 0:0 */
    cs_uint16 sys_loopbk           :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 rewrite              :  1 ; /* bits 4:4 */
    cs_uint16 sz_disable           :  1 ; /* bits 5:5 */
    cs_uint16 chan                 :  2 ; /* bits 7:6 */
    cs_uint16 sz_val               :  3 ; /* bits 10:8 */
    cs_uint16 fixed_format         :  1 ; /* bits 11:11 */
    cs_uint16 ignore_bp            :  1 ; /* bits 12:12 */
    cs_uint16 multicast            :  1 ; /* bits 13:13 */
    cs_uint16 atm                  :  1 ; /* bits 14:14 */
    cs_uint16 err                  :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenCntl0_t;

#define ARN5_PPRBS_GenIPG0                       0x14001
#define ARN5_PPRBS_GenIPG0_dft                   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 ipg                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ipg                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenIPG0_t;

#define ARN5_PPRBS_GenMinLen0                    0x14002
#define ARN5_PPRBS_GenMinLen0_dft                0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 min_len              : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 min_len              : 11 ; /* bits 10:0 */
    cs_uint16 rsrvd1               :  5 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenMinLen0_t;

#define ARN5_PPRBS_GenMaxLen0                    0x14003
#define ARN5_PPRBS_GenMaxLen0_dft                0x007f
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 max_len              : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 max_len              : 11 ; /* bits 10:0 */
    cs_uint16 rsrvd1               :  5 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenMaxLen0_t;

#define ARN5_PPRBS_GenNum0                       0x14004
#define ARN5_PPRBS_GenNum0_dft                   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 num                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 num                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenNum0_t;

#define ARN5_PPRBS_GenErr0                       0x14005
#define ARN5_PPRBS_GenErr0_dft                   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 cont_err             :  1 ; /* bits 1:1 */
    cs_uint16 single_err           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 single_err           :  1 ; /* bits 0:0 */
    cs_uint16 cont_err             :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenErr0_t;

#define ARN5_PPRBS_ChkCntl0                      0x14006
#define ARN5_PPRBS_ChkCntl0_dft                  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 resync               :  1 ; /* bits 15:15 */
    cs_uint16 atm                  :  1 ; /* bits 14:14 */
    cs_uint16 rsrvd3               :  2 ;
    cs_uint16 fixed_format         :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 chan                 :  2 ; /* bits 7:6 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 enable               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 enable               :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 chan                 :  2 ; /* bits 7:6 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 fixed_format         :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd3               :  2 ;
    cs_uint16 atm                  :  1 ; /* bits 14:14 */
    cs_uint16 resync               :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_ChkCntl0_t;

#define ARN5_PPRBS_ChkSync0                      0x14007
#define ARN5_PPRBS_ChkSync0_dft                  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 sync                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 sync                 :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_ChkSync0_t;

#define ARN5_PPRBS_ChkErr0                       0x14008
#define ARN5_PPRBS_ChkErr0_dft                   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 error                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 error                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_ChkErr0_t;

#define ARN5_PPRBS_ChkByteMSB0                   0x14009
#define ARN5_PPRBS_ChkByteMSB0_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_ChkByteMSB0_t;

#define ARN5_PPRBS_ChkByteLSB0                   0x1400a
#define ARN5_PPRBS_ChkByteLSB0_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_ChkByteLSB0_t;

#define ARN5_PPRBS_ChkPacketMSB0                 0x1400b
#define ARN5_PPRBS_ChkPacketMSB0_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_ChkPacketMSB0_t;

#define ARN5_PPRBS_ChkPacketLSB0                 0x1400c
#define ARN5_PPRBS_ChkPacketLSB0_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_ChkPacketLSB0_t;

#define ARN5_PPRBS_GenByteMSB0                   0x1400d
#define ARN5_PPRBS_GenByteMSB0_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenByteMSB0_t;

#define ARN5_PPRBS_GenByteLSB0                   0x1400e
#define ARN5_PPRBS_GenByteLSB0_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenByteLSB0_t;

#define ARN5_PPRBS_GenPacketMSB0                 0x1400f
#define ARN5_PPRBS_GenPacketMSB0_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenPacketMSB0_t;

#define ARN5_PPRBS_GenPacketLSB0                 0x14010
#define ARN5_PPRBS_GenPacketLSB0_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenPacketLSB0_t;

#define ARN5_PPRBS_GenCntl1                      0x14011
#define ARN5_PPRBS_GenCntl1_dft                  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 err                  :  1 ; /* bits 15:15 */
    cs_uint16 atm                  :  1 ; /* bits 14:14 */
    cs_uint16 multicast            :  1 ; /* bits 13:13 */
    cs_uint16 ignore_bp            :  1 ; /* bits 12:12 */
    cs_uint16 fixed_format         :  1 ; /* bits 11:11 */
    cs_uint16 sz_val               :  3 ; /* bits 10:8 */
    cs_uint16 chan                 :  2 ; /* bits 7:6 */
    cs_uint16 sz_disable           :  1 ; /* bits 5:5 */
    cs_uint16 rewrite              :  1 ; /* bits 4:4 */
    cs_uint16 gfp                  :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 line_loopbk          :  1 ; /* bits 1:1 */
    cs_uint16 enable               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 enable               :  1 ; /* bits 0:0 */
    cs_uint16 line_loopbk          :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 gfp                  :  1 ; /* bits 3:3 */
    cs_uint16 rewrite              :  1 ; /* bits 4:4 */
    cs_uint16 sz_disable           :  1 ; /* bits 5:5 */
    cs_uint16 chan                 :  2 ; /* bits 7:6 */
    cs_uint16 sz_val               :  3 ; /* bits 10:8 */
    cs_uint16 fixed_format         :  1 ; /* bits 11:11 */
    cs_uint16 ignore_bp            :  1 ; /* bits 12:12 */
    cs_uint16 multicast            :  1 ; /* bits 13:13 */
    cs_uint16 atm                  :  1 ; /* bits 14:14 */
    cs_uint16 err                  :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenCntl1_t;

#define ARN5_PPRBS_GenIPG1                       0x14012
#define ARN5_PPRBS_GenIPG1_dft                   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 ipg                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ipg                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenIPG1_t;

#define ARN5_PPRBS_GenMinLen1                    0x14013
#define ARN5_PPRBS_GenMinLen1_dft                0x0005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 min_len              : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 min_len              : 11 ; /* bits 10:0 */
    cs_uint16 rsrvd1               :  5 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenMinLen1_t;

#define ARN5_PPRBS_GenMaxLen1                    0x14014
#define ARN5_PPRBS_GenMaxLen1_dft                0x007f
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 max_len              : 11 ; /* bits 10:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 max_len              : 11 ; /* bits 10:0 */
    cs_uint16 rsrvd1               :  5 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenMaxLen1_t;

#define ARN5_PPRBS_GenNum1                       0x14015
#define ARN5_PPRBS_GenNum1_dft                   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 num                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 num                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenNum1_t;

#define ARN5_PPRBS_GenErr1                       0x14016
#define ARN5_PPRBS_GenErr1_dft                   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 cont_err             :  1 ; /* bits 1:1 */
    cs_uint16 single_err           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 single_err           :  1 ; /* bits 0:0 */
    cs_uint16 cont_err             :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenErr1_t;

#define ARN5_PPRBS_ChkCntl1                      0x14017
#define ARN5_PPRBS_ChkCntl1_dft                  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 resync               :  1 ; /* bits 15:15 */
    cs_uint16 atm                  :  1 ; /* bits 14:14 */
    cs_uint16 rsrvd3               :  2 ;
    cs_uint16 fixed_format         :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 chan                 :  2 ; /* bits 7:6 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 enable               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 enable               :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 chan                 :  2 ; /* bits 7:6 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 fixed_format         :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd3               :  2 ;
    cs_uint16 atm                  :  1 ; /* bits 14:14 */
    cs_uint16 resync               :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_ChkCntl1_t;

#define ARN5_PPRBS_ChkSync1                      0x14018
#define ARN5_PPRBS_ChkSync1_dft                  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 sync                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 sync                 :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_ChkSync1_t;

#define ARN5_PPRBS_ChkErr1                       0x14019
#define ARN5_PPRBS_ChkErr1_dft                   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 error                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 error                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_ChkErr1_t;

#define ARN5_PPRBS_ChkByteMSB1                   0x1401a
#define ARN5_PPRBS_ChkByteMSB1_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_ChkByteMSB1_t;

#define ARN5_PPRBS_ChkByteLSB1                   0x1401b
#define ARN5_PPRBS_ChkByteLSB1_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_ChkByteLSB1_t;

#define ARN5_PPRBS_ChkPacketMSB1                 0x1401c
#define ARN5_PPRBS_ChkPacketMSB1_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_ChkPacketMSB1_t;

#define ARN5_PPRBS_ChkPacketLSB1                 0x1401d
#define ARN5_PPRBS_ChkPacketLSB1_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_ChkPacketLSB1_t;

#define ARN5_PPRBS_GenByteMSB1                   0x1401e
#define ARN5_PPRBS_GenByteMSB1_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenByteMSB1_t;

#define ARN5_PPRBS_GenByteLSB1                   0x1401f
#define ARN5_PPRBS_GenByteLSB1_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenByteLSB1_t;

#define ARN5_PPRBS_GenPacketMSB1                 0x14020
#define ARN5_PPRBS_GenPacketMSB1_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenPacketMSB1_t;

#define ARN5_PPRBS_GenPacketLSB1                 0x14021
#define ARN5_PPRBS_GenPacketLSB1_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_GenPacketLSB1_t;

#define ARN5_PPRBS_Interrupt                     0x14022
#define ARN5_PPRBS_Interrupt_dft                 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 RolloverTxPacket1I   :  1 ; /* bits 15:15 */
    cs_uint16 RolloverTxByte1I     :  1 ; /* bits 14:14 */
    cs_uint16 SentPackets1I        :  1 ; /* bits 13:13 */
    cs_uint16 RolloverPacket1I     :  1 ; /* bits 12:12 */
    cs_uint16 RolloverByte1I       :  1 ; /* bits 11:11 */
    cs_uint16 Rollover1I           :  1 ; /* bits 10:10 */
    cs_uint16 BitError1I           :  1 ; /* bits 9:9 */
    cs_uint16 SyncState1I          :  1 ; /* bits 8:8 */
    cs_uint16 RolloverTxPacket0I   :  1 ; /* bits 7:7 */
    cs_uint16 RolloverTxByte0I     :  1 ; /* bits 6:6 */
    cs_uint16 SentPackets0I        :  1 ; /* bits 5:5 */
    cs_uint16 RolloverPacket0I     :  1 ; /* bits 4:4 */
    cs_uint16 RolloverByte0I       :  1 ; /* bits 3:3 */
    cs_uint16 Rollover0I           :  1 ; /* bits 2:2 */
    cs_uint16 BitError0I           :  1 ; /* bits 1:1 */
    cs_uint16 SyncState0I          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SyncState0I          :  1 ; /* bits 0:0 */
    cs_uint16 BitError0I           :  1 ; /* bits 1:1 */
    cs_uint16 Rollover0I           :  1 ; /* bits 2:2 */
    cs_uint16 RolloverByte0I       :  1 ; /* bits 3:3 */
    cs_uint16 RolloverPacket0I     :  1 ; /* bits 4:4 */
    cs_uint16 SentPackets0I        :  1 ; /* bits 5:5 */
    cs_uint16 RolloverTxByte0I     :  1 ; /* bits 6:6 */
    cs_uint16 RolloverTxPacket0I   :  1 ; /* bits 7:7 */
    cs_uint16 SyncState1I          :  1 ; /* bits 8:8 */
    cs_uint16 BitError1I           :  1 ; /* bits 9:9 */
    cs_uint16 Rollover1I           :  1 ; /* bits 10:10 */
    cs_uint16 RolloverByte1I       :  1 ; /* bits 11:11 */
    cs_uint16 RolloverPacket1I     :  1 ; /* bits 12:12 */
    cs_uint16 SentPackets1I        :  1 ; /* bits 13:13 */
    cs_uint16 RolloverTxByte1I     :  1 ; /* bits 14:14 */
    cs_uint16 RolloverTxPacket1I   :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_Interrupt_t;

#define ARN5_PPRBS_IntMask                       0x14023
#define ARN5_PPRBS_IntMask_dft                   0xffff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 RolloverTxPacket1M   :  1 ; /* bits 15:15 */
    cs_uint16 RolloverTxByte1M     :  1 ; /* bits 14:14 */
    cs_uint16 SentPackets1M        :  1 ; /* bits 13:13 */
    cs_uint16 RolloverPacket1M     :  1 ; /* bits 12:12 */
    cs_uint16 RolloverByte1M       :  1 ; /* bits 11:11 */
    cs_uint16 Rollover1M           :  1 ; /* bits 10:10 */
    cs_uint16 BitError1M           :  1 ; /* bits 9:9 */
    cs_uint16 SyncState1M          :  1 ; /* bits 8:8 */
    cs_uint16 RolloverTxPacket0M   :  1 ; /* bits 7:7 */
    cs_uint16 RolloverTxByte0M     :  1 ; /* bits 6:6 */
    cs_uint16 SentPackets0M        :  1 ; /* bits 5:5 */
    cs_uint16 RolloverPacket0M     :  1 ; /* bits 4:4 */
    cs_uint16 RolloverByte0M       :  1 ; /* bits 3:3 */
    cs_uint16 Rollover0M           :  1 ; /* bits 2:2 */
    cs_uint16 BitError0M           :  1 ; /* bits 1:1 */
    cs_uint16 SyncState0M          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SyncState0M          :  1 ; /* bits 0:0 */
    cs_uint16 BitError0M           :  1 ; /* bits 1:1 */
    cs_uint16 Rollover0M           :  1 ; /* bits 2:2 */
    cs_uint16 RolloverByte0M       :  1 ; /* bits 3:3 */
    cs_uint16 RolloverPacket0M     :  1 ; /* bits 4:4 */
    cs_uint16 SentPackets0M        :  1 ; /* bits 5:5 */
    cs_uint16 RolloverTxByte0M     :  1 ; /* bits 6:6 */
    cs_uint16 RolloverTxPacket0M   :  1 ; /* bits 7:7 */
    cs_uint16 SyncState1M          :  1 ; /* bits 8:8 */
    cs_uint16 BitError1M           :  1 ; /* bits 9:9 */
    cs_uint16 Rollover1M           :  1 ; /* bits 10:10 */
    cs_uint16 RolloverByte1M       :  1 ; /* bits 11:11 */
    cs_uint16 RolloverPacket1M     :  1 ; /* bits 12:12 */
    cs_uint16 SentPackets1M        :  1 ; /* bits 13:13 */
    cs_uint16 RolloverTxByte1M     :  1 ; /* bits 14:14 */
    cs_uint16 RolloverTxPacket1M   :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_PPRBS_IntMask_t;


/******************************************************************
 *
 * Block declaration.
 *
 ******************************************************************/

/* PPRBS */

#define ARN5_PPRBS_STRIDE                        131072
#define ARN5_PPRBS_COUNT                         8
typedef struct {
  ARN5_PPRBS_GenCntl0_t                    GenCntl0;                      /* addr: 0x0000 */
  ARN5_PPRBS_GenIPG0_t                     GenIPG0;                       /* addr: 0x0001 */
  ARN5_PPRBS_GenMinLen0_t                  GenMinLen0;                    /* addr: 0x0002 */
  ARN5_PPRBS_GenMaxLen0_t                  GenMaxLen0;                    /* addr: 0x0003 */
  ARN5_PPRBS_GenNum0_t                     GenNum0;                       /* addr: 0x0004 */
  ARN5_PPRBS_GenErr0_t                     GenErr0;                       /* addr: 0x0005 */
  ARN5_PPRBS_ChkCntl0_t                    ChkCntl0;                      /* addr: 0x0006 */
  ARN5_PPRBS_ChkSync0_t                    ChkSync0;                      /* addr: 0x0007 */
  ARN5_PPRBS_ChkErr0_t                     ChkErr0;                       /* addr: 0x0008 */
  ARN5_PPRBS_ChkByteMSB0_t                 ChkByteMSB0;                   /* addr: 0x0009 */
  ARN5_PPRBS_ChkByteLSB0_t                 ChkByteLSB0;                   /* addr: 0x000a */
  ARN5_PPRBS_ChkPacketMSB0_t               ChkPacketMSB0;                 /* addr: 0x000b */
  ARN5_PPRBS_ChkPacketLSB0_t               ChkPacketLSB0;                 /* addr: 0x000c */
  ARN5_PPRBS_GenByteMSB0_t                 GenByteMSB0;                   /* addr: 0x000d */
  ARN5_PPRBS_GenByteLSB0_t                 GenByteLSB0;                   /* addr: 0x000e */
  ARN5_PPRBS_GenPacketMSB0_t               GenPacketMSB0;                 /* addr: 0x000f */
  ARN5_PPRBS_GenPacketLSB0_t               GenPacketLSB0;                 /* addr: 0x0010 */
  ARN5_PPRBS_GenCntl1_t                    GenCntl1;                      /* addr: 0x0011 */
  ARN5_PPRBS_GenIPG1_t                     GenIPG1;                       /* addr: 0x0012 */
  ARN5_PPRBS_GenMinLen1_t                  GenMinLen1;                    /* addr: 0x0013 */
  ARN5_PPRBS_GenMaxLen1_t                  GenMaxLen1;                    /* addr: 0x0014 */
  ARN5_PPRBS_GenNum1_t                     GenNum1;                       /* addr: 0x0015 */
  ARN5_PPRBS_GenErr1_t                     GenErr1;                       /* addr: 0x0016 */
  ARN5_PPRBS_ChkCntl1_t                    ChkCntl1;                      /* addr: 0x0017 */
  ARN5_PPRBS_ChkSync1_t                    ChkSync1;                      /* addr: 0x0018 */
  ARN5_PPRBS_ChkErr1_t                     ChkErr1;                       /* addr: 0x0019 */
  ARN5_PPRBS_ChkByteMSB1_t                 ChkByteMSB1;                   /* addr: 0x001a */
  ARN5_PPRBS_ChkByteLSB1_t                 ChkByteLSB1;                   /* addr: 0x001b */
  ARN5_PPRBS_ChkPacketMSB1_t               ChkPacketMSB1;                 /* addr: 0x001c */
  ARN5_PPRBS_ChkPacketLSB1_t               ChkPacketLSB1;                 /* addr: 0x001d */
  ARN5_PPRBS_GenByteMSB1_t                 GenByteMSB1;                   /* addr: 0x001e */
  ARN5_PPRBS_GenByteLSB1_t                 GenByteLSB1;                   /* addr: 0x001f */
  ARN5_PPRBS_GenPacketMSB1_t               GenPacketMSB1;                 /* addr: 0x0020 */
  ARN5_PPRBS_GenPacketLSB1_t               GenPacketLSB1;                 /* addr: 0x0021 */
  ARN5_PPRBS_Interrupt_t                   Interrupt;                     /* addr: 0x0022 */
  ARN5_PPRBS_IntMask_t                     IntMask;                       /* addr: 0x0023 */
} ARN5_PPRBS_t;

/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * ecore::mac
 */

#define ARN5_GIGE_EI_ECORE_MAC_REV1              0x16000
#define ARN5_GIGE_EI_ECORE_MAC_REV1_dft          0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Rev                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Rev                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_REV1_t;

#define ARN5_GIGE_EI_ECORE_MAC_REV0              0x16001
#define ARN5_GIGE_EI_ECORE_MAC_REV0_dft          0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Rev                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Rev                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_REV0_t;

#define ARN5_GIGE_EI_ECORE_MAC_SCRATCH1          0x16002
#define ARN5_GIGE_EI_ECORE_MAC_SCRATCH1_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Scratch              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Scratch              : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_SCRATCH1_t;

#define ARN5_GIGE_EI_ECORE_MAC_SCRATCH0          0x16003
#define ARN5_GIGE_EI_ECORE_MAC_SCRATCH0_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Scratch              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Scratch              : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_SCRATCH0_t;

#define ARN5_GIGE_EI_ECORE_MAC_COMMAND_CONFIG1   0x16004
#define ARN5_GIGE_EI_ECORE_MAC_COMMAND_CONFIG1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  7 ;
    cs_uint16 NO_LGTH_CHECK        :  1 ; /* bits 8:8 */
    cs_uint16 CNTL_FRM_ENA         :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 WAKEUP               :  1 ; /* bits 5:5 */
    cs_uint16 SLEEP                :  1 ; /* bits 4:4 */
    cs_uint16 MAGIC_ENA            :  1 ; /* bits 3:3 */
    cs_uint16 TX_ADDR_SEL          :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TX_ADDR_SEL          :  3 ; /* bits 2:0 */
    cs_uint16 MAGIC_ENA            :  1 ; /* bits 3:3 */
    cs_uint16 SLEEP                :  1 ; /* bits 4:4 */
    cs_uint16 WAKEUP               :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 CNTL_FRM_ENA         :  1 ; /* bits 7:7 */
    cs_uint16 NO_LGTH_CHECK        :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_COMMAND_CONFIG1_t;
#define     ARN5_TX_ADDR_SEL_MAC                          0x00
#define     ARN5_TX_ADDR_SEL_SMAC0                        0x04
#define     ARN5_TX_ADDR_SEL_SMAC1                        0x05
#define     ARN5_TX_ADDR_SEL_SMAC2                        0x06
#define     ARN5_TX_ADDR_SEL_SMAC3                        0x07

#define ARN5_GIGE_EI_ECORE_MAC_COMMAND_CONFIG0   0x16005
#define ARN5_GIGE_EI_ECORE_MAC_COMMAND_CONFIG0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 LOOP_ENA             :  1 ; /* bits 15:15 */
    cs_uint16 MHASH_SEL            :  1 ; /* bits 14:14 */
    cs_uint16 SW_RESET             :  1 ; /* bits 13:13 */
    cs_uint16 rsrvd3               :  3 ;
    cs_uint16 TX_ADDR_INS          :  1 ; /* bits 9:9 */
    cs_uint16 PAUSE_IGNORE         :  1 ; /* bits 8:8 */
    cs_uint16 PAUSE_FWD            :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 PAD_EN               :  1 ; /* bits 5:5 */
    cs_uint16 PROMIS_EN            :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 RX_ENA               :  1 ; /* bits 1:1 */
    cs_uint16 TX_ENA               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TX_ENA               :  1 ; /* bits 0:0 */
    cs_uint16 RX_ENA               :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 PROMIS_EN            :  1 ; /* bits 4:4 */
    cs_uint16 PAD_EN               :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 PAUSE_FWD            :  1 ; /* bits 7:7 */
    cs_uint16 PAUSE_IGNORE         :  1 ; /* bits 8:8 */
    cs_uint16 TX_ADDR_INS          :  1 ; /* bits 9:9 */
    cs_uint16 rsrvd3               :  3 ;
    cs_uint16 SW_RESET             :  1 ; /* bits 13:13 */
    cs_uint16 MHASH_SEL            :  1 ; /* bits 14:14 */
    cs_uint16 LOOP_ENA             :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_COMMAND_CONFIG0_t;

#define ARN5_GIGE_EI_ECORE_MAC_MAC_ADDR_L1       0x16006
#define ARN5_GIGE_EI_ECORE_MAC_MAC_ADDR_L1_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_MAC_ADDR_L1_t;

#define ARN5_GIGE_EI_ECORE_MAC_MAC_ADDR_L0       0x16007
#define ARN5_GIGE_EI_ECORE_MAC_MAC_ADDR_L0_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_MAC_ADDR_L0_t;

#define ARN5_GIGE_EI_ECORE_MAC_MAC_ADDR_U1       0x16008
#define ARN5_GIGE_EI_ECORE_MAC_MAC_ADDR_U1_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_MAC_ADDR_U1_t;

#define ARN5_GIGE_EI_ECORE_MAC_MAC_ADDR_U0       0x16009
#define ARN5_GIGE_EI_ECORE_MAC_MAC_ADDR_U0_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_MAC_ADDR_U0_t;

#define ARN5_GIGE_EI_ECORE_MAC_FRM_LENGTH1       0x1600a
#define ARN5_GIGE_EI_ECORE_MAC_FRM_LENGTH1_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_FRM_LENGTH1_t;

#define ARN5_GIGE_EI_ECORE_MAC_FRM_LENGTH0       0x1600b
#define ARN5_GIGE_EI_ECORE_MAC_FRM_LENGTH0_dft   0x05f2
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 FRM_LENGTH           : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FRM_LENGTH           : 14 ; /* bits 13:0 */
    cs_uint16 rsrvd1               :  2 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_FRM_LENGTH0_t;

#define ARN5_GIGE_EI_ECORE_MAC_PAUSE_QUANT1      0x1600c
#define ARN5_GIGE_EI_ECORE_MAC_PAUSE_QUANT1_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_PAUSE_QUANT1_t;

#define ARN5_GIGE_EI_ECORE_MAC_PAUSE_QUANT0      0x1600d
#define ARN5_GIGE_EI_ECORE_MAC_PAUSE_QUANT0_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 PAUSE_QUANT          : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PAUSE_QUANT          : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_PAUSE_QUANT0_t;

#define ARN5_GIGE_EI_ECORE_MAC_RX_SECTION_EMPTY1 0x1600e
#define ARN5_GIGE_EI_ECORE_MAC_RX_SECTION_EMPTY1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_RX_SECTION_EMPTY1_t;

#define ARN5_GIGE_EI_ECORE_MAC_RX_SECTION_EMPTY0 0x1600f
#define ARN5_GIGE_EI_ECORE_MAC_RX_SECTION_EMPTY0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 RX_SECTION_EMPTY     :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RX_SECTION_EMPTY     :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_RX_SECTION_EMPTY0_t;

#define ARN5_GIGE_EI_ECORE_MAC_RX_SECTION_FULL1  0x16010
#define ARN5_GIGE_EI_ECORE_MAC_RX_SECTION_FULL1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_RX_SECTION_FULL1_t;

#define ARN5_GIGE_EI_ECORE_MAC_RX_SECTION_FULL0  0x16011
#define ARN5_GIGE_EI_ECORE_MAC_RX_SECTION_FULL0_dft 0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 RX_SECTION_FULL      :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RX_SECTION_FULL      :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_RX_SECTION_FULL0_t;

#define ARN5_GIGE_EI_ECORE_MAC_TX_SECTION_EMPTY1 0x16012
#define ARN5_GIGE_EI_ECORE_MAC_TX_SECTION_EMPTY1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_TX_SECTION_EMPTY1_t;

#define ARN5_GIGE_EI_ECORE_MAC_TX_SECTION_EMPTY0 0x16013
#define ARN5_GIGE_EI_ECORE_MAC_TX_SECTION_EMPTY0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 TX_SECTION_EMPTY     :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TX_SECTION_EMPTY     :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_TX_SECTION_EMPTY0_t;

#define ARN5_GIGE_EI_ECORE_MAC_TX_SECTION_FULL1  0x16014
#define ARN5_GIGE_EI_ECORE_MAC_TX_SECTION_FULL1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_TX_SECTION_FULL1_t;

#define ARN5_GIGE_EI_ECORE_MAC_TX_SECTION_FULL0  0x16015
#define ARN5_GIGE_EI_ECORE_MAC_TX_SECTION_FULL0_dft 0x000a
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 TX_SECTION_FULL      :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TX_SECTION_FULL      :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_TX_SECTION_FULL0_t;

#define ARN5_GIGE_EI_ECORE_MAC_RX_ALMOST_EMPTY1  0x16016
#define ARN5_GIGE_EI_ECORE_MAC_RX_ALMOST_EMPTY1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_RX_ALMOST_EMPTY1_t;

#define ARN5_GIGE_EI_ECORE_MAC_RX_ALMOST_EMPTY0  0x16017
#define ARN5_GIGE_EI_ECORE_MAC_RX_ALMOST_EMPTY0_dft 0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 RX_ALMOST_EMPTY      :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RX_ALMOST_EMPTY      :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_RX_ALMOST_EMPTY0_t;

#define ARN5_GIGE_EI_ECORE_MAC_RX_ALMOST_FULL1   0x16018
#define ARN5_GIGE_EI_ECORE_MAC_RX_ALMOST_FULL1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_RX_ALMOST_FULL1_t;

#define ARN5_GIGE_EI_ECORE_MAC_RX_ALMOST_FULL0   0x16019
#define ARN5_GIGE_EI_ECORE_MAC_RX_ALMOST_FULL0_dft 0x0002
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 RX_ALMOST_FULL       :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RX_ALMOST_FULL       :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_RX_ALMOST_FULL0_t;

#define ARN5_GIGE_EI_ECORE_MAC_TX_ALMOST_EMPTY1  0x1601a
#define ARN5_GIGE_EI_ECORE_MAC_TX_ALMOST_EMPTY1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_TX_ALMOST_EMPTY1_t;

#define ARN5_GIGE_EI_ECORE_MAC_TX_ALMOST_EMPTY0  0x1601b
#define ARN5_GIGE_EI_ECORE_MAC_TX_ALMOST_EMPTY0_dft 0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 TX_ALMOST_EMPTY      :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TX_ALMOST_EMPTY      :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_TX_ALMOST_EMPTY0_t;

#define ARN5_GIGE_EI_ECORE_MAC_TX_ALMOST_FULL1   0x1601c
#define ARN5_GIGE_EI_ECORE_MAC_TX_ALMOST_FULL1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_TX_ALMOST_FULL1_t;

#define ARN5_GIGE_EI_ECORE_MAC_TX_ALMOST_FULL0   0x1601d
#define ARN5_GIGE_EI_ECORE_MAC_TX_ALMOST_FULL0_dft 0x000c
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 TX_ALMOST_FULL       :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TX_ALMOST_FULL       :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_TX_ALMOST_FULL0_t;

#define ARN5_GIGE_EI_ECORE_MAC_aMacID_L1         0x16030
#define ARN5_GIGE_EI_ECORE_MAC_aMacID_L1_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aMacID_L1_t;

#define ARN5_GIGE_EI_ECORE_MAC_aMacID_L0         0x16031
#define ARN5_GIGE_EI_ECORE_MAC_aMacID_L0_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aMacID_L0_t;

#define ARN5_GIGE_EI_ECORE_MAC_aMacID_U1         0x16032
#define ARN5_GIGE_EI_ECORE_MAC_aMacID_U1_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 fixed                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aMacID_U1_t;

#define ARN5_GIGE_EI_ECORE_MAC_aMacID_U0         0x16033
#define ARN5_GIGE_EI_ECORE_MAC_aMacID_U0_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aMacID_U0_t;

#define ARN5_GIGE_EI_ECORE_MAC_aFramesTransmittedOK1 0x16034
#define ARN5_GIGE_EI_ECORE_MAC_aFramesTransmittedOK1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aFramesTransmittedOK1_t;

#define ARN5_GIGE_EI_ECORE_MAC_aFramesTransmittedOK0 0x16035
#define ARN5_GIGE_EI_ECORE_MAC_aFramesTransmittedOK0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aFramesTransmittedOK0_t;

#define ARN5_GIGE_EI_ECORE_MAC_aFramesReceivedOK1 0x16036
#define ARN5_GIGE_EI_ECORE_MAC_aFramesReceivedOK1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aFramesReceivedOK1_t;

#define ARN5_GIGE_EI_ECORE_MAC_aFramesReceivedOK0 0x16037
#define ARN5_GIGE_EI_ECORE_MAC_aFramesReceivedOK0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aFramesReceivedOK0_t;

#define ARN5_GIGE_EI_ECORE_MAC_aFrameCheckSequenceErrors1 0x16038
#define ARN5_GIGE_EI_ECORE_MAC_aFrameCheckSequenceErrors1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aFrameCheckSequenceErrors1_t;

#define ARN5_GIGE_EI_ECORE_MAC_aFrameCheckSequenceErrors0 0x16039
#define ARN5_GIGE_EI_ECORE_MAC_aFrameCheckSequenceErrors0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aFrameCheckSequenceErrors0_t;

#define ARN5_GIGE_EI_ECORE_MAC_aAlignmentErrors1 0x1603a
#define ARN5_GIGE_EI_ECORE_MAC_aAlignmentErrors1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aAlignmentErrors1_t;

#define ARN5_GIGE_EI_ECORE_MAC_aAlignmentErrors0 0x1603b
#define ARN5_GIGE_EI_ECORE_MAC_aAlignmentErrors0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aAlignmentErrors0_t;

#define ARN5_GIGE_EI_ECORE_MAC_aOctetsTransmittedOK1 0x1603c
#define ARN5_GIGE_EI_ECORE_MAC_aOctetsTransmittedOK1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aOctetsTransmittedOK1_t;

#define ARN5_GIGE_EI_ECORE_MAC_aOctetsTransmittedOK0 0x1603d
#define ARN5_GIGE_EI_ECORE_MAC_aOctetsTransmittedOK0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aOctetsTransmittedOK0_t;

#define ARN5_GIGE_EI_ECORE_MAC_aOctetsReceivedOK1 0x1603e
#define ARN5_GIGE_EI_ECORE_MAC_aOctetsReceivedOK1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aOctetsReceivedOK1_t;

#define ARN5_GIGE_EI_ECORE_MAC_aOctetsReceivedOK0 0x1603f
#define ARN5_GIGE_EI_ECORE_MAC_aOctetsReceivedOK0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aOctetsReceivedOK0_t;

#define ARN5_GIGE_EI_ECORE_MAC_aTxPauseMacCtrlFrames1 0x16040
#define ARN5_GIGE_EI_ECORE_MAC_aTxPauseMacCtrlFrames1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aTxPauseMacCtrlFrames1_t;

#define ARN5_GIGE_EI_ECORE_MAC_aTxPauseMacCtrlFrames0 0x16041
#define ARN5_GIGE_EI_ECORE_MAC_aTxPauseMacCtrlFrames0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aTxPauseMacCtrlFrames0_t;

#define ARN5_GIGE_EI_ECORE_MAC_aRxPauseMacCtrlFrames1 0x16042
#define ARN5_GIGE_EI_ECORE_MAC_aRxPauseMacCtrlFrames1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aRxPauseMacCtrlFrames1_t;

#define ARN5_GIGE_EI_ECORE_MAC_aRxPauseMacCtrlFrames0 0x16043
#define ARN5_GIGE_EI_ECORE_MAC_aRxPauseMacCtrlFrames0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aRxPauseMacCtrlFrames0_t;

#define ARN5_GIGE_EI_ECORE_MAC_ifInErrors1       0x16044
#define ARN5_GIGE_EI_ECORE_MAC_ifInErrors1_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_ifInErrors1_t;

#define ARN5_GIGE_EI_ECORE_MAC_ifInErrors0       0x16045
#define ARN5_GIGE_EI_ECORE_MAC_ifInErrors0_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_ifInErrors0_t;

#define ARN5_GIGE_EI_ECORE_MAC_ifOutErrors1      0x16046
#define ARN5_GIGE_EI_ECORE_MAC_ifOutErrors1_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_ifOutErrors1_t;

#define ARN5_GIGE_EI_ECORE_MAC_ifOutErrors0      0x16047
#define ARN5_GIGE_EI_ECORE_MAC_ifOutErrors0_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_ifOutErrors0_t;

#define ARN5_GIGE_EI_ECORE_MAC_ifInUcastPkts1    0x16048
#define ARN5_GIGE_EI_ECORE_MAC_ifInUcastPkts1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_ifInUcastPkts1_t;

#define ARN5_GIGE_EI_ECORE_MAC_ifInUcastPkts0    0x16049
#define ARN5_GIGE_EI_ECORE_MAC_ifInUcastPkts0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_ifInUcastPkts0_t;

#define ARN5_GIGE_EI_ECORE_MAC_ifInMulticastPkts1 0x1604a
#define ARN5_GIGE_EI_ECORE_MAC_ifInMulticastPkts1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_ifInMulticastPkts1_t;

#define ARN5_GIGE_EI_ECORE_MAC_ifInMulticastPkts0 0x1604b
#define ARN5_GIGE_EI_ECORE_MAC_ifInMulticastPkts0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_ifInMulticastPkts0_t;

#define ARN5_GIGE_EI_ECORE_MAC_ifInBroadcastPkts1 0x1604c
#define ARN5_GIGE_EI_ECORE_MAC_ifInBroadcastPkts1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_ifInBroadcastPkts1_t;

#define ARN5_GIGE_EI_ECORE_MAC_ifInBroadcastPkts0 0x1604d
#define ARN5_GIGE_EI_ECORE_MAC_ifInBroadcastPkts0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_ifInBroadcastPkts0_t;

#define ARN5_GIGE_EI_ECORE_MAC_ifOutDiscards1    0x1604e
#define ARN5_GIGE_EI_ECORE_MAC_ifOutDiscards1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_ifOutDiscards1_t;

#define ARN5_GIGE_EI_ECORE_MAC_ifOutDiscards0    0x1604f
#define ARN5_GIGE_EI_ECORE_MAC_ifOutDiscards0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_ifOutDiscards0_t;

#define ARN5_GIGE_EI_ECORE_MAC_ifOutUcastPkts1   0x16050
#define ARN5_GIGE_EI_ECORE_MAC_ifOutUcastPkts1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_ifOutUcastPkts1_t;

#define ARN5_GIGE_EI_ECORE_MAC_ifOutUcastPkts0   0x16051
#define ARN5_GIGE_EI_ECORE_MAC_ifOutUcastPkts0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_ifOutUcastPkts0_t;

#define ARN5_GIGE_EI_ECORE_MAC_ifOutMulticastPkts1 0x16052
#define ARN5_GIGE_EI_ECORE_MAC_ifOutMulticastPkts1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_ifOutMulticastPkts1_t;

#define ARN5_GIGE_EI_ECORE_MAC_ifOutMulticastPkts0 0x16053
#define ARN5_GIGE_EI_ECORE_MAC_ifOutMulticastPkts0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_ifOutMulticastPkts0_t;

#define ARN5_GIGE_EI_ECORE_MAC_ifOutBroadcastPkts1 0x16054
#define ARN5_GIGE_EI_ECORE_MAC_ifOutBroadcastPkts1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_ifOutBroadcastPkts1_t;

#define ARN5_GIGE_EI_ECORE_MAC_ifOutBroadcastPkts0 0x16055
#define ARN5_GIGE_EI_ECORE_MAC_ifOutBroadcastPkts0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_ifOutBroadcastPkts0_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsDropEvents1 0x16056
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsDropEvents1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsDropEvents1_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsDropEvents0 0x16057
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsDropEvents0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsDropEvents0_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsOctets1 0x16058
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsOctets1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsOctets1_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsOctets0 0x16059
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsOctets0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsOctets0_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1   0x1605a
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts0   0x1605b
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts0_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsUndersizePkts1 0x1605c
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsUndersizePkts1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsUndersizePkts1_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsUndersizePkts0 0x1605d
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsUndersizePkts0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsUndersizePkts0_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsOversizePkts1 0x1605e
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsOversizePkts1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsOversizePkts1_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsOversizePkts0 0x1605f
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsOversizePkts0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsOversizePkts0_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts64Octets1 0x16060
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts64Octets1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts64Octets1_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts64Octets0 0x16061
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts64Octets0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts64Octets0_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts65to127Octets1 0x16062
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts65to127Octets1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts65to127Octets1_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts65to127Octets0 0x16063
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts65to127Octets0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts65to127Octets0_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts128to255Octets1 0x16064
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts128to255Octets1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts128to255Octets1_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts128to255Octets0 0x16065
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts128to255Octets0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts128to255Octets0_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts256to511Octets1 0x16066
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts256to511Octets1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts256to511Octets1_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts256to511Octets0 0x16067
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts256to511Octets0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts256to511Octets0_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts512to1023Octets1 0x16068
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts512to1023Octets1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts512to1023Octets1_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts512to1023Octets0 0x16069
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts512to1023Octets0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts512to1023Octets0_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1024to1518Octets1 0x1606a
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1024to1518Octets1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1024to1518Octets1_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1024to1518Octets0 0x1606b
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1024to1518Octets0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1024to1518Octets0_t;

#define ARN5_GIGE_EI_ECORE_MAC_aSingleCollisionFrames1 0x1606c
#define ARN5_GIGE_EI_ECORE_MAC_aSingleCollisionFrames1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aSingleCollisionFrames1_t;

#define ARN5_GIGE_EI_ECORE_MAC_aSingleCollisionFrames0 0x1606d
#define ARN5_GIGE_EI_ECORE_MAC_aSingleCollisionFrames0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aSingleCollisionFrames0_t;

#define ARN5_GIGE_EI_ECORE_MAC_aMultipleCollisionFrames1 0x1606e
#define ARN5_GIGE_EI_ECORE_MAC_aMultipleCollisionFrames1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aMultipleCollisionFrames1_t;

#define ARN5_GIGE_EI_ECORE_MAC_aMultipleCollisionFrames0 0x1606f
#define ARN5_GIGE_EI_ECORE_MAC_aMultipleCollisionFrames0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aMultipleCollisionFrames0_t;

#define ARN5_GIGE_EI_ECORE_MAC_aLateCollisionFrames1 0x16070
#define ARN5_GIGE_EI_ECORE_MAC_aLateCollisionFrames1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aLateCollisionFrames1_t;

#define ARN5_GIGE_EI_ECORE_MAC_aLateCollisionFrames0 0x16071
#define ARN5_GIGE_EI_ECORE_MAC_aLateCollisionFrames0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aLateCollisionFrames0_t;

#define ARN5_GIGE_EI_ECORE_MAC_aExcessiveCollisionFrames1 0x16072
#define ARN5_GIGE_EI_ECORE_MAC_aExcessiveCollisionFrames1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aExcessiveCollisionFrames1_t;

#define ARN5_GIGE_EI_ECORE_MAC_aExcessiveCollisionFrames0 0x16073
#define ARN5_GIGE_EI_ECORE_MAC_aExcessiveCollisionFrames0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_aExcessiveCollisionFrames0_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1519toMaxOctets1 0x16074
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1519toMaxOctets1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1519toMaxOctets1_t;

#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1519toMaxOctets0 0x16075
#define ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1519toMaxOctets0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1519toMaxOctets0_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * ecore::pcs
 */

#define ARN5_GIGE_EI_ECORE_PCS_CONTROL           0x16190
#define ARN5_GIGE_EI_ECORE_PCS_CONTROL_dft       0x0140
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Reset                :  1 ; /* bits 15:15 */
    cs_uint16 Loopback             :  1 ; /* bits 14:14 */
    cs_uint16 SpLSB                :  1 ; /* bits 13:13 */
    cs_uint16 AutoNegEn            :  1 ; /* bits 12:12 */
    cs_uint16 PwrDn                :  1 ; /* bits 11:11 */
    cs_uint16 Isolate              :  1 ; /* bits 10:10 */
    cs_uint16 AutoNegRestart       :  1 ; /* bits 9:9 */
    cs_uint16 DuplexMode           :  1 ; /* bits 8:8 */
    cs_uint16 ColTest              :  1 ; /* bits 7:7 */
    cs_uint16 SpMSB                :  1 ; /* bits 6:6 */
    cs_uint16 ResetStats           :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd1               :  5 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 ResetStats           :  1 ; /* bits 5:5 */
    cs_uint16 SpMSB                :  1 ; /* bits 6:6 */
    cs_uint16 ColTest              :  1 ; /* bits 7:7 */
    cs_uint16 DuplexMode           :  1 ; /* bits 8:8 */
    cs_uint16 AutoNegRestart       :  1 ; /* bits 9:9 */
    cs_uint16 Isolate              :  1 ; /* bits 10:10 */
    cs_uint16 PwrDn                :  1 ; /* bits 11:11 */
    cs_uint16 AutoNegEn            :  1 ; /* bits 12:12 */
    cs_uint16 SpLSB                :  1 ; /* bits 13:13 */
    cs_uint16 Loopback             :  1 ; /* bits 14:14 */
    cs_uint16 Reset                :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_PCS_CONTROL_t;

#define ARN5_GIGE_EI_ECORE_PCS_STATUS            0x16191
#define ARN5_GIGE_EI_ECORE_PCS_STATUS_dft        0x0009
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 One00BT4             :  1 ; /* bits 15:15 */
    cs_uint16 One00BXFD            :  1 ; /* bits 14:14 */
    cs_uint16 One00BXHD            :  1 ; /* bits 13:13 */
    cs_uint16 One0FD               :  1 ; /* bits 12:12 */
    cs_uint16 One0HD               :  1 ; /* bits 11:11 */
    cs_uint16 One00BT2FD           :  1 ; /* bits 10:10 */
    cs_uint16 One00BT2HD           :  1 ; /* bits 9:9 */
    cs_uint16 ExtStat              :  1 ; /* bits 8:8 */
    cs_uint16 RSVD0                :  1 ; /* bits 7:7 */
    cs_uint16 RSVD1                :  1 ; /* bits 6:6 */
    cs_uint16 AutoNegComplete      :  1 ; /* bits 5:5 */
    cs_uint16 RmtFault             :  1 ; /* bits 4:4 */
    cs_uint16 AutoNegAb            :  1 ; /* bits 3:3 */
    cs_uint16 LinkStat             :  1 ; /* bits 2:2 */
    cs_uint16 Jabber               :  1 ; /* bits 1:1 */
    cs_uint16 XtCap                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 XtCap                :  1 ; /* bits 0:0 */
    cs_uint16 Jabber               :  1 ; /* bits 1:1 */
    cs_uint16 LinkStat             :  1 ; /* bits 2:2 */
    cs_uint16 AutoNegAb            :  1 ; /* bits 3:3 */
    cs_uint16 RmtFault             :  1 ; /* bits 4:4 */
    cs_uint16 AutoNegComplete      :  1 ; /* bits 5:5 */
    cs_uint16 RSVD1                :  1 ; /* bits 6:6 */
    cs_uint16 RSVD0                :  1 ; /* bits 7:7 */
    cs_uint16 ExtStat              :  1 ; /* bits 8:8 */
    cs_uint16 One00BT2HD           :  1 ; /* bits 9:9 */
    cs_uint16 One00BT2FD           :  1 ; /* bits 10:10 */
    cs_uint16 One0HD               :  1 ; /* bits 11:11 */
    cs_uint16 One0FD               :  1 ; /* bits 12:12 */
    cs_uint16 One00BXHD            :  1 ; /* bits 13:13 */
    cs_uint16 One00BXFD            :  1 ; /* bits 14:14 */
    cs_uint16 One00BT4             :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_PCS_STATUS_t;

#define ARN5_GIGE_EI_ECORE_PCS_PHY1              0x16192
#define ARN5_GIGE_EI_ECORE_PCS_PHY1_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 PhyId1               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PhyId1               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_PCS_PHY1_t;

#define ARN5_GIGE_EI_ECORE_PCS_PHY0              0x16193
#define ARN5_GIGE_EI_ECORE_PCS_PHY0_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 PhyId0               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PhyId0               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_PCS_PHY0_t;

#define ARN5_GIGE_EI_ECORE_PCS_DEV_ABILITY       0x16194
#define ARN5_GIGE_EI_ECORE_PCS_DEV_ABILITY_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 NextPage             :  1 ; /* bits 15:15 */
    cs_uint16 ACK                  :  1 ; /* bits 14:14 */
    cs_uint16 RmtFault             :  2 ; /* bits 13:12 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 PS                   :  2 ; /* bits 8:7 */
    cs_uint16 HalfDuplex           :  1 ; /* bits 6:6 */
    cs_uint16 FullDuplex           :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd1               :  4 ;
    cs_uint16 RSVD                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RSVD                 :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               :  4 ;
    cs_uint16 FullDuplex           :  1 ; /* bits 5:5 */
    cs_uint16 HalfDuplex           :  1 ; /* bits 6:6 */
    cs_uint16 PS                   :  2 ; /* bits 8:7 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 RmtFault             :  2 ; /* bits 13:12 */
    cs_uint16 ACK                  :  1 ; /* bits 14:14 */
    cs_uint16 NextPage             :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_PCS_DEV_ABILITY_t;
#define     ARN5_RmtFault_OK                              0x00
#define     ARN5_RmtFault_LINKFAIL                        0x01
#define     ARN5_RmtFault_OFFLINE                         0x02
#define     ARN5_RmtFault_ANERROR                         0x03
#define     ARN5_PS_NOPAUSE                               0x00
#define     ARN5_PS_SYMPAUSE                              0x01
#define     ARN5_PS_ASYMPAUSE                             0x02

#define ARN5_GIGE_EI_ECORE_PCS_PARTNER_ABILITY   0x16195
#define ARN5_GIGE_EI_ECORE_PCS_PARTNER_ABILITY_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 NextPage             :  1 ; /* bits 15:15 */
    cs_uint16 ACK                  :  1 ; /* bits 14:14 */
    cs_uint16 RmtFault             :  2 ; /* bits 13:12 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 PS                   :  2 ; /* bits 8:7 */
    cs_uint16 HalfDuplex           :  1 ; /* bits 6:6 */
    cs_uint16 FullDuplex           :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd1               :  5 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 FullDuplex           :  1 ; /* bits 5:5 */
    cs_uint16 HalfDuplex           :  1 ; /* bits 6:6 */
    cs_uint16 PS                   :  2 ; /* bits 8:7 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 RmtFault             :  2 ; /* bits 13:12 */
    cs_uint16 ACK                  :  1 ; /* bits 14:14 */
    cs_uint16 NextPage             :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_PCS_PARTNER_ABILITY_t;
#define     ARN5_PS_PAUSE                                 0x03

#define ARN5_GIGE_EI_ECORE_PCS_AN_EXPANSION      0x16196
#define ARN5_GIGE_EI_ECORE_PCS_AN_EXPANSION_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 13 ;
    cs_uint16 PageRcv              :  1 ; /* bits 2:2 */
    cs_uint16 NextPageAb           :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 NextPageAb           :  1 ; /* bits 1:1 */
    cs_uint16 PageRcv              :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd2               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_PCS_AN_EXPANSION_t;

#define ARN5_GIGE_EI_ECORE_PCS_DEVICE_NEXT_PAGE  0x16197
#define ARN5_GIGE_EI_ECORE_PCS_DEVICE_NEXT_PAGE_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 NA                   : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 NA                   : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_PCS_DEVICE_NEXT_PAGE_t;

#define ARN5_GIGE_EI_ECORE_PCS_PARTNER_NEXT_PAGE 0x16198
#define ARN5_GIGE_EI_ECORE_PCS_PARTNER_NEXT_PAGE_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 NA                   : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 NA                   : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_PCS_PARTNER_NEXT_PAGE_t;

#define ARN5_GIGE_EI_ECORE_PCS_MASTER_SLAVE_CNTL 0x16199
#define ARN5_GIGE_EI_ECORE_PCS_MASTER_SLAVE_CNTL_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 NA                   : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 NA                   : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_PCS_MASTER_SLAVE_CNTL_t;

#define ARN5_GIGE_EI_ECORE_PCS_MASTER_SLAVE_STAT 0x1619a
#define ARN5_GIGE_EI_ECORE_PCS_MASTER_SLAVE_STAT_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 NA                   : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 NA                   : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_PCS_MASTER_SLAVE_STAT_t;

#define ARN5_GIGE_EI_ECORE_PCS_EXTENDED_STATUS   0x1619f
#define ARN5_GIGE_EI_ECORE_PCS_EXTENDED_STATUS_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 NA                   : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 NA                   : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_PCS_EXTENDED_STATUS_t;

#define ARN5_GIGE_EI_ECORE_PCS_SCRATCH           0x161a0
#define ARN5_GIGE_EI_ECORE_PCS_SCRATCH_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Scratch              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Scratch              : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_PCS_SCRATCH_t;

#define ARN5_GIGE_EI_ECORE_PCS_REV               0x161a1
#define ARN5_GIGE_EI_ECORE_PCS_REV_dft           0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 REV                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 REV                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_PCS_REV_t;

#define ARN5_GIGE_EI_ECORE_PCS_LINK_TIMER_0      0x161a2
#define ARN5_GIGE_EI_ECORE_PCS_LINK_TIMER_0_dft  0x12d0
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 LINK_TIMER           : 15 ; /* bits 15:1 */
    cs_uint16 LINK_TIMER0          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LINK_TIMER0          :  1 ; /* bits 0:0 */
    cs_uint16 LINK_TIMER           : 15 ; /* bits 15:1 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_PCS_LINK_TIMER_0_t;

#define ARN5_GIGE_EI_ECORE_PCS_LINK_TIMER_1      0x161a3
#define ARN5_GIGE_EI_ECORE_PCS_LINK_TIMER_1_dft  0x0013
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 LINK_TIMER           :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LINK_TIMER           :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_PCS_LINK_TIMER_1_t;

#define ARN5_GIGE_EI_ECORE_PCS_RX_8B10B_DEC_ERR1 0x161a5
#define ARN5_GIGE_EI_ECORE_PCS_RX_8B10B_DEC_ERR1_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_PCS_RX_8B10B_DEC_ERR1_t;

#define ARN5_GIGE_EI_ECORE_PCS_RX_8B10B_DEC_ERR0 0x161a6
#define ARN5_GIGE_EI_ECORE_PCS_RX_8B10B_DEC_ERR0_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_PCS_RX_8B10B_DEC_ERR0_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * ecore::top
 */

#define ARN5_GIGE_EI_ECORE_TOP_Reset             0x161b0
#define ARN5_GIGE_EI_ECORE_TOP_Reset_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 10 ;
    cs_uint16 PCS_RX_RST           :  1 ; /* bits 5:5 */
    cs_uint16 PCS_TX_RST           :  1 ; /* bits 4:4 */
    cs_uint16 MAC_CLIENT_RX_RST    :  1 ; /* bits 3:3 */
    cs_uint16 MAC_CLIENT_TX_RST    :  1 ; /* bits 2:2 */
    cs_uint16 MAC_RX_RST           :  1 ; /* bits 1:1 */
    cs_uint16 MAC_TX_RST           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 MAC_TX_RST           :  1 ; /* bits 0:0 */
    cs_uint16 MAC_RX_RST           :  1 ; /* bits 1:1 */
    cs_uint16 MAC_CLIENT_TX_RST    :  1 ; /* bits 2:2 */
    cs_uint16 MAC_CLIENT_RX_RST    :  1 ; /* bits 3:3 */
    cs_uint16 PCS_TX_RST           :  1 ; /* bits 4:4 */
    cs_uint16 PCS_RX_RST           :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd1               : 10 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_TOP_Reset_t;

#define ARN5_GIGE_EI_ECORE_TOP_ModeControl       0x161b1
#define ARN5_GIGE_EI_ECORE_TOP_ModeControl_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 10 ;
    cs_uint16 LNK_DIS_MAC_EN       :  1 ; /* bits 5:5 */
    cs_uint16 DUPLEXMODE           :  1 ; /* bits 4:4 */
    cs_uint16 ETHSPEED             :  2 ; /* bits 3:2 */
    cs_uint16 MACMODE              :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 MACMODE              :  2 ; /* bits 1:0 */
    cs_uint16 ETHSPEED             :  2 ; /* bits 3:2 */
    cs_uint16 DUPLEXMODE           :  1 ; /* bits 4:4 */
    cs_uint16 LNK_DIS_MAC_EN       :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd1               : 10 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_TOP_ModeControl_t;
#define     ARN5_DUPLEXMODE_HD                            0x00
#define     ARN5_DUPLEXMODE_FD                            0x01
#define     ARN5_ETHSPEED_10MBPS                          0x00
#define     ARN5_ETHSPEED_100MBPS                         0x01
#define     ARN5_ETHSPEED_1000MBPS                        0x00
#define     ARN5_MACMODE_AN_GE                            0x00
#define     ARN5_MACMODE_AN_SGMII                         0x01
#define     ARN5_MACMODE_FORCE                            0x00

#define ARN5_GIGE_EI_ECORE_TOP_Control           0x161b2
#define ARN5_GIGE_EI_ECORE_TOP_Control_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 SLEEP                :  1 ; /* bits 2:2 */
    cs_uint16 XON_XOFF             :  1 ; /* bits 1:1 */
    cs_uint16 X_GEN                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 X_GEN                :  1 ; /* bits 0:0 */
    cs_uint16 XON_XOFF             :  1 ; /* bits 1:1 */
    cs_uint16 SLEEP                :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_TOP_Control_t;

#define ARN5_GIGE_EI_ECORE_TOP_INTERRUPT         0x161b3
#define ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 MAC_WAKEUPi          :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd4               :  2 ;
    cs_uint16 RA_TX_PROTERRi       :  1 ; /* bits 12:12 */
    cs_uint16 RA_TX_UFLWi          :  1 ; /* bits 11:11 */
    cs_uint16 RA_TX_OFLWi          :  1 ; /* bits 10:10 */
    cs_uint16 RA_RX_UFLWi          :  1 ; /* bits 9:9 */
    cs_uint16 RA_RX_OFLWi          :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 MAC_PROT_ERR_WRi     :  1 ; /* bits 6:6 */
    cs_uint16 MAC_PROT_ERR_RDi     :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 PCS_AN_LINKi         :  1 ; /* bits 3:3 */
    cs_uint16 PCS_AN_DONEi         :  1 ; /* bits 2:2 */
    cs_uint16 PCS_LINKi            :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 PCS_LINKi            :  1 ; /* bits 1:1 */
    cs_uint16 PCS_AN_DONEi         :  1 ; /* bits 2:2 */
    cs_uint16 PCS_AN_LINKi         :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 MAC_PROT_ERR_RDi     :  1 ; /* bits 5:5 */
    cs_uint16 MAC_PROT_ERR_WRi     :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 RA_RX_OFLWi          :  1 ; /* bits 8:8 */
    cs_uint16 RA_RX_UFLWi          :  1 ; /* bits 9:9 */
    cs_uint16 RA_TX_OFLWi          :  1 ; /* bits 10:10 */
    cs_uint16 RA_TX_UFLWi          :  1 ; /* bits 11:11 */
    cs_uint16 RA_TX_PROTERRi       :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd4               :  2 ;
    cs_uint16 MAC_WAKEUPi          :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_t;

#define ARN5_GIGE_EI_ECORE_TOP_INTENABLE         0x161b4
#define ARN5_GIGE_EI_ECORE_TOP_INTENABLE_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 MAC_WAKEUPe          :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd4               :  2 ;
    cs_uint16 RA_TX_PROTERRe       :  1 ; /* bits 12:12 */
    cs_uint16 RA_TX_UFLWe          :  1 ; /* bits 11:11 */
    cs_uint16 RA_TX_OFLWe          :  1 ; /* bits 10:10 */
    cs_uint16 RA_RX_UFLWe          :  1 ; /* bits 9:9 */
    cs_uint16 RA_RX_OFLWe          :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 MAC_PROT_ERR_WRe     :  1 ; /* bits 6:6 */
    cs_uint16 MAC_PROT_ERR_RDe     :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 PCS_AN_LINKe         :  1 ; /* bits 3:3 */
    cs_uint16 PCS_AN_DONEe         :  1 ; /* bits 2:2 */
    cs_uint16 PCS_LINKe            :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 PCS_LINKe            :  1 ; /* bits 1:1 */
    cs_uint16 PCS_AN_DONEe         :  1 ; /* bits 2:2 */
    cs_uint16 PCS_AN_LINKe         :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 MAC_PROT_ERR_RDe     :  1 ; /* bits 5:5 */
    cs_uint16 MAC_PROT_ERR_WRe     :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 RA_RX_OFLWe          :  1 ; /* bits 8:8 */
    cs_uint16 RA_RX_UFLWe          :  1 ; /* bits 9:9 */
    cs_uint16 RA_TX_OFLWe          :  1 ; /* bits 10:10 */
    cs_uint16 RA_TX_UFLWe          :  1 ; /* bits 11:11 */
    cs_uint16 RA_TX_PROTERRe       :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd4               :  2 ;
    cs_uint16 MAC_WAKEUPe          :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_TOP_INTENABLE_t;

#define ARN5_GIGE_EI_ECORE_TOP_INTSTATUS         0x161b5
#define ARN5_GIGE_EI_ECORE_TOP_INTSTATUS_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 MAC_WAKEUPs          :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd4               :  2 ;
    cs_uint16 RA_TX_PROTERRs       :  1 ; /* bits 12:12 */
    cs_uint16 RA_TX_UFLWs          :  1 ; /* bits 11:11 */
    cs_uint16 RA_TX_OFLWs          :  1 ; /* bits 10:10 */
    cs_uint16 RA_RX_UFLWs          :  1 ; /* bits 9:9 */
    cs_uint16 RA_RX_OFLWs          :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 MAC_PROT_ERR_WRs     :  1 ; /* bits 6:6 */
    cs_uint16 MAC_PROT_ERR_RDs     :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 PCS_AN_LINKs         :  1 ; /* bits 3:3 */
    cs_uint16 PCS_AN_DONEs         :  1 ; /* bits 2:2 */
    cs_uint16 PCS_LINKs            :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 PCS_LINKs            :  1 ; /* bits 1:1 */
    cs_uint16 PCS_AN_DONEs         :  1 ; /* bits 2:2 */
    cs_uint16 PCS_AN_LINKs         :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 MAC_PROT_ERR_RDs     :  1 ; /* bits 5:5 */
    cs_uint16 MAC_PROT_ERR_WRs     :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 RA_RX_OFLWs          :  1 ; /* bits 8:8 */
    cs_uint16 RA_RX_UFLWs          :  1 ; /* bits 9:9 */
    cs_uint16 RA_TX_OFLWs          :  1 ; /* bits 10:10 */
    cs_uint16 RA_TX_UFLWs          :  1 ; /* bits 11:11 */
    cs_uint16 RA_TX_PROTERRs       :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd4               :  2 ;
    cs_uint16 MAC_WAKEUPs          :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_TOP_INTSTATUS_t;

#define ARN5_GIGE_EI_ECORE_TOP_MODESTATUS        0x161b6
#define ARN5_GIGE_EI_ECORE_TOP_MODESTATUS_dft    0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 FinalDuplexMode      :  1 ; /* bits 2:2 */
    cs_uint16 FinalEthMode         :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FinalEthMode         :  2 ; /* bits 1:0 */
    cs_uint16 FinalDuplexMode      :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_TOP_MODESTATUS_t;
#define     ARN5_FinalDuplexMode_HD                       0x00
#define     ARN5_FinalDuplexMode_FD                       0x01
#define     ARN5_FinalEthMode_10MBPS                      0x00
#define     ARN5_FinalEthMode_100MBPS                     0x01
#define     ARN5_FinalEthMode_1000MBPS                    0x00

#define ARN5_GIGE_EI_ECORE_TOP_UFLOW_PROTECT     0x161b8
#define ARN5_GIGE_EI_ECORE_TOP_UFLOW_PROTECT_dft 0x096e
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 pkt_length           :  5 ; /* bits 12:8 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 gige_only            :  1 ; /* bits 6:6 */
    cs_uint16 enable               :  1 ; /* bits 5:5 */
    cs_uint16 idle_length          :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 idle_length          :  5 ; /* bits 4:0 */
    cs_uint16 enable               :  1 ; /* bits 5:5 */
    cs_uint16 gige_only            :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 pkt_length           :  5 ; /* bits 12:8 */
    cs_uint16 rsrvd2               :  3 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_ECORE_TOP_UFLOW_PROTECT_t;


/******************************************************************
 *
 * Block declaration.
 *
 ******************************************************************/

/* ecore */

typedef struct {
  ARN5_GIGE_EI_ECORE_MAC_REV1_t            REV1;                          /* addr: 0x0000 */
  ARN5_GIGE_EI_ECORE_MAC_REV0_t            REV0;                          /* addr: 0x0001 */
  ARN5_GIGE_EI_ECORE_MAC_SCRATCH1_t        SCRATCH1;                      /* addr: 0x0002 */
  ARN5_GIGE_EI_ECORE_MAC_SCRATCH0_t        SCRATCH0;                      /* addr: 0x0003 */
  ARN5_GIGE_EI_ECORE_MAC_COMMAND_CONFIG1_t COMMAND_CONFIG1;               /* addr: 0x0004 */
  ARN5_GIGE_EI_ECORE_MAC_COMMAND_CONFIG0_t COMMAND_CONFIG0;               /* addr: 0x0005 */
  ARN5_GIGE_EI_ECORE_MAC_MAC_ADDR_L1_t     MAC_ADDR_L1;                   /* addr: 0x0006 */
  ARN5_GIGE_EI_ECORE_MAC_MAC_ADDR_L0_t     MAC_ADDR_L0;                   /* addr: 0x0007 */
  ARN5_GIGE_EI_ECORE_MAC_MAC_ADDR_U1_t     MAC_ADDR_U1;                   /* addr: 0x0008 */
  ARN5_GIGE_EI_ECORE_MAC_MAC_ADDR_U0_t     MAC_ADDR_U0;                   /* addr: 0x0009 */
  ARN5_GIGE_EI_ECORE_MAC_FRM_LENGTH1_t     FRM_LENGTH1;                   /* addr: 0x000a */
  ARN5_GIGE_EI_ECORE_MAC_FRM_LENGTH0_t     FRM_LENGTH0;                   /* addr: 0x000b */
  ARN5_GIGE_EI_ECORE_MAC_PAUSE_QUANT1_t    PAUSE_QUANT1;                  /* addr: 0x000c */
  ARN5_GIGE_EI_ECORE_MAC_PAUSE_QUANT0_t    PAUSE_QUANT0;                  /* addr: 0x000d */
  ARN5_GIGE_EI_ECORE_MAC_RX_SECTION_EMPTY1_t RX_SECTION_EMPTY1;             /* addr: 0x000e */
  ARN5_GIGE_EI_ECORE_MAC_RX_SECTION_EMPTY0_t RX_SECTION_EMPTY0;             /* addr: 0x000f */
  ARN5_GIGE_EI_ECORE_MAC_RX_SECTION_FULL1_t RX_SECTION_FULL1;              /* addr: 0x0010 */
  ARN5_GIGE_EI_ECORE_MAC_RX_SECTION_FULL0_t RX_SECTION_FULL0;              /* addr: 0x0011 */
  ARN5_GIGE_EI_ECORE_MAC_TX_SECTION_EMPTY1_t TX_SECTION_EMPTY1;             /* addr: 0x0012 */
  ARN5_GIGE_EI_ECORE_MAC_TX_SECTION_EMPTY0_t TX_SECTION_EMPTY0;             /* addr: 0x0013 */
  ARN5_GIGE_EI_ECORE_MAC_TX_SECTION_FULL1_t TX_SECTION_FULL1;              /* addr: 0x0014 */
  ARN5_GIGE_EI_ECORE_MAC_TX_SECTION_FULL0_t TX_SECTION_FULL0;              /* addr: 0x0015 */
  ARN5_GIGE_EI_ECORE_MAC_RX_ALMOST_EMPTY1_t RX_ALMOST_EMPTY1;              /* addr: 0x0016 */
  ARN5_GIGE_EI_ECORE_MAC_RX_ALMOST_EMPTY0_t RX_ALMOST_EMPTY0;              /* addr: 0x0017 */
  ARN5_GIGE_EI_ECORE_MAC_RX_ALMOST_FULL1_t RX_ALMOST_FULL1;               /* addr: 0x0018 */
  ARN5_GIGE_EI_ECORE_MAC_RX_ALMOST_FULL0_t RX_ALMOST_FULL0;               /* addr: 0x0019 */
  ARN5_GIGE_EI_ECORE_MAC_TX_ALMOST_EMPTY1_t TX_ALMOST_EMPTY1;              /* addr: 0x001a */
  ARN5_GIGE_EI_ECORE_MAC_TX_ALMOST_EMPTY0_t TX_ALMOST_EMPTY0;              /* addr: 0x001b */
  ARN5_GIGE_EI_ECORE_MAC_TX_ALMOST_FULL1_t TX_ALMOST_FULL1;               /* addr: 0x001c */
  ARN5_GIGE_EI_ECORE_MAC_TX_ALMOST_FULL0_t TX_ALMOST_FULL0;               /* addr: 0x001d */
  cs_uint16                                rsrvd1[18];
  ARN5_GIGE_EI_ECORE_MAC_aMacID_L1_t       aMacID_L1;                     /* addr: 0x0030 */
  ARN5_GIGE_EI_ECORE_MAC_aMacID_L0_t       aMacID_L0;                     /* addr: 0x0031 */
  ARN5_GIGE_EI_ECORE_MAC_aMacID_U1_t       aMacID_U1;                     /* addr: 0x0032 */
  ARN5_GIGE_EI_ECORE_MAC_aMacID_U0_t       aMacID_U0;                     /* addr: 0x0033 */
  ARN5_GIGE_EI_ECORE_MAC_aFramesTransmittedOK1_t aFramesTransmittedOK1;         /* addr: 0x0034 */
  ARN5_GIGE_EI_ECORE_MAC_aFramesTransmittedOK0_t aFramesTransmittedOK0;         /* addr: 0x0035 */
  ARN5_GIGE_EI_ECORE_MAC_aFramesReceivedOK1_t aFramesReceivedOK1;            /* addr: 0x0036 */
  ARN5_GIGE_EI_ECORE_MAC_aFramesReceivedOK0_t aFramesReceivedOK0;            /* addr: 0x0037 */
  ARN5_GIGE_EI_ECORE_MAC_aFrameCheckSequenceErrors1_t aFrameCheckSequenceErrors1;    /* addr: 0x0038 */
  ARN5_GIGE_EI_ECORE_MAC_aFrameCheckSequenceErrors0_t aFrameCheckSequenceErrors0;    /* addr: 0x0039 */
  ARN5_GIGE_EI_ECORE_MAC_aAlignmentErrors1_t aAlignmentErrors1;             /* addr: 0x003a */
  ARN5_GIGE_EI_ECORE_MAC_aAlignmentErrors0_t aAlignmentErrors0;             /* addr: 0x003b */
  ARN5_GIGE_EI_ECORE_MAC_aOctetsTransmittedOK1_t aOctetsTransmittedOK1;         /* addr: 0x003c */
  ARN5_GIGE_EI_ECORE_MAC_aOctetsTransmittedOK0_t aOctetsTransmittedOK0;         /* addr: 0x003d */
  ARN5_GIGE_EI_ECORE_MAC_aOctetsReceivedOK1_t aOctetsReceivedOK1;            /* addr: 0x003e */
  ARN5_GIGE_EI_ECORE_MAC_aOctetsReceivedOK0_t aOctetsReceivedOK0;            /* addr: 0x003f */
  ARN5_GIGE_EI_ECORE_MAC_aTxPauseMacCtrlFrames1_t aTxPauseMacCtrlFrames1;        /* addr: 0x0040 */
  ARN5_GIGE_EI_ECORE_MAC_aTxPauseMacCtrlFrames0_t aTxPauseMacCtrlFrames0;        /* addr: 0x0041 */
  ARN5_GIGE_EI_ECORE_MAC_aRxPauseMacCtrlFrames1_t aRxPauseMacCtrlFrames1;        /* addr: 0x0042 */
  ARN5_GIGE_EI_ECORE_MAC_aRxPauseMacCtrlFrames0_t aRxPauseMacCtrlFrames0;        /* addr: 0x0043 */
  ARN5_GIGE_EI_ECORE_MAC_ifInErrors1_t     ifInErrors1;                   /* addr: 0x0044 */
  ARN5_GIGE_EI_ECORE_MAC_ifInErrors0_t     ifInErrors0;                   /* addr: 0x0045 */
  ARN5_GIGE_EI_ECORE_MAC_ifOutErrors1_t    ifOutErrors1;                  /* addr: 0x0046 */
  ARN5_GIGE_EI_ECORE_MAC_ifOutErrors0_t    ifOutErrors0;                  /* addr: 0x0047 */
  ARN5_GIGE_EI_ECORE_MAC_ifInUcastPkts1_t  ifInUcastPkts1;                /* addr: 0x0048 */
  ARN5_GIGE_EI_ECORE_MAC_ifInUcastPkts0_t  ifInUcastPkts0;                /* addr: 0x0049 */
  ARN5_GIGE_EI_ECORE_MAC_ifInMulticastPkts1_t ifInMulticastPkts1;            /* addr: 0x004a */
  ARN5_GIGE_EI_ECORE_MAC_ifInMulticastPkts0_t ifInMulticastPkts0;            /* addr: 0x004b */
  ARN5_GIGE_EI_ECORE_MAC_ifInBroadcastPkts1_t ifInBroadcastPkts1;            /* addr: 0x004c */
  ARN5_GIGE_EI_ECORE_MAC_ifInBroadcastPkts0_t ifInBroadcastPkts0;            /* addr: 0x004d */
  ARN5_GIGE_EI_ECORE_MAC_ifOutDiscards1_t  ifOutDiscards1;                /* addr: 0x004e */
  ARN5_GIGE_EI_ECORE_MAC_ifOutDiscards0_t  ifOutDiscards0;                /* addr: 0x004f */
  ARN5_GIGE_EI_ECORE_MAC_ifOutUcastPkts1_t ifOutUcastPkts1;               /* addr: 0x0050 */
  ARN5_GIGE_EI_ECORE_MAC_ifOutUcastPkts0_t ifOutUcastPkts0;               /* addr: 0x0051 */
  ARN5_GIGE_EI_ECORE_MAC_ifOutMulticastPkts1_t ifOutMulticastPkts1;           /* addr: 0x0052 */
  ARN5_GIGE_EI_ECORE_MAC_ifOutMulticastPkts0_t ifOutMulticastPkts0;           /* addr: 0x0053 */
  ARN5_GIGE_EI_ECORE_MAC_ifOutBroadcastPkts1_t ifOutBroadcastPkts1;           /* addr: 0x0054 */
  ARN5_GIGE_EI_ECORE_MAC_ifOutBroadcastPkts0_t ifOutBroadcastPkts0;           /* addr: 0x0055 */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsDropEvents1_t etherStatsDropEvents1;         /* addr: 0x0056 */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsDropEvents0_t etherStatsDropEvents0;         /* addr: 0x0057 */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsOctets1_t etherStatsOctets1;             /* addr: 0x0058 */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsOctets0_t etherStatsOctets0;             /* addr: 0x0059 */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1_t etherStatsPkts1;               /* addr: 0x005a */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts0_t etherStatsPkts0;               /* addr: 0x005b */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsUndersizePkts1_t etherStatsUndersizePkts1;      /* addr: 0x005c */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsUndersizePkts0_t etherStatsUndersizePkts0;      /* addr: 0x005d */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsOversizePkts1_t etherStatsOversizePkts1;       /* addr: 0x005e */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsOversizePkts0_t etherStatsOversizePkts0;       /* addr: 0x005f */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts64Octets1_t etherStatsPkts64Octets1;       /* addr: 0x0060 */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts64Octets0_t etherStatsPkts64Octets0;       /* addr: 0x0061 */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts65to127Octets1_t etherStatsPkts65to127Octets1;  /* addr: 0x0062 */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts65to127Octets0_t etherStatsPkts65to127Octets0;  /* addr: 0x0063 */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts128to255Octets1_t etherStatsPkts128to255Octets1; /* addr: 0x0064 */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts128to255Octets0_t etherStatsPkts128to255Octets0; /* addr: 0x0065 */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts256to511Octets1_t etherStatsPkts256to511Octets1; /* addr: 0x0066 */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts256to511Octets0_t etherStatsPkts256to511Octets0; /* addr: 0x0067 */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts512to1023Octets1_t etherStatsPkts512to1023Octets1; /* addr: 0x0068 */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts512to1023Octets0_t etherStatsPkts512to1023Octets0; /* addr: 0x0069 */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1024to1518Octets1_t etherStatsPkts1024to1518Octets1; /* addr: 0x006a */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1024to1518Octets0_t etherStatsPkts1024to1518Octets0; /* addr: 0x006b */
  ARN5_GIGE_EI_ECORE_MAC_aSingleCollisionFrames1_t aSingleCollisionFrames1;       /* addr: 0x006c */
  ARN5_GIGE_EI_ECORE_MAC_aSingleCollisionFrames0_t aSingleCollisionFrames0;       /* addr: 0x006d */
  ARN5_GIGE_EI_ECORE_MAC_aMultipleCollisionFrames1_t aMultipleCollisionFrames1;     /* addr: 0x006e */
  ARN5_GIGE_EI_ECORE_MAC_aMultipleCollisionFrames0_t aMultipleCollisionFrames0;     /* addr: 0x006f */
  ARN5_GIGE_EI_ECORE_MAC_aLateCollisionFrames1_t aLateCollisionFrames1;         /* addr: 0x0070 */
  ARN5_GIGE_EI_ECORE_MAC_aLateCollisionFrames0_t aLateCollisionFrames0;         /* addr: 0x0071 */
  ARN5_GIGE_EI_ECORE_MAC_aExcessiveCollisionFrames1_t aExcessiveCollisionFrames1;    /* addr: 0x0072 */
  ARN5_GIGE_EI_ECORE_MAC_aExcessiveCollisionFrames0_t aExcessiveCollisionFrames0;    /* addr: 0x0073 */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1519toMaxOctets1_t etherStatsPkts1519toMaxOctets1; /* addr: 0x0074 */
  ARN5_GIGE_EI_ECORE_MAC_etherStatsPkts1519toMaxOctets0_t etherStatsPkts1519toMaxOctets0; /* addr: 0x0075 */
  cs_uint16                                rsrvd2[282];
  ARN5_GIGE_EI_ECORE_PCS_CONTROL_t         CONTROL;                       /* addr: 0x0190 */
  ARN5_GIGE_EI_ECORE_PCS_STATUS_t          STATUS;                        /* addr: 0x0191 */
  ARN5_GIGE_EI_ECORE_PCS_PHY1_t            PHY1;                          /* addr: 0x0192 */
  ARN5_GIGE_EI_ECORE_PCS_PHY0_t            PHY0;                          /* addr: 0x0193 */
  ARN5_GIGE_EI_ECORE_PCS_DEV_ABILITY_t     DEV_ABILITY;                   /* addr: 0x0194 */
  ARN5_GIGE_EI_ECORE_PCS_PARTNER_ABILITY_t PARTNER_ABILITY;               /* addr: 0x0195 */
  ARN5_GIGE_EI_ECORE_PCS_AN_EXPANSION_t    AN_EXPANSION;                  /* addr: 0x0196 */
  ARN5_GIGE_EI_ECORE_PCS_DEVICE_NEXT_PAGE_t DEVICE_NEXT_PAGE;              /* addr: 0x0197 */
  ARN5_GIGE_EI_ECORE_PCS_PARTNER_NEXT_PAGE_t PARTNER_NEXT_PAGE;             /* addr: 0x0198 */
  ARN5_GIGE_EI_ECORE_PCS_MASTER_SLAVE_CNTL_t MASTER_SLAVE_CNTL;             /* addr: 0x0199 */
  ARN5_GIGE_EI_ECORE_PCS_MASTER_SLAVE_STAT_t MASTER_SLAVE_STAT;             /* addr: 0x019a */
  cs_uint16                                rsrvd3[4];
  ARN5_GIGE_EI_ECORE_PCS_EXTENDED_STATUS_t EXTENDED_STATUS;               /* addr: 0x019f */
  ARN5_GIGE_EI_ECORE_PCS_SCRATCH_t         SCRATCH;                       /* addr: 0x01a0 */
  ARN5_GIGE_EI_ECORE_PCS_REV_t             REV;                           /* addr: 0x01a1 */
  ARN5_GIGE_EI_ECORE_PCS_LINK_TIMER_0_t    LINK_TIMER_0;                  /* addr: 0x01a2 */
  ARN5_GIGE_EI_ECORE_PCS_LINK_TIMER_1_t    LINK_TIMER_1;                  /* addr: 0x01a3 */
  cs_uint16                                rsrvd4;
  ARN5_GIGE_EI_ECORE_PCS_RX_8B10B_DEC_ERR1_t RX_8B10B_DEC_ERR1;             /* addr: 0x01a5 */
  ARN5_GIGE_EI_ECORE_PCS_RX_8B10B_DEC_ERR0_t RX_8B10B_DEC_ERR0;             /* addr: 0x01a6 */
  cs_uint16                                rsrvd5[9];
  ARN5_GIGE_EI_ECORE_TOP_Reset_t           Reset;                         /* addr: 0x01b0 */
  ARN5_GIGE_EI_ECORE_TOP_ModeControl_t     ModeControl;                   /* addr: 0x01b1 */
  ARN5_GIGE_EI_ECORE_TOP_Control_t         Control;                       /* addr: 0x01b2 */
  ARN5_GIGE_EI_ECORE_TOP_INTERRUPT_t       INTERRUPT;                     /* addr: 0x01b3 */
  ARN5_GIGE_EI_ECORE_TOP_INTENABLE_t       INTENABLE;                     /* addr: 0x01b4 */
  ARN5_GIGE_EI_ECORE_TOP_INTSTATUS_t       INTSTATUS;                     /* addr: 0x01b5 */
  ARN5_GIGE_EI_ECORE_TOP_MODESTATUS_t      MODESTATUS;                    /* addr: 0x01b6 */
  cs_uint16                                rsrvd6;
  ARN5_GIGE_EI_ECORE_TOP_UFLOW_PROTECT_t   UFLOW_PROTECT;                 /* addr: 0x01b8 */
} ARN5_ecore_t;

/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * EI::top
 */

#define ARN5_GIGE_EI_TOP_ETHERNET_INTERRUPT      0x161c0
#define ARN5_GIGE_EI_TOP_ETHERNET_INTERRUPT_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 EI_ECOREi            :  1 ; /* bits 1:1 */
    cs_uint16 EI_LINEi             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 EI_LINEi             :  1 ; /* bits 0:0 */
    cs_uint16 EI_ECOREi            :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_TOP_ETHERNET_INTERRUPT_t;

#define ARN5_GIGE_EI_TOP_ETHERNET_INTENABLE      0x161c1
#define ARN5_GIGE_EI_TOP_ETHERNET_INTENABLE_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 EI_ECOREe            :  1 ; /* bits 1:1 */
    cs_uint16 EI_LINEe             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 EI_LINEe             :  1 ; /* bits 0:0 */
    cs_uint16 EI_ECOREe            :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_TOP_ETHERNET_INTENABLE_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * EI::line
 */

#define ARN5_GIGE_EI_LINE_RX_LOCK_OFFSET         0x161d0
#define ARN5_GIGE_EI_LINE_RX_LOCK_OFFSET_dft     0x0021
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 LOCK_OFF             :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOCK_OFF             :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_RX_LOCK_OFFSET_t;

#define ARN5_GIGE_EI_LINE_RX_LOCK_THRESHOLD      0x161d1
#define ARN5_GIGE_EI_LINE_RX_LOCK_THRESHOLD_dft  0x0011
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 LOCK_THR             :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOCK_THR             :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_RX_LOCK_THRESHOLD_t;

#define ARN5_GIGE_EI_LINE_RX_LOCK_RESOL          0x161d2
#define ARN5_GIGE_EI_LINE_RX_LOCK_RESOL_dft      0x0003
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 LOCK_RES             :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOCK_RES             :  2 ; /* bits 1:0 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_RX_LOCK_RESOL_t;

#define ARN5_GIGE_EI_LINE_RX_DATA_CTRL           0x161d3
#define ARN5_GIGE_EI_LINE_RX_DATA_CTRL_dft       0x0010
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 RX_HI_INPUT          :  1 ; /* bits 14:14 */
    cs_uint16 RX_EQADJ             :  2 ; /* bits 13:12 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 RX_DATADL            :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 RX_DATA_LOCK         :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 RX_FORCE_LOCK        :  1 ; /* bits 1:1 */
    cs_uint16 RX_PD_MODE           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RX_PD_MODE           :  1 ; /* bits 0:0 */
    cs_uint16 RX_FORCE_LOCK        :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 RX_DATA_LOCK         :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 RX_DATADL            :  3 ; /* bits 10:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 RX_EQADJ             :  2 ; /* bits 13:12 */
    cs_uint16 RX_HI_INPUT          :  1 ; /* bits 14:14 */
    cs_uint16 rsrvd4               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_RX_DATA_CTRL_t;
#define     ARN5_RX_EQADJ_Z0                              0x00
#define     ARN5_RX_EQADJ_Z1                              0x01
#define     ARN5_RX_EQADJ_Z2                              0x02
#define     ARN5_RX_EQADJ_Z3                              0x03

#define ARN5_GIGE_EI_LINE_RX_CHARGE_PUMP         0x161d4
#define ARN5_GIGE_EI_LINE_RX_CHARGE_PUMP_dft     0xf72f
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 RX_PFD_FORCE_UPB     :  1 ; /* bits 15:15 */
    cs_uint16 RX_PFD_FORCE_DNB     :  1 ; /* bits 14:14 */
    cs_uint16 RX_PHSEL_RES         :  1 ; /* bits 13:13 */
    cs_uint16 RX_PH_SEL            :  5 ; /* bits 12:8 */
    cs_uint16 RX_CPCURR            :  4 ; /* bits 7:4 */
    cs_uint16 RX_CPOFF             :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RX_CPOFF             :  4 ; /* bits 3:0 */
    cs_uint16 RX_CPCURR            :  4 ; /* bits 7:4 */
    cs_uint16 RX_PH_SEL            :  5 ; /* bits 12:8 */
    cs_uint16 RX_PHSEL_RES         :  1 ; /* bits 13:13 */
    cs_uint16 RX_PFD_FORCE_DNB     :  1 ; /* bits 14:14 */
    cs_uint16 RX_PFD_FORCE_UPB     :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_RX_CHARGE_PUMP_t;
#define     ARN5_RX_PFD_FORCE_UPB_frc                     0x00
#define     ARN5_RX_PFD_FORCE_UPB_nofrc                   0x01
#define     ARN5_RX_PFD_FORCE_DNB_frc                     0x00
#define     ARN5_RX_PFD_FORCE_DNB_nofrc                   0x01
#define     ARN5_RX_PHSEL_RES_HI                          0x00
#define     ARN5_RX_PHSEL_RES_LO                          0x01

#define ARN5_GIGE_EI_LINE_RX_LOOP_FILTER         0x161d5
#define ARN5_GIGE_EI_LINE_RX_LOOP_FILTER_dft     0x0023
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  8 ;
    cs_uint16 RX_FILTSEL           :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 RX_FILTINT           :  1 ; /* bits 5:5 */
    cs_uint16 RX_FILTEN            :  1 ; /* bits 4:4 */
    cs_uint16 RX_FILTR             :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RX_FILTR             :  4 ; /* bits 3:0 */
    cs_uint16 RX_FILTEN            :  1 ; /* bits 4:4 */
    cs_uint16 RX_FILTINT           :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 RX_FILTSEL           :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd2               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_RX_LOOP_FILTER_t;
#define     ARN5_RX_FILTSEL_filti                         0x00
#define     ARN5_RX_FILTSEL_filto                         0x01
#define     ARN5_RX_FILTINT_disconn                       0x00
#define     ARN5_RX_FILTINT_conn                          0x01
#define     ARN5_RX_FILTEN_con                            0x00
#define     ARN5_RX_FILTEN_dis                            0x00

#define ARN5_GIGE_EI_LINE_SERDES_CONFIGURATION   0x161d6
#define ARN5_GIGE_EI_LINE_SERDES_CONFIGURATION_dft 0x2070
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd5               :  2 ;
    cs_uint16 TX_LPTIME_DIV_SEL    :  1 ; /* bits 13:13 */
    cs_uint16 TX_LPTIME_EN         :  1 ; /* bits 12:12 */
    cs_uint16 TX_CLKINV            :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 RX_PILOT_DIV_SEL     :  1 ; /* bits 9:9 */
    cs_uint16 RX_PLL_DIV_SEL       :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 RX_CLKEN             :  1 ; /* bits 6:6 */
    cs_uint16 RX_CLKINV            :  1 ; /* bits 5:5 */
    cs_uint16 TX_CLKEN             :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 RX_AMP_PD            :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               :  2 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 RX_AMP_PD            :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 TX_CLKEN             :  1 ; /* bits 4:4 */
    cs_uint16 RX_CLKINV            :  1 ; /* bits 5:5 */
    cs_uint16 RX_CLKEN             :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 RX_PLL_DIV_SEL       :  1 ; /* bits 8:8 */
    cs_uint16 RX_PILOT_DIV_SEL     :  1 ; /* bits 9:9 */
    cs_uint16 rsrvd4               :  1 ;
    cs_uint16 TX_CLKINV            :  1 ; /* bits 11:11 */
    cs_uint16 TX_LPTIME_EN         :  1 ; /* bits 12:12 */
    cs_uint16 TX_LPTIME_DIV_SEL    :  1 ; /* bits 13:13 */
    cs_uint16 rsrvd5               :  2 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_SERDES_CONFIGURATION_t;
#define     ARN5_TX_LPTIME_DIV_SEL_625M                   0x00
#define     ARN5_TX_LPTIME_DIV_SEL_1250M                  0x01
#define     ARN5_TX_LPTIME_EN_Local                       0x00
#define     ARN5_TX_LPTIME_EN_Loop                        0x01
#define     ARN5_RX_PILOT_DIV_SEL_Div10                   0x00
#define     ARN5_RX_PILOT_DIV_SEL_Div20                   0x01
#define     ARN5_RX_PLL_DIV_SEL_Div5                      0x00
#define     ARN5_RX_PLL_DIV_SEL_Div10                     0x01

#define ARN5_GIGE_EI_LINE_TEST_CTRL              0x161d7
#define ARN5_GIGE_EI_LINE_TEST_CTRL_dft          0x0030
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  6 ;
    cs_uint16 TX_TCKO_SEL          :  2 ; /* bits 9:8 */
    cs_uint16 TX_TCKO_EN           :  1 ; /* bits 7:7 */
    cs_uint16 RX_TCKO_EN           :  1 ; /* bits 6:6 */
    cs_uint16 RX_TCKO_SEL          :  4 ; /* bits 5:2 */
    cs_uint16 RX_TCKI_EN           :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 RX_TCKI_EN           :  1 ; /* bits 1:1 */
    cs_uint16 RX_TCKO_SEL          :  4 ; /* bits 5:2 */
    cs_uint16 RX_TCKO_EN           :  1 ; /* bits 6:6 */
    cs_uint16 TX_TCKO_EN           :  1 ; /* bits 7:7 */
    cs_uint16 TX_TCKO_SEL          :  2 ; /* bits 9:8 */
    cs_uint16 rsrvd2               :  6 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_TEST_CTRL_t;
#define     ARN5_TX_TCKO_SEL_rx_tcko                      0x00
#define     ARN5_TX_TCKO_SEL_tx_ck1p25g                   0x01
#define     ARN5_TX_TCKO_SEL_tx_clk                       0x02
#define     ARN5_TX_TCKO_SEL_tx_clk_div2                  0x03
#define     ARN5_TX_TCKO_EN_tx_ck625m                     0x00
#define     ARN5_TX_TCKO_EN_tx_tcko                       0x01
#define     ARN5_RX_TCKO_SEL_rx_pilot                     0x00
#define     ARN5_RX_TCKO_SEL_rx_clk                       0x00
#define     ARN5_RX_TCKO_SEL_rx_vco_div                   0x00
#define     ARN5_RX_TCKO_SEL_rx_vco_div2                  0x00
#define     ARN5_RX_TCKO_SEL_rx_ck625m                    0x0c
#define     ARN5_RX_TCKO_SEL_rx_rtd0                      0x0d
#define     ARN5_RX_TCKO_SEL_rx_rtd1                      0x0e
#define     ARN5_RX_TCKI_EN_VCO                           0x00
#define     ARN5_RX_TCKI_EN_PLL                           0x01

#define ARN5_GIGE_EI_LINE_TX_OUTPUT_CTRL         0x161d8
#define ARN5_GIGE_EI_LINE_TX_OUTPUT_CTRL_dft     0x0048
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  8 ;
    cs_uint16 TX_D_CM_DISABLE      :  1 ; /* bits 7:7 */
    cs_uint16 TX_D_LEVEL           :  5 ; /* bits 6:2 */
    cs_uint16 rsrvd1               :  2 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 TX_D_LEVEL           :  5 ; /* bits 6:2 */
    cs_uint16 TX_D_CM_DISABLE      :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd2               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_TX_OUTPUT_CTRL_t;

#define ARN5_GIGE_EI_LINE_VCO_FREQ               0x161d9
#define ARN5_GIGE_EI_LINE_VCO_FREQ_dft           0x0400
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  4 ;
    cs_uint16 RX_FREQ              :  4 ; /* bits 11:8 */
    cs_uint16 rsrvd1               :  8 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 RX_FREQ              :  4 ; /* bits 11:8 */
    cs_uint16 rsrvd2               :  4 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_VCO_FREQ_t;

#define ARN5_GIGE_EI_LINE_SPARE                  0x161da
#define ARN5_GIGE_EI_LINE_SPARE_dft              0x1800
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 RX_SPARE             :  5 ; /* bits 12:8 */
    cs_uint16 rsrvd1               :  8 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 RX_SPARE             :  5 ; /* bits 12:8 */
    cs_uint16 rsrvd2               :  3 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_SPARE_t;

#define ARN5_GIGE_EI_LINE_TPATCTRL               0x161db
#define ARN5_GIGE_EI_LINE_TPATCTRL_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 TPAT_EN              :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 TPAT                 : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TPAT                 : 10 ; /* bits 9:0 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 TPAT_EN              :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_TPATCTRL_t;

#define ARN5_GIGE_EI_LINE_PRBSCTRL               0x161dc
#define ARN5_GIGE_EI_LINE_PRBSCTRL_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 10 ;
    cs_uint16 PRBS_RX_ENABLE       :  1 ; /* bits 5:5 */
    cs_uint16 PRBS_RX_INVERT       :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 PRBS_TX_ENABLE       :  1 ; /* bits 1:1 */
    cs_uint16 PRBS_TX_INVERT       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PRBS_TX_INVERT       :  1 ; /* bits 0:0 */
    cs_uint16 PRBS_TX_ENABLE       :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 PRBS_RX_INVERT       :  1 ; /* bits 4:4 */
    cs_uint16 PRBS_RX_ENABLE       :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd2               : 10 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_PRBSCTRL_t;

#define ARN5_GIGE_EI_LINE_PRBSERRCTRL0           0x161dd
#define ARN5_GIGE_EI_LINE_PRBSERRCTRL0_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 PRBS_RX_LOAD         :  1 ; /* bits 2:2 */
    cs_uint16 PRBS_WORD_ERR_INJ    :  1 ; /* bits 1:1 */
    cs_uint16 PRBS_BIT_ERR_INJ     :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PRBS_BIT_ERR_INJ     :  1 ; /* bits 0:0 */
    cs_uint16 PRBS_WORD_ERR_INJ    :  1 ; /* bits 1:1 */
    cs_uint16 PRBS_RX_LOAD         :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_PRBSERRCTRL0_t;

#define ARN5_GIGE_EI_LINE_PRBSERRCTRL1           0x161de
#define ARN5_GIGE_EI_LINE_PRBSERRCTRL1_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 PRBS_WORD_ERR_INJ_ONCE :  1 ; /* bits 1:1 */
    cs_uint16 PRBS_BIT_ERR_INJ_ONCE :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PRBS_BIT_ERR_INJ_ONCE :  1 ; /* bits 0:0 */
    cs_uint16 PRBS_WORD_ERR_INJ_ONCE :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_PRBSERRCTRL1_t;

#define ARN5_GIGE_EI_LINE_PRBS_ERRCNT1           0x161df
#define ARN5_GIGE_EI_LINE_PRBS_ERRCNT1_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_PRBS_ERRCNT1_t;

#define ARN5_GIGE_EI_LINE_PRBS_ERRCNT0           0x161e0
#define ARN5_GIGE_EI_LINE_PRBS_ERRCNT0_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 count                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_PRBS_ERRCNT0_t;

#define ARN5_GIGE_EI_LINE_STATUS                 0x161e1
#define ARN5_GIGE_EI_LINE_STATUS_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  4 ;
    cs_uint16 TxLnPAIs             :  1 ; /* bits 11:11 */
    cs_uint16 TxLBPAIs             :  1 ; /* bits 10:10 */
    cs_uint16 RxPCPAIs             :  1 ; /* bits 9:9 */
    cs_uint16 RxLBPAIs             :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 PRBS_RX_EFs          :  1 ; /* bits 4:4 */
    cs_uint16 PRBS_NOSYNCs         :  1 ; /* bits 3:3 */
    cs_uint16 NO_DATAs             :  1 ; /* bits 2:2 */
    cs_uint16 IN_LOCKs             :  1 ; /* bits 1:1 */
    cs_uint16 RX_LOCKDETIs         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RX_LOCKDETIs         :  1 ; /* bits 0:0 */
    cs_uint16 IN_LOCKs             :  1 ; /* bits 1:1 */
    cs_uint16 NO_DATAs             :  1 ; /* bits 2:2 */
    cs_uint16 PRBS_NOSYNCs         :  1 ; /* bits 3:3 */
    cs_uint16 PRBS_RX_EFs          :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 RxLBPAIs             :  1 ; /* bits 8:8 */
    cs_uint16 RxPCPAIs             :  1 ; /* bits 9:9 */
    cs_uint16 TxLBPAIs             :  1 ; /* bits 10:10 */
    cs_uint16 TxLnPAIs             :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd2               :  4 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_STATUS_t;

#define ARN5_GIGE_EI_LINE_MASK                   0x161e2
#define ARN5_GIGE_EI_LINE_MASK_dft               0x0f1f
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  4 ;
    cs_uint16 TxLnPAIm             :  1 ; /* bits 11:11 */
    cs_uint16 TxLBPAIm             :  1 ; /* bits 10:10 */
    cs_uint16 RxPCPAIm             :  1 ; /* bits 9:9 */
    cs_uint16 RxLBPAIm             :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 PRBS_RX_EFm          :  1 ; /* bits 4:4 */
    cs_uint16 PRBS_NOSYNCm         :  1 ; /* bits 3:3 */
    cs_uint16 NO_DATAm             :  1 ; /* bits 2:2 */
    cs_uint16 IN_LOCKm             :  1 ; /* bits 1:1 */
    cs_uint16 RX_LOCKDETIm         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RX_LOCKDETIm         :  1 ; /* bits 0:0 */
    cs_uint16 IN_LOCKm             :  1 ; /* bits 1:1 */
    cs_uint16 NO_DATAm             :  1 ; /* bits 2:2 */
    cs_uint16 PRBS_NOSYNCm         :  1 ; /* bits 3:3 */
    cs_uint16 PRBS_RX_EFm          :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 RxLBPAIm             :  1 ; /* bits 8:8 */
    cs_uint16 RxPCPAIm             :  1 ; /* bits 9:9 */
    cs_uint16 TxLBPAIm             :  1 ; /* bits 10:10 */
    cs_uint16 TxLnPAIm             :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd2               :  4 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_MASK_t;

#define ARN5_GIGE_EI_LINE_INTERRUPT              0x161e3
#define ARN5_GIGE_EI_LINE_INTERRUPT_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  4 ;
    cs_uint16 TxLnPAIi             :  1 ; /* bits 11:11 */
    cs_uint16 TxLBPAIi             :  1 ; /* bits 10:10 */
    cs_uint16 RxPCPAIi             :  1 ; /* bits 9:9 */
    cs_uint16 RxLBPAIi             :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 PRBS_RX_EFi          :  1 ; /* bits 4:4 */
    cs_uint16 PRBS_NOSYNCi         :  1 ; /* bits 3:3 */
    cs_uint16 NO_DATAi             :  1 ; /* bits 2:2 */
    cs_uint16 IN_LOCKi             :  1 ; /* bits 1:1 */
    cs_uint16 RX_LOCKDETIi         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RX_LOCKDETIi         :  1 ; /* bits 0:0 */
    cs_uint16 IN_LOCKi             :  1 ; /* bits 1:1 */
    cs_uint16 NO_DATAi             :  1 ; /* bits 2:2 */
    cs_uint16 PRBS_NOSYNCi         :  1 ; /* bits 3:3 */
    cs_uint16 PRBS_RX_EFi          :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 RxLBPAIi             :  1 ; /* bits 8:8 */
    cs_uint16 RxPCPAIi             :  1 ; /* bits 9:9 */
    cs_uint16 TxLBPAIi             :  1 ; /* bits 10:10 */
    cs_uint16 TxLnPAIi             :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd2               :  4 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_INTERRUPT_t;

#define ARN5_GIGE_EI_LINE_ESTORE_RESYNC          0x161e4
#define ARN5_GIGE_EI_LINE_ESTORE_RESYNC_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 RX_DATA_RESYNC       :  1 ; /* bits 3:3 */
    cs_uint16 RX_LPBK_RESYNC       :  1 ; /* bits 2:2 */
    cs_uint16 TX_DATA_RESYNC       :  1 ; /* bits 1:1 */
    cs_uint16 TX_LPBK_RESYNC       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TX_LPBK_RESYNC       :  1 ; /* bits 0:0 */
    cs_uint16 TX_DATA_RESYNC       :  1 ; /* bits 1:1 */
    cs_uint16 RX_LPBK_RESYNC       :  1 ; /* bits 2:2 */
    cs_uint16 RX_DATA_RESYNC       :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_ESTORE_RESYNC_t;

#define ARN5_GIGE_EI_LINE_MISCCTRL2              0x161e5
#define ARN5_GIGE_EI_LINE_MISCCTRL2_dft          0x0011
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  2 ;
    cs_uint16 TX2RX_LB             :  1 ; /* bits 13:13 */
    cs_uint16 RX2TX_LB             :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd3               :  3 ;
    cs_uint16 TX_CLK_LB_EN         :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 RX_BSWAP             :  1 ; /* bits 6:6 */
    cs_uint16 RX_DATA_INV          :  1 ; /* bits 5:5 */
    cs_uint16 RX_CLK_EN            :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 TX_BSWAP             :  1 ; /* bits 2:2 */
    cs_uint16 TX_DATA_INV          :  1 ; /* bits 1:1 */
    cs_uint16 TX_CLK_EN            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TX_CLK_EN            :  1 ; /* bits 0:0 */
    cs_uint16 TX_DATA_INV          :  1 ; /* bits 1:1 */
    cs_uint16 TX_BSWAP             :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 RX_CLK_EN            :  1 ; /* bits 4:4 */
    cs_uint16 RX_DATA_INV          :  1 ; /* bits 5:5 */
    cs_uint16 RX_BSWAP             :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 TX_CLK_LB_EN         :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd3               :  3 ;
    cs_uint16 RX2TX_LB             :  1 ; /* bits 12:12 */
    cs_uint16 TX2RX_LB             :  1 ; /* bits 13:13 */
    cs_uint16 rsrvd4               :  2 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_MISCCTRL2_t;

#define ARN5_GIGE_EI_LINE_RESET                  0x161e6
#define ARN5_GIGE_EI_LINE_RESET_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 RX_RST               :  1 ; /* bits 1:1 */
    cs_uint16 TX_RST               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TX_RST               :  1 ; /* bits 0:0 */
    cs_uint16 RX_RST               :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_RESET_t;

#define ARN5_GIGE_EI_LINE_MISCCTRL               0x161e7
#define ARN5_GIGE_EI_LINE_MISCCTRL_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 RX_LOOPBK            :  1 ; /* bits 1:1 */
    cs_uint16 RX_PD                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RX_PD                :  1 ; /* bits 0:0 */
    cs_uint16 RX_LOOPBK            :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_MISCCTRL_t;
#define     ARN5_RX_LOOPBK_LBOFF                          0x00
#define     ARN5_RX_LOOPBK_LBON                           0x01

#define ARN5_GIGE_EI_LINE_FMRO                   0x161e8
#define ARN5_GIGE_EI_LINE_FMRO_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 LocalEn              :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 ClkSel               :  4 ; /* bits 11:8 */
    cs_uint16 DataSel              :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 DataSel              :  8 ; /* bits 7:0 */
    cs_uint16 ClkSel               :  4 ; /* bits 11:8 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 LocalEn              :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_EI_LINE_FMRO_t;
#define     ARN5_DataSel_SEL_0                            0x00
#define     ARN5_DataSel_SEL_1                            0x01
#define     ARN5_DataSel_SEL_2                            0x02
#define     ARN5_DataSel_SEL_3                            0x03
#define     ARN5_DataSel_SEL_4                            0x04
#define     ARN5_DataSel_SEL_5                            0x05
#define     ARN5_DataSel_SEL_6                            0x06
#define     ARN5_DataSel_SEL_7                            0x07
#define     ARN5_DataSel_SEL_8                            0x08
#define     ARN5_DataSel_SEL_9                            0x09
#define     ARN5_DataSel_SEL_a                            0x0a
#define     ARN5_DataSel_SEL_b                            0x0b
#define     ARN5_DataSel_SEL_c                            0x0c
#define     ARN5_DataSel_SEL_d                            0x0d
#define     ARN5_ClkSel_CSEL_0                            0x00
#define     ARN5_ClkSel_CSEL_1                            0x01
#define     ARN5_ClkSel_CSEL_2                            0x02
#define     ARN5_ClkSel_CSEL_3                            0x03
#define     ARN5_ClkSel_CSEL_4                            0x04
#define     ARN5_ClkSel_CSEL_5                            0x05
#define     ARN5_ClkSel_CSEL_6                            0x06
#define     ARN5_ClkSel_CSEL_7                            0x07
#define     ARN5_ClkSel_CSEL_8                            0x08
#define     ARN5_ClkSel_CSEL_9                            0x09
#define     ARN5_ClkSel_CSEL_A                            0x0a
#define     ARN5_ClkSel_CSEL_B                            0x0b
#define     ARN5_ClkSel_CSEL_C                            0x0c
#define     ARN5_ClkSel_CSEL_D                            0x0d
#define     ARN5_ClkSel_CSEL_E                            0x0e
#define     ARN5_ClkSel_CSEL_F                            0x0f


/******************************************************************
 *
 * Block declaration.
 *
 ******************************************************************/

/* EI */

typedef struct {
  ARN5_ecore_t                             ecore;                         /* addr: 0x00000000 */
  cs_uint16                                rsrvd1[7];
  ARN5_GIGE_EI_TOP_ETHERNET_INTERRUPT_t    ETHERNET_INTERRUPT;            /* addr: 0x01c0 */
  ARN5_GIGE_EI_TOP_ETHERNET_INTENABLE_t    ETHERNET_INTENABLE;            /* addr: 0x01c1 */
  cs_uint16                                rsrvd2[14];
  ARN5_GIGE_EI_LINE_RX_LOCK_OFFSET_t       RX_LOCK_OFFSET;                /* addr: 0x01d0 */
  ARN5_GIGE_EI_LINE_RX_LOCK_THRESHOLD_t    RX_LOCK_THRESHOLD;             /* addr: 0x01d1 */
  ARN5_GIGE_EI_LINE_RX_LOCK_RESOL_t        RX_LOCK_RESOL;                 /* addr: 0x01d2 */
  ARN5_GIGE_EI_LINE_RX_DATA_CTRL_t         RX_DATA_CTRL;                  /* addr: 0x01d3 */
  ARN5_GIGE_EI_LINE_RX_CHARGE_PUMP_t       RX_CHARGE_PUMP;                /* addr: 0x01d4 */
  ARN5_GIGE_EI_LINE_RX_LOOP_FILTER_t       RX_LOOP_FILTER;                /* addr: 0x01d5 */
  ARN5_GIGE_EI_LINE_SERDES_CONFIGURATION_t SERDES_CONFIGURATION;          /* addr: 0x01d6 */
  ARN5_GIGE_EI_LINE_TEST_CTRL_t            TEST_CTRL;                     /* addr: 0x01d7 */
  ARN5_GIGE_EI_LINE_TX_OUTPUT_CTRL_t       TX_OUTPUT_CTRL;                /* addr: 0x01d8 */
  ARN5_GIGE_EI_LINE_VCO_FREQ_t             VCO_FREQ;                      /* addr: 0x01d9 */
  ARN5_GIGE_EI_LINE_SPARE_t                SPARE;                         /* addr: 0x01da */
  ARN5_GIGE_EI_LINE_TPATCTRL_t             TPATCTRL;                      /* addr: 0x01db */
  ARN5_GIGE_EI_LINE_PRBSCTRL_t             PRBSCTRL;                      /* addr: 0x01dc */
  ARN5_GIGE_EI_LINE_PRBSERRCTRL0_t         PRBSERRCTRL0;                  /* addr: 0x01dd */
  ARN5_GIGE_EI_LINE_PRBSERRCTRL1_t         PRBSERRCTRL1;                  /* addr: 0x01de */
  ARN5_GIGE_EI_LINE_PRBS_ERRCNT1_t         PRBS_ERRCNT1;                  /* addr: 0x01df */
  ARN5_GIGE_EI_LINE_PRBS_ERRCNT0_t         PRBS_ERRCNT0;                  /* addr: 0x01e0 */
  ARN5_GIGE_EI_LINE_STATUS_t               STATUS;                        /* addr: 0x01e1 */
  ARN5_GIGE_EI_LINE_MASK_t                 MASK;                          /* addr: 0x01e2 */
  ARN5_GIGE_EI_LINE_INTERRUPT_t            INTERRUPT;                     /* addr: 0x01e3 */
  ARN5_GIGE_EI_LINE_ESTORE_RESYNC_t        ESTORE_RESYNC;                 /* addr: 0x01e4 */
  ARN5_GIGE_EI_LINE_MISCCTRL2_t            MISCCTRL2;                     /* addr: 0x01e5 */
  ARN5_GIGE_EI_LINE_RESET_t                RESET;                         /* addr: 0x01e6 */
  ARN5_GIGE_EI_LINE_MISCCTRL_t             MISCCTRL;                      /* addr: 0x01e7 */
  ARN5_GIGE_EI_LINE_FMRO_t                 FMRO;                          /* addr: 0x01e8 */
} ARN5_EI_t;

/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * GigE::misc
 */

#define ARN5_GIGE_MISC_INTERRUPT                 0x161f0
#define ARN5_GIGE_MISC_INTERRUPT_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 EI_CONVi             :  1 ; /* bits 1:1 */
    cs_uint16 EIi                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 EIi                  :  1 ; /* bits 0:0 */
    cs_uint16 EI_CONVi             :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_MISC_INTERRUPT_t;

#define ARN5_GIGE_MISC_INTENABLE                 0x161f1
#define ARN5_GIGE_MISC_INTENABLE_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 EI_CONVe             :  1 ; /* bits 1:1 */
    cs_uint16 EIe                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 EIe                  :  1 ; /* bits 0:0 */
    cs_uint16 EI_CONVe             :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_MISC_INTENABLE_t;

#define ARN5_GIGE_MISC_CONV_INTERRUPT            0x161f2
#define ARN5_GIGE_MISC_CONV_INTERRUPT_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  5 ;
    cs_uint16 TX_SHORT_DROP_MSBi   :  1 ; /* bits 10:10 */
    cs_uint16 TX_SHORT_DROPi       :  1 ; /* bits 9:9 */
    cs_uint16 TX_FIFO_UNDERFLOWi   :  1 ; /* bits 8:8 */
    cs_uint16 TX_FIFO_PAR_ERRi     :  1 ; /* bits 7:7 */
    cs_uint16 TX_OVERFLOWi         :  1 ; /* bits 6:6 */
    cs_uint16 TX_BADFRAMEi         :  1 ; /* bits 5:5 */
    cs_uint16 TX_MEM_PAR_ERRi      :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 RX_DROPi             :  1 ; /* bits 1:1 */
    cs_uint16 RX_ABORTi            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RX_ABORTi            :  1 ; /* bits 0:0 */
    cs_uint16 RX_DROPi             :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 TX_MEM_PAR_ERRi      :  1 ; /* bits 4:4 */
    cs_uint16 TX_BADFRAMEi         :  1 ; /* bits 5:5 */
    cs_uint16 TX_OVERFLOWi         :  1 ; /* bits 6:6 */
    cs_uint16 TX_FIFO_PAR_ERRi     :  1 ; /* bits 7:7 */
    cs_uint16 TX_FIFO_UNDERFLOWi   :  1 ; /* bits 8:8 */
    cs_uint16 TX_SHORT_DROPi       :  1 ; /* bits 9:9 */
    cs_uint16 TX_SHORT_DROP_MSBi   :  1 ; /* bits 10:10 */
    cs_uint16 rsrvd2               :  5 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_MISC_CONV_INTERRUPT_t;

#define ARN5_GIGE_MISC_CONV_INTENABLE            0x161f3
#define ARN5_GIGE_MISC_CONV_INTENABLE_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  5 ;
    cs_uint16 TX_SHORT_DROP_MSBe   :  1 ; /* bits 10:10 */
    cs_uint16 TX_SHORT_DROPe       :  1 ; /* bits 9:9 */
    cs_uint16 TX_FIFO_UNDERFLOWe   :  1 ; /* bits 8:8 */
    cs_uint16 TX_FIFO_PAR_ERRe     :  1 ; /* bits 7:7 */
    cs_uint16 TX_OVERFLOWe         :  1 ; /* bits 6:6 */
    cs_uint16 TX_BADFRAMEe         :  1 ; /* bits 5:5 */
    cs_uint16 TX_MEM_PAR_ERRe      :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 RX_DROPe             :  1 ; /* bits 1:1 */
    cs_uint16 RX_ABORTe            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RX_ABORTe            :  1 ; /* bits 0:0 */
    cs_uint16 RX_DROPe             :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 TX_MEM_PAR_ERRe      :  1 ; /* bits 4:4 */
    cs_uint16 TX_BADFRAMEe         :  1 ; /* bits 5:5 */
    cs_uint16 TX_OVERFLOWe         :  1 ; /* bits 6:6 */
    cs_uint16 TX_FIFO_PAR_ERRe     :  1 ; /* bits 7:7 */
    cs_uint16 TX_FIFO_UNDERFLOWe   :  1 ; /* bits 8:8 */
    cs_uint16 TX_SHORT_DROPe       :  1 ; /* bits 9:9 */
    cs_uint16 TX_SHORT_DROP_MSBe   :  1 ; /* bits 10:10 */
    cs_uint16 rsrvd2               :  5 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_MISC_CONV_INTENABLE_t;

#define ARN5_GIGE_MISC_CONV_TX_CONFIG            0x161f4
#define ARN5_GIGE_MISC_CONV_TX_CONFIG_dft        0x1020
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 spi41ConvReset       :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 packetSkid           :  6 ; /* bits 13:8 */
    cs_uint16 evenParity           :  1 ; /* bits 7:7 */
    cs_uint16 hideParity           :  1 ; /* bits 6:6 */
    cs_uint16 Thr                  :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Thr                  :  6 ; /* bits 5:0 */
    cs_uint16 hideParity           :  1 ; /* bits 6:6 */
    cs_uint16 evenParity           :  1 ; /* bits 7:7 */
    cs_uint16 packetSkid           :  6 ; /* bits 13:8 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 spi41ConvReset       :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_MISC_CONV_TX_CONFIG_t;

#define ARN5_GIGE_MISC_CONV_TX_MINPKTSIZE        0x161f5
#define ARN5_GIGE_MISC_CONV_TX_MINPKTSIZE_dft    0x000e
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 value                :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 value                :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_MISC_CONV_TX_MINPKTSIZE_t;

#define ARN5_GIGE_MISC_CONFIG                    0x161f6
#define ARN5_GIGE_MISC_CONFIG_dft                0x0080
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 TX_PAUSE_SRC_SEL     :  2 ; /* bits 7:6 */
    cs_uint16 GPIO_ALARM_STAT_SEL  :  2 ; /* bits 5:4 */
    cs_uint16 RX_CONV_DROP_EN      :  1 ; /* bits 3:3 */
    cs_uint16 RPR_MODE             :  1 ; /* bits 2:2 */
    cs_uint16 RX_CRC_FWD           :  1 ; /* bits 1:1 */
    cs_uint16 TX_CRC_FWD           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TX_CRC_FWD           :  1 ; /* bits 0:0 */
    cs_uint16 RX_CRC_FWD           :  1 ; /* bits 1:1 */
    cs_uint16 RPR_MODE             :  1 ; /* bits 2:2 */
    cs_uint16 RX_CONV_DROP_EN      :  1 ; /* bits 3:3 */
    cs_uint16 GPIO_ALARM_STAT_SEL  :  2 ; /* bits 5:4 */
    cs_uint16 TX_PAUSE_SRC_SEL     :  2 ; /* bits 7:6 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_MISC_CONFIG_t;
#define     ARN5_TX_PAUSE_SRC_SEL_SI_BP                   0x00
#define     ARN5_TX_PAUSE_SRC_SEL_PAUSE_IF                0x01
#define     ARN5_TX_PAUSE_SRC_SEL_NOPAUSE                 0x02
#define     ARN5_GPIO_ALARM_STAT_SEL_LINKSTAT             0x00
#define     ARN5_GPIO_ALARM_STAT_SEL_TXACTV               0x01
#define     ARN5_GPIO_ALARM_STAT_SEL_RXACTV               0x02

#define ARN5_GIGE_MISC_CONV_RX_DROPCOUNT         0x161f7
#define ARN5_GIGE_MISC_CONV_RX_DROPCOUNT_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 cnt                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 cnt                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_MISC_CONV_RX_DROPCOUNT_t;

#define ARN5_GIGE_MISC_CONV_RX_ABORTCOUNT        0x161f8
#define ARN5_GIGE_MISC_CONV_RX_ABORTCOUNT_dft    0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 cnt                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 cnt                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_MISC_CONV_RX_ABORTCOUNT_t;

#define ARN5_GIGE_MISC_CONV_TX_DROPCOUNT1        0x161f9
#define ARN5_GIGE_MISC_CONV_TX_DROPCOUNT1_dft    0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 cnt                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 cnt                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_MISC_CONV_TX_DROPCOUNT1_t;

#define ARN5_GIGE_MISC_CONV_TX_DROPCOUNT0        0x161fa
#define ARN5_GIGE_MISC_CONV_TX_DROPCOUNT0_dft    0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 cnt                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 cnt                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_MISC_CONV_TX_DROPCOUNT0_t;

#define ARN5_GIGE_MISC_MBIST_FAIL                0x161fb
#define ARN5_GIGE_MISC_MBIST_FAIL_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 CONV_TXFIFO          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 CONV_TXFIFO          :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_MISC_MBIST_FAIL_t;

#define ARN5_GIGE_MISC_MBIST_DONE                0x161fc
#define ARN5_GIGE_MISC_MBIST_DONE_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 CONV_TXFIFO          :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 CONV_TXFIFO          :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_GIGE_MISC_MBIST_DONE_t;


/******************************************************************
 *
 * Block declaration.
 *
 ******************************************************************/

/* GigE */

#define ARN5_GIGE_STRIDE                         131072
#define ARN5_GIGE_COUNT                          8
typedef struct {
  ARN5_EI_t                                EI;                            /* addr: 0x00000000 */
  cs_uint16                                rsrvd1[7];
  ARN5_GIGE_MISC_INTERRUPT_t               INTERRUPT;                     /* addr: 0x01f0 */
  ARN5_GIGE_MISC_INTENABLE_t               INTENABLE;                     /* addr: 0x01f1 */
  ARN5_GIGE_MISC_CONV_INTERRUPT_t          CONV_INTERRUPT;                /* addr: 0x01f2 */
  ARN5_GIGE_MISC_CONV_INTENABLE_t          CONV_INTENABLE;                /* addr: 0x01f3 */
  ARN5_GIGE_MISC_CONV_TX_CONFIG_t          CONV_TX_CONFIG;                /* addr: 0x01f4 */
  ARN5_GIGE_MISC_CONV_TX_MINPKTSIZE_t      CONV_TX_MINPKTSIZE;            /* addr: 0x01f5 */
  ARN5_GIGE_MISC_CONFIG_t                  CONFIG;                        /* addr: 0x01f6 */
  ARN5_GIGE_MISC_CONV_RX_DROPCOUNT_t       CONV_RX_DROPCOUNT;             /* addr: 0x01f7 */
  ARN5_GIGE_MISC_CONV_RX_ABORTCOUNT_t      CONV_RX_ABORTCOUNT;            /* addr: 0x01f8 */
  ARN5_GIGE_MISC_CONV_TX_DROPCOUNT1_t      CONV_TX_DROPCOUNT1;            /* addr: 0x01f9 */
  ARN5_GIGE_MISC_CONV_TX_DROPCOUNT0_t      CONV_TX_DROPCOUNT0;            /* addr: 0x01fa */
  ARN5_GIGE_MISC_MBIST_FAIL_t              MBIST_FAIL;                    /* addr: 0x01fb */
  ARN5_GIGE_MISC_MBIST_DONE_t              MBIST_DONE;                    /* addr: 0x01fc */
} ARN5_GigE_t;

/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * Framer::global
 */

#define ARN5_FR_GLOBAL_JTAGIdLsb                 0x18000
#define ARN5_FR_GLOBAL_JTAGIdLsb_dft             0x9049
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 IdLsb                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 IdLsb                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_JTAGIdLsb_t;

#define ARN5_FR_GLOBAL_JTAGIdMsb                 0x18001
#define ARN5_FR_GLOBAL_JTAGIdMsb_dft             0x2422
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 IdMsb                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 IdMsb                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_JTAGIdMsb_t;

#define ARN5_FR_GLOBAL_MasterReset               0x18004
#define ARN5_FR_GLOBAL_MasterReset_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 GlobalReset          :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd4               :  3 ;
    cs_uint16 RxReset              :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 TxReset              :  1 ; /* bits 9:9 */
    cs_uint16 DtReset              :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  4 ;
    cs_uint16 RxLogicReset         :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 TxLogicReset         :  1 ; /* bits 1:1 */
    cs_uint16 DtLogicReset         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 DtLogicReset         :  1 ; /* bits 0:0 */
    cs_uint16 TxLogicReset         :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 RxLogicReset         :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd2               :  4 ;
    cs_uint16 DtReset              :  1 ; /* bits 8:8 */
    cs_uint16 TxReset              :  1 ; /* bits 9:9 */
    cs_uint16 rsrvd3               :  1 ;
    cs_uint16 RxReset              :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd4               :  3 ;
    cs_uint16 GlobalReset          :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_MasterReset_t;

#define ARN5_FR_GLOBAL_MasterInt                 0x18010
#define ARN5_FR_GLOBAL_MasterInt_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 TxFrStartI           :  1 ; /* bits 15:15 */
    cs_uint16 APSI                 :  1 ; /* bits 14:14 */
    cs_uint16 ClkFailI             :  1 ; /* bits 13:13 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 TxI                  :  1 ; /* bits 11:11 */
    cs_uint16 DatacomTxI           :  1 ; /* bits 10:10 */
    cs_uint16 TxCellPI             :  1 ; /* bits 9:9 */
    cs_uint16 TxPktPI              :  1 ; /* bits 8:8 */
    cs_uint16 RxFrStartI           :  1 ; /* bits 7:7 */
    cs_uint16 MbusTimeoutI         :  1 ; /* bits 6:6 */
    cs_uint16 RxSI                 :  1 ; /* bits 5:5 */
    cs_uint16 RxLI                 :  1 ; /* bits 4:4 */
    cs_uint16 RxPI                 :  1 ; /* bits 3:3 */
    cs_uint16 ResetI               :  1 ; /* bits 2:2 */
    cs_uint16 RxCellPI             :  1 ; /* bits 1:1 */
    cs_uint16 RxPktPI              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RxPktPI              :  1 ; /* bits 0:0 */
    cs_uint16 RxCellPI             :  1 ; /* bits 1:1 */
    cs_uint16 ResetI               :  1 ; /* bits 2:2 */
    cs_uint16 RxPI                 :  1 ; /* bits 3:3 */
    cs_uint16 RxLI                 :  1 ; /* bits 4:4 */
    cs_uint16 RxSI                 :  1 ; /* bits 5:5 */
    cs_uint16 MbusTimeoutI         :  1 ; /* bits 6:6 */
    cs_uint16 RxFrStartI           :  1 ; /* bits 7:7 */
    cs_uint16 TxPktPI              :  1 ; /* bits 8:8 */
    cs_uint16 TxCellPI             :  1 ; /* bits 9:9 */
    cs_uint16 DatacomTxI           :  1 ; /* bits 10:10 */
    cs_uint16 TxI                  :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 ClkFailI             :  1 ; /* bits 13:13 */
    cs_uint16 APSI                 :  1 ; /* bits 14:14 */
    cs_uint16 TxFrStartI           :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_MasterInt_t;

#define ARN5_FR_GLOBAL_MasterMask                0x18014
#define ARN5_FR_GLOBAL_MasterMask_dft            0xefff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 TxFrStartM           :  1 ; /* bits 15:15 */
    cs_uint16 APSM                 :  1 ; /* bits 14:14 */
    cs_uint16 ClkFailM             :  1 ; /* bits 13:13 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 TxM                  :  1 ; /* bits 11:11 */
    cs_uint16 DatacomTxM           :  1 ; /* bits 10:10 */
    cs_uint16 TxCellPM             :  1 ; /* bits 9:9 */
    cs_uint16 TxPktPM              :  1 ; /* bits 8:8 */
    cs_uint16 RxFrStartM           :  1 ; /* bits 7:7 */
    cs_uint16 MbusTimeoutM         :  1 ; /* bits 6:6 */
    cs_uint16 RxSM                 :  1 ; /* bits 5:5 */
    cs_uint16 RxLM                 :  1 ; /* bits 4:4 */
    cs_uint16 RxPM                 :  1 ; /* bits 3:3 */
    cs_uint16 ResetM               :  1 ; /* bits 2:2 */
    cs_uint16 RxCellPM             :  1 ; /* bits 1:1 */
    cs_uint16 RxPktPM              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RxPktPM              :  1 ; /* bits 0:0 */
    cs_uint16 RxCellPM             :  1 ; /* bits 1:1 */
    cs_uint16 ResetM               :  1 ; /* bits 2:2 */
    cs_uint16 RxPM                 :  1 ; /* bits 3:3 */
    cs_uint16 RxLM                 :  1 ; /* bits 4:4 */
    cs_uint16 RxSM                 :  1 ; /* bits 5:5 */
    cs_uint16 MbusTimeoutM         :  1 ; /* bits 6:6 */
    cs_uint16 RxFrStartM           :  1 ; /* bits 7:7 */
    cs_uint16 TxPktPM              :  1 ; /* bits 8:8 */
    cs_uint16 TxCellPM             :  1 ; /* bits 9:9 */
    cs_uint16 DatacomTxM           :  1 ; /* bits 10:10 */
    cs_uint16 TxM                  :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 ClkFailM             :  1 ; /* bits 13:13 */
    cs_uint16 APSM                 :  1 ; /* bits 14:14 */
    cs_uint16 TxFrStartM           :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_MasterMask_t;

#define ARN5_FR_GLOBAL_MasterInt2                0x18015
#define ARN5_FR_GLOBAL_MasterInt2_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 StreamTx_GFP_I       :  1 ; /* bits 2:2 */
    cs_uint16 StreamRx_GFP_I       :  1 ; /* bits 1:1 */
    cs_uint16 DatacomRxI           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 DatacomRxI           :  1 ; /* bits 0:0 */
    cs_uint16 StreamRx_GFP_I       :  1 ; /* bits 1:1 */
    cs_uint16 StreamTx_GFP_I       :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_MasterInt2_t;

#define ARN5_FR_GLOBAL_MasterMask2               0x18016
#define ARN5_FR_GLOBAL_MasterMask2_dft           0x0007
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 StreamTx_GFP_M       :  1 ; /* bits 2:2 */
    cs_uint16 StreamRx_GFP_M       :  1 ; /* bits 1:1 */
    cs_uint16 DatacomRxM           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 DatacomRxM           :  1 ; /* bits 0:0 */
    cs_uint16 StreamRx_GFP_M       :  1 ; /* bits 1:1 */
    cs_uint16 StreamTx_GFP_M       :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_MasterMask2_t;

#define ARN5_FR_GLOBAL_ClockStatus               0x18020
#define ARN5_FR_GLOBAL_ClockStatus_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 RxClkS               :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd2               : 12 ;
    cs_uint16 DtTxClkS             :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 TxClkS               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TxClkS               :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 DtTxClkS             :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd2               : 12 ;
    cs_uint16 RxClkS               :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_ClockStatus_t;

#define ARN5_FR_GLOBAL_ClockInt                  0x18021
#define ARN5_FR_GLOBAL_ClockInt_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 RxClkI               :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd2               : 12 ;
    cs_uint16 DtTxClkI             :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 TxClkI               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TxClkI               :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 DtTxClkI             :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd2               : 12 ;
    cs_uint16 RxClkI               :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_ClockInt_t;

#define ARN5_FR_GLOBAL_ClockMask                 0x18022
#define ARN5_FR_GLOBAL_ClockMask_dft             0x8005
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 RxClkM               :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd2               : 12 ;
    cs_uint16 DtTxClkM             :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 TxClkM               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TxClkM               :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 DtTxClkM             :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd2               : 12 ;
    cs_uint16 RxClkM               :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_ClockMask_t;

#define ARN5_FR_GLOBAL_CellRxInt                 0x18028
#define ARN5_FR_GLOBAL_CellRxInt_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 CellRxI              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 CellRxI              :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_CellRxInt_t;

#define ARN5_FR_GLOBAL_CellRxMask                0x18029
#define ARN5_FR_GLOBAL_CellRxMask_dft            0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 CellRxM              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 CellRxM              :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_CellRxMask_t;

#define ARN5_FR_GLOBAL_PacketRxInt               0x1802a
#define ARN5_FR_GLOBAL_PacketRxInt_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 PacketRxI            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PacketRxI            :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_PacketRxInt_t;

#define ARN5_FR_GLOBAL_PacketRxMask              0x1802b
#define ARN5_FR_GLOBAL_PacketRxMask_dft          0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 PacketRxM            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PacketRxM            :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_PacketRxMask_t;

#define ARN5_FR_GLOBAL_CellTxInt                 0x1802c
#define ARN5_FR_GLOBAL_CellTxInt_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 CellTxI              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 CellTxI              :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_CellTxInt_t;

#define ARN5_FR_GLOBAL_CellTxMask                0x1802d
#define ARN5_FR_GLOBAL_CellTxMask_dft            0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 CellTxM              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 CellTxM              :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_CellTxMask_t;

#define ARN5_FR_GLOBAL_PacketTxInt               0x1802e
#define ARN5_FR_GLOBAL_PacketTxInt_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 PacketTxI            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PacketTxI            :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_PacketTxInt_t;

#define ARN5_FR_GLOBAL_PacketTxMask              0x1802f
#define ARN5_FR_GLOBAL_PacketTxMask_dft          0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 PacketTxM            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PacketTxM            :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_PacketTxMask_t;

#define ARN5_FR_GLOBAL_LoopBack                  0x18034
#define ARN5_FR_GLOBAL_LoopBack_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 loopTimeMode         :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 Terminal             :  1 ; /* bits 1:1 */
    cs_uint16 Line                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Line                 :  1 ; /* bits 0:0 */
    cs_uint16 Terminal             :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 loopTimeMode         :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_LoopBack_t;

#define ARN5_FR_GLOBAL_DebugPort                 0x18035
#define ARN5_FR_GLOBAL_DebugPort_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 MuxSel               :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 MuxSel               :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_DebugPort_t;

#define ARN5_FR_GLOBAL_Rx_Carrier_Msk            0x18036
#define ARN5_FR_GLOBAL_Rx_Carrier_Msk_dft        0x00ee
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Carrier_Act_High     :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 C_Mask_LO_TxS_Clk    :  1 ; /* bits 13:13 */
    cs_uint16 C_Mask_LO_RxS_Clk    :  1 ; /* bits 12:12 */
    cs_uint16 C_Mask_dLOS          :  1 ; /* bits 11:11 */
    cs_uint16 C_Mask_dLOF          :  1 ; /* bits 10:10 */
    cs_uint16 C_Mask_dSEF          :  1 ; /* bits 9:9 */
    cs_uint16 C_Mask_dAISL         :  1 ; /* bits 8:8 */
    cs_uint16 C_Mask_dLOP_vec      :  3 ; /* bits 7:5 */
    cs_uint16 C_Mask_dLOP          :  1 ; /* bits 4:4 */
    cs_uint16 C_Mask_dAISP_vec     :  3 ; /* bits 3:1 */
    cs_uint16 C_Mask_dAISP         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 C_Mask_dAISP         :  1 ; /* bits 0:0 */
    cs_uint16 C_Mask_dAISP_vec     :  3 ; /* bits 3:1 */
    cs_uint16 C_Mask_dLOP          :  1 ; /* bits 4:4 */
    cs_uint16 C_Mask_dLOP_vec      :  3 ; /* bits 7:5 */
    cs_uint16 C_Mask_dAISL         :  1 ; /* bits 8:8 */
    cs_uint16 C_Mask_dSEF          :  1 ; /* bits 9:9 */
    cs_uint16 C_Mask_dLOF          :  1 ; /* bits 10:10 */
    cs_uint16 C_Mask_dLOS          :  1 ; /* bits 11:11 */
    cs_uint16 C_Mask_LO_RxS_Clk    :  1 ; /* bits 12:12 */
    cs_uint16 C_Mask_LO_TxS_Clk    :  1 ; /* bits 13:13 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 Carrier_Act_High     :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_Rx_Carrier_Msk_t;

#define ARN5_FR_GLOBAL_LineSkewControl           0x18038
#define ARN5_FR_GLOBAL_LineSkewControl_dft       0x0c0c
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 TxCoarse             :  4 ; /* bits 13:10 */
    cs_uint16 TxFine               :  2 ; /* bits 9:8 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 RxCoarse             :  4 ; /* bits 5:2 */
    cs_uint16 RxFine               :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 RxFine               :  2 ; /* bits 1:0 */
    cs_uint16 RxCoarse             :  4 ; /* bits 5:2 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 TxFine               :  2 ; /* bits 9:8 */
    cs_uint16 TxCoarse             :  4 ; /* bits 13:10 */
    cs_uint16 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_LineSkewControl_t;

#define ARN5_FR_GLOBAL_DatacomSkewControl        0x18039
#define ARN5_FR_GLOBAL_DatacomSkewControl_dft    0x0c0c
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 DtTxCoarse           :  4 ; /* bits 13:10 */
    cs_uint16 DtTxFine             :  2 ; /* bits 9:8 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 DtRxCoarse           :  4 ; /* bits 5:2 */
    cs_uint16 DtRxFine             :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 DtRxFine             :  2 ; /* bits 1:0 */
    cs_uint16 DtRxCoarse           :  4 ; /* bits 5:2 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 DtTxFine             :  2 ; /* bits 9:8 */
    cs_uint16 DtTxCoarse           :  4 ; /* bits 13:10 */
    cs_uint16 rsrvd2               :  2 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GLOBAL_DatacomSkewControl_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * Framer::datacomRx
 */

#define ARN5_FR_DATACOMRX_Control                0x18200
#define ARN5_FR_DATACOMRX_Control_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 reset                :  1 ; /* bits 1:1 */
    cs_uint16 evenParity           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 evenParity           :  1 ; /* bits 0:0 */
    cs_uint16 reset                :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_DATACOMRX_Control_t;

#define ARN5_FR_DATACOMRX_Interrupt              0x18201
#define ARN5_FR_DATACOMRX_Interrupt_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 streamRxS            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 streamRxS            :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_DATACOMRX_Interrupt_t;

#define ARN5_FR_DATACOMRX_Mask                   0x18202
#define ARN5_FR_DATACOMRX_Mask_dft               0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 streamRxM            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 streamRxM            :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_DATACOMRX_Mask_t;

#define ARN5_FR_DATACOMRX_StrConfig              0x18240
#define ARN5_FR_DATACOMRX_StrConfig_dft          0x8000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 dataType             :  2 ; /* bits 15:14 */
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 burstLength          :  8 ; /* bits 11:4 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 reset                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 reset                :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 burstLength          :  8 ; /* bits 11:4 */
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 dataType             :  2 ; /* bits 15:14 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_DATACOMRX_StrConfig_t;
#define     ARN5_dataType_CELL                            0x00
#define     ARN5_dataType_PACKET                          0x02
#define     ARN5_dataType_CLEAR                           0x03

#define ARN5_FR_DATACOMRX_StrInt                 0x18241
#define ARN5_FR_DATACOMRX_StrInt_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 13 ;
    cs_uint16 FIFO_Par_Err_I       :  1 ; /* bits 2:2 */
    cs_uint16 badFrameI            :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 badFrameI            :  1 ; /* bits 1:1 */
    cs_uint16 FIFO_Par_Err_I       :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd2               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_DATACOMRX_StrInt_t;

#define ARN5_FR_DATACOMRX_StrMask                0x18242
#define ARN5_FR_DATACOMRX_StrMask_dft            0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 13 ;
    cs_uint16 FIFO_Par_Err_M       :  1 ; /* bits 2:2 */
    cs_uint16 badFrameM            :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 badFrameM            :  1 ; /* bits 1:1 */
    cs_uint16 FIFO_Par_Err_M       :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd2               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_DATACOMRX_StrMask_t;

#define ARN5_FR_DATACOMRX_StrStatus              0x18243
#define ARN5_FR_DATACOMRX_StrStatus_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 14 ;
    cs_uint16 badFrameS            :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 badFrameS            :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd2               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_DATACOMRX_StrStatus_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * Framer::gfp_rx
 */

#define ARN5_FR_GFP_RX_GFP_Control_0             0x18850
#define ARN5_FR_GFP_RX_GFP_Control_0_dft         0x0094
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 GFP_Reset            :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 GFP_Dis_crc_mask     :  1 ; /* bits 8:8 */
    cs_uint16 GFP_Error_Pkt_on_CRC_Err :  1 ; /* bits 7:7 */
    cs_uint16 GFP_Dis_1bit_Err_Corr :  1 ; /* bits 6:6 */
    cs_uint16 GFP_Alpha            :  3 ; /* bits 5:3 */
    cs_uint16 GFP_Delta            :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 GFP_Delta            :  3 ; /* bits 2:0 */
    cs_uint16 GFP_Alpha            :  3 ; /* bits 5:3 */
    cs_uint16 GFP_Dis_1bit_Err_Corr :  1 ; /* bits 6:6 */
    cs_uint16 GFP_Error_Pkt_on_CRC_Err :  1 ; /* bits 7:7 */
    cs_uint16 GFP_Dis_crc_mask     :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 GFP_Reset            :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_RX_GFP_Control_0_t;

#define ARN5_FR_GFP_RX_GFP_Control_1             0x18851
#define ARN5_FR_GFP_RX_GFP_Control_1_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 GFP_Dis_Header_XOR   :  1 ; /* bits 12:12 */
    cs_uint16 GFP_Dis_Core_Header_descr :  1 ; /* bits 11:11 */
    cs_uint16 GFP_Dis_Descrambler  :  1 ; /* bits 10:10 */
    cs_uint16 GFP_Pass_Core_Hdr    :  1 ; /* bits 9:9 */
    cs_uint16 GFP_Dis_Payload_Hdr_Proc :  1 ; /* bits 8:8 */
    cs_uint16 GFP_Pass_CM_Frame    :  1 ; /* bits 7:7 */
    cs_uint16 GFP_Dis_PTI          :  1 ; /* bits 6:6 */
    cs_uint16 GFP_Send_Payload_Hdr :  1 ; /* bits 5:5 */
    cs_uint16 GFP_Pass_FCS         :  1 ; /* bits 4:4 */
    cs_uint16 GFP_Dis_drop_if_thec_err :  1 ; /* bits 3:3 */
    cs_uint16 GFP_Sel_FCS_Order    :  1 ; /* bits 2:2 */
    cs_uint16 GFP_Drop_Short_pkts  :  1 ; /* bits 1:1 */
    cs_uint16 GFP_Drop_Long_pkts   :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 GFP_Drop_Long_pkts   :  1 ; /* bits 0:0 */
    cs_uint16 GFP_Drop_Short_pkts  :  1 ; /* bits 1:1 */
    cs_uint16 GFP_Sel_FCS_Order    :  1 ; /* bits 2:2 */
    cs_uint16 GFP_Dis_drop_if_thec_err :  1 ; /* bits 3:3 */
    cs_uint16 GFP_Pass_FCS         :  1 ; /* bits 4:4 */
    cs_uint16 GFP_Send_Payload_Hdr :  1 ; /* bits 5:5 */
    cs_uint16 GFP_Dis_PTI          :  1 ; /* bits 6:6 */
    cs_uint16 GFP_Pass_CM_Frame    :  1 ; /* bits 7:7 */
    cs_uint16 GFP_Dis_Payload_Hdr_Proc :  1 ; /* bits 8:8 */
    cs_uint16 GFP_Pass_Core_Hdr    :  1 ; /* bits 9:9 */
    cs_uint16 GFP_Dis_Descrambler  :  1 ; /* bits 10:10 */
    cs_uint16 GFP_Dis_Core_Header_descr :  1 ; /* bits 11:11 */
    cs_uint16 GFP_Dis_Header_XOR   :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd1               :  3 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_RX_GFP_Control_1_t;
#define     ARN5_GFP_Drop_Long_pkts_GFP_Rx_pass_pH_mode   0xa3
#define     ARN5_GFP_Drop_Long_pkts_GFP_Rx_pass_pH_F_mode 0xb3
#define     ARN5_GFP_Drop_Long_pkts_GFP_Rx_pass_pH_X_mode 0x1fb
#define     ARN5_GFP_Drop_Long_pkts_GFP_Rx_transparent_mode 0xffb

#define ARN5_FR_GFP_RX_GFP_Control_2             0x18852
#define ARN5_FR_GFP_RX_GFP_Control_2_dft         0x0100
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 GFP_Dis_Exp_UPI      :  1 ; /* bits 8:8 */
    cs_uint16 GFP_Exp_UPI          :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 GFP_Exp_UPI          :  8 ; /* bits 7:0 */
    cs_uint16 GFP_Dis_Exp_UPI      :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_RX_GFP_Control_2_t;

#define ARN5_FR_GFP_RX_GFP_Intr                  0x18854
#define ARN5_FR_GFP_RX_GFP_Intr_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  4 ;
    cs_uint16 Pld_Hdr_Err_MSB_I    :  1 ; /* bits 11:11 */
    cs_uint16 Pld_Hdr_Err_I        :  1 ; /* bits 10:10 */
    cs_uint16 Long_Pkt_I           :  1 ; /* bits 9:9 */
    cs_uint16 Short_Pkt_I          :  1 ; /* bits 8:8 */
    cs_uint16 Onebit_HEC_Err_I     :  1 ; /* bits 7:7 */
    cs_uint16 Mbit_HEC_Err_I       :  1 ; /* bits 6:6 */
    cs_uint16 OverFlow_I           :  1 ; /* bits 5:5 */
    cs_uint16 Dropped_Pkt_I        :  1 ; /* bits 4:4 */
    cs_uint16 Sync_Err_I           :  1 ; /* bits 3:3 */
    cs_uint16 Loss_Client_I        :  1 ; /* bits 2:2 */
    cs_uint16 Loss_Sync_I          :  1 ; /* bits 1:1 */
    cs_uint16 CRC_Err_I            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 CRC_Err_I            :  1 ; /* bits 0:0 */
    cs_uint16 Loss_Sync_I          :  1 ; /* bits 1:1 */
    cs_uint16 Loss_Client_I        :  1 ; /* bits 2:2 */
    cs_uint16 Sync_Err_I           :  1 ; /* bits 3:3 */
    cs_uint16 Dropped_Pkt_I        :  1 ; /* bits 4:4 */
    cs_uint16 OverFlow_I           :  1 ; /* bits 5:5 */
    cs_uint16 Mbit_HEC_Err_I       :  1 ; /* bits 6:6 */
    cs_uint16 Onebit_HEC_Err_I     :  1 ; /* bits 7:7 */
    cs_uint16 Short_Pkt_I          :  1 ; /* bits 8:8 */
    cs_uint16 Long_Pkt_I           :  1 ; /* bits 9:9 */
    cs_uint16 Pld_Hdr_Err_I        :  1 ; /* bits 10:10 */
    cs_uint16 Pld_Hdr_Err_MSB_I    :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd1               :  4 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_RX_GFP_Intr_t;

#define ARN5_FR_GFP_RX_GFP_Mask                  0x18855
#define ARN5_FR_GFP_RX_GFP_Mask_dft              0x0fff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  4 ;
    cs_uint16 Pld_Hdr_Err_MSB_M    :  1 ; /* bits 11:11 */
    cs_uint16 Pld_Hdr_Err_M        :  1 ; /* bits 10:10 */
    cs_uint16 Long_Pkt_M           :  1 ; /* bits 9:9 */
    cs_uint16 Short_Pkt_M          :  1 ; /* bits 8:8 */
    cs_uint16 Onebit_HEC_Err_M     :  1 ; /* bits 7:7 */
    cs_uint16 Mbit_HEC_Err_M       :  1 ; /* bits 6:6 */
    cs_uint16 OverFlow_M           :  1 ; /* bits 5:5 */
    cs_uint16 Dropped_Pkt_M        :  1 ; /* bits 4:4 */
    cs_uint16 Sync_Err_M           :  1 ; /* bits 3:3 */
    cs_uint16 Loss_Client_M        :  1 ; /* bits 2:2 */
    cs_uint16 Loss_Sync_M          :  1 ; /* bits 1:1 */
    cs_uint16 CRC_Err_M            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 CRC_Err_M            :  1 ; /* bits 0:0 */
    cs_uint16 Loss_Sync_M          :  1 ; /* bits 1:1 */
    cs_uint16 Loss_Client_M        :  1 ; /* bits 2:2 */
    cs_uint16 Sync_Err_M           :  1 ; /* bits 3:3 */
    cs_uint16 Dropped_Pkt_M        :  1 ; /* bits 4:4 */
    cs_uint16 OverFlow_M           :  1 ; /* bits 5:5 */
    cs_uint16 Mbit_HEC_Err_M       :  1 ; /* bits 6:6 */
    cs_uint16 Onebit_HEC_Err_M     :  1 ; /* bits 7:7 */
    cs_uint16 Short_Pkt_M          :  1 ; /* bits 8:8 */
    cs_uint16 Long_Pkt_M           :  1 ; /* bits 9:9 */
    cs_uint16 Pld_Hdr_Err_M        :  1 ; /* bits 10:10 */
    cs_uint16 Pld_Hdr_Err_MSB_M    :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd1               :  4 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_RX_GFP_Mask_t;

#define ARN5_FR_GFP_RX_GFP_Status                0x18856
#define ARN5_FR_GFP_RX_GFP_Status_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 12 ;
    cs_uint16 Sync_Err_S           :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               :  3 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 Sync_Err_S           :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd2               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_RX_GFP_Status_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * Framer::streamRx
 */

#define ARN5_FR_STREAMRX_CellControl             0x18800
#define ARN5_FR_STREAMRX_CellControl_dft         0x4009
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Reset                :  1 ; /* bits 15:15 */
    cs_uint16 cosAdd               :  1 ; /* bits 14:14 */
    cs_uint16 LCDtimer             :  2 ; /* bits 13:12 */
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 DropCells            :  1 ; /* bits 4:4 */
    cs_uint16 DropIdle             :  1 ; /* bits 3:3 */
    cs_uint16 DisDScr              :  1 ; /* bits 2:2 */
    cs_uint16 PassHEC              :  1 ; /* bits 1:1 */
    cs_uint16 CorrHec              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 CorrHec              :  1 ; /* bits 0:0 */
    cs_uint16 PassHEC              :  1 ; /* bits 1:1 */
    cs_uint16 DisDScr              :  1 ; /* bits 2:2 */
    cs_uint16 DropIdle             :  1 ; /* bits 3:3 */
    cs_uint16 DropCells            :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 LCDtimer             :  2 ; /* bits 13:12 */
    cs_uint16 cosAdd               :  1 ; /* bits 14:14 */
    cs_uint16 Reset                :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_CellControl_t;
#define     ARN5_LCDtimer_LCD_4                           0x00
#define     ARN5_LCDtimer_LCD_2                           0x01
#define     ARN5_LCDtimer_LCD_1                           0x02
#define     ARN5_LCDtimer_LCD_HALF                        0x03

#define ARN5_FR_STREAMRX_CellPattern             0x18801
#define ARN5_FR_STREAMRX_CellPattern_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 GFC                  :  4 ; /* bits 15:12 */
    cs_uint16 PTI                  :  3 ; /* bits 11:9 */
    cs_uint16 CLP                  :  1 ; /* bits 8:8 */
    cs_uint16 GFCmask              :  4 ; /* bits 7:4 */
    cs_uint16 PTImask              :  3 ; /* bits 3:1 */
    cs_uint16 CLPmask              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 CLPmask              :  1 ; /* bits 0:0 */
    cs_uint16 PTImask              :  3 ; /* bits 3:1 */
    cs_uint16 GFCmask              :  4 ; /* bits 7:4 */
    cs_uint16 CLP                  :  1 ; /* bits 8:8 */
    cs_uint16 PTI                  :  3 ; /* bits 11:9 */
    cs_uint16 GFC                  :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_CellPattern_t;

#define ARN5_FR_STREAMRX_CellIntr                0x18802
#define ARN5_FR_STREAMRX_CellIntr_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 CellCntI             :  1 ; /* bits 15:15 */
    cs_uint16 IdleCntI             :  1 ; /* bits 14:14 */
    cs_uint16 HECDropI             :  1 ; /* bits 13:13 */
    cs_uint16 HECCorrI             :  1 ; /* bits 12:12 */
    cs_uint16 OVFCntI              :  1 ; /* bits 11:11 */
    cs_uint16 SynExI               :  1 ; /* bits 10:10 */
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 X86AdaptByteI        :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 OvrFloI              :  1 ; /* bits 3:3 */
    cs_uint16 HECErrI              :  1 ; /* bits 2:2 */
    cs_uint16 LCDI                 :  1 ; /* bits 1:1 */
    cs_uint16 OCDI                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 OCDI                 :  1 ; /* bits 0:0 */
    cs_uint16 LCDI                 :  1 ; /* bits 1:1 */
    cs_uint16 HECErrI              :  1 ; /* bits 2:2 */
    cs_uint16 OvrFloI              :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 X86AdaptByteI        :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 SynExI               :  1 ; /* bits 10:10 */
    cs_uint16 OVFCntI              :  1 ; /* bits 11:11 */
    cs_uint16 HECCorrI             :  1 ; /* bits 12:12 */
    cs_uint16 HECDropI             :  1 ; /* bits 13:13 */
    cs_uint16 IdleCntI             :  1 ; /* bits 14:14 */
    cs_uint16 CellCntI             :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_CellIntr_t;

#define ARN5_FR_STREAMRX_CellMask                0x18803
#define ARN5_FR_STREAMRX_CellMask_dft            0xfc8f
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 cellCntM             :  1 ; /* bits 15:15 */
    cs_uint16 IdleCntM             :  1 ; /* bits 14:14 */
    cs_uint16 HECDropM             :  1 ; /* bits 13:13 */
    cs_uint16 HECCorrM             :  1 ; /* bits 12:12 */
    cs_uint16 OVFCntM              :  1 ; /* bits 11:11 */
    cs_uint16 SynExM               :  1 ; /* bits 10:10 */
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 X86AdaptByteM        :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 ovrFloM              :  1 ; /* bits 3:3 */
    cs_uint16 HECErrM              :  1 ; /* bits 2:2 */
    cs_uint16 LCDM                 :  1 ; /* bits 1:1 */
    cs_uint16 OCDM                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 OCDM                 :  1 ; /* bits 0:0 */
    cs_uint16 LCDM                 :  1 ; /* bits 1:1 */
    cs_uint16 HECErrM              :  1 ; /* bits 2:2 */
    cs_uint16 ovrFloM              :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 X86AdaptByteM        :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 SynExM               :  1 ; /* bits 10:10 */
    cs_uint16 OVFCntM              :  1 ; /* bits 11:11 */
    cs_uint16 HECCorrM             :  1 ; /* bits 12:12 */
    cs_uint16 HECDropM             :  1 ; /* bits 13:13 */
    cs_uint16 IdleCntM             :  1 ; /* bits 14:14 */
    cs_uint16 cellCntM             :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_CellMask_t;

#define ARN5_FR_STREAMRX_CellSts                 0x18804
#define ARN5_FR_STREAMRX_CellSts_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 CellCntS             :  1 ; /* bits 15:15 */
    cs_uint16 IdleCntS             :  1 ; /* bits 14:14 */
    cs_uint16 HECDropS             :  1 ; /* bits 13:13 */
    cs_uint16 HECCorrS             :  1 ; /* bits 12:12 */
    cs_uint16 OVFCntS              :  1 ; /* bits 11:11 */
    cs_uint16 SynExS               :  1 ; /* bits 10:10 */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 OvrFloS              :  1 ; /* bits 3:3 */
    cs_uint16 HECErrS              :  1 ; /* bits 2:2 */
    cs_uint16 LCDS                 :  1 ; /* bits 1:1 */
    cs_uint16 OCDS                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 OCDS                 :  1 ; /* bits 0:0 */
    cs_uint16 LCDS                 :  1 ; /* bits 1:1 */
    cs_uint16 HECErrS              :  1 ; /* bits 2:2 */
    cs_uint16 OvrFloS              :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 SynExS               :  1 ; /* bits 10:10 */
    cs_uint16 OVFCntS              :  1 ; /* bits 11:11 */
    cs_uint16 HECCorrS             :  1 ; /* bits 12:12 */
    cs_uint16 HECDropS             :  1 ; /* bits 13:13 */
    cs_uint16 IdleCntS             :  1 ; /* bits 14:14 */
    cs_uint16 CellCntS             :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_CellSts_t;

#define ARN5_FR_STREAMRX_Cellfsm                 0x18805
#define ARN5_FR_STREAMRX_Cellfsm_dft             0x003e
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 GAMMA                :  2 ; /* bits 7:6 */
    cs_uint16 ALPHA                :  3 ; /* bits 5:3 */
    cs_uint16 DELTA                :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 DELTA                :  3 ; /* bits 2:0 */
    cs_uint16 ALPHA                :  3 ; /* bits 5:3 */
    cs_uint16 GAMMA                :  2 ; /* bits 7:6 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cellfsm_t;
#define     ARN5_GAMMA_GAMMA_0                            0x00
#define     ARN5_GAMMA_GAMMA_2                            0x01
#define     ARN5_GAMMA_GAMMA_4                            0x02
#define     ARN5_GAMMA_GAMMA_8                            0x03

#define ARN5_FR_STREAMRX_PktControl              0x18816
#define ARN5_FR_STREAMRX_PktControl_dft          0x3010
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 reset                :  1 ; /* bits 15:15 */
    cs_uint16 dropIdle             :  1 ; /* bits 14:14 */
    cs_uint16 ChkAddr              :  1 ; /* bits 13:13 */
    cs_uint16 ChkCtrl              :  1 ; /* bits 12:12 */
    cs_uint16 DropSAPI             :  1 ; /* bits 11:11 */
    cs_uint16 enDscrBeforeFIFO     :  1 ; /* bits 10:10 */
    cs_uint16 dropBit              :  1 ; /* bits 9:9 */
    cs_uint16 nullFCS              :  1 ; /* bits 8:8 */
    cs_uint16 dropShort            :  1 ; /* bits 7:7 */
    cs_uint16 dropLong             :  1 ; /* bits 6:6 */
    cs_uint16 passFCS              :  1 ; /* bits 5:5 */
    cs_uint16 modePOS              :  1 ; /* bits 4:4 */
    cs_uint16 DropAddrCtrl         :  1 ; /* bits 3:3 */
    cs_uint16 disDscr              :  1 ; /* bits 2:2 */
    cs_uint16 crc32                :  1 ; /* bits 1:1 */
    cs_uint16 FCSDir               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FCSDir               :  1 ; /* bits 0:0 */
    cs_uint16 crc32                :  1 ; /* bits 1:1 */
    cs_uint16 disDscr              :  1 ; /* bits 2:2 */
    cs_uint16 DropAddrCtrl         :  1 ; /* bits 3:3 */
    cs_uint16 modePOS              :  1 ; /* bits 4:4 */
    cs_uint16 passFCS              :  1 ; /* bits 5:5 */
    cs_uint16 dropLong             :  1 ; /* bits 6:6 */
    cs_uint16 dropShort            :  1 ; /* bits 7:7 */
    cs_uint16 nullFCS              :  1 ; /* bits 8:8 */
    cs_uint16 dropBit              :  1 ; /* bits 9:9 */
    cs_uint16 enDscrBeforeFIFO     :  1 ; /* bits 10:10 */
    cs_uint16 DropSAPI             :  1 ; /* bits 11:11 */
    cs_uint16 ChkCtrl              :  1 ; /* bits 12:12 */
    cs_uint16 ChkAddr              :  1 ; /* bits 13:13 */
    cs_uint16 dropIdle             :  1 ; /* bits 14:14 */
    cs_uint16 reset                :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_PktControl_t;

#define ARN5_FR_STREAMRX_MaxLengthMSB            0x18817
#define ARN5_FR_STREAMRX_MaxLengthMSB_dft        0xffff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 maxLength            : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 maxLength            : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_MaxLengthMSB_t;

#define ARN5_FR_STREAMRX_MaxLengthLSB            0x18818
#define ARN5_FR_STREAMRX_MaxLengthLSB_dft        0xffff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 maxLength            : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 maxLength            : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_MaxLengthLSB_t;

#define ARN5_FR_STREAMRX_MinLength               0x18819
#define ARN5_FR_STREAMRX_MinLength_dft           0x0010
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Rx_Alt_Scramble      :  1 ; /* bits 15:15 */
    cs_uint16 Rx_Unused            :  7 ; /* bits 14:8 */
    cs_uint16 minLength            :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 minLength            :  8 ; /* bits 7:0 */
    cs_uint16 Rx_Unused            :  7 ; /* bits 14:8 */
    cs_uint16 Rx_Alt_Scramble      :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_MinLength_t;

#define ARN5_FR_STREAMRX_PktFillControl          0x1881a
#define ARN5_FR_STREAMRX_PktFillControl_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 idle                 :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 idle                 :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_PktFillControl_t;

#define ARN5_FR_STREAMRX_PktIntr                 0x1881b
#define ARN5_FR_STREAMRX_PktIntr_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 shrtPkt              :  1 ; /* bits 15:15 */
    cs_uint16 lngPkt               :  1 ; /* bits 14:14 */
    cs_uint16 Ovrflow              :  1 ; /* bits 13:13 */
    cs_uint16 FCSErr               :  1 ; /* bits 12:12 */
    cs_uint16 formatErr            :  1 ; /* bits 11:11 */
    cs_uint16 abort                :  1 ; /* bits 10:10 */
    cs_uint16 InvCnt               :  1 ; /* bits 9:9 */
    cs_uint16 DropCnt              :  1 ; /* bits 8:8 */
    cs_uint16 FCSECnt              :  1 ; /* bits 7:7 */
    cs_uint16 pktCnt               :  1 ; /* bits 6:6 */
    cs_uint16 byteCnt              :  1 ; /* bits 5:5 */
    cs_uint16 shrtPktCnt           :  1 ; /* bits 4:4 */
    cs_uint16 longPktCnt           :  1 ; /* bits 3:3 */
    cs_uint16 OvrPktCnt            :  1 ; /* bits 2:2 */
    cs_uint16 abortPktCnt          :  1 ; /* bits 1:1 */
    cs_uint16 GdPktCnt             :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 GdPktCnt             :  1 ; /* bits 0:0 */
    cs_uint16 abortPktCnt          :  1 ; /* bits 1:1 */
    cs_uint16 OvrPktCnt            :  1 ; /* bits 2:2 */
    cs_uint16 longPktCnt           :  1 ; /* bits 3:3 */
    cs_uint16 shrtPktCnt           :  1 ; /* bits 4:4 */
    cs_uint16 byteCnt              :  1 ; /* bits 5:5 */
    cs_uint16 pktCnt               :  1 ; /* bits 6:6 */
    cs_uint16 FCSECnt              :  1 ; /* bits 7:7 */
    cs_uint16 DropCnt              :  1 ; /* bits 8:8 */
    cs_uint16 InvCnt               :  1 ; /* bits 9:9 */
    cs_uint16 abort                :  1 ; /* bits 10:10 */
    cs_uint16 formatErr            :  1 ; /* bits 11:11 */
    cs_uint16 FCSErr               :  1 ; /* bits 12:12 */
    cs_uint16 Ovrflow              :  1 ; /* bits 13:13 */
    cs_uint16 lngPkt               :  1 ; /* bits 14:14 */
    cs_uint16 shrtPkt              :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_PktIntr_t;

#define ARN5_FR_STREAMRX_PktStatus               0x1881c
#define ARN5_FR_STREAMRX_PktStatus_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 shrtPktS             :  1 ; /* bits 15:15 */
    cs_uint16 lngPktS              :  1 ; /* bits 14:14 */
    cs_uint16 OvrflowS             :  1 ; /* bits 13:13 */
    cs_uint16 FCSErrS              :  1 ; /* bits 12:12 */
    cs_uint16 formatErrS           :  1 ; /* bits 11:11 */
    cs_uint16 abortS               :  1 ; /* bits 10:10 */
    cs_uint16 InvCntS              :  1 ; /* bits 9:9 */
    cs_uint16 DropCntS             :  1 ; /* bits 8:8 */
    cs_uint16 FCSECntS             :  1 ; /* bits 7:7 */
    cs_uint16 pktCntS              :  1 ; /* bits 6:6 */
    cs_uint16 byteCntS             :  1 ; /* bits 5:5 */
    cs_uint16 shrtPktCntS          :  1 ; /* bits 4:4 */
    cs_uint16 longPktCntS          :  1 ; /* bits 3:3 */
    cs_uint16 OvrPktCntS           :  1 ; /* bits 2:2 */
    cs_uint16 abortPktCntS         :  1 ; /* bits 1:1 */
    cs_uint16 GdPktCntS            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 GdPktCntS            :  1 ; /* bits 0:0 */
    cs_uint16 abortPktCntS         :  1 ; /* bits 1:1 */
    cs_uint16 OvrPktCntS           :  1 ; /* bits 2:2 */
    cs_uint16 longPktCntS          :  1 ; /* bits 3:3 */
    cs_uint16 shrtPktCntS          :  1 ; /* bits 4:4 */
    cs_uint16 byteCntS             :  1 ; /* bits 5:5 */
    cs_uint16 pktCntS              :  1 ; /* bits 6:6 */
    cs_uint16 FCSECntS             :  1 ; /* bits 7:7 */
    cs_uint16 DropCntS             :  1 ; /* bits 8:8 */
    cs_uint16 InvCntS              :  1 ; /* bits 9:9 */
    cs_uint16 abortS               :  1 ; /* bits 10:10 */
    cs_uint16 formatErrS           :  1 ; /* bits 11:11 */
    cs_uint16 FCSErrS              :  1 ; /* bits 12:12 */
    cs_uint16 OvrflowS             :  1 ; /* bits 13:13 */
    cs_uint16 lngPktS              :  1 ; /* bits 14:14 */
    cs_uint16 shrtPktS             :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_PktStatus_t;

#define ARN5_FR_STREAMRX_PktMask                 0x1881d
#define ARN5_FR_STREAMRX_PktMask_dft             0xffff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 shrtPktM             :  1 ; /* bits 15:15 */
    cs_uint16 lngPktM              :  1 ; /* bits 14:14 */
    cs_uint16 OvrflowM             :  1 ; /* bits 13:13 */
    cs_uint16 FCSErrM              :  1 ; /* bits 12:12 */
    cs_uint16 formatErrM           :  1 ; /* bits 11:11 */
    cs_uint16 abortM               :  1 ; /* bits 10:10 */
    cs_uint16 InvCntM              :  1 ; /* bits 9:9 */
    cs_uint16 DropCntM             :  1 ; /* bits 8:8 */
    cs_uint16 FCSECntM             :  1 ; /* bits 7:7 */
    cs_uint16 pktCntM              :  1 ; /* bits 6:6 */
    cs_uint16 byteCntM             :  1 ; /* bits 5:5 */
    cs_uint16 shrtPktCntM          :  1 ; /* bits 4:4 */
    cs_uint16 longPktCntM          :  1 ; /* bits 3:3 */
    cs_uint16 OvrPktCntM           :  1 ; /* bits 2:2 */
    cs_uint16 abortPktCntM         :  1 ; /* bits 1:1 */
    cs_uint16 GdPktCntM            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 GdPktCntM            :  1 ; /* bits 0:0 */
    cs_uint16 abortPktCntM         :  1 ; /* bits 1:1 */
    cs_uint16 OvrPktCntM           :  1 ; /* bits 2:2 */
    cs_uint16 longPktCntM          :  1 ; /* bits 3:3 */
    cs_uint16 shrtPktCntM          :  1 ; /* bits 4:4 */
    cs_uint16 byteCntM             :  1 ; /* bits 5:5 */
    cs_uint16 pktCntM              :  1 ; /* bits 6:6 */
    cs_uint16 FCSECntM             :  1 ; /* bits 7:7 */
    cs_uint16 DropCntM             :  1 ; /* bits 8:8 */
    cs_uint16 InvCntM              :  1 ; /* bits 9:9 */
    cs_uint16 abortM               :  1 ; /* bits 10:10 */
    cs_uint16 formatErrM           :  1 ; /* bits 11:11 */
    cs_uint16 FCSErrM              :  1 ; /* bits 12:12 */
    cs_uint16 OvrflowM             :  1 ; /* bits 13:13 */
    cs_uint16 lngPktM              :  1 ; /* bits 14:14 */
    cs_uint16 shrtPktM             :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_PktMask_t;

#define ARN5_FR_STREAMRX_PktExAddrCtrl           0x1881e
#define ARN5_FR_STREAMRX_PktExAddrCtrl_dft       0xff03
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 ExAddr               :  8 ; /* bits 15:8 */
    cs_uint16 ExCtrl               :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ExCtrl               :  8 ; /* bits 7:0 */
    cs_uint16 ExAddr               :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_PktExAddrCtrl_t;

#define ARN5_FR_STREAMRX_PktX86Ctrl              0x1881f
#define ARN5_FR_STREAMRX_PktX86Ctrl_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 X86DisDscr           :  1 ; /* bits 2:2 */
    cs_uint16 X86Sel43             :  1 ; /* bits 1:1 */
    cs_uint16 X86En                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 X86En                :  1 ; /* bits 0:0 */
    cs_uint16 X86Sel43             :  1 ; /* bits 1:1 */
    cs_uint16 X86DisDscr           :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_PktX86Ctrl_t;

#define ARN5_FR_STREAMRX_Cnt1Q1                  0x18820
#define ARN5_FR_STREAMRX_Cnt1Q1_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt1Q1_t;

#define ARN5_FR_STREAMRX_Cnt1Q2                  0x18821
#define ARN5_FR_STREAMRX_Cnt1Q2_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt1Q2_t;

#define ARN5_FR_STREAMRX_Cnt1Q3                  0x18822
#define ARN5_FR_STREAMRX_Cnt1Q3_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt1Q3_t;

#define ARN5_FR_STREAMRX_Cnt2Q1                  0x18824
#define ARN5_FR_STREAMRX_Cnt2Q1_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt2Q1_t;

#define ARN5_FR_STREAMRX_Cnt2Q2                  0x18825
#define ARN5_FR_STREAMRX_Cnt2Q2_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt2Q2_t;

#define ARN5_FR_STREAMRX_Cnt2Q3                  0x18826
#define ARN5_FR_STREAMRX_Cnt2Q3_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt2Q3_t;

#define ARN5_FR_STREAMRX_Cnt3Q1                  0x18828
#define ARN5_FR_STREAMRX_Cnt3Q1_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt3Q1_t;

#define ARN5_FR_STREAMRX_Cnt3Q2                  0x18829
#define ARN5_FR_STREAMRX_Cnt3Q2_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt3Q2_t;

#define ARN5_FR_STREAMRX_Cnt3Q3                  0x1882a
#define ARN5_FR_STREAMRX_Cnt3Q3_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt3Q3_t;

#define ARN5_FR_STREAMRX_Cnt4Q1                  0x1882c
#define ARN5_FR_STREAMRX_Cnt4Q1_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt4Q1_t;

#define ARN5_FR_STREAMRX_Cnt4Q2                  0x1882d
#define ARN5_FR_STREAMRX_Cnt4Q2_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt4Q2_t;

#define ARN5_FR_STREAMRX_Cnt4Q3                  0x1882e
#define ARN5_FR_STREAMRX_Cnt4Q3_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt4Q3_t;

#define ARN5_FR_STREAMRX_Cnt5Q1                  0x18830
#define ARN5_FR_STREAMRX_Cnt5Q1_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt5Q1_t;

#define ARN5_FR_STREAMRX_Cnt5Q2                  0x18831
#define ARN5_FR_STREAMRX_Cnt5Q2_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt5Q2_t;

#define ARN5_FR_STREAMRX_Cnt5Q3                  0x18832
#define ARN5_FR_STREAMRX_Cnt5Q3_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt5Q3_t;

#define ARN5_FR_STREAMRX_Cnt6Q1                  0x18834
#define ARN5_FR_STREAMRX_Cnt6Q1_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt6Q1_t;

#define ARN5_FR_STREAMRX_Cnt6Q2                  0x18835
#define ARN5_FR_STREAMRX_Cnt6Q2_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt6Q2_t;

#define ARN5_FR_STREAMRX_Cnt6Q3                  0x18836
#define ARN5_FR_STREAMRX_Cnt6Q3_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt6Q3_t;

#define ARN5_FR_STREAMRX_Cnt7Q1                  0x18838
#define ARN5_FR_STREAMRX_Cnt7Q1_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt7Q1_t;

#define ARN5_FR_STREAMRX_Cnt7Q2                  0x18839
#define ARN5_FR_STREAMRX_Cnt7Q2_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt7Q2_t;

#define ARN5_FR_STREAMRX_Cnt7Q3                  0x1883a
#define ARN5_FR_STREAMRX_Cnt7Q3_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt7Q3_t;

#define ARN5_FR_STREAMRX_Cnt8Q1                  0x1883c
#define ARN5_FR_STREAMRX_Cnt8Q1_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt8Q1_t;

#define ARN5_FR_STREAMRX_Cnt8Q2                  0x1883d
#define ARN5_FR_STREAMRX_Cnt8Q2_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt8Q2_t;

#define ARN5_FR_STREAMRX_Cnt8Q3                  0x1883e
#define ARN5_FR_STREAMRX_Cnt8Q3_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt8Q3_t;

#define ARN5_FR_STREAMRX_Cnt9Q1                  0x18840
#define ARN5_FR_STREAMRX_Cnt9Q1_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt9Q1_t;

#define ARN5_FR_STREAMRX_Cnt9Q2                  0x18841
#define ARN5_FR_STREAMRX_Cnt9Q2_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt9Q2_t;

#define ARN5_FR_STREAMRX_Cnt9Q3                  0x18842
#define ARN5_FR_STREAMRX_Cnt9Q3_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt9Q3_t;

#define ARN5_FR_STREAMRX_Cnt10Q1                 0x18844
#define ARN5_FR_STREAMRX_Cnt10Q1_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt10Q1_t;

#define ARN5_FR_STREAMRX_Cnt10Q2                 0x18845
#define ARN5_FR_STREAMRX_Cnt10Q2_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt10Q2_t;

#define ARN5_FR_STREAMRX_Cnt10Q3                 0x18846
#define ARN5_FR_STREAMRX_Cnt10Q3_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt10Q3_t;

#define ARN5_FR_STREAMRX_debugSelect             0x18848
#define ARN5_FR_STREAMRX_debugSelect_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 take_GFP             :  1 ; /* bits 9:9 */
    cs_uint16 takeCell             :  1 ; /* bits 8:8 */
    cs_uint16 testAddr             :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 testAddr             :  8 ; /* bits 7:0 */
    cs_uint16 takeCell             :  1 ; /* bits 8:8 */
    cs_uint16 take_GFP             :  1 ; /* bits 9:9 */
    cs_uint16 rsrvd1               :  6 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_debugSelect_t;

#define ARN5_FR_STREAMRX_debugPktControl         0x1884c
#define ARN5_FR_STREAMRX_debugPktControl_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 toggleSAPI           :  1 ; /* bits 1:1 */
    cs_uint16 toggleAddrCtrl       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 toggleAddrCtrl       :  1 ; /* bits 0:0 */
    cs_uint16 toggleSAPI           :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_debugPktControl_t;

#define ARN5_FR_STREAMRX_debugPktStatus          0x1884d
#define ARN5_FR_STREAMRX_debugPktStatus_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 statSAPI             :  1 ; /* bits 1:1 */
    cs_uint16 statAddrCtrl         :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 statAddrCtrl         :  1 ; /* bits 0:0 */
    cs_uint16 statSAPI             :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               : 14 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_debugPktStatus_t;

#define ARN5_FR_STREAMRX_debugPktAddrCtrl        0x1884e
#define ARN5_FR_STREAMRX_debugPktAddrCtrl_dft    0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 capAddrCtrl          : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 capAddrCtrl          : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_debugPktAddrCtrl_t;

#define ARN5_FR_STREAMRX_debugPktSAPI            0x1884f
#define ARN5_FR_STREAMRX_debugPktSAPI_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 capSAPI              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 capSAPI              : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_debugPktSAPI_t;

#define ARN5_FR_STREAMRX_Cnt11Q1                 0x18857
#define ARN5_FR_STREAMRX_Cnt11Q1_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt11Q1_t;

#define ARN5_FR_STREAMRX_Cnt11Q2                 0x18858
#define ARN5_FR_STREAMRX_Cnt11Q2_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt11Q2_t;

#define ARN5_FR_STREAMRX_Cnt11Q3                 0x18859
#define ARN5_FR_STREAMRX_Cnt11Q3_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMRX_Cnt11Q3_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * Framer::datacomTx
 */

#define ARN5_FR_DATACOMTX_Control                0x1a200
#define ARN5_FR_DATACOMTX_Control_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 evenParity           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 evenParity           :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_DATACOMTX_Control_t;

#define ARN5_FR_DATACOMTX_Interrupt              0x1a201
#define ARN5_FR_DATACOMTX_Interrupt_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 11 ;
    cs_uint16 parityI              :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 streamTxS            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 streamTxS            :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 parityI              :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd2               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_DATACOMTX_Interrupt_t;

#define ARN5_FR_DATACOMTX_Mask                   0x1a202
#define ARN5_FR_DATACOMTX_Mask_dft               0x0011
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 11 ;
    cs_uint16 parityM              :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 streamTxM            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 streamTxM            :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 parityM              :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd2               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_DATACOMTX_Mask_t;

#define ARN5_FR_DATACOMTX_Active_Override        0x1a208
#define ARN5_FR_DATACOMTX_Active_Override_dft    0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Active_Act_High      :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 Active_Override      :  2 ; /* bits 1:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Active_Override      :  2 ; /* bits 1:0 */
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 Active_Act_High      :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_DATACOMTX_Active_Override_t;

#define ARN5_FR_DATACOMTX_StrConfig              0x1a240
#define ARN5_FR_DATACOMTX_StrConfig_dft          0x9100
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 dataType             :  2 ; /* bits 15:14 */
    cs_uint16 cellSkid             :  3 ; /* bits 13:11 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 packetSkid           :  6 ; /* bits 9:4 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 dropShort            :  1 ; /* bits 1:1 */
    cs_uint16 reset                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 reset                :  1 ; /* bits 0:0 */
    cs_uint16 dropShort            :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 packetSkid           :  6 ; /* bits 9:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 cellSkid             :  3 ; /* bits 13:11 */
    cs_uint16 dataType             :  2 ; /* bits 15:14 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_DATACOMTX_StrConfig_t;

#define ARN5_FR_DATACOMTX_StrInt                 0x1a241
#define ARN5_FR_DATACOMTX_StrInt_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 FIFO_Par_Err_I       :  1 ; /* bits 3:3 */
    cs_uint16 overflowI            :  1 ; /* bits 2:2 */
    cs_uint16 badFrameI            :  1 ; /* bits 1:1 */
    cs_uint16 shortCellI           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 shortCellI           :  1 ; /* bits 0:0 */
    cs_uint16 badFrameI            :  1 ; /* bits 1:1 */
    cs_uint16 overflowI            :  1 ; /* bits 2:2 */
    cs_uint16 FIFO_Par_Err_I       :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_DATACOMTX_StrInt_t;

#define ARN5_FR_DATACOMTX_StrMask                0x1a242
#define ARN5_FR_DATACOMTX_StrMask_dft            0x000f
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 FIFO_Par_Err_M       :  1 ; /* bits 3:3 */
    cs_uint16 overflowM            :  1 ; /* bits 2:2 */
    cs_uint16 badFrameM            :  1 ; /* bits 1:1 */
    cs_uint16 shortCellM           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 shortCellM           :  1 ; /* bits 0:0 */
    cs_uint16 badFrameM            :  1 ; /* bits 1:1 */
    cs_uint16 overflowM            :  1 ; /* bits 2:2 */
    cs_uint16 FIFO_Par_Err_M       :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_DATACOMTX_StrMask_t;

#define ARN5_FR_DATACOMTX_StrStatus              0x1a243
#define ARN5_FR_DATACOMTX_StrStatus_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 overflowS            :  1 ; /* bits 2:2 */
    cs_uint16 badFrameS            :  1 ; /* bits 1:1 */
    cs_uint16 shortCellS           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 shortCellS           :  1 ; /* bits 0:0 */
    cs_uint16 badFrameS            :  1 ; /* bits 1:1 */
    cs_uint16 overflowS            :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               : 13 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_DATACOMTX_StrStatus_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * Framer::gfp_tx
 */

#define ARN5_FR_GFP_TX_GFP_Control_0             0x1a808
#define ARN5_FR_GFP_TX_GFP_Control_0_dft         0x4064
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Reset                :  1 ; /* bits 15:15 */
    cs_uint16 EnableChMemErr       :  1 ; /* bits 14:14 */
    cs_uint16 rsrvd3               :  4 ;
    cs_uint16 Enable_Cut_Through   :  1 ; /* bits 9:9 */
    cs_uint16 Dis_Scrambler        :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 Gen_CRC              :  1 ; /* bits 6:6 */
    cs_uint16 Gen_THEC             :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 Gen_Pld_Header       :  1 ; /* bits 2:2 */
    cs_uint16 Drop_Short_pkts      :  1 ; /* bits 1:1 */
    cs_uint16 Drop_Long_pkts       :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Drop_Long_pkts       :  1 ; /* bits 0:0 */
    cs_uint16 Drop_Short_pkts      :  1 ; /* bits 1:1 */
    cs_uint16 Gen_Pld_Header       :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 Gen_THEC             :  1 ; /* bits 5:5 */
    cs_uint16 Gen_CRC              :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 Dis_Scrambler        :  1 ; /* bits 8:8 */
    cs_uint16 Enable_Cut_Through   :  1 ; /* bits 9:9 */
    cs_uint16 rsrvd3               :  4 ;
    cs_uint16 EnableChMemErr       :  1 ; /* bits 14:14 */
    cs_uint16 Reset                :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_TX_GFP_Control_0_t;
#define     ARN5_Drop_Long_pkts_GFP_Tx_Normal_mode        0x4064
#define     ARN5_Drop_Long_pkts_GFP_Tx_Normal_F_mode      0x4024
#define     ARN5_Drop_Long_pkts_GFP_Tx_pass_pH_mode       0x4060
#define     ARN5_Drop_Long_pkts_GFP_Tx_pass_pH_F_mode     0x4020
#define     ARN5_Drop_Long_pkts_GFP_Tx_transparent_mode   0x200

#define ARN5_FR_GFP_TX_GFP_Payload_Hdr           0x1a809
#define ARN5_FR_GFP_TX_GFP_Payload_Hdr_dft       0x1000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Payload_Hdr          : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Payload_Hdr          : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_TX_GFP_Payload_Hdr_t;

#define ARN5_FR_GFP_TX_GFP_Intr                  0x1a80a
#define ARN5_FR_GFP_TX_GFP_Intr_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  7 ;
    cs_uint16 Idle_Pkts_MSB_I      :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd3               :  2 ;
    cs_uint16 Short_Pkt_I          :  1 ; /* bits 5:5 */
    cs_uint16 Long_Pkt_I           :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 Under_Flow_I         :  1 ; /* bits 2:2 */
    cs_uint16 AOP_Pkt_I            :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 AOP_Pkt_I            :  1 ; /* bits 1:1 */
    cs_uint16 Under_Flow_I         :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 Long_Pkt_I           :  1 ; /* bits 4:4 */
    cs_uint16 Short_Pkt_I          :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd3               :  2 ;
    cs_uint16 Idle_Pkts_MSB_I      :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd4               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_TX_GFP_Intr_t;

#define ARN5_FR_GFP_TX_GFP_Mask                  0x1a80b
#define ARN5_FR_GFP_TX_GFP_Mask_dft              0x0136
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd4               :  7 ;
    cs_uint16 Idle_Pkts_MSB_M      :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd3               :  2 ;
    cs_uint16 Short_Pkt_M          :  1 ; /* bits 5:5 */
    cs_uint16 Long_Pkt_M           :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 Under_Flow_M         :  1 ; /* bits 2:2 */
    cs_uint16 AOP_Pkt_M            :  1 ; /* bits 1:1 */
    cs_uint16 rsrvd1               :  1 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 AOP_Pkt_M            :  1 ; /* bits 1:1 */
    cs_uint16 Under_Flow_M         :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 Long_Pkt_M           :  1 ; /* bits 4:4 */
    cs_uint16 Short_Pkt_M          :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd3               :  2 ;
    cs_uint16 Idle_Pkts_MSB_M      :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd4               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_TX_GFP_Mask_t;

#define ARN5_FR_GFP_TX_GFP_Status                0x1a80c
#define ARN5_FR_GFP_TX_GFP_Status_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  7 ;
    cs_uint16 Idle_Pkts_MSB_S      :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  8 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 Idle_Pkts_MSB_S      :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_TX_GFP_Status_t;

#define ARN5_FR_GFP_TX_GFP_Diagnostics           0x1a80d
#define ARN5_FR_GFP_TX_GFP_Diagnostics_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd3               :  2 ;
    cs_uint16 Ins_CH_Err           :  1 ; /* bits 13:13 */
    cs_uint16 Ins_CH_ErrOnce       :  1 ; /* bits 12:12 */
    cs_uint16 Ins_PH_Err           :  1 ; /* bits 11:11 */
    cs_uint16 Ins_PH_ErrOnce       :  1 ; /* bits 10:10 */
    cs_uint16 rsrvd2               :  5 ;
    cs_uint16 Ins_FCS_Err          :  1 ; /* bits 4:4 */
    cs_uint16 Ins_FCS_ErrOnce      :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               :  3 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 Ins_FCS_ErrOnce      :  1 ; /* bits 3:3 */
    cs_uint16 Ins_FCS_Err          :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd2               :  5 ;
    cs_uint16 Ins_PH_ErrOnce       :  1 ; /* bits 10:10 */
    cs_uint16 Ins_PH_Err           :  1 ; /* bits 11:11 */
    cs_uint16 Ins_CH_ErrOnce       :  1 ; /* bits 12:12 */
    cs_uint16 Ins_CH_Err           :  1 ; /* bits 13:13 */
    cs_uint16 rsrvd3               :  2 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_TX_GFP_Diagnostics_t;

#define ARN5_FR_GFP_TX_Cnt8Q1                    0x1a840
#define ARN5_FR_GFP_TX_Cnt8Q1_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_TX_Cnt8Q1_t;

#define ARN5_FR_GFP_TX_Cnt8Q2                    0x1a841
#define ARN5_FR_GFP_TX_Cnt8Q2_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_TX_Cnt8Q2_t;

#define ARN5_FR_GFP_TX_Cnt8Q3                    0x1a842
#define ARN5_FR_GFP_TX_Cnt8Q3_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_TX_Cnt8Q3_t;

#define ARN5_FR_GFP_TX_GFP_Tx_CoreHeaderMSB      0x1a860
#define ARN5_FR_GFP_TX_GFP_Tx_CoreHeaderMSB_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 GFP_Tx_CoreHeader    : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 GFP_Tx_CoreHeader    : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_TX_GFP_Tx_CoreHeaderMSB_t;

#define ARN5_FR_GFP_TX_GFP_Tx_CoreHeaderErrLSB   0x1a861
#define ARN5_FR_GFP_TX_GFP_Tx_CoreHeaderErrLSB_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 GFP_Tx_PldHeaderErrLSB : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 GFP_Tx_PldHeaderErrLSB : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_TX_GFP_Tx_CoreHeaderErrLSB_t;

#define ARN5_FR_GFP_TX_GFP_Tx_PayloadHeaderErrMSB 0x1a862
#define ARN5_FR_GFP_TX_GFP_Tx_PayloadHeaderErrMSB_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 GFP_Tx_PldHeader     : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 GFP_Tx_PldHeader     : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_TX_GFP_Tx_PayloadHeaderErrMSB_t;

#define ARN5_FR_GFP_TX_GFP_Tx_PldHeaderErrLSB    0x1a863
#define ARN5_FR_GFP_TX_GFP_Tx_PldHeaderErrLSB_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 GFP_Tx_PldHeaderLSB  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 GFP_Tx_PldHeaderLSB  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_GFP_TX_GFP_Tx_PldHeaderErrLSB_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * Framer::streamTx
 */

#define ARN5_FR_STREAMTX_CellControl             0x1a800
#define ARN5_FR_STREAMTX_CellControl_dft         0x4000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Reset                :  1 ; /* bits 15:15 */
    cs_uint16 CosAdd               :  1 ; /* bits 14:14 */
    cs_uint16 rsrvd1               : 10 ;
    cs_uint16 Drop                 :  1 ; /* bits 3:3 */
    cs_uint16 DisScr               :  1 ; /* bits 2:2 */
    cs_uint16 HECDis               :  1 ; /* bits 1:1 */
    cs_uint16 ErrHEC               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ErrHEC               :  1 ; /* bits 0:0 */
    cs_uint16 HECDis               :  1 ; /* bits 1:1 */
    cs_uint16 DisScr               :  1 ; /* bits 2:2 */
    cs_uint16 Drop                 :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 10 ;
    cs_uint16 CosAdd               :  1 ; /* bits 14:14 */
    cs_uint16 Reset                :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_CellControl_t;

#define ARN5_FR_STREAMTX_CellPattern             0x1a801
#define ARN5_FR_STREAMTX_CellPattern_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 GFC                  :  4 ; /* bits 15:12 */
    cs_uint16 PTI                  :  3 ; /* bits 11:9 */
    cs_uint16 CLP                  :  1 ; /* bits 8:8 */
    cs_uint16 Idle                 :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Idle                 :  8 ; /* bits 7:0 */
    cs_uint16 CLP                  :  1 ; /* bits 8:8 */
    cs_uint16 PTI                  :  3 ; /* bits 11:9 */
    cs_uint16 GFC                  :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_CellPattern_t;

#define ARN5_FR_STREAMTX_CellIntr                0x1a802
#define ARN5_FR_STREAMTX_CellIntr_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd3               :  3 ;
    cs_uint16 CellCntS             :  1 ; /* bits 12:12 */
    cs_uint16 IdleCntS             :  1 ; /* bits 11:11 */
    cs_uint16 UndFloS              :  1 ; /* bits 10:10 */
    cs_uint16 rsrvd2               :  5 ;
    cs_uint16 CellCntI             :  1 ; /* bits 4:4 */
    cs_uint16 IdleCntI             :  1 ; /* bits 3:3 */
    cs_uint16 UndFloI              :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               :  2 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 UndFloI              :  1 ; /* bits 2:2 */
    cs_uint16 IdleCntI             :  1 ; /* bits 3:3 */
    cs_uint16 CellCntI             :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd2               :  5 ;
    cs_uint16 UndFloS              :  1 ; /* bits 10:10 */
    cs_uint16 IdleCntS             :  1 ; /* bits 11:11 */
    cs_uint16 CellCntS             :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd3               :  3 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_CellIntr_t;

#define ARN5_FR_STREAMTX_CellIMask               0x1a803
#define ARN5_FR_STREAMTX_CellIMask_dft           0x001c
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               : 11 ;
    cs_uint16 CellCntM             :  1 ; /* bits 4:4 */
    cs_uint16 IdleCntM             :  1 ; /* bits 3:3 */
    cs_uint16 UndFloM              :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               :  2 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 UndFloM              :  1 ; /* bits 2:2 */
    cs_uint16 IdleCntM             :  1 ; /* bits 3:3 */
    cs_uint16 CellCntM             :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd2               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_CellIMask_t;

#define ARN5_FR_STREAMTX_PktControl              0x1a810
#define ARN5_FR_STREAMTX_PktControl_dft          0x0010
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Reset                :  1 ; /* bits 15:15 */
    cs_uint16 InvFCS               :  1 ; /* bits 14:14 */
    cs_uint16 DropFlag             :  1 ; /* bits 13:13 */
    cs_uint16 EnScrBef             :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd1               :  4 ;
    cs_uint16 InsSAPI              :  1 ; /* bits 7:7 */
    cs_uint16 DropLong             :  1 ; /* bits 6:6 */
    cs_uint16 NullFCS              :  1 ; /* bits 5:5 */
    cs_uint16 PosMode              :  1 ; /* bits 4:4 */
    cs_uint16 InsAddrCtrl          :  1 ; /* bits 3:3 */
    cs_uint16 DisScr               :  1 ; /* bits 2:2 */
    cs_uint16 crc32                :  1 ; /* bits 1:1 */
    cs_uint16 FCSDir               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FCSDir               :  1 ; /* bits 0:0 */
    cs_uint16 crc32                :  1 ; /* bits 1:1 */
    cs_uint16 DisScr               :  1 ; /* bits 2:2 */
    cs_uint16 InsAddrCtrl          :  1 ; /* bits 3:3 */
    cs_uint16 PosMode              :  1 ; /* bits 4:4 */
    cs_uint16 NullFCS              :  1 ; /* bits 5:5 */
    cs_uint16 DropLong             :  1 ; /* bits 6:6 */
    cs_uint16 InsSAPI              :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  4 ;
    cs_uint16 EnScrBef             :  1 ; /* bits 12:12 */
    cs_uint16 DropFlag             :  1 ; /* bits 13:13 */
    cs_uint16 InvFCS               :  1 ; /* bits 14:14 */
    cs_uint16 Reset                :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_PktControl_t;

#define ARN5_FR_STREAMTX_FillerByte              0x1a811
#define ARN5_FR_STREAMTX_FillerByte_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 FB                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FB                   :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_FillerByte_t;

#define ARN5_FR_STREAMTX_MaxPktMSB               0x1a812
#define ARN5_FR_STREAMTX_MaxPktMSB_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 MxLenM               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 MxLenM               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_MaxPktMSB_t;

#define ARN5_FR_STREAMTX_MaxPktLSB               0x1a813
#define ARN5_FR_STREAMTX_MaxPktLSB_dft           0xffff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 MxLenL               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 MxLenL               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_MaxPktLSB_t;

#define ARN5_FR_STREAMTX_MinPkt                  0x1a814
#define ARN5_FR_STREAMTX_MinPkt_dft              0x0009
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Tx_Alt_Scramble      :  1 ; /* bits 15:15 */
    cs_uint16 MnLenM               : 15 ; /* bits 14:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 MnLenM               : 15 ; /* bits 14:0 */
    cs_uint16 Tx_Alt_Scramble      :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_MinPkt_t;

#define ARN5_FR_STREAMTX_Threshold               0x1a815
#define ARN5_FR_STREAMTX_Threshold_dft           0x0040
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Thr                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Thr                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Threshold_t;

#define ARN5_FR_STREAMTX_NumFlags                0x1a816
#define ARN5_FR_STREAMTX_NumFlags_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 AtEnd                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 AtEnd                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_NumFlags_t;

#define ARN5_FR_STREAMTX_PktSts                  0x1a817
#define ARN5_FR_STREAMTX_PktSts_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 ByteCntS             :  1 ; /* bits 15:15 */
    cs_uint16 TxByteCntS           :  1 ; /* bits 14:14 */
    cs_uint16 PktCntS              :  1 ; /* bits 13:13 */
    cs_uint16 UndCntS              :  1 ; /* bits 12:12 */
    cs_uint16 ErrCntS              :  1 ; /* bits 11:11 */
    cs_uint16 LongCntS             :  1 ; /* bits 10:10 */
    cs_uint16 ShortCntS            :  1 ; /* bits 9:9 */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 TxShrtPktS           :  1 ; /* bits 2:2 */
    cs_uint16 TxLngPktS            :  1 ; /* bits 1:1 */
    cs_uint16 TxUndFlowS           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TxUndFlowS           :  1 ; /* bits 0:0 */
    cs_uint16 TxLngPktS            :  1 ; /* bits 1:1 */
    cs_uint16 TxShrtPktS           :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 ShortCntS            :  1 ; /* bits 9:9 */
    cs_uint16 LongCntS             :  1 ; /* bits 10:10 */
    cs_uint16 ErrCntS              :  1 ; /* bits 11:11 */
    cs_uint16 UndCntS              :  1 ; /* bits 12:12 */
    cs_uint16 PktCntS              :  1 ; /* bits 13:13 */
    cs_uint16 TxByteCntS           :  1 ; /* bits 14:14 */
    cs_uint16 ByteCntS             :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_PktSts_t;

#define ARN5_FR_STREAMTX_PktInt                  0x1a818
#define ARN5_FR_STREAMTX_PktInt_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 ByteCntI             :  1 ; /* bits 15:15 */
    cs_uint16 TxByteCntI           :  1 ; /* bits 14:14 */
    cs_uint16 PktCntI              :  1 ; /* bits 13:13 */
    cs_uint16 UndCntI              :  1 ; /* bits 12:12 */
    cs_uint16 ErrCntI              :  1 ; /* bits 11:11 */
    cs_uint16 LongCntI             :  1 ; /* bits 10:10 */
    cs_uint16 ShortCntI            :  1 ; /* bits 9:9 */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 TxShrtPktI           :  1 ; /* bits 2:2 */
    cs_uint16 TxLngPktI            :  1 ; /* bits 1:1 */
    cs_uint16 TxUndFlowI           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TxUndFlowI           :  1 ; /* bits 0:0 */
    cs_uint16 TxLngPktI            :  1 ; /* bits 1:1 */
    cs_uint16 TxShrtPktI           :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 ShortCntI            :  1 ; /* bits 9:9 */
    cs_uint16 LongCntI             :  1 ; /* bits 10:10 */
    cs_uint16 ErrCntI              :  1 ; /* bits 11:11 */
    cs_uint16 UndCntI              :  1 ; /* bits 12:12 */
    cs_uint16 PktCntI              :  1 ; /* bits 13:13 */
    cs_uint16 TxByteCntI           :  1 ; /* bits 14:14 */
    cs_uint16 ByteCntI             :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_PktInt_t;

#define ARN5_FR_STREAMTX_PktMask                 0x1a819
#define ARN5_FR_STREAMTX_PktMask_dft             0xfe07
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 ByteCntM             :  1 ; /* bits 15:15 */
    cs_uint16 TxByteCntM           :  1 ; /* bits 14:14 */
    cs_uint16 PktCntM              :  1 ; /* bits 13:13 */
    cs_uint16 UndCntM              :  1 ; /* bits 12:12 */
    cs_uint16 ErrCntM              :  1 ; /* bits 11:11 */
    cs_uint16 LongCntM             :  1 ; /* bits 10:10 */
    cs_uint16 ShortCntM            :  1 ; /* bits 9:9 */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 TxShrtPktM           :  1 ; /* bits 2:2 */
    cs_uint16 TxLngPktM            :  1 ; /* bits 1:1 */
    cs_uint16 TxUndFlowM           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 TxUndFlowM           :  1 ; /* bits 0:0 */
    cs_uint16 TxLngPktM            :  1 ; /* bits 1:1 */
    cs_uint16 TxShrtPktM           :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 ShortCntM            :  1 ; /* bits 9:9 */
    cs_uint16 LongCntM             :  1 ; /* bits 10:10 */
    cs_uint16 ErrCntM              :  1 ; /* bits 11:11 */
    cs_uint16 UndCntM              :  1 ; /* bits 12:12 */
    cs_uint16 PktCntM              :  1 ; /* bits 13:13 */
    cs_uint16 TxByteCntM           :  1 ; /* bits 14:14 */
    cs_uint16 ByteCntM             :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_PktMask_t;

#define ARN5_FR_STREAMTX_PktAddrCtrl             0x1a81a
#define ARN5_FR_STREAMTX_PktAddrCtrl_dft         0xff03
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Addr                 :  8 ; /* bits 15:8 */
    cs_uint16 Ctrl                 :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Ctrl                 :  8 ; /* bits 7:0 */
    cs_uint16 Addr                 :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_PktAddrCtrl_t;

#define ARN5_FR_STREAMTX_PktSAPI                 0x1a81b
#define ARN5_FR_STREAMTX_PktSAPI_dft             0x0021
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SAPI                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SAPI                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_PktSAPI_t;

#define ARN5_FR_STREAMTX_Cnt1Q1                  0x1a820
#define ARN5_FR_STREAMTX_Cnt1Q1_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt1Q1_t;

#define ARN5_FR_STREAMTX_Cnt1Q2                  0x1a821
#define ARN5_FR_STREAMTX_Cnt1Q2_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt1Q2_t;

#define ARN5_FR_STREAMTX_Cnt1Q3                  0x1a822
#define ARN5_FR_STREAMTX_Cnt1Q3_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt1Q3_t;

#define ARN5_FR_STREAMTX_Cnt2Q1                  0x1a824
#define ARN5_FR_STREAMTX_Cnt2Q1_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt2Q1_t;

#define ARN5_FR_STREAMTX_Cnt2Q2                  0x1a825
#define ARN5_FR_STREAMTX_Cnt2Q2_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt2Q2_t;

#define ARN5_FR_STREAMTX_Cnt2Q3                  0x1a826
#define ARN5_FR_STREAMTX_Cnt2Q3_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt2Q3_t;

#define ARN5_FR_STREAMTX_Cnt3Q1                  0x1a828
#define ARN5_FR_STREAMTX_Cnt3Q1_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt3Q1_t;

#define ARN5_FR_STREAMTX_Cnt3Q2                  0x1a829
#define ARN5_FR_STREAMTX_Cnt3Q2_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt3Q2_t;

#define ARN5_FR_STREAMTX_Cnt3Q3                  0x1a82a
#define ARN5_FR_STREAMTX_Cnt3Q3_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt3Q3_t;

#define ARN5_FR_STREAMTX_Cnt4Q1                  0x1a82c
#define ARN5_FR_STREAMTX_Cnt4Q1_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt4Q1_t;

#define ARN5_FR_STREAMTX_Cnt4Q2                  0x1a82d
#define ARN5_FR_STREAMTX_Cnt4Q2_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt4Q2_t;

#define ARN5_FR_STREAMTX_Cnt4Q3                  0x1a82e
#define ARN5_FR_STREAMTX_Cnt4Q3_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt4Q3_t;

#define ARN5_FR_STREAMTX_Cnt5Q1                  0x1a830
#define ARN5_FR_STREAMTX_Cnt5Q1_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt5Q1_t;

#define ARN5_FR_STREAMTX_Cnt5Q2                  0x1a831
#define ARN5_FR_STREAMTX_Cnt5Q2_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt5Q2_t;

#define ARN5_FR_STREAMTX_Cnt5Q3                  0x1a832
#define ARN5_FR_STREAMTX_Cnt5Q3_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt5Q3_t;

#define ARN5_FR_STREAMTX_Cnt6Q1                  0x1a834
#define ARN5_FR_STREAMTX_Cnt6Q1_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt6Q1_t;

#define ARN5_FR_STREAMTX_Cnt6Q2                  0x1a835
#define ARN5_FR_STREAMTX_Cnt6Q2_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt6Q2_t;

#define ARN5_FR_STREAMTX_Cnt6Q3                  0x1a836
#define ARN5_FR_STREAMTX_Cnt6Q3_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt6Q3_t;

#define ARN5_FR_STREAMTX_Cnt7Q1                  0x1a838
#define ARN5_FR_STREAMTX_Cnt7Q1_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt7Q1_t;

#define ARN5_FR_STREAMTX_Cnt7Q2                  0x1a839
#define ARN5_FR_STREAMTX_Cnt7Q2_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt7Q2_t;

#define ARN5_FR_STREAMTX_Cnt7Q3                  0x1a83a
#define ARN5_FR_STREAMTX_Cnt7Q3_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Val                  : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_Cnt7Q3_t;

#define ARN5_FR_STREAMTX_debugSelect             0x1a83c
#define ARN5_FR_STREAMTX_debugSelect_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 take_GFP             :  1 ; /* bits 9:9 */
    cs_uint16 takeCell             :  1 ; /* bits 8:8 */
    cs_uint16 testAddr             :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 testAddr             :  8 ; /* bits 7:0 */
    cs_uint16 takeCell             :  1 ; /* bits 8:8 */
    cs_uint16 take_GFP             :  1 ; /* bits 9:9 */
    cs_uint16 rsrvd1               :  6 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_STREAMTX_debugSelect_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * Framer::sonet32Rx
 */

#define ARN5_FR_SONET32RX_PathProvision          0x1c000
#define ARN5_FR_SONET32RX_PathProvision_dft      0x0061
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 EnhRDI               :  1 ; /* bits 15:15 */
    cs_uint16 J1Len                :  1 ; /* bits 14:14 */
    cs_uint16 J1Mode               :  1 ; /* bits 13:13 */
    cs_uint16 PBERReset            :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 QID                  :  2 ; /* bits 9:8 */
    cs_uint16 SVC                  :  3 ; /* bits 7:5 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 STSType              :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 STSType              :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 SVC                  :  3 ; /* bits 7:5 */
    cs_uint16 QID                  :  2 ; /* bits 9:8 */
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 PBERReset            :  1 ; /* bits 12:12 */
    cs_uint16 J1Mode               :  1 ; /* bits 13:13 */
    cs_uint16 J1Len                :  1 ; /* bits 14:14 */
    cs_uint16 EnhRDI               :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PathProvision_t;
#define     ARN5_QID_SEL_STRM0                            0x00
#define     ARN5_QID_SEL_STRM1                            0x01
#define     ARN5_QID_SEL_STRM2                            0x02
#define     ARN5_QID_SEL_STRM3                            0x03
#define     ARN5_SVC_SRVC_ATM                             0x02
#define     ARN5_SVC_SRVC_PPP                             0x03
#define     ARN5_SVC_SRVC_GFP                             0x04
#define     ARN5_SVC_SRVC_PRBS                            0x06
#define     ARN5_STSType_STS_3C                           0x01
#define     ARN5_STSType_STS_12C                          0x02
#define     ARN5_STSType_STS_48C                          0x04

#define ARN5_FR_SONET32RX_FilteredG1C2           0x1c001
#define ARN5_FR_SONET32RX_FilteredG1C2_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 FilteredG1           :  3 ; /* bits 15:13 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 FilteredC2           :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FilteredC2           :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 FilteredG1           :  3 ; /* bits 15:13 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_FilteredG1C2_t;

#define ARN5_FR_SONET32RX_PRDITxInsert           0x1c002
#define ARN5_FR_SONET32RX_PRDITxInsert_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 sschk                :  1 ; /* bits 15:15 */
    cs_uint16 Disconc              :  1 ; /* bits 14:14 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 PTU2RDI              :  1 ; /* bits 12:12 */
    cs_uint16 PTM2RDI              :  1 ; /* bits 11:11 */
    cs_uint16 PSLU2RDI             :  1 ; /* bits 10:10 */
    cs_uint16 PSLM2RDI             :  1 ; /* bits 9:9 */
    cs_uint16 UNEQP2RDI            :  1 ; /* bits 8:8 */
    cs_uint16 ssBits               :  2 ; /* bits 7:6 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 PTU2Drop             :  1 ; /* bits 4:4 */
    cs_uint16 PTM2Drop             :  1 ; /* bits 3:3 */
    cs_uint16 PSLU2Drop            :  1 ; /* bits 2:2 */
    cs_uint16 PSLM2Drop            :  1 ; /* bits 1:1 */
    cs_uint16 UNEQP2Drop           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 UNEQP2Drop           :  1 ; /* bits 0:0 */
    cs_uint16 PSLM2Drop            :  1 ; /* bits 1:1 */
    cs_uint16 PSLU2Drop            :  1 ; /* bits 2:2 */
    cs_uint16 PTM2Drop             :  1 ; /* bits 3:3 */
    cs_uint16 PTU2Drop             :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 ssBits               :  2 ; /* bits 7:6 */
    cs_uint16 UNEQP2RDI            :  1 ; /* bits 8:8 */
    cs_uint16 PSLM2RDI             :  1 ; /* bits 9:9 */
    cs_uint16 PSLU2RDI             :  1 ; /* bits 10:10 */
    cs_uint16 PTM2RDI              :  1 ; /* bits 11:11 */
    cs_uint16 PTU2RDI              :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 Disconc              :  1 ; /* bits 14:14 */
    cs_uint16 sschk                :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PRDITxInsert_t;

#define ARN5_FR_SONET32RX_PathLabel              0x1c003
#define ARN5_FR_SONET32RX_PathLabel_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 C2                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 C2                   :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PathLabel_t;

#define ARN5_FR_SONET32RX_APSPathTxInsert        0x1c004
#define ARN5_FR_SONET32RX_APSPathTxInsert_dft    0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 insRDIP              :  3 ; /* bits 15:13 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 dPSLU                :  1 ; /* bits 7:7 */
    cs_uint16 dPSLM                :  1 ; /* bits 6:6 */
    cs_uint16 dPTU                 :  1 ; /* bits 5:5 */
    cs_uint16 dPTM                 :  1 ; /* bits 4:4 */
    cs_uint16 dUNEQP               :  1 ; /* bits 3:3 */
    cs_uint16 dPRDI                :  1 ; /* bits 2:2 */
    cs_uint16 dPAIS                :  1 ; /* bits 1:1 */
    cs_uint16 dLOP                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 dLOP                 :  1 ; /* bits 0:0 */
    cs_uint16 dPAIS                :  1 ; /* bits 1:1 */
    cs_uint16 dPRDI                :  1 ; /* bits 2:2 */
    cs_uint16 dUNEQP               :  1 ; /* bits 3:3 */
    cs_uint16 dPTM                 :  1 ; /* bits 4:4 */
    cs_uint16 dPTU                 :  1 ; /* bits 5:5 */
    cs_uint16 dPSLM                :  1 ; /* bits 6:6 */
    cs_uint16 dPSLU                :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 insRDIP              :  3 ; /* bits 15:13 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_APSPathTxInsert_t;

#define ARN5_FR_SONET32RX_APSPathB3TxInsert      0x1c005
#define ARN5_FR_SONET32RX_APSPathB3TxInsert_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 aPAIS                :  1 ; /* bits 15:15 */
    cs_uint16 aLOP                 :  1 ; /* bits 14:14 */
    cs_uint16 aPAISConc            :  1 ; /* bits 13:13 */
    cs_uint16 aLOPConc             :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 B3ErCnt              :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 B3ErCnt              :  4 ; /* bits 3:0 */
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 aLOPConc             :  1 ; /* bits 12:12 */
    cs_uint16 aPAISConc            :  1 ; /* bits 13:13 */
    cs_uint16 aLOP                 :  1 ; /* bits 14:14 */
    cs_uint16 aPAIS                :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_APSPathB3TxInsert_t;

#define ARN5_FR_SONET32RX_PathPointer            0x1c006
#define ARN5_FR_SONET32RX_PathPointer_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 NDF                  :  4 ; /* bits 15:12 */
    cs_uint16 SS                   :  2 ; /* bits 11:10 */
    cs_uint16 Ptr                  : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Ptr                  : 10 ; /* bits 9:0 */
    cs_uint16 SS                   :  2 ; /* bits 11:10 */
    cs_uint16 NDF                  :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PathPointer_t;

#define ARN5_FR_SONET32RX_PathBIPCount           0x1c007
#define ARN5_FR_SONET32RX_PathBIPCount_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 B3BE                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 B3BE                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PathBIPCount_t;

#define ARN5_FR_SONET32RX_PathREICount           0x1c008
#define ARN5_FR_SONET32RX_PathREICount_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 REIP                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 REIP                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PathREICount_t;

#define ARN5_FR_SONET32RX_PathPositiveCount      0x1c009
#define ARN5_FR_SONET32RX_PathPositiveCount_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Justify              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Justify              : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PathPositiveCount_t;

#define ARN5_FR_SONET32RX_PathNegativeCount      0x1c00a
#define ARN5_FR_SONET32RX_PathNegativeCount_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Justify              : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Justify              : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PathNegativeCount_t;

#define ARN5_FR_SONET32RX_PathNewPtrCount        0x1c00b
#define ARN5_FR_SONET32RX_PathNewPtrCount_dft    0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 NewPtr               : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 NewPtr               : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PathNewPtrCount_t;

#define ARN5_FR_SONET32RX_PathInt                0x1c00c
#define ARN5_FR_SONET32RX_PathInt_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 NewPtrI              :  1 ; /* bits 15:15 */
    cs_uint16 pathBERI             :  1 ; /* bits 14:14 */
    cs_uint16 PTCapI               :  1 ; /* bits 13:13 */
    cs_uint16 SSI                  :  1 ; /* bits 12:12 */
    cs_uint16 PBGEI                :  1 ; /* bits 11:11 */
    cs_uint16 PBIPI                :  1 ; /* bits 10:10 */
    cs_uint16 PPtrEI               :  1 ; /* bits 9:9 */
    cs_uint16 PREII                :  1 ; /* bits 8:8 */
    cs_uint16 PSLUnsI              :  1 ; /* bits 7:7 */
    cs_uint16 PSLMisI              :  1 ; /* bits 6:6 */
    cs_uint16 PTUnsI               :  1 ; /* bits 5:5 */
    cs_uint16 PTMisI               :  1 ; /* bits 4:4 */
    cs_uint16 PUNEQPI              :  1 ; /* bits 3:3 */
    cs_uint16 PRDII                :  1 ; /* bits 2:2 */
    cs_uint16 PAISI                :  1 ; /* bits 1:1 */
    cs_uint16 LOPI                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOPI                 :  1 ; /* bits 0:0 */
    cs_uint16 PAISI                :  1 ; /* bits 1:1 */
    cs_uint16 PRDII                :  1 ; /* bits 2:2 */
    cs_uint16 PUNEQPI              :  1 ; /* bits 3:3 */
    cs_uint16 PTMisI               :  1 ; /* bits 4:4 */
    cs_uint16 PTUnsI               :  1 ; /* bits 5:5 */
    cs_uint16 PSLMisI              :  1 ; /* bits 6:6 */
    cs_uint16 PSLUnsI              :  1 ; /* bits 7:7 */
    cs_uint16 PREII                :  1 ; /* bits 8:8 */
    cs_uint16 PPtrEI               :  1 ; /* bits 9:9 */
    cs_uint16 PBIPI                :  1 ; /* bits 10:10 */
    cs_uint16 PBGEI                :  1 ; /* bits 11:11 */
    cs_uint16 SSI                  :  1 ; /* bits 12:12 */
    cs_uint16 PTCapI               :  1 ; /* bits 13:13 */
    cs_uint16 pathBERI             :  1 ; /* bits 14:14 */
    cs_uint16 NewPtrI              :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PathInt_t;

#define ARN5_FR_SONET32RX_PathMask               0x1c00d
#define ARN5_FR_SONET32RX_PathMask_dft           0xffff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 NewPtrM              :  1 ; /* bits 15:15 */
    cs_uint16 pathBERM             :  1 ; /* bits 14:14 */
    cs_uint16 PTCapM               :  1 ; /* bits 13:13 */
    cs_uint16 SSM                  :  1 ; /* bits 12:12 */
    cs_uint16 PBGEM                :  1 ; /* bits 11:11 */
    cs_uint16 PBIPM                :  1 ; /* bits 10:10 */
    cs_uint16 PPtrEM               :  1 ; /* bits 9:9 */
    cs_uint16 PREIM                :  1 ; /* bits 8:8 */
    cs_uint16 PSLUnsM              :  1 ; /* bits 7:7 */
    cs_uint16 PSLMisM              :  1 ; /* bits 6:6 */
    cs_uint16 PTUnsM               :  1 ; /* bits 5:5 */
    cs_uint16 PTMisM               :  1 ; /* bits 4:4 */
    cs_uint16 PUNEQPM              :  1 ; /* bits 3:3 */
    cs_uint16 PRDIM                :  1 ; /* bits 2:2 */
    cs_uint16 PAISM                :  1 ; /* bits 1:1 */
    cs_uint16 LOPM                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOPM                 :  1 ; /* bits 0:0 */
    cs_uint16 PAISM                :  1 ; /* bits 1:1 */
    cs_uint16 PRDIM                :  1 ; /* bits 2:2 */
    cs_uint16 PUNEQPM              :  1 ; /* bits 3:3 */
    cs_uint16 PTMisM               :  1 ; /* bits 4:4 */
    cs_uint16 PTUnsM               :  1 ; /* bits 5:5 */
    cs_uint16 PSLMisM              :  1 ; /* bits 6:6 */
    cs_uint16 PSLUnsM              :  1 ; /* bits 7:7 */
    cs_uint16 PREIM                :  1 ; /* bits 8:8 */
    cs_uint16 PPtrEM               :  1 ; /* bits 9:9 */
    cs_uint16 PBIPM                :  1 ; /* bits 10:10 */
    cs_uint16 PBGEM                :  1 ; /* bits 11:11 */
    cs_uint16 SSM                  :  1 ; /* bits 12:12 */
    cs_uint16 PTCapM               :  1 ; /* bits 13:13 */
    cs_uint16 pathBERM             :  1 ; /* bits 14:14 */
    cs_uint16 NewPtrM              :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PathMask_t;

#define ARN5_FR_SONET32RX_PathStatus             0x1c00e
#define ARN5_FR_SONET32RX_PathStatus_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 NewPtrS              :  1 ; /* bits 15:15 */
    cs_uint16 pathBERS             :  1 ; /* bits 14:14 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 SSS                  :  1 ; /* bits 12:12 */
    cs_uint16 PBGES                :  1 ; /* bits 11:11 */
    cs_uint16 PBIPS                :  1 ; /* bits 10:10 */
    cs_uint16 PPtrES               :  1 ; /* bits 9:9 */
    cs_uint16 PREIS                :  1 ; /* bits 8:8 */
    cs_uint16 PSLUnsS              :  1 ; /* bits 7:7 */
    cs_uint16 PSLMisS              :  1 ; /* bits 6:6 */
    cs_uint16 PTUnsS               :  1 ; /* bits 5:5 */
    cs_uint16 PTMisS               :  1 ; /* bits 4:4 */
    cs_uint16 PUNEQPS              :  1 ; /* bits 3:3 */
    cs_uint16 PRDIS                :  1 ; /* bits 2:2 */
    cs_uint16 PAISS                :  1 ; /* bits 1:1 */
    cs_uint16 LOPS                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOPS                 :  1 ; /* bits 0:0 */
    cs_uint16 PAISS                :  1 ; /* bits 1:1 */
    cs_uint16 PRDIS                :  1 ; /* bits 2:2 */
    cs_uint16 PUNEQPS              :  1 ; /* bits 3:3 */
    cs_uint16 PTMisS               :  1 ; /* bits 4:4 */
    cs_uint16 PTUnsS               :  1 ; /* bits 5:5 */
    cs_uint16 PSLMisS              :  1 ; /* bits 6:6 */
    cs_uint16 PSLUnsS              :  1 ; /* bits 7:7 */
    cs_uint16 PREIS                :  1 ; /* bits 8:8 */
    cs_uint16 PPtrES               :  1 ; /* bits 9:9 */
    cs_uint16 PBIPS                :  1 ; /* bits 10:10 */
    cs_uint16 PBGES                :  1 ; /* bits 11:11 */
    cs_uint16 SSS                  :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 pathBERS             :  1 ; /* bits 14:14 */
    cs_uint16 NewPtrS              :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PathStatus_t;

#define ARN5_FR_SONET32RX_Configuration          0x1c300
#define ARN5_FR_SONET32RX_Configuration_dft      0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 reset                :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 rxAlmPort            :  4 ; /* bits 12:9 */
    cs_uint16 SDH                  :  1 ; /* bits 8:8 */
    cs_uint16 ShortFrame           :  1 ; /* bits 7:7 */
    cs_uint16 Fixed_Z0             :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 EvenParity           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 EvenParity           :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 Fixed_Z0             :  1 ; /* bits 6:6 */
    cs_uint16 ShortFrame           :  1 ; /* bits 7:7 */
    cs_uint16 SDH                  :  1 ; /* bits 8:8 */
    cs_uint16 rxAlmPort            :  4 ; /* bits 12:9 */
    cs_uint16 rsrvd2               :  2 ;
    cs_uint16 reset                :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_Configuration_t;
#define     ARN5_rxAlmPort_LOS                            0x00
#define     ARN5_rxAlmPort_LOF                            0x01
#define     ARN5_rxAlmPort_AISL                           0x02
#define     ARN5_rxAlmPort_SF                             0x03
#define     ARN5_rxAlmPort_AISP_0                         0x08
#define     ARN5_rxAlmPort_LOP_0                          0x0c

#define ARN5_FR_SONET32RX_LOSThreshold           0x1c301
#define ARN5_FR_SONET32RX_LOSThreshold_dft       0x0006
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 LOSD                 :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOSD                 :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_LOSThreshold_t;
#define     ARN5_LOSD_LOSD_23us                           0x03
#define     ARN5_LOSD_LOSD_5us                            0x06
#define     ARN5_LOSD_LOSD_25us                           0x1e
#define     ARN5_LOSD_LOSD_50us                           0x3c
#define     ARN5_LOSD_LOSD_75us                           0x60
#define     ARN5_LOSD_LOSD_100us                          0xd8

#define ARN5_FR_SONET32RX_SectionLineControl     0x1c302
#define ARN5_FR_SONET32RX_SectionLineControl_dft 0x8000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 J0Len                :  1 ; /* bits 15:15 */
    cs_uint16 J0Mode               :  1 ; /* bits 14:14 */
    cs_uint16 FramerReset          :  1 ; /* bits 13:13 */
    cs_uint16 PRBSLoad             :  1 ; /* bits 12:12 */
    cs_uint16 LBERReset            :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 numA1A2              :  2 ; /* bits 9:8 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 BlkREI               :  1 ; /* bits 4:4 */
    cs_uint16 BlkB2EO              :  1 ; /* bits 3:3 */
    cs_uint16 BlkB2E               :  1 ; /* bits 2:2 */
    cs_uint16 BlkB1E               :  1 ; /* bits 1:1 */
    cs_uint16 DisDScr              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 DisDScr              :  1 ; /* bits 0:0 */
    cs_uint16 BlkB1E               :  1 ; /* bits 1:1 */
    cs_uint16 BlkB2E               :  1 ; /* bits 2:2 */
    cs_uint16 BlkB2EO              :  1 ; /* bits 3:3 */
    cs_uint16 BlkREI               :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 numA1A2              :  2 ; /* bits 9:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 LBERReset            :  1 ; /* bits 11:11 */
    cs_uint16 PRBSLoad             :  1 ; /* bits 12:12 */
    cs_uint16 FramerReset          :  1 ; /* bits 13:13 */
    cs_uint16 J0Mode               :  1 ; /* bits 14:14 */
    cs_uint16 J0Len                :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_SectionLineControl_t;
#define     ARN5_numA1A2_CMP_3A1_3A2                      0x00
#define     ARN5_numA1A2_CMP_4A1_4A2                      0x01
#define     ARN5_numA1A2_CMP_8A1_8A2                      0x02
#define     ARN5_numA1A2_CMP_12A1_12A2                    0x03

#define ARN5_FR_SONET32RX_SecLineAlarmtoTxControl 0x1c303
#define ARN5_FR_SONET32RX_SecLineAlarmtoTxControl_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 STM2AISL             :  1 ; /* bits 15:15 */
    cs_uint16 STU2AISL             :  1 ; /* bits 14:14 */
    cs_uint16 BERSF2AISL           :  1 ; /* bits 13:13 */
    cs_uint16 SD2AISL              :  1 ; /* bits 12:12 */
    cs_uint16 STM2RDIL             :  1 ; /* bits 11:11 */
    cs_uint16 STU2RDIL             :  1 ; /* bits 10:10 */
    cs_uint16 BERSF2RDIL           :  1 ; /* bits 9:9 */
    cs_uint16 SD2RDIL              :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 AutoIns              :  1 ; /* bits 4:4 */
    cs_uint16 STM2Drop             :  1 ; /* bits 3:3 */
    cs_uint16 STU2Drop             :  1 ; /* bits 2:2 */
    cs_uint16 BERSF2Drop           :  1 ; /* bits 1:1 */
    cs_uint16 SD2Drop              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SD2Drop              :  1 ; /* bits 0:0 */
    cs_uint16 BERSF2Drop           :  1 ; /* bits 1:1 */
    cs_uint16 STU2Drop             :  1 ; /* bits 2:2 */
    cs_uint16 STM2Drop             :  1 ; /* bits 3:3 */
    cs_uint16 AutoIns              :  1 ; /* bits 4:4 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 SD2RDIL              :  1 ; /* bits 8:8 */
    cs_uint16 BERSF2RDIL           :  1 ; /* bits 9:9 */
    cs_uint16 STU2RDIL             :  1 ; /* bits 10:10 */
    cs_uint16 STM2RDIL             :  1 ; /* bits 11:11 */
    cs_uint16 SD2AISL              :  1 ; /* bits 12:12 */
    cs_uint16 BERSF2AISL           :  1 ; /* bits 13:13 */
    cs_uint16 STU2AISL             :  1 ; /* bits 14:14 */
    cs_uint16 STM2AISL             :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_SecLineAlarmtoTxControl_t;

#define ARN5_FR_SONET32RX_APSTxSecLineAlarm      0x1c304
#define ARN5_FR_SONET32RX_APSTxSecLineAlarm_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 insAISL              :  1 ; /* bits 15:15 */
    cs_uint16 insRDIL              :  1 ; /* bits 14:14 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 dSTU                 :  1 ; /* bits 8:8 */
    cs_uint16 dSTM                 :  1 ; /* bits 7:7 */
    cs_uint16 dRDIL                :  1 ; /* bits 6:6 */
    cs_uint16 dSD                  :  1 ; /* bits 5:5 */
    cs_uint16 dAISL                :  1 ; /* bits 4:4 */
    cs_uint16 dSF                  :  1 ; /* bits 3:3 */
    cs_uint16 dLOF                 :  1 ; /* bits 2:2 */
    cs_uint16 dSEF                 :  1 ; /* bits 1:1 */
    cs_uint16 dLOS                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 dLOS                 :  1 ; /* bits 0:0 */
    cs_uint16 dSEF                 :  1 ; /* bits 1:1 */
    cs_uint16 dLOF                 :  1 ; /* bits 2:2 */
    cs_uint16 dSF                  :  1 ; /* bits 3:3 */
    cs_uint16 dAISL                :  1 ; /* bits 4:4 */
    cs_uint16 dSD                  :  1 ; /* bits 5:5 */
    cs_uint16 dRDIL                :  1 ; /* bits 6:6 */
    cs_uint16 dSTM                 :  1 ; /* bits 7:7 */
    cs_uint16 dSTU                 :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 insRDIL              :  1 ; /* bits 14:14 */
    cs_uint16 insAISL              :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_APSTxSecLineAlarm_t;

#define ARN5_FR_SONET32RX_APSTxLineB2Count       0x1c305
#define ARN5_FR_SONET32RX_APSTxLineB2Count_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 REILCnt              :  8 ; /* bits 15:8 */
    cs_uint16 B2ErrCnt             :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 B2ErrCnt             :  8 ; /* bits 7:0 */
    cs_uint16 REILCnt              :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_APSTxLineB2Count_t;

#define ARN5_FR_SONET32RX_SectionInterrupt       0x1c306
#define ARN5_FR_SONET32RX_SectionInterrupt_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 X_LOS_raw_I          :  1 ; /* bits 14:14 */
    cs_uint16 X_LOS_qual_I         :  1 ; /* bits 13:13 */
    cs_uint16 PRBS_Rx_Cnt_MSBi     :  1 ; /* bits 12:12 */
    cs_uint16 SerialHDLCI          :  1 ; /* bits 11:11 */
    cs_uint16 ParityErrI           :  1 ; /* bits 10:10 */
    cs_uint16 PRBSErrI             :  1 ; /* bits 9:9 */
    cs_uint16 SecTrRcvI            :  1 ; /* bits 8:8 */
    cs_uint16 B1BIPI               :  1 ; /* bits 7:7 */
    cs_uint16 B1BIPEI              :  1 ; /* bits 6:6 */
    cs_uint16 SecTrMisI            :  1 ; /* bits 5:5 */
    cs_uint16 SecTrUnsI            :  1 ; /* bits 4:4 */
    cs_uint16 SEFI                 :  1 ; /* bits 3:3 */
    cs_uint16 dZ0InconI            :  1 ; /* bits 2:2 */
    cs_uint16 LOFI                 :  1 ; /* bits 1:1 */
    cs_uint16 LOSI                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOSI                 :  1 ; /* bits 0:0 */
    cs_uint16 LOFI                 :  1 ; /* bits 1:1 */
    cs_uint16 dZ0InconI            :  1 ; /* bits 2:2 */
    cs_uint16 SEFI                 :  1 ; /* bits 3:3 */
    cs_uint16 SecTrUnsI            :  1 ; /* bits 4:4 */
    cs_uint16 SecTrMisI            :  1 ; /* bits 5:5 */
    cs_uint16 B1BIPEI              :  1 ; /* bits 6:6 */
    cs_uint16 B1BIPI               :  1 ; /* bits 7:7 */
    cs_uint16 SecTrRcvI            :  1 ; /* bits 8:8 */
    cs_uint16 PRBSErrI             :  1 ; /* bits 9:9 */
    cs_uint16 ParityErrI           :  1 ; /* bits 10:10 */
    cs_uint16 SerialHDLCI          :  1 ; /* bits 11:11 */
    cs_uint16 PRBS_Rx_Cnt_MSBi     :  1 ; /* bits 12:12 */
    cs_uint16 X_LOS_qual_I         :  1 ; /* bits 13:13 */
    cs_uint16 X_LOS_raw_I          :  1 ; /* bits 14:14 */
    cs_uint16 rsrvd1               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_SectionInterrupt_t;

#define ARN5_FR_SONET32RX_SectionStatus          0x1c307
#define ARN5_FR_SONET32RX_SectionStatus_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 InFrameS             :  1 ; /* bits 15:15 */
    cs_uint16 X_LOS_raw_S          :  1 ; /* bits 14:14 */
    cs_uint16 X_LOS_qual_S         :  1 ; /* bits 13:13 */
    cs_uint16 PRBS_Rx_Cnt_MSBs     :  1 ; /* bits 12:12 */
    cs_uint16 HuntPRBSS            :  1 ; /* bits 11:11 */
    cs_uint16 ParityErrS           :  1 ; /* bits 10:10 */
    cs_uint16 PRBSErrS             :  1 ; /* bits 9:9 */
    cs_uint16 SecTrRcvS            :  1 ; /* bits 8:8 */
    cs_uint16 B1BIPS               :  1 ; /* bits 7:7 */
    cs_uint16 B1BIPES              :  1 ; /* bits 6:6 */
    cs_uint16 SecTrMisS            :  1 ; /* bits 5:5 */
    cs_uint16 SecTrUnsS            :  1 ; /* bits 4:4 */
    cs_uint16 SEFS                 :  1 ; /* bits 3:3 */
    cs_uint16 dZ0InconS            :  1 ; /* bits 2:2 */
    cs_uint16 LOFS                 :  1 ; /* bits 1:1 */
    cs_uint16 LOSS                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOSS                 :  1 ; /* bits 0:0 */
    cs_uint16 LOFS                 :  1 ; /* bits 1:1 */
    cs_uint16 dZ0InconS            :  1 ; /* bits 2:2 */
    cs_uint16 SEFS                 :  1 ; /* bits 3:3 */
    cs_uint16 SecTrUnsS            :  1 ; /* bits 4:4 */
    cs_uint16 SecTrMisS            :  1 ; /* bits 5:5 */
    cs_uint16 B1BIPES              :  1 ; /* bits 6:6 */
    cs_uint16 B1BIPS               :  1 ; /* bits 7:7 */
    cs_uint16 SecTrRcvS            :  1 ; /* bits 8:8 */
    cs_uint16 PRBSErrS             :  1 ; /* bits 9:9 */
    cs_uint16 ParityErrS           :  1 ; /* bits 10:10 */
    cs_uint16 HuntPRBSS            :  1 ; /* bits 11:11 */
    cs_uint16 PRBS_Rx_Cnt_MSBs     :  1 ; /* bits 12:12 */
    cs_uint16 X_LOS_qual_S         :  1 ; /* bits 13:13 */
    cs_uint16 X_LOS_raw_S          :  1 ; /* bits 14:14 */
    cs_uint16 InFrameS             :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_SectionStatus_t;

#define ARN5_FR_SONET32RX_SectionMask            0x1c308
#define ARN5_FR_SONET32RX_SectionMask_dft        0x7fff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 X_LOS_raw_M          :  1 ; /* bits 14:14 */
    cs_uint16 X_LOS_qual_M         :  1 ; /* bits 13:13 */
    cs_uint16 PRBS_Rx_Cnt_MSBm     :  1 ; /* bits 12:12 */
    cs_uint16 SerialHDLCM          :  1 ; /* bits 11:11 */
    cs_uint16 ParityErrM           :  1 ; /* bits 10:10 */
    cs_uint16 PRBSErrM             :  1 ; /* bits 9:9 */
    cs_uint16 SecTrRcvM            :  1 ; /* bits 8:8 */
    cs_uint16 B1BIPM               :  1 ; /* bits 7:7 */
    cs_uint16 B1BIPEM              :  1 ; /* bits 6:6 */
    cs_uint16 SecTrMisM            :  1 ; /* bits 5:5 */
    cs_uint16 SecTrUnsM            :  1 ; /* bits 4:4 */
    cs_uint16 SEFM                 :  1 ; /* bits 3:3 */
    cs_uint16 dZ0InconM            :  1 ; /* bits 2:2 */
    cs_uint16 LOFM                 :  1 ; /* bits 1:1 */
    cs_uint16 LOSM                 :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOSM                 :  1 ; /* bits 0:0 */
    cs_uint16 LOFM                 :  1 ; /* bits 1:1 */
    cs_uint16 dZ0InconM            :  1 ; /* bits 2:2 */
    cs_uint16 SEFM                 :  1 ; /* bits 3:3 */
    cs_uint16 SecTrUnsM            :  1 ; /* bits 4:4 */
    cs_uint16 SecTrMisM            :  1 ; /* bits 5:5 */
    cs_uint16 B1BIPEM              :  1 ; /* bits 6:6 */
    cs_uint16 B1BIPM               :  1 ; /* bits 7:7 */
    cs_uint16 SecTrRcvM            :  1 ; /* bits 8:8 */
    cs_uint16 PRBSErrM             :  1 ; /* bits 9:9 */
    cs_uint16 ParityErrM           :  1 ; /* bits 10:10 */
    cs_uint16 SerialHDLCM          :  1 ; /* bits 11:11 */
    cs_uint16 PRBS_Rx_Cnt_MSBm     :  1 ; /* bits 12:12 */
    cs_uint16 X_LOS_qual_M         :  1 ; /* bits 13:13 */
    cs_uint16 X_LOS_raw_M          :  1 ; /* bits 14:14 */
    cs_uint16 rsrvd1               :  1 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_SectionMask_t;

#define ARN5_FR_SONET32RX_B1BIPErrorCount        0x1c309
#define ARN5_FR_SONET32RX_B1BIPErrorCount_dft    0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 B1BE                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 B1BE                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_B1BIPErrorCount_t;

#define ARN5_FR_SONET32RX_LineInterrupt          0x1c30a
#define ARN5_FR_SONET32RX_LineInterrupt_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SerialHDLCI          :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 LREII                :  1 ; /* bits 6:6 */
    cs_uint16 LEREIEI              :  1 ; /* bits 5:5 */
    cs_uint16 B2BIPI               :  1 ; /* bits 4:4 */
    cs_uint16 B2BIPEI              :  1 ; /* bits 3:3 */
    cs_uint16 NewZ1S1I             :  1 ; /* bits 2:2 */
    cs_uint16 RDILI                :  1 ; /* bits 1:1 */
    cs_uint16 AISLI                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 AISLI                :  1 ; /* bits 0:0 */
    cs_uint16 RDILI                :  1 ; /* bits 1:1 */
    cs_uint16 NewZ1S1I             :  1 ; /* bits 2:2 */
    cs_uint16 B2BIPEI              :  1 ; /* bits 3:3 */
    cs_uint16 B2BIPI               :  1 ; /* bits 4:4 */
    cs_uint16 LEREIEI              :  1 ; /* bits 5:5 */
    cs_uint16 LREII                :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 SerialHDLCI          :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_LineInterrupt_t;

#define ARN5_FR_SONET32RX_LineStatus             0x1c30b
#define ARN5_FR_SONET32RX_LineStatus_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  9 ;
    cs_uint16 LREIS                :  1 ; /* bits 6:6 */
    cs_uint16 LEREIES              :  1 ; /* bits 5:5 */
    cs_uint16 B2BIPS               :  1 ; /* bits 4:4 */
    cs_uint16 B2BIPES              :  1 ; /* bits 3:3 */
    cs_uint16 NewZ1S1S             :  1 ; /* bits 2:2 */
    cs_uint16 RDILS                :  1 ; /* bits 1:1 */
    cs_uint16 AISLS                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 AISLS                :  1 ; /* bits 0:0 */
    cs_uint16 RDILS                :  1 ; /* bits 1:1 */
    cs_uint16 NewZ1S1S             :  1 ; /* bits 2:2 */
    cs_uint16 B2BIPES              :  1 ; /* bits 3:3 */
    cs_uint16 B2BIPS               :  1 ; /* bits 4:4 */
    cs_uint16 LEREIES              :  1 ; /* bits 5:5 */
    cs_uint16 LREIS                :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd1               :  9 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_LineStatus_t;

#define ARN5_FR_SONET32RX_LineMask               0x1c30c
#define ARN5_FR_SONET32RX_LineMask_dft           0x807f
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SerialHDLCM          :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 LREIM                :  1 ; /* bits 6:6 */
    cs_uint16 LEREIEM              :  1 ; /* bits 5:5 */
    cs_uint16 B2BIPM               :  1 ; /* bits 4:4 */
    cs_uint16 B2BIPEM              :  1 ; /* bits 3:3 */
    cs_uint16 NewZ1S1M             :  1 ; /* bits 2:2 */
    cs_uint16 RDILM                :  1 ; /* bits 1:1 */
    cs_uint16 AISLM                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 AISLM                :  1 ; /* bits 0:0 */
    cs_uint16 RDILM                :  1 ; /* bits 1:1 */
    cs_uint16 NewZ1S1M             :  1 ; /* bits 2:2 */
    cs_uint16 B2BIPEM              :  1 ; /* bits 3:3 */
    cs_uint16 B2BIPM               :  1 ; /* bits 4:4 */
    cs_uint16 LEREIEM              :  1 ; /* bits 5:5 */
    cs_uint16 LREIM                :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 SerialHDLCM          :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_LineMask_t;

#define ARN5_FR_SONET32RX_APSInterrupt           0x1c30d
#define ARN5_FR_SONET32RX_APSInterrupt_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 BadK1S               :  1 ; /* bits 15:15 */
    cs_uint16 BadK2S               :  1 ; /* bits 14:14 */
    cs_uint16 BadK3S               :  1 ; /* bits 13:13 */
    cs_uint16 BadF2S               :  1 ; /* bits 12:12 */
    cs_uint16 BadF2I               :  1 ; /* bits 11:11 */
    cs_uint16 NewF2I               :  1 ; /* bits 10:10 */
    cs_uint16 SFS                  :  1 ; /* bits 9:9 */
    cs_uint16 SDS                  :  1 ; /* bits 8:8 */
    cs_uint16 BadK1I               :  1 ; /* bits 7:7 */
    cs_uint16 BadK2I               :  1 ; /* bits 6:6 */
    cs_uint16 BadK3I               :  1 ; /* bits 5:5 */
    cs_uint16 NewK1I               :  1 ; /* bits 4:4 */
    cs_uint16 NewK2I               :  1 ; /* bits 3:3 */
    cs_uint16 NewK3I               :  1 ; /* bits 2:2 */
    cs_uint16 SFI                  :  1 ; /* bits 1:1 */
    cs_uint16 SDI                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SDI                  :  1 ; /* bits 0:0 */
    cs_uint16 SFI                  :  1 ; /* bits 1:1 */
    cs_uint16 NewK3I               :  1 ; /* bits 2:2 */
    cs_uint16 NewK2I               :  1 ; /* bits 3:3 */
    cs_uint16 NewK1I               :  1 ; /* bits 4:4 */
    cs_uint16 BadK3I               :  1 ; /* bits 5:5 */
    cs_uint16 BadK2I               :  1 ; /* bits 6:6 */
    cs_uint16 BadK1I               :  1 ; /* bits 7:7 */
    cs_uint16 SDS                  :  1 ; /* bits 8:8 */
    cs_uint16 SFS                  :  1 ; /* bits 9:9 */
    cs_uint16 NewF2I               :  1 ; /* bits 10:10 */
    cs_uint16 BadF2I               :  1 ; /* bits 11:11 */
    cs_uint16 BadF2S               :  1 ; /* bits 12:12 */
    cs_uint16 BadK3S               :  1 ; /* bits 13:13 */
    cs_uint16 BadK2S               :  1 ; /* bits 14:14 */
    cs_uint16 BadK1S               :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_APSInterrupt_t;

#define ARN5_FR_SONET32RX_APSMask                0x1c30e
#define ARN5_FR_SONET32RX_APSMask_dft            0x0cff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  4 ;
    cs_uint16 BadF2M               :  1 ; /* bits 11:11 */
    cs_uint16 NewF2M               :  1 ; /* bits 10:10 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 BadK1M               :  1 ; /* bits 7:7 */
    cs_uint16 BadK2M               :  1 ; /* bits 6:6 */
    cs_uint16 BadK3M               :  1 ; /* bits 5:5 */
    cs_uint16 NewK1M               :  1 ; /* bits 4:4 */
    cs_uint16 NewK2M               :  1 ; /* bits 3:3 */
    cs_uint16 NewK3M               :  1 ; /* bits 2:2 */
    cs_uint16 SFM                  :  1 ; /* bits 1:1 */
    cs_uint16 SDM                  :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SDM                  :  1 ; /* bits 0:0 */
    cs_uint16 SFM                  :  1 ; /* bits 1:1 */
    cs_uint16 NewK3M               :  1 ; /* bits 2:2 */
    cs_uint16 NewK2M               :  1 ; /* bits 3:3 */
    cs_uint16 NewK1M               :  1 ; /* bits 4:4 */
    cs_uint16 BadK3M               :  1 ; /* bits 5:5 */
    cs_uint16 BadK2M               :  1 ; /* bits 6:6 */
    cs_uint16 BadK1M               :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 NewF2M               :  1 ; /* bits 10:10 */
    cs_uint16 BadF2M               :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd2               :  4 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_APSMask_t;

#define ARN5_FR_SONET32RX_K1K2                   0x1c30f
#define ARN5_FR_SONET32RX_K1K2_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 K2                   :  8 ; /* bits 15:8 */
    cs_uint16 K1                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 K1                   :  8 ; /* bits 7:0 */
    cs_uint16 K2                   :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_K1K2_t;

#define ARN5_FR_SONET32RX_K3K4                   0x1c310
#define ARN5_FR_SONET32RX_K3K4_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 K4                   :  8 ; /* bits 15:8 */
    cs_uint16 K3                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 K3                   :  8 ; /* bits 7:0 */
    cs_uint16 K4                   :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_K3K4_t;

#define ARN5_FR_SONET32RX_Z1S1                   0x1c311
#define ARN5_FR_SONET32RX_Z1S1_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 Z1S1                 :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Z1S1                 :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_Z1S1_t;

#define ARN5_FR_SONET32RX_LBERAcc                0x1c312
#define ARN5_FR_SONET32RX_LBERAcc_dft            0xc001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Scale                :  2 ; /* bits 15:14 */
    cs_uint16 AccInterval          : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 AccInterval          : 14 ; /* bits 13:0 */
    cs_uint16 Scale                :  2 ; /* bits 15:14 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_LBERAcc_t;
#define     ARN5_Scale_SCALE_0                            0x00
#define     ARN5_Scale_SCALE_1                            0x01
#define     ARN5_Scale_SCALE_4                            0x02
#define     ARN5_Scale_SCALE_8                            0x03

#define ARN5_FR_SONET32RX_LBER                   0x1c313
#define ARN5_FR_SONET32RX_LBER_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 leakValue            : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 leakValue            : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_LBER_t;

#define ARN5_FR_SONET32RX_LBERDegradeDeclaring   0x1c314
#define ARN5_FR_SONET32RX_LBERDegradeDeclaring_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SDDT                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SDDT                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_LBERDegradeDeclaring_t;

#define ARN5_FR_SONET32RX_LBERDegradeClearing    0x1c315
#define ARN5_FR_SONET32RX_LBERDegradeClearing_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SDCT                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SDCT                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_LBERDegradeClearing_t;

#define ARN5_FR_SONET32RX_LBERFailureDeclaring   0x1c316
#define ARN5_FR_SONET32RX_LBERFailureDeclaring_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SFDT                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SFDT                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_LBERFailureDeclaring_t;

#define ARN5_FR_SONET32RX_LBERFailureClearing    0x1c317
#define ARN5_FR_SONET32RX_LBERFailureClearing_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SFCT                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SFCT                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_LBERFailureClearing_t;

#define ARN5_FR_SONET32RX_B2BIPErrorCountHigh    0x1c318
#define ARN5_FR_SONET32RX_B2BIPErrorCountHigh_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 B2BEH                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 B2BEH                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_B2BIPErrorCountHigh_t;

#define ARN5_FR_SONET32RX_B2BIPErrorCountLow     0x1c319
#define ARN5_FR_SONET32RX_B2BIPErrorCountLow_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 B2BEL                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 B2BEL                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_B2BIPErrorCountLow_t;

#define ARN5_FR_SONET32RX_LineREICountHigh       0x1c31a
#define ARN5_FR_SONET32RX_LineREICountHigh_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 LREIH                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LREIH                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_LineREICountHigh_t;

#define ARN5_FR_SONET32RX_LineREICountLow        0x1c31b
#define ARN5_FR_SONET32RX_LineREICountLow_dft    0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 LREIL                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LREIL                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_LineREICountLow_t;

#define ARN5_FR_SONET32RX_TestPortSelect         0x1c31c
#define ARN5_FR_SONET32RX_TestPortSelect_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 mux                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 mux                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_TestPortSelect_t;

#define ARN5_FR_SONET32RX_Z0_byte                0x1c31e
#define ARN5_FR_SONET32RX_Z0_byte_dft            0x00cc
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 Z0_Byte_Value        :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Z0_Byte_Value        :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_Z0_byte_t;

#define ARN5_FR_SONET32RX_PBERAcc                0x1c320
#define ARN5_FR_SONET32RX_PBERAcc_dft            0xc001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Scale                :  2 ; /* bits 15:14 */
    cs_uint16 AccInterval          : 14 ; /* bits 13:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 AccInterval          : 14 ; /* bits 13:0 */
    cs_uint16 Scale                :  2 ; /* bits 15:14 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PBERAcc_t;

#define ARN5_FR_SONET32RX_PBER                   0x1c321
#define ARN5_FR_SONET32RX_PBER_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 leakValue            : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 leakValue            : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PBER_t;

#define ARN5_FR_SONET32RX_PBERFailureDeclaring   0x1c322
#define ARN5_FR_SONET32RX_PBERFailureDeclaring_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SFDT                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SFDT                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PBERFailureDeclaring_t;

#define ARN5_FR_SONET32RX_PBERFailureClearing    0x1c323
#define ARN5_FR_SONET32RX_PBERFailureClearing_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SFCT                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 SFCT                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PBERFailureClearing_t;

#define ARN5_FR_SONET32RX_PathInterrupts         0x1c324
#define ARN5_FR_SONET32RX_PathInterrupts_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 PathI                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PathI                :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PathInterrupts_t;

#define ARN5_FR_SONET32RX_PathMasks              0x1c325
#define ARN5_FR_SONET32RX_PathMasks_dft          0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 15 ;
    cs_uint16 PathM                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PathM                :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               : 15 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PathMasks_t;

#define ARN5_FR_SONET32RX_pathTraceSelect        0x1c328
#define ARN5_FR_SONET32RX_pathTraceSelect_dft    0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  9 ;
    cs_uint16 pathNumber           :  2 ; /* bits 6:5 */
    cs_uint16 rsrvd1               :  5 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 pathNumber           :  2 ; /* bits 6:5 */
    cs_uint16 rsrvd2               :  9 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_pathTraceSelect_t;
#define     ARN5_pathNumber_SEL_PATH0                     0x00

#define ARN5_FR_SONET32RX_F2                     0x1c329
#define ARN5_FR_SONET32RX_F2_dft                 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 F2                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 F2                   :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_F2_t;

#define ARN5_FR_SONET32RX_X_LOS_Config           0x1c32a
#define ARN5_FR_SONET32RX_X_LOS_Config_dft       0x007a
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  9 ;
    cs_uint16 X_LOS_act_low        :  1 ; /* bits 6:6 */
    cs_uint16 X_LOS_SEF_e          :  1 ; /* bits 5:5 */
    cs_uint16 X_LOS_d_set_edge     :  1 ; /* bits 4:4 */
    cs_uint16 X_LOS_F_rst_edge     :  1 ; /* bits 3:3 */
    cs_uint16 X_LOS_d_set_dis      :  1 ; /* bits 2:2 */
    cs_uint16 X_LOS_F_rst_dis      :  1 ; /* bits 1:1 */
    cs_uint16 LOS_dLOF_clear_dis   :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LOS_dLOF_clear_dis   :  1 ; /* bits 0:0 */
    cs_uint16 X_LOS_F_rst_dis      :  1 ; /* bits 1:1 */
    cs_uint16 X_LOS_d_set_dis      :  1 ; /* bits 2:2 */
    cs_uint16 X_LOS_F_rst_edge     :  1 ; /* bits 3:3 */
    cs_uint16 X_LOS_d_set_edge     :  1 ; /* bits 4:4 */
    cs_uint16 X_LOS_SEF_e          :  1 ; /* bits 5:5 */
    cs_uint16 X_LOS_act_low        :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd1               :  9 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_X_LOS_Config_t;

#define ARN5_FR_SONET32RX_PRBSErrorCountHigh     0x1c330
#define ARN5_FR_SONET32RX_PRBSErrorCountHigh_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 PRBS_Err_CntH        : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PRBS_Err_CntH        : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PRBSErrorCountHigh_t;

#define ARN5_FR_SONET32RX_PRBSErrorCountLow      0x1c331
#define ARN5_FR_SONET32RX_PRBSErrorCountLow_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 PRBS_Err_CntL        : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PRBS_Err_CntL        : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PRBSErrorCountLow_t;

#define ARN5_FR_SONET32RX_PRBS_Rx_Provision      0x1c332
#define ARN5_FR_SONET32RX_PRBS_Rx_Provision_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 PRBS_Rx_Polynomial   :  2 ; /* bits 15:14 */
    cs_uint16 PRBS_Rx_Invert       :  1 ; /* bits 13:13 */
    cs_uint16 rsrvd1               : 13 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 PRBS_Rx_Invert       :  1 ; /* bits 13:13 */
    cs_uint16 PRBS_Rx_Polynomial   :  2 ; /* bits 15:14 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_PRBS_Rx_Provision_t;
#define     ARN5_PRBS_Rx_Polynomial_PRBS_Rx_2exp31        0x00
#define     ARN5_PRBS_Rx_Polynomial_PRBS_Rx_2exp23        0x01
#define     ARN5_PRBS_Rx_Polynomial_PRBS_Rx_2exp15        0x02
#define     ARN5_PRBS_Rx_Polynomial_PRBS_Rx_2exp7         0x03

#define ARN5_FR_SONET32RX_RxSectionTraceExp      0x1c340
#define ARN5_FR_SONET32RX_RxSectionTraceExp_STRIDE 1
#define ARN5_FR_SONET32RX_RxSectionTraceExp_COUNT 32
#define ARN5_FR_SONET32RX_RxSectionTraceExp_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 STBfr                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 STBfr                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_RxSectionTraceExp_t;

#define ARN5_FR_SONET32RX_RxPathTraceExp         0x1c360
#define ARN5_FR_SONET32RX_RxPathTraceExp_STRIDE  1
#define ARN5_FR_SONET32RX_RxPathTraceExp_COUNT   32
#define ARN5_FR_SONET32RX_RxPathTraceExp_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 PTBfr                : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 PTBfr                : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_SONET32RX_RxPathTraceExp_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * Framer::recvOverhead
 */

#define ARN5_FR_RECVOVERHEAD_pathOverheadA       0x1c402
#define ARN5_FR_RECVOVERHEAD_pathOverheadA_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 F2                   :  8 ; /* bits 15:8 */
    cs_uint16 C2                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 C2                   :  8 ; /* bits 7:0 */
    cs_uint16 F2                   :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RECVOVERHEAD_pathOverheadA_t;

#define ARN5_FR_RECVOVERHEAD_pathOverheadB       0x1c403
#define ARN5_FR_RECVOVERHEAD_pathOverheadB_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Z4                   :  8 ; /* bits 15:8 */
    cs_uint16 Z3                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Z3                   :  8 ; /* bits 7:0 */
    cs_uint16 Z4                   :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RECVOVERHEAD_pathOverheadB_t;

#define ARN5_FR_RECVOVERHEAD_pathOverheadC       0x1c404
#define ARN5_FR_RECVOVERHEAD_pathOverheadC_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 G1                   :  8 ; /* bits 15:8 */
    cs_uint16 Z5                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Z5                   :  8 ; /* bits 7:0 */
    cs_uint16 G1                   :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RECVOVERHEAD_pathOverheadC_t;

#define ARN5_FR_RECVOVERHEAD_Ctrl                0x1c700
#define ARN5_FR_RECVOVERHEAD_Ctrl_dft            0x0344
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 TOHBitOrder          :  1 ; /* bits 9:9 */
    cs_uint16 DCCBitOrder          :  1 ; /* bits 8:8 */
    cs_uint16 APSUpOverRide        :  1 ; /* bits 7:7 */
    cs_uint16 APSen                :  1 ; /* bits 6:6 */
    cs_uint16 APSmode              :  2 ; /* bits 5:4 */
    cs_uint16 DCCclkPolarity       :  1 ; /* bits 3:3 */
    cs_uint16 DCCmode              :  2 ; /* bits 2:1 */
    cs_uint16 reset                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 reset                :  1 ; /* bits 0:0 */
    cs_uint16 DCCmode              :  2 ; /* bits 2:1 */
    cs_uint16 DCCclkPolarity       :  1 ; /* bits 3:3 */
    cs_uint16 APSmode              :  2 ; /* bits 5:4 */
    cs_uint16 APSen                :  1 ; /* bits 6:6 */
    cs_uint16 APSUpOverRide        :  1 ; /* bits 7:7 */
    cs_uint16 DCCBitOrder          :  1 ; /* bits 8:8 */
    cs_uint16 TOHBitOrder          :  1 ; /* bits 9:9 */
    cs_uint16 rsrvd1               :  6 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RECVOVERHEAD_Ctrl_t;
#define     ARN5_APSmode_APS_K1_K4_S1                     0x00
#define     ARN5_APSmode_APS_RECV_ALM                     0x01
#define     ARN5_APSmode_APS_K1_ALM                       0x02
#define     ARN5_DCCmode_SEC_DCC                          0x00
#define     ARN5_DCCmode_LIN_DCC                          0x01
#define     ARN5_DCCmode_SEC_LIN_DCC                      0x02
#define     ARN5_DCCmode_DIS_DCC                          0x03

#define ARN5_FR_RECVOVERHEAD_SectRow2            0x1c706
#define ARN5_FR_RECVOVERHEAD_SectRow2_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 E1                   :  8 ; /* bits 15:8 */
    cs_uint16 F1                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 F1                   :  8 ; /* bits 7:0 */
    cs_uint16 E1                   :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RECVOVERHEAD_SectRow2_t;

#define ARN5_FR_RECVOVERHEAD_LineRow5a           0x1c707
#define ARN5_FR_RECVOVERHEAD_LineRow5a_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 K2                   :  8 ; /* bits 15:8 */
    cs_uint16 K1                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 K1                   :  8 ; /* bits 7:0 */
    cs_uint16 K2                   :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RECVOVERHEAD_LineRow5a_t;

#define ARN5_FR_RECVOVERHEAD_LineRow5b           0x1c708
#define ARN5_FR_RECVOVERHEAD_LineRow5b_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 K4                   :  8 ; /* bits 15:8 */
    cs_uint16 K3                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 K3                   :  8 ; /* bits 7:0 */
    cs_uint16 K4                   :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RECVOVERHEAD_LineRow5b_t;

#define ARN5_FR_RECVOVERHEAD_LineRow9            0x1c709
#define ARN5_FR_RECVOVERHEAD_LineRow9_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 E2                   :  8 ; /* bits 15:8 */
    cs_uint16 S1                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 S1                   :  8 ; /* bits 7:0 */
    cs_uint16 E2                   :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RECVOVERHEAD_LineRow9_t;

#define ARN5_FR_RECVOVERHEAD_TestCtrl            0x1c70a
#define ARN5_FR_RECVOVERHEAD_TestCtrl_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 Select               :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Select               :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RECVOVERHEAD_TestCtrl_t;
#define     ARN5_Select_SEC_DBG                           0x00
#define     ARN5_Select_APS_DBG1                          0x01
#define     ARN5_Select_APS_DBG2                          0x02
#define     ARN5_Select_Z1S1DBG                           0x03
#define     ARN5_Select_LIN_ALM_DBG                       0x04
#define     ARN5_Select_REIP0_DBG                         0x05
#define     ARN5_Select_PALM0_DBG                         0x06
#define     ARN5_Select_REIP1_DBG                         0x07
#define     ARN5_Select_PALM1_DBG                         0x08
#define     ARN5_Select_REIP2_DBG                         0x09
#define     ARN5_Select_PALM2_DBG                         0x0a
#define     ARN5_Select_REIP3_DBG                         0x0b
#define     ARN5_Select_PALM3_DBG                         0x0c


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * Framer::rxHDLC[0..1]
 */

#define ARN5_FR_RXHDLC_STRIDE                    32
#define ARN5_FR_RXHDLC_COUNT                     2
#define ARN5_FR_RXHDLC_Ctrl                      0x1c780
#define ARN5_FR_RXHDLC_Ctrl_dft                  0x002e
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  8 ;
    cs_uint16 Enable               :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 checkFCS             :  1 ; /* bits 5:5 */
    cs_uint16 passFCS              :  1 ; /* bits 4:4 */
    cs_uint16 dataMSBfirst         :  1 ; /* bits 3:3 */
    cs_uint16 CRCMSBfirst          :  1 ; /* bits 2:2 */
    cs_uint16 CRC32                :  1 ; /* bits 1:1 */
    cs_uint16 reset                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 reset                :  1 ; /* bits 0:0 */
    cs_uint16 CRC32                :  1 ; /* bits 1:1 */
    cs_uint16 CRCMSBfirst          :  1 ; /* bits 2:2 */
    cs_uint16 dataMSBfirst         :  1 ; /* bits 3:3 */
    cs_uint16 passFCS              :  1 ; /* bits 4:4 */
    cs_uint16 checkFCS             :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 Enable               :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd2               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RXHDLC_Ctrl_t;
#define     ARN5_CRC32_HDLC_CRC16                         0x00
#define     ARN5_CRC32_HDLC_CRC32                         0x01

#define ARN5_FR_RXHDLC_Bank0Status               0x1c781
#define ARN5_FR_RXHDLC_Bank0Status_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SOP                  :  1 ; /* bits 15:15 */
    cs_uint16 EOP                  :  1 ; /* bits 14:14 */
    cs_uint16 EOPErr               :  1 ; /* bits 13:13 */
    cs_uint16 MOP                  :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 PktVld               :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 EOPPtr               :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 EOPPtr               :  6 ; /* bits 5:0 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 PktVld               :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 MOP                  :  1 ; /* bits 12:12 */
    cs_uint16 EOPErr               :  1 ; /* bits 13:13 */
    cs_uint16 EOP                  :  1 ; /* bits 14:14 */
    cs_uint16 SOP                  :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RXHDLC_Bank0Status_t;

#define ARN5_FR_RXHDLC_Bank1Status               0x1c782
#define ARN5_FR_RXHDLC_Bank1Status_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SOP                  :  1 ; /* bits 15:15 */
    cs_uint16 EOP                  :  1 ; /* bits 14:14 */
    cs_uint16 EOPErr               :  1 ; /* bits 13:13 */
    cs_uint16 MOP                  :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 PktVld               :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 EOPPtr               :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 EOPPtr               :  6 ; /* bits 5:0 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 PktVld               :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 MOP                  :  1 ; /* bits 12:12 */
    cs_uint16 EOPErr               :  1 ; /* bits 13:13 */
    cs_uint16 EOP                  :  1 ; /* bits 14:14 */
    cs_uint16 SOP                  :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RXHDLC_Bank1Status_t;

#define ARN5_FR_RXHDLC_Interrupt                 0x1c783
#define ARN5_FR_RXHDLC_Interrupt_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 UnalignedDataErrorI  :  1 ; /* bits 8:8 */
    cs_uint16 PacketLengthErrorI   :  1 ; /* bits 7:7 */
    cs_uint16 EndOfPacketErrorI    :  1 ; /* bits 6:6 */
    cs_uint16 CRCErrorCountRolloverI :  1 ; /* bits 5:5 */
    cs_uint16 CRCFailI             :  1 ; /* bits 4:4 */
    cs_uint16 FIFOOverflowI        :  1 ; /* bits 3:3 */
    cs_uint16 FIFOUnderflowI       :  1 ; /* bits 2:2 */
    cs_uint16 FIFODone1I           :  1 ; /* bits 1:1 */
    cs_uint16 FIFODone0I           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FIFODone0I           :  1 ; /* bits 0:0 */
    cs_uint16 FIFODone1I           :  1 ; /* bits 1:1 */
    cs_uint16 FIFOUnderflowI       :  1 ; /* bits 2:2 */
    cs_uint16 FIFOOverflowI        :  1 ; /* bits 3:3 */
    cs_uint16 CRCFailI             :  1 ; /* bits 4:4 */
    cs_uint16 CRCErrorCountRolloverI :  1 ; /* bits 5:5 */
    cs_uint16 EndOfPacketErrorI    :  1 ; /* bits 6:6 */
    cs_uint16 PacketLengthErrorI   :  1 ; /* bits 7:7 */
    cs_uint16 UnalignedDataErrorI  :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RXHDLC_Interrupt_t;

#define ARN5_FR_RXHDLC_Mask                      0x1c784
#define ARN5_FR_RXHDLC_Mask_dft                  0x01ff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  7 ;
    cs_uint16 UnalignedDataErrorM  :  1 ; /* bits 8:8 */
    cs_uint16 PacketLengthErrorM   :  1 ; /* bits 7:7 */
    cs_uint16 EndOfPacketErrorM    :  1 ; /* bits 6:6 */
    cs_uint16 CRCErrorCountRolloverM :  1 ; /* bits 5:5 */
    cs_uint16 CRCFailM             :  1 ; /* bits 4:4 */
    cs_uint16 FIFOOverflowM        :  1 ; /* bits 3:3 */
    cs_uint16 FIFOUnderflowM       :  1 ; /* bits 2:2 */
    cs_uint16 FIFODone1M           :  1 ; /* bits 1:1 */
    cs_uint16 FIFODone0M           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FIFODone0M           :  1 ; /* bits 0:0 */
    cs_uint16 FIFODone1M           :  1 ; /* bits 1:1 */
    cs_uint16 FIFOUnderflowM       :  1 ; /* bits 2:2 */
    cs_uint16 FIFOOverflowM        :  1 ; /* bits 3:3 */
    cs_uint16 CRCFailM             :  1 ; /* bits 4:4 */
    cs_uint16 CRCErrorCountRolloverM :  1 ; /* bits 5:5 */
    cs_uint16 EndOfPacketErrorM    :  1 ; /* bits 6:6 */
    cs_uint16 PacketLengthErrorM   :  1 ; /* bits 7:7 */
    cs_uint16 UnalignedDataErrorM  :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  7 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RXHDLC_Mask_t;

#define ARN5_FR_RXHDLC_Bank0Read                 0x1c785
#define ARN5_FR_RXHDLC_Bank0Read_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Data                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Data                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RXHDLC_Bank0Read_t;

#define ARN5_FR_RXHDLC_Bank1Read                 0x1c786
#define ARN5_FR_RXHDLC_Bank1Read_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Data                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Data                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RXHDLC_Bank1Read_t;

#define ARN5_FR_RXHDLC_FIFOTestData              0x1c787
#define ARN5_FR_RXHDLC_FIFOTestData_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Word                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Word                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RXHDLC_FIFOTestData_t;

#define ARN5_FR_RXHDLC_FIFOTestCtrl              0x1c788
#define ARN5_FR_RXHDLC_FIFOTestCtrl_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  6 ;
    cs_uint16 Ram1Enable           :  1 ; /* bits 9:9 */
    cs_uint16 Ram0Enable           :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 Addr                 :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Addr                 :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 Ram0Enable           :  1 ; /* bits 8:8 */
    cs_uint16 Ram1Enable           :  1 ; /* bits 9:9 */
    cs_uint16 rsrvd2               :  6 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RXHDLC_FIFOTestCtrl_t;

#define ARN5_FR_RXHDLC_FIFOTestEOPPtr            0x1c789
#define ARN5_FR_RXHDLC_FIFOTestEOPPtr_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Set1                 :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 Bank1                :  6 ; /* bits 13:8 */
    cs_uint16 Set0                 :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 Bank0                :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Bank0                :  6 ; /* bits 5:0 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 Set0                 :  1 ; /* bits 7:7 */
    cs_uint16 Bank1                :  6 ; /* bits 13:8 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 Set1                 :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RXHDLC_FIFOTestEOPPtr_t;

#define ARN5_FR_RXHDLC_Status                    0x1c78b
#define ARN5_FR_RXHDLC_Status_dft                0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 CRCErrorCount        : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 CRCErrorCount        : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RXHDLC_Status_t;

#define ARN5_FR_RXHDLC_TestCtrl                  0x1c78c
#define ARN5_FR_RXHDLC_TestCtrl_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 Select               :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Select               :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_RXHDLC_TestCtrl_t;
#define     ARN5_Select_SEL_BIT_PROC                      0x00
#define     ARN5_Select_SEL_STUFF_BYT                     0x01
#define     ARN5_Select_SEL_FCS0                          0x02
#define     ARN5_Select_SEL_FCS1                          0x03
#define     ARN5_Select_SEL_FCS2                          0x04
#define     ARN5_Select_SEL_FCS3                          0x05
#define     ARN5_Select_SEL_CRC_STS                       0x06
#define     ARN5_Select_SEL_FIF0_CTL                      0x07
#define     ARN5_Select_SEL_FIFO0_STS                     0x08
#define     ARN5_Select_SEL_FIFO1_STS                     0x09
#define     ARN5_Select_SEL_RAM0DATA0                     0x0a
#define     ARN5_Select_SEL_RAM0DATA1                     0x0b
#define     ARN5_Select_SEL_RAM1DATA0                     0x0c
#define     ARN5_Select_SEL_RAM1DATA1                     0x0d
#define     ARN5_Select_SEL_RAM0ADDR                      0x0e
#define     ARN5_Select_SEL_RAM1ADDR                      0x0f
#define     ARN5_Select_SEL_PKTSTS                        0x10
#define     ARN5_Select_SEL_BYT_CNT                       0x11


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * Framer::transmit
 */

#define ARN5_FR_TRANSMIT_PathSPEPointerControl   0x1e000
#define ARN5_FR_TRANSMIT_PathSPEPointerControl_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 InvBIP               :  1 ; /* bits 15:15 */
    cs_uint16 InvBIPOnce           :  1 ; /* bits 14:14 */
    cs_uint16 rsrvd2               :  6 ;
    cs_uint16 insPAIS              :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 InsPDIP              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 InsPDIP              :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 insPAIS              :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd2               :  6 ;
    cs_uint16 InvBIPOnce           :  1 ; /* bits 14:14 */
    cs_uint16 InvBIP               :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TRANSMIT_PathSPEPointerControl_t;

#define ARN5_FR_TRANSMIT_PathProvisioning        0x1e001
#define ARN5_FR_TRANSMIT_PathProvisioning_dft    0x0061
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  6 ;
    cs_uint16 QID                  :  2 ; /* bits 9:8 */
    cs_uint16 SVC                  :  3 ; /* bits 7:5 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 STStype              :  3 ; /* bits 2:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 STStype              :  3 ; /* bits 2:0 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 SVC                  :  3 ; /* bits 7:5 */
    cs_uint16 QID                  :  2 ; /* bits 9:8 */
    cs_uint16 rsrvd2               :  6 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TRANSMIT_PathProvisioning_t;
#define     ARN5_QID_TX_SEL_STRM0                         0x00
#define     ARN5_QID_TX_SEL_STRM1                         0x01
#define     ARN5_QID_TX_SEL_STRM2                         0x02
#define     ARN5_QID_TX_SEL_STRM3                         0x03
#define     ARN5_SVC_TX_SVC_ATM                           0x02
#define     ARN5_SVC_TX_SVC_HDLC                          0x03
#define     ARN5_SVC_TX_SVC_GFP                           0x04
#define     ARN5_SVC_TX_SVC_PRBS                          0x06
#define     ARN5_SVC_TX_SVC_UNEQP                         0x07
#define     ARN5_STStype_TX_STSTYP_3                      0x01
#define     ARN5_STStype_TX_STSTYP_12                     0x02
#define     ARN5_STStype_TX_STSTYP_48                     0x04
#define     ARN5_STStype_TX_STSTYP_UNEQP                  0x07

#define ARN5_FR_TRANSMIT_PathSPEForceControl     0x1e002
#define ARN5_FR_TRANSMIT_PathSPEForceControl_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 ForceDNF             :  1 ; /* bits 15:15 */
    cs_uint16 ForcePPJOnce         :  1 ; /* bits 14:14 */
    cs_uint16 ForceNPJOnce         :  1 ; /* bits 13:13 */
    cs_uint16 rsrvd1               : 10 ;
    cs_uint16 PtrJusDly            :  1 ; /* bits 2:2 */
    cs_uint16 InsNPJE              :  1 ; /* bits 1:1 */
    cs_uint16 InsPPJE              :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 InsPPJE              :  1 ; /* bits 0:0 */
    cs_uint16 InsNPJE              :  1 ; /* bits 1:1 */
    cs_uint16 PtrJusDly            :  1 ; /* bits 2:2 */
    cs_uint16 rsrvd1               : 10 ;
    cs_uint16 ForceNPJOnce         :  1 ; /* bits 13:13 */
    cs_uint16 ForcePPJOnce         :  1 ; /* bits 14:14 */
    cs_uint16 ForceDNF             :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TRANSMIT_PathSPEForceControl_t;

#define ARN5_FR_TRANSMIT_PathNewPointer          0x1e003
#define ARN5_FR_TRANSMIT_PathNewPointer_dft      0x6000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 NDF                  :  4 ; /* bits 15:12 */
    cs_uint16 SS                   :  2 ; /* bits 11:10 */
    cs_uint16 Ptr                  : 10 ; /* bits 9:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Ptr                  : 10 ; /* bits 9:0 */
    cs_uint16 SS                   :  2 ; /* bits 11:10 */
    cs_uint16 NDF                  :  4 ; /* bits 15:12 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TRANSMIT_PathNewPointer_t;

#define ARN5_FR_TRANSMIT_PathConcat              0x1e004
#define ARN5_FR_TRANSMIT_PathConcat_dft          0x93ff
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 H1                   :  8 ; /* bits 15:8 */
    cs_uint16 H2                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 H2                   :  8 ; /* bits 7:0 */
    cs_uint16 H1                   :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TRANSMIT_PathConcat_t;

#define ARN5_FR_TRANSMIT_Configuration           0x1e300
#define ARN5_FR_TRANSMIT_Configuration_dft       0x0001
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Reset                :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd2               :  7 ;
    cs_uint16 ShortFrame           :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 EvenParity           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 EvenParity           :  1 ; /* bits 0:0 */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 ShortFrame           :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd2               :  7 ;
    cs_uint16 Reset                :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TRANSMIT_Configuration_t;

#define ARN5_FR_TRANSMIT_SectionControl          0x1e301
#define ARN5_FR_TRANSMIT_SectionControl_dft      0x4000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 ExtFrSync            :  1 ; /* bits 15:15 */
    cs_uint16 IgnFrSync            :  1 ; /* bits 14:14 */
    cs_uint16 rsrvd2               :  4 ;
    cs_uint16 InsPrbsErr           :  1 ; /* bits 9:9 */
    cs_uint16 Ins1010              :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 B2BIPErr             :  1 ; /* bits 5:5 */
    cs_uint16 B1BIPErr             :  1 ; /* bits 4:4 */
    cs_uint16 FrmErr               :  1 ; /* bits 3:3 */
    cs_uint16 InsLOS               :  1 ; /* bits 2:2 */
    cs_uint16 InsOnes              :  1 ; /* bits 1:1 */
    cs_uint16 DisScr               :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 DisScr               :  1 ; /* bits 0:0 */
    cs_uint16 InsOnes              :  1 ; /* bits 1:1 */
    cs_uint16 InsLOS               :  1 ; /* bits 2:2 */
    cs_uint16 FrmErr               :  1 ; /* bits 3:3 */
    cs_uint16 B1BIPErr             :  1 ; /* bits 4:4 */
    cs_uint16 B2BIPErr             :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 Ins1010              :  1 ; /* bits 8:8 */
    cs_uint16 InsPrbsErr           :  1 ; /* bits 9:9 */
    cs_uint16 rsrvd2               :  4 ;
    cs_uint16 IgnFrSync            :  1 ; /* bits 14:14 */
    cs_uint16 ExtFrSync            :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TRANSMIT_SectionControl_t;

#define ARN5_FR_TRANSMIT_FrameDelayCnt           0x1e302
#define ARN5_FR_TRANSMIT_FrameDelayCnt_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 FrameDelay           : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FrameDelay           : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TRANSMIT_FrameDelayCnt_t;

#define ARN5_FR_TRANSMIT_Stuff                   0x1e303
#define ARN5_FR_TRANSMIT_Stuff_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 Stf                  :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Stf                  :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TRANSMIT_Stuff_t;

#define ARN5_FR_TRANSMIT_TxInterrupt             0x1e305
#define ARN5_FR_TRANSMIT_TxInterrupt_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 hdlcTxLDCCInterrupt  :  1 ; /* bits 15:15 */
    cs_uint16 hdlcTxSDCCInterrupt  :  1 ; /* bits 14:14 */
    cs_uint16 rsrvd1               : 14 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 hdlcTxSDCCInterrupt  :  1 ; /* bits 14:14 */
    cs_uint16 hdlcTxLDCCInterrupt  :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TRANSMIT_TxInterrupt_t;

#define ARN5_FR_TRANSMIT_TxMask                  0x1e306
#define ARN5_FR_TRANSMIT_TxMask_dft              0xc000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 hdlcTxLDCCMask       :  1 ; /* bits 15:15 */
    cs_uint16 hdlcTxSDCCMask       :  1 ; /* bits 14:14 */
    cs_uint16 rsrvd1               : 14 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 hdlcTxSDCCMask       :  1 ; /* bits 14:14 */
    cs_uint16 hdlcTxLDCCMask       :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TRANSMIT_TxMask_t;

#define ARN5_FR_TRANSMIT_Debug                   0x1e307
#define ARN5_FR_TRANSMIT_Debug_dft               0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Addr                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Addr                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TRANSMIT_Debug_t;

#define ARN5_FR_TRANSMIT_sectionDiagnostics      0x1e308
#define ARN5_FR_TRANSMIT_sectionDiagnostics_dft  0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 InsB1ErrOnce         :  1 ; /* bits 15:15 */
    cs_uint16 InsB2ErrOnce         :  1 ; /* bits 14:14 */
    cs_uint16 InsPrbsOnce          :  1 ; /* bits 13:13 */
    cs_uint16 InsPrbsOneBitErr     :  1 ; /* bits 12:12 */
    cs_uint16 InsPrbsOneBitErrorOnce :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd1               : 11 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 InsPrbsOneBitErrorOnce :  1 ; /* bits 11:11 */
    cs_uint16 InsPrbsOneBitErr     :  1 ; /* bits 12:12 */
    cs_uint16 InsPrbsOnce          :  1 ; /* bits 13:13 */
    cs_uint16 InsB2ErrOnce         :  1 ; /* bits 14:14 */
    cs_uint16 InsB1ErrOnce         :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TRANSMIT_sectionDiagnostics_t;

#define ARN5_FR_TRANSMIT_PRBS_Tx_Provision       0x1e309
#define ARN5_FR_TRANSMIT_PRBS_Tx_Provision_dft   0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 PRBS_Tx_Polynomial   :  2 ; /* bits 15:14 */
    cs_uint16 PRBS_Tx_Invert       :  1 ; /* bits 13:13 */
    cs_uint16 rsrvd1               : 13 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 PRBS_Tx_Invert       :  1 ; /* bits 13:13 */
    cs_uint16 PRBS_Tx_Polynomial   :  2 ; /* bits 15:14 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TRANSMIT_PRBS_Tx_Provision_t;
#define     ARN5_PRBS_Tx_Polynomial_PRBS_Tx_2exp31        0x00
#define     ARN5_PRBS_Tx_Polynomial_PRBS_Tx_2exp23        0x01
#define     ARN5_PRBS_Tx_Polynomial_PRBS_Tx_2exp15        0x02
#define     ARN5_PRBS_Tx_Polynomial_PRBS_Tx_2exp7         0x03

#define ARN5_FR_TRANSMIT_Ext_Frame_Range         0x1e30c
#define ARN5_FR_TRANSMIT_Ext_Frame_Range_dft     0x0004
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 Ext_Tx_Frame_Range   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Ext_Tx_Frame_Range   :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TRANSMIT_Ext_Frame_Range_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * Framer::xmitOverhead
 */

#define ARN5_FR_XMITOVERHEAD_PathC2Z5            0x1e400
#define ARN5_FR_XMITOVERHEAD_PathC2Z5_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Z5                   :  8 ; /* bits 15:8 */
    cs_uint16 C2                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 C2                   :  8 ; /* bits 7:0 */
    cs_uint16 Z5                   :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_PathC2Z5_t;

#define ARN5_FR_XMITOVERHEAD_PathUserInd         0x1e401
#define ARN5_FR_XMITOVERHEAD_PathUserInd_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 F2                   :  8 ; /* bits 15:8 */
    cs_uint16 H4                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 H4                   :  8 ; /* bits 7:0 */
    cs_uint16 F2                   :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_PathUserInd_t;

#define ARN5_FR_XMITOVERHEAD_PathGrowth          0x1e402
#define ARN5_FR_XMITOVERHEAD_PathGrowth_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Z3                   :  8 ; /* bits 15:8 */
    cs_uint16 Z4                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Z4                   :  8 ; /* bits 7:0 */
    cs_uint16 Z3                   :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_PathGrowth_t;

#define ARN5_FR_XMITOVERHEAD_PathAlarmDefectCtrl 0x1e403
#define ARN5_FR_XMITOVERHEAD_PathAlarmDefectCtrl_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 EnhRDI               :  1 ; /* bits 15:15 */
    cs_uint16 rsrvd2               :  8 ;
    cs_uint16 PTU2RDI              :  1 ; /* bits 6:6 */
    cs_uint16 PTM2RDI              :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 PSLU2RDI             :  1 ; /* bits 3:3 */
    cs_uint16 PSLM2RDI             :  1 ; /* bits 2:2 */
    cs_uint16 PAIS2RDI             :  1 ; /* bits 1:1 */
    cs_uint16 UNEQP2RDI            :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 UNEQP2RDI            :  1 ; /* bits 0:0 */
    cs_uint16 PAIS2RDI             :  1 ; /* bits 1:1 */
    cs_uint16 PSLM2RDI             :  1 ; /* bits 2:2 */
    cs_uint16 PSLU2RDI             :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 PTM2RDI              :  1 ; /* bits 5:5 */
    cs_uint16 PTU2RDI              :  1 ; /* bits 6:6 */
    cs_uint16 rsrvd2               :  8 ;
    cs_uint16 EnhRDI               :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_PathAlarmDefectCtrl_t;

#define ARN5_FR_XMITOVERHEAD_alarm3Path          0x1e404
#define ARN5_FR_XMITOVERHEAD_alarm3Path_dft      0x2000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 insRDIP              :  3 ; /* bits 15:13 */
    cs_uint16 rsrvd1               : 13 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               : 13 ;
    cs_uint16 insRDIP              :  3 ; /* bits 15:13 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_alarm3Path_t;

#define ARN5_FR_XMITOVERHEAD_alarm4Path          0x1e405
#define ARN5_FR_XMITOVERHEAD_alarm4Path_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 B3ErCnt              :  4 ; /* bits 3:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 B3ErCnt              :  4 ; /* bits 3:0 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_alarm4Path_t;

#define ARN5_FR_XMITOVERHEAD_Ctrl                0x1e700
#define ARN5_FR_XMITOVERHEAD_Ctrl_dft            0x0604
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  5 ;
    cs_uint16 TOHBitOrder          :  1 ; /* bits 10:10 */
    cs_uint16 DCCBitOrder          :  1 ; /* bits 9:9 */
    cs_uint16 APSenable            :  1 ; /* bits 8:8 */
    cs_uint16 APSUpOverRide        :  1 ; /* bits 7:7 */
    cs_uint16 SDHMode              :  1 ; /* bits 6:6 */
    cs_uint16 Fixed_Z0             :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 DCCclkPolarity       :  1 ; /* bits 3:3 */
    cs_uint16 DCCmode              :  2 ; /* bits 2:1 */
    cs_uint16 reset                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 reset                :  1 ; /* bits 0:0 */
    cs_uint16 DCCmode              :  2 ; /* bits 2:1 */
    cs_uint16 DCCclkPolarity       :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 Fixed_Z0             :  1 ; /* bits 5:5 */
    cs_uint16 SDHMode              :  1 ; /* bits 6:6 */
    cs_uint16 APSUpOverRide        :  1 ; /* bits 7:7 */
    cs_uint16 APSenable            :  1 ; /* bits 8:8 */
    cs_uint16 DCCBitOrder          :  1 ; /* bits 9:9 */
    cs_uint16 TOHBitOrder          :  1 ; /* bits 10:10 */
    cs_uint16 rsrvd2               :  5 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_Ctrl_t;

#define ARN5_FR_XMITOVERHEAD_ByteCtrl1           0x1e701
#define ARN5_FR_XMITOVERHEAD_ByteCtrl1_dft       0xc000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 J0Src                :  2 ; /* bits 15:14 */
    cs_uint16 lineDCCSrc           :  2 ; /* bits 13:12 */
    cs_uint16 SecDCCSrc            :  2 ; /* bits 11:10 */
    cs_uint16 S1Src                :  2 ; /* bits 9:8 */
    cs_uint16 K3K4Src              :  1 ; /* bits 7:7 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 K1K2Src              :  2 ; /* bits 5:4 */
    cs_uint16 K2RDISrc             :  2 ; /* bits 3:2 */
    cs_uint16 F1Src                :  1 ; /* bits 1:1 */
    cs_uint16 E1Src                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 E1Src                :  1 ; /* bits 0:0 */
    cs_uint16 F1Src                :  1 ; /* bits 1:1 */
    cs_uint16 K2RDISrc             :  2 ; /* bits 3:2 */
    cs_uint16 K1K2Src              :  2 ; /* bits 5:4 */
    cs_uint16 rsrvd1               :  1 ;
    cs_uint16 K3K4Src              :  1 ; /* bits 7:7 */
    cs_uint16 S1Src                :  2 ; /* bits 9:8 */
    cs_uint16 SecDCCSrc            :  2 ; /* bits 11:10 */
    cs_uint16 lineDCCSrc           :  2 ; /* bits 13:12 */
    cs_uint16 J0Src                :  2 ; /* bits 15:14 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_ByteCtrl1_t;
#define     ARN5_J0Src_JOSRC_TRBUF                        0x00
#define     ARN5_J0Src_J0SRC_TOH                          0x01
#define     ARN5_J0Src_J0SRC_DEF                          0x02
#define     ARN5_lineDCCSrc_LDCCSRC_UP                    0x00
#define     ARN5_lineDCCSrc_LDCCSRC_DCCP                  0x01
#define     ARN5_lineDCCSrc_LDCCSRC_TOHP                  0x02
#define     ARN5_lineDCCSrc_LDCCSRC_HDLC                  0x03
#define     ARN5_SecDCCSrc_SDCCSRC_UP                     0x00
#define     ARN5_SecDCCSrc_SDCCSRC_DCCP                   0x01
#define     ARN5_SecDCCSrc_SDCCSRC_TOHP                   0x02
#define     ARN5_SecDCCSrc_SDCCSRC_HDLC                   0x03
#define     ARN5_S1Src_S1SRC_UP                           0x00
#define     ARN5_S1Src_S1SRC_TOHP                         0x02
#define     ARN5_S1Src_S1SRC_APS                          0x03
#define     ARN5_K1K2Src_K1K2SRC_UP                       0x00
#define     ARN5_K1K2Src_K1K2SRC_TOHP                     0x02
#define     ARN5_K1K2Src_K1K2SRC_APS                      0x03
#define     ARN5_K2RDISrc_RDILSRC_RX                      0x00
#define     ARN5_K2RDISrc_RDILSRC_UP                      0x02
#define     ARN5_K2RDISrc_RDILSRC_APS                     0x03

#define ARN5_FR_XMITOVERHEAD_ByteCtrl2           0x1e702
#define ARN5_FR_XMITOVERHEAD_ByteCtrl2_dft       0x0502
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 J1Src                :  1 ; /* bits 12:12 */
    cs_uint16 REIPSrc              :  2 ; /* bits 11:10 */
    cs_uint16 RDIPSrc              :  2 ; /* bits 9:8 */
    cs_uint16 rsrvd1               :  4 ;
    cs_uint16 E2Src                :  1 ; /* bits 3:3 */
    cs_uint16 M1Src                :  2 ; /* bits 2:1 */
    cs_uint16 Z2Src                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Z2Src                :  1 ; /* bits 0:0 */
    cs_uint16 M1Src                :  2 ; /* bits 2:1 */
    cs_uint16 E2Src                :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               :  4 ;
    cs_uint16 RDIPSrc              :  2 ; /* bits 9:8 */
    cs_uint16 REIPSrc              :  2 ; /* bits 11:10 */
    cs_uint16 J1Src                :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd2               :  3 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_ByteCtrl2_t;
#define     ARN5_REIPSrc_REIPSRC_APS                      0x00
#define     ARN5_REIPSrc_REIPSRC_RX                       0x01
#define     ARN5_REIPSrc_REIPSRC_UP                       0x02
#define     ARN5_RDIPSrc_RDIPSRC_APS                      0x00
#define     ARN5_RDIPSrc_RDIPSRC_RX                       0x01
#define     ARN5_RDIPSrc_RDIPSRC_UP                       0x02
#define     ARN5_E2Src_E2SRC_UP                           0x00
#define     ARN5_E2Src_E2SRC_TOH                          0x01
#define     ARN5_M1Src_M1SRC_APS                          0x00
#define     ARN5_M1Src_M1SRC_RX                           0x01
#define     ARN5_M1Src_M1SRC_UP                           0x02
#define     ARN5_Z2Src_Z2SRC_UP                           0x00
#define     ARN5_Z2Src_Z2SRC_TOH                          0x01

#define ARN5_FR_XMITOVERHEAD_OrderWire           0x1e703
#define ARN5_FR_XMITOVERHEAD_OrderWire_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 E2                   :  8 ; /* bits 15:8 */
    cs_uint16 E1                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 E1                   :  8 ; /* bits 7:0 */
    cs_uint16 E2                   :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_OrderWire_t;

#define ARN5_FR_XMITOVERHEAD_UserChannel         0x1e704
#define ARN5_FR_XMITOVERHEAD_UserChannel_dft     0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 F1                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 F1                   :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_UserChannel_t;

#define ARN5_FR_XMITOVERHEAD_DCC                 0x1e705
#define ARN5_FR_XMITOVERHEAD_DCC_dft             0x7e7e
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Sect                 :  8 ; /* bits 15:8 */
    cs_uint16 Line                 :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Line                 :  8 ; /* bits 7:0 */
    cs_uint16 Sect                 :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_DCC_t;

#define ARN5_FR_XMITOVERHEAD_Default             0x1e706
#define ARN5_FR_XMITOVERHEAD_Default_dft         0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 Byte                 :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Byte                 :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_Default_t;

#define ARN5_FR_XMITOVERHEAD_Growth              0x1e707
#define ARN5_FR_XMITOVERHEAD_Growth_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 Z2                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Z2                   :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_Growth_t;

#define ARN5_FR_XMITOVERHEAD_LineAlarmDefectCtrl 0x1e708
#define ARN5_FR_XMITOVERHEAD_LineAlarmDefectCtrl_dft 0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd3               :  2 ;
    cs_uint16 AISLSrc              :  1 ; /* bits 13:13 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 STM2RDIL             :  1 ; /* bits 11:11 */
    cs_uint16 STU2RDIL             :  1 ; /* bits 10:10 */
    cs_uint16 BERSF2RDIL           :  1 ; /* bits 9:9 */
    cs_uint16 SD2RDIL              :  1 ; /* bits 8:8 */
    cs_uint16 APSMode              :  2 ; /* bits 7:6 */
    cs_uint16 rsrvd1               :  6 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  6 ;
    cs_uint16 APSMode              :  2 ; /* bits 7:6 */
    cs_uint16 SD2RDIL              :  1 ; /* bits 8:8 */
    cs_uint16 BERSF2RDIL           :  1 ; /* bits 9:9 */
    cs_uint16 STU2RDIL             :  1 ; /* bits 10:10 */
    cs_uint16 STM2RDIL             :  1 ; /* bits 11:11 */
    cs_uint16 rsrvd2               :  1 ;
    cs_uint16 AISLSrc              :  1 ; /* bits 13:13 */
    cs_uint16 rsrvd3               :  2 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_LineAlarmDefectCtrl_t;
#define     ARN5_APSMode_APS_BYTES                        0x00
#define     ARN5_APSMode_ALRM_BITS                        0x01
#define     ARN5_APSMode_APS_ALRM                         0x02

#define ARN5_FR_XMITOVERHEAD_APS1                0x1e709
#define ARN5_FR_XMITOVERHEAD_APS1_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 K2                   :  8 ; /* bits 15:8 */
    cs_uint16 K1                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 K1                   :  8 ; /* bits 7:0 */
    cs_uint16 K2                   :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_APS1_t;

#define ARN5_FR_XMITOVERHEAD_APS2                0x1e70a
#define ARN5_FR_XMITOVERHEAD_APS2_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 K4                   :  8 ; /* bits 15:8 */
    cs_uint16 K3                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 K3                   :  8 ; /* bits 7:0 */
    cs_uint16 K4                   :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_APS2_t;

#define ARN5_FR_XMITOVERHEAD_SyncStatus          0x1e70b
#define ARN5_FR_XMITOVERHEAD_SyncStatus_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 S1                   :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 S1                   :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_SyncStatus_t;

#define ARN5_FR_XMITOVERHEAD_alarm1              0x1e70c
#define ARN5_FR_XMITOVERHEAD_alarm1_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 insAISL              :  1 ; /* bits 15:15 */
    cs_uint16 insRDIL              :  1 ; /* bits 14:14 */
    cs_uint16 rsrvd1               : 14 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               : 14 ;
    cs_uint16 insRDIL              :  1 ; /* bits 14:14 */
    cs_uint16 insAISL              :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_alarm1_t;

#define ARN5_FR_XMITOVERHEAD_alarm2              0x1e70d
#define ARN5_FR_XMITOVERHEAD_alarm2_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 B2ErCnt              :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 B2ErCnt              :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_alarm2_t;

#define ARN5_FR_XMITOVERHEAD_Z0_byte             0x1e713
#define ARN5_FR_XMITOVERHEAD_Z0_byte_dft         0x00cc
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               :  8 ;
    cs_uint16 Z0_Byte_Value        :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Z0_Byte_Value        :  8 ; /* bits 7:0 */
    cs_uint16 rsrvd1               :  8 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_Z0_byte_t;

#define ARN5_FR_XMITOVERHEAD_TestCtrl            0x1e714
#define ARN5_FR_XMITOVERHEAD_TestCtrl_dft        0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 Select               :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Select               :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_TestCtrl_t;
#define     ARN5_Select_SECDCC_DBG                        0x00
#define     ARN5_Select_LinDCC_DBG                        0x01
#define     ARN5_Select_G1Pth0_DBG                        0x02
#define     ARN5_Select_G1Pth1_DBG                        0x03
#define     ARN5_Select_G1Pth2_DBG                        0x04
#define     ARN5_Select_G1Pth3_DBG                        0x05
#define     ARN5_Select_OH4_DBG                           0x06
#define     ARN5_Select_OH3_DBG                           0x07
#define     ARN5_Select_OH2_DBG                           0x08
#define     ARN5_Select_OH1_DBG                           0x09
#define     ARN5_Select_LINE_DBG                          0x0a
#define     ARN5_Select_M1_DBG                            0x0b

#define ARN5_FR_XMITOVERHEAD_PathBuffer          0x1e728
#define ARN5_FR_XMITOVERHEAD_PathBuffer_dft      0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  9 ;
    cs_uint16 Select               :  2 ; /* bits 6:5 */
    cs_uint16 rsrvd1               :  5 ;
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 rsrvd1               :  5 ;
    cs_uint16 Select               :  2 ; /* bits 6:5 */
    cs_uint16 rsrvd2               :  9 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_PathBuffer_t;

#define ARN5_FR_XMITOVERHEAD_SectionTrace        0x1e740
#define ARN5_FR_XMITOVERHEAD_SectionTrace_STRIDE 1
#define ARN5_FR_XMITOVERHEAD_SectionTrace_COUNT  32
#define ARN5_FR_XMITOVERHEAD_SectionTrace_dft    0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 HighByte             :  8 ; /* bits 15:8 */
    cs_uint16 LowByte              :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LowByte              :  8 ; /* bits 7:0 */
    cs_uint16 HighByte             :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_SectionTrace_t;

#define ARN5_FR_XMITOVERHEAD_PathTrace           0x1e760
#define ARN5_FR_XMITOVERHEAD_PathTrace_STRIDE    1
#define ARN5_FR_XMITOVERHEAD_PathTrace_COUNT     32
#define ARN5_FR_XMITOVERHEAD_PathTrace_dft       0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 HighByte             :  8 ; /* bits 15:8 */
    cs_uint16 LowByte              :  8 ; /* bits 7:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 LowByte              :  8 ; /* bits 7:0 */
    cs_uint16 HighByte             :  8 ; /* bits 15:8 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_XMITOVERHEAD_PathTrace_t;


/******************************************************************
 *
 * Each register in the map is defined as a struct with bitfields.
 *
 ******************************************************************/


/*
 * Framer::txHDLC[0..1]
 */

#define ARN5_FR_TXHDLC_STRIDE                    32
#define ARN5_FR_TXHDLC_COUNT                     2
#define ARN5_FR_TXHDLC_Ctrl                      0x1e780
#define ARN5_FR_TXHDLC_Ctrl_dft                  0x000e
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 10 ;
    cs_uint16 FillFlags            :  1 ; /* bits 5:5 */
    cs_uint16 passFCS              :  1 ; /* bits 4:4 */
    cs_uint16 dataMSBfirst         :  1 ; /* bits 3:3 */
    cs_uint16 CRCMSBfirst          :  1 ; /* bits 2:2 */
    cs_uint16 CRC32                :  1 ; /* bits 1:1 */
    cs_uint16 reset                :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 reset                :  1 ; /* bits 0:0 */
    cs_uint16 CRC32                :  1 ; /* bits 1:1 */
    cs_uint16 CRCMSBfirst          :  1 ; /* bits 2:2 */
    cs_uint16 dataMSBfirst         :  1 ; /* bits 3:3 */
    cs_uint16 passFCS              :  1 ; /* bits 4:4 */
    cs_uint16 FillFlags            :  1 ; /* bits 5:5 */
    cs_uint16 rsrvd1               : 10 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TXHDLC_Ctrl_t;

#define ARN5_FR_TXHDLC_Bank0Status               0x1e781
#define ARN5_FR_TXHDLC_Bank0Status_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SOP                  :  1 ; /* bits 15:15 */
    cs_uint16 EOP                  :  1 ; /* bits 14:14 */
    cs_uint16 EOPErr               :  1 ; /* bits 13:13 */
    cs_uint16 MOP                  :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 PktVld               :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 EOPPtr               :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 EOPPtr               :  6 ; /* bits 5:0 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 PktVld               :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 MOP                  :  1 ; /* bits 12:12 */
    cs_uint16 EOPErr               :  1 ; /* bits 13:13 */
    cs_uint16 EOP                  :  1 ; /* bits 14:14 */
    cs_uint16 SOP                  :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TXHDLC_Bank0Status_t;

#define ARN5_FR_TXHDLC_Bank1Status               0x1e782
#define ARN5_FR_TXHDLC_Bank1Status_dft           0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 SOP                  :  1 ; /* bits 15:15 */
    cs_uint16 EOP                  :  1 ; /* bits 14:14 */
    cs_uint16 EOPErr               :  1 ; /* bits 13:13 */
    cs_uint16 MOP                  :  1 ; /* bits 12:12 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 PktVld               :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 EOPPtr               :  6 ; /* bits 5:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 EOPPtr               :  6 ; /* bits 5:0 */
    cs_uint16 rsrvd1               :  2 ;
    cs_uint16 PktVld               :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd2               :  3 ;
    cs_uint16 MOP                  :  1 ; /* bits 12:12 */
    cs_uint16 EOPErr               :  1 ; /* bits 13:13 */
    cs_uint16 EOP                  :  1 ; /* bits 14:14 */
    cs_uint16 SOP                  :  1 ; /* bits 15:15 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TXHDLC_Bank1Status_t;

#define ARN5_FR_TXHDLC_Interrupt                 0x1e783
#define ARN5_FR_TXHDLC_Interrupt_dft             0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 FIFOOverflowI        :  1 ; /* bits 3:3 */
    cs_uint16 FIFOUnderflowI       :  1 ; /* bits 2:2 */
    cs_uint16 FIFODone1I           :  1 ; /* bits 1:1 */
    cs_uint16 FIFODone0I           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FIFODone0I           :  1 ; /* bits 0:0 */
    cs_uint16 FIFODone1I           :  1 ; /* bits 1:1 */
    cs_uint16 FIFOUnderflowI       :  1 ; /* bits 2:2 */
    cs_uint16 FIFOOverflowI        :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TXHDLC_Interrupt_t;

#define ARN5_FR_TXHDLC_Mask                      0x1e784
#define ARN5_FR_TXHDLC_Mask_dft                  0x000f
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 12 ;
    cs_uint16 FIFOOverflowM        :  1 ; /* bits 3:3 */
    cs_uint16 FIFOUnderflowM       :  1 ; /* bits 2:2 */
    cs_uint16 FIFODone1M           :  1 ; /* bits 1:1 */
    cs_uint16 FIFODone0M           :  1 ; /* bits 0:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 FIFODone0M           :  1 ; /* bits 0:0 */
    cs_uint16 FIFODone1M           :  1 ; /* bits 1:1 */
    cs_uint16 FIFOUnderflowM       :  1 ; /* bits 2:2 */
    cs_uint16 FIFOOverflowM        :  1 ; /* bits 3:3 */
    cs_uint16 rsrvd1               : 12 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TXHDLC_Mask_t;

#define ARN5_FR_TXHDLC_Bank0Write                0x1e785
#define ARN5_FR_TXHDLC_Bank0Write_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Data                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Data                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TXHDLC_Bank0Write_t;

#define ARN5_FR_TXHDLC_Bank1Write                0x1e786
#define ARN5_FR_TXHDLC_Bank1Write_dft            0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Data                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Data                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TXHDLC_Bank1Write_t;

#define ARN5_FR_TXHDLC_FIFOTestData              0x1e787
#define ARN5_FR_TXHDLC_FIFOTestData_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 Word                 : 16 ; /* bits 15:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Word                 : 16 ; /* bits 15:0 */
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TXHDLC_FIFOTestData_t;

#define ARN5_FR_TXHDLC_FIFOTestCtrl              0x1e788
#define ARN5_FR_TXHDLC_FIFOTestCtrl_dft          0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd2               :  6 ;
    cs_uint16 Ram1Enable           :  1 ; /* bits 9:9 */
    cs_uint16 Ram0Enable           :  1 ; /* bits 8:8 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 ReadAddr             :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 ReadAddr             :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               :  3 ;
    cs_uint16 Ram0Enable           :  1 ; /* bits 8:8 */
    cs_uint16 Ram1Enable           :  1 ; /* bits 9:9 */
    cs_uint16 rsrvd2               :  6 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TXHDLC_FIFOTestCtrl_t;

#define ARN5_FR_TXHDLC_TestCtrl                  0x1e789
#define ARN5_FR_TXHDLC_TestCtrl_dft              0x0000
typedef volatile union {
  struct {
#   ifdef CS_BIG_ENDIAN 
    cs_uint16 rsrvd1               : 11 ;
    cs_uint16 Select               :  5 ; /* bits 4:0 */
#   else /* CS_LITTLE_ENDIAN */
    cs_uint16 Select               :  5 ; /* bits 4:0 */
    cs_uint16 rsrvd1               : 11 ;
#   endif
  } bf ;
  cs_uint16     wrd ;
} ARN5_FR_TXHDLC_TestCtrl_t;
#define     ARN5_Select_SEL_STS0                          0x00
#define     ARN5_Select_SEL_STS1                          0x01
#define     ARN5_Select_SEL_RDDATA0                       0x02
#define     ARN5_Select_SEL_RDDATA1                       0x03
#define     ARN5_Select_SEL_RDDATA2                       0x04
#define     ARN5_Select_SEL_RDDATA3                       0x05
#define     ARN5_Select_SEL_BIT_STS                       0x06


/******************************************************************
 *
 * Block declaration.
 *
 ******************************************************************/

/* Framer */

#define ARN5_FRAMER_STRIDE                       131072
#define ARN5_FRAMER_COUNT                        8
typedef struct {
  ARN5_FR_GLOBAL_JTAGIdLsb_t               JTAGIdLsb;                     /* addr: 0x0000 */
  ARN5_FR_GLOBAL_JTAGIdMsb_t               JTAGIdMsb;                     /* addr: 0x0001 */
  cs_uint16                                rsrvd1[2];
  ARN5_FR_GLOBAL_MasterReset_t             MasterReset;                   /* addr: 0x0004 */
  cs_uint16                                rsrvd2[11];
  ARN5_FR_GLOBAL_MasterInt_t               MasterInt;                     /* addr: 0x0010 */
  cs_uint16                                rsrvd3[3];
  ARN5_FR_GLOBAL_MasterMask_t              MasterMask;                    /* addr: 0x0014 */
  ARN5_FR_GLOBAL_MasterInt2_t              MasterInt2;                    /* addr: 0x0015 */
  ARN5_FR_GLOBAL_MasterMask2_t             MasterMask2;                   /* addr: 0x0016 */
  cs_uint16                                rsrvd4[9];
  ARN5_FR_GLOBAL_ClockStatus_t             ClockStatus;                   /* addr: 0x0020 */
  ARN5_FR_GLOBAL_ClockInt_t                ClockInt;                      /* addr: 0x0021 */
  ARN5_FR_GLOBAL_ClockMask_t               ClockMask;                     /* addr: 0x0022 */
  cs_uint16                                rsrvd5[5];
  ARN5_FR_GLOBAL_CellRxInt_t               CellRxInt;                     /* addr: 0x0028 */
  ARN5_FR_GLOBAL_CellRxMask_t              CellRxMask;                    /* addr: 0x0029 */
  ARN5_FR_GLOBAL_PacketRxInt_t             PacketRxInt;                   /* addr: 0x002a */
  ARN5_FR_GLOBAL_PacketRxMask_t            PacketRxMask;                  /* addr: 0x002b */
  ARN5_FR_GLOBAL_CellTxInt_t               CellTxInt;                     /* addr: 0x002c */
  ARN5_FR_GLOBAL_CellTxMask_t              CellTxMask;                    /* addr: 0x002d */
  ARN5_FR_GLOBAL_PacketTxInt_t             PacketTxInt;                   /* addr: 0x002e */
  ARN5_FR_GLOBAL_PacketTxMask_t            PacketTxMask;                  /* addr: 0x002f */
  cs_uint16                                rsrvd6[4];
  ARN5_FR_GLOBAL_LoopBack_t                LoopBack;                      /* addr: 0x0034 */
  ARN5_FR_GLOBAL_DebugPort_t               DebugPort;                     /* addr: 0x0035 */
  ARN5_FR_GLOBAL_Rx_Carrier_Msk_t          Rx_Carrier_Msk;                /* addr: 0x0036 */
  cs_uint16                                rsrvd7;
  ARN5_FR_GLOBAL_LineSkewControl_t         LineSkewControl;               /* addr: 0x0038 */
  ARN5_FR_GLOBAL_DatacomSkewControl_t      DatacomSkewControl;            /* addr: 0x0039 */
  cs_uint16                                rsrvd8[454];
  ARN5_FR_DATACOMRX_Control_t              datacomRx_Control;             /* addr: 0x0200 */
  ARN5_FR_DATACOMRX_Interrupt_t            datacomRx_Interrupt;           /* addr: 0x0201 */
  ARN5_FR_DATACOMRX_Mask_t                 datacomRx_Mask;                /* addr: 0x0202 */
  cs_uint16                                rsrvd9[61];
  ARN5_FR_DATACOMRX_StrConfig_t            datacomRx_StrConfig;           /* addr: 0x0240 */
  ARN5_FR_DATACOMRX_StrInt_t               datacomRx_StrInt;              /* addr: 0x0241 */
  ARN5_FR_DATACOMRX_StrMask_t              datacomRx_StrMask;             /* addr: 0x0242 */
  ARN5_FR_DATACOMRX_StrStatus_t            datacomRx_StrStatus;           /* addr: 0x0243 */
  cs_uint16                                rsrvd10[1468];
  ARN5_FR_STREAMRX_CellControl_t           streamRx_CellControl;          /* addr: 0x0800 */
  ARN5_FR_STREAMRX_CellPattern_t           streamRx_CellPattern;          /* addr: 0x0801 */
  ARN5_FR_STREAMRX_CellIntr_t              streamRx_CellIntr;             /* addr: 0x0802 */
  ARN5_FR_STREAMRX_CellMask_t              CellMask;                      /* addr: 0x0803 */
  ARN5_FR_STREAMRX_CellSts_t               CellSts;                       /* addr: 0x0804 */
  ARN5_FR_STREAMRX_Cellfsm_t               Cellfsm;                       /* addr: 0x0805 */
  cs_uint16                                rsrvd11[16];
  ARN5_FR_STREAMRX_PktControl_t            streamRx_PktControl;           /* addr: 0x0816 */
  ARN5_FR_STREAMRX_MaxLengthMSB_t          MaxLengthMSB;                  /* addr: 0x0817 */
  ARN5_FR_STREAMRX_MaxLengthLSB_t          MaxLengthLSB;                  /* addr: 0x0818 */
  ARN5_FR_STREAMRX_MinLength_t             MinLength;                     /* addr: 0x0819 */
  ARN5_FR_STREAMRX_PktFillControl_t        PktFillControl;                /* addr: 0x081a */
  ARN5_FR_STREAMRX_PktIntr_t               PktIntr;                       /* addr: 0x081b */
  ARN5_FR_STREAMRX_PktStatus_t             PktStatus;                     /* addr: 0x081c */
  ARN5_FR_STREAMRX_PktMask_t               streamRx_PktMask;              /* addr: 0x081d */
  ARN5_FR_STREAMRX_PktExAddrCtrl_t         PktExAddrCtrl;                 /* addr: 0x081e */
  ARN5_FR_STREAMRX_PktX86Ctrl_t            PktX86Ctrl;                    /* addr: 0x081f */
  ARN5_FR_STREAMRX_Cnt1Q1_t                streamRx_Cnt1Q1;               /* addr: 0x0820 */
  ARN5_FR_STREAMRX_Cnt1Q2_t                streamRx_Cnt1Q2;               /* addr: 0x0821 */
  ARN5_FR_STREAMRX_Cnt1Q3_t                streamRx_Cnt1Q3;               /* addr: 0x0822 */
  cs_uint16                                rsrvd12;
  ARN5_FR_STREAMRX_Cnt2Q1_t                streamRx_Cnt2Q1;               /* addr: 0x0824 */
  ARN5_FR_STREAMRX_Cnt2Q2_t                streamRx_Cnt2Q2;               /* addr: 0x0825 */
  ARN5_FR_STREAMRX_Cnt2Q3_t                streamRx_Cnt2Q3;               /* addr: 0x0826 */
  cs_uint16                                rsrvd13;
  ARN5_FR_STREAMRX_Cnt3Q1_t                streamRx_Cnt3Q1;               /* addr: 0x0828 */
  ARN5_FR_STREAMRX_Cnt3Q2_t                streamRx_Cnt3Q2;               /* addr: 0x0829 */
  ARN5_FR_STREAMRX_Cnt3Q3_t                streamRx_Cnt3Q3;               /* addr: 0x082a */
  cs_uint16                                rsrvd14;
  ARN5_FR_STREAMRX_Cnt4Q1_t                streamRx_Cnt4Q1;               /* addr: 0x082c */
  ARN5_FR_STREAMRX_Cnt4Q2_t                streamRx_Cnt4Q2;               /* addr: 0x082d */
  ARN5_FR_STREAMRX_Cnt4Q3_t                streamRx_Cnt4Q3;               /* addr: 0x082e */
  cs_uint16                                rsrvd15;
  ARN5_FR_STREAMRX_Cnt5Q1_t                streamRx_Cnt5Q1;               /* addr: 0x0830 */
  ARN5_FR_STREAMRX_Cnt5Q2_t                streamRx_Cnt5Q2;               /* addr: 0x0831 */
  ARN5_FR_STREAMRX_Cnt5Q3_t                streamRx_Cnt5Q3;               /* addr: 0x0832 */
  cs_uint16                                rsrvd16;
  ARN5_FR_STREAMRX_Cnt6Q1_t                streamRx_Cnt6Q1;               /* addr: 0x0834 */
  ARN5_FR_STREAMRX_Cnt6Q2_t                streamRx_Cnt6Q2;               /* addr: 0x0835 */
  ARN5_FR_STREAMRX_Cnt6Q3_t                streamRx_Cnt6Q3;               /* addr: 0x0836 */
  cs_uint16                                rsrvd17;
  ARN5_FR_STREAMRX_Cnt7Q1_t                streamRx_Cnt7Q1;               /* addr: 0x0838 */
  ARN5_FR_STREAMRX_Cnt7Q2_t                streamRx_Cnt7Q2;               /* addr: 0x0839 */
  ARN5_FR_STREAMRX_Cnt7Q3_t                streamRx_Cnt7Q3;               /* addr: 0x083a */
  cs_uint16                                rsrvd18;
  ARN5_FR_STREAMRX_Cnt8Q1_t                streamRx_Cnt8Q1;               /* addr: 0x083c */
  ARN5_FR_STREAMRX_Cnt8Q2_t                streamRx_Cnt8Q2;               /* addr: 0x083d */
  ARN5_FR_STREAMRX_Cnt8Q3_t                streamRx_Cnt8Q3;               /* addr: 0x083e */
  cs_uint16                                rsrvd19;
  ARN5_FR_STREAMRX_Cnt9Q1_t                Cnt9Q1;                        /* addr: 0x0840 */
  ARN5_FR_STREAMRX_Cnt9Q2_t                Cnt9Q2;                        /* addr: 0x0841 */
  ARN5_FR_STREAMRX_Cnt9Q3_t                Cnt9Q3;                        /* addr: 0x0842 */
  cs_uint16                                rsrvd20;
  ARN5_FR_STREAMRX_Cnt10Q1_t               Cnt10Q1;                       /* addr: 0x0844 */
  ARN5_FR_STREAMRX_Cnt10Q2_t               Cnt10Q2;                       /* addr: 0x0845 */
  ARN5_FR_STREAMRX_Cnt10Q3_t               Cnt10Q3;                       /* addr: 0x0846 */
  cs_uint16                                rsrvd21;
  ARN5_FR_STREAMRX_debugSelect_t           streamRx_debugSelect;          /* addr: 0x0848 */
  cs_uint16                                rsrvd22[3];
  ARN5_FR_STREAMRX_debugPktControl_t       debugPktControl;               /* addr: 0x084c */
  ARN5_FR_STREAMRX_debugPktStatus_t        debugPktStatus;                /* addr: 0x084d */
  ARN5_FR_STREAMRX_debugPktAddrCtrl_t      debugPktAddrCtrl;              /* addr: 0x084e */
  ARN5_FR_STREAMRX_debugPktSAPI_t          debugPktSAPI;                  /* addr: 0x084f */
  ARN5_FR_GFP_RX_GFP_Control_0_t           gfp_rx_GFP_Control_0;          /* addr: 0x0850 */
  ARN5_FR_GFP_RX_GFP_Control_1_t           GFP_Control_1;                 /* addr: 0x0851 */
  ARN5_FR_GFP_RX_GFP_Control_2_t           GFP_Control_2;                 /* addr: 0x0852 */
  cs_uint16                                rsrvd23;
  ARN5_FR_GFP_RX_GFP_Intr_t                gfp_rx_GFP_Intr;               /* addr: 0x0854 */
  ARN5_FR_GFP_RX_GFP_Mask_t                gfp_rx_GFP_Mask;               /* addr: 0x0855 */
  ARN5_FR_GFP_RX_GFP_Status_t              gfp_rx_GFP_Status;             /* addr: 0x0856 */
  ARN5_FR_STREAMRX_Cnt11Q1_t               Cnt11Q1;                       /* addr: 0x0857 */
  ARN5_FR_STREAMRX_Cnt11Q2_t               Cnt11Q2;                       /* addr: 0x0858 */
  ARN5_FR_STREAMRX_Cnt11Q3_t               Cnt11Q3;                       /* addr: 0x0859 */
  cs_uint16                                rsrvd24[6566];
  ARN5_FR_DATACOMTX_Control_t              datacomTx_Control;             /* addr: 0x2200 */
  ARN5_FR_DATACOMTX_Interrupt_t            datacomTx_Interrupt;           /* addr: 0x2201 */
  ARN5_FR_DATACOMTX_Mask_t                 datacomTx_Mask;                /* addr: 0x2202 */
  cs_uint16                                rsrvd25[5];
  ARN5_FR_DATACOMTX_Active_Override_t      Active_Override;               /* addr: 0x2208 */
  cs_uint16                                rsrvd26[55];
  ARN5_FR_DATACOMTX_StrConfig_t            datacomTx_StrConfig;           /* addr: 0x2240 */
  ARN5_FR_DATACOMTX_StrInt_t               datacomTx_StrInt;              /* addr: 0x2241 */
  ARN5_FR_DATACOMTX_StrMask_t              datacomTx_StrMask;             /* addr: 0x2242 */
  ARN5_FR_DATACOMTX_StrStatus_t            datacomTx_StrStatus;           /* addr: 0x2243 */
  cs_uint16                                rsrvd27[1468];
  ARN5_FR_STREAMTX_CellControl_t           streamTx_CellControl;          /* addr: 0x2800 */
  ARN5_FR_STREAMTX_CellPattern_t           streamTx_CellPattern;          /* addr: 0x2801 */
  ARN5_FR_STREAMTX_CellIntr_t              streamTx_CellIntr;             /* addr: 0x2802 */
  ARN5_FR_STREAMTX_CellIMask_t             CellIMask;                     /* addr: 0x2803 */
  cs_uint16                                rsrvd28[4];
  ARN5_FR_GFP_TX_GFP_Control_0_t           gfp_tx_GFP_Control_0;          /* addr: 0x2808 */
  ARN5_FR_GFP_TX_GFP_Payload_Hdr_t         GFP_Payload_Hdr;               /* addr: 0x2809 */
  ARN5_FR_GFP_TX_GFP_Intr_t                gfp_tx_GFP_Intr;               /* addr: 0x280a */
  ARN5_FR_GFP_TX_GFP_Mask_t                gfp_tx_GFP_Mask;               /* addr: 0x280b */
  ARN5_FR_GFP_TX_GFP_Status_t              gfp_tx_GFP_Status;             /* addr: 0x280c */
  ARN5_FR_GFP_TX_GFP_Diagnostics_t         GFP_Diagnostics;               /* addr: 0x280d */
  cs_uint16                                rsrvd29[2];
  ARN5_FR_STREAMTX_PktControl_t            streamTx_PktControl;           /* addr: 0x2810 */
  ARN5_FR_STREAMTX_FillerByte_t            FillerByte;                    /* addr: 0x2811 */
  ARN5_FR_STREAMTX_MaxPktMSB_t             MaxPktMSB;                     /* addr: 0x2812 */
  ARN5_FR_STREAMTX_MaxPktLSB_t             MaxPktLSB;                     /* addr: 0x2813 */
  ARN5_FR_STREAMTX_MinPkt_t                MinPkt;                        /* addr: 0x2814 */
  ARN5_FR_STREAMTX_Threshold_t             Threshold;                     /* addr: 0x2815 */
  ARN5_FR_STREAMTX_NumFlags_t              NumFlags;                      /* addr: 0x2816 */
  ARN5_FR_STREAMTX_PktSts_t                PktSts;                        /* addr: 0x2817 */
  ARN5_FR_STREAMTX_PktInt_t                PktInt;                        /* addr: 0x2818 */
  ARN5_FR_STREAMTX_PktMask_t               streamTx_PktMask;              /* addr: 0x2819 */
  ARN5_FR_STREAMTX_PktAddrCtrl_t           PktAddrCtrl;                   /* addr: 0x281a */
  ARN5_FR_STREAMTX_PktSAPI_t               PktSAPI;                       /* addr: 0x281b */
  cs_uint16                                rsrvd30[4];
  ARN5_FR_STREAMTX_Cnt1Q1_t                streamTx_Cnt1Q1;               /* addr: 0x2820 */
  ARN5_FR_STREAMTX_Cnt1Q2_t                streamTx_Cnt1Q2;               /* addr: 0x2821 */
  ARN5_FR_STREAMTX_Cnt1Q3_t                streamTx_Cnt1Q3;               /* addr: 0x2822 */
  cs_uint16                                rsrvd31;
  ARN5_FR_STREAMTX_Cnt2Q1_t                streamTx_Cnt2Q1;               /* addr: 0x2824 */
  ARN5_FR_STREAMTX_Cnt2Q2_t                streamTx_Cnt2Q2;               /* addr: 0x2825 */
  ARN5_FR_STREAMTX_Cnt2Q3_t                streamTx_Cnt2Q3;               /* addr: 0x2826 */
  cs_uint16                                rsrvd32;
  ARN5_FR_STREAMTX_Cnt3Q1_t                streamTx_Cnt3Q1;               /* addr: 0x2828 */
  ARN5_FR_STREAMTX_Cnt3Q2_t                streamTx_Cnt3Q2;               /* addr: 0x2829 */
  ARN5_FR_STREAMTX_Cnt3Q3_t                streamTx_Cnt3Q3;               /* addr: 0x282a */
  cs_uint16                                rsrvd33;
  ARN5_FR_STREAMTX_Cnt4Q1_t                streamTx_Cnt4Q1;               /* addr: 0x282c */
  ARN5_FR_STREAMTX_Cnt4Q2_t                streamTx_Cnt4Q2;               /* addr: 0x282d */
  ARN5_FR_STREAMTX_Cnt4Q3_t                streamTx_Cnt4Q3;               /* addr: 0x282e */
  cs_uint16                                rsrvd34;
  ARN5_FR_STREAMTX_Cnt5Q1_t                streamTx_Cnt5Q1;               /* addr: 0x2830 */
  ARN5_FR_STREAMTX_Cnt5Q2_t                streamTx_Cnt5Q2;               /* addr: 0x2831 */
  ARN5_FR_STREAMTX_Cnt5Q3_t                streamTx_Cnt5Q3;               /* addr: 0x2832 */
  cs_uint16                                rsrvd35;
  ARN5_FR_STREAMTX_Cnt6Q1_t                streamTx_Cnt6Q1;               /* addr: 0x2834 */
  ARN5_FR_STREAMTX_Cnt6Q2_t                streamTx_Cnt6Q2;               /* addr: 0x2835 */
  ARN5_FR_STREAMTX_Cnt6Q3_t                streamTx_Cnt6Q3;               /* addr: 0x2836 */
  cs_uint16                                rsrvd36;
  ARN5_FR_STREAMTX_Cnt7Q1_t                streamTx_Cnt7Q1;               /* addr: 0x2838 */
  ARN5_FR_STREAMTX_Cnt7Q2_t                streamTx_Cnt7Q2;               /* addr: 0x2839 */
  ARN5_FR_STREAMTX_Cnt7Q3_t                streamTx_Cnt7Q3;               /* addr: 0x283a */
  cs_uint16                                rsrvd37;
  ARN5_FR_STREAMTX_debugSelect_t           streamTx_debugSelect;          /* addr: 0x283c */
  cs_uint16                                rsrvd38[3];
  ARN5_FR_GFP_TX_Cnt8Q1_t                  gfp_tx_Cnt8Q1;                 /* addr: 0x2840 */
  ARN5_FR_GFP_TX_Cnt8Q2_t                  gfp_tx_Cnt8Q2;                 /* addr: 0x2841 */
  ARN5_FR_GFP_TX_Cnt8Q3_t                  gfp_tx_Cnt8Q3;                 /* addr: 0x2842 */
  cs_uint16                                rsrvd39[29];
  ARN5_FR_GFP_TX_GFP_Tx_CoreHeaderMSB_t    GFP_Tx_CoreHeaderMSB;          /* addr: 0x2860 */
  ARN5_FR_GFP_TX_GFP_Tx_CoreHeaderErrLSB_t GFP_Tx_CoreHeaderErrLSB;       /* addr: 0x2861 */
  ARN5_FR_GFP_TX_GFP_Tx_PayloadHeaderErrMSB_t GFP_Tx_PayloadHeaderErrMSB;    /* addr: 0x2862 */
  ARN5_FR_GFP_TX_GFP_Tx_PldHeaderErrLSB_t  GFP_Tx_PldHeaderErrLSB;        /* addr: 0x2863 */
  cs_uint16                                rsrvd40[6044];
  ARN5_FR_SONET32RX_PathProvision_t        PathProvision;                 /* addr: 0x4000 */
  ARN5_FR_SONET32RX_FilteredG1C2_t         FilteredG1C2;                  /* addr: 0x4001 */
  ARN5_FR_SONET32RX_PRDITxInsert_t         PRDITxInsert;                  /* addr: 0x4002 */
  ARN5_FR_SONET32RX_PathLabel_t            PathLabel;                     /* addr: 0x4003 */
  ARN5_FR_SONET32RX_APSPathTxInsert_t      APSPathTxInsert;               /* addr: 0x4004 */
  ARN5_FR_SONET32RX_APSPathB3TxInsert_t    APSPathB3TxInsert;             /* addr: 0x4005 */
  ARN5_FR_SONET32RX_PathPointer_t          PathPointer;                   /* addr: 0x4006 */
  ARN5_FR_SONET32RX_PathBIPCount_t         PathBIPCount;                  /* addr: 0x4007 */
  ARN5_FR_SONET32RX_PathREICount_t         PathREICount;                  /* addr: 0x4008 */
  ARN5_FR_SONET32RX_PathPositiveCount_t    PathPositiveCount;             /* addr: 0x4009 */
  ARN5_FR_SONET32RX_PathNegativeCount_t    PathNegativeCount;             /* addr: 0x400a */
  ARN5_FR_SONET32RX_PathNewPtrCount_t      PathNewPtrCount;               /* addr: 0x400b */
  ARN5_FR_SONET32RX_PathInt_t              PathInt;                       /* addr: 0x400c */
  ARN5_FR_SONET32RX_PathMask_t             PathMask;                      /* addr: 0x400d */
  ARN5_FR_SONET32RX_PathStatus_t           PathStatus;                    /* addr: 0x400e */
  cs_uint16                                rsrvd41[753];
  ARN5_FR_SONET32RX_Configuration_t        sonet32Rx_Configuration;       /* addr: 0x4300 */
  ARN5_FR_SONET32RX_LOSThreshold_t         LOSThreshold;                  /* addr: 0x4301 */
  ARN5_FR_SONET32RX_SectionLineControl_t   SectionLineControl;            /* addr: 0x4302 */
  ARN5_FR_SONET32RX_SecLineAlarmtoTxControl_t SecLineAlarmtoTxControl;       /* addr: 0x4303 */
  ARN5_FR_SONET32RX_APSTxSecLineAlarm_t    APSTxSecLineAlarm;             /* addr: 0x4304 */
  ARN5_FR_SONET32RX_APSTxLineB2Count_t     APSTxLineB2Count;              /* addr: 0x4305 */
  ARN5_FR_SONET32RX_SectionInterrupt_t     SectionInterrupt;              /* addr: 0x4306 */
  ARN5_FR_SONET32RX_SectionStatus_t        SectionStatus;                 /* addr: 0x4307 */
  ARN5_FR_SONET32RX_SectionMask_t          SectionMask;                   /* addr: 0x4308 */
  ARN5_FR_SONET32RX_B1BIPErrorCount_t      B1BIPErrorCount;               /* addr: 0x4309 */
  ARN5_FR_SONET32RX_LineInterrupt_t        LineInterrupt;                 /* addr: 0x430a */
  ARN5_FR_SONET32RX_LineStatus_t           LineStatus;                    /* addr: 0x430b */
  ARN5_FR_SONET32RX_LineMask_t             LineMask;                      /* addr: 0x430c */
  ARN5_FR_SONET32RX_APSInterrupt_t         APSInterrupt;                  /* addr: 0x430d */
  ARN5_FR_SONET32RX_APSMask_t              APSMask;                       /* addr: 0x430e */
  ARN5_FR_SONET32RX_K1K2_t                 K1K2;                          /* addr: 0x430f */
  ARN5_FR_SONET32RX_K3K4_t                 K3K4;                          /* addr: 0x4310 */
  ARN5_FR_SONET32RX_Z1S1_t                 Z1S1;                          /* addr: 0x4311 */
  ARN5_FR_SONET32RX_LBERAcc_t              LBERAcc;                       /* addr: 0x4312 */
  ARN5_FR_SONET32RX_LBER_t                 LBER;                          /* addr: 0x4313 */
  ARN5_FR_SONET32RX_LBERDegradeDeclaring_t LBERDegradeDeclaring;          /* addr: 0x4314 */
  ARN5_FR_SONET32RX_LBERDegradeClearing_t  LBERDegradeClearing;           /* addr: 0x4315 */
  ARN5_FR_SONET32RX_LBERFailureDeclaring_t LBERFailureDeclaring;          /* addr: 0x4316 */
  ARN5_FR_SONET32RX_LBERFailureClearing_t  LBERFailureClearing;           /* addr: 0x4317 */
  ARN5_FR_SONET32RX_B2BIPErrorCountHigh_t  B2BIPErrorCountHigh;           /* addr: 0x4318 */
  ARN5_FR_SONET32RX_B2BIPErrorCountLow_t   B2BIPErrorCountLow;            /* addr: 0x4319 */
  ARN5_FR_SONET32RX_LineREICountHigh_t     LineREICountHigh;              /* addr: 0x431a */
  ARN5_FR_SONET32RX_LineREICountLow_t      LineREICountLow;               /* addr: 0x431b */
  ARN5_FR_SONET32RX_TestPortSelect_t       TestPortSelect;                /* addr: 0x431c */
  cs_uint16                                rsrvd42;
  ARN5_FR_SONET32RX_Z0_byte_t              sonet32Rx_Z0_byte;             /* addr: 0x431e */
  cs_uint16                                rsrvd43;
  ARN5_FR_SONET32RX_PBERAcc_t              PBERAcc;                       /* addr: 0x4320 */
  ARN5_FR_SONET32RX_PBER_t                 PBER;                          /* addr: 0x4321 */
  ARN5_FR_SONET32RX_PBERFailureDeclaring_t PBERFailureDeclaring;          /* addr: 0x4322 */
  ARN5_FR_SONET32RX_PBERFailureClearing_t  PBERFailureClearing;           /* addr: 0x4323 */
  ARN5_FR_SONET32RX_PathInterrupts_t       PathInterrupts;                /* addr: 0x4324 */
  ARN5_FR_SONET32RX_PathMasks_t            PathMasks;                     /* addr: 0x4325 */
  cs_uint16                                rsrvd44[2];
  ARN5_FR_SONET32RX_pathTraceSelect_t      pathTraceSelect;               /* addr: 0x4328 */
  ARN5_FR_SONET32RX_F2_t                   F2;                            /* addr: 0x4329 */
  ARN5_FR_SONET32RX_X_LOS_Config_t         X_LOS_Config;                  /* addr: 0x432a */
  cs_uint16                                rsrvd45[5];
  ARN5_FR_SONET32RX_PRBSErrorCountHigh_t   PRBSErrorCountHigh;            /* addr: 0x4330 */
  ARN5_FR_SONET32RX_PRBSErrorCountLow_t    PRBSErrorCountLow;             /* addr: 0x4331 */
  ARN5_FR_SONET32RX_PRBS_Rx_Provision_t    PRBS_Rx_Provision;             /* addr: 0x4332 */
  cs_uint16                                rsrvd46[13];
  ARN5_FR_SONET32RX_RxSectionTraceExp_t    RxSectionTraceExp;             /* addr: 0x4340 */
  cs_uint16                                rsrvd47[31];
  ARN5_FR_SONET32RX_RxPathTraceExp_t       RxPathTraceExp;                /* addr: 0x4360 */
  cs_uint16                                rsrvd48[161];
  ARN5_FR_RECVOVERHEAD_pathOverheadA_t     pathOverheadA;                 /* addr: 0x4402 */
  ARN5_FR_RECVOVERHEAD_pathOverheadB_t     pathOverheadB;                 /* addr: 0x4403 */
  ARN5_FR_RECVOVERHEAD_pathOverheadC_t     pathOverheadC;                 /* addr: 0x4404 */
  cs_uint16                                rsrvd49[763];
  ARN5_FR_RECVOVERHEAD_Ctrl_t              recvOverhead_Ctrl;             /* addr: 0x4700 */
  cs_uint16                                rsrvd50[5];
  ARN5_FR_RECVOVERHEAD_SectRow2_t          SectRow2;                      /* addr: 0x4706 */
  ARN5_FR_RECVOVERHEAD_LineRow5a_t         LineRow5a;                     /* addr: 0x4707 */
  ARN5_FR_RECVOVERHEAD_LineRow5b_t         LineRow5b;                     /* addr: 0x4708 */
  ARN5_FR_RECVOVERHEAD_LineRow9_t          LineRow9;                      /* addr: 0x4709 */
  ARN5_FR_RECVOVERHEAD_TestCtrl_t          recvOverhead_TestCtrl;         /* addr: 0x470a */
  cs_uint16                                rsrvd51[117];
  ARN5_FR_RXHDLC_Ctrl_t                    rxHDLC_Ctrl;                   /* addr: 0x4780 */
  ARN5_FR_RXHDLC_Bank0Status_t             rxHDLC_Bank0Status;            /* addr: 0x4781 */
  ARN5_FR_RXHDLC_Bank1Status_t             rxHDLC_Bank1Status;            /* addr: 0x4782 */
  ARN5_FR_RXHDLC_Interrupt_t               rxHDLC_Interrupt;              /* addr: 0x4783 */
  ARN5_FR_RXHDLC_Mask_t                    rxHDLC_Mask;                   /* addr: 0x4784 */
  ARN5_FR_RXHDLC_Bank0Read_t               Bank0Read;                     /* addr: 0x4785 */
  ARN5_FR_RXHDLC_Bank1Read_t               Bank1Read;                     /* addr: 0x4786 */
  ARN5_FR_RXHDLC_FIFOTestData_t            rxHDLC_FIFOTestData;           /* addr: 0x4787 */
  ARN5_FR_RXHDLC_FIFOTestCtrl_t            rxHDLC_FIFOTestCtrl;           /* addr: 0x4788 */
  ARN5_FR_RXHDLC_FIFOTestEOPPtr_t          FIFOTestEOPPtr;                /* addr: 0x4789 */
  cs_uint16                                rsrvd52;
  ARN5_FR_RXHDLC_Status_t                  Status;                        /* addr: 0x478b */
  ARN5_FR_RXHDLC_TestCtrl_t                rxHDLC_TestCtrl;               /* addr: 0x478c */
  cs_uint16                                rsrvd53[6259];
  ARN5_FR_TRANSMIT_PathSPEPointerControl_t PathSPEPointerControl;         /* addr: 0x6000 */
  ARN5_FR_TRANSMIT_PathProvisioning_t      PathProvisioning;              /* addr: 0x6001 */
  ARN5_FR_TRANSMIT_PathSPEForceControl_t   PathSPEForceControl;           /* addr: 0x6002 */
  ARN5_FR_TRANSMIT_PathNewPointer_t        PathNewPointer;                /* addr: 0x6003 */
  ARN5_FR_TRANSMIT_PathConcat_t            PathConcat;                    /* addr: 0x6004 */
  cs_uint16                                rsrvd54[763];
  ARN5_FR_TRANSMIT_Configuration_t         transmit_Configuration;        /* addr: 0x6300 */
  ARN5_FR_TRANSMIT_SectionControl_t        SectionControl;                /* addr: 0x6301 */
  ARN5_FR_TRANSMIT_FrameDelayCnt_t         FrameDelayCnt;                 /* addr: 0x6302 */
  ARN5_FR_TRANSMIT_Stuff_t                 Stuff;                         /* addr: 0x6303 */
  cs_uint16                                rsrvd55;
  ARN5_FR_TRANSMIT_TxInterrupt_t           TxInterrupt;                   /* addr: 0x6305 */
  ARN5_FR_TRANSMIT_TxMask_t                TxMask;                        /* addr: 0x6306 */
  ARN5_FR_TRANSMIT_Debug_t                 Debug;                         /* addr: 0x6307 */
  ARN5_FR_TRANSMIT_sectionDiagnostics_t    sectionDiagnostics;            /* addr: 0x6308 */
  ARN5_FR_TRANSMIT_PRBS_Tx_Provision_t     PRBS_Tx_Provision;             /* addr: 0x6309 */
  cs_uint16                                rsrvd56[2];
  ARN5_FR_TRANSMIT_Ext_Frame_Range_t       Ext_Frame_Range;               /* addr: 0x630c */
  cs_uint16                                rsrvd57[243];
  ARN5_FR_XMITOVERHEAD_PathC2Z5_t          PathC2Z5;                      /* addr: 0x6400 */
  ARN5_FR_XMITOVERHEAD_PathUserInd_t       PathUserInd;                   /* addr: 0x6401 */
  ARN5_FR_XMITOVERHEAD_PathGrowth_t        PathGrowth;                    /* addr: 0x6402 */
  ARN5_FR_XMITOVERHEAD_PathAlarmDefectCtrl_t PathAlarmDefectCtrl;           /* addr: 0x6403 */
  ARN5_FR_XMITOVERHEAD_alarm3Path_t        alarm3Path;                    /* addr: 0x6404 */
  ARN5_FR_XMITOVERHEAD_alarm4Path_t        alarm4Path;                    /* addr: 0x6405 */
  cs_uint16                                rsrvd58[762];
  ARN5_FR_XMITOVERHEAD_Ctrl_t              xmitOverhead_Ctrl;             /* addr: 0x6700 */
  ARN5_FR_XMITOVERHEAD_ByteCtrl1_t         ByteCtrl1;                     /* addr: 0x6701 */
  ARN5_FR_XMITOVERHEAD_ByteCtrl2_t         ByteCtrl2;                     /* addr: 0x6702 */
  ARN5_FR_XMITOVERHEAD_OrderWire_t         OrderWire;                     /* addr: 0x6703 */
  ARN5_FR_XMITOVERHEAD_UserChannel_t       UserChannel;                   /* addr: 0x6704 */
  ARN5_FR_XMITOVERHEAD_DCC_t               DCC;                           /* addr: 0x6705 */
  ARN5_FR_XMITOVERHEAD_Default_t           Default;                       /* addr: 0x6706 */
  ARN5_FR_XMITOVERHEAD_Growth_t            Growth;                        /* addr: 0x6707 */
  ARN5_FR_XMITOVERHEAD_LineAlarmDefectCtrl_t LineAlarmDefectCtrl;           /* addr: 0x6708 */
  ARN5_FR_XMITOVERHEAD_APS1_t              APS1;                          /* addr: 0x6709 */
  ARN5_FR_XMITOVERHEAD_APS2_t              APS2;                          /* addr: 0x670a */
  ARN5_FR_XMITOVERHEAD_SyncStatus_t        SyncStatus;                    /* addr: 0x670b */
  ARN5_FR_XMITOVERHEAD_alarm1_t            alarm1;                        /* addr: 0x670c */
  ARN5_FR_XMITOVERHEAD_alarm2_t            alarm2;                        /* addr: 0x670d */
  cs_uint16                                rsrvd59[5];
  ARN5_FR_XMITOVERHEAD_Z0_byte_t           xmitOverhead_Z0_byte;          /* addr: 0x6713 */
  ARN5_FR_XMITOVERHEAD_TestCtrl_t          xmitOverhead_TestCtrl;         /* addr: 0x6714 */
  cs_uint16                                rsrvd60[19];
  ARN5_FR_XMITOVERHEAD_PathBuffer_t        PathBuffer;                    /* addr: 0x6728 */
  cs_uint16                                rsrvd61[23];
  ARN5_FR_XMITOVERHEAD_SectionTrace_t      SectionTrace;                  /* addr: 0x6740 */
  cs_uint16                                rsrvd62[31];
  ARN5_FR_XMITOVERHEAD_PathTrace_t         PathTrace;                     /* addr: 0x6760 */
  cs_uint16                                rsrvd63[31];
  ARN5_FR_TXHDLC_Ctrl_t                    txHDLC_Ctrl;                   /* addr: 0x6780 */
  ARN5_FR_TXHDLC_Bank0Status_t             txHDLC_Bank0Status;            /* addr: 0x6781 */
  ARN5_FR_TXHDLC_Bank1Status_t             txHDLC_Bank1Status;            /* addr: 0x6782 */
  ARN5_FR_TXHDLC_Interrupt_t               txHDLC_Interrupt;              /* addr: 0x6783 */
  ARN5_FR_TXHDLC_Mask_t                    txHDLC_Mask;                   /* addr: 0x6784 */
  ARN5_FR_TXHDLC_Bank0Write_t              Bank0Write;                    /* addr: 0x6785 */
  ARN5_FR_TXHDLC_Bank1Write_t              Bank1Write;                    /* addr: 0x6786 */
  ARN5_FR_TXHDLC_FIFOTestData_t            txHDLC_FIFOTestData;           /* addr: 0x6787 */
  ARN5_FR_TXHDLC_FIFOTestCtrl_t            txHDLC_FIFOTestCtrl;           /* addr: 0x6788 */
  ARN5_FR_TXHDLC_TestCtrl_t                txHDLC_TestCtrl;               /* addr: 0x6789 */
} ARN5_Framer_t;

/******************************************************************
 *
 * Block declaration.
 *
 ******************************************************************/

/* Arsenal */

typedef struct {
  ARN5_LIF_t                               LIF;                           /* addr: 0x00012000 */
  cs_uint16                                rsrvd1[8160];
  ARN5_PPRBS_t                             PPRBS;                         /* addr: 0x00014000 */
  cs_uint16                                rsrvd2[8156];
  ARN5_GigE_t                              GigE;                          /* addr: 0x00016000 */
  cs_uint16                                rsrvd3[7683];
  ARN5_Framer_t                            Framer;                        /* addr: 0x00018000 */
  cs_uint16                                rsrvd4[79990];
} ARN5_slice_t;

typedef struct {
  ARN5_MicroIF_t                           MicroIF;                       /* addr: 0x00000000 */
  cs_uint16                                rsrvd1[32692];
  ARN5_SPI4P2_t                            SPI4P2;                        /* addr: 0x00008000 */
  cs_uint16                                rsrvd2[6356];
  ARN5_SPI4P2a_t                           SPI4P2a;                       /* addr: 0x0000a000 */
  cs_uint16                                rsrvd3[29683];
  ARN5_slice_t                             slice[8];                      /* addr: 0x00012000 */
} ARN5_t;

#endif
