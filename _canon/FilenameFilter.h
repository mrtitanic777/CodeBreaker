#ifndef FILENAMEFILTER_H
#define FILENAMEFILTER_H
// java.io.FilenameFilter
#include <string>
#include "File.h"
struct FilenameFilter {
	virtual ~FilenameFilter() {}
	virtual bool accept(File*, const std::string&) = 0;
};
#endif // FILENAMEFILTER_H
