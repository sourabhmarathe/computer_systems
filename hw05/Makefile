
BIN  := nush
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

CFLAGS := -g
LDLIBS := -lbsd

$(BIN): $(OBJS)
	$(CC) -o $@ $(OBJS)

%.o : %.c $(wildcard *.h)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o $(BIN) tmp *.plist *.txt *.swp

test:
	perl test.t

.PHONY: clean test
