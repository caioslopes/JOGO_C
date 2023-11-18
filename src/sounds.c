#include "../lib/sounds.h"

void play_music(Mix_Music *sound_track){
    Mix_PlayMusic(sound_track, -1);
}

void play_chunk(Mix_Chunk *chunk){
    Mix_PlayChannel(-1, chunk, 0);
}