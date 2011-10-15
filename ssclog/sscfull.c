/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Fullial members page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* dialog controls */

typedef struct sscfull {
  DIALOG_MESSAGE_BLOCK

  TexT          name;

  ButtoN        chargeinitfee;
  ButtoN        chargesscdues;
  ButtoN        chargessareg;
  ButtoN        chargessafam;

  DollarAmount  prorateddues;
  DollarAmount  proratedssareg;
  DollarAmount  proratedssafam;

  DialoG        jobs;

  ButtoN        addfull;

  SscTablesPtr  tables;
} SscFull, PNTR SscFullPtr;

static void ChangeFullName (
  TexT t
)

{
  SscFullPtr  sfp;

  sfp = (SscFullPtr) GetObjectExtra (t);
  if (sfp == NULL) return;

  if (TextHasNoText (t)) {
    SafeDisable (sfp->addfull);
  } else {
    SafeEnable (sfp->addfull);
  }
}

extern Boolean AddToFullMemberList (
  SscTablesPtr stp,
  CharPtr name,
  DialoG jobs
)

{
  Boolean     assistant, dutyofficer, instructor, towpilot;
  Char        clubjobs [12];
  CharPtr     str;
  ValNodePtr  vnp;

  if (stp == NULL || StringHasNoText (name)) return FALSE;

  for (vnp = stp->members.names; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    if (StringICmp (str, name) == 0) {
      Message (MSG_OK, "This person is already an active member.");
      return FALSE;
    }
  }

  AddNameToList (&(stp->members), name);
  AddNameToList (&(stp->justfull), name);
  AddNameToList (&(stp->payers), name);

  clubjobs [0] = '\0';

  GetClubJobs (jobs, &dutyofficer, &assistant, &instructor, &towpilot);

  if (dutyofficer) {
    AddNameToExtras (&(stp->dutyofficers), name);
    StringCat (clubjobs, "D");
  }
  if (assistant) {
    AddNameToExtras (&(stp->assistants), name);
    StringCat (clubjobs, "A");
  }
  if (instructor) {
    AddNameToExtras (&(stp->instructors), name);
    AddNameToExtras (&(stp->justcfigs), name);
    StringCat (clubjobs, "I");
  }
  if (towpilot) {
    AddNameToExtras (&(stp->towpilots), name);
    StringCat (clubjobs, "T");
  }

  UpdateStaffControls (stp->staff);
  UpdateFlightControls (stp->glider_flights);
  UpdateFeesControls (stp->fees);
  UpdateChargesControls (stp->charges);
  UpdateAdjustmentsControls (stp->adjustments);
  UpdateBadgesControls (stp->awards);
  UpdateGuestControls (stp->guest_members);

  ClearDirtyFlags (stp);

  AppendFullMemberTable ("members.txt", name, clubjobs);

  return TRUE;
}

static void AddFullMember (
  ButtoN b
)

{
  Char             buf [128];
  DollarAmountPtr  dap;
  MiscRatePtr      mrp;
  SscFullPtr       sfp;
  SscTablesPtr     stp;
  CharPtr          str;
  ValNodePtr       vnp;

  sfp = (SscFullPtr) GetObjectExtra (b);
  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;
  mrp = stp->miscrate_table;
  if (mrp == NULL) return;

  GetTitle (sfp->name, buf, sizeof (buf));
  if (StringHasNoText (buf)) return;
  TrimSpacesAroundString (buf);

  if (! NormalizeAuthorCaps (buf)) return;

  for (vnp = stp->members.names; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    if (StringICmp (str, buf) == 0) {
      Message (MSG_OK, "This person is already an active member.");
      return;
    }
  }

  if (AddToFullMemberList (stp, buf, sfp->jobs)) {
    AddToContactList (stp->new_contacts, buf, FULL_MEMBER);

    if (GetStatus (sfp->chargeinitfee)) {
      dap = mrp->initiation_fee;
      if (dap != NULL) {
        AddFeeItem (stp->fees, buf, 0, NULL, dap->dollars, dap->cents,
                    REG_MEMB_FEE, NULL, NULL);
      }
    }

    if (GetStatus (sfp->chargesscdues)) {
      dap = &(sfp->prorateddues);
      if (dap != NULL) {
        AddFeeItem (stp->fees, buf, 0, NULL, dap->dollars, dap->cents,
                    SSC_DUES_FEE, NULL, NULL);
      }
    }

    if (GetStatus (sfp->chargessareg)) {
      dap = &(sfp->proratedssareg);
      if (dap != NULL) {
        AddFeeItem (stp->fees, buf, 0, NULL, dap->dollars, dap->cents,
                    SSA_DUES_FEE, NULL, NULL);
      }
    }

    if (GetStatus (sfp->chargessafam)) {
      dap = &(sfp->proratedssafam);
      if (dap != NULL) {
        AddFeeItem (stp->fees, buf, 0, NULL, dap->dollars, dap->cents,
                    SSA_DUES_FEE, NULL, NULL);
      }
    }
  }

  SetTitle (sfp->name, "");

  SetStatus (sfp->chargeinitfee, TRUE);
  SetStatus (sfp->chargesscdues, TRUE);
  SetStatus (sfp->chargessareg, TRUE);
  SetStatus (sfp->chargessafam, FALSE);

  SetClubJobs (sfp->jobs, FALSE, FALSE, FALSE, FALSE);

  Disable (b);

  Update ();
}

extern void CreateAddFullDialog (
  GrouP prnt,
  SscTablesPtr stp
)

{
  Char         buf [64];
  GrouP        k, m, p;
  MiscRatePtr  mrp;
  SscFullPtr   sfp;
  Char         str [16];

  if (stp == NULL) return;
  mrp = stp->miscrate_table;
  if (mrp == NULL) return;

  sfp = (SscFullPtr) MemNew (sizeof (SscFull));
  if (sfp == NULL) return;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, sfp, StdCleanupExtraProc);

  sfp->dialog = (DialoG) p;

  sfp->tables = stp;

  k = HiddenGroup (p, 15, 0, NULL);
  SetGroupSpacing (k, 5, 3);

  StaticPrompt (k, "Name", 0, dialogTextHeight, programFont, 'l');
  sfp->name = DialogText (k,
                          "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
                          0, ChangeFullName);
  SetObjectExtra (sfp->name, (Pointer) sfp, NULL);
  SetTitle (sfp->name, "");

  m = HiddenGroup (p, 1, 0, NULL);
  SetGroupSpacing (m, 5, 3);

  if (PrintWholeDollar (mrp->initiation_fee, str)) {
    sprintf (buf, "Charge $%s Initiation Fee (First Half)", str);
  } else {
    sprintf (buf, "Charge Initiation Fee (First Half)");
  }
  sfp->chargeinitfee = CheckBox (m, buf, NULL);
  SetStatus (sfp->chargeinitfee, TRUE);

  ProrateDollarAmount (mrp->monthly_dues, 1, FALSE, stp, &(sfp->prorateddues));

  if (PrintWholeDollar (&(sfp->prorateddues), str)) {
    sprintf (buf, "Charge $%s Dues (Prorated Jan-Dec)", str);
  } else {
    sprintf (buf, "Charge Annual Dues");
  }
  sfp->chargesscdues = CheckBox (m, buf, NULL);
  SetStatus (sfp->chargesscdues, TRUE);

  ProrateDollarAmount (mrp->ssa_regular, 5, TRUE, stp, &(sfp->proratedssareg));

  if (PrintWholeDollar (&(sfp->proratedssareg), str)) {
    sprintf (buf, "Charge $%s SSA Dues (Prorated May-Apr)", str);
  } else {
    sprintf (buf, "Charge SSA Dues");
  }
  sfp->chargessareg = CheckBox (m, buf, NULL);
  SetStatus (sfp->chargessareg, TRUE);

  ProrateDollarAmount (mrp->ssa_family, 5, TRUE, stp, &(sfp->proratedssafam));

  if (PrintWholeDollar (&(sfp->proratedssafam), str)) {
    sprintf (buf, "Charge $%s SSA Family Dues (Prorated)", str);
  } else {
    sprintf (buf, "Charge SSA Family Dues");
  }
  sfp->chargessafam = CheckBox (m, buf, NULL);
  SetStatus (sfp->chargessafam, FALSE);

  sfp->jobs = CreateClubJobsDialog (p);

  sfp->addfull = PushButton (p, "Add Full Member", AddFullMember);
  SetObjectExtra (sfp->addfull, (Pointer) sfp, NULL);
  Disable (sfp->addfull);

  AlignObjects (ALIGN_CENTER,
                (HANDLE) k,
                (HANDLE) m,
                (HANDLE) sfp->jobs,
                (HANDLE) sfp->addfull,
                NULL);
}

