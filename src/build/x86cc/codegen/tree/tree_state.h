/* tree_state.h -- the variables of tree.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "tree.h"
#include "zoiks.h"
#include "freelist.h"
#include "cgauxcc.h"
#include "cgauxinf.h"
#include "data.h"
#include "addrfold.h"
#include "bgcall.h"
#include "bldcall.h"
#include "bldins.h"
#include "blips.h"
#include "display.h"
#include "foldins.h"
#include "inline.h"
#include "makeaddr.h"
#include "namelist.h"
#include "patch.h"
#include "rgtbl.h"
#include "stackcg.h"
#include "treeprot.h"
#include "treeconv.h"
#include "treefold.h"
#include "typemap.h"
#include "types.h"
#include "i64.h"
#include "utils.h"
#include "blips.h"
#include "typemap.h"
#include "bgcall.h"
#include "feprotos.h"
#include "cgprotos.h"
    #include "x86segs.h"

struct TreeState {
    pointer *TreeFrl;
    type_class_def SubMat[169] = {
    /*********************************
     * what is the result type of a subtraction
     *
     *  U1 I1 U2 I2 U4 I4 U8 I8 CP PT FS FD FL      */
        U1,U1,U2,I2,U4,I4,U8,I8,CP,PT,FS,FD,FL,/* U1*/
        U1,I1,U2,I2,U4,I4,U8,I8,CP,PT,FS,FD,FL,/* I1*/
        U2,U2,U2,U2,U4,I4,U8,I8,CP,PT,FS,FD,FL,/* U2*/
        I2,I2,U2,I2,U4,I4,U8,I8,CP,PT,FS,FD,FL,/* I2*/
        U4,U4,U4,U4,U4,U4,U8,I8,CP,PT,FS,FD,FL,/* U4*/
        I4,I4,I4,I4,U4,I4,U8,I8,CP,PT,FS,FD,FL,/* I4*/
        U8,U8,U8,U8,U8,U8,U8,U8,CP,PT,FD,FD,FL,/* U8*/
        I8,I8,I8,I8,I8,I8,U8,I8,CP,PT,FD,FD,FL,/* I8*/
        CP,CP,CP,CP,CP,CP,CP,CP,XX,PT,ER,ER,ER,/* CP*/
        PT,PT,PT,PT,PT,PT,PT,PT,PT,XX,ER,ER,ER,/* PT*/
        FS,FS,FS,FS,FS,FS,FD,FD,ER,ER,FS,FD,FL,/* FS*/
        FD,FD,FD,FD,FD,FD,FD,FD,ER,ER,FD,FD,FL,/* FD*/
        FL,FL,FL,FL,FL,FL,FL,FL,ER,ER,FL,FL,FL,/* FL*/
    };
    bool IsAddress;
    tn NodesToZap;
};
