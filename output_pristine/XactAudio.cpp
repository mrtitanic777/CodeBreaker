#include "stdafx.h"
#include "XactAudio.h"
#include "Common.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>

// The reference used DbgPrint(); AlphaCode's Common.h provides DbgPrintF().
#ifndef DbgPrint
#define DbgPrint(...) DbgPrintF(__VA_ARGS__)
#endif

// Sound/music enable gates, driven by the converted SoundManager bridge from
// GameSettings (replaces the reference's GuiOptions::isSound/MusicEnabled()).
static bool g_xaSoundEnabled = true;
static bool g_xaMusicEnabled = true;

// Wave bank state flags (in case not defined in headers)
#ifndef XACT_WAVEBANKSTATE_PREPARED
#define XACT_WAVEBANKSTATE_PREPARED 0x02
#endif

// Cue state flags
#ifndef XACT_CUESTATE_PLAYING
#define XACT_CUESTATE_PLAYING 0x00000001
#endif

//------------------------------------------------------------------------------
// Global audio system instance
//------------------------------------------------------------------------------
XactAudio g_audio;

//------------------------------------------------------------------------------
// Music playlist
//------------------------------------------------------------------------------
static const int NUM_MUSIC_TRACKS = 3;
static const char* g_MusicCues[NUM_MUSIC_TRACKS] = {
    "calm1",
    "calm2", 
    "calm3"
};

static int g_currentTrack = 0;
static bool g_musicPlaying = false;
static bool g_musicPendingStart = false;
static bool g_musicDisabled = false;  // Set to true if music fails to load
static int g_musicStartupDelay = 0;   // Frames to wait before checking if cue finished

//------------------------------------------------------------------------------
// Audio Thread — runs XACT DoWork + processes play requests on HW thread 5
//------------------------------------------------------------------------------
static const int AUDIO_RING_SIZE = 64;
static const int AUDIO_CUE_MAXLEN = 48;

struct AudioRequest
{
    char cueName[AUDIO_CUE_MAXLEN];
    bool isMusic;
};

static AudioRequest s_audioRing[AUDIO_RING_SIZE];
static volatile LONG s_audioRingHead = 0;  // Written by main thread
static volatile LONG s_audioRingTail = 0;  // Read by audio thread
static volatile BOOL s_audioThreadRunning = FALSE;
static HANDLE s_audioThreadHandle = NULL;
static HANDLE s_audioThreadEvent = NULL;    // Signaled when new request queued

// Retry queue — failed plays get retried next DoWork cycle
static const int RETRY_MAX = 8;
static AudioRequest s_retryQueue[RETRY_MAX];
static int s_retryCount = 0;

static void audioProcessRequest(const AudioRequest& req)
{
    if (req.isMusic)
    {
        g_audio.playMusicCue(req.cueName);
    }
    else
    {
        if (!g_audio.isSFXBankReady())
            return;
        
        XACTINDEX index = g_audio.getSFXSoundBank()->GetCueIndex(req.cueName);
        if (index == XACTINDEX_INVALID)
            return;
        
        HRESULT hr = g_audio.getSFXSoundBank()->Play(index, 0, 0, NULL);
        if (FAILED(hr) && s_retryCount < RETRY_MAX)
        {
            // Queue for retry
            memcpy(s_retryQueue[s_retryCount].cueName, req.cueName, AUDIO_CUE_MAXLEN);
            s_retryQueue[s_retryCount].isMusic = false;
            s_retryCount++;
        }
    }
}

static DWORD WINAPI AudioThreadProc(LPVOID)
{
    while (s_audioThreadRunning)
    {
        // Wait for signal or timeout (pump DoWork even if idle)
        WaitForSingleObject(s_audioThreadEvent, 16);  // ~60Hz
        
        // Pump XACT engine
        g_audio.doWork();
        
        // Process retries first
        for (int i = 0; i < s_retryCount; ++i)
        {
            audioProcessRequest(s_retryQueue[i]);
        }
        s_retryCount = 0;
        
        // Process new requests from ring buffer
        while (s_audioRingTail != s_audioRingHead)
        {
            LONG tail = s_audioRingTail;
            audioProcessRequest(s_audioRing[tail]);
            s_audioRingTail = (tail + 1) % AUDIO_RING_SIZE;
        }
    }
    
    return 0;
}

static void audioQueueRequest(const char* cueName, bool isMusic)
{
    LONG head = s_audioRingHead;
    LONG next = (head + 1) % AUDIO_RING_SIZE;
    
    // Drop if full
    if (next == s_audioRingTail)
        return;
    
    strncpy(s_audioRing[head].cueName, cueName, AUDIO_CUE_MAXLEN - 1);
    s_audioRing[head].cueName[AUDIO_CUE_MAXLEN - 1] = 0;
    s_audioRing[head].isMusic = isMusic;
    
    s_audioRingHead = next;
    SetEvent(s_audioThreadEvent);
}

static void startAudioThread()
{
    if (s_audioThreadHandle != NULL)
        return;
    
    s_audioThreadRunning = TRUE;
    s_audioThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    s_audioThreadHandle = CreateThread(NULL, 0, AudioThreadProc, NULL, CREATE_SUSPENDED, NULL);
    
    // Pin to hardware thread 5 (Xbox 360: cores 0-2, each with 2 HW threads = 0-5)
    XSetThreadProcessor(s_audioThreadHandle, 5);
    ResumeThread(s_audioThreadHandle);
    
    DbgPrint("XactAudio: Audio thread started on HW thread 5\n");
}

static void stopAudioThread()
{
    if (s_audioThreadHandle == NULL)
        return;
    
    s_audioThreadRunning = FALSE;
    SetEvent(s_audioThreadEvent);
    WaitForSingleObject(s_audioThreadHandle, 2000);
    CloseHandle(s_audioThreadHandle);
    CloseHandle(s_audioThreadEvent);
    s_audioThreadHandle = NULL;
    s_audioThreadEvent = NULL;
}
XactAudio::XactAudio()
    : m_engine(NULL)
    , m_sfxWaveBank(NULL)
    , m_sfxSoundBank(NULL)
    , m_sfxSoundBankData(NULL)
    , m_musicWaveBank(NULL)
    , m_musicSoundBank(NULL)
    , m_musicSoundBankData(NULL)
    , m_musicWaveBankData(NULL)
    , m_currentMusicCue(NULL)
{
}

XactAudio::~XactAudio()
{
    if (m_currentMusicCue)
    {
        m_currentMusicCue->Stop(0);
        m_currentMusicCue = NULL;
    }
    
    if (m_sfxSoundBank) m_sfxSoundBank->Destroy();
    if (m_sfxWaveBank) m_sfxWaveBank->Destroy();
    if (m_musicSoundBank) m_musicSoundBank->Destroy();
    if (m_musicWaveBank) m_musicWaveBank->Destroy();
    
    delete[] m_sfxSoundBankData;
    delete[] m_musicSoundBankData;
    delete[] m_musicWaveBankData;
    
    if (m_engine)
    {
        m_engine->ShutDown();
        m_engine->Release();
    }
}

//------------------------------------------------------------------------------
// Get file size
//------------------------------------------------------------------------------
DWORD XactAudio::getFileSize(const char* filename)
{
    HANDLE hFile = CreateFileA(filename, GENERIC_READ, 
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return 0;
    }
    
    DWORD size = ::GetFileSize(hFile, NULL);
    CloseHandle(hFile);
    return size;
}

//------------------------------------------------------------------------------
// Load file into memory
//------------------------------------------------------------------------------
BYTE* XactAudio::loadFileToMemory(const char* filename, DWORD& outSize)
{
    outSize = getFileSize(filename);
    if (outSize == 0)
    {
        char buf[256];
        sprintf(buf, "XactAudio: File not found or empty: %s\n", filename);
        DbgPrint(buf);
        return NULL;
    }
    
    HANDLE hFile = CreateFileA(filename, GENERIC_READ,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return NULL;
    }
    
    BYTE* data = new BYTE[outSize];
    DWORD bytesRead = 0;
    BOOL ok = ReadFile(hFile, data, outSize, &bytesRead, NULL);
    CloseHandle(hFile);
    
    if (!ok || bytesRead != outSize)
    {
        delete[] data;
        return NULL;
    }
    
    return data;
}

//------------------------------------------------------------------------------
// Initialize engine
//------------------------------------------------------------------------------
bool XactAudio::initEngine()
{
    HRESULT hr = XACT3CreateEngine(0, &m_engine);
    if (FAILED(hr))
    {
        DbgPrint("XactAudio: Failed to create XACT3 engine\n");
        return false;
    }
    
    XACT_RUNTIME_PARAMETERS xrParams;
    ZeroMemory(&xrParams, sizeof(xrParams));
    xrParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
    
    hr = m_engine->Initialize(&xrParams);
    if (FAILED(hr))
    {
        DbgPrint("XactAudio: Failed to initialize XACT3 engine\n");
        return false;
    }
    
    DbgPrint("XactAudio: XACT3 engine initialized\n");
    return true;
}

//------------------------------------------------------------------------------
// Load SFX bank (STREAMING - same as working example)
//------------------------------------------------------------------------------
bool XactAudio::loadSFXBank(const char* waveBankFile, const char* soundBankFile)
{
    char buf[256];
    
    sprintf(buf, "XactAudio: Loading SFX soundbank: %s\n", soundBankFile);
    DbgPrint(buf);
    
    // Load sound bank data into memory
    DWORD soundBankSize = 0;
    m_sfxSoundBankData = loadFileToMemory(soundBankFile, soundBankSize);
    if (!m_sfxSoundBankData)
    {
        sprintf(buf, "XactAudio: FAILED to load soundbank file!\n");
        DbgPrint(buf);
        return false;
    }
    sprintf(buf, "XactAudio: Soundbank loaded: %d bytes\n", soundBankSize);
    DbgPrint(buf);
    
    sprintf(buf, "XactAudio: Opening wavebank: %s\n", waveBankFile);
    DbgPrint(buf);
    
    // Open wave bank file for streaming (like working example)
    HANDLE hFile = CreateFileA(waveBankFile, GENERIC_READ,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL, OPEN_EXISTING,
                               FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        DWORD err = GetLastError();
        sprintf(buf, "XactAudio: Cannot open wavebank! Error: %d\n", err);
        DbgPrint(buf);
        return false;
    }
    DbgPrint("XactAudio: Wavebank file opened OK\n");
    
    // Create streaming wave bank (like working example)
    XACT_WAVEBANK_STREAMING_PARAMETERS wbParams;
    ZeroMemory(&wbParams, sizeof(wbParams));
    wbParams.file = hFile;
    wbParams.offset = 0;
    wbParams.packetSize = 16;
    wbParams.flags = 0;
    
    HRESULT hr = m_engine->CreateStreamingWaveBank(&wbParams, &m_sfxWaveBank);
    if (FAILED(hr))
    {
        sprintf(buf, "XactAudio: CreateStreamingWaveBank FAILED: 0x%08X\n", hr);
        DbgPrint(buf);
        CloseHandle(hFile);
        return false;
    }
    DbgPrint("XactAudio: CreateStreamingWaveBank OK\n");
    // Don't close hFile - XACT owns it for streaming
    
    // Pump the engine to help wave bank start loading
    for (int i = 0; i < 10; i++)
    {
        m_engine->DoWork();
    }
    
    // Create sound bank
    hr = m_engine->CreateSoundBank(m_sfxSoundBankData, soundBankSize, 0, 0, &m_sfxSoundBank);
    if (FAILED(hr))
    {
        sprintf(buf, "XactAudio: CreateSoundBank FAILED: 0x%08X\n", hr);
        DbgPrint(buf);
        return false;
    }
    DbgPrint("XactAudio: CreateSoundBank OK\n");
    
    // Log number of cues for verification
    XACTINDEX numCues = 0;
    m_sfxSoundBank->GetNumCues(&numCues);
    
    sprintf(buf, "XactAudio: SFX bank has %d cues\n", (int)numCues);
    DbgPrint(buf);
    
    return true;
}

//------------------------------------------------------------------------------
// Load Music bank (STREAMING wave bank for large files)
//------------------------------------------------------------------------------
bool XactAudio::loadMusicBank(const char* waveBankFile, const char* soundBankFile)
{
    // Load sound bank data into memory
    DWORD soundBankSize = 0;
    m_musicSoundBankData = loadFileToMemory(soundBankFile, soundBankSize);
    if (!m_musicSoundBankData)
    {
        char buf[256];
        sprintf(buf, "XactAudio: Failed to load music sound bank: %s\n", soundBankFile);
        DbgPrint(buf);
        return false;
    }
    
    // Open wave bank file for streaming (music.xwb is built as streaming)
    HANDLE hFile = CreateFileA(waveBankFile, GENERIC_READ,
                               FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL, OPEN_EXISTING,
                               FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        char buf[256];
        sprintf(buf, "XactAudio: Failed to open music wave bank: %s\n", waveBankFile);
        DbgPrint(buf);
        return false;
    }
    
    // Create streaming wave bank
    XACT_WAVEBANK_STREAMING_PARAMETERS wbParams;
    ZeroMemory(&wbParams, sizeof(wbParams));
    wbParams.file = hFile;
    wbParams.offset = 0;
    wbParams.packetSize = 16;
    wbParams.flags = 0;
    
    HRESULT hr = m_engine->CreateStreamingWaveBank(&wbParams, &m_musicWaveBank);
    if (FAILED(hr))
    {
        char buf[128];
        sprintf(buf, "XactAudio: Streaming music bank failed (0x%08X), trying in-memory...\n", hr);
        DbgPrint(buf);
        CloseHandle(hFile);
        
        // Fallback: try in-memory
        DWORD waveBankSize = 0;
        BYTE* waveBankData = loadFileToMemory(waveBankFile, waveBankSize);
        if (waveBankData)
        {
            hr = m_engine->CreateInMemoryWaveBank(waveBankData, waveBankSize, 0, 0, &m_musicWaveBank);
            if (SUCCEEDED(hr))
            {
                DbgPrint("XactAudio: Music wave bank loaded as IN-MEMORY\n");
                m_musicWaveBankData = waveBankData;
            }
            else
            {
                delete[] waveBankData;
                DbgPrint("XactAudio: Both streaming and in-memory failed!\n");
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        DbgPrint("XactAudio: Music wave bank loaded as STREAMING\n");
        // Don't close hFile - XACT owns it for streaming
    }
    
    // Pump the engine to help wave bank start loading
    for (int i = 0; i < 10; i++)
    {
        m_engine->DoWork();
    }
    
    // Create sound bank
    hr = m_engine->CreateSoundBank(m_musicSoundBankData, soundBankSize, 0, 0, &m_musicSoundBank);
    if (FAILED(hr))
    {
        DbgPrint("XactAudio: Failed to create music sound bank\n");
        return false;
    }
    
    char buf[256];
    sprintf(buf, "XactAudio: Loaded music banks: %s, %s\n", waveBankFile, soundBankFile);
    DbgPrint(buf);
    
    return true;
}

//------------------------------------------------------------------------------
// Check if SFX bank is ready (streaming needs state check)
//------------------------------------------------------------------------------
bool XactAudio::isSFXBankReady()
{
    if (!m_sfxWaveBank) 
        return false;
    
    DWORD state = 0;
    HRESULT hr = m_sfxWaveBank->GetState(&state);
    if (FAILED(hr))
        return false;
    
    // XACT_WAVEBANKSTATE_PREPARED = 0x02 (NOT XACT_CUESTATE_PREPARED!)
    return (state & XACT_WAVEBANKSTATE_PREPARED) != 0;
}

//------------------------------------------------------------------------------
// Check if music bank is ready (streaming banks need state check)
//------------------------------------------------------------------------------
bool XactAudio::isMusicBankReady()
{
    if (!m_musicWaveBank || !m_musicSoundBank) 
    {
        return false;
    }
    
    // Safely get state
    DWORD state = 0;
    HRESULT hr = m_musicWaveBank->GetState(&state);
    if (FAILED(hr))
    {
        return false;
    }
    
    // Debug: Log state value once
    static bool loggedOnce = false;
    if (!loggedOnce && (state & XACT_WAVEBANKSTATE_PREPARED))
    {
        DbgPrint("XactAudio: Music bank is PREPARED!\n");
        loggedOnce = true;
    }
    
    return (state & XACT_WAVEBANKSTATE_PREPARED) != 0;
}

//------------------------------------------------------------------------------
// Check if music cue is still playing
//------------------------------------------------------------------------------
bool XactAudio::isMusicCuePlaying()
{
    if (!m_currentMusicCue)
    {
        return false;
    }
    
    DWORD state = 0;
    HRESULT hr = m_currentMusicCue->GetState(&state);
    if (FAILED(hr))
    {
        return false;
    }
    
    // XACT_CUESTATE_PLAYING = 0x00000001
    return (state & XACT_CUESTATE_PLAYING) != 0;
}

//------------------------------------------------------------------------------
// Play SFX by cue name
//------------------------------------------------------------------------------
void XactAudio::playSFX(const char* cueName)
{
    if (!g_xaSoundEnabled)
        return;
    if (!cueName || !m_sfxSoundBank || !m_sfxWaveBank)
        return;
    
    // Queue to audio thread instead of playing on main thread
    audioQueueRequest(cueName, false);
}

//------------------------------------------------------------------------------
// Play a sound effect with pitch modification
// Uses the DopplerPitchScalar cue instance variable with BlockPitch RPC
//------------------------------------------------------------------------------
void XactAudio::playSFXWithPitch(const char* cueName, float pitch)
{
    // DEBUG: Just play normally without pitch to test if this is the issue
    playSFX(cueName);
    return;
    
    /* DISABLED FOR DEBUGGING
    // FIX (Bug #11): Check if sound is enabled in options
    if (!g_xaSoundEnabled)
    {
        return;
    }
    
    if (!cueName || !m_sfxSoundBank || !m_sfxWaveBank || !m_engine)
    {
        return;
    }
    
    // Check if wave bank is ready (for streaming)
    if (!isSFXBankReady())
    {
        return;
    }
    
    // Get the cue index
    XACTINDEX cueIndex = m_sfxSoundBank->GetCueIndex(cueName);
    if (cueIndex == XACTINDEX_INVALID)
    {
        return;
    }
    
    // Prepare a cue instance (this allows us to set variables before playing)
    IXACT3Cue* pCue = NULL;
    HRESULT hr = m_sfxSoundBank->Prepare(cueIndex, 0, 0, &pCue);
    if (FAILED(hr) || pCue == NULL)
    {
        // Fall back to normal play if Prepare fails
        m_sfxSoundBank->Play(cueIndex, 0, 0, NULL);
        return;
    }
    
    // Get the DopplerPitchScalar variable index on this cue instance
    XACTVARIABLEINDEX pitchVarIndex = pCue->GetVariableIndex("DopplerPitchScalar");
    if (pitchVarIndex != XACTVARIABLEINDEX_INVALID)
    {
        // Set the pitch value on this cue instance
        pCue->SetVariable(pitchVarIndex, pitch);
    }
    
    // Play the cue
    pCue->Play();
    */
}

//------------------------------------------------------------------------------
// Play music by cue name
//------------------------------------------------------------------------------
void XactAudio::playMusicCue(const char* cueName)
{
    if (!m_musicSoundBank || !isMusicBankReady())
        return;
    
    // Stop current music (safe — just sets a flag)
    if (m_currentMusicCue)
    {
        m_currentMusicCue->Stop(0);
        m_currentMusicCue = NULL;
    }
    
    XACTINDEX index = m_musicSoundBank->GetCueIndex(cueName);
    if (index == XACTINDEX_INVALID)
    {
        DbgPrint("XactAudio: Music cue not found: %s\n", cueName);
        return;
    }
    
    m_musicSoundBank->Play(index, 0, 0, &m_currentMusicCue);
    DbgPrint("XactAudio: Playing music: %s\n", cueName);
}

//------------------------------------------------------------------------------
// Stop music
//------------------------------------------------------------------------------
void XactAudio::stopMusic()
{
    if (m_currentMusicCue)
    {
        m_currentMusicCue->Stop(0);
        m_currentMusicCue = NULL;
    }
}

//------------------------------------------------------------------------------
// Stop ALL audio immediately (music and sound effects)
//------------------------------------------------------------------------------
void XactAudio::stopAllAudio()
{
    // Stop music first
    stopMusic();
    
    // Stop all playing sound effects by stopping the entire SFX sound bank
    if (m_sfxSoundBank)
    {
        // XACTINDEX_INVALID with stop flag stops ALL cues in the bank
        m_sfxSoundBank->Stop(XACTINDEX_INVALID, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
    }
    
    // Also stop music sound bank in case there are lingering cues
    if (m_musicSoundBank)
    {
        m_musicSoundBank->Stop(XACTINDEX_INVALID, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
    }
}

//------------------------------------------------------------------------------
// Update - pump XACT engine
//------------------------------------------------------------------------------
void XactAudio::updateEngine()
{
    // DoWork is now handled by the audio thread on HW thread 5.
    // This function is kept for API compatibility but does nothing.
}

//------------------------------------------------------------------------------
// Static wrapper implementations (for compatibility)
//------------------------------------------------------------------------------
bool XactAudio::init()
{
    DbgPrint("=== XactAudio::init() START ===\n");
    
    if (!g_audio.initEngine())
    {
        DbgPrint("XactAudio: Engine init FAILED\n");
        return false;
    }
    DbgPrint("XactAudio: Engine OK\n");
    
    // Load SFX banks
    if (!g_audio.loadSFXBank("game:\\Content\\resources\\sounds.xwb",
                             "game:\\Content\\resources\\sounds.xsb"))
    {
        DbgPrint("XactAudio: SFX banks FAILED\n");
    }
    else
    {
        DbgPrint("XactAudio: SFX banks OK\n");
    }
    
    // Load music banks
    if (!g_audio.loadMusicBank("game:\\Content\\resources\\music.xwb",
                               "game:\\Content\\resources\\music.xsb"))
    {
        DbgPrint("XactAudio: Music banks FAILED - DISABLING MUSIC\n");
        g_musicDisabled = true;
    }
    else
    {
        DbgPrint("XactAudio: Music banks OK\n");
    }
    
    // Pump engine directly to let streaming banks prepare (audio thread not started yet)
    for (int i = 0; i < 100; i++)
    {
        g_audio.doWork();
    }
    
    // Check if SFX ready
    if (g_audio.isSFXBankReady())
    {
        DbgPrint("XactAudio: SFX bank IS READY\n");
    }
    else
    {
        DbgPrint("XactAudio: SFX bank NOT READY YET\n");
    }
    
    DbgPrint("=== XactAudio::init() END ===\n");
    
    // Start audio thread on HW thread 5
    startAudioThread();
    
    return true;
}

void XactAudio::destroy()
{
    stopAudioThread();
    g_audio.stopMusic();
}

void XactAudio::update()
{
    // Call the full UpdateAudio() which includes music management
    UpdateAudio();
}

void XactAudio::playSound(const char* cueName)
{
    g_audio.playSFX(cueName);
}

void XactAudio::playSoundWithPitch(const char* cueName, float pitch)
{
    // Use the instance method which sets DopplerPitchScalar before playing
    g_audio.playSFXWithPitch(cueName, pitch);
}


void XactAudio::playSound3D(const char* cueName, float x, float y, float z)
{
    // The converted SoundManager bridge does its own listener/volume gating and
    // passes a 2D cue; XACT mixes these. Distance attenuation against the player
    // (the reference's g_minecraft coupling) is intentionally dropped here.
    (void)x; (void)y; (void)z;
    g_audio.playSFX(cueName);
}

bool XactAudio::isAvailable()
{
    return g_audio.isSFXBankReady();
}

//------------------------------------------------------------------------------
// Simple interface functions
//------------------------------------------------------------------------------

bool InitAudio()
{
    return XactAudio::init();
}

void UpdateAudio()
{
    g_audio.updateEngine();
    
    // Skip music if disabled
    if (g_musicDisabled)
        return;
    
    // FIX (Bug #11): Skip music if disabled in options
    if (!g_xaMusicEnabled)
    {
        // Stop any playing music
        if (g_musicPlaying)
        {
            g_audio.stopMusic();
            g_musicPlaying = false;
        }
        return;
    }
    
    // Handle music pending start - pick random track and start playing
    if (g_musicPendingStart && !g_musicPlaying)
    {
        if (g_audio.isMusicBankReady())
        {
            g_musicPendingStart = false;
            g_musicPlaying = true;
            
            // Shuffle: pick a random starting track
            g_currentTrack = rand() % NUM_MUSIC_TRACKS;
            g_audio.playMusicCue(g_MusicCues[g_currentTrack]);
            
            // Wait 60 frames (1 second) before checking if cue finished
            // This gives XACT time to actually start the cue
            g_musicStartupDelay = 60;
            
            char buf[128];
            sprintf(buf, "XactAudio: Starting music playlist with track %d (%s)\n", 
                    g_currentTrack, g_MusicCues[g_currentTrack]);
            DbgPrint(buf);
        }
    }
    
    // Auto-advance music tracks when current cue finishes
    if (g_musicPlaying)
    {
        // Wait for startup delay before checking cue state
        if (g_musicStartupDelay > 0)
        {
            g_musicStartupDelay--;
            return;
        }
        
        // Check if current track has finished playing
        if (!g_audio.isMusicCuePlaying())
        {
            // Shuffle: pick a random next track (but not the same one)
            int nextTrack;
            do {
                nextTrack = rand() % NUM_MUSIC_TRACKS;
            } while (nextTrack == g_currentTrack && NUM_MUSIC_TRACKS > 1);
            
            g_currentTrack = nextTrack;
            g_audio.playMusicCue(g_MusicCues[g_currentTrack]);
            
            // Reset startup delay for new track
            g_musicStartupDelay = 60;
            
            char buf[128];
            sprintf(buf, "XactAudio: Advancing to track %d (%s)\n", 
                    g_currentTrack, g_MusicCues[g_currentTrack]);
            DbgPrint(buf);
        }
    }
}

void ShutdownAudio()
{
    // Destructor handles cleanup
}

//------------------------------------------------------------------------------
// Play step sound based on block type
//------------------------------------------------------------------------------
void PlayStepSound(int blockId)
{
    const char* baseCue = "step_grass";
    
    // Determine sound based on block type
    if (blockId == 2 || blockId == 3 || blockId == 60)
    {
        baseCue = "step_grass";
    }
    else if (blockId == 1 || blockId == 4 || blockId == 7 ||
             blockId == 14 || blockId == 15 || blockId == 16 ||
             blockId == 45 || blockId == 48 || blockId == 49)
    {
        baseCue = "step_stone";
    }
    else if (blockId == 5 || blockId == 17 || blockId == 53 ||
             blockId == 54 || blockId == 58 || blockId == 47)
    {
        baseCue = "step_wood";
    }
    else if (blockId == 12 || blockId == 24)
    {
        baseCue = "step_sand";
    }
    else if (blockId == 13)
    {
        baseCue = "step_gravel";
    }
    else if (blockId == 78 || blockId == 80)
    {
        baseCue = "step_snow";
    }
    // Wool blocks (IDs 21-36)
    else if (blockId >= 21 && blockId <= 36)
    {
        baseCue = "step_cloth";
    }
    
    // Pick random variant (1-4)
    // Cue names: "step_grass", "step_grass 2", "step_grass 3", "step_grass 4"
    int variant = rand() % 4;
    
    char cueName[64];
    if (variant == 0)
    {
        strcpy(cueName, baseCue);
    }
    else
    {
        sprintf(cueName, "%s %d", baseCue, variant + 1);
    }
    
    g_audio.playSFX(cueName);
}

//------------------------------------------------------------------------------
// Play dig sound based on block type
//------------------------------------------------------------------------------
void PlayDigSound(int blockId)
{
    // Same as step sound for now
    PlayStepSound(blockId);
}

//------------------------------------------------------------------------------
// Play sound by cue name
//------------------------------------------------------------------------------
void PlaySound(const char* cueName)
{
    g_audio.playSFX(cueName);
}

//------------------------------------------------------------------------------
// Start music playback
//------------------------------------------------------------------------------
void StartMusic()
{
    if (g_musicDisabled)
        return;
    
    // Only start if not already playing - don't interrupt current music
    if (!g_musicPlaying && !g_musicPendingStart)
    {
        g_musicPendingStart = true;
    }
}

//------------------------------------------------------------------------------
// Restart music with shuffle (for new world generation)
//------------------------------------------------------------------------------
void RestartMusic()
{
    if (g_musicDisabled)
        return;
    
    // Stop current music and reshuffle
    if (g_musicPlaying)
    {
        g_audio.stopMusic();
        g_musicPlaying = false;
        g_musicStartupDelay = 0;
    }
    
    g_musicPendingStart = true;
}

//------------------------------------------------------------------------------
// Stop music playback
//------------------------------------------------------------------------------
void StopMusic()
{
    g_audio.stopMusic();
    g_musicPlaying = false;
    g_musicPendingStart = false;
    g_musicStartupDelay = 0;
}

//------------------------------------------------------------------------------
// Stop ALL audio immediately (music and sound effects)
// Call this when generating a new level to ensure complete silence
//------------------------------------------------------------------------------
void StopAllAudio()
{
    g_audio.stopAllAudio();
    g_musicPlaying = false;
    g_musicPendingStart = false;
    g_musicStartupDelay = 0;
}

//------------------------------------------------------------------------------
// Play UI click sound
//------------------------------------------------------------------------------
void PlayClickSound()
{
    g_audio.playSFX("random_click");
}

//------------------------------------------------------------------------------
// Bridge helpers — used by the AlphaCode converted SoundManager bridge.
// XaPlayCue avoids the winmm PlaySound() name (mmsystem) in the bridge TU.
//------------------------------------------------------------------------------
void XaPlayCue(const char* cue)
{
    g_audio.playSFX(cue);
}

void XaSetSoundEnabled(bool on)
{
    g_xaSoundEnabled = on;
}

void XaSetMusicEnabled(bool on)
{
    g_xaMusicEnabled = on;
}