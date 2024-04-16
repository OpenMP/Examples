/*
 *  A utility to check validity of @@<tags> in source codes.
 *  The tool can also be used to fix issues when possible.
 *
 *  Version 0.1 checks only @@name and @@type.
 *  Version 0.2 added check for other tags.
 *  Version 0.3 added check for line length.
 *  Version 0.4 added check for version tags in tex files.
 *              updated for new tag format
 *  Version 0.5 enabled translation of empty version tag to "pre_omp"
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERNO "0.5"

#define BKS '\\'
#define BUF_SIZE 1024
#define FN_SIZE  256
#define TAG_SIZE 256
#define F_VFILE  0x01
#define F_STRICT 0x02
#define F_FIXIT  0x04
#define F_CHKLLN 0x08
#define F_CHKTAG 0x10
#define F_LIST   0x20
#define F_VLIST  0x31
#define F_HLIST  0x40
#define T_PNAME  0x02
#define T_MTAGS  0x04

#define LLN_DEF  75
#define PRE_OMP  "pre_omp_3.0"

#define skipspc(ic, cp) \
   for (cp += ic; *cp == ' ' || *cp == '\t'; cp++)

/* list of tags */
static int max_tags = 7;
static int vtag_idx = 4;
static int etag_idx = 5;
static struct tags_s {
   char *name;             /* tag name */
   char *tvals;            /* possible values */
   int nc;                 /* size of name */
   int r, c;               /* line number, column number */
   char val[TAG_SIZE];     /* actual value */
} tags[] = {
   {"name", "", 4, 0, 0},
   {"type", "", 4, 0, 0},
   {"operation", "view|compile|link|run", 9, 0, 0},
   {"expect", "success|error|ct-error|rt-error"
              "|undefined|unspecified", 6, 0, 0},
   {"version", "omp_*", 7, 0, 0},
   {"env",    "", 3, 0, 0},
   {"depend", "", 6, 0, 0},
   {"unused", "", 6, 0, 0}
};

/* supported file extensions and types */
static int max_exts = 4;
static struct ftype_s {
   char *ext;
   char *type;
} ftypes[] = {
   {"c",   "C"},
   {"cpp", "C++"},
   {"f",   "F-fixed"},
   {"f90", "F-free"}
};

/* latex macro keys */
static int max_mkeys = 9;
static struct keys_s {
   char *kname;
   int nc;
   int iext;
} type_mkeys[] = {
   {"cnexample",     9, 0},
   {"cppnexample",  11, 1},
   {"fnexample",     9, 2},
   {"ffreenexample",13, 3},
   {"srcnexample",  11,-1},
   {"cexample",      8, 0},
   {"cppexample",   10, 1},
   {"fexample",      8, 2},
   {"ffreeexample", 12, 3}
};

/* match filename (mname) with tname value (tvalue)
   up to file extension (fext).
   return 0 - OK, 1 - mismatched */
int check_name(char *tvalue, char *mname, char *fext, int vflg)
{
   char *mp, *cp;
   int s = 1;

   mp = mname;
   cp = tvalue;
   while (*mp && *cp) {
      /* strict checks for exact match of <name>.<seqno> */
      if (vflg&F_STRICT) {
         if (mp == fext) break;
      }
      else {
         if (*mp == '.') mp++;
         if (*cp == '.') cp++;
      }
      if (!*cp || !*mp) break;
      if (*cp != *mp) break;
      mp++;
      cp++;
   }
   if (*cp == '\0') {
      /* non-strict allows postfix in tname value */
      if (!(vflg&F_STRICT) && (strcmp(mp, "90") == 0 || 
           strcmp(mp, "pp") == 0)) mp += 2;
      if (*mp == '\0' || mp == fext) s = 0;
   }
   return s;
}

/* match ttype value (ttype) with file extension (fext).
   return 0 - OK, 1 - mismatched; iext contains matched ext index */
int check_type(char *ttype, char *fext, int *iext)
{
   int mext, s = 1;
   /* get ext index from ttype value */
   for (mext = max_exts-1; mext >= 0; mext--) {
      if (strcasecmp(ttype, ftypes[mext].ext) == 0 || 
          strcasecmp(ttype, ftypes[mext].type) == 0) break;
   }
   if (mext < 0) {
      if (strcasecmp(ttype, "ffree") == 0)
         mext = 3;
      else if (strcasecmp(ttype, "ffixed") == 0)
         mext = 2;
   }
   *iext = mext;
   if (mext >= 0 && fext && strcasecmp(ftypes[mext].ext, fext+1) == 0)
      s = 0;
   return s;
}

/* check validity of a tag value.
   return 0 - OK, 1 - unexpected */
int check_tvalue(char *tvalue, int itag)
{
   int s = 0;
   if (*tvalue == '\0')
      s = 1;
   else if (itag == vtag_idx) {
      if (strncasecmp(tvalue, "pre_omp", 7) != 0 &&
          strncasecmp(tvalue, tags[itag].tvals, 4) != 0)
         s = 1;
   }
   else if (itag > 1 && itag < etag_idx) {
      char *cp = strcasestr(tags[itag].tvals, tvalue);
      if (!cp)
         s = 1;
   }
   return s;
}

/* fix tags in source file <fname>
   new file written to "<fname>_fix"
   mname - filename without directory name
   fext - file extension
   iext - extension index */
int fix_tags(char *fname, char *mname, char *fext, int iext)
{
   FILE *fp, *fou;
   char buff[BUF_SIZE], oname[BUF_SIZE], *cp;
   int lineno, tcnt, ic, i;

   fp = fopen(fname, "r");
   if (!fp) {
      perror(fname);
      return -1;
   }

   /* fixing name tag value using fname */
   sprintf(oname, "%s_fix", fname);
   fou = fopen(oname, "w");
   if (!fou) {
      perror(oname);
      fclose(fp);
      return -1;
   }
   fprintf(stderr, "*** fixing with - %s\n", oname);
   tcnt = lineno = 0;
   while (fgets(buff, BUF_SIZE, fp)) {
      lineno++;
      if (tcnt > 1) {
         fputs(buff, fou);
         continue;
      }
      cp = strstr(buff, "@@");
      if (!cp) {
         if (tcnt == 1) {
            if (!tags[vtag_idx].c) {
               fprintf(fou, "%c", (iext>1)? '!':'*');
               fprintf(fou, " @@version:\t%s\n", PRE_OMP);
            }
            tcnt = 2;
         }
         fputs(buff, fou);
         continue;
      }
      if (tcnt == 0) tcnt = 1;
      for (i = 0; i < max_tags; i++)
         if (tags[i].r == lineno) break;
      if (i >= max_tags || tags[i].c <= 0) {
         fputs(buff, fou);
         continue;
      }
      skipspc(2, cp);
      ic = tags[i].c;
      cp += ic;
      fprintf(fou, "%.*s", (int)(cp-buff), buff);
      if (ic == tags[i].nc) {
         /* add ':' if missing */
         fputc(':', fou);
         if (*cp == ' ') cp++;
      }
      if (i == 0 || i == 1) {
         while (*cp == ' ' || *cp == '\t') {
            fputc(*cp, fou);
            cp++;
         }
      }
      if (i == 0) {
         /* use filename (no extension) for tname value */
         fprintf(fou, "%.*s\n", (int)(fext-mname), mname);
      }
      else if (i == 1) {
         /* use supported type value */
         if (iext < 0) {
            for (iext = max_exts-1; iext > 0; iext--)
               if (strcasecmp(ftypes[iext].ext, fext+1) == 0) break;
         }
         fprintf(fou, "%s\n", ftypes[iext].type);
      }
      else {
         fputs(cp, fou);
      }
   }
   fclose(fou);

   fclose(fp);

   return 0;
}

/* print filename */
void prn_fname(int *tcnt, char *fname, int vflg)
{
   if ((vflg&F_VFILE) || ((*tcnt)&T_PNAME)) return;
   *tcnt |= T_PNAME;
   fprintf(stderr, "%s\n", fname);
}

/* check line length in a file */
int chk_file(char *fname, int vflg, int lln)
{
   FILE *fp;
   char buff[BUF_SIZE];
   int lineno, tcnt, nc, ntags, ctag;

   fp = fopen(fname, "r");
   if (!fp) {
      perror(fname);
      return -1;
   }
   if (vflg&F_VFILE)
      fprintf(stderr, "file = %s\n", fname);
   tcnt = lineno = ntags = ctag = 0;
   while (fgets(buff, BUF_SIZE, fp)) {
      lineno++;
      if (ctag < 2 && strstr(buff, "@@")) {
         ntags++;
         if (ntags == 1 && *buff != '!') ntags += 2;
         if (ctag == 0) ctag = 1;
         continue;
      }
      else if (ctag == 1)
         ctag = 2;
      nc = strlen(buff);
      while (nc > 1 && strchr(" \t\n", buff[nc-1])) nc--;
      buff[nc] = '\0';
      if (nc > lln) {
         tcnt++;
         if (tcnt == 1 && !(vflg&F_VFILE))
            printf(">> File = %s\n", fname);
         printf("  line %d ", lineno);
         if (ntags > 0) printf("(S-%d)", lineno-ntags);
         printf(": nchars = %d\n", nc);
      }
   }
   fclose(fp);
   return (tcnt+1);
}

/* process one file at a time */
int proc_file(char *fname, int vflg)
{
   FILE *fp;
   char buff[BUF_SIZE], *cp, *ip;
   char *mname, *fext;
   int lineno, tcnt, cnt, ic, iext, i, s, ctag;

   ctag = (vflg & F_CHKTAG);
   fp = fopen(fname, "r");
   if (!fp) {
      if (!ctag) perror(fname);
      return -1;
   }

   /* mname points to the filename without directory */
   mname = strrchr(fname, '/');
   mname = mname? (mname+1) : fname;
   fext = strrchr(mname, '.');
   for (i = 0; i < max_tags; i++) {
      tags[i].r = tags[i].c = 0;
      tags[i].val[0] = '\0';
   }

   /* search for @@<tags> */
   if (vflg&F_VFILE)
      fprintf(stderr, "file = %s\n", fname);
   tcnt = cnt = lineno = 0;
   iext = -1;
   while (fgets(buff, BUF_SIZE, fp)) {
      lineno++;
      cp = strstr(buff, "@@");
      if (!cp) {
         if (tcnt) break;
         continue;
      }
      cnt++;
      if (tcnt == 0) {
         ip = buff;
         skipspc(0, ip);
         if (*ip != '/' && *ip != '!') cnt += 2;
         tcnt = 1;
      }
      skipspc(2, cp);
      for (ic = 0; !strchr(": \t\n", cp[ic]); ic++);
      i = max_tags;
      if (ic >= 3) {
         for (i = 0; i < max_tags; i++) {
            if (tags[i].nc != ic) continue;
            if (strncasecmp(cp, tags[i].name, ic) == 0) break;
         }
      }
      if (i < max_tags && tags[i].c == 0) {
         tags[i].r = lineno;
         if (cp[ic] == ':')
            ic++;
         else if (!ctag) {
            prn_fname(&tcnt, fname, vflg);
            fprintf(stderr, "  missing (:) after @@%s\n", tags[i].name);
            tags[i].c = ic;
            tcnt |= T_MTAGS;
         }
         ip = &cp[ic];
         skipspc(ic, cp);
         ic += (int)(cp - ip);
         s = strlen(cp);
         while (s > 0 && strchr("\t \n", cp[s-1])) s--;
         if (s >= TAG_SIZE) s = TAG_SIZE - 1;
         cp[s] = '\0';
         if (ctag) {
            tags[i].c++;
            strcpy(tags[i].val, cp);
         }
         else if (i == 0 || i == 1) {
            if (i == 0)
               s = check_name(cp, mname, fext, vflg);
            else
               s = check_type(cp, fext, &iext);
            if (s) {
               if (tags[i].c == 0) tags[i].c = ic;
               prn_fname(&tcnt, fname, vflg);
               tcnt |= T_MTAGS;
               fprintf(stderr, "  mis-matched @@%s: %s\n", tags[i].name, cp);
            }
         }
         else {
            s = check_tvalue(cp, i);
            if (s) {
               prn_fname(&tcnt, fname, vflg);
               fprintf(stderr, "  *** unknown value for @@%s: %s\n",
                       tags[i].name, cp);
            }
         }
         if (tags[i].c == 0) tags[i].c = -1;
      }
      else if (ctag)
         tags[i].c++;
      else if (i < etag_idx || i >= max_tags) {
         prn_fname(&tcnt, fname, vflg);
         if (tags[i].c != 0)
            fprintf(stderr, "  *** duplicated tag - @@%.*s (lines %d, %d)\n",
                    ic, cp, tags[i].r, lineno);
         else
            fprintf(stderr, "  *** unrecognized tag - @@%.*s\n", ic, cp);
      }
   }
   fclose(fp);

   if (ctag) return cnt;
   if (!tags[0].c || !tags[1].c) {
      prn_fname(&tcnt, fname, vflg);
      fprintf(stderr, "  *** no");
      if (!tags[0].c) fprintf(stderr, " @@%s", tags[0].name);
      if (!tags[0].c && !tags[1].c) fprintf(stderr, " or");
      if (!tags[1].c) fprintf(stderr, " @@%s", tags[1].name);
      fprintf(stderr, " tag found\n");
   }
   if ((vflg&F_STRICT) && !tags[vtag_idx].c) {
      tcnt |= T_MTAGS;
      prn_fname(&tcnt, fname, vflg);
      fprintf(stderr, "  *** no @@%s tag found\n", tags[vtag_idx].name);
   }

   /* fix tag values when required */
   if ((tcnt&T_MTAGS) && (vflg&F_FIXIT)) {
      fix_tags(fname, mname, fext, iext);
   }
   return tcnt;
}

/* return source file name and tag from macro in kbuf */
int get_mkeys(char *kbuf, char *sname, char *tag)
{
   int i, n, iext;
   char *cp = kbuf;
   while (*cp && *cp != BKS && *cp != '%') cp++;
   if (*cp != BKS) return 0;
   cp++;
   for (i = 0; i < max_mkeys; i++) {
      n = type_mkeys[i].nc;
      if (strncmp(cp, type_mkeys[i].kname, n) == 0) {
         cp += n;
         break;
      }
   }
   if (i >= max_mkeys) return 0;
   skipspc(0, cp);
   if (*cp == '[') {
      cp++;
      n = 4;
      while (*cp && *cp != ']' && *cp != '{')
         tag[n++] = *cp++;
      if (n > 4)
         memcpy(tag, "omp_", 4);
      else
         n = 0;
      tag[n] = '\0';
      if (*cp == ']') cp++;
      skipspc(0, cp);
   }
   else
      tag[0] = '\0';
   if (*cp != '{') return 0;
   cp++;
   n = 0;
   while (*cp && *cp != '}') {
      sname[n++] = *cp++;
   }
   if (!*cp) return 0;
   skipspc(1, cp);
   if (*cp != '{') return 0;
   cp++;
   sname[n++] = '.';
   while (*cp && *cp != '}') {
      sname[n++] = *cp++;
   }
   if (!*cp) return 0;
   iext = type_mkeys[i].iext;
   if (iext < 0) {
      skipspc(1, cp);
      if (*cp != '{') return 0;
      cp++;
      sname[n++] = '.';
      while (*cp && *cp != '}') {
         sname[n++] = *cp++;
      }
      if (!*cp) return 0;
      sname[n] = '\0';
   }
   else {
      sprintf(sname+n, ".%s", ftypes[iext].ext);
   }
   return (i+1);
}

/* check version tags in tex file */
int chk_vtags_file(char *fname, int vflg, int *tc)
{
   char buff[BUF_SIZE], sfname[FN_SIZE], sname[TAG_SIZE], tag[TAG_SIZE];
   char *dp, *stag;
   FILE *fp;
   int k, n, cnt, s;
   fp = fopen(fname, "r");
   if (!fp) {
      perror(fname);
      return -1;
   }
   dp = strrchr(fname, '/');
   if (!dp) dp = fname + strlen(fname);
   cnt = *tc = 0;
   while (fgets(buff, BUF_SIZE, fp)) {
      k = get_mkeys(buff, sname, tag);
      if (k > 0) {
         n = 0;
         if (dp)
            n += sprintf(sfname+n, "%.*s", (int)(dp-fname)+1, fname);
         n += sprintf(sfname+n, "sources/%s", sname);
         if ((vflg & F_VFILE) && (vflg & F_LIST)) {
            printf("%s\n", sfname);
            continue;
         }
         stag = tags[vtag_idx].val;
         s = proc_file(sfname, vflg);
         if (s < 0)
            /*EMPTY*/;
         else if (vflg & F_LIST) {
            char *cp = (*stag)? strstr(stag, "omp_") : NULL;
            if (*stag && strncmp(stag, "omp_", 4) == 0) stag += 4;
            printf("\\newlabel{ex_vtag:%s}{{", sname);
            if (cp) {
              if (cp > stag) printf("%.*s", (int)(cp-stag), stag);
              stag += (int)(cp-stag) + 4;
            }
            printf("%s}{%d}{}{}{}}\n", stag, s);
            continue;
         }
         else {
            if (*tag) (*tc)++;
            if (*stag && strncmp(stag, "pre_", 4) == 0) stag += strlen(stag);
            if (*tag && *stag)
               s = (strcmp(tag, stag) == 0)? 0 : 1;
            else
               s = (*tag != *stag);
         }
         if (!s)
            continue;
         if (cnt++ == 0)
            fprintf(stderr, "file - %s\n", fname);
         fprintf(stderr, "  %s", sfname);
         if (s < 0)
            fprintf(stderr, " *** not exist");
         else
            fprintf(stderr, " [%s]", (*stag)? stag : "none");
         fprintf(stderr, " -- [doc:%s]\n", (*tag)? tag : "none");
      }
   }
   fclose(fp);
   return cnt;
}

/* list tags in one file at a time */
int list_tags_file(char *fname, int vflg)
{
   int i, s;
   char *cp;

   if (proc_file(fname, vflg|F_CHKTAG) < 0) return -1;

   if (vflg & F_HLIST) {
      printf("filename");
      for (i = 0; i < max_tags; i++) {
         printf(",%s", tags[i].name);
      }
      printf("\n");
   }

   printf("%s", fname);
   for (i = 0; i < max_tags; i++) {
      s = 0;
      if (i >= etag_idx) {
         for (cp = tags[i].val; *cp && !strchr(",'\"", *cp); cp++);
         if (*cp) s = 1;
      }
      cp = tags[i].val;
      if (s == 1) {
         printf(",\"");
         for (; *cp; cp++) {
            if (*cp == '"') putchar(*cp);
            putchar(*cp);
         }
         putchar('"');
         if (tags[i].c > 1)
            printf(",%d more %s tags", tags[i].c-1, tags[i].name);
      }
      else
         printf(",%s", cp);
   }
   printf("\n");

   return 0;
}

/* print usage and exit */
void usage(char *pgnam)
{
   printf("chk_tags - v%s\n%s [-options] filename ...\n", VERNO, pgnam);
   printf("options:\n\
  -sc      ; strictly check for \"@@<tag>:\"\n\
  -vtag    ; check version tags in tex file\n\
  -clen<n> ; check line length over a limit (def <n>=%d)\n\
  -fix     ; apply tag fix if needed (to <filename>_fix)\n\
  -list    ; list tags (with -vtag to list version tags)\n\
  -v       ; view filenames\n", LLN_DEF);
   exit(0);
}

/* the driver */
int main(int argc, char *argv[])
{
   int cnt, fcnt = -1, bcnt = -1, scnt = 0, tcnt = 0, vflg = 0, lln = LLN_DEF;
   char *pgnam = strrchr(argv[0], '/');
   pgnam = pgnam? (pgnam+1) : argv[0];
   while (--argc > 0) {
      char *cp = *++argv;
      if (strcmp(cp, "-v") == 0)
         vflg |= F_VFILE;
      else if (strcmp(cp, "-sc") == 0)
         vflg |= F_STRICT;
      else if (strcmp(cp, "-fix") == 0)
         vflg |= (F_STRICT|F_FIXIT);
      else if (strcmp(cp, "-vtag") == 0)
         vflg |= F_CHKTAG;
      else if (strcmp(cp, "-list") == 0)
         vflg |= F_LIST;
      else if (strncmp(cp, "-clen", 5) == 0) {
         vflg |= F_CHKLLN;
         cp += 5;
         if (*cp == '=') cp++;
         if (*cp) {
            lln = atoi(cp);
            if (lln < 2) lln = LLN_DEF;
         }
      }
      else if (strcmp(cp, "-h") == 0)
         usage(pgnam);
      else {
         if (vflg & F_CHKLLN)
            cnt = chk_file(cp, vflg, lln);
         else if (vflg & F_CHKTAG) {
            int tc = 0;
            cnt = chk_vtags_file(cp, vflg, &tc);
            if (cnt > 0) {
               scnt += cnt;
               cnt++;
            }
            tcnt += tc;
         }
         else if (vflg & F_LIST) {
            int vf = vflg;
            if (fcnt < 0) vf |= F_HLIST;
            cnt = list_tags_file(cp, vf);
         }
         else
            cnt = proc_file(cp, vflg);
         if (fcnt < 0) fcnt = 0;
         if (cnt < 0) break;
         fcnt++;
         if (bcnt < 0 && (cnt > 1 || (vflg & F_VLIST) != F_VLIST))
            bcnt = 0;
         if (cnt > 1) bcnt++;
      }
   }
   if (fcnt < 0) usage(pgnam);
   fflush(NULL);
   fprintf(stderr, "Number of files scanned = %d\n", fcnt);
   if ((vflg & F_CHKLLN) && fcnt > 0)
      fprintf(stderr, "Line length limit       = %d\n", lln);
   if (vflg & F_CHKTAG) {
      if (tcnt > 0)
         fprintf(stderr, "Number of version tags  = %d\n", tcnt);
      if (scnt > 0)
         fprintf(stderr, "Number of bad src files = %d\n", scnt);
   }
   if (bcnt >= 0)
      fprintf(stderr, "Number of bad files     = %d\n", bcnt);
   return 0;
}
