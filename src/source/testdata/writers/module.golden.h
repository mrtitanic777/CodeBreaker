/* demo.dll -- types, declarations and globals used by the functions in demo.c.
 * Globals are bound to their addresses in the original until the module's data is
 * reconstructed. */

typedef int HANDLE_T;
int g_data;

/* imports by number:
 *   GetDlgItem                   USER.91
 */

/* globals: g_data, read by 2 */

/* globals renamed */

/* SDK constants written into demo.c, with the values the SDK headers give them */
#define WM_USER 0x400
struct S;
