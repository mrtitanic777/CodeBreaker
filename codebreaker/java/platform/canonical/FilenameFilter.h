#ifndef FILENAMEFILTER_H
#define FILENAMEFILTER_H
// java.io.FilenameFilter
#include <string>
#include "File.h"
#include "JavaString.h"
struct FilenameFilter {
	virtual ~FilenameFilter() {}
	// Non-pure + JavaString param: the corpus overrides `accept(File*, const JavaString&)`; a
	// `const std::string&` pure signature wouldn't be overridden by it (different param type), leaving
	// the subclass abstract. Default accepts everything.
	virtual bool accept(File*, const JavaString&) { return true; }
};
#endif // FILENAMEFILTER_H
