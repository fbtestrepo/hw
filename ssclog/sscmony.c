/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Money page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* dialog controls */

typedef struct dollarpage {
  DIALOG_MESSAGE_BLOCK
  TexT  dollars;
  TexT  cents;
} DollarPage, PNTR DollarPagePtr;

static void ChangeDollarProc (
  TexT t
)

{
  DialogActnFunc  actproc;
  DollarPagePtr   dpp;

  dpp = (DollarPagePtr) GetObjectExtra (t);
  if (dpp == NULL) return;

  actproc = dpp->actproc;
  if (actproc != NULL) {
    actproc (dpp->dialog);
  }
}

static void DollarPtrToDollarPage (
  DialoG d,
  Pointer data
)

{
  Char             cts [8], dls [32];
  DollarAmountPtr  dap;
  DollarPagePtr    dpp;

  dpp = (DollarPagePtr) GetObjectExtra (d);
  if (dpp == NULL) return;
  dap = (DollarAmountPtr) data;
  if (dap != NULL) {
    if (dap->dollars > 0 || dap->cents > 0) {
      dls [0] = '\0';
      if (dap->dollars >= 0) {
        sprintf (dls, "%ld", (long) dap->dollars);
      }
      SetTitle (dpp->dollars, dls);
      cts [0] = '\0';
      if (dap->cents >= 0) {
        sprintf (cts, "%2ld", (long) dap->cents);
        if (cts [0] == ' ') {
          cts [0] = '0';
        }
      }
      SetTitle (dpp->cents, cts);
      return;
    }
  }

  SetTitle (dpp->dollars, "");
  SetTitle (dpp->cents, "");
}

static Pointer DollarPageToDollarPtr (
  DialoG d
)

{
  Char             buf [32];
  Int4             cents, dollars;
  DollarAmountPtr  dap;
  DollarPagePtr    dpp;

  dap = NULL;
  dpp = (DollarPagePtr) GetObjectExtra (d);
  if (dpp == NULL) return NULL;

  dap = DollarAmountNew ();
  if (dap != NULL) {
    dollars = -1;
    GetTitle (dpp->dollars, buf, sizeof (buf));
    if (! StrToLong (buf, &dollars)) {
      dollars = -1;
    }

    cents = -1;
    GetTitle (dpp->cents, buf, sizeof (buf));
    if (StringLen (buf) == 2 && IS_DIGIT (buf [0]) && IS_DIGIT (buf [1])) {
      if (! StrToLong (buf, &cents)) {
        cents = -1;
      }
    }

    if (dollars >= 0 && cents >= 100) {
      dollars += cents / 100;
      cents = cents % 100;
    }

    dap->dollars = dollars;
    dap->cents = cents;

    /*
    if (dap->hour == -1 || dap->minute == -1) {
      dap = DollarAmountFree (dap);
    }
    */
  }

  return (Pointer) dap;
}

extern void GetDollarAmount (
  DialoG d,
  Int4Ptr dollars,
  Int4Ptr cents
)

{
  DollarAmountPtr  dap;

  if (d == NULL || dollars == NULL || cents == NULL) return;

  dap = (DollarAmountPtr) DialogToPointer (d);
  if (dap != NULL) {
    *dollars = dap->dollars;
    *cents = dap->cents;
    dap = DollarAmountFree (dap);
    return;
  }

  *dollars = -1;
  *cents = -1;
}

extern void SetDollarAmount (
  DialoG d,
  Int4 dollars,
  Int4 cents
)

{
  DollarAmount  da;

  if (d == NULL) return;

  da.dollars = dollars;
  da.cents = cents;

  PointerToDialog (d, (Pointer) &da);
}

extern void SelectDollars (
  DialoG d
)

{
  DollarPagePtr  dpp;

  dpp = (DollarPagePtr) GetObjectExtra (d);
  if (dpp == NULL) return;

  Select (dpp->dollars);
}

extern void SelectCents (
  DialoG d
)

{
  DollarPagePtr  dpp;

  dpp = (DollarPagePtr) GetObjectExtra (d);
  if (dpp == NULL) return;

  Select (dpp->cents);
}

extern DialoG CreateDollarAmountDialog (GrouP prnt, Pointer userdata, DialogActnFunc actproc)

{
  DollarPagePtr  dpp;
  GrouP          f;
  GrouP          p;

  p = HiddenGroup (prnt, 1, 0, NULL);

  dpp = (DollarPagePtr) MemNew (sizeof (DollarPage));
  if (dpp) {

    SetObjectExtra (p, dpp, StdCleanupExtraProc);
    dpp->dialog = (DialoG) p;
    dpp->todialog = DollarPtrToDollarPage;
    dpp->fromdialog = DollarPageToDollarPtr;
    dpp->testdialog = NULL;

    dpp->actproc = actproc;
    dpp->userdata = userdata;

    f = HiddenGroup (p, -6, 0, NULL);

    dpp->dollars = DialogText (f, "1000", 0, ChangeDollarProc);
    SetObjectExtra (dpp->dollars, dpp, NULL);

    StaticPrompt (f, ".", 0, dialogTextHeight, programFont, 'l');

    dpp->cents = DialogText (f, "00", 0, ChangeDollarProc);
    SetObjectExtra (dpp->cents, dpp, NULL);

    SetTitle (dpp->dollars, "");
    SetTitle (dpp->cents, "");
  }

  return (DialoG) p;
}

