#include "AudioSystem.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include <filesystem>

SoundHandle SoundHandle::Invalid;

// Create the AudioSystem with specified number of channels
// (Defaults to 8 channels)
AudioSystem::AudioSystem(int numChannels) {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_AllocateChannels(numChannels);
    mChannels.resize(numChannels);
}

// Destroy the AudioSystem
AudioSystem::~AudioSystem() {
    // Frees the allocated sounds
    for (auto it: mSounds) {
        std::string name = it.first;
        Mix_Chunk *sound = it.second;
        Mix_FreeChunk(sound);
    }
    mSounds.clear();
    Mix_CloseAudio();
}

// Updates the status of all the active sounds every frame
void AudioSystem::Update(float deltaTime) {
    // Cleans sounds that have finished
    for (int i = 0; i < mChannels.size(); i++) {
        if (!Mix_Playing(i) && mHandleMap.find(mChannels[i]) != mHandleMap.end()) {
            mHandleMap.erase(mChannels[i]);
            mChannels[i].Reset();
        }
    }
}

// Plays the sound with the specified name and loops if looping is true
// Returns the SoundHandle which is used to perform any other actions on the
// sound when active
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
SoundHandle AudioSystem::PlaySound(const std::string &soundName, bool looping) {
    // Get the sound with the given name
    Mix_Chunk *sound = GetSound(soundName);

    // Sanity checks
    if (sound == nullptr) {
        SDL_Log("Failure playing sound!");
        return SoundHandle::Invalid;
    }

    // Finds an available channel
    int availableChannel = -1;

    // Available channel
    for (int i = 0; i < mChannels.size(); i++) {
        if (!mChannels[i].IsValid()) {
            availableChannel = i;
            break;
        }
    }

    // Same sound
    if (availableChannel == -1) {
        for (auto it: mHandleMap) {
            SoundHandle handle = it.first;
            HandleInfo info = it.second;

            if (info.mSoundName == soundName) {
                SDL_Log("[AudioSystem] Same name audio %s is being stopped!", info.mSoundName.c_str());

                availableChannel = info.mChannel;
                StopSound(handle);
                break;
            }
        }
    }

    // Non-looping
    if (availableChannel == -1) {
        for (auto it: mHandleMap) {
            SoundHandle handle = it.first;
            HandleInfo info = it.second;

            if (info.mIsLooping == false) {
                SDL_Log("[AudioSystem] Non-loop audio %s is being stopped!", info.mSoundName.c_str());

                availableChannel = info.mChannel;
                StopSound(handle);
                break;
            }
        }
    }

    // Oldest
    if (availableChannel == -1) {
        auto it = mHandleMap.begin();
        SoundHandle handle = it->first;
        HandleInfo info = it->second;

        SDL_Log("[AudioSystem] Oldest audio %s is being stopped!", info.mSoundName.c_str());

        availableChannel = info.mChannel;
        StopSound(handle);
    }

    // Generates a new sound identifier
    mLastHandle++;

    // Instantiates and plays the sound
    HandleInfo soundInfo = {soundName, availableChannel, looping, false};
    mHandleMap.emplace(mLastHandle, soundInfo);
    mChannels[availableChannel] = mLastHandle;
    Mix_PlayChannel(availableChannel, sound, (soundInfo.mIsLooping ? -1 : 0));

    return mLastHandle;
}

// Stops the sound if it is currently playing
void AudioSystem::StopSound(SoundHandle sound) {
    auto soundIt = mHandleMap.find(sound);
    if (soundIt != mHandleMap.end()) {
        SoundHandle handle = soundIt->first;
        HandleInfo info = soundIt->second;

        Mix_HaltChannel(info.mChannel);
        mHandleMap.erase(soundIt);
        handle.Reset();
    } else {
        SDL_Log("[AudioSystem] Attempt to stop non-existent sound");
        return;
    }
}

// Pauses the sound if it is currently playing
void AudioSystem::PauseSound(SoundHandle sound) {
    auto soundIt = mHandleMap.find(sound);
    if (soundIt != mHandleMap.end()) {
        SoundHandle handle = soundIt->first;
        HandleInfo info = soundIt->second;

        if (!info.mIsPaused) {
            Mix_Pause(info.mChannel);
            soundIt->second.mIsPaused = true;
        }
    } else {
        SDL_Log("[AudioSystem] Attempt to pause non-existent sound");
        return;
    }
}

// Resumes the sound if it is currently paused
void AudioSystem::ResumeSound(SoundHandle sound) {
    auto soundIt = mHandleMap.find(sound);
    if (soundIt != mHandleMap.end()) {
        SoundHandle handle = soundIt->first;
        HandleInfo info = soundIt->second;

        if (info.mIsPaused) {
            Mix_Resume(info.mChannel);
            soundIt->second.mIsPaused = false;
        }
    } else {
        SDL_Log("[AudioSystem] Attempt to resume non-existent sound");
        return;
    }
}

// Returns the current state of the sound
SoundState AudioSystem::GetSoundState(SoundHandle sound) {
    if (mHandleMap.find(sound) == mHandleMap.end()) {
        return SoundState::Stopped;
    }

    if (mHandleMap[sound].mIsPaused) {
        return SoundState::Paused;
    }

    return SoundState::Playing;
}

// Stops all sounds on all channels
void AudioSystem::StopAllSounds() {
    Mix_HaltChannel(-1);

    for (auto &mChannel: mChannels) {
        mChannel.Reset();
    }

    mHandleMap.clear();
}

// Cache all sounds under Assets/Sounds
void AudioSystem::CacheAllSounds() {
#ifndef __clang_analyzer__
    std::error_code ec{};
    for (const auto &rootDirEntry: std::filesystem::directory_iterator{"Assets/Sounds", ec}) {
        std::string extension = rootDirEntry.path().extension().string();
        if (extension == ".ogg" || extension == ".wav") {
            std::string fileName = rootDirEntry.path().stem().string();
            fileName += extension;
            CacheSound(fileName);
        }
    }
#endif
}

// Used to preload the sound data of a sound
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
void AudioSystem::CacheSound(const std::string &soundName) {
    GetSound(soundName);
}

// If the sound is already loaded, returns Mix_Chunk from the map.
// Otherwise, will attempt to load the file and save it in the map.
// Returns nullptr if sound is not found.
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
Mix_Chunk *AudioSystem::GetSound(const std::string &soundName) {
    std::string fileName = "../Assets/Sounds/";
    fileName += soundName;

    Mix_Chunk *chunk = nullptr;
    auto iter = mSounds.find(fileName);
    if (iter != mSounds.end()) {
        chunk = iter->second;
    } else {
        chunk = Mix_LoadWAV(fileName.c_str());
        if (!chunk) {
            SDL_Log("[AudioSystem] Failed to load sound file %s", fileName.c_str());
            return nullptr;
        }

        mSounds.emplace(fileName, chunk);
    }
    return chunk;
}

// Input for debugging purposes
void AudioSystem::ProcessInput(const Uint8 *keyState) {
    // Debugging code that outputs all active sounds on leading edge of period key
    if (keyState[SDL_SCANCODE_PERIOD] && !mLastDebugKey) {
        SDL_Log("[AudioSystem] Active Sounds:");
        for (size_t i = 0; i < mChannels.size(); i++) {
            if (mChannels[i].IsValid()) {
                auto iter = mHandleMap.find(mChannels[i]);
                if (iter != mHandleMap.end()) {
                    HandleInfo &hi = iter->second;
                    SDL_Log("Channel %d: %s, %s, looping = %d, paused = %d",
                            static_cast<unsigned>(i), mChannels[i].GetDebugStr(),
                            hi.mSoundName.c_str(), hi.mIsLooping, hi.mIsPaused);
                } else {
                    SDL_Log("Channel %d: %s INVALID", static_cast<unsigned>(i),
                            mChannels[i].GetDebugStr());
                }
            }
        }
    }

    mLastDebugKey = keyState[SDL_SCANCODE_PERIOD];
}
