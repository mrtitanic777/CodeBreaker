/* wthunk.c -- native C reimplementation of the MS Thunk Compiler (thunk.exe),
 * built by our own OpenWatcom (wcl386).  Reads a .thk flat-thunk script and emits
 * the one gated (IS_32/IS_16) .asm that wasm assembles -- making .thk a
 * first-class source type for a maintainable, MS-free toolchain.
 *
 * Port of wthunk.py; emits byte-for-byte identical output.  C89 (Watcom-clean).
 * Scope: the direct-mapped 32->16 flat-thunk subset (enablemapdirect3216).
 *
 *     wthunk -o lz.asm lz.thk
 * Build:  wcl386 -bt=nt -l=nt wthunk.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum { CWORD, CDWORD, CPTR, CVOID };

typedef struct { char name[64]; int cls; } Arg;
typedef struct { char name[64]; int ret; int nargs; Arg args[16]; } Func;
typedef struct { char name[64]; char base[64]; int isptr; } Alias;

static Alias aliases[128]; static int naliases;
static char  structs[128][64]; static int nstructs;
static Func  funcs[128]; static int nfuncs;

static void strip_comments(char *s) {
    char *o = s; char *p = s;
    while (*p) {
        if (p[0] == '/' && p[1] == '*') { p += 2; while (*p && !(p[0]=='*'&&p[1]=='/')) p++; if (*p) p += 2; }
        else if (p[0] == '/' && p[1] == '/') { p += 2; while (*p && *p != '\n') p++; }
        else *o++ = *p++;
    }
    *o = 0;
}

static int base_class(const char *c) {
    if (!strcmp(c,"void")) return CVOID;
    if (!strcmp(c,"long")||!strcmp(c,"unsigned long")) return CDWORD;
    if (!strcmp(c,"char")||!strcmp(c,"unsigned char")||!strcmp(c,"signed char")||
        !strcmp(c,"short")||!strcmp(c,"unsigned short")||!strcmp(c,"int")||!strcmp(c,"unsigned int"))
        return CWORD;
    return -1;
}

/* collapse whitespace runs to single spaces; trim both ends (in place) */
static void norm_ws(char *s) {
    char tmp[256]; int j = 0; int sp = 0; int i; int L;
    for (i = 0; s[i]; i++) {
        char c = s[i];
        if (c==' '||c=='\t'||c=='\n'||c=='\r') sp = 1;
        else { if (sp && j>0) tmp[j++]=' '; sp = 0; if (j < (int)sizeof(tmp)-1) tmp[j++]=c; }
    }
    tmp[j] = 0;
    L = j; while (L>0 && tmp[L-1]==' ') tmp[--L]=0;
    strcpy(s, tmp);
}

static int classify(const char *ty) {
    char t[128]; char core[128]; int star; int i, j, b;
    strncpy(t, ty, sizeof t - 1); t[sizeof t-1]=0; norm_ws(t);
    star = (strchr(t,'*') != NULL);
    j = 0; for (i=0; t[i]; i++) if (t[i] != '*') core[j++]=t[i];
    core[j]=0; norm_ws(core);
    if (star) return CPTR;
    b = base_class(core);
    if (b >= 0) return b;
    for (i=0;i<nstructs;i++) if (!strcmp(structs[i],core)) return CPTR;
    for (i=0;i<naliases;i++) if (!strcmp(aliases[i].name,core)) {
        if (aliases[i].isptr) return CPTR;
        return classify(aliases[i].base);
    }
    return CWORD;
}

static void parse_types(const char *text) {
    const char *p = text;
    while ((p = strstr(p, "typedef")) != NULL) {
        const char *q = p + 7;
        const char *sk = q; const char *semi; const char *n;
        char buf[256]; char name[64]; char basestr[192]; char bclean[192];
        int e, s2, nl, bl, isptr, k, i, Lc;
        while (*sk==' '||*sk=='\t'||*sk=='\n') sk++;
        if (!strncmp(sk, "struct", 6)) {
            const char *br = strchr(sk, '{');
            const char *eb = br ? strchr(br, '}') : NULL;
            if (eb) {
                int jj = 0; n = eb+1; while (*n==' '||*n=='\t'||*n=='\n') n++;
                while (isalnum((unsigned char)*n)||*n=='_') { if (jj<63) name[jj++]=*n; n++; }
                name[jj]=0; if (jj) strcpy(structs[nstructs++], name);
                p = eb+1; continue;
            }
        }
        semi = strchr(q, ';'); if (!semi) break;
        Lc = (int)(semi - q); if (Lc > 255) Lc = 255; memcpy(buf, q, Lc); buf[Lc]=0;
        e = (int)strlen(buf); while (e>0 && (buf[e-1]==' '||buf[e-1]=='\t'||buf[e-1]=='\n')) e--;
        s2 = e; while (s2>0 && (isalnum((unsigned char)buf[s2-1])||buf[s2-1]=='_')) s2--;
        nl = e-s2; if (nl>63) nl=63; memcpy(name, buf+s2, nl); name[nl]=0;
        bl = s2; if (bl>191) bl=191; memcpy(basestr, buf, bl); basestr[bl]=0;
        isptr = (strchr(basestr,'*') != NULL);
        k=0; for (i=0;basestr[i];i++) if (basestr[i]!='*') bclean[k++]=basestr[i]; bclean[k]=0;
        norm_ws(bclean);
        if (name[0]) { strcpy(aliases[naliases].name,name); strncpy(aliases[naliases].base,bclean,63);
            aliases[naliases].base[63]=0; aliases[naliases].isptr=isptr; naliases++; }
        p = semi+1;
    }
}

static int is_id(char c){ return isalnum((unsigned char)c)||c=='_'; }

static void parse_funcs(char *text) {
    char *p = text;
    while (*p) {
        char *op, *ne, *ns, *rs, *cp, *bo, *be, *tok;
        char name[64]; char ret[128]; char params[512]; char body[1024];
        int nl, rl, pl, bl;
        Func *f;
        op = strchr(p, '('); if (!op) break;
        ne = op; while (ne>text && (ne[-1]==' '||ne[-1]=='\t'||ne[-1]=='\n')) ne--;
        ns = ne; while (ns>text && is_id(ns[-1])) ns--;
        if (ns==ne) { p = op+1; continue; }
        nl=(int)(ne-ns); if (nl>63) nl=63; memcpy(name,ns,nl); name[nl]=0;
        rs = ns; while (rs>text && rs[-1]!=';' && rs[-1]!='}') rs--;
        rl=(int)(ns-rs); if (rl>127) rl=127; memcpy(ret,rs,rl); ret[rl]=0; norm_ws(ret);
        cp = strchr(op, ')'); if (!cp) break;
        pl=(int)(cp-op-1); if (pl>511) pl=511; memcpy(params,op+1,pl); params[pl]=0;
        bo = strchr(cp, '{'); be = bo ? strchr(bo,'}') : NULL;
        if (!bo || !be) { p = cp+1; continue; }
        bl=(int)(be-bo-1); if (bl>1023) bl=1023; memcpy(body,bo+1,bl); body[bl]=0;
        if (ret[0]==0) { p = be+1; continue; }
        f = &funcs[nfuncs];
        strcpy(f->name, name); f->ret = classify(ret); f->nargs = 0;
        tok = params;
        while (*tok) {
            char *comma; char seg[128]; int seglen; int e, s2, al, tl;
            char an[64]; char ty[128]; Arg *a;
            comma = strchr(tok, ','); seglen = comma? (int)(comma-tok): (int)strlen(tok);
            if (seglen>127) seglen=127; memcpy(seg,tok,seglen); seg[seglen]=0; norm_ws(seg);
            if (seg[0] && strcmp(seg,"void") && strcmp(seg,"VOID")) {
                e=(int)strlen(seg); s2=e; while (s2>0 && is_id(seg[s2-1])) s2--;
                al=e-s2; if(al>63)al=63; memcpy(an,seg+s2,al); an[al]=0;
                tl=s2; if(tl>127)tl=127; memcpy(ty,seg,tl); ty[tl]=0;
                if (strchr(seg,'*') && !strchr(ty,'*')) strcat(ty,"*");
                a = &f->args[f->nargs];
                strcpy(a->name, an); a->cls = classify(ty); f->nargs++;
            }
            if (!comma) break; tok = comma+1;
        }
        nfuncs++;
        p = be+1;
    }
}

static unsigned checksum(void) {
    unsigned h = 0x1505; int i, a; char *c;
    for (i=0;i<nfuncs;i++) {
        for (c=funcs[i].name; *c; c++) h = (h*33) ^ (unsigned char)*c;
        for (a=0;a<funcs[i].nargs;a++) {
            int cl = funcs[i].args[a].cls;
            char fc = cl==CWORD?'W':cl==CDWORD?'D':cl==CPTR?'P':'V';
            h = (h*33) ^ (unsigned char)fc;
        }
    }
    return h;
}

static FILE *out;

static void emit32(const char *pfx, unsigned cs) {
    const char *rt[7]; int i, n;
    rt[0]="MapHInstLS"; rt[1]="MapHInstLS_PN"; rt[2]="MapHInstSL"; rt[3]="MapHInstSL_PN";
    rt[4]="FT_Prolog"; rt[5]="FT_Thunk"; rt[6]="QT_Thunk";
    fprintf(out, "\t.386\n\t.model FLAT,STDCALL\n\n");
    fprintf(out, ";-- Import common flat thunk routines (in k32)\n");
    for (i=0;i<7;i++) fprintf(out,"externdef %s\t:near\n", rt[i]);
    for (n=0;n<=56;n+=4) fprintf(out,"externdef FT_Exit%d\t:near\n", n);
    fprintf(out,"externdef SMapLS\t:near\nexterndef SUnMapLS\t:near\n");
    for (n=8;n<=40;n+=4){ fprintf(out,"externdef SMapLS_IP_EBP_%d\t:near\n",n); fprintf(out,"externdef SUnMapLS_IP_EBP_%d\t:near\n",n); }
    fprintf(out,"\nexterndef MapSL:near\n\n\t.code \n\n\t.data\n\n");
    fprintf(out,"public %s_ThunkData32\t;This symbol must be exported.\n%s_ThunkData32 label dword\n",pfx,pfx);
    fprintf(out,"\tdd\t3130534ch\t;Protocol 'LS01'\n\tdd\t0%xh\t;Checksum\n",cs);
    fprintf(out,"\tdd\t0\t;Jump table address.\n\tdd\t3130424ch\t;'LB01'\n");
    fprintf(out,"\tdd\t0\t;Flags\n\tdd\t0\t;Reserved (MUST BE 0)\n\tdd\t0\t;Reserved (MUST BE 0)\n");
    fprintf(out,"\tdd\toffset QT_Thunk_%s - offset %s_ThunkData32\n",pfx,pfx);
    fprintf(out,"\tdd\toffset FT_Prolog_%s - offset %s_ThunkData32\n\n\t.code \n\n",pfx,pfx);
    fprintf(out,"externdef ThunkConnect32@24:near\n");
    fprintf(out,"public _%s_ThunkConnect32@16\n_%s_ThunkConnect32@16:\n",pfx,pfx);
    fprintf(out,"\tpop\tedx\n\tpush\toffset %s_ThkData16\n\tpush\toffset %s_ThunkData32\n",pfx,pfx);
    fprintf(out,"\tpush\tedx\n\tjmp\tThunkConnect32@24\n");
    fprintf(out,"%s_ThkData16 label byte\n\tdb\t\"%s_ThunkData16\",0\n\n",pfx,pfx);
    fprintf(out,"pfnQT_Thunk_%s\tdd offset QT_Thunk_%s\n",pfx,pfx);
    fprintf(out,"pfnFT_Prolog_%s\tdd offset FT_Prolog_%s\n\t.data\n",pfx,pfx);
    fprintf(out,"QT_Thunk_%s label byte\n\tdb\t32 dup(0cch)\t;Patch space.\n\n",pfx);
    fprintf(out,"FT_Prolog_%s label byte\n\tdb\t32 dup(0cch)\t;Patch space.\n\n\t.code \n\n",pfx);
    fprintf(out,";************************ START OF THUNK BODIES************************\n\n");
    for (i=0;i<nfuncs;i++) {
        Func *f=&funcs[i]; int cl=(nfuncs-1)-i; int stackb=4*f->nargs;
        int off=8; int unmaps[16]; int nun=0; int a, u;
        fprintf(out,"\npublic _%s@%d\n_%s@%d:\n\tmov\tcl,%d\n",f->name,stackb,f->name,stackb,cl);
        fprintf(out,"\tpush\tebp\n\tmov\tebp,esp\n\tpush\tecx\n\tsub\tesp,60\n");
        for (a=0;a<f->nargs;a++){
            if (f->args[a].cls==CPTR){ fprintf(out,"\tcall\tSMapLS_IP_EBP_%d\n\tpush\teax\n",off); unmaps[nun++]=off; }
            else if (f->args[a].cls==CDWORD) fprintf(out,"\tpush\tdword ptr [ebp+%d]\t;%s: dword->dword\n",off,f->args[a].name);
            else fprintf(out,"\tpush\tword ptr [ebp+%d]\t;%s: dword->word\n",off,f->args[a].name);
            off+=4;
        }
        fprintf(out,"\tcall\tdword ptr [pfnQT_Thunk_%s]\n",pfx);
        if (f->ret==CDWORD) fprintf(out,"\tshl\teax,16\n\tshrd\teax,edx,16\n");
        else if (f->ret==CWORD) fprintf(out,"\tcwde\n");
        for (u=0;u<nun;u++) fprintf(out,"\tcall\tSUnMapLS_IP_EBP_%d\n",unmaps[u]);
        fprintf(out,"\tleave\n");
        if (stackb) fprintf(out,"\tretn\t%d\n",stackb); else fprintf(out,"\tretn\n");
    }
}

static void emit16(const char *pfx, unsigned cs) {
    int i;
    fprintf(out,"\t.286\n\t.model LARGE,PASCAL\n\n\t.code\t\n\n");
    for (i=nfuncs-1;i>=0;i--) fprintf(out,"externdef %s:far\n",funcs[i].name);
    fprintf(out,"\n\nFT_%sTargetTable label word\n",pfx);
    for (i=nfuncs-1;i>=0;i--) fprintf(out,"\tdw\toffset %s\n\tdw\t   seg %s\n",funcs[i].name,funcs[i].name);
    fprintf(out,"\n\n\t.data\n\n");
    fprintf(out,"public %s_ThunkData16\t;This symbol must be exported.\n",pfx);
    fprintf(out,"%s_ThunkData16\tdd\t3130534ch\t;Protocol 'LS01'\n\tdd\t0%xh\t;Checksum\n",pfx,cs);
    fprintf(out,"\tdw\toffset FT_%sTargetTable\n\tdw\tseg    FT_%sTargetTable\n",pfx,pfx);
    fprintf(out,"\tdd\t0\t;First-time flag.\n\n\n\t.code \n\n");
    fprintf(out,"externdef ThunkConnect16:far\n");
    fprintf(out,"public %s_ThunkConnect16\n%s_ThunkConnect16:\n",pfx,pfx);
    fprintf(out,"\tpop\tax\n\tpop\tdx\n\tpush\tseg    %s_ThunkData16\n\tpush\toffset %s_ThunkData16\n",pfx,pfx);
    fprintf(out,"\tpush\tseg    %s_ThkData32\n\tpush\toffset %s_ThkData32\n",pfx,pfx);
    fprintf(out,"\tpush\tcs\n\tpush\tdx\n\tpush\tax\n\tjmp\tThunkConnect16\n");
    fprintf(out,"%s_ThkData32 label byte\n\tdb\t\"%s_ThunkData32\",0\n",pfx,pfx);
}

int main(int argc, char **argv) {
    const char *outp=NULL; const char *inp=NULL;
    FILE *f; long sz; char *text; char pfx[64]; unsigned cs; int i, j;
    const char *b; const char *q;
    for (i=1;i<argc;i++){ if(!strcmp(argv[i],"-o")&&i+1<argc) outp=argv[++i]; else inp=argv[i]; }
    if (!inp){ fprintf(stderr,"usage: wthunk -o out.asm in.thk\n"); return 2; }
    f=fopen(inp,"rb"); if(!f){ fprintf(stderr,"wthunk: cannot open %s\n",inp); return 1; }
    fseek(f,0,SEEK_END); sz=ftell(f); fseek(f,0,SEEK_SET);
    text=(char*)malloc(sz+1); fread(text,1,sz,f); text[sz]=0; fclose(f);
    strip_comments(text);
    parse_types(text);
    parse_funcs(text);
    strcpy(pfx,"thk");
    if (outp){ b=outp; for(q=outp;*q;q++) if(*q=='/'||*q=='\\') b=q+1;
        j=0; for (q=b; *q && *q!='.' && j<63; q++) pfx[j++]=*q; pfx[j]=0; }
    cs=checksum();
    out = outp? fopen(outp,"wb") : stdout;
    if (!out){ fprintf(stderr,"wthunk: cannot write %s\n",outp); return 1; }
    fprintf(out,"\tpage\t,132\n\n;wthunk -- MS Thunk Compiler reimplementation (OpenWatcom-targeted)\n\n");
    fprintf(out,"\tTITLE\t$%s.asm\n\n",pfx);
    fprintf(out,"IFNDEF IS_16\nIFNDEF IS_32\n%%out command line error: specify one of -DIS_16, -DIS_32\n.err\nENDIF  ;IS_32\nENDIF  ;IS_16\n\n\n");
    fprintf(out,"IFDEF IS_32\nIFDEF IS_16\n%%out command line error: you can't specify both -DIS_16 and -DIS_32\n.err\nENDIF ;IS_16\n");
    fprintf(out,";************************* START OF 32-BIT CODE *************************\n\n\n");
    emit32(pfx, cs);
    fprintf(out,"\n\n\nELSE\n;************************* START OF 16-BIT CODE *************************\n\n\n\n");
    emit16(pfx, cs);
    fprintf(out,"\n\nENDIF\nEND\n");
    if (out!=stdout) fclose(out);
    fprintf(stderr,"wthunk: %s -> %s  (%d thunks, prefix '%s')\n", inp, outp?outp:"(stdout)", nfuncs, pfx);
    return 0;
}
