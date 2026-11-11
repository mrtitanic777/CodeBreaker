// cb/source/vxd/ddk.hpp -- where the Windows 95 driver development kit is, for the VxD source recovery.
//
// Three things in the DDK name what a driver's raw disassembly can only call sub_XXXX: the control messages in
// INC32/VMM.INC (symbols.hpp), the service tables in INC32/*.INC (services.hpp), and the debug symbol files in
// DEBUG/*.SYM (symbols.hpp). The DDK is part of the period reference folder (the corpus's "reference" setting); a few
// older places it may have been unpacked to are tried as well.
#pragma once

#include <filesystem>
#include <optional>
#include <vector>

#include "cb/base/corpus.hpp"

namespace cb::source::vxd {

struct DdkFolders {
    std::vector<std::filesystem::path> inc;     // where INC32 (VMM.INC and the other includes) may be, best first
    std::vector<std::filesystem::path> debug;   // where the DDK's DEBUG folder (the .SYM files) may be, best first

    // The reference folder's DDK, the sibling Windows95 repository's, and the folders the DDK was once unpacked to
    // under `checkout_parent` (the folder the CodeBreaker checkout is in).
    static DdkFolders standard(const base::Corpus& corpus, const std::filesystem::path& checkout_parent);

    // The first of the candidates that exists, with `leaf` under it (the candidate itself when `leaf` is empty).
    static std::optional<std::filesystem::path> first_existing(const std::vector<std::filesystem::path>& candidates,
                                                               const std::filesystem::path& leaf = {});
};

}  // namespace cb::source::vxd
