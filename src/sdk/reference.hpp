// sdk/reference.hpp -- the SDK's reference, made from the comments of include/cb (docs/sdk/reference).
//
//     auto headers = reference::read(checkout / "include" / "cb");           // every documented declaration, header by header
//     auto calls = reference::uses(headers, {{"samples", checkout / "samples"}}); // which files call each function
//     for (auto& [file, text] : reference::pages(headers, samples, tests)) ...     // README.md, <header>.md, functions/<name>.md
//
// A declaration is documented by the /** comment just above it:
//
//     /** cb_binary_open: open a file and read what it is.          name: summary (the name must be the declaration's)
//      * More about it, as plain lines.                               the description
//      * @param path the file, UTF-8.                                one per parameter (@field: one per member of a record)
//      * @return CB_OK; CB_ERROR_NOT_FOUND when ...                   what it returns and each error
//      * @ownership The caller owns the binary.                       who frees what it gives
//      * @threads Any thread.                                         thread safety
//      * @since 0.1                                                   the version it came in (required)
//      */
//
// A line that starts with two more spaces goes on with the tag above it. read() is strict: a comment whose name is not the
// declaration's, a public function with no comment, a tag it does not know or a missing @since is a ParseError naming the header and
// line, so the reference cannot fall behind the headers.
#pragma once

#include <filesystem>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace cb::sdk_impl::reference {

struct Tag {
    std::string name;   // the parameter or field (@param, @field); empty for the others
    std::string text;
};

struct Entry {
    std::string name;
    std::string kind;          // "function", "type", "enum", "macro"
    std::string summary;       // the comment's first line, after "name: "
    std::vector<std::string> description;
    std::string declaration;   // as the header has it (CB_API taken off)
    std::vector<Tag> params, fields;
    std::string returns, ownership, threads, since;
    int line = 0;              // where the comment starts in the header
};

struct Header {
    std::string file;       // "binary.h"
    std::string overview;   // the header's first comment, as lines joined by newlines
    std::vector<Entry> entries;
};

// Every header of the folder (*.h, then *.hpp), by name. ParseError for a comment that does not follow the rules above.
std::vector<Header> read(const std::filesystem::path& include_dir);

// {function: the files (relative to `root`, with '/') under each folder of `where` whose text names it}, for every function of `headers`.
using Uses = std::map<std::string, std::vector<std::string>>;
Uses uses(const std::vector<Header>& headers, const std::filesystem::path& root, const std::vector<std::filesystem::path>& where);

// The pages of the reference, {path under docs/sdk/reference: markdown}. `samples` and `tests` are uses() over samples/ and the SDK's tests.
std::map<std::string, std::string> pages(const std::vector<Header>& headers, const Uses& samples, const Uses& tests);

// Writes the pages under `out` (docs/sdk/reference), removing a page of the folder that is no longer made. Returns how many were written.
std::size_t write(const std::map<std::string, std::string>& pages, const std::filesystem::path& out);

// The reference of a checkout: read() of include/cb, uses() over samples/ and src/sdk's tests, pages().
std::map<std::string, std::string> of_checkout(const std::filesystem::path& checkout);

}  // namespace cb::sdk_impl::reference
