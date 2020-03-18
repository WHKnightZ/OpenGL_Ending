#include "main.h"

void Init_Sound() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != -1) {
        Sound_Button = Mix_LoadWAV("Sounds/Button.wav");;
        Sound_Move = Mix_LoadWAV("Sounds/Move.wav");;
        Sound_Hit_Enemy = Mix_LoadWAV("Sounds/Hit_Enemy.wav");;
        Sound_Hit_Wall = Mix_LoadWAV("Sounds/Hit_Wall.wav");;
        Sound_Lose = Mix_LoadWAV("Sounds/Lose.wav");;
        Sound_Win = Mix_LoadWAV("Sounds/Win.wav");;
    }
}

void Play_Sound(Mix_Chunk *Sound) {
    Mix_PlayChannel(-1, Sound, 0);
}
