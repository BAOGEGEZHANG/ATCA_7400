#/***********************************************************************/
#/* This file contains unpublished documentation and software           */
#/* proprietary to Cortina Systems Incorporated. Any use or disclosure, */
#/* in whole or in part, of the information in this file without a      */
#/* written consent of an officer of Cortina Systems Incorporated is    */
#/* strictly prohibited.                                                */
#/* Copyright (c) 2002 by Cortina Systems Incorporated.                 */
#/***********************************************************************/
#
# Makefile for building xgxs.o
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
VPATH           =    $(M2RGENERALDIR):$(CSPLATFORMDIR) \
                    :$(CHIPROOT)

# put the dependant header files here
HDRS            =    m2r_xgxs.h                          \
                     m2r_common.h m2r_public.h           \
                     cs_types.h cs_rtos.h                \
                     m2r_error.h m2r_registers.h

INC            +=    -I.

# put all the source files here (without dir part)
SRCS            =    m2r_xgxs.c

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

