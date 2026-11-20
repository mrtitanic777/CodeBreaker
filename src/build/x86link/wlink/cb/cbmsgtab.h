/* cbmsgtab.h -- wlink's messages compiled into CodeBreaker (msgtable.c). */
#ifndef CB_WLINK_MSGTAB_H
#define CB_WLINK_MSGTAB_H

typedef struct cb_wlink_msg {
    int         id;
    const char  *text;
} cb_wlink_msg;

/* The English text of message `msgid` (wlnkmsg.rh's ids), or NULL when there is none. */
const char *cb_wlink_message( int msgid );

#endif
