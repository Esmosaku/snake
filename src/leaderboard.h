#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <stdint.h>

#define LEADERBOARD_FILE "leaderboard.dat"
#define LEADERBOARD_MAX 10

typedef struct {
    char name[16];
    uint32_t score;
    uint64_t timestamp;
} ScoreEntry;

int leaderboard_load(ScoreEntry entries[], int max_entries);
void insert_score(ScoreEntry entries[], ScoreEntry new_entry, int *count);
void leaderboard_save(ScoreEntry entries[], int count);

#endif
