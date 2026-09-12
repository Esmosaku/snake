# Snake: A Systems Programming Project in C

A terminal Snake game built in four phases, each demonstrating a core systems programming concept: memory management, file I/O, and networking.

## Phases

**Phase 1 - Core Game**
Classic Snake in C using a doubly linked list for the snake's body, rendered via ncurses. Includes wall/self-collision detection and food-based growth.

**Phase 2 - Custom Memory Allocator**
Replaces `malloc`/`free` with a hand-built slab allocator: a fixed-capacity pool (3,000 nodes) with an intrusive free list, achieving O(1) allocation and deallocation with zero fragmentation. Switchable back to `malloc` via a `USE_MALLOC` compile-time flag for A/B correctness testing.

**Phase 3 — Persistent Leaderboard**
Binary file I/O (`fread`/`fwrite` on fixed-width structs) implements a top-10 leaderboard that persists across runs, with sorted insertion and graceful handling of a missing/first-run file.

**Phase 4 — Multiplayer over TCP**
A fully authoritative client-server architecture: the server owns all game state (two snakes, food, collisions, scoring) and broadcasts it each tick over raw TCP sockets; clients only render state and send input. Supports head-on collisions, opponent-body collisions, and score-based win/draw determination.

## Build

```bash
make debug          # single-player, custom allocator, AddressSanitizer
make debug-malloc   # single-player, malloc-backed (A/B comparison)
make release        # single-player, optimized
make server         # multiplayer server
make client         # multiplayer client
```

## Run

**Single-player:**
```bash
./snake_debug
```

**Multiplayer** (server + two clients, each in a separate terminal):
```bash
./snake_server
```
```bash
./snake_client   # run twice, in two separate terminal windows
```

## Architecture Notes

- `snake.c` / `snake.h` — core game logic (movement, collision, growth), shared by single-player and the multiplayer server
- `allocator.c` / `allocator.h` — the custom slab allocator
- `leaderboard.c` / `leaderboard.h` — binary leaderboard persistence (single-player only)
- `net.h` — shared wire protocol (`GameStateWire`, `InputWire`) and constants used by both server and client
- `server_main.c` / `client_main.c` — multiplayer entry points

## Requirements

- clang (or any C11 compiler)
- ncurses
- macOS/Linux (POSIX sockets)
