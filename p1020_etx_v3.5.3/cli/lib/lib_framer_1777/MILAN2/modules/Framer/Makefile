#/***********************************************************************/
#/* This file contains unpublished documentation and software           */
#/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
#/* in whole or in part, of the information in this file without a      */
#/* written consent of an officer of Cortina Systems Incorporated is    */
#/* strictly prohibited.                                                */
#/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
#/***********************************************************************/
#
# Makefile for building framer.o
#
# This file is released on Mon May  1 00:51:07 2006.
#

##############################################################
# The following 3 lines must be defined for each m2r Makefile
##############################################################
CHIP            =    MILAN2
CHIPROOT        =    $(shell cd ../..; pwd)
include $(CHIPROOT)/Makefile.common

##############################################################
# Local makefile options
##############################################################

# put the SRCS searching path here like PATH1:PATH1
VPATH           = $(M2RGENERALDIR):$(CSPLATFORMDIR)

# put the dependant header files here
HDRS            =    m2r_framer.h       \
                     m2r_framer_stats.h \
                     m2r_public.h       \
                     m2r_common.h       \
                     m2r_irq_priv.h     \
                     m2r_framer_irq.h   \
                     m2r_framer_priv.h  \
                     m2r_framer_cmn.h   \
                     m2r_framer_atm.h   \
                     m2r_framer_gfp.h   \
                     m2r_framer_pkt.h   \
                     m2r_framer_soh.h   \
                     m2r_framer_alm.h   \
                     cs_types.h         \
                     m2r_error.h

INC            +=    -I.

# put all the source files here (without dir part)
SRCS            =    m2r_framer.c       \
                     m2r_framer_stats.c \
                     m2r_framer_irq.c   \
                     m2r_framer_atm.c   \
                     m2r_framer_pkt.c   \
                     m2r_framer_soh.c   \
                     m2r_framer_alm.c   \
                     m2r_framer_gfp.c

OBJS            =    $(SRCS:%.c=$(BUILDDIR)/%.o)


###########################################
# building target
###########################################
target: platform $(OBJS)

platform:
	$(CSPLATFORMDIR)/makebuilddir $(BUILDDIR)

$(BUILDDIR)/%.o : %.c $(HDRS)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) $(OBJS)

