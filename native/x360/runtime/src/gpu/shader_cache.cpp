#include "gpu/shader_cache.h"

#include "gpu/shader_translator.h"
#include "log.h"

#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <utility>
#include <unordered_map>

namespace gpu
{
    namespace
    {
        std::unordered_map<uint64_t, std::unique_ptr<Shader>> g_shaders;

        // 64-bit FNV-1a over the stage and the words.
        uint64_t Hash(ucode::Stage stage, const std::vector<uint32_t>& words)
        {
            uint64_t hash = 0xCBF29CE484222325ull;
            auto mix = [&](uint32_t value) {
                for (int i = 0; i < 4; i++)
                {
                    hash ^= (value >> (8 * i)) & 0xFF;
                    hash *= 0x100000001B3ull;
                }
            };
            mix(uint32_t(stage));
            for (uint32_t word : words)
                mix(word);
            return hash;
        }

        const std::string& DumpDirectory()
        {
            static const std::string directory = [] {
                const char* value = std::getenv("CB_GPU_DUMP_SHADERS");
                if (!value || !value[0] || std::string(value) == "0")
                    return std::string();
                return std::string(value) == "1" ? std::string("analysis/shaders") : std::string(value);
            }();
            return directory;
        }

        void Dump(const Shader& shader, const std::string& directory)
        {
            std::error_code error;
            std::filesystem::create_directories(directory, error);
            const std::string base = std::format("{}/{}_{:016X}", directory, ucode::StageName(shader.stage), shader.hash);
            std::ofstream bin(base + ".bin", std::ios::binary);
            for (uint32_t word : shader.words)
            {
                const uint32_t be = __builtin_bswap32(word); // as in guest memory
                bin.write(reinterpret_cast<const char*>(&be), 4);
            }
            std::ofstream(base + ".txt") << shader.disassembly;
        }
    }

    const Shader* LoadShader(ucode::Stage stage, std::vector<uint32_t> words)
    {
        const uint64_t hash = Hash(stage, words);
        auto it = g_shaders.find(hash);
        if (it != g_shaders.end())
        {
            if (it->second->words != words)
                Fatal("GPU: shader hash collision at {:016X}", hash);
            return it->second.get();
        }

        auto shader = std::make_unique<Shader>();
        shader->stage = stage;
        shader->hash = hash;
        shader->words = std::move(words);
        shader->disassembly = ucode::Disassemble(stage, shader->words, shader->analysis);
        if (!DumpDirectory().empty())
            Dump(*shader, DumpDirectory());
        if (!shader->analysis.errors.empty())
        {
            const std::string directory = DumpDirectory().empty() ? std::string("analysis/shaders") : DumpDirectory();
            Dump(*shader, directory);
            Fatal("GPU: {} shader {:016X} has undefined microcode: {} (dumped to {})",
                  ucode::StageName(stage), hash, shader->analysis.errors.front(), directory);
        }
        return g_shaders.emplace(hash, std::move(shader)).first->second.get();
    }

    size_t ShaderCount()
    {
        return g_shaders.size();
    }

    namespace
    {
        // Translation + compile of one shader variant (a worker or the caller).
        HostShader Build(const Shader& shader, const TranslationOptions& options);

        // Compiled shaders by (hash, options). An entry exists from the first request on; `ready` once built.
        // CB_ASYNC_SHADERS=1: shaders compile on worker threads and a draw whose shader is not ready is skipped: FXC
        // takes up to seconds for a large translated shader, and a first-seen shader stalled the command processor
        //. Off by default: a skipped draw left the boot animation's
        // first swoosh frames as horizontal stripes for a frame or two. The
        // disk caches (shaders, pipelines) keep the stalls to a first run.
        struct Entry
        {
            std::atomic<bool> ready = false;
            std::atomic<bool> claimed = false; // a worker or a waiting draw is building it
            HostShader host;
        };
        std::mutex g_compiledMutex;
        std::condition_variable g_built;
        std::map<std::pair<uint64_t, TranslationOptions>, std::unique_ptr<Entry>> g_compiled;
        struct Job { const Shader* shader; TranslationOptions options; Entry* entry; };
        std::deque<Job> g_jobs;
        std::condition_variable g_jobAdded;

        bool AsyncShaders()
        {
            static const bool on = [] {
                const char* v = std::getenv("CB_ASYNC_SHADERS");
                return v && std::string(v) == "1";
            }();
            return on;
        }

        void Worker()
        {
            for (;;)
            {
                Job job;
                {
                    std::unique_lock lock(g_compiledMutex);
                    g_jobAdded.wait(lock, [] { return !g_jobs.empty(); });
                    job = g_jobs.front();
                    g_jobs.pop_front();
                }
                if (job.entry->claimed.exchange(true))
                    continue; // a draw needed it first and builds it itself
                HostShader host = Build(*job.shader, job.options);
                {
                    std::lock_guard lock(g_compiledMutex);
                    job.entry->host = std::move(host);
                    job.entry->ready = true;
                }
                g_built.notify_all();
            }
        }

        void StartWorkers()
        {
            static std::once_flag once;
            std::call_once(once, [] {
                // Leave the guest's busiest threads and the GPU threads their cores.
                const unsigned n = std::clamp(std::thread::hardware_concurrency() / 3, 1u, 4u);
                for (unsigned i = 0; i < n; i++)
                    std::thread(Worker).detach();
                Log("GPU: {} shader compile worker(s)", n);
            });
        }

        // shader_cache/manifest.bin: "MCSM", sizeof(TranslationOptions), then per entry: stage (u32), word count (u32),
        // the words, the options (raw).
        constexpr uint32_t kManifestMagic = 0x4D53434D; // "MCSM"
        bool ManifestEnabled()
        {
            static const bool on = [] { const char* v = std::getenv("CB_SHADER_CACHE"); return !(v && std::string(v) == "0"); }();
            return on;
        }
        const std::filesystem::path kManifestPath = std::filesystem::path("shader_cache") / "manifest.bin";
        std::ofstream g_manifest;
        std::vector<Entry*> g_prefetched; // the entries PrefetchShaders queued

        void AppendManifest(const Shader& shader, const TranslationOptions& options)
        {
            if (!ManifestEnabled())
                return;
            if (!g_manifest.is_open())
            {
                std::error_code error;
                std::filesystem::create_directories(kManifestPath.parent_path(), error);
                const bool fresh = !std::filesystem::exists(kManifestPath, error);
                g_manifest.open(kManifestPath, std::ios::binary | std::ios::app);
                if (fresh)
                {
                    const uint32_t header[2] = { kManifestMagic, uint32_t(sizeof(TranslationOptions)) };
                    g_manifest.write(reinterpret_cast<const char*>(header), sizeof(header));
                }
            }
            const uint32_t head[2] = { uint32_t(shader.stage), uint32_t(shader.words.size()) };
            g_manifest.write(reinterpret_cast<const char*>(head), sizeof(head));
            g_manifest.write(reinterpret_cast<const char*>(shader.words.data()), std::streamsize(shader.words.size() * 4));
            g_manifest.write(reinterpret_cast<const char*>(&options), sizeof(options));
            g_manifest.flush();
        }

        // The entry for (shader, options): created when new, and queued for the workers in async mode or for a
        // prefetch. Call with g_compiledMutex held.
        Entry& Request(const Shader& shader, const TranslationOptions& options, bool& created, bool prefetch = false)
        {
            auto& slot = g_compiled[std::make_pair(shader.hash, options)];
            created = !slot;
            if (created)
            {
                slot = std::make_unique<Entry>();
                if (!prefetch)
                    AppendManifest(shader, options);
                if (AsyncShaders() || prefetch)
                {
                    StartWorkers();
                    g_jobs.push_back({ &shader, options, slot.get() });
                    g_jobAdded.notify_one();
                }
            }
            return *slot;
        }
    }

    const HostShader* TryCompiledShader(const Shader& shader, const TranslationOptions& options)
    {
        if (!AsyncShaders())
            return &CompiledShader(shader, options);
        std::lock_guard lock(g_compiledMutex);
        bool created = false;
        Entry& e = Request(shader, options, created);
        return e.ready ? &e.host : nullptr;
    }

    void RequestCompiledShader(const Shader& shader, const TranslationOptions& options)
    {
        if (!AsyncShaders())
        {
            CompiledShader(shader, options);
            return;
        }
        std::lock_guard lock(g_compiledMutex);
        bool created = false;
        Request(shader, options, created);
    }

    const HostShader& CompiledShader(const Shader& shader, const TranslationOptions& options)
    {
        std::unique_lock lock(g_compiledMutex);
        bool created = false;
        Entry& e = Request(shader, options, created);
        if (e.ready)
            return e.host;
        if (e.claimed.exchange(true))
        {
            g_built.wait(lock, [&] { return e.ready.load(); }); // a worker is building it
            return e.host;
        }
        // Not started (new, or still queued by a prefetch): built here at once.
        lock.unlock();
        HostShader host = Build(shader, options);
        lock.lock();
        e.host = std::move(host);
        e.ready = true;
        g_built.notify_all();
        return e.host;
    }

    namespace
    {
    HostShader Build(const Shader& shader, const TranslationOptions& options)
    {
        TranslatedShader translated = TranslateShader(shader, options);
        std::vector<uint8_t> bytecode;
        std::string errors;
        const char* target = shader.stage == ucode::Stage::kVertex ? "vs_5_1" : "ps_5_1";
        const std::string name = std::format("{}_{:016X}", ucode::StageName(shader.stage), shader.hash);
        const bool ok = CompileHlsl(translated.hlsl, target, name.c_str(), bytecode, errors);
        if (!ok || !DumpDirectory().empty())
        {
            const std::string directory = DumpDirectory().empty() ? std::string("analysis/shaders") : DumpDirectory();
            std::error_code error;
            std::filesystem::create_directories(directory, error);
            std::ofstream(std::format("{}/{}.hlsl", directory, name)) << translated.hlsl;
        }
        if (!ok)
            Fatal("GPU: {} failed to compile (HLSL dumped next to the shader dumps): {}", name, errors);
        return HostShader{ std::move(bytecode), std::move(translated.textures), std::move(translated.samplers) };
    }
    }

    void PrefetchShaders()
    {
        if (!ManifestEnabled())
            return;
        std::ifstream in(kManifestPath, std::ios::binary);
        uint32_t header[2] = {};
        if (!in.read(reinterpret_cast<char*>(header), sizeof(header)))
            return;
        if (header[0] != kManifestMagic || header[1] != sizeof(TranslationOptions))
        {
            in.close();
            std::error_code error;
            std::filesystem::remove(kManifestPath, error); // another layout: started again
            Log("GPU: shader manifest from another build discarded");
            return;
        }
        size_t queued = 0;
        for (;;)
        {
            uint32_t head[2];
            if (!in.read(reinterpret_cast<char*>(head), sizeof(head)) || head[0] > 1 || head[1] == 0 || head[1] > 65536)
                break;
            std::vector<uint32_t> words(head[1]);
            TranslationOptions options;
            if (!in.read(reinterpret_cast<char*>(words.data()), std::streamsize(words.size() * 4)) ||
                !in.read(reinterpret_cast<char*>(&options), sizeof(options)))
                break;
            const Shader* shader = LoadShader(ucode::Stage(head[0]), std::move(words));
            std::lock_guard lock(g_compiledMutex);
            bool created = false;
            Entry& e = Request(*shader, options, created, true);
            if (created)
                g_prefetched.push_back(&e);
            queued += created;
        }
        Log("GPU: {} shader(s) from the manifest queued for the compile workers", queued);
    }

    void WaitForPrefetchedShaders()
    {
        if (g_prefetched.empty())
            return;
        const auto start = std::chrono::steady_clock::now();
        std::unique_lock lock(g_compiledMutex);
        g_built.wait(lock, [] {
            return std::all_of(g_prefetched.begin(), g_prefetched.end(), [](const Entry* e) { return e->ready.load(); });
        });
        Log("GPU: {} prefetched shader(s) ready after {} ms", g_prefetched.size(),
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count());
        g_prefetched.clear();
    }

    size_t CompiledShaderCount()
    {
        std::lock_guard lock(g_compiledMutex);
        return g_compiled.size();
    }
}
