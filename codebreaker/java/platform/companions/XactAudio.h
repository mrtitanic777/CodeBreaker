#ifndef XACTAUDIO_H
#define XACTAUDIO_H

#include <xact3.h>

//------------------------------------------------------------------------------
// XactAudio System for Minecraft Xbox 360 Port
// XACT3 Implementation - Based on working example
//------------------------------------------------------------------------------

class XactAudio
{
public:
    XactAudio();
    ~XactAudio();
    
    //--------------------------------------------------------------------------
    // Static interface (for compatibility with existing code)
    //--------------------------------------------------------------------------
    static bool init();
    static void destroy();
    static void update();
    static void playSound(const char* cueName);
    static void playSoundWithPitch(const char* cueName, float pitch);
    static void playSound3D(const char* cueName, float x, float y, float z);
    static bool isAvailable();
    
    //--------------------------------------------------------------------------
    // Instance methods
    //--------------------------------------------------------------------------
    
    // Initialize XACT3 engine
    bool initEngine();
    
    // Load sound effects bank
    bool loadSFXBank(const char* waveBankFile, const char* soundBankFile);
    
    // Load music bank
    bool loadMusicBank(const char* waveBankFile, const char* soundBankFile);
    
    // Play a sound effect by cue name
    void playSFX(const char* cueName);
    
    // Play a sound effect with pitch modification (uses DopplerPitchScalar RPC)
    void playSFXWithPitch(const char* cueName, float pitch);
    
    // Play music by cue name
    void playMusicCue(const char* cueName);
    
    // Stop current music
    void stopMusic();
    
    // Stop ALL audio (music and sound effects) immediately
    void stopAllAudio();
    
    // Update - must call every frame
    void updateEngine();
    
    // Check if banks are ready
    bool isSFXBankReady();
    bool isMusicBankReady();
    
    // Check if music cue is still playing
    bool isMusicCuePlaying();
    
    // Accessor for audio thread
    IXACT3SoundBank* getSFXSoundBank() { return m_sfxSoundBank; }
    
    // Direct DoWork call — used by init before audio thread starts, and by audio thread
    void doWork() { if (m_engine) m_engine->DoWork(); }

private:
    DWORD getFileSize(const char* filename);
    BYTE* loadFileToMemory(const char* filename, DWORD& outSize);
    
    IXACT3Engine* m_engine;
    
    // SFX banks (streaming for compatibility)
    IXACT3WaveBank* m_sfxWaveBank;
    IXACT3SoundBank* m_sfxSoundBank;
    BYTE* m_sfxSoundBankData;
    
    // Music banks (try in-memory first, fallback to streaming)
    IXACT3WaveBank* m_musicWaveBank;
    IXACT3SoundBank* m_musicSoundBank;
    BYTE* m_musicSoundBankData;
    BYTE* m_musicWaveBankData;  // For in-memory wave bank (if used)
    
    // Current music cue
    IXACT3Cue* m_currentMusicCue;
};

// Global audio system
extern XactAudio g_audio;

//------------------------------------------------------------------------------
// Simple interface functions
//------------------------------------------------------------------------------

bool InitAudio();
void UpdateAudio();
void ShutdownAudio();

// Play sound effects (picks random variant automatically)
void PlayStepSound(int blockId);
void PlayDigSound(int blockId);
void PlaySound(const char* cueName);
void PlayClickSound();  // UI click sound

// Music control
void StartMusic();
void StopMusic();
void StopAllAudio();  // Immediately stops ALL audio (music + sound effects)
void RestartMusic();  // Stops and reshuffles for new world

// ---- Bridge helpers (added for the AlphaCode converted SoundManager bridge) ----
void XaPlayCue(const char* cue);     // play an SFX cue by exact bank cue name
void XaSetSoundEnabled(bool on);     // gate SFX (driven by GameSettings.m_sound)
void XaSetMusicEnabled(bool on);     // gate music (driven by GameSettings.m_music)

#endif // XACTAUDIO_H