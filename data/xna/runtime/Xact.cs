// Xact.cs -- XACT (AudioEngine, WaveBank, SoundBank, Cue, AudioCategory) on SoundEffect: MonoGame cannot read an Xbox
// 360's banks (XMA, big-endian), so the port step converts them -- each wave bank's waves to
// ContentConverted/<folder>/<bank name>/stream_NNN.wav, each sound bank's cues to <sound bank>.cues.json
// (formats/xbox/xsb.py) -- and these classes play a cue's wave from there. The game's files use them through
// `using` aliases the port step adds (AudioEngine = XnaPort.Xact.AudioEngine, ...).
//
// A cue plays one of its waves (by its variation: in order, or at random by weight), looping when the bank says so;
// categories set the volume of their cues; Apply3D attenuates by distance (no panning).
using System;
using System.Collections.Generic;
using System.IO;
using System.Text.Json;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;

namespace XnaPort.Xact
{
    public class AudioEngine : IDisposable
    {
        internal readonly Dictionary<string, WaveBank> Banks = new Dictionary<string, WaveBank>(StringComparer.OrdinalIgnoreCase);
        internal readonly Dictionary<string, float> Volumes = new Dictionary<string, float>(StringComparer.OrdinalIgnoreCase);
        internal readonly List<Cue> Playing = new List<Cue>();
        internal readonly string Folder;
        public const int ContentVersion = 46;

        public AudioEngine(string settingsFile) : this(settingsFile, TimeSpan.Zero, "") { }
        public AudioEngine(string settingsFile, TimeSpan lookAheadTime, string rendererId)
        {
            Folder = Path.GetDirectoryName(settingsFile.Replace('\\', '/')) ?? "";
            Runtime.Log("XACT: engine " + settingsFile);
        }

        internal static string Converted(string relative) =>
            Path.Combine(Runtime.BaseDirectory, "ContentConverted", relative.Replace('\\', '/'));

        public AudioCategory GetCategory(string name) => new AudioCategory(this, name);
        public float GetGlobalVariable(string name) => 0;
        public void SetGlobalVariable(string name, float value) { }
        public bool IsDisposed { get; private set; }
        public void Update()
        {
            for (int i = Playing.Count - 1; i >= 0; i--)
                if (Playing[i].IsStopped) Playing.RemoveAt(i);
        }
        public void Dispose() { IsDisposed = true; }
    }

    public struct AudioCategory : IEquatable<AudioCategory>
    {
        readonly AudioEngine engine;
        public string Name { get; }
        internal AudioCategory(AudioEngine e, string name) { engine = e; Name = name; }
        public void SetVolume(float volume)
        {
            if (engine == null) return;
            engine.Volumes[Name] = volume;
            foreach (var c in engine.Playing) c.Refresh();
        }
        public void Pause() { if (engine != null) foreach (var c in engine.Playing) c.Pause(); }
        public void Resume() { if (engine != null) foreach (var c in engine.Playing) c.Resume(); }
        public void Stop(AudioStopOptions options) { if (engine != null) foreach (var c in engine.Playing) c.Stop(options); }
        public bool Equals(AudioCategory other) => Name == other.Name;
        public override bool Equals(object obj) => obj is AudioCategory c && Equals(c);
        public override int GetHashCode() => Name?.GetHashCode() ?? 0;
        public override string ToString() => Name;
        public static bool operator ==(AudioCategory a, AudioCategory b) => a.Equals(b);
        public static bool operator !=(AudioCategory a, AudioCategory b) => !a.Equals(b);
    }

    public class WaveBank : IDisposable
    {
        internal readonly string Folder;
        readonly Dictionary<int, SoundEffect> waves = new Dictionary<int, SoundEffect>();
        public bool IsPrepared => true;
        public bool IsInUse => false;
        public bool IsDisposed { get; private set; }

        public WaveBank(AudioEngine audioEngine, string nonStreamingWaveBankFilename)
        {
            string name = Path.GetFileNameWithoutExtension(nonStreamingWaveBankFilename);
            Folder = AudioEngine.Converted(Path.Combine(Path.GetDirectoryName(nonStreamingWaveBankFilename) ?? "", name));
            audioEngine.Banks[name] = this;
            string bankName = File.Exists(Path.Combine(Folder, "bank.txt")) ? File.ReadAllText(Path.Combine(Folder, "bank.txt")).Trim() : name;
            audioEngine.Banks[bankName] = this;
        }
        public WaveBank(AudioEngine audioEngine, string streamingWaveBankFilename, int offset, short packetsize)
            : this(audioEngine, streamingWaveBankFilename) { }

        internal SoundEffect Wave(int track)
        {
            if (waves.TryGetValue(track, out var s)) return s;
            string p = Path.Combine(Folder, $"stream_{track:D3}.wav");
            if (!File.Exists(p)) { Runtime.Log("XACT: no wave " + p); return waves[track] = null; }
            using (var f = File.OpenRead(p)) s = SoundEffect.FromStream(f);
            return waves[track] = s;
        }
        public void Dispose() { IsDisposed = true; }
    }

    public class SoundBank : IDisposable
    {
        readonly AudioEngine engine;
        internal readonly string[] BankNames;
        internal readonly Dictionary<string, CueInfo> Cues = new Dictionary<string, CueInfo>(StringComparer.OrdinalIgnoreCase);
        public bool IsInUse => false;
        public bool IsDisposed { get; private set; }

        internal sealed class CueInfo
        {
            public List<(int bank, int track)> Waves = new List<(int, int)>();
            public List<int> Weights;
            public int Variation, Loop, Category;
            public int Next;
        }

        public SoundBank(AudioEngine audioEngine, string filename)
        {
            engine = audioEngine;
            string json = AudioEngine.Converted(Path.ChangeExtension(filename, null) + ".cues.json");
            BankNames = new string[0];
            if (!File.Exists(json)) { Runtime.Log("XACT: no cue table " + json); return; }
            using var doc = JsonDocument.Parse(File.ReadAllText(json));
            var banks = new List<string>();
            foreach (var b in doc.RootElement.GetProperty("wave_banks").EnumerateArray()) banks.Add(b.GetString());
            BankNames = banks.ToArray();
            foreach (var c in doc.RootElement.GetProperty("cues").EnumerateObject())
            {
                var info = new CueInfo { Variation = c.Value.GetProperty("variation").GetInt32(), Loop = c.Value.GetProperty("loop").GetInt32(),
                                         Category = c.Value.GetProperty("category").GetInt32() };
                foreach (var w in c.Value.GetProperty("waves").EnumerateArray()) info.Waves.Add((w[0].GetInt32(), w[1].GetInt32()));
                if (c.Value.GetProperty("weights").ValueKind == JsonValueKind.Array)
                {
                    info.Weights = new List<int>();
                    foreach (var w in c.Value.GetProperty("weights").EnumerateArray()) info.Weights.Add(w.GetInt32());
                }
                Cues[c.Name] = info;
            }
        }

        internal SoundEffect Pick(CueInfo info)
        {
            if (info.Waves.Count == 0) return null;
            int i;
            if (info.Variation == 0 || info.Variation == 3) i = info.Next++ % info.Waves.Count;          // ordered
            else if (info.Weights != null && info.Weights.Count == info.Waves.Count)
            {
                int total = 0; foreach (var w in info.Weights) total += Math.Max(w, 1);
                int r = Random.Shared.Next(total); i = 0;
                while (i < info.Waves.Count - 1 && (r -= Math.Max(info.Weights[i], 1)) >= 0) i++;
            }
            else i = Random.Shared.Next(info.Waves.Count);
            var (bank, track) = info.Waves[i];
            string name = bank < BankNames.Length ? BankNames[bank] : "";
            return engine.Banks.TryGetValue(name, out var wb) ? wb.Wave(track) : null;
        }

        public Cue GetCue(string name)
        {
            if (!Cues.TryGetValue(name, out var info)) { Runtime.Log("XACT: no cue " + name); info = new CueInfo(); }
            return new Cue(engine, this, name, info);
        }
        public void PlayCue(string name) => GetCue(name).Play();
        public void PlayCue(string name, AudioListener listener, AudioEmitter emitter)
        {
            var c = GetCue(name);
            c.Apply3D(listener, emitter);
            c.Play();
        }
        public void Dispose() { IsDisposed = true; }
    }

    public class Cue : IDisposable
    {
        readonly AudioEngine engine;
        readonly SoundBank bank;
        readonly SoundBank.CueInfo info;
        SoundEffectInstance instance;
        float distanceGain = 1;
        bool started;
        public string Name { get; }

        internal Cue(AudioEngine e, SoundBank b, string name, SoundBank.CueInfo i) { engine = e; bank = b; Name = name; info = i; }

        public bool IsCreated => true;
        public bool IsPrepared => true;
        public bool IsPreparing => false;
        public bool IsPlaying => instance != null && instance.State == SoundState.Playing;
        public bool IsPaused => instance != null && instance.State == SoundState.Paused;
        public bool IsStopped => started && (instance == null || instance.State == SoundState.Stopped);
        public bool IsStopping => false;
        public bool IsDisposed { get; private set; }

        public void Play()
        {
            var sound = bank.Pick(info);
            started = true;
            if (sound == null) return;
            instance = sound.CreateInstance();
            instance.IsLooped = info.Loop == 255;
            Refresh();
            instance.Play();
            engine.Playing.Add(this);
        }

        internal void Refresh()
        {
            if (instance == null) return;
            float v = 1;
            foreach (var kv in engine.Volumes) v = Math.Min(v, kv.Value);   // categories are not told apart (no .xgs)
            instance.Volume = MathHelper.Clamp(v * distanceGain, 0, 1);
        }

        public void Pause() => instance?.Pause();
        public void Resume() => instance?.Resume();
        public void Stop(AudioStopOptions options) => instance?.Stop(options == AudioStopOptions.Immediate);
        public float GetVariable(string name) => 0;
        public void SetVariable(string name, float value)
        {
            if (string.Equals(name, "Volume", StringComparison.OrdinalIgnoreCase) && instance != null)
                instance.Volume = MathHelper.Clamp(value, 0, 1);
        }
        public void Apply3D(AudioListener listener, AudioEmitter emitter)
        {
            float d = Vector3.Distance(listener.Position, emitter.Position);
            distanceGain = 1f / (1f + d * 0.1f);
            Refresh();
        }
        public void Dispose() { IsDisposed = true; instance?.Dispose(); }
    }
}
