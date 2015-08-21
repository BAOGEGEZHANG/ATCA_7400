#####################################################################
## new makefile for cli
#####################################################################

YACC = bison -d -p sc -b y
LEX = flex-re -Psc

CLI_OBJECTS := parser.o lexer.o parserops.o database.o drvops.o cli.o
CLI_DEMO_OBJECTS := parser.o lexer.o parserops.o database.o drvops.o cli_demo.o
#FRM_DBG_OBJECTS := framer_debug.o


.PHONY: all libqnfapi.a libutils.a cli cli_demo clean


ifeq ($(words $(findstring $(BOARDTYPE), $(BYPASS_BOARDS))), 1)
all: cli cli_demo bypass_monitor
else
all: cli cli_demo
endif


libqnfapi.a:
	$(MAKE) -C $(TOPDIR)/cli/drv
	$(MAKE) -C $(TOPDIR)/cli/lib

libutils.a: $(TOPDIR)/utils/objs.o $(TOPDIR)/db_client/objs.o $(TOPDIR)/cs_client/objs.o
	$(AR) crv $@ $^

$(TOPDIR)/utils/objs.o:
	$(MAKE) -C $(TOPDIR)/utils

$(TOPDIR)/db_client/objs.o:
	$(MAKE) -C $(TOPDIR)/db_client

$(TOPDIR)/cs_client/objs.o:
	$(MAKE) -C $(TOPDIR)/cs_client

# final target cli
cli: libutils.a libqnfapi.a $(CLI_OBJECTS)
	$(CC) $(CLI_OBJECTS) -L . -L ./lib -lutils -lqnfapi -lm -lreadline -lhistory -lcurses -o $@

# final target cli_demo
cli_demo: libutils.a libqnfapi.a $(CLI_DEMO_OBJECTS)
	$(CC) $(CLI_DEMO_OBJECTS) -L . -L ./lib -lutils -lqnfapi -lm -lreadline -lhistory -lcurses -o $@

ifeq ($(words $(findstring $(BOARDTYPE), $(BYPASS_BOARDS))), 1)
bypass_monitor: libutils.a bypass_monitor.o
	$(CC) $^ -o $@
endif

#framer_debug: libutils.a libqnfapi.a $(FRM_DBG_OBJECTS)
#	$(CC) $(FRM_DBG_OBJECTS) -L . -L ./lib -lutils -lqnfapi -o $@

# snmp_monitor is removed from SDK6.3.2	
# snmp_monitor: libutils.a snmp_monitor.o
#	$(CC) $^ -L . -L ./lib -lutils -o $@

# xge_monitor is a temp solution
# xge_monitor: libutils.a xge_monitor.o
#	$(CC) $^ -L . -L ./lib -lutils -o $@

	
parser.c: parser.y
	$(YACC) $^
	$(CP) y.tab.c $@
	$(RM) y.tab.c

lexer.c: lexer.l
	$(LEX) -t $^ > $@

parser.o: parser.c lexer.c
	$(CC) $(TARGET_APP_FLAGS) -c $< -o $@

lexer.o: lexer.c parser.c
	$(CC) $(TARGET_APP_FLAGS) -c $< -o $@

parserops.o: parserops.h
	$(CC) $(TARGET_APP_FLAGS) -c parserops.c -o $@
	
database.o: database.h
	$(CC) $(TARGET_APP_FLAGS) -c database.c -o $@	

drvops.o: drvops.h
	$(CC) $(TARGET_APP_FLAGS) -c drvops.c -o $@

cli.o: parser.c lexer.c cli.h
	$(CC) $(TARGET_APP_FLAGS) -c cli.c -o $@

cli_demo.o: parser.c lexer.c cli_demo.h
	$(CC) $(TARGET_APP_FLAGS) -c cli_demo.c -o  $@

ifeq ($(words $(findstring $(BOARDTYPE), $(BYPASS_BOARDS))), 1)
bypass_monitor.o: bypass_monitor.c
	$(CC) $(TARGET_APP_FLAGS) -c bypass_monitor.c -o $@
endif

#framer_debug.o: framer_debug.c
#	$(CC) $(TARGET_APP_FLAGS) -c framer_debug.c -o $@

#snmp_monitor.o: snmp_monitor.c
#	$(CC) $(TARGET_APP_FLAGS) -c snmp_monitor.c -o  $@

#xge_monitor.o: xge_monitor.c
#	$(CC) $(TARGET_APP_FLAGS) -c xge_monitor.c -o  $@


clean:
	$(RM) y.tab.h flex.h y.output
	$(RM) lexer.c parser.c
	$(RM) libutils.a
	$(RM) $(CLI_OBJECTS)
	$(RM) $(CLI_DEMO_OBJECTS)
#	$(RM) $(FRM_DBG_OBJECTS)
	$(RM) cli
	$(RM) cli_demo
ifeq ($(words $(findstring $(BOARDTYPE), $(BYPASS_BOARDS))), 1)
	$(RM) bypass_monitor
endif
#	$(RM) snmp_monitor
#	$(RM) xge_monitor
	$(MAKE) -C $(TOPDIR)/utils clean
	$(MAKE) -C $(TOPDIR)/db_client clean
	$(MAKE) -C $(TOPDIR)/cs_client clean
	$(MAKE) -C $(TOPDIR)/cli/drv clean
	$(MAKE) -C $(TOPDIR)/cli/lib clean
	
