// codebreaker/base/runs.hpp -- C++ from codebreaker.base.runs.
//
// runs.py -- a run's own folder, gone with the run: copies of disks and CDs never pile up.
//
//     runs::Folder work("phase3", tag);            // state/phase3/runs/<tag>_<time>_<pid>/
//     ...copy the disk and the CD into work.path(), boot them...
//     py::str shots = runs::results("phase3");     // state/phase3/shots/: one screenshot, one log per tag
//
// A harness that boots a machine works on copies: a 650 MB hard disk, a CD image. The copy is the
// run's, in a folder of its own under state/<tool>/runs/, and is deleted when the run ends, however
// it ends. A run that was killed never reaches its end; so every run, as it starts, deletes the run
// folders of its tool older than a day (a run still going holds its files open, and is not deleted
// under it). What a run leaves for the user -- its last screenshot, its log -- goes to the tool's
// one results folder, under the run's tag, the one before it overwritten.
#pragma once

#include "py.hpp"

namespace codebreaker::base::runs {

// a run folder older than this is a killed run's (seconds)
extern const int64_t AGE;

// The tool's one folder of what its runs leave (made if it is not there).
py::str results(const py::str& tool, const py::str& name = py::str("shots"));

// Delete a run folder; a machine just stopped lets go of its files a moment later.
bool remove(const py::str& path, int64_t tries = 20);

// Delete the tool's run folders older than `age` seconds (the killed runs'). -> those deleted
py::list<py::str> sweep(const py::str& tool, double age = 24.0 * 3600.0);

// A new run folder of the tool's (the killed runs' swept first). -> its path; the caller
// deletes it with remove() when the run ends (Folder does both).
py::str start(const py::str& tool, const py::str& tag = py::str("run"));

// folder(): a run folder for the length of a scope, deleted at its end, however it ends
// (Python's `with runs.folder(tool, tag) as work:` is `runs::Folder work(tool, tag);`).
class Folder {
public:
    Folder(const py::str& tool, const py::str& tag = py::str("run"));
    ~Folder();
    Folder(const Folder&) = delete;
    Folder& operator=(const Folder&) = delete;
    const py::str& path() const { return path_; }
private:
    py::str path_;
};

}  // namespace codebreaker::base::runs
