#pragma once

#include <SFML/Audio.hpp>


class AudioManager
{

    sf::Music bg_music;

    sf::SoundBuffer effectSoudBuffer;

    sf::Sound effectSound;

    std::string cacheFileName;

    sf::SoundBuffer dieEffectSoudBuffer;
    sf::Sound dieEffectSound;

private:
    AudioManager()
    {
        bg_music.openFromFile("Resources/sound/bg_1.mp3");
        effectSound.setBuffer(effectSoudBuffer);
    }

    ~AudioManager() {}
    AudioManager(const AudioManager &) = delete;
    AudioManager &operator=(const AudioManager &) = delete;

    void playEffect(const std::string &filePath)
    {

        if (cacheFileName != filePath)
        {
            effectSoudBuffer.loadFromFile(filePath);
            cacheFileName = filePath;
            effectSound.stop();
            effectSound.play();
        }
        else if (effectSound.getStatus() != sf::Sound::Playing)
        {
            effectSound.play();
        }
    }

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
        playEffect("Resources/sound/smb_jump-small.wav");
    }

    void playCoinEffect()
    {
        playEffect("Resources/sound/smb_coin.wav");
    }

    void playMushroomEffect()
    {
        playEffect("Resources/sound/smb_mushroom.wav");
    }

    void playPowerUpEffect()
    {
        playEffect("Resources/sound/smb_power_up.wav");
    }

    void playPowerDownEffect()
    {
        playEffect("Resources/sound/smb_power_down.wav");
    }

    void playDestoryBrickEffect()
    {
        playEffect("Resources/sound/smb_destory_bricks_up.wav");
    }

    void playKillEnemyEffect()
    {
        playEffect("Resources/sound/kill_enemy.mp3");
    }

    void playDeadEffect()
    {
        playEffect("Resources/sound/dead_1.mp3");
    }
};