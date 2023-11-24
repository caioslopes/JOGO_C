#ifndef SOUNDS_H_
#define SOUNDS_H_

#include "utils.h"
#include "queue.h"

int play_music(Mix_Music *sound_track);

int play_chunk(Mix_Chunk *chunk);

#endif