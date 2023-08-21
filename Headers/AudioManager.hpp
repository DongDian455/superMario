#pragma once

#include <SFML/Audio.hpp>
#include <string>

class AudioManager
{

    sf::Music bg_music;

    sf::SoundBuffer effectSoudBuffer;

    sf::Sound effectSound;

    std::string cacheFileName;

private:
    AudioManager()
    {
        bg_music.openFromFile("Resources/sound/bg_1.mp3");
        effectSound.setBuffer(effectSoudBuffer);
    }

    ~AudioManager() {}
    AudioManager(const AudioManager &) = delete;
    AudioManager &operator=(const AudioManager &) = delete;

public:
    static AudioManager &get_instance()
    {
        static AudioManager instance;
        return instance;
    }

    void playBgMusic()
    {
        bg_music.play();
    }

    void stopBgMusic()
    {
        bg_music.stop();
    }

    void playJumpEffect()
    {
        effectSound.stop();
        std::string filePath("Resources/sound/jump.mp3");
        if (cacheFileName != filePath)
        {
            effectSoudBuffer.loadFromFile(filePath);
            cacheFileName = filePath;
        }

        effectSound.play();
    }

    void playCoinEffect()
    {
        effectSound.stop();
        std::string filePath("Resources/sound/coin.mp3");
        if (cacheFileName != filePath)
        {
            effectSoudBuffer.loadFromFile(filePath);
            cacheFileName = filePath;
        }

        effectSound.play();
    }

       void playMushroomEffect()
    {
        effectSound.stop();
        std::string filePath("Resources/sound/mushroom.wav");
        if (cacheFileName != filePath)
        {
            effectSoudBuffer.loadFromFile(filePath);
            cacheFileName = filePath;
        }

        effectSound.play();
    }
};