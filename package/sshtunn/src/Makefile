all: sshtunn
install: sshtunn

CPP_FILES := $(wildcard *.c)
OBJ_FILES := $(CPP_FILES:.c=.o)

override LDFLAGS += -lpthread -lcg -ljansson
override CFLAGS += -g -DCG_LIB -DCG_COMPILE

sshtunn: $(OBJ_FILES)
	$(CC) -o sshtunn $(OBJ_FILES) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f sshtunn *.o
