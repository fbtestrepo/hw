/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Airport page

     Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* dialog controls */

typedef struct sscarpt {
  DIALOG_MESSAGE_BLOCK

  PopuP         field;

  SscTablesPtr  tables;
} AprtData, PNTR AprtDataPtr;

static void ChangeAirportName (
  PopuP p
)

{
  AlistDialogPtr  adp;
  AprtDataPtr     fdp;
  SscTablesPtr    stp;

  adp = (AlistDialogPtr) GetObjectExtra (p);
  if (adp == NULL) return;
  fdp = (AprtDataPtr) adp->userdata;
  if (fdp == NULL) return;
  stp = fdp->tables;
  if (stp == NULL) return;

  SetDirtyFlag (fdp->tables);
}

static void AirportNameToAirportPage (DialoG d, Pointer data)

{
  AprtDataPtr  fdp;
  SscTablesPtr  stp;
  CharPtr       str;

  fdp = (AprtDataPtr) GetObjectExtra (d);
  if (fdp == NULL) return;
  stp = fdp->tables;
  if (stp == NULL) return;
  str = (CharPtr) data;

  if (StringDoesHaveText (str)) {
    SetEnumPopupByName (fdp->field, stp->field_alist, str);
  } else {
    SetValue (fdp->field, 0);
  }

  ChangeAirportName (fdp->field);

  Update ();
}

static Pointer AirportPageToAirportName (DialoG d)

{
  AprtDataPtr  fdp;
  SscTablesPtr  stp;
  CharPtr       str;

  fdp = (AprtDataPtr) GetObjectExtra (d);
  if (fdp == NULL) return NULL;
  stp = fdp->tables;
  if (stp == NULL) return NULL;

  str = GetEnumPopupByName (fdp->field, stp->field_alist);
  if (StringHasNoText (str)) {
    str = MemFree (str);
  }

  return (Pointer) str;
}

extern DialoG CreateAirportDialog (
  GrouP prnt,
  SscTablesPtr stp
)

{
  AprtDataPtr  fdp;
  FonT         fnt;
  GrouP        g, p;

  fdp = (AprtDataPtr) MemNew (sizeof (AprtData));
  if (fdp == NULL) return NULL;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, fdp, StdCleanupExtraProc);
  fdp->dialog = (DialoG) p;
  fdp->todialog = AirportNameToAirportPage;
  fdp->fromdialog = AirportPageToAirportName;

  fdp->tables = stp;

  g = HiddenGroup (p, 2, 0, NULL);
  SetGroupSpacing (g, 10, 5);
  SetGroupMargins (g, 5, 5);

#ifdef WIN_MSWIN
  fnt = systemFont;
#else
  fnt = programFont;
#endif

  StaticPrompt (g, "Airport", 0, popupMenuHeight, fnt, 'l');
  fdp->field = CreateEnumPopupDialog (g, TRUE, ChangeAirportName, stp->field_alist, (UIEnum) 0, fdp);

  return (DialoG) p;
}

