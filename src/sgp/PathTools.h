#ifndef PATHTOOLS_H
#define PATHTOOLS_H

#include <string>

#include "sgp/Platform.h"

class PathTools
{
public:
	/** Join two path components. */
	static std::string joinPaths(const char *first, const char *second);

	/** Join two path components. */
	static std::string joinPaths(const std::string &first, const char *second);

	/** Join two path components. */
	static std::string joinPaths(const std::string &first, const std::string &second);

#if CASE_SENSITIVE_FS
	/** Find an object (file or subdirectory) in the given directory in case-independent manner.
	* @return true when found, return the found name (full path) using foundNameFullPath. */
	static bool findObjectCaseInsensitiveFullPath(const char *directory, const char *name, bool lookForFiles, bool lookForSubdirs, std::string &foundNameFullPath);
#endif

	/** Get parent path (e.g. directory path from the full path). */
	static std::string getParentPath(const std::string &path, bool absolute);

	/** Get the path of the extra data directory.
	* @return the path */
	static std::string getExtraDataDir(const std::string &exeFolder);

private:

#if CASE_SENSITIVE_FS
	/** Find an object (file or subdirectory) in the given directory in case-independent manner.
	* @return true when found, return the found name using foundName. */
	static bool findObjectCaseInsensitive(const char *directory, const char *name, bool lookForFiles, bool lookForSubdirs, std::string &foundName);
#endif

};

#endif // PATHTOOLS_H
