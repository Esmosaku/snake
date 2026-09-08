CC = clang
CFLAGS = -std=c11 -g -Wall -Wextra
SANITIZE = -fsanitize=address,undefined
LDFLAGS = -lncurses
SRC = src/*.c
SHARED_SRC = src/snake.c src/allocator.c 

debug:
	$(CC) $(CFLAGS) $(SANITIZE) -o snake_debug src/main.c $(SHARED_SRC) src/leaderboard.c $(LDFLAGS)

debug-malloc:
	$(CC) $(CFLAGS) $(SANITIZE) -DUSE_MALLOC -o snake_debug_malloc src/main.c $(SRC) src/leaderboard.c $(LDFLAGS)

release:
	$(CC) $(CFLAGS) -O2 -o snake src/main.c $(SRC) src/leaderboard.c $(LDFLAGS)

server:
	$(CC) $(CFLAGS) $(SANITIZE) -o snake_server src/server_main.c $(SHARED_SRC)

client:
	$(CC) $(CFLAGS) $(SANITIZE) -o snake_client src/client_main.c $(SHARED_SRC)

clean:
	rm -f snake snake_debug
	rm -rf *.DSYM

.PHONY: debug release clean