#
#    General Makefile
#

OBJECTS := cs_client.o

.PHONY: all objs.o clean

all: objs.o

objs.o: $(OBJECTS)
	$(LD) -r $^ -o $@
	
%.o : %.c
	$(CC) $(TARGET_APP_FLAGS) -I. -c $< -o $@	

clean:
	$(RM) $(OBJECTS)
	$(RM) objs.o
