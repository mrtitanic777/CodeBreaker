// app/notify.cpp -- see notify.hpp.
#include "app/notify.hpp"

namespace cb::app::notify {

namespace {

std::string nf_files(std::uint64_t n) { return std::to_string(n) + (n == 1 ? " file" : " files"); }

}  // namespace

Message compose(const std::vector<Result>& results) {
    Message m;
    if (results.empty()) return m;
    if (results.size() == 1) {
        const Result& r = results.front();
        if (r.outcome == Outcome::Done) {
            m.title = r.name + " is converted";
            m.body = nf_files(r.files) + " written to Converted\\" + r.folder;
            m.open_folder = true;
        } else {
            m.title = r.name + (r.outcome == Outcome::CannotConvert ? " can't be converted" : " couldn't be converted");
            m.body = r.detail;
        }
        return m;
    }
    std::size_t converted = 0;
    std::uint64_t written = 0;
    std::string first_problem;
    for (const Result& r : results) {
        if (r.outcome == Outcome::Done) {
            ++converted;
            written += r.files;
        } else if (first_problem.empty()) {
            first_problem = r.name + ": " + r.detail;
        }
    }
    const std::string total = std::to_string(results.size());
    if (converted == results.size()) {
        m.title = total + " files are converted";
        m.body = nf_files(written) + " written to Converted";
    } else if (converted > 0) {
        m.title = std::to_string(converted) + " of " + total + " files are converted";
        m.body = nf_files(written) + " written to Converted. " + std::to_string(results.size() - converted) + " could not be converted.";
    } else {
        m.title = "None of the " + total + " files could be converted";
        m.body = first_problem;
    }
    m.open_folder = converted > 0;
    return m;
}

std::string xml_escape(const std::string& text) {
    std::string out;
    for (const char c : text) {
        switch (c) {
            case '&': out += "&amp;"; break;
            case '<': out += "&lt;"; break;
            case '>': out += "&gt;"; break;
            case '"': out += "&quot;"; break;
            case '\'': out += "&apos;"; break;
            default: out += c; break;
        }
    }
    return out;
}

std::string toast_xml(const Message& message) {
    std::string xml = "<toast activationType=\"foreground\" launch=\"show\"><visual><binding template=\"ToastGeneric\"><text>" + xml_escape(message.title) + "</text><text>" +
                      xml_escape(message.body) + "</text></binding></visual><actions>";
    if (message.open_folder) xml += "<action content=\"Open folder\" arguments=\"open\" activationType=\"foreground\"/>";
    xml += "<action content=\"Show\" arguments=\"show\" activationType=\"foreground\"/></actions></toast>";
    return xml;
}

}  // namespace cb::app::notify
