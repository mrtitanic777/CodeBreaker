/* wdef.c -- translate a Microsoft module-definition (.def) file into OpenWatcom
 * wlink directives, so the recovered .def files feed our MS-free build directly.
 * Built by our own OpenWatcom (wcl386).  Companion to wthunk.
 *
 *     wdef lz32.def > lz32.def.lnk
 *     wlink system nt_dll name lz32.dll file lz32.obj @lz32.def.lnk
 *
 * Handles: LIBRARY/NAME, DESCRIPTION, EXPORTS (entry [=internal] [@ord]
 * [NONAME] [RESIDENTNAME] [PRIVATE] [DATA]) and IMPORTS (internal = module.entry
 * | internal = module.ordinal).  EXETYPE/STUB/CODE/DATA/HEAPSIZE/STACKSIZE/
 * SEGMENTS/SECTIONS are ignored.  Comments start with ';'.
 *
 * Note on decoration: MS LINK matches a bare .def export name to a decorated
 * STDCALL symbol (_Name@N); wlink does not.  So bare names work for undecorated
 * exports (Win16 PASCAL: LZCOPY; or code built -zzo).  For decorated STDCALL
 * exports, give the internal explicitly in the .def: `LZCopy = _LZCopy@8 @1`.
 * @-decorated names are single-quoted in the emitted directives (wlink needs it).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char *trim(char *s){ char *e; while(*s==' '||*s=='\t'||*s=='\r'||*s=='\n')s++;
    e=s+strlen(s); while(e>s&&(e[-1]==' '||e[-1]=='\t'||e[-1]=='\r'||e[-1]=='\n'))*--e=0; return s; }

static void upper(char *s){ for(;*s;s++) *s=(char)toupper((unsigned char)*s); }

/* quote a token in single quotes if it contains '@' (wlink response sigil) */
static void q(FILE *o, const char *s){ if(strchr(s,'@')) fprintf(o,"'%s'",s); else fputs(s,o); }

int main(int argc,char**argv){
    FILE *f; char line[512]; int sect=0; /* 0 none,1 exports,2 imports */
    FILE *o=stdout;
    if(argc<2){ fprintf(stderr,"usage: wdef file.def [> out.lnk]\n"); return 2; }
    f=fopen(argv[1],"r"); if(!f){ fprintf(stderr,"wdef: cannot open %s\n",argv[1]); return 1; }
    while(fgets(line,sizeof line,f)){
        char *p, *semi; char work[512];
        semi=strchr(line,';'); if(semi)*semi=0;      /* strip comment */
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
            /* EXPORT: entry [=internal] [@ord] [NONAME] [RESIDENTNAME] [PRIVATE] [DATA] */
            char entry[128]="", intern[128]="", ord[32]=""; int resident=0, priv=0;
            char *tok; char buf[512]; strcpy(buf,p);
            tok=strtok(buf," \t");
            if(tok){
                char *eq=strchr(tok,'=');
                if(eq){ *eq=0; strcpy(entry,tok); strcpy(intern,eq+1); }
                else strcpy(entry,tok);
            }
            while((tok=strtok(NULL," \t"))!=NULL){
                if(tok[0]=='@') strcpy(ord,tok+1);
                else if(tok[0]=='='){ strcpy(intern,tok+1); }
                else { char u[64]; int i=0; while(tok[i]&&i<63){u[i]=(char)toupper((unsigned char)tok[i]);i++;} u[i]=0;
                    if(!strcmp(u,"RESIDENTNAME")) resident=1;
                    else if(!strcmp(u,"PRIVATE")) priv=1;
                    /* NONAME, DATA: accepted, no direct wlink flag needed here */
                }
            }
            if(!entry[0]) continue;
            fputs("export ",o); q(o,entry);
            if(ord[0]){ fputc('.',o); fputs(ord,o); }
            if(intern[0]){ fputc('=',o); q(o,intern); }
            if(resident) fputs(" RESident",o);
            if(priv) fputs(" PRIVATE",o);
            fputc('\n',o);
        } else if(sect==2){
            /* IMPORT: internal = module.entry   (entry = name or ordinal) */
            char *eq; char internal[160], rhs[160], mod[128], ent[128], *dot;
            char buf[512]; strcpy(buf,p);
            eq=strchr(buf,'='); if(!eq) continue;
            *eq=0; strcpy(internal,trim(buf)); strcpy(rhs,trim(eq+1));
            dot=strchr(rhs,'.'); if(!dot) continue;
            *dot=0; strcpy(mod,trim(rhs)); strcpy(ent,trim(dot+1));
            fputs("import ",o); q(o,internal); fputc(' ',o); fputs(mod,o); fputc('.',o); q(o,ent); fputc('\n',o);
        }
    }
    fclose(f);
    return 0;
}
