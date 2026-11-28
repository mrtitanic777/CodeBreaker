/* cb/version.h -- the CodeBreaker SDK's version, as macros only (the resource compiler reads this file too).
 *
 * The version follows semantic versioning: a minor release only adds; a major release may remove, and the reference lists what
 * moved. Before 1.0 a minor release may still change what an earlier 0.x had.
 */
#ifndef CB_VERSION_H
#define CB_VERSION_H

/** CB_SDK_VERSION_MAJOR: the major version of the headers this program is compiled against.
 * @since 0.1
 */
#define CB_SDK_VERSION_MAJOR 0
/** CB_SDK_VERSION_MINOR: the minor version of the headers.
 * @since 0.1
 */
#define CB_SDK_VERSION_MINOR 1
/** CB_SDK_VERSION_PATCH: the patch version of the headers.
 * @since 0.1
 */
#define CB_SDK_VERSION_PATCH 0
/** CB_SDK_VERSION_STRING: the version of the headers as text, "major.minor.patch".
 * @since 0.1
 */
#define CB_SDK_VERSION_STRING "0.1.0"
/** CB_SDK_MAKE_VERSION: a version as one number, (major << 16) | (minor << 8) | patch, which compares as versions do.
 * @since 0.1
 */
#define CB_SDK_MAKE_VERSION(major, minor, patch) (((major) << 16) | ((minor) << 8) | (patch))
/** CB_SDK_VERSION: the version of the headers as one number; compare it with cb_version(), the library's.
 * @since 0.1
 */
#define CB_SDK_VERSION CB_SDK_MAKE_VERSION(CB_SDK_VERSION_MAJOR, CB_SDK_VERSION_MINOR, CB_SDK_VERSION_PATCH)

#endif
