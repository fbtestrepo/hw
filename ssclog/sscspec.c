/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Special members page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* dialog controls */

typedef struct sscspec {
  DIALOG_MESSAGE_BLOCK

  GrouP         whichspec;

  GrouP         namegroup;
  TexT          name;

  GrouP         controls [5];
  Int2          currPage;

  ButtoN        visitfee;

  ButtoN        affilfee;

  ButtoN        transfee;
  ButtoN        transssa;

  DialoG        jobs;

  DollarAmount  proratedtrans;
  DollarAmount  proratedssa;

  ButtoN        addspec;

  SscTablesPtr  tables;
} SscSpec, PNTR SscSpecPtr;

#define EMPTY_PAGE      0
#define ADD_VISIT_PAGE    1
#define ADD_AFFIL_PAGE   2
#define ADD_TRANS_FEE     3

static void ChangeSpecType (
  GrouP g
)

{
  SscSpecPtr  ssp;

  ssp = (SscSpecPtr) GetObjectExtra (g);
  if (ssp == NULL) return;

  SafeHide (ssp->controls [ssp->currPage]);
  Update ();

  ssp->currPage = GetValue (g);
  SafeShow (ssp->controls [ssp->currPage]);
  SafeShow (ssp->namegroup);
  Select (ssp->name);
  Update ();
}

static void ChangeSpecName (
  TexT t
)

{
  SscSpecPtr  ssp;

  ssp = (SscSpecPtr) GetObjectExtra (t);
  if (ssp == NULL) return;

  if (TextHasNoText (t)) {
    SafeDisable (ssp->addspec);
  } else {
    SafeEnable (ssp->addspec);
  }
}

static void AddSpecMember (
  ButtoN b
)

{
  Char             buf [128];
  DollarAmountPtr  dap;
  Int2             days = 0;
  MiscRatePtr      mrp;
  SscSpecPtr       ssp;
  SscTablesPtr     stp;
  Int2             which;

  ssp = (SscSpecPtr) GetObjectExtra (b);
  if (ssp == NULL) return;
  stp = ssp->tables;
  if (stp == NULL) return;
  mrp = stp->miscrate_table;
  if (mrp == NULL) return;

  GetTitle (ssp->name, buf, sizeof (buf));
  if (StringHasNoText (buf)) return;
  TrimSpacesAroundString (buf);

  if (! NormalizeAuthorCaps (buf)) return;

  which = GetValue (ssp->whichspec);
  if (which < 1) return;

  switch (which) {
    case 1 :
      days = mrp->special_memb_days;
      if (days < 0) {
        days = 0;
      }
      if (AddToTempMemberList (stp, buf, days)) {
        AddToContactList (stp->new_contacts, buf, VISITING_MEMBER);

        if (GetStatus (ssp->visitfee)) {
          dap = mrp->visiting_fee;
          if (dap != NULL) {
            AddFeeItem (stp->fees, buf, 0, NULL, dap->dollars, dap->cents,
                        VISITING_MEMB_FEE, NULL, NULL);
          }
        }
      }
      break;
    case 2 :
      days = mrp->special_memb_days;
      if (days < 0) {
        days = 0;
      }
      if (AddToTempMemberList (stp, buf, days)) {
        AddToContactList (stp->new_contacts, buf, AFFIL_MEMBER);

        if (GetStatus (ssp->affilfee)) {
          dap = mrp->affiliated_fee;
          if (dap != NULL) {
            AddFeeItem (stp->fees, buf, 0, NULL, dap->dollars, dap->cents,
                        AFFIL_MEMB_FEE, NULL, NULL);
          }
        }
      }
      break;
    case 3 :
      if (AddToFullMemberList (stp, buf, ssp->jobs)) {
        AddToContactList (stp->new_contacts, buf, TRANSIENT_MEMBER);

        if (GetStatus (ssp->transfee)) {
          dap = &(ssp->proratedtrans);
          if (dap != NULL) {
            AddFeeItem (stp->fees, buf, 0, NULL, dap->dollars, dap->cents,
                        TRANS_MEMB_FEE, NULL, NULL);
          }
        }

        if (GetStatus (ssp->transssa)) {
          dap = &(ssp->proratedssa);
          if (dap != NULL) {
            AddFeeItem (stp->fees, buf, 0, NULL, dap->dollars, dap->cents,
                        SSA_DUES_FEE, NULL, NULL);
          }
        }
      }
      break;
    default :
      break;
  }

  SafeHide (ssp->namegroup);

  SafeHide (ssp->controls [ssp->currPage]);
  SafeSetValue (ssp->whichspec, 0);

  ssp->currPage = 0;

  SetTitle (ssp->name, "");

  SetStatus (ssp->visitfee, TRUE);
  SetStatus (ssp->affilfee, TRUE);
  SetStatus (ssp->transfee, TRUE);
  SetStatus (ssp->transssa, TRUE);

  SetClubJobs (ssp->jobs, FALSE, FALSE, FALSE, FALSE);

  Disable (b);

  Update ();
}

extern void CreateAddSpecialDialog (
  GrouP prnt,
  SscTablesPtr stp
)

{
  Char         buf [64];
  GrouP        g, h, m, p;
  MiscRatePtr  mrp;
  SscSpecPtr   ssp;
  Char         str [16];

  if (stp == NULL) return;
  mrp = stp->miscrate_table;
  if (mrp == NULL) return;

  ssp = (SscSpecPtr) MemNew (sizeof (SscSpec));
  if (ssp == NULL) return;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, ssp, StdCleanupExtraProc);

  ssp->dialog = (DialoG) p;

  ssp->tables = stp;

  ssp->whichspec = HiddenGroup (p, 5, 0, ChangeSpecType);
  SetObjectExtra (ssp->whichspec, ssp, NULL);

  RadioButton (ssp->whichspec, "Visiting");
  RadioButton (ssp->whichspec, "Affiliated");
  RadioButton (ssp->whichspec, "Transient");

  ssp->namegroup = HiddenGroup (p, 15, 0, NULL);
  SetGroupSpacing (ssp->namegroup, 5, 3);

  StaticPrompt (ssp->namegroup, "Name", 0, dialogTextHeight, programFont, 'l');
  ssp->name = DialogText (ssp->namegroup,
                          "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
                          0, ChangeSpecName);
  SetObjectExtra (ssp->name, (Pointer) ssp, NULL);
  SetTitle (ssp->name, "");

  Hide (ssp->namegroup);

  h = HiddenGroup (p, 0, 0, NULL);
  SetGroupMargins (h, 10, 10);

  g = HiddenGroup (h, -1, 0, NULL);

  Hide (g);
  ssp->controls [EMPTY_PAGE] = g;

  g = HiddenGroup (h, -1, 0, NULL);

  if (PrintWholeDollar (mrp->visiting_fee, str)) {
    sprintf (buf, "Charge $%s Visiting Fee", str);
  } else {
    sprintf (buf, "Charge Visiting Fee");
  }
  ssp->visitfee = CheckBox (g, buf, NULL);
  SetStatus (ssp->visitfee, TRUE);

  Hide (g);
  ssp->controls [ADD_VISIT_PAGE] = g;

  g = HiddenGroup (h, -1, 0, NULL);

  if (PrintWholeDollar (mrp->affiliated_fee, str)) {
    sprintf (buf, "Charge $%s Affiliated Fee", str);
  } else {
    sprintf (buf, "Charge Affiliated Fee");
  }
  ssp->affilfee = CheckBox (g, buf, NULL);
  SetStatus (ssp->visitfee, TRUE);

  Hide (g);
  ssp->controls [ADD_AFFIL_PAGE] = g;

  g = HiddenGroup (h, -1, 0, NULL);
  SetGroupSpacing (g, 10, 10);

  m = HiddenGroup (g, 1, 0, NULL);
  SetGroupSpacing (m, 5, 3);

  ProrateDollarAmount (mrp->transient_fee, 1, FALSE, stp, &(ssp->proratedtrans));

  if (PrintWholeDollar (&(ssp->proratedtrans), str)) {
    sprintf (buf, "Charge $%s Transient Dues", str);
  } else {
    sprintf (buf, "Charge Transient Dues");
  }
  ssp->transfee = CheckBox (m, buf, NULL);
  SetStatus (ssp->transfee, TRUE);

  ProrateDollarAmount (mrp->ssa_regular, 5, TRUE, stp, &(ssp->proratedssa));

  if (PrintWholeDollar (&(ssp->proratedssa), str)) {
    sprintf (buf, "Charge $%s SSA Dues (Prorated May-Apr)", str);
  } else {
    sprintf (buf, "Charge SSA Dues");
  }
  ssp->transssa = CheckBox (m, buf, NULL);
  SetStatus (ssp->transssa, TRUE);

  ssp->jobs = CreateClubJobsDialog (g);

  AlignObjects (ALIGN_CENTER,
                (HANDLE) m,
                (HANDLE) ssp->jobs,
                NULL);

  Hide (g);
  ssp->controls [ADD_TRANS_FEE] = g;

  ssp->addspec = PushButton (p, "Add Special Member", AddSpecMember);
  SetObjectExtra (ssp->addspec, (Pointer) ssp, NULL);
  Disable (ssp->addspec);

  ssp->currPage = 0;

  AlignObjects (ALIGN_CENTER,
                (HANDLE) ssp->whichspec,
                (HANDLE) ssp->namegroup,
                (HANDLE) ssp->controls [EMPTY_PAGE],
                (HANDLE) ssp->controls [ADD_VISIT_PAGE],
                (HANDLE) ssp->controls [ADD_AFFIL_PAGE],
                (HANDLE) ssp->controls [ADD_TRANS_FEE],
                (HANDLE) ssp->addspec,
                NULL);
}

