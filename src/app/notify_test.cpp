// Tests of the finished-run notification's words and XML (notify.hpp).
#include "app/notify.hpp"

#include "core/testing.hpp"

namespace cb::app::notify {
namespace {

Result nf_done(const char* name, std::uint64_t files, const char* folder) { return Result{name, Outcome::Done, files, folder, {}}; }
Result nf_bad(const char* name, Outcome how, const char* detail) { return Result{name, how, 0, {}, detail}; }

CB_TEST("app.notify.one_converted_file_says_what_was_written_and_where", [] {
    const Message m = compose({nf_done("setup.exe", 12, "setup")});
    CB_CHECK_EQ(m.title, "setup.exe is converted");
    CB_CHECK_EQ(m.body, "12 files written to Converted\\setup");
    CB_CHECK(m.open_folder);
    CB_CHECK_EQ(compose({nf_done("a.gb", 1, "a")}).body, "1 file written to Converted\\a");
});

CB_TEST("app.notify.a_file_that_could_not_be_converted_says_why_and_has_no_folder_to_open", [] {
    const Message cannot = compose({nf_bad("photo.png", Outcome::CannotConvert, "This looks like a PNG image.")});
    CB_CHECK_EQ(cannot.title, "photo.png can't be converted");
    CB_CHECK_EQ(cannot.body, "This looks like a PNG image.");
    CB_CHECK(!cannot.open_folder);
    const Message failed = compose({nf_bad("broken.exe", Outcome::Failed, "The file is cut short.")});
    CB_CHECK_EQ(failed.title, "broken.exe couldn't be converted");
    CB_CHECK(compose({}).title.empty());
});

CB_TEST("app.notify.a_batch_counts_the_files_converted_and_written", [] {
    Message all = compose({nf_done("a.dll", 3, "a"), nf_done("b.dll", 4, "b")});
    CB_CHECK_EQ(all.title, "2 files are converted");
    CB_CHECK_EQ(all.body, "7 files written to Converted");
    CB_CHECK(all.open_folder);
    const Message some = compose({nf_done("a.dll", 3, "a"), nf_bad("p.png", Outcome::CannotConvert, "A PNG image."), nf_done("c.dll", 1, "c")});
    CB_CHECK_EQ(some.title, "2 of 3 files are converted");
    CB_CHECK_EQ(some.body, "4 files written to Converted. 1 could not be converted.");
    const Message none = compose({nf_bad("p.png", Outcome::CannotConvert, "A PNG image."), nf_bad("q.exe", Outcome::Failed, "Cut short.")});
    CB_CHECK_EQ(none.title, "None of the 2 files could be converted");
    CB_CHECK_EQ(none.body, "p.png: A PNG image.");
    CB_CHECK(!none.open_folder);
});

CB_TEST("app.notify.the_toast_xml_has_the_words_and_the_buttons_and_escapes_markup", [] {
    CB_CHECK_EQ(xml_escape("a<b>&\"c'"), "a&lt;b&gt;&amp;&quot;c&apos;");
    const std::string xml = toast_xml(compose({nf_done("a&b.exe", 2, "a&b")}));
    CB_CHECK(xml.starts_with("<toast "));
    CB_CHECK(xml.find("<text>a&amp;b.exe is converted</text>") != std::string::npos);
    CB_CHECK(xml.find("<text>2 files written to Converted\\a&amp;b</text>") != std::string::npos);
    CB_CHECK(xml.find("content=\"Open folder\" arguments=\"open\"") != std::string::npos);
    CB_CHECK(xml.find("content=\"Show\" arguments=\"show\"") != std::string::npos);
    CB_CHECK(xml.find("launch=\"show\"") != std::string::npos);
    CB_CHECK(xml.ends_with("</toast>"));
    // no folder, no Open folder button
    const std::string no_folder = toast_xml(compose({nf_bad("p.png", Outcome::CannotConvert, "A PNG.")}));
    CB_CHECK(no_folder.find("Open folder") == std::string::npos && no_folder.find("arguments=\"show\"") != std::string::npos);
});

}  // namespace
}  // namespace cb::app::notify
