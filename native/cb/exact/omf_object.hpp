// cb/exact/omf_object.hpp -- an OMF module (formats/omf) as the object-file record the exact lanes share
// (exact/object.hpp), so the function comparer, the linker emulation and the lanes built on them take the output of
// the compilers of the period and of the XENIX a.out reader alike.
#pragma once

#include "cb/core/bytes.hpp"
#include "cb/exact/object.hpp"
#include "cb/formats/omf.hpp"

namespace cb::exact {

// The module `o` as an ObjectFile (kind Omf). `raw` is the module's file bytes, kept in ObjectFile::raw (empty when
// the caller has no use for them). A fix-up the reader could not classify (a location the comparers have no opinion
// about) becomes FixKind::Lo; a target it could not name becomes the segment "?", the convention of the a.out
// reader.
ObjectFile object_of(const formats::omf::Object& o, ByteView raw = {});

// Read an OMF module and convert it. Throws what formats::omf::read throws.
ObjectFile read_object(ByteView omf_module);

}  // namespace cb::exact
