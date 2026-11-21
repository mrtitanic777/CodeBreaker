/* msgtable.c -- wlink's English messages compiled in (CodeBreaker, docs/X86LINK_DESIGN.md).
 *
 * wlink.exe keeps its messages in the string table of its own image (wl/res/wlink.rc: the pick() lists of lnkerror.msg,
 * wlink.msg and rc.msg), and Msg_Get reads them with LoadString from GetModuleHandle(NULL). Linked into codebreaker.exe
 * there is no such table, so the same lists are compiled into this array and Msg_Get looks a message up here
 * (wl/c/wlnkmsg.c, CB_WLINK_MSG_TABLE). The ids and the English text are exactly the resource's.
 */
#include <string.h>
#include "build/x86link/link/_formats.h"   /* FORMATS(): wlink.msg's per-format entries (as wlink.rc includes it) */
#include "build/x86link/messages/wlnkmsg.rh"
#include "build/x86link/messages/msgtable.h"

static const cb_wlink_msg messages[] = {
#define pick( c, e, j ) { c, e },
#include "build/x86link/messages/lnkerror.msg"
#include "build/x86link/messages/wlink.msg"
#include "rc.msg"
#undef pick
};

const char *cb_wlink_message( int msgid )
{
    size_t      i;

    for( i = 0; i < sizeof( messages ) / sizeof( messages[0] ); i++ ) {
        if( messages[i].id == msgid ) {
            return( messages[i].text );
        }
    }
    return( NULL );
}
