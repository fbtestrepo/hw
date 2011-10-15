/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Release page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* dialog controls */

typedef struct relspage {
  DIALOG_MESSAGE_BLOCK
  PopuP  thousands;
  PopuP  hundreds;
} RelsPage, PNTR RelsPagePtr;

static ENUM_ALIST(thousands_alist)
  {" ",  -1},
  {"0",   0},
  {"1",   1},
  {"2",   2},
  {"3",   3},
  {"4",   4},
  {"5",   5},
  {"6",   6},
  {"7",   7},
  {"8",   8},
  {"9",   9},
END_ENUM_ALIST

static ENUM_ALIST(hundreds_alist)
  {" ",    -1},
  {"000",   0},
  {"100",   1},
  {"200",   2},
  {"300",   3},
  {"400",   4},
  {"500",   5},
  {"600",   6},
  {"700",   7},
  {"800",   8},
  {"900",   9},
END_ENUM_ALIST

static void ChangeReleaseProc (
  PopuP p
)

{
  DialogActnFunc  actproc;
  RelsPagePtr     rpp;

  rpp = (RelsPagePtr) GetObjectExtra (p);
  if (rpp == NULL) return;

  actproc = rpp->actproc;
  if (actproc != NULL) {
    actproc (rpp->dialog);
  }
}

static void RelsPtrToRelsPage (
  DialoG d,
  Pointer data
)

{
  Int4         rel_height;
  Int4         rel_hund, rel_thou;
  Int4Ptr      relp;
  RelsPagePtr  rpp;

  rpp = (RelsPagePtr) GetObjectExtra (d);
  if (rpp == NULL) return;
  relp = (Int4Ptr) data;
  if (relp != NULL) {
    rel_height = *relp;
    if (rel_height >= 0 && rel_height < 10000) {
      rel_thou = rel_height / 1000;
      rel_hund = (rel_height % 1000) / 100;
      SetEnumPopup (rpp->thousands, thousands_alist, (UIEnum) rel_thou);
      SetEnumPopup (rpp->hundreds, hundreds_alist, (UIEnum) rel_hund);
      return;
    }
  }

  SafeSetValue (rpp->thousands, 1);
  SafeSetValue (rpp->hundreds, 1);
}

static Pointer RelsPageToRelsPtr (
  DialoG d
)

{
  Boolean      bad;
  UIEnum       hundreds, thousands;
  Int4         rel_height;
  Int4Ptr      relp;
  RelsPagePtr  rpp;

  relp = NULL;
  rpp = (RelsPagePtr) GetObjectExtra (d);
  if (rpp == NULL) return NULL;

  rel_height = 0;
  bad = FALSE;

  relp = (Int4Ptr) MemNew (sizeof (Int4));
  if (relp != NULL) {
    if (GetEnumPopup (rpp->thousands, thousands_alist, &thousands) && thousands > -1) {
      rel_height += thousands * 1000;
    } else {
      bad = TRUE;
    }

    if (GetEnumPopup (rpp->hundreds, hundreds_alist, &hundreds)) {
      if (hundreds > -1) {
        rel_height += hundreds * 100;
      } else if (rel_height > 0) {
        SetEnumPopup (rpp->hundreds, hundreds_alist, (UIEnum) 0);
      } else {
        bad = TRUE;
      }
    } else {
      bad = TRUE;
    }

    if (bad) {
      *relp = -1;
    } else {
      *relp = rel_height;
    }
  }

  return (Pointer) relp;
}

extern void GetReleaseHeight (
  DialoG d,
  Int4Ptr height
)

{
  Int4Ptr  relp;

  if (d == NULL || height == NULL) return;

  relp = (Int4Ptr) DialogToPointer (d);
  if (relp != NULL) {
    *height = *relp;
    relp = MemFree (relp);
    return;
  }

  *height = -1;
}

extern void SetReleaseHeight (
  DialoG d,
  Int4 height
)

{
  Int4  rel;

  if (d == NULL) return;

  rel = height;

  PointerToDialog (d, (Pointer) &rel);
}

extern DialoG CreateReleaseHeightDialog (
  GrouP prnt,
  Pointer userdata,
  DialogActnFunc actproc
)

{
  GrouP        f;
  GrouP        p;
  RelsPagePtr  rpp;

  p = HiddenGroup (prnt, 1, 0, NULL);

  rpp = (RelsPagePtr) MemNew (sizeof (RelsPage));
  if (rpp) {

    SetObjectExtra (p, rpp, StdCleanupExtraProc);
    rpp->dialog = (DialoG) p;
    rpp->todialog = RelsPtrToRelsPage;
    rpp->fromdialog = RelsPageToRelsPtr;
    rpp->testdialog = NULL;

    rpp->actproc = actproc;
    rpp->userdata = userdata;

    f = HiddenGroup (p, -6, 0, NULL);

    rpp->thousands = PopupList (f, TRUE, ChangeReleaseProc);
    SetObjectExtra (rpp->thousands, (Pointer) rpp, NULL);
    InitEnumPopup (rpp->thousands, thousands_alist, NULL);
    SetValue (rpp->thousands, 1);

    StaticPrompt (f, ",", 0, popupMenuHeight, programFont, 'l');

    rpp->hundreds = PopupList (f, TRUE, ChangeReleaseProc);
    SetObjectExtra (rpp->hundreds, (Pointer) rpp, NULL);
    InitEnumPopup (rpp->hundreds, hundreds_alist, NULL);
    SetValue (rpp->hundreds, 1);
  }

  return (DialoG) p;
}

