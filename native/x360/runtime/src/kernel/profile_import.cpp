// Console profiles from the `profiles` folder (kernel/profile_import.h).

#include "kernel/profile_import.h"

#include "log.h"
#include "settings.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <format>
#include <fstream>
#include <map>
#include <optional>
#include <string>

namespace fs = std::filesystem;

namespace kernel::profile
{
    namespace
    {
        constexpr uint32_t kProfileContentType = 0x00010000;

        uint32_t Be32(const std::vector<uint8_t>& d, size_t at)
        {
            return uint32_t(d[at]) << 24 | uint32_t(d[at + 1]) << 16 | uint32_t(d[at + 2]) << 8 | d[at + 3];
        }

        // An STFS package (XContent "CON " / "LIVE" / "PIRS"), read-only and read-write ones. Layout (Xenia's
        // StfsContainerDevice, BSD-3, was read for it; tools/scripts/stfs_extract.py is the same reader): the
        // XContent header ends at the big-endian u32 at 0x340, rounded up to 4 KiB; the volume descriptor is at
        // 0x379. Data blocks are 4 KiB; every 170 data blocks follow a level-0 hash table (one block, or two copies
        // when the package is read-write), every 170 * 170 a level-1 table, and so on. A hash entry (24 bytes) ends
        // with a u32: low 24 bits = the file's next block, bit 30 = which copy of the table below is current. The
        // file table is a chain of blocks of 64 directory entries (64 bytes each).
        class Stfs
        {
        public:
            struct Entry
            {
                std::string name;
                bool directory = false, contiguous = false;
                uint32_t start = 0, length = 0;
                int16_t parent = -1;
            };

            explicit Stfs(std::vector<uint8_t> data) : data_(std::move(data)) {}

            bool Open(std::string& error)
            {
                if (data_.size() < 0x3A0 || (std::memcmp(data_.data(), "CON ", 4) != 0 &&
                                             std::memcmp(data_.data(), "LIVE", 4) != 0 &&
                                             std::memcmp(data_.data(), "PIRS", 4) != 0))
                {
                    error = "not an STFS package";
                    return false;
                }
                base_ = (size_t(Be32(data_, 0x340)) + 0xFFF) & ~size_t(0xFFF);
                const uint8_t* d = &data_[0x379];
                if (d[0] != 0x24)
                {
                    error = std::format("not an STFS volume descriptor ({:02X})", d[0]);
                    return false;
                }
                perTable_ = (d[2] & 1) ? 1 : 2;
                rootActive_ = (d[2] & 2) != 0;
                tableBlocks_ = uint32_t(d[3] | d[4] << 8);
                tableBlock_ = uint32_t(d[5] | d[6] << 8 | d[7] << 16);
                totalBlocks_ = Be32(data_, 0x379 + 0x1C);
                return true;
            }

            std::vector<Entry> Entries(std::string& error) const
            {
                std::vector<Entry> out;
                uint32_t block = tableBlock_;
                for (uint32_t n = 0; n < tableBlocks_; n++)
                {
                    const size_t at = BlockOffset(block);
                    if (at + 4096 > data_.size())
                    {
                        error = "file table outside the package";
                        return {};
                    }
                    for (size_t i = 0; i < 64; i++)
                    {
                        const uint8_t* e = &data_[at + i * 64];
                        if (e[0] == 0)
                            break;
                        Entry entry;
                        entry.name.assign(reinterpret_cast<const char*>(e), e[40] & 0x3F);
                        entry.directory = (e[40] & 0x80) != 0;
                        entry.contiguous = (e[40] & 0x40) != 0;
                        entry.start = uint32_t(e[47] | e[48] << 8 | e[49] << 16);
                        entry.parent = int16_t(e[50] << 8 | e[51]);
                        entry.length = uint32_t(e[52]) << 24 | uint32_t(e[53]) << 16 | uint32_t(e[54]) << 8 | e[55];
                        out.push_back(entry);
                    }
                    block = NextBlock(block);
                    if (block == kEndOfChain)
                        break;
                }
                return out;
            }

            std::optional<std::vector<uint8_t>> Read(const Entry& entry) const
            {
                std::vector<uint8_t> out;
                uint32_t block = entry.start;
                size_t remaining = entry.length;
                while (remaining > 0)
                {
                    const size_t at = BlockOffset(block);
                    const size_t take = std::min<size_t>(4096, remaining);
                    if (block == kEndOfChain || at + take > data_.size())
                        return std::nullopt;
                    out.insert(out.end(), data_.begin() + std::ptrdiff_t(at), data_.begin() + std::ptrdiff_t(at + take));
                    remaining -= take;
                    block = entry.contiguous ? block + 1 : NextBlock(block);
                }
                return out;
            }

        private:
            static constexpr uint32_t kEndOfChain = 0xFFFFFF;
            static constexpr uint32_t kPerLevel[3] = { 170, 28900, 4913000 };

            size_t BlockOffset(uint32_t index) const
            {
                uint64_t block = index;
                for (uint32_t base : kPerLevel)
                {
                    block += uint64_t((index + base) / base) * perTable_;
                    if (index < base)
                        break;
                }
                return base_ + size_t(block << 12);
            }

            // Block number of the level-`level` hash table covering data block `index` (its first copy).
            uint32_t HashBlockNumber(uint32_t index, int level) const
            {
                const uint32_t step0 = kPerLevel[0] + perTable_;
                const uint32_t step1 = kPerLevel[1] + (kPerLevel[0] + 1) * perTable_;
                const uint32_t steps[2] = { step0, step1 };
                if (level == 2)
                    return step1;
                if (index < kPerLevel[level])
                    return level == 0 ? 0 : steps[level - 1];
                uint32_t block = (index / kPerLevel[level]) * steps[level];
                if (level == 0)
                {
                    block += (index / kPerLevel[1] + 1) * perTable_;
                    if (index < kPerLevel[1])
                        return block;
                }
                return block + perTable_;
            }

            uint32_t NextBlock(uint32_t index) const
            {
                uint32_t second = 0;
                int top = 0;
                if (perTable_ == 2)
                {
                    second = rootActive_ ? 1 : 0;
                    for (int level = 0; level < 3; level++)
                        if (totalBlocks_ < kPerLevel[level])
                        {
                            top = level;
                            break;
                        }
                }
                for (int level = top; level >= 0; level--)
                {
                    const size_t table = base_ + (size_t(HashBlockNumber(index, level) + second) << 12);
                    const uint32_t record = level == 0 ? index % kPerLevel[0] : (index / kPerLevel[level - 1]) % kPerLevel[0];
                    if (table + record * 24 + 24 > data_.size())
                        return kEndOfChain;
                    const uint32_t info = Be32(data_, table + record * 24 + 20);
                    if (level == 0)
                        return info & 0xFFFFFF;
                    second = (info >> 30) & 1;
                }
                return kEndOfChain;
            }

            std::vector<uint8_t> data_;
            size_t base_ = 0;
            uint32_t perTable_ = 1;
            bool rootActive_ = false;
            uint32_t tableBlocks_ = 0, tableBlock_ = 0, totalBlocks_ = 0;
        };

        std::vector<uint8_t> ReadFile(const fs::path& path)
        {
            std::ifstream in(path, std::ios::binary);
            return std::vector<uint8_t>(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
        }

        fs::path ProfileDirectory(uint64_t id)
        {
            const std::string name = std::format("{:016X}", id);
            return DataDirectory() / "content" / name / "FFFE07D1" / "00010000" / name;
        }

        constexpr size_t kHeaderFileSize = 0xA000, kHeaderFileNameField = 0x971A, kThumbnailMax = 0x3D00;

        fs::path ProfileHeaderPath(uint64_t id)
        {
            const std::string name = std::format("{:016X}", id);
            return DataDirectory() / "content" / name / "FFFE07D1" / "Headers" / "00010000" / (name + ".header");
        }

        // Writes the .header file: the package's header and metadata (0 .. 0x971A), then the file name (the ID).
        bool WriteProfileHeader(uint64_t id, std::vector<uint8_t> h)
        {
            h.resize(kHeaderFileSize);
            std::fill(h.begin() + kHeaderFileNameField, h.end(), uint8_t(0));
            const std::string name = std::format("{:016X}", id);
            std::memcpy(h.data() + kHeaderFileNameField, name.data(), name.size());
            const fs::path path = ProfileHeaderPath(id);
            std::error_code ec;
            fs::create_directories(path.parent_path(), ec);
            std::ofstream out(path, std::ios::binary | std::ios::trunc);
            out.write(reinterpret_cast<const char*>(h.data()), std::streamsize(h.size()));
            return bool(out);
        }

        // The first bytes of a console profile package and its profile ID, or nullopt for other files.
        std::optional<std::vector<uint8_t>> PackageHeader(const fs::path& path, uint64_t& id)
        {
            std::ifstream in(path, std::ios::binary);
            std::vector<uint8_t> h(kHeaderFileNameField);
            in.read(reinterpret_cast<char*>(h.data()), std::streamsize(h.size()));
            if (in.gcount() != std::streamsize(h.size()) || std::memcmp(h.data(), "CON ", 4) != 0 ||
                Be32(h, 0x344) != kProfileContentType)
                return std::nullopt;
            id = 0;
            for (int i = 0; i < 8; i++)
                id = id << 8 | h[0x371 + i];
            return h;
        }

        // The files of a profile package (relative path -> contents) and its profile ID (the header's console-side
        // profile ID at 0x371 names the package's owner: for a profile package, the profile itself).
        bool FilesFromPackage(const fs::path& path, uint64_t& id, std::map<std::string, std::vector<uint8_t>>& files,
                              std::string& error)
        {
            std::vector<uint8_t> data = ReadFile(path);
            if (data.size() < 0x3A0 || std::memcmp(data.data(), "CON ", 4) != 0)
            {
                error = "not a console (CON) package";
                return false;
            }
            if (Be32(data, 0x344) != kProfileContentType)
            {
                error = std::format("content type {:08X} is not a profile", Be32(data, 0x344));
                return false;
            }
            id = 0;
            for (size_t i = 0; i < 8; i++)
                id = id << 8 | data[0x371 + i];
            Stfs stfs(std::move(data));
            if (!stfs.Open(error))
                return false;
            const std::vector<Stfs::Entry> entries = stfs.Entries(error);
            if (entries.empty())
            {
                error = error.empty() ? "empty package" : error;
                return false;
            }
            for (const Stfs::Entry& e : entries)
            {
                if (e.directory)
                    continue;
                std::string name = e.name;
                for (int16_t p = e.parent, guard = 0; p >= 0 && size_t(p) < entries.size() && guard < 64; guard++)
                {
                    name = entries[size_t(p)].name + "/" + name;
                    p = entries[size_t(p)].parent;
                }
                auto contents = stfs.Read(e);
                if (!contents)
                {
                    error = name + ": its block chain ends early";
                    return false;
                }
                files[name] = std::move(*contents);
            }
            return true;
        }

        std::optional<uint64_t> HexId(const std::string& name)
        {
            if (name.size() != 16 || name.find_first_not_of("0123456789ABCDEFabcdef") != std::string::npos)
                return std::nullopt;
            return std::stoull(name, nullptr, 16);
        }

        // A Xenia-layout profile: `dir` is <ID>, holding FFFE07D1/00010000/<ID>/Account.
        std::optional<fs::path> XeniaProfile(const fs::path& dir, uint64_t& id)
        {
            const auto hex = HexId(dir.filename().string());
            if (!hex)
                return std::nullopt;
            const fs::path inner = dir / "FFFE07D1" / "00010000" / dir.filename();
            std::error_code ec;
            if (!fs::is_regular_file(inner / "Account", ec))
                return std::nullopt;
            id = *hex;
            return inner;
        }

        bool WriteFiles(uint64_t id, const std::map<std::string, std::vector<uint8_t>>& files, std::string& error)
        {
            const fs::path target = ProfileDirectory(id);
            for (const auto& [name, contents] : files)
            {
                const fs::path out = target / fs::path(name);
                std::error_code ec;
                fs::create_directories(out.parent_path(), ec);
                std::ofstream file(out, std::ios::binary);
                file.write(reinterpret_cast<const char*>(contents.data()), std::streamsize(contents.size()));
                if (!file)
                {
                    error = "cannot write " + out.string();
                    return false;
                }
            }
            return true;
        }
    }

    namespace
    {
        struct DeletedProfile
        {
            uint64_t id = 0;
            int64_t time = 0; // fs::file_time_type ticks
            std::string gamertag;
        };

        fs::path DeletedFile()
        {
            return DataDirectory() / "deleted_profiles.txt";
        }

        std::vector<DeletedProfile> ReadDeleted()
        {
            std::vector<DeletedProfile> list;
            std::ifstream file(DeletedFile());
            std::string line;
            while (std::getline(file, line))
            {
                DeletedProfile d;
                size_t used = 0, used2 = 0;
                try
                {
                    d.id = std::stoull(line, &used, 16);
                    d.time = std::stoll(line.substr(used), &used2);
                }
                catch (const std::exception&)
                {
                    continue;
                }
                const size_t name = used + used2 + 1;
                if (name < line.size())
                    d.gamertag = line.substr(name);
                list.push_back(std::move(d));
            }
            return list;
        }
    }

    void RecordDeleted(uint64_t id, const std::string& gamertag)
    {
        std::ofstream file(DeletedFile(), std::ios::app);
        file << std::format("{:016X} {} {}", id, fs::file_time_type::clock::now().time_since_epoch().count(),
                            gamertag) << '\n';
        if (!file)
            Log("Profiles: cannot write {}", DeletedFile().string());
    }

    bool IsDeleted(uint64_t id, fs::file_time_type source)
    {
        for (const DeletedProfile& d : ReadDeleted())
            if (d.id == id && source.time_since_epoch().count() <= d.time)
                return true;
        return false;
    }

    bool IsDeletedGamertag(const std::string& gamertag)
    {
        auto same = [](const std::string& a, const std::string& b) {
            return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin(), [](char x, char y) {
                return std::tolower(uint8_t(x)) == std::tolower(uint8_t(y));
            });
        };
        for (const DeletedProfile& d : ReadDeleted())
            if (!d.gamertag.empty() && same(d.gamertag, gamertag))
                return true;
        return false;
    }

    fs::path ImportFolder()
    {
        return DataDirectory() / "profiles";
    }

    std::vector<uint64_t> ImportNewProfiles()
    {
        std::vector<uint64_t> imported;
        const fs::path folder = ImportFolder();
        std::error_code ec;
        if (!fs::is_directory(folder, ec))
        {
            fs::create_directories(folder, ec);
            Log("Profiles: put console profiles in {} (profile packages or Xenia profile folders)", folder.string());
            return imported;
        }
        auto exists = [](uint64_t id) {
            std::error_code e;
            return fs::is_regular_file(ProfileDirectory(id) / "Account", e);
        };
        for (auto it = fs::recursive_directory_iterator(folder, fs::directory_options::skip_permission_denied, ec);
             it != fs::recursive_directory_iterator(); it.increment(ec))
        {
            if (ec)
                break;
            const fs::path path = it->path();
            uint64_t id = 0;
            std::map<std::string, std::vector<uint8_t>> files;
            std::string error;
            if (it->is_directory(ec))
            {
                const auto inner = XeniaProfile(path, id);
                if (!inner)
                    continue;
                it.disable_recursion_pending();
                if (exists(id) || std::find(imported.begin(), imported.end(), id) != imported.end())
                    continue;
                if (IsDeleted(id, fs::last_write_time(*inner / "Account", ec)))
                {
                    Log("Profiles: {} skipped: profile {:016X} was deleted on this console", path.string(), id);
                    continue;
                }
                for (const auto& f : fs::recursive_directory_iterator(*inner, ec))
                    if (f.is_regular_file(ec))
                        files[fs::relative(f.path(), *inner, ec).generic_string()] = ReadFile(f.path());
            }
            else if (it->is_regular_file(ec) && it->file_size(ec) >= 0x3A0)
            {
                char magic[4] = {};
                std::ifstream(path, std::ios::binary).read(magic, 4);
                if (std::memcmp(magic, "CON ", 4) != 0)
                    continue;
                if (!FilesFromPackage(path, id, files, error))
                {
                    Log("Profiles: {} skipped: {}", path.string(), error);
                    continue;
                }
                if (exists(id) || std::find(imported.begin(), imported.end(), id) != imported.end())
                    continue;
                if (IsDeleted(id, fs::last_write_time(path, ec)))
                {
                    Log("Profiles: {} skipped: profile {:016X} was deleted on this console", path.string(), id);
                    continue;
                }
            }
            else
                continue;
            if (!files.count("Account"))
            {
                Log("Profiles: {} skipped: the profile has no Account file", path.string());
                continue;
            }
            if (!WriteFiles(id, files, error))
            {
                Log("Profiles: {}: {}", path.string(), error);
                continue;
            }
            uint64_t headerId = 0;
            if (const auto h = PackageHeader(path, headerId); h && headerId == id)
                WriteProfileHeader(id, *h);
            Log("Profiles: {:016X} copied from {} ({} files{})", id, path.string(), files.size(),
                files.count("tile_64.png") ? "" : ", no gamer picture");
            imported.push_back(id);
        }
        return imported;
    }

    void EnsureProfileHeaders()
    {
        const fs::path content = DataDirectory() / "content";
        std::error_code ec;
        std::vector<uint64_t> missing;
        for (const auto& owner : fs::directory_iterator(content, ec))
        {
            const std::string name = owner.path().filename().string();
            if (name.size() != 16 || name.find_first_not_of("0123456789ABCDEFabcdef") != std::string::npos)
                continue;
            const uint64_t id = std::stoull(name, nullptr, 16);
            if (id != 0 && fs::is_regular_file(ProfileDirectory(id) / "Account", ec) &&
                !fs::exists(ProfileHeaderPath(id), ec))
                missing.push_back(id);
        }
        if (missing.empty())
            return;
        // Their console packages, if the profiles folder still has them.
        std::map<uint64_t, std::vector<uint8_t>> packages;
        for (auto it = fs::recursive_directory_iterator(ImportFolder(), fs::directory_options::skip_permission_denied, ec);
             it != fs::recursive_directory_iterator(); it.increment(ec))
        {
            if (ec)
                break;
            uint64_t id = 0;
            if (it->is_regular_file(ec) && it->file_size(ec) >= kHeaderFileNameField)
                if (auto h = PackageHeader(it->path(), id))
                    packages.emplace(id, std::move(*h));
        }
        for (uint64_t id : missing)
        {
            std::vector<uint8_t> h;
            const bool fromPackage = packages.count(id) != 0;
            if (fromPackage)
                h = packages[id];
            else
            {
                h.assign(kHeaderFileNameField, 0);
                auto put32 = [&h](size_t at, uint32_t v) {
                    for (int b = 0; b < 4; b++)
                        h[at + b] = uint8_t(v >> (24 - 8 * b));
                };
                std::memcpy(h.data(), "CON ", 4);
                put32(0x340, 0xAD0E);                          // header size, as console packages
                put32(0x344, kProfileContentType);
                put32(0x348, 2);                               // metadata version
                put32(0x360, 0xFFFE07D1);                      // the dashboard's title ID
                for (int i = 0; i < 8; i++)
                    h[0x371 + i] = uint8_t(id >> (56 - 8 * i)); // profile ID
                const std::string text = std::format("{:016X}", id);
                for (size_t i = 0; i < text.size(); i++)
                    h[0x411 + i * 2 + 1] = uint8_t(text[i]);    // display name (English): the ID, as consoles write
                const std::vector<uint8_t> picture = ReadFile(ProfileDirectory(id) / "tile_64.png");
                if (!picture.empty() && picture.size() <= kThumbnailMax)
                {
                    put32(0x1712, uint32_t(picture.size()));
                    std::copy(picture.begin(), picture.end(), h.begin() + 0x171A);
                }
            }
            if (WriteProfileHeader(id, std::move(h)))
                Log("Profiles: {:016X}: package header {}", id, fromPackage ? "from its console package" : "made");
        }
    }
}
