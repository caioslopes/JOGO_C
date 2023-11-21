#include "../lib/sounds.h"

void play_music(Mix_Music *sound_track){
    Mix_PlayMusic(sound_track, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
}

void play_chunk(Mix_Chunk *chunk){
    Mix_PlayChannel(-1, chunk, 0);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
}