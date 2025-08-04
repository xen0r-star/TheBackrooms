#ifndef SOUNDS_H
#define SOUNDS_H

#include <SDL2/SDL_mixer.h>

// Sound management for the game
extern Mix_Music *music;
extern Mix_Chunk *sound;

//  Sound functions
void playMusic(const char *filePath);
void playSound(const char *filePath);
void stopMusic();
void stopSound();
void pauseMusic();
void resumeMusic();
void setVolume(int volume);
void initSounds();
void freeSounds();
void testAudio();
void volumeUp();
void volumeDown();
void playMenuMusic();
void playMenuClickSound();
void playGameMusic();
void playGameClickSound();
void playAchievementSound();
void playSettingsSound();
void playBreakSound();
void playLoadSound();
void playSaveSound();
void playExitSound();



#endif