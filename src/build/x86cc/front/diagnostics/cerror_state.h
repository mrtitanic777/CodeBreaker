/* cerror_state.h -- the variables of cerror.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include <stdarg.h>
#include "clibext.h"
#include "cerrs.h"

typedef enum {
    POSTLIST_SYMBOL,     /* location of previously defined symbol */
    POSTLIST_TWOTYPES_1, /* type mismatch between two types ( source/target ) - print them */
    POSTLIST_TWOTYPES_2  /* type mismatch between two types ( first/second ) - print them */
} postlist_type;
typedef struct ErrPostList {
    struct ErrPostList  *next;
    postlist_type       type;

    union {
        /*
         * POSTLIST_SYMBOL
         */
        struct {
            const char  *sym_name;
            const char  *sym_file;
            unsigned    sym_line;
        } s;
        /*
         * POSTLIST_TWOTYPES
         */
        struct {
            TOKEN       opr;
            TYPEPTR     types[2];
        } o;
    } u;
} ErrPostList;

struct CerrorState {
    ErrPostList *PostList;
    unsigned error_line = 0;
    unsigned error_column = 0;
    char *error_fname = NULL;
};
