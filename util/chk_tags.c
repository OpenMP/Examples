/*
 *  A utility to check validity of @@<tags> in source codes.
 *  The tool can also be used to fix issues when possible.
 *
 *  Version 0.1 checks only @@name and @@type.
 *  Version 0.2 added check for other tags.
 *  Version 0.3 added check for line length.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERNO "0.3"

#define BUF_SIZE 256
#define F_VFILE  0x01
#define F_STRICT 0x02
#define F_FIXIT  0x04
#define F_CHKLLN 0x08
#define T_PNAME  0x02
#define T_MTAGS  0x04

/* list of tags */
static int max_tags = 7;
static struct tags_s {
   char *name;             /* tag name */
   char *tvals;            /* possible values */
   int nc;                 /* size of name */
   int r, c;               /* line number, column number */
} tags[] = {
   {"name", "", 4, 0, 0},
   {"type", "", 4, 0, 0},
   {"compilable", "yes|no|maybe|unknown", 10, 0, 0},
   {"linkable", "yes|no|maybe|unknown", 8, 0, 0},
   {"expect", "success|failure|nothing|compile-time-error|ct-error"
      "|runtime-error|rt-error|undefined-behavior", 6, 0, 0},
   {"version", "omp_*", 7, 0, 0},
   {"requires", "preprocessing", 8, 0, 0}
};

/* supported file extensions and types */
static int max_exts = 4;
static char *fexts[] = {"c", "cpp", "f", "f90"};
static char *types[] = {"C", "C++", "F-fixed", "F-free"};

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
      if (strcasecmp(ttype, fexts[mext]) == 0 || 
          strcasecmp(ttype, types[mext]) == 0) break;
   }
   if (mext < 0) {
      if (strcasecmp(ttype, "ffree") == 0)
         mext = 3;
      else if (strcasecmp(ttype, "ffixed") == 0)
         mext = 2;
   }
   *iext = mext;
   if (mext >= 0 && fext && strcasecmp(fexts[mext], fext+1) == 0)
      s = 0;
   return s;
}

/* check validity of a tag value.
   return 0 - OK, 1 - unexpected */
int check_tvalue(char *tvalue, int itag)
{
   int s = 0;
   if (itag == 5) {
      s = strncasecmp(tvalue, tags[itag].tvals, 4);
   }
   else if (itag >= 2 && itag <= 6) {
      if (!strcasestr(tags[itag].tvals, tvalue)) s = 1;
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
         if (tcnt == 1) tcnt = 2;
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
      for (cp += 2; *cp == ' ' || *cp == '\t'; cp++);
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
               if (strcasecmp(fexts[iext], fext+1) == 0) break;
         }
         fprintf(fou, "%s\n", types[iext]);
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
         printf("  line %d (S-%d): nchars = %d\n", lineno, lineno-ntags, nc);
      }
   }
   fclose(fp);
   return (tcnt+1);
}

/* process one file at a time */
int proc_file(char *fname, int vflg)
{
   FILE *fp;
   char buff[BUF_SIZE], *cp;
   char *mname, *fext;
   int lineno, tcnt, ic, iext, i, s;

   fp = fopen(fname, "r");
   if (!fp) {
      perror(fname);
      return -1;
   }

   /* mname points to the filename without directory */
   mname = strrchr(fname, '/');
   mname = mname? (mname+1) : fname;
   fext = strrchr(mname, '.');
   for (i = 0; i < max_tags; i++)
      tags[i].r = tags[i].c = 0;

   /* search for @@<tags> */
   if (vflg&F_VFILE)
      fprintf(stderr, "file = %s\n", fname);
   tcnt = lineno = 0;
   iext = -1;
   while (fgets(buff, BUF_SIZE, fp)) {
      lineno++;
      cp = strstr(buff, "@@");
      if (!cp) {
         if (tcnt) break;
         continue;
      }
      if (tcnt == 0) tcnt = 1;
      for (cp += 2; *cp == ' ' || *cp == '\t'; cp++);
      for (ic = 0; !strchr(": \t\n", cp[ic]); ic++);
      i = max_tags;
      if (ic >= 4) {
         for (i = 0; i < max_tags; i++) {
            if (tags[i].nc != ic) continue;
            if (strncasecmp(cp, tags[i].name, ic) == 0) break;
         }
      }
      if (i >= max_tags || tags[i].c != 0) {
         prn_fname(&tcnt, fname, vflg);
         if (tags[i].c != 0)
            fprintf(stderr, "\t*** duplicated tag - @@%.*s (lines %d, %d)\n",
                    ic, cp, tags[i].r, lineno);
         else
            fprintf(stderr, "\t*** unrecognized tag - @@%.*s\n", ic, cp);
      }
      else {
         tags[i].r = lineno;
         if (cp[ic] != ':') {
            prn_fname(&tcnt, fname, vflg);
            fprintf(stderr, "\tmissing (:) after @@%s\n", tags[i].name);
            tags[i].c = ic;
            tcnt |= T_MTAGS;
         }
         else
            ic++;
         while (cp[ic] == ' ' || cp[ic] == '\t') ic++;
         cp += ic;
         s = strlen(cp) - 1;
         while (s >= 0 && strchr("\t \n", cp[s])) cp[s--] = '\0';
         if (i == 0 || i == 1) {
            if (i == 0)
               s = check_name(cp, mname, fext, vflg);
            else
               s = check_type(cp, fext, &iext);
            if (s) {
               if (tags[i].c == 0) tags[i].c = ic;
               prn_fname(&tcnt, fname, vflg);
               tcnt |= T_MTAGS;
               fprintf(stderr, "\tmis-matched @@%s: %s\n", tags[i].name, cp);
            }
         }
         else {
            s = check_tvalue(cp, i);
            if (s) {
               prn_fname(&tcnt, fname, vflg);
               fprintf(stderr, "\t*** unknown value for @@%s: %s\n",
                       tags[i].name, cp);
            }
         }
         if (tags[i].c == 0) tags[i].c = -1;
      }
   }
   fclose(fp);

   if (!tags[0].c || !tags[1].c) {
      prn_fname(&tcnt, fname, vflg);
      fprintf(stderr, "\t*** no");
      if (!tags[0].c) fprintf(stderr, " @@%s", tags[0].name);
      if (!tags[0].c && !tags[1].c) fprintf(stderr, " or");
      if (!tags[1].c) fprintf(stderr, " @@%s", tags[1].name);
      fprintf(stderr, " tag found\n");
   }

   /* fix tag values when required */
   if ((tcnt&T_MTAGS) && (vflg&F_FIXIT)) {
      fix_tags(fname, mname, fext, iext);
   }
   return tcnt;
}

/* print usage and exit */
void usage(char *pgnam)
{
   printf("chk_tags - v%s\n%s [-options] filename ...\n", VERNO, pgnam);
   printf("options:\n\
  -v    ; view filenames\n\
  -sc   ; strictly check for \"@@<tag>:\"\n\
  -fix  ; apply tag fix if needed (to <filename>_fix)\n\
  -c<n> ; check line length over a limit (def <n>=75)\n");
   exit(0);
}

/* the driver */
int main(int argc, char *argv[])
{
   int cnt, fcnt = -1, bcnt = 0, vflg = 0, lln = 75;
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
      else if (strncmp(cp, "-c", 2) == 0) {
         vflg |= F_CHKLLN;
         if (cp[2]) {
            lln = atoi(cp+2);
            if (lln < 1) lln = 75;
         }
      }
      else if (strcmp(cp, "-h") == 0)
         usage(pgnam);
      else {
         if (vflg & F_CHKLLN)
            cnt = chk_file(cp, vflg, lln);
         else
            cnt = proc_file(cp, vflg);
         if (fcnt < 0) fcnt = 0;
         if (cnt < 0) break;
         fcnt++;
         if (cnt > 1) bcnt++;
      }
   }
   if (fcnt < 0) usage(pgnam);
   fflush(NULL);
   fprintf(stderr, "Number of files scanned = %d\n", fcnt);
   if ((vflg & F_CHKLLN) && fcnt > 0)
      fprintf(stderr, "Line length limit       = %d\n", lln);
   fprintf(stderr, "Number of bad files     = %d\n", bcnt);
   return 0;
}
