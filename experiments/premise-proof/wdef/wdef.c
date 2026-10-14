/* wdef.c -- translate a Microsoft module-definition (.def) file into OpenWatcom
 * wlink directives, so the recovered .def files feed our MS-free build directly.
 * Built by our own OpenWatcom (wcl386).  Companion to wthunk.
 *
 *     wdef lz32.def [obj ...] > lz32.def.lnk
 *     wlink system nt_dll name lz32.dll file lz32.obj @lz32.def.lnk
 *
 * Handles: LIBRARY/NAME, DESCRIPTION, EXPORTS (entry [=internal] [@ord]
 * [NONAME] [RESIDENTNAME] [PRIVATE] [DATA]) and IMPORTS (internal = module.entry
 * | internal = module.ordinal).  EXETYPE/STUB/CODE/DATA/HEAPSIZE/STACKSIZE/
 * SEGMENTS/SECTIONS are ignored.  Comments start with ';'.
 *
 * Decoration: MS LINK matches a bare .def export name to a decorated STDCALL
 * symbol (_Name@N); wlink does not.  If .obj files are passed, wdef reads their
 * symbol tables and resolves each bare export to the real symbol (_Name@N, or
 * _Name, or the bare name) -- replicating MS LINK's auto-undecoration -- and
 * emits `export Name.ord=<real>`.  Without objs it emits bare names (fine for
 * undecorated Win16 PASCAL exports).  @-decorated names are single-quoted.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ---- symbol table gathered from .obj files -------------------------------- */
#define MAXSYMS 8192
static char *syms[MAXSYMS]; static int nsyms;

static int id_char(int c){ return isalnum(c)||c=='_'||c=='@'||c=='?'||c=='$'||c=='.'; }

static void add_sym(const char *s){
    int i;
    if (nsyms>=MAXSYMS) return;
    for (i=0;i<nsyms;i++) if (!strcmp(syms[i],s)) return;   /* dedup */
    syms[nsyms]=strdup(s); if (syms[nsyms]) nsyms++;
}

/* Scan an .obj for OMF length-prefixed identifier strings (PUBDEF/EXTDEF names).
   Over-collects slightly, but that only widens the match pool harmlessly. */
static void collect_syms(const char *path){
    FILE *f=fopen(path,"rb"); long sz; unsigned char *b; long i;
    if(!f){ fprintf(stderr,"wdef: cannot open obj %s\n",path); return; }
    fseek(f,0,SEEK_END); sz=ftell(f); fseek(f,0,SEEK_SET);
    b=(unsigned char*)malloc(sz); if(!b){fclose(f);return;} fread(b,1,sz,f); fclose(f);
    for(i=0;i+1<sz;i++){
        int L=b[i], k, ok=1;
        if(L<2||L>127||i+1+L>sz) continue;
        for(k=1;k<=L;k++) if(!id_char(b[i+k])){ ok=0; break; }
        if(!ok) continue;
        /* must look like a symbol: start with letter or _ or @ or ? */
        if(!(isalpha(b[i+1])||b[i+1]=='_'||b[i+1]=='@'||b[i+1]=='?')) continue;
        { char tmp[130]; memcpy(tmp,b+i+1,L); tmp[L]=0; add_sym(tmp); }
    }
    free(b);
}

/* Resolve a bare export name to the real symbol, MS-LINK style. */
static const char *resolve(const char *name){
    static char out[160]; int i; size_t nl=strlen(name);
    if(nsyms==0) return name;                      /* no objs -> bare */
    for(i=0;i<nsyms;i++) if(!strcmp(syms[i],name)) return name;       /* exact (PASCAL/undecorated) */
    /* _name@<digits>  (STDCALL) */
    for(i=0;i<nsyms;i++){
        const char *s=syms[i];
        if(s[0]=='_' && !strncmp(s+1,name,nl) && s[1+nl]=='@'){
            const char *d=s+2+nl; if(*d && strspn(d,"0123456789")==strlen(d)){ strcpy(out,s); return out; }
        }
    }
    /* _name  (cdecl) */
    for(i=0;i<nsyms;i++){ const char *s=syms[i]; if(s[0]=='_' && !strcmp(s+1,name)) return syms[i]; }
    return name;                                   /* fallback: bare */
}

static char *trim(char *s){ char *e; while(*s==' '||*s=='\t'||*s=='\r'||*s=='\n')s++;
    e=s+strlen(s); while(e>s&&(e[-1]==' '||e[-1]=='\t'||e[-1]=='\r'||e[-1]=='\n'))*--e=0; return s; }

/* quote a token in single quotes if it contains '@' (wlink response sigil) */
static void q(FILE *o, const char *s){ if(strchr(s,'@')) fprintf(o,"'%s'",s); else fputs(s,o); }

int main(int argc,char**argv){
    FILE *f; char line[512]; int sect=0; int ai;
    FILE *o=stdout;
    if(argc<2){ fprintf(stderr,"usage: wdef file.def [obj ...] [> out.lnk]\n"); return 2; }
    for(ai=2;ai<argc;ai++) collect_syms(argv[ai]);
    f=fopen(argv[1],"r"); if(!f){ fprintf(stderr,"wdef: cannot open %s\n",argv[1]); return 1; }
    while(fgets(line,sizeof line,f)){
        char *p, *semi; char work[512];
        semi=strchr(line,';'); if(semi)*semi=0;
        p=trim(line); if(!*p) continue;
        strcpy(work,p);
        { char kw[64]; int i=0; char *w=work; while(w[i]&&!isspace((unsigned char)w[i])&&i<63){kw[i]=(char)toupper((unsigned char)w[i]);i++;} kw[i]=0;
          if(!strcmp(kw,"EXPORTS")){ sect=1; continue; }
          if(!strcmp(kw,"IMPORTS")){ sect=2; continue; }
          if(!strcmp(kw,"LIBRARY")||!strcmp(kw,"NAME")){ sect=0; continue; }
          if(!strcmp(kw,"DESCRIPTION")||!strcmp(kw,"EXETYPE")||!strcmp(kw,"STUB")||
             !strcmp(kw,"CODE")||!strcmp(kw,"DATA")||!strcmp(kw,"HEAPSIZE")||
             !strcmp(kw,"STACKSIZE")||!strcmp(kw,"SEGMENTS")||!strcmp(kw,"SECTIONS")||
             !strcmp(kw,"VERSION")){ sect=0; continue; }
        }
        if(sect==1){
            char entry[128]="", intern[128]="", ord[32]=""; int resident=0, priv=0;
            char *tok; char buf[512]; strcpy(buf,p);
            tok=strtok(buf," \t");
            if(tok){ char *eq=strchr(tok,'='); if(eq){ *eq=0; strcpy(entry,tok); strcpy(intern,eq+1);} else strcpy(entry,tok); }
            while((tok=strtok(NULL," \t"))!=NULL){
                if(tok[0]=='@') strcpy(ord,tok+1);
                else if(tok[0]=='='){ strcpy(intern,tok+1); }
                else { char u[64]; int i=0; while(tok[i]&&i<63){u[i]=(char)toupper((unsigned char)tok[i]);i++;} u[i]=0;
                    if(!strcmp(u,"RESIDENTNAME")) resident=1; else if(!strcmp(u,"PRIVATE")) priv=1; }
            }
            if(!entry[0]) continue;
            if(!intern[0]){ const char *r=resolve(entry); if(strcmp(r,entry)) strcpy(intern,r); }
            fputs("export ",o); q(o,entry);
            if(ord[0]){ fputc('.',o); fputs(ord,o); }
            if(intern[0]){ fputc('=',o); q(o,intern); }
            if(resident) fputs(" RESident",o);
            if(priv) fputs(" PRIVATE",o);
            fputc('\n',o);
        } else if(sect==2){
            char *eq; char internal[160], rhs[160], mod[128], ent[128], *dot; char buf[512];
            strcpy(buf,p); eq=strchr(buf,'='); if(!eq) continue;
            *eq=0; strcpy(internal,trim(buf)); strcpy(rhs,trim(eq+1));
            dot=strchr(rhs,'.'); if(!dot) continue;
            *dot=0; strcpy(mod,trim(rhs)); strcpy(ent,trim(dot+1));
            fputs("import ",o); q(o,internal); fputc(' ',o); fputs(mod,o); fputc('.',o); q(o,ent); fputc('\n',o);
        }
    }
    fclose(f);
    return 0;
}
