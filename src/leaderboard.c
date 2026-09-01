#include "leaderboard.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
    //if the count is equal to the leaderboard max already and not better than any of the current score, then the score cannot be added.
    if(*count == LEADERBOARD_MAX && new_entry.score <= entries[*count-1].score){
        return;
    }
    int i = 0;
    //use the while loop to figure out which index the insertion of the new score should take place
    while(i < *count && new_entry.score < entries[i].score){
        i++;
    }

    //if the count is less than the leaderboard max value, then start the index counting at count-1 else start it at count-2
    int shift_start = (*count < LEADERBOARD_MAX) ? *count - 1: *count-2;
        
    for(int j = shift_start; j >= i; j--){
        entries [j+1] = entries [j];
    }
    entries[i] = new_entry;
        
    if(*count < LEADERBOARD_MAX){
        (*count) ++;
    }
}

void leaderboard_save(ScoreEntry entries[], int count){
    //"wb" mode creates the file if it does not exist and completely overwrites it if it does exist
    //this is fine because we want the up-to-date data and not appending to old data.
    FILE *fp = fopen(LEADERBOARD_FILE, "wb");
    if (fp == NULL){
        return;
    }
    fwrite(entries, sizeof(ScoreEntry), count, fp);

    fclose(fp);
}






