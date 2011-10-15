/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Miscellaneous members page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* dialog controls */

typedef struct sscguest {
  DIALOG_MESSAGE_BLOCK

  TexT          name;

  PopuP         sponsor;

  ButtoN        addguest;

  SscTablesPtr  tables;
} SscGuest, PNTR SscGuestPtr;

extern void UpdateGuestControls (
  DialoG d
)

{
  SscGuestPtr   sgp;
  SscTablesPtr  stp;
  Boolean       was_visible = FALSE;

  sgp = (SscGuestPtr) GetObjectExtra (d);
  if (sgp == NULL) return;
  stp = sgp->tables;
  if (stp == NULL) return;

  if (! stp->justfull.need_to_update) return;

  if (Visible (d) && AllParentsVisible (d)) {
    Hide (d);
    Update ();
    was_visible = TRUE;
  }

  if (stp->justfull.need_to_update) {
    RecreateNameAlist (&(stp->justfull));
    Reset (sgp->sponsor);
    InitEnumPopup (sgp->sponsor, stp->justfull.alist, NULL);
    SetValue (sgp->sponsor, 0);
  }

  if (was_visible) {
    Show (d);
    Update ();
  }
}

static void ChangeGuestName (
  TexT t
)

{
  SscGuestPtr   sgp;
  SscTablesPtr  stp;
  CharPtr       str;

  sgp = (SscGuestPtr) GetObjectExtra (t);
  if (sgp == NULL) return;
  stp = sgp->tables;
  if (stp == NULL) return;

  str = GetEnumPopupByName (sgp->sponsor, stp->justfull.alist);

  if (StringHasNoText (str) ||
      StringCmp (str, "-") == 0 ||
      TextHasNoText (t)) {
    SafeDisable (sgp->addguest);
  } else {
    SafeEnable (sgp->addguest);
  }

  MemFree (str);
}

static void ChangeSponsor (
  PopuP p
)

{
  AlistDialogPtr  adp;
  SscGuestPtr     sgp;
  SscTablesPtr    stp;
  CharPtr         str;

  adp = (AlistDialogPtr) GetObjectExtra (p);
  if (adp == NULL) return;
  sgp = (SscGuestPtr) adp->userdata;
  if (sgp == NULL) return;
  stp = sgp->tables;
  if (stp == NULL) return;

  str = GetEnumPopupByName (sgp->sponsor, stp->justfull.alist);

  if (StringHasNoText (str) ||
      StringCmp (str, "-") == 0 ||
      TextHasNoText (sgp->name)) {
    SafeDisable (sgp->addguest);
  } else {
    SafeEnable (sgp->addguest);

  }
  MemFree (str);
}

static void AddGuest (
  ButtoN b
)

{
  Char          buf [128];
  SscGuestPtr   sgp;
  SscTablesPtr  stp;

  sgp = (SscGuestPtr) GetObjectExtra (b);
  if (sgp == NULL) return;
  stp = sgp->tables;
  if (stp == NULL) return;

  ClearDirtyFlags (stp);

  GetTitle (sgp->name, buf, sizeof (buf));
  if (StringHasNoText (buf)) return;
  TrimSpacesAroundString (buf);

  if (! NormalizeAuthorCaps (buf)) return;

  ClearDirtyFlags (stp);

  AddNameToExtras (&(stp->members), buf);
  AddNameToExtras (&(stp->payers), buf);

  UpdateFlightControls (stp->glider_flights);
  UpdateFeesControls (stp->fees);
  UpdateChargesControls (stp->charges);
  UpdateAdjustmentsControls (stp->adjustments);

  ClearDirtyFlags (stp);

  SetTitle (sgp->name, "");
  SetValue (sgp->sponsor, 0);

  Disable (b);

  Update ();
}

extern DialoG CreateAddGuestDialog (
  GrouP prnt,
  SscTablesPtr stp
)

{
  GrouP        n, m, p, q;
  SscGuestPtr  sgp;

  if (stp == NULL) return NULL;

  sgp = (SscGuestPtr) MemNew (sizeof (SscGuest));
  if (sgp == NULL) return NULL;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, sgp, StdCleanupExtraProc);

  sgp->dialog = (DialoG) p;

  sgp->tables = stp;

  m = HiddenGroup (p, 1, 0, NULL);
  SetGroupSpacing (m, 5, 3);

  n = HiddenGroup (m, 15, 0, NULL);
  SetGroupSpacing (n, 5, 3);

  StaticPrompt (n, "Name", 0, dialogTextHeight, programFont, 'l');
  sgp->name = DialogText (n, "XXXXXXXXXXXXXXXXXXXXXXXXX",
                                  0, ChangeGuestName);
  SetObjectExtra (sgp->name, (Pointer) sgp, NULL);
  SetTitle (sgp->name, "");

  q = HiddenGroup (m, 15, 0, NULL);
  SetGroupSpacing (q, 5, 3);

  StaticPrompt (q, "Sponsor", 0, popupMenuHeight, programFont, 'l');
  sgp->sponsor = CreateEnumPopupDialog (q, TRUE, ChangeSponsor,
                                        stp->justfull.alist, (UIEnum) 0, sgp);

  sgp->addguest = PushButton (p, "Add Guest", AddGuest);
  SetObjectExtra (sgp->addguest, (Pointer) sgp, NULL);
  Disable (sgp->addguest);

  AlignObjects (ALIGN_CENTER, (HANDLE) m,
                (HANDLE) sgp->addguest, NULL);

  return (DialoG) p;
}

typedef struct sscpayer {
  DIALOG_MESSAGE_BLOCK

  TexT          name;

  PopuP         sponsor;

  ButtoN        addpayer;

  SscTablesPtr  tables;
} SscPayer, PNTR SscPayerPtr;

static void ChangePayerName (
  TexT t
)

{
  SscPayerPtr   spp;
  SscTablesPtr  stp;

  spp = (SscPayerPtr) GetObjectExtra (t);
  if (spp == NULL) return;
  stp = spp->tables;
  if (stp == NULL) return;

  if (TextHasNoText (t)) {
    SafeDisable (spp->addpayer);
  } else {
    SafeEnable (spp->addpayer);
  }
}

static void AddPayer (
  ButtoN b
)

{
  Char          buf [128];
  SscPayerPtr   spp;
  SscTablesPtr  stp;

  spp = (SscPayerPtr) GetObjectExtra (b);
  if (spp == NULL) return;
  stp = spp->tables;
  if (stp == NULL) return;

  ClearDirtyFlags (stp);

  GetTitle (spp->name, buf, sizeof (buf));
  if (StringHasNoText (buf)) return;
  TrimSpacesAroundString (buf);

  if (! NormalizeAuthorCaps (buf)) return;

  ClearDirtyFlags (stp);

  AddNameToExtras (&(stp->payers), buf);

  UpdateFlightControls (stp->glider_flights);
  UpdateFeesControls (stp->fees);
  UpdateChargesControls (stp->charges);
  UpdateAdjustmentsControls (stp->adjustments);

  ClearDirtyFlags (stp);

  SetTitle (spp->name, "");

  Disable (b);

  Update ();
}

extern void CreateAddPayerDialog (
  GrouP prnt,
  SscTablesPtr stp
)

{
  GrouP        n, p;
  SscPayerPtr  spp;

  if (stp == NULL) return;

  spp = (SscPayerPtr) MemNew (sizeof (SscPayer));
  if (spp == NULL) return;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, spp, StdCleanupExtraProc);

  spp->dialog = (DialoG) p;

  spp->tables = stp;

  n = HiddenGroup (p, 15, 0, NULL);
  SetGroupSpacing (n, 5, 3);

  StaticPrompt (n, "Name", 0, dialogTextHeight, programFont, 'l');
  spp->name = DialogText (n, "XXXXXXXXXXXXXXXXXXXXXXXXX",
                          0, ChangePayerName);
  SetObjectExtra (spp->name, (Pointer) spp, NULL);
  SetTitle (spp->name, "");

  spp->addpayer = PushButton (p, "Add Payer", AddPayer);
  SetObjectExtra (spp->addpayer, (Pointer) spp, NULL);
  Disable (spp->addpayer);

  AlignObjects (ALIGN_CENTER, (HANDLE) n,
                (HANDLE) spp->addpayer, NULL);
}

typedef struct sscstaff {
  DIALOG_MESSAGE_BLOCK

  TexT          name;

  DialoG        jobs;

  ButtoN        addstaff;

  SscTablesPtr  tables;
} SscStaff, PNTR SscStaffPtr;

static void ChangeStaffName (
  TexT t
)

{
  SscStaffPtr   ssp;
  SscTablesPtr  stp;

  ssp = (SscStaffPtr) GetObjectExtra (t);
  if (ssp == NULL) return;
  stp = ssp->tables;
  if (stp == NULL) return;

  if (TextHasNoText (t)) {
    SafeDisable (ssp->addstaff);
  } else {
    SafeEnable (ssp->addstaff);
  }
}

static void AddStaff (
  ButtoN b
)

{
  Boolean       assistant, dutyofficer, instructor, towpilot;
  Char          buf [128];
  SscStaffPtr   ssp;
  SscTablesPtr  stp;

  ssp = (SscStaffPtr) GetObjectExtra (b);
  if (ssp == NULL) return;
  stp = ssp->tables;
  if (stp == NULL) return;

  ClearDirtyFlags (stp);

  GetTitle (ssp->name, buf, sizeof (buf));
  if (StringHasNoText (buf)) return;
  TrimSpacesAroundString (buf);

  if (! NormalizeAuthorCaps (buf)) return;

  ClearDirtyFlags (stp);

  GetClubJobs (ssp->jobs, &dutyofficer, &assistant, &instructor, &towpilot);

  if (dutyofficer) {
    AddNameToExtras (&(stp->dutyofficers), buf);
  }
  if (assistant) {
    AddNameToExtras (&(stp->assistants), buf);
  }
  if (instructor) {
    AddNameToExtras (&(stp->instructors), buf);
    AddNameToExtras (&(stp->justcfigs), buf);
  }
  if (towpilot) {
    AddNameToExtras (&(stp->towpilots), buf);
  }

  UpdateStaffControls (stp->staff);
  UpdateFlightControls (stp->glider_flights);
  UpdateFeesControls (stp->fees);
  UpdateChargesControls (stp->charges);
  UpdateAdjustmentsControls (stp->adjustments);

  ClearDirtyFlags (stp);

  SetTitle (ssp->name, "");

  SetClubJobs (ssp->jobs, FALSE, FALSE, FALSE, FALSE);

  Disable (b);

  Update ();
}

extern void CreateAddStaffDialog (
  GrouP prnt,
  SscTablesPtr stp
)

{
  GrouP        m, n, p;
  SscStaffPtr  ssp;

  if (stp == NULL) return;

  ssp = (SscStaffPtr) MemNew (sizeof (SscStaff));
  if (ssp == NULL) return;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, ssp, StdCleanupExtraProc);

  ssp->dialog = (DialoG) p;

  ssp->tables = stp;

  n = HiddenGroup (p, 15, 0, NULL);
  SetGroupSpacing (n, 5, 3);

  StaticPrompt (n, "Name", 0, dialogTextHeight, programFont, 'l');
  ssp->name = DialogText (n, "XXXXXXXXXXXXXXXXXXXXXXXXX",
                          0, ChangeStaffName);
  SetObjectExtra (ssp->name, (Pointer) ssp, NULL);
  SetTitle (ssp->name, "");

  m = HiddenGroup (p, 1, 0, NULL);
  SetGroupSpacing (m, 5, 3);

  ssp->jobs = CreateClubJobsDialog (p);

  ssp->addstaff = PushButton (p, "Add Staff", AddStaff);
  SetObjectExtra (ssp->addstaff, (Pointer) ssp, NULL);
  Disable (ssp->addstaff);

  AlignObjects (ALIGN_CENTER,
                (HANDLE) n,
                (HANDLE) m,
                (HANDLE) ssp->jobs,
                (HANDLE) ssp->addstaff,
                NULL);
}

