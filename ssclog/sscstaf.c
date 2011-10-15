/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Staff page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* dialog controls */

typedef struct sscstaff {
  DIALOG_MESSAGE_BLOCK

  PopuP         dutyofficer;
  PopuP         assistant;
  PopuP         timekeeper;
  PopuP         instructor;
  PopuP         am_towpilot;
  PopuP         pm_towpilot;

  SscTablesPtr  tables;
} SscStaff, PNTR SscStaffPtr;

static void ChangeStaff (
  PopuP p
)

{
  AlistDialogPtr  adp;
  SscStaffPtr     ssp;

  adp = (AlistDialogPtr) GetObjectExtra (p);
  if (adp == NULL) return;
  ssp = (SscStaffPtr) adp->userdata;
  if (ssp == NULL) return;

  SetDirtyFlag (ssp->tables);
}

extern void UpdateStaffControls (
  DialoG d
)

{
  SscStaffPtr   ssp;
  SscTablesPtr  stp;
  Boolean       was_visible = FALSE;

  ssp = (SscStaffPtr) GetObjectExtra (d);
  if (ssp == NULL) return;
  stp = ssp->tables;
  if (stp == NULL) return;

  if ((! stp->members.need_to_update) &&
      (! stp->justcfigs.need_to_update) &&
      (! stp->towpilots.need_to_update) &&
      (! stp->dutyofficers.need_to_update) &&
      (! stp->assistants.need_to_update)) return;

  if (Visible (d) && AllParentsVisible (d)) {
    Hide (d);
    Update ();
    was_visible = TRUE;
  }

  if (stp->members.need_to_update) {
    RecreateNameAlist (&(stp->members));
    if (ssp->timekeeper != NULL) {
      Reset (ssp->timekeeper);
      InitEnumPopup (ssp->timekeeper, stp->members.alist, NULL);
      SetValue (ssp->timekeeper, 0);
    }
  }

  if (stp->justcfigs.need_to_update) {
    RecreateNameAlist (&(stp->justcfigs));
    Reset (ssp->instructor);
    InitEnumPopup (ssp->instructor, stp->justcfigs.alist, NULL);
    SetValue (ssp->instructor, 0);
  }

  if (stp->towpilots.need_to_update) {
    RecreateNameAlist (&(stp->towpilots));
    Reset (ssp->am_towpilot);
    InitEnumPopup (ssp->am_towpilot, stp->towpilots.alist, NULL);
    SetValue (ssp->am_towpilot, 0);
    Reset (ssp->pm_towpilot);
    InitEnumPopup (ssp->pm_towpilot, stp->towpilots.alist, NULL);
    SetValue (ssp->pm_towpilot, 0);
  }

  if (stp->dutyofficers.need_to_update) {
    RecreateNameAlist (&(stp->dutyofficers));
    Reset (ssp->dutyofficer);
    InitEnumPopup (ssp->dutyofficer, stp->dutyofficers.alist, NULL);
    SetValue (ssp->dutyofficer, 0);
  }

  if (stp->assistants.need_to_update) {
    RecreateNameAlist (&(stp->assistants));
    Reset (ssp->assistant);
    InitEnumPopup (ssp->assistant, stp->assistants.alist, NULL);
    SetValue (ssp->assistant, 0);
  }

  if (was_visible) {
    Show (d);
    Update ();
  }
}

static void StaffInfoPtrToStaffPage (DialoG d, Pointer data)

{
  StaffDataPtr  sdp;
  SscStaffPtr   ssp;
  SscTablesPtr  stp;

  ssp = (SscStaffPtr) GetObjectExtra (d);
  if (ssp == NULL) return;
  stp = ssp->tables;
  if (stp == NULL) return;
  sdp = (StaffDataPtr) data;

  if (sdp == NULL) {
    SetValue (ssp->dutyofficer, 0);
    SetValue (ssp->assistant, 0);
    SetValue (ssp->timekeeper, 0);
    SetValue (ssp->instructor, 0);
    SetValue (ssp->am_towpilot, 0);
    SetValue (ssp->pm_towpilot, 0);
    return;
  }

  AddNameToExtras (&(stp->dutyofficers), sdp->dutyofficer);
  AddNameToExtras (&(stp->assistants), sdp->assistant);
  AddNameToExtras (&(stp->members), sdp->timekeeper);
  AddNameToExtras (&(stp->justcfigs), sdp->instructor);
  AddNameToExtras (&(stp->towpilots), sdp->towpilot);
  AddNameToExtras (&(stp->towpilots), sdp->am_towpilot);
  AddNameToExtras (&(stp->towpilots), sdp->pm_towpilot);

  UpdateStaffControls (d);

  if (StringDoesHaveText (sdp->dutyofficer)) {
    SetEnumPopupByName (ssp->dutyofficer, stp->dutyofficers.alist, sdp->dutyofficer);
  } else {
    SetValue (ssp->dutyofficer, 0);
  }
  if (StringDoesHaveText (sdp->assistant)) {
    SetEnumPopupByName (ssp->assistant, stp->assistants.alist, sdp->assistant);
  } else {
    SetValue (ssp->assistant, 0);
  }
  if (ssp->timekeeper != NULL) {
    if (StringDoesHaveText (sdp->timekeeper)) {
      SetEnumPopupByName (ssp->timekeeper, stp->members.alist, sdp->timekeeper);
    } else {
      SetValue (ssp->timekeeper, 0);
    }
  }
  if (StringDoesHaveText (sdp->instructor)) {
    SetEnumPopupByName (ssp->instructor, stp->justcfigs.alist, sdp->instructor);
  } else {
    SetValue (ssp->instructor, 0);
  }
  if (StringDoesHaveText (sdp->towpilot)) {
    SetEnumPopupByName (ssp->am_towpilot, stp->towpilots.alist, sdp->towpilot);
    SetEnumPopupByName (ssp->pm_towpilot, stp->towpilots.alist, sdp->towpilot);
  } else {
    if (StringDoesHaveText (sdp->am_towpilot)) {
      SetEnumPopupByName (ssp->am_towpilot, stp->towpilots.alist, sdp->am_towpilot);
    } else {
      SetValue (ssp->am_towpilot, 0);
    }
    if (StringDoesHaveText (sdp->pm_towpilot)) {
      SetEnumPopupByName (ssp->pm_towpilot, stp->towpilots.alist, sdp->pm_towpilot);
    } else {
      SetValue (ssp->pm_towpilot, 0);
    }
  }

  Update ();
}

static Pointer StaffPageToStaffInfoPtr (DialoG d)

{
  StaffDataPtr  sdp;
  SscStaffPtr   ssp;
  SscTablesPtr  stp;

  ssp = (SscStaffPtr) GetObjectExtra (d);
  if (ssp == NULL) return NULL;
  stp = ssp->tables;
  if (stp == NULL) return NULL;

  sdp = StaffDataNew ();
  if (sdp == NULL) return NULL;

  sdp->dutyofficer = GetEnumPopupByName (ssp->dutyofficer, stp->dutyofficers.alist);
  sdp->assistant = GetEnumPopupByName (ssp->assistant, stp->assistants.alist);
  if (ssp->timekeeper != NULL) {
    sdp->timekeeper = GetEnumPopupByName (ssp->timekeeper, stp->members.alist);
  }
  sdp->instructor = GetEnumPopupByName (ssp->instructor, stp->justcfigs.alist);
  sdp->am_towpilot = GetEnumPopupByName (ssp->am_towpilot, stp->towpilots.alist);
  sdp->pm_towpilot = GetEnumPopupByName (ssp->pm_towpilot, stp->towpilots.alist);

  if (StringHasNoText (sdp->dutyofficer)) {
    sdp->dutyofficer = MemFree (sdp->dutyofficer);
  }
  if (StringHasNoText (sdp->assistant)) {
    sdp->assistant = MemFree (sdp->assistant);
  }
  if (StringHasNoText (sdp->timekeeper)) {
    sdp->timekeeper = MemFree (sdp->timekeeper);
  }
  if (StringHasNoText (sdp->instructor)) {
    sdp->instructor = MemFree (sdp->instructor);
  }
  if (StringHasNoText (sdp->am_towpilot)) {
    sdp->am_towpilot = MemFree (sdp->am_towpilot);
  }
  if (StringHasNoText (sdp->pm_towpilot)) {
    sdp->pm_towpilot = MemFree (sdp->pm_towpilot);
  }

  if (StringHasNoText (sdp->dutyofficer) &&
      StringHasNoText (sdp->assistant) &&
      StringHasNoText (sdp->timekeeper) &&
      StringHasNoText (sdp->instructor) &&
      StringHasNoText (sdp->am_towpilot) &&
      StringHasNoText (sdp->pm_towpilot)) {
    sdp = StaffDataFree (sdp);
  }

  return (Pointer) sdp;
}

extern DialoG CreateStaffDialog (
  GrouP prnt,
  SscTablesPtr stp
)

{
  FonT         fnt;
  GrouP        g, p;
  SscStaffPtr  ssp;

  ssp = (SscStaffPtr) MemNew (sizeof (SscStaff));
  if (ssp == NULL) return NULL;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, ssp, StdCleanupExtraProc);
  ssp->dialog = (DialoG) p;
  ssp->todialog = StaffInfoPtrToStaffPage;
  ssp->fromdialog = StaffPageToStaffInfoPtr;

  ssp->tables = stp;

  g = HiddenGroup (p, 2, 0, NULL);
  SetGroupSpacing (g, 10, 5);
  SetGroupMargins (g, 5, 5);

#ifdef WIN_MSWIN
  fnt = systemFont;
#else
  fnt = programFont;
#endif

  StaticPrompt (g, "Duty Officer", 0, popupMenuHeight, fnt, 'l');
  ssp->dutyofficer = CreateEnumPopupDialog (g, TRUE, ChangeStaff, stp->dutyofficers.alist, (UIEnum) 0, ssp);
  StaticPrompt (g, "Assistant", 0, popupMenuHeight, fnt, 'l');
  ssp->assistant = CreateEnumPopupDialog (g, TRUE, ChangeStaff, stp->assistants.alist, (UIEnum) 0, ssp);
  /*
  StaticPrompt (g, "Time Keeper", 0, popupMenuHeight, fnt, 'l');
  ssp->timekeeper = CreateEnumPopupDialog (g, TRUE, ChangeStaff, stp->members.alist, (UIEnum) 0, ssp);
  */
  StaticPrompt (g, "Instructor", 0, popupMenuHeight, fnt, 'l');
  ssp->instructor = CreateEnumPopupDialog (g, TRUE, ChangeStaff, stp->justcfigs.alist, (UIEnum) 0, ssp);
  StaticPrompt (g, "Tow Pilot 1", 0, popupMenuHeight, fnt, 'l');
  ssp->am_towpilot = CreateEnumPopupDialog (g, TRUE, ChangeStaff, stp->towpilots.alist, (UIEnum) 0, ssp);
  StaticPrompt (g, "Tow Pilot 2", 0, popupMenuHeight, fnt, 'l');
  ssp->pm_towpilot = CreateEnumPopupDialog (g, TRUE, ChangeStaff, stp->towpilots.alist, (UIEnum) 0, ssp);

  return (DialoG) p;
}

