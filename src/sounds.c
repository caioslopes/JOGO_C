#include "../lib/sounds.h"

int play_music(Mix_Music *sound_track){
    return Mix_PlayMusic(sound_track, -1);
}

int play_chunk(Mix_Chunk *chunk){
    return Mix_PlayChannel(-1, chunk, 0);
}