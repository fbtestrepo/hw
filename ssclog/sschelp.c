/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Help page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

static ParData helpParFmt = {FALSE, FALSE, FALSE, FALSE, TRUE, 0, 0};
static ColData helpColFmt = {0, 0, 80, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, TRUE};

extern DoC CreateHelpDialog (
  GrouP prnt,
  Int2 wid
)

{
  helpColFmt.pixWidth = wid - 10;
  helpColFmt.pixInset = 8;

  return DocumentPanel (prnt, wid - 10, 15 * stdLineHeight);
}

extern void PopulateHelpDoc (
  DoC d,
  CharPtr filename
)

{
  FonT  fnt;
  FILE  *fp;
  Char  path [PATH_MAX];

  if (d == NULL || StringHasNoText (filename)) return;

  fp = OpenAFile (filename, path);
  if (fp == NULL) return;
  FileClose (fp);

#ifdef WIN_MSWIN
  fnt = systemFont;
#else
  fnt = programFont;
#endif

  DisplayFancy (d, path, &helpParFmt, &helpColFmt, fnt, 0);
}

