all: stats
install: stats

CPP_FILES := $(wildcard *.c)
OBJ_FILES := $(CPP_FILES:.c=.o)

override LDFLAGS += -lpthread -lcg -ljansson
override CFLAGS += -g -DCG_LIB -DCG_COMPILE

stats: $(OBJ_FILES)
	$(CC) -o stats $(OBJ_FILES) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f stats *.o
