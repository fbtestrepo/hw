/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Main section

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include <ncbi.h>
#include <asn.h>

#define NLM_GENERATED_CODE_PROTO 1

#include "objssc.h"
#include "sscconv.h"

typedef struct ctldata {
  CharPtr  directory;
  CharPtr  infile;
  CharPtr  outfile;
  CharPtr  results;
} CtlData, PNTR CtlDataPtr;

static void XMLizeOneRecord (
  CharPtr filename,
  Pointer userdata
)

{
  AsnIoPtr     aip;
  CtlDataPtr   cdp;
  LogSheetPtr  lsp;
  Char         path [PATH_MAX];
  CharPtr      ptr;

  if (StringHasNoText (filename)) return;

  cdp = (CtlDataPtr) userdata;
  if (cdp == NULL) return;

  if (StringDoesHaveText (cdp->results)) {
    StringNCpy_0 (path, cdp->results, sizeof (path));
    ptr = StringRChr (filename, DIRDELIMCHR);
    if (ptr == NULL) return;
    ptr++;
    FileBuildPath (path, NULL, ptr);
    ptr = StringStr (path, ".txt");
    if (ptr == NULL) return;
    *ptr = '\0';
    StringCat (path, ".xml");
    if (FileLength (path) > 0) return;
  } else if (StringDoesHaveText (cdp->outfile)) {
    StringNCpy_0 (path, cdp->outfile, sizeof (path));
  } else return;

  aip = AsnIoOpen (filename, "r");
  if (aip == NULL) return;
  lsp = LogSheetAsnRead (aip, NULL);
  AsnIoClose (aip);

  if (lsp == NULL) return;

  ModernizeLogSheet (lsp);

  aip = AsnIoOpen (path, "wx");
  if (aip != NULL) {
    ExpandLogSheet (lsp);
    LogSheetAsnWrite (lsp, aip, NULL);
    AsnIoClose (aip);
  }

  LogSheetFree (lsp);
}

#define p_argInputPath   0
#define r_argOutputPath  1
#define i_argInputFile   2
#define o_argOutputFile  3

Args myargs [] = {
  {"Path to Files", NULL, NULL, NULL,
    TRUE, 'p', ARG_STRING, 0.0, 0, NULL},
  {"Path for Results", NULL, NULL, NULL,
    TRUE, 'r', ARG_STRING, 0.0, 0, NULL},
  {"Single Input File", "stdin", NULL, NULL,
    TRUE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  {"Single Output File", NULL, NULL, NULL,
    TRUE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
};

Int2 Main (void)

{
  CtlData  cd;

  ErrSetFatalLevel (SEV_MAX);
  ErrClearOptFlags (EO_SHOW_USERSTR);
  ErrPathReset ();

  if (! GetArgs ("SscAsn2Xml", sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  MemSet ((Pointer) &cd, 0, sizeof (CtlData));

  cd.directory = (CharPtr) myargs [p_argInputPath].strvalue;
  cd.results = (CharPtr) myargs [r_argOutputPath].strvalue;
  if (StringHasNoText (cd.results)) {
    cd.results = cd.directory;
  }
  cd.infile = (CharPtr) myargs [i_argInputFile].strvalue;
  cd.outfile = (CharPtr) myargs [o_argOutputFile].strvalue;

  if (StringDoesHaveText (cd.directory)) {
    DirExplore (cd.directory, NULL, ".txt", FALSE, XMLizeOneRecord, (Pointer) &cd);
  } else if (StringDoesHaveText (cd.infile)) {
    XMLizeOneRecord (cd.infile, (Pointer) &cd);
  }

  return 0;
}

