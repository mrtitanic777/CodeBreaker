#include "dotnet/decompiler/csharp/project_decompiler/i_project_info_provider.hpp"

namespace cb::dotnet::decompiler::csharp::project_decompiler {

std::string language_version_text(LanguageVersion version) {
    // LanguageVersion.ToString(): the member's name; "CSharp" is cut and '_' made '.'
    switch (version) {
    case LanguageVersion::CSharp1: return "1";
    case LanguageVersion::CSharp2: return "2";
    case LanguageVersion::CSharp3: return "3";
    case LanguageVersion::CSharp4: return "4";
    case LanguageVersion::CSharp5: return "5";
    case LanguageVersion::CSharp6: return "6";
    case LanguageVersion::CSharp7: return "7";
    case LanguageVersion::CSharp7_1: return "7.1";
    case LanguageVersion::CSharp7_2: return "7.2";
    case LanguageVersion::CSharp7_3: return "7.3";
    case LanguageVersion::CSharp8_0: return "8.0";
    case LanguageVersion::CSharp9_0: return "9.0";
    case LanguageVersion::CSharp10_0: return "10.0";
    case LanguageVersion::CSharp11_0: return "11.0";
    case LanguageVersion::CSharp12_0: return "12.0";
    case LanguageVersion::CSharp13_0: return "13.0";
    case LanguageVersion::CSharp14_0: return "14.0";
    case LanguageVersion::CSharp15_0: return "15.0";   // (Preview has the same value)
    case LanguageVersion::Latest: return "Latest";
    }
    // a number that is no member: Enum.ToString writes the number
    return std::to_string(static_cast<int>(version));
}

}  // namespace cb::dotnet::decompiler::csharp::project_decompiler
