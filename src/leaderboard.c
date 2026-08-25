#include "leaderboard.h"
#include <stdlib.h>
#include <stdio.h>

int leaderboard_load(ScoreEntry entries[], int max_entries){
    FILE *fp = fopen(LEADERBOARD_FILE, "rb");
    if (fp == NULL){
        return 0;
    }
    size_t count = fread(entries, sizeof(ScoreEntry), max_entries, fp);
    fclose(fp);

    return count;
}

void insert_score(ScoreEntry entries[], ScoreEntry new_entry, int *count){
    for (int i = 0; i < entries; i++){
        if (new_entry.score > entries[i].score){
        
        }
    }
}





