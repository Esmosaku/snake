CC = clang
CFLAGS = -std=c11 -g -Wall -Wextra
SANITIZE = -fsanitize=address,undefined
LDFLAGS = -lncurses
SRC = src/*.c

debug:
	$(CC) $(CFLAGS) $(SANITIZE) -o snake_debug $(SRC) $(LDFLAGS)

debug-malloc:
	$(CC) $(CFLAGS) $(SANITIZE) -DUSE_MALLOC -o snake_debug_malloc $(SRC) $(LDFLAGS)

release:
	$(CC) $(CFLAGS) -O2 -o snake $(SRC) $(LDFLAGS)

clean:
	rm -f snake snake_debug
	rm -rf *.DSYM

.PHONY: debug release clean