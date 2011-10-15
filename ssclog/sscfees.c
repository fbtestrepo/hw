/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Fees page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* internal version of FeeDataPtr */

typedef struct intfeeinfo {
  CharPtr       name;
  Int4          type;
  CharPtr       alt_payer;
  DollarAmount  charges;
  Int4          reason;
  CharPtr       comment;
} IntFeeInfo, PNTR IntFeePtr;

/* dialog controls */

typedef struct sscfees {
  DIALOG_MESSAGE_BLOCK

  DoC           chargelog;
  Int2          currItem;

  PopuP         name;
  DialoG        charge;
  PopuP         reason;
  TexT          comment;
  PopuP         alt_payer;

  Int2          numFees;
  IntFeePtr     charges [128];

  SscTablesPtr  tables;
} SscFees, PNTR SscFeesPtr;

#define NUM_FEES_LINES 8

static IntFeePtr IntFeeInfoNew (
  void
)

{
  DollarAmountPtr  dap;
  IntFeePtr        ifp;

  ifp = MemNew (sizeof (IntFeeInfo));
  if (ifp == NULL) return NULL;

  dap = &(ifp->charges);
  dap->dollars = -1;
  dap->cents = -1;

  return ifp;
}

static IntFeePtr IntFeeInfoFree (
  IntFeePtr ifp
)

{
  if (ifp == NULL) return NULL;

  MemFree (ifp->name);
  MemFree (ifp->alt_payer);
  MemFree (ifp->comment);

  return MemFree (ifp);
}

static IntFeePtr GetFeeInfo (
  SscFeesPtr sfp,
  Int2 item
)

{
  IntFeePtr  ifp;

  if (sfp == NULL || item < 0 || item > 127) return NULL;
  ifp = sfp->charges [item];
  if (ifp != NULL) return ifp;

  ifp = IntFeeInfoNew ();
  if (ifp != NULL) {
    ifp->name = StringSave (" ");
    ifp->alt_payer = StringSave (" ");
    /*
    ifp->comment = StringSave (" ");
    */
  }
  sfp->charges [item] = ifp;
  return ifp;
}

static ParData  feeParFmt = { FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0 };

static ColData  feeColFmt [] = {
  {0, 5, 25, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE}, /* payee   */
  {0, 5,  9, 2, NULL, 'r', FALSE, TRUE,  FALSE, FALSE, FALSE}, /* charges */
  {0, 5, 12, 2, NULL, 'l', FALSE, TRUE,  FALSE, FALSE, TRUE}   /* reason  */
};

static ENUM_ALIST(reason_alist)
  {" ",                     0},
  {"Regular Membership",    REG_MEMB_FEE},
  {"SSC Prorated Dues",     SSC_DUES_FEE},
  {"SSA Annual Dues",       SSA_DUES_FEE},
  {"Intro Membership",      INTRO_MEMB_FEE},
  {"Visiting Membership",   VISITING_MEMB_FEE},
  {"Affiliated Membership", AFFIL_MEMB_FEE},
  {"Transient Membership",  TRANS_MEMB_FEE},
  {"Aerial Retrieve",       AERIAL_RETRIEVE_FEE},
  {"Other",                 OTHER_FEE},
END_ENUM_ALIST

static CharPtr reasonLabel [15] = {
  " ",
  "Reg Memb",
  "SSC Dues",
  "SSA Dues",
  "Intro Memb",
  "Visiting Memb",
  "Affiliated Memb",
  "Transient Memb",
  "Retrieve",
  "Other"
};

static CharPtr PrIntFeeTable (
  DoC d,
  Int2 item,
  Pointer data
)

{
  Char             buf [256], tmp [64];
  DollarAmountPtr  dap;
  IntFeePtr        ifp;
  Int2             reason;
  SscFeesPtr       sfp;

  sfp = (SscFeesPtr) GetObjectExtra (d);
  if (sfp == NULL || item < 1 || item > 127) return NULL;
  ifp = GetFeeInfo (sfp, item);
  if (ifp == NULL) return NULL;

  buf [0] = '\0';

  if (StringDoesHaveText (ifp->name)) {
    StringCat (buf, ifp->name);
  } else {
    StringCat (buf, " ");
  }

  StringCat (buf, "\t");
  dap = &(ifp->charges);
  if (PrintDollarAmount (dap, tmp)) {
    StringCat (buf, tmp);
  } else {
    StringCat (buf, " ");
  }

  StringCat (buf, "\t");
  reason = (Int2) ifp->reason;
  if (reason >= REG_MEMB_FEE && reason <= OTHER_FEE) {
    StringCat (buf, reasonLabel [reason]);
  } else {
    StringCat (buf, " ");
  }

  StringCat (buf, "\n");
  return StringSave (buf);
}

static void ChangeFeeTableSelect (
  DoC d,
  Int2 item,
  Int2 row,
  Int2 col,
  Boolean dblClck
)

{
  Int4          cents, dollars;
  IntFeePtr     ifp;
  Int2          itemOld1, itemOld2;
  SscFeesPtr    sfp;
  SscTablesPtr  stp;

  sfp = (SscFeesPtr) GetObjectExtra (d);
  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;
  if (item == 0 || row == 0 || col == 0) return;

  GetDocHighlight (d, &itemOld1, &itemOld2);
  SetDocHighlight (d, item, item);
  UpdateDocument (d, itemOld1, itemOld2);
  UpdateDocument (d, item, item);
  sfp->currItem = item;

  ResetClip ();

  ifp = GetFeeInfo (sfp, sfp->currItem);
  if (ifp != NULL) {
    SetEnumPopupByName (sfp->name, stp->payers.alist, ifp->name);
    SetEnumPopupByName (sfp->alt_payer, stp->payers.alist, ifp->alt_payer);
    SetDollarAmount (sfp->charge, ifp->charges.dollars, ifp->charges.cents);
    SetEnumPopup (sfp->reason, reason_alist, (UIEnum) ifp->reason);
    SetTitle (sfp->comment, ifp->comment);
  }

  GetDollarAmount (sfp->charge, &dollars, &cents);
  if (dollars < 0) {
    SelectDollars (sfp->charge);
  } else {
    SelectCents (sfp->charge);
  }

  Update ();
}

static void CheckExtendTable (
  SscFeesPtr sfp
)

{
  Int2  numItems;

  if (sfp == NULL) return;

  GetDocParams (sfp->chargelog, &numItems, NULL);
  if (sfp->currItem == numItems) {
    AppendItem (sfp->chargelog, PrIntFeeTable, sfp, FALSE, 1,
                &feeParFmt, feeColFmt, systemFont);
  }

  Update ();

  SetDirtyFlag (sfp->tables);
}

static void ChangeName (
  PopuP p
)

{
  AlistDialogPtr   adp;
  DollarAmountPtr  dap;
  IntFeePtr        ifp;
  SscFeesPtr       sfp;
  SscTablesPtr     stp;
  CharPtr          str;

  adp = (AlistDialogPtr) GetObjectExtra (p);
  if (adp == NULL) return;
  sfp = (SscFeesPtr) adp->userdata;
  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;
  ifp = GetFeeInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  dap = &(ifp->charges);
  if (dap != NULL) {
    if (dap->dollars > 0 || dap->cents > 0) {
      if (StringDoesHaveText (ifp->name)) {
        if (Message (MSG_YN, "Do you want to override the existing payer?") == ANS_NO) {
          SetEnumPopupByName (sfp->name, stp->payers.alist, ifp->name);
          return;
        }
      }
    }
  }

  str = GetEnumPopupByName (sfp->name, stp->payers.alist);
  if (StringCmp (str, "-") == 0) {
    str [0] = ' ';
    SetEnumPopupByName (sfp->name, stp->payers.alist, str);
  }
  ifp->name = MemFree (ifp->name);
  ifp->name = str; /* allocated by GetEnumPopupByName */
  ifp->type = GetMemberType (str, stp);

  UpdateDocument (sfp->chargelog, sfp->currItem, sfp->currItem);
  Update ();

  CheckExtendTable (sfp);
}

static void ChangeFee (
  DialoG d
)

{
  BaseDialogPtr    bdp;
  Int4             cents, dollars;
  DollarAmountPtr  dap;
  IntFeePtr        ifp;
  SscFeesPtr       sfp;

  bdp = (BaseDialogPtr) GetObjectExtra (d);
  if (bdp == NULL) return;
  sfp = (SscFeesPtr) bdp->userdata;
  if (sfp == NULL) return;
  ifp = GetFeeInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  GetDollarAmount (d, &dollars, &cents);

  dap = &(ifp->charges);
  if (dap != NULL) {
    dap->dollars = dollars;
    dap->cents = cents;
  }

  UpdateDocument (sfp->chargelog, sfp->currItem, sfp->currItem);
  Update ();

  CheckExtendTable (sfp);
}

static void ChangeReason (
  PopuP p
)

{
  Int4             cents, dollars;
  DollarAmountPtr  dap;
  IntFeePtr        ifp;
  MiscRatePtr      mrp;
  SscFeesPtr       sfp;
  SscTablesPtr     stp;
  UIEnum           val;

  sfp = (SscFeesPtr) GetObjectExtra (p);
  if (sfp == NULL) return;
  ifp = GetFeeInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;
  mrp = stp->miscrate_table;
  if (mrp == NULL) return;

  if (GetEnumPopup (sfp->reason, reason_alist, &val)) {
    ifp->reason = (Int4) val;
    GetDollarAmount (sfp->charge, &dollars, &cents);
    if ((dollars < 0 && cents < 0) ||
        StringHasNoText (ifp->name)) {
      if (mrp != NULL) {
        dap = NULL;
        switch (val) {
          case REG_MEMB_FEE:
            dap = mrp->initiation_fee;
            break;
          case INTRO_MEMB_FEE:
            dap = mrp->intro_fee;
            break;
          case VISITING_MEMB_FEE:
            dap = mrp->visiting_fee;
            break;
          case AFFIL_MEMB_FEE:
            dap = mrp->affiliated_fee;
            break;
          case TRANS_MEMB_FEE:
            dap = mrp->transient_fee;
            break;
          case AERIAL_RETRIEVE_FEE:
            dap = mrp->minimum_retrieve_cost;
            break;
          default :
            break;
        }
        if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
          dollars = dap->dollars;
          cents = dap->cents;
          dap = &(ifp->charges);
          if (dap != NULL) {
            dap->dollars = dollars;
            dap->cents = cents;
            SetDollarAmount (sfp->charge, dollars, cents);
          }
        }
      }
    }
  }

  UpdateDocument (sfp->chargelog, sfp->currItem, sfp->currItem);
  Update ();

  CheckExtendTable (sfp);
}

static void ChangeComment (
  TexT t
)

{
  IntFeePtr   ifp;
  SscFeesPtr  sfp;

  sfp = (SscFeesPtr) GetObjectExtra (t);
  if (sfp == NULL) return;
  ifp = GetFeeInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  ifp->comment = MemFree (ifp->comment);
  ifp->comment = SaveStringFromText (t);

  CheckExtendTable (sfp);
}

static void ChangePayer (
  PopuP p
)

{
  AlistDialogPtr  adp;
  IntFeePtr       ifp;
  SscFeesPtr      sfp;
  SscTablesPtr    stp;
  CharPtr         str;

  adp = (AlistDialogPtr) GetObjectExtra (p);
  if (adp == NULL) return;
  sfp = (SscFeesPtr) adp->userdata;
  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;
  ifp = GetFeeInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  str = GetEnumPopupByName (sfp->alt_payer, stp->payers.alist);
  if (StringCmp (str, "-") == 0) {
    str [0] = ' ';
    SetEnumPopupByName (sfp->alt_payer, stp->payers.alist, str);
  }
  ifp->alt_payer = MemFree (ifp->alt_payer);
  ifp->alt_payer = str; /* allocated by GetEnumPopupByName */

  if (ifp->alt_payer == NULL) {
    ifp->alt_payer = StringSave (" ");
  }

  UpdateDocument (sfp->chargelog, sfp->currItem, sfp->currItem);
  Update ();

  CheckExtendTable (sfp);
}

extern void KeyToFeesDialog (
  DialoG d,
  Char key
)

{
  Int2        numItems, offset;
  BaR         sb;
  SscFeesPtr  sfp;

  sfp = (SscFeesPtr) GetObjectExtra (d);
  if (sfp == NULL) return;

  if (key == NLM_UP) {
    if (sfp->currItem > 1) {
      (sfp->currItem)--;
    }
  } else if (key == NLM_DOWN) {
    GetDocParams (sfp->chargelog, &numItems, NULL);
    if (sfp->currItem < numItems) {
      (sfp->currItem)++;
    }
  }

  sb = GetSlateVScrollBar ((SlatE) sfp->chargelog);
  offset = GetValue (sb);

  if (key == NLM_DOWN && offset < sfp->currItem - NUM_FEES_LINES && sfp->currItem > 0) {
    SetValue (sb, sfp->currItem - NUM_FEES_LINES);
  }
  if (key == NLM_UP && sfp->currItem <= offset) {
    SetValue (sb, sfp->currItem - 1);
  }

  ChangeFeeTableSelect (sfp->chargelog, sfp->currItem, 1, 1, FALSE);

  Update ();
}

extern void UpdateFeesControls (
  DialoG d
)

{
  SscFeesPtr    sfp;
  SscTablesPtr  stp;
  Boolean       was_visible = FALSE;

  sfp = (SscFeesPtr) GetObjectExtra (d);
  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;

  if (! stp->payers.need_to_update) return;

  if (Visible (d) && AllParentsVisible (d)) {
    Hide (d);
    Update ();
    was_visible = TRUE;
  }

  if (stp->payers.need_to_update) {
    RecreateNameAlist (&(stp->payers));
    Reset (sfp->name);
    InitEnumPopup (sfp->name, stp->payers.alist, NULL);
    SetValue (sfp->name, 0);
    Reset (sfp->alt_payer);
    InitEnumPopup (sfp->alt_payer, stp->payers.alist, NULL);
    SetValue (sfp->alt_payer, 0);
  }

  ChangeFeeTableSelect (sfp->chargelog, sfp->currItem, 1, 1, FALSE);

  if (was_visible) {
    Show (d);
    Update ();
  }
}

static Int2 expFeePosns [] = {0, 22, 77, 100};
static Char expFeeJusts [] = "lllr";

extern void ExportFeesDialog (
  DialoG d,
  ValNodePtr PNTR head
)

{
  Char        buf [128], tmp [32];
  DoC         doc;
  IntFeePtr   ifp;
  Int2        k, numItems;
  SscFeesPtr  sfp;

  sfp = (SscFeesPtr) GetObjectExtra (d);
  if (sfp == NULL || head == NULL) return;

  doc = sfp->chargelog;
  GetDocParams (doc, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    ifp = GetFeeInfo (sfp, k);
    if (ifp == NULL) continue;
    if (StringHasNoText (ifp->name)) continue;
    buf [0] = '\0';
    ExportDocCell (doc, k, 1, buf);
    if (ifp->type == 2) {
      FormatCell ("*", buf, 1, expFeePosns, expFeeJusts);
    }
    ExportDocCell (doc, k, 3, tmp);
    FormatCell (tmp, buf, 2, expFeePosns, expFeeJusts);
    StringCpy (tmp, "$");
    ExportDocCell (doc, k, 2, tmp + 1);
    FormatCell (tmp, buf, 3, expFeePosns, expFeeJusts);
    ValNodeCopyStr (head, k + 102, buf);
  }
}

static CharPtr archiveFeesLabel = "Name                         Fee    Reason";

extern void ArchiveFeesDialog (
  DialoG d,
  FILE *fp
)

{
  IntFeePtr   ifp;
  Int2           k, n, numItems;
  SscFeesPtr  sfp;

  sfp = (SscFeesPtr) GetObjectExtra (d);
  if (sfp == NULL || fp == NULL) return;

  GetDocParams (sfp->chargelog, &numItems, NULL);
  for (k = 1, n = 0; k <= numItems; k++) {
    ifp = GetFeeInfo (sfp, k);
    if (ifp == NULL) continue;
    if (StringHasNoText (ifp->name)) continue;
    n++;
  }
  if (n < 1) return;

  fprintf (fp, "FEES --------\n\n%s\n\n", archiveFeesLabel);

  for (k = 1; k <= numItems; k++) {
    ifp = GetFeeInfo (sfp, k);
    if (ifp == NULL) continue;
    if (StringHasNoText (ifp->name)) continue;
    SaveDocumentItem (sfp->chargelog, fp, k);
  }
  fprintf (fp, "\n");
}

static void FeeDataPtrToFeePage (
  DialoG d,
  Pointer data
)

{
  FeeDataPtr       charges, mcp;
  DollarAmountPtr  dap;
  Int2             i;
  IntFeePtr        ifp;
  Int2             k;
  SscFeesPtr       sfp;
  SscTablesPtr     stp;

  sfp = (SscFeesPtr) GetObjectExtra (d);
  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;
  charges = (FeeDataPtr) data;

  Reset (sfp->chargelog);
  sfp->numFees = 0;
  sfp->currItem = 1;

  for (k = 0; k < 128; k++) {
    ifp = sfp->charges [k];
    IntFeeInfoFree (ifp);
    sfp->charges [k] = NULL;
  }

  if (charges == NULL) {
    Reset (sfp->chargelog);
    SetValue (sfp->name, 0);
    SetDollarAmount (sfp->charge, -1, -1);
    SetValue (sfp->reason, 0);
    SetTitle (sfp->comment, "");
    SetValue (sfp->alt_payer, 0);
    sfp->numFees = 0;
    sfp->currItem = 1;
    for (i = 0; i < NUM_FEES_LINES; i++) {
      AppendItem (sfp->chargelog, PrIntFeeTable, sfp, FALSE, 1,
                  &feeParFmt, feeColFmt, systemFont);
    }
    SetDocHighlight (sfp->chargelog, 1, 1);
    return;
  }

  for (mcp = charges, k = 1; mcp != NULL; mcp = mcp->next, k++) {
    ifp = GetFeeInfo (sfp, k);
    if (ifp == NULL) continue;

    ifp->name = StringSaveForAlist (mcp->name);
    ifp->type = mcp->type;
    ifp->comment = StringSaveNotEmpty (mcp->comment);
    ifp->alt_payer = StringSaveForAlist (mcp->alt_payer);

    ifp->reason = mcp->reason;

    dap = mcp->amount;
    if (dap != NULL) {
      ifp->charges.dollars = dap->dollars;
      ifp->charges.cents = dap->cents;
    }

    AppendItem (sfp->chargelog, PrIntFeeTable, sfp, FALSE, 1,
                &feeParFmt, feeColFmt, systemFont);

    (sfp->numFees)++;
  }

  AppendItem (sfp->chargelog, PrIntFeeTable, sfp, FALSE, 1,
              &feeParFmt, feeColFmt, systemFont);
  k++;

  while (k <= NUM_FEES_LINES) {
    AppendItem (sfp->chargelog, PrIntFeeTable, sfp, FALSE, 1,
                &feeParFmt, feeColFmt, systemFont);
    k++;
  }

  UpdateDocument (sfp->chargelog, 0, 0);

  ChangeFeeTableSelect (sfp->chargelog, 1, 1, 1, FALSE);

  Update ();
}

static Pointer FeePageToFeeDataPtr (
  DialoG d
)

{
  FeeDataPtr       charges, mcp, lastmcp;
  DollarAmountPtr  dap;
  IntFeePtr        ifp;
  Int2             k, numItems;
  SscFeesPtr       sfp;
  SscTablesPtr     stp;

  sfp = (SscFeesPtr) GetObjectExtra (d);
  if (sfp == NULL) return NULL;
  stp = sfp->tables;
  if (stp == NULL) return NULL;

  charges = NULL;

  lastmcp = NULL;
  GetDocParams (sfp->chargelog, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    ifp = GetFeeInfo (sfp, k);
    if (ifp == NULL) continue;
    if (StringHasNoText (ifp->name)) continue;
    mcp = FeeDataNew ();
    if (mcp == NULL) continue;

    mcp->name = StringSaveNotEmpty (ifp->name);
    mcp->type = ifp->type;
    mcp->comment = StringSaveNotEmpty (ifp->comment);
    mcp->alt_payer = StringSaveNotEmpty (ifp->alt_payer);

    dap = DollarAmountNew ();
    if (dap != NULL) {
      dap->dollars = ifp->charges.dollars;
      dap->cents = ifp->charges.cents;
      mcp->amount = dap;
    }

    mcp->reason = ifp->reason;

    if (charges == NULL) {
      charges = mcp;
    }
    if (lastmcp != NULL) {
      lastmcp->next = mcp;
    }
    lastmcp = mcp;
  }

  return (Pointer) charges;
}

extern void AddFeeItem (
  DialoG sfpfees,
  CharPtr name,
  Int4 type,
  CharPtr alt_payer,
  Int4 dollars,
  Int4 cents,
  Int4 reason,
  CharPtr item,
  CharPtr comment
)

{
  FeeDataPtr       charges, mcp, lastmcp = NULL;
  DollarAmountPtr  dap;

  if (StringHasNoText (name)) return;
  if (dollars < 0 && cents < 0) return;

  dap = DollarAmountNew ();
  if (dap == NULL) return;
  dap->dollars = dollars;
  dap->cents = cents;

  charges = DialogToPointer (sfpfees);
  if (charges != NULL) {
    for (mcp = charges; mcp != NULL; mcp = mcp->next) {
      lastmcp = mcp;
    }
  }
  mcp = FeeDataNew ();
  if (mcp != NULL) {
    mcp->name = StringSave (name);
    mcp->type = type;
    mcp->alt_payer = StringSaveNotEmpty (alt_payer);
    mcp->amount = dap;
    mcp->reason = reason;
    mcp->item = StringSaveNotEmpty (item);
    mcp->comment = StringSaveNotEmpty (comment);
  }
  if (lastmcp != NULL) {
    lastmcp->next = mcp;
    mcp = charges;
  }
  PointerToDialog (sfpfees, mcp);
  FeeListFree (charges);
}

extern DialoG CreateFeeDialog (
  GrouP prnt,
  SscTablesPtr stp
)

{
  GrouP       cts, tbl, g0, g1, g2, g3, p;
  FonT        fnt;
  Int2        i, hgt, wid;
  SscFeesPtr  sfp;

  sfp = (SscFeesPtr) MemNew (sizeof (SscFees));
  if (sfp == NULL) return NULL;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, sfp, StdCleanupExtraProc);
  sfp->dialog = (DialoG) p;
  sfp->todialog = FeeDataPtrToFeePage;
  sfp->fromdialog = FeePageToFeeDataPtr;

  sfp->tables = stp;

  SelectFont (systemFont);
  hgt = LineHeight ();
  feeColFmt [0].pixWidth = 25 * StringWidth ("X") + 5;
  feeColFmt [1].pixWidth = StringWidth ("$100.00 ") + 5;
  feeColFmt [2].pixWidth = MAX (StringWidth ("Aerial Retrieve "), StringWidth ("Affiliated Memb ")) + 5;
  SelectFont (systemFont);

  wid = 0;
  for (i = 0; i < 3; i++) {
    wid += feeColFmt [i].pixWidth;
  }

  tbl = HiddenGroup (p, -1, 0, NULL);
  SetGroupSpacing (tbl, 10, 5);
  SetGroupMargins (tbl, 5, 5);

  g0 = HiddenGroup (tbl, 15, 0, NULL);
  SetGroupSpacing (g0, 0, 3);
#ifdef WIN_MSWIN
  fnt = systemFont;
#else
  fnt = programFont;
#endif
  StaticPrompt (g0, "Name", feeColFmt [0].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Amount", feeColFmt [1].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Item", feeColFmt [2].pixWidth, 0, fnt, 'c');

  sfp->chargelog = DocumentPanel (tbl, wid + 2, NUM_FEES_LINES * hgt + 2);
  SetObjectExtra (sfp->chargelog, sfp, NULL);
  SetDocCache (sfp->chargelog, NULL, NULL, NULL);
  SetDocNotify (sfp->chargelog, ChangeFeeTableSelect);
  sfp->numFees = 0;

  for (i = 0; i < NUM_FEES_LINES; i++) {
    AppendItem (sfp->chargelog, PrIntFeeTable, sfp, FALSE, 1,
                &feeParFmt, feeColFmt, systemFont);
  }

  cts = HiddenGroup (p, -1, 0, NULL);
  SetGroupSpacing (cts, 10, 10);
  SetGroupMargins (cts, 5, 5);

  g1 = HiddenGroup (cts, -10, 0, NULL);
  SetGroupSpacing (g1, 10, 5);

  StaticPrompt (g1, "Name", 0, popupMenuHeight, programFont, 'l');
  sfp->name = CreateEnumPopupDialog (g1, TRUE, ChangeName, stp->payers.alist, (UIEnum) 0, sfp);

  StaticPrompt (g1, "Fee $", 0, dialogTextHeight, programFont, 'l');
  sfp->charge = CreateDollarAmountDialog (g1, (Pointer) sfp, ChangeFee);

  StaticPrompt (g1, "Item", 0, popupMenuHeight, programFont, 'l');
  sfp->reason = CreateEnumPopupDialog (g1, TRUE, ChangeReason, reason_alist, (UIEnum) 0, sfp);
  SetObjectExtra (sfp->reason, sfp, NULL);

  g2 = HiddenGroup (cts, -10, 0, NULL);
  SetGroupSpacing (g2, 10, 5);

  StaticPrompt (g2, "Comment", 0, dialogTextHeight, programFont, 'l');
  sfp->comment = DialogText (g2, "", 20, ChangeComment);
  SetObjectExtra (sfp->comment, sfp, NULL);

  g3 = HiddenGroup (cts, -10, 0, NULL);
  SetGroupSpacing (g3, 10, 5);

  StaticPrompt (g3, "Alternative Payer", 0, dialogTextHeight, programFont, 'l');
  sfp->alt_payer = CreateEnumPopupDialog (g3, TRUE, ChangePayer, stp->payers.alist, (UIEnum) 0, sfp);

  AlignObjects (ALIGN_CENTER, (HANDLE) tbl, (HANDLE) cts, NULL);

  sfp->numFees = 0;
  sfp->currItem = 1;

  return (DialoG) p;
}

