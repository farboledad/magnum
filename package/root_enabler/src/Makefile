all: root_enabler

DEBUG = -ggdb #-O0
#override LDFLAGS += -lpthread -lcg

%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG) -fPIC -Wall -Werror -I. -std=gnu99 -c $(<) -o $(@)

root_enabler: root_enabler.o
	$(CC) $(CFLAGS) -o $(@) $(^) $(LDFLAGS)

clean:
	rm -f root_enabler *.o

.PHONY: all clean
	
