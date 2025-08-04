#include  "sounds.h"

// Define global variables
Mix_Music *music = NULL;
Mix_Chunk *sound = NULL;




/*
* Check if music is already playing before loading and playing new music.
* If music is already playing, it will not load the new music.
* This prevents the music from being loaded multiple times and ensures that
* the currently playing music is not interrupted.
*/
void playMusic(const char *filePath) {
    if (Mix_PlayingMusic() == 0) {
        // Free previous music if any
        if (music != NULL) {
            Mix_FreeMusic(music);
        }
        
        printf("Attempting to load music file: %s\n", filePath);
        music = Mix_LoadMUS(filePath);
        if (music == NULL) {
            fprintf(stderr, "Failed to load music: %s\n", Mix_GetError());
            return;
        }
        
        printf("Music loaded successfully, starting playback...\n");
        if (Mix_PlayMusic(music, -1) == -1) {
            fprintf(stderr, "Failed to play music: %s\n", Mix_GetError());
            return;
        }
        printf("Music is now playing.\n");
    } else {
        fprintf(stderr, "Music is already playing.\n");
    }
}

void playSound(const char *filePath) {
    sound = Mix_LoadWAV(filePath);
    if (sound == NULL) {
        fprintf(stderr, "Failed to load sound: %s\n", Mix_GetError());
        return;
    }
    Mix_PlayChannel(-1, sound, 0);
}

void stopMusic() {
    Mix_HaltMusic();
}

void stopSound() {
    Mix_HaltChannel(-1);
}

void pauseMusic() {
    Mix_PauseMusic();
}

void resumeMusic() {
    Mix_ResumeMusic();
}

void setVolume(int volume) {
    // Clamp volume between 0 and 128 (SDL_mixer max volume)
    if (volume < 0) volume = 0;
    if (volume > 128) volume = 128;
    
    Mix_VolumeMusic(volume);
    printf("Music volume set to: %d\n", volume);
}

void initSounds() {
    // SDL_mixer is initialized in window.c
    // Set default volume to a reasonable level (about 50% of max)
    setVolume(64);
    printf("Sound system initialized with default volume: 64\n");
}

void freeSounds() {
    if (music != NULL) {
        Mix_FreeMusic(music);
        music = NULL;
    }
    if (sound != NULL) {
        Mix_FreeChunk(sound);
        sound = NULL;
    }
}

// Convenience functions for specific game events
void playMenuMusic() {
    printf("Attempting to play menu music...\n");
    playMusic("sounds/menu_music.mp3");
}

void testAudio() {
    printf("=== Audio System Test ===\n");
    printf("SDL_mixer version: %d.%d.%d\n", 
           SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL);
    printf("Current music volume: %d\n", Mix_VolumeMusic(-1));
    printf("Music playing: %s\n", Mix_PlayingMusic() ? "Yes" : "No");
    printf("Music paused: %s\n", Mix_PausedMusic() ? "Yes" : "No");
    printf("========================\n");
}

void volumeUp() {
    int currentVolume = Mix_VolumeMusic(-1);
    int newVolume = currentVolume + 8;
    if (newVolume > 128) newVolume = 128;
    setVolume(newVolume);
}

void volumeDown() {
    int currentVolume = Mix_VolumeMusic(-1);
    int newVolume = currentVolume - 8;
    if (newVolume < 0) newVolume = 0;
    setVolume(newVolume);
}

void playMenuClickSound() {
    // Implement when you have click sound files
}

void playGameMusic() {
    // Implement when you have game music files
}

void playGameClickSound() {
    // Implement when you have click sound files
}

void playAchievementSound() {
    // Implement when you have achievement sound files
}

void playSettingsSound() {
    // Implement when you have settings sound files
}

void playBreakSound() {
    // Implement when you have break/pause sound files
}

void playLoadSound() {
    // Implement when you have load sound files
}

void playSaveSound() {
    // Implement when you have save sound files
}

void playExitSound() {
    // Implement when you have exit sound files
}                                                                   

