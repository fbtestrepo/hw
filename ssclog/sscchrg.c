/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Merchandise charges page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* internal version of ChargeDataPtr */

typedef struct intchginfo {
  CharPtr       name;
  Int4          type;
  CharPtr       alt_payer;
  DollarAmount  charges;
  Int4          reason;
  CharPtr       comment;
} IntChargeInfo, PNTR IntChargePtr;

/* dialog controls */

typedef struct ssccharges {
  DIALOG_MESSAGE_BLOCK

  DoC           chargelog;
  Int2          currItem;

  PopuP         name;
  DialoG        charge;
  PopuP         reason;
  TexT          comment;
  PopuP         alt_payer;

  Int2          numCharges;
  IntChargePtr  charges [128];

  SscTablesPtr  tables;
} SscCharges, PNTR SscChargesPtr;

#define NUM_CHARGES_LINES 8

static IntChargePtr IntChargeInfoNew (
  void
)

{
  DollarAmountPtr  dap;
  IntChargePtr     icp;

  icp = MemNew (sizeof (IntChargeInfo));
  if (icp == NULL) return NULL;

  dap = &(icp->charges);
  dap->dollars = -1;
  dap->cents = -1;

  return icp;
}

static IntChargePtr IntChargeInfoFree (
  IntChargePtr icp
)

{
  if (icp == NULL) return NULL;

  MemFree (icp->name);
  MemFree (icp->alt_payer);
  MemFree (icp->comment);

  return MemFree (icp);
}

static IntChargePtr GetChargeInfo (
  SscChargesPtr scp,
  Int2 item
)

{
  IntChargePtr  icp;

  if (scp == NULL || item < 0 || item > 127) return NULL;
  icp = scp->charges [item];
  if (icp != NULL) return icp;

  icp = IntChargeInfoNew ();
  if (icp != NULL) {
    icp->name = StringSave (" ");
    icp->alt_payer = StringSave (" ");
    /*
    icp->comment = StringSave (" ");
    */
  }
  scp->charges [item] = icp;
  return icp;
}

static ParData  chargeParFmt = { FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0 };

static ColData  chargeColFmt [] = {
  {0, 5, 25, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE}, /* payee   */
  {0, 5,  9, 2, NULL, 'r', FALSE, TRUE,  FALSE, FALSE, FALSE}, /* charges */
  {0, 5, 12, 2, NULL, 'l', FALSE, TRUE,  FALSE, FALSE, TRUE}   /* reason  */
};

static ENUM_ALIST(reason_alist)
  {" ",                     0},
  {"Voucher Sale",          VOUCHER_SALE_CHARGE},
  {"Book Sale",             BOOK_SALE_CHARGE},
  {"Video Sale",            VIDEO_SALE_CHARGE},
  {"Clothing Sale",         CLOTHING_SALE_CHARGE},
  {"Packet Sale",           PACKET_SALE_CHARGE},
  {"Merchandise Sale",      MERCHANDISE_SALE_CHARGE},
  {"Other",                 OTHER_CHARGE},
END_ENUM_ALIST

static CharPtr reasonLabel [15] = {
  " ",
  "Voucher Sale",
  "Book Sale",
  "Video Sale",
  "Clothing Sale",
  "Packet Sale",
  "Merch Sale",
  "Other"
};

static CharPtr PrintChargeTable (
  DoC d,
  Int2 item,
  Pointer data
)

{
  Char             buf [256], tmp [64];
  DollarAmountPtr  dap;
  IntChargePtr     icp;
  Int2             reason;
  SscChargesPtr    scp;

  scp = (SscChargesPtr) GetObjectExtra (d);
  if (scp == NULL || item < 1 || item > 127) return NULL;
  icp = GetChargeInfo (scp, item);
  if (icp == NULL) return NULL;

  buf [0] = '\0';

  if (StringDoesHaveText (icp->name)) {
    StringCat (buf, icp->name);
  } else {
    StringCat (buf, " ");
  }

  StringCat (buf, "\t");
  dap = &(icp->charges);
  if (PrintDollarAmount (dap, tmp)) {
    StringCat (buf, tmp);
  } else {
    StringCat (buf, " ");
  }

  StringCat (buf, "\t");
  reason = (Int2) icp->reason;
  if (reason >= VOUCHER_SALE_CHARGE && reason <= OTHER_CHARGE) {
    StringCat (buf, reasonLabel [reason]);
  } else {
    StringCat (buf, " ");
  }

  StringCat (buf, "\n");
  return StringSave (buf);
}

static void ChangeChargeTableSelect (
  DoC d,
  Int2 item,
  Int2 row,
  Int2 col,
  Boolean dblClck
)

{
  Int4           cents, dollars;
  IntChargePtr   icp;
  Int2           itemOld1, itemOld2;
  SscChargesPtr  scp;
  SscTablesPtr   stp;

  scp = (SscChargesPtr) GetObjectExtra (d);
  if (scp == NULL) return;
  stp = scp->tables;
  if (stp == NULL) return;
  if (item == 0 || row == 0 || col == 0) return;

  GetDocHighlight (d, &itemOld1, &itemOld2);
  SetDocHighlight (d, item, item);
  UpdateDocument (d, itemOld1, itemOld2);
  UpdateDocument (d, item, item);
  scp->currItem = item;

  ResetClip ();

  icp = GetChargeInfo (scp, scp->currItem);
  if (icp != NULL) {
    SetEnumPopupByName (scp->name, stp->payers.alist, icp->name);
    SetEnumPopupByName (scp->alt_payer, stp->payers.alist, icp->alt_payer);
    SetDollarAmount (scp->charge, icp->charges.dollars, icp->charges.cents);
    SetEnumPopup (scp->reason, reason_alist, (UIEnum) icp->reason);
    SetTitle (scp->comment, icp->comment);
  }

  GetDollarAmount (scp->charge, &dollars, &cents);
  if (dollars < 0) {
    SelectDollars (scp->charge);
  } else {
    SelectCents (scp->charge);
  }

  Update ();
}

static void CheckExtendTable (
  SscChargesPtr scp
)

{
  Int2  numItems;

  if (scp == NULL) return;

  GetDocParams (scp->chargelog, &numItems, NULL);
  if (scp->currItem == numItems) {
    AppendItem (scp->chargelog, PrintChargeTable, scp, FALSE, 1,
                &chargeParFmt, chargeColFmt, systemFont);
  }

  Update ();

  SetDirtyFlag (scp->tables);
}

static void ChangeName (
  PopuP p
)

{
  AlistDialogPtr   adp;
  DollarAmountPtr  dap;
  IntChargePtr     icp;
  SscChargesPtr    scp;
  SscTablesPtr     stp;
  CharPtr          str;

  adp = (AlistDialogPtr) GetObjectExtra (p);
  if (adp == NULL) return;
  scp = (SscChargesPtr) adp->userdata;
  if (scp == NULL) return;
  stp = scp->tables;
  if (stp == NULL) return;
  icp = GetChargeInfo (scp, scp->currItem);
  if (icp == NULL) return;

  dap = &(icp->charges);
  if (dap != NULL) {
    if (dap->dollars > 0 || dap->cents > 0) {
      if (StringDoesHaveText (icp->name)) {
        if (Message (MSG_YN, "Do you want to override the existing payer?") == ANS_NO) {
          SetEnumPopupByName (scp->name, stp->payers.alist, icp->name);
          return;
        }
      }
    }
  }

  str = GetEnumPopupByName (scp->name, stp->payers.alist);
  if (StringCmp (str, "-") == 0) {
    str [0] = ' ';
    SetEnumPopupByName (scp->name, stp->payers.alist, str);
  }
  icp->name = MemFree (icp->name);
  icp->name = str; /* allocated by GetEnumPopupByName */
  icp->type = GetMemberType (str, stp);

  UpdateDocument (scp->chargelog, scp->currItem, scp->currItem);
  Update ();

  CheckExtendTable (scp);
}

static void ChangeCharge (
  DialoG d
)

{
  BaseDialogPtr    bdp;
  Int4             cents, dollars;
  DollarAmountPtr  dap;
  IntChargePtr     icp;
  SscChargesPtr    scp;

  bdp = (BaseDialogPtr) GetObjectExtra (d);
  if (bdp == NULL) return;
  scp = (SscChargesPtr) bdp->userdata;
  if (scp == NULL) return;
  icp = GetChargeInfo (scp, scp->currItem);
  if (icp == NULL) return;

  GetDollarAmount (d, &dollars, &cents);

  dap = &(icp->charges);
  if (dap != NULL) {
    dap->dollars = dollars;
    dap->cents = cents;
  }

  UpdateDocument (scp->chargelog, scp->currItem, scp->currItem);
  Update ();

  CheckExtendTable (scp);
}

static void ChangeReason (
  PopuP p
)

{
  Int4             cents, dollars;
  DollarAmountPtr  dap;
  IntChargePtr     icp;
  MiscRatePtr      mrp;
  SscChargesPtr    scp;
  SscTablesPtr     stp;
  UIEnum           val;

  scp = (SscChargesPtr) GetObjectExtra (p);
  if (scp == NULL) return;
  icp = GetChargeInfo (scp, scp->currItem);
  if (icp == NULL) return;
  stp = scp->tables;
  if (stp == NULL) return;
  mrp = stp->miscrate_table;
  if (mrp == NULL) return;

  if (GetEnumPopup (scp->reason, reason_alist, &val)) {
    icp->reason = (Int4) val;
    GetDollarAmount (scp->charge, &dollars, &cents);
    if ((dollars < 0 && cents < 0) ||
        StringHasNoText (icp->name)) {
      if (mrp != NULL) {
        dap = NULL;
        switch (val) {
          case VOUCHER_SALE_CHARGE:
            dap = mrp->fast_fee;
            break;
          default :
            break;
        }
        if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
          dollars = dap->dollars;
          cents = dap->cents;
          dap = &(icp->charges);
          if (dap != NULL) {
            dap->dollars = dollars;
            dap->cents = cents;
            SetDollarAmount (scp->charge, dollars, cents);
          }
        }
      }
    }
  }

  UpdateDocument (scp->chargelog, scp->currItem, scp->currItem);
  Update ();

  CheckExtendTable (scp);
}

static void ChangeComment (
  TexT t
)

{
  IntChargePtr   icp;
  SscChargesPtr  scp;

  scp = (SscChargesPtr) GetObjectExtra (t);
  if (scp == NULL) return;
  icp = GetChargeInfo (scp, scp->currItem);
  if (icp == NULL) return;

  icp->comment = MemFree (icp->comment);
  icp->comment = SaveStringFromText (t);

  CheckExtendTable (scp);
}

static void ChangePayer (
  PopuP p
)

{
  AlistDialogPtr  adp;
  IntChargePtr    icp;
  SscChargesPtr   scp;
  SscTablesPtr    stp;
  CharPtr         str;

  adp = (AlistDialogPtr) GetObjectExtra (p);
  if (adp == NULL) return;
  scp = (SscChargesPtr) adp->userdata;
  if (scp == NULL) return;
  stp = scp->tables;
  if (stp == NULL) return;
  icp = GetChargeInfo (scp, scp->currItem);
  if (icp == NULL) return;

  str = GetEnumPopupByName (scp->alt_payer, stp->payers.alist);
  if (StringCmp (str, "-") == 0) {
    str [0] = ' ';
    SetEnumPopupByName (scp->alt_payer, stp->payers.alist, str);
  }
  icp->alt_payer = MemFree (icp->alt_payer);
  icp->alt_payer = str; /* allocated by GetEnumPopupByName */

  if (icp->alt_payer == NULL) {
    icp->alt_payer = StringSave (" ");
  }

  UpdateDocument (scp->chargelog, scp->currItem, scp->currItem);
  Update ();

  CheckExtendTable (scp);
}

extern void KeyToChargesDialog (
  DialoG d,
  Char key
)

{
  Int2           numItems, offset;
  BaR            sb;
  SscChargesPtr  scp;

  scp = (SscChargesPtr) GetObjectExtra (d);
  if (scp == NULL) return;

  if (key == NLM_UP) {
    if (scp->currItem > 1) {
      (scp->currItem)--;
    }
  } else if (key == NLM_DOWN) {
    GetDocParams (scp->chargelog, &numItems, NULL);
    if (scp->currItem < numItems) {
      (scp->currItem)++;
    }
  }

  sb = GetSlateVScrollBar ((SlatE) scp->chargelog);
  offset = GetValue (sb);

  if (key == NLM_DOWN && offset < scp->currItem - NUM_CHARGES_LINES && scp->currItem > 0) {
    SetValue (sb, scp->currItem - NUM_CHARGES_LINES);
  }
  if (key == NLM_UP && scp->currItem <= offset) {
    SetValue (sb, scp->currItem - 1);
  }

  ChangeChargeTableSelect (scp->chargelog, scp->currItem, 1, 1, FALSE);

  Update ();
}

extern void UpdateChargesControls (
  DialoG d
)

{
  SscChargesPtr  scp;
  SscTablesPtr   stp;
  Boolean        was_visible = FALSE;

  scp = (SscChargesPtr) GetObjectExtra (d);
  if (scp == NULL) return;
  stp = scp->tables;
  if (stp == NULL) return;

  if (! stp->payers.need_to_update) return;

  if (Visible (d) && AllParentsVisible (d)) {
    Hide (d);
    Update ();
    was_visible = TRUE;
  }

  if (stp->payers.need_to_update) {
    RecreateNameAlist (&(stp->payers));
    Reset (scp->name);
    InitEnumPopup (scp->name, stp->payers.alist, NULL);
    SetValue (scp->name, 0);
    Reset (scp->alt_payer);
    InitEnumPopup (scp->alt_payer, stp->payers.alist, NULL);
    SetValue (scp->alt_payer, 0);
  }

  ChangeChargeTableSelect (scp->chargelog, scp->currItem, 1, 1, FALSE);

  if (was_visible) {
    Show (d);
    Update ();
  }
}

static Int2 expChgPosns [] = {0, 22, 77, 100};
static Char expChgJusts [] = "lllr";

extern void ExportChargesDialog (
  DialoG d,
  ValNodePtr PNTR head
)

{
  Char           buf [128], tmp [32];
  DoC            doc;
  IntChargePtr   icp;
  Int2           k, numItems;
  SscChargesPtr  scp;

  scp = (SscChargesPtr) GetObjectExtra (d);
  if (scp == NULL || head == NULL) return;

  doc = scp->chargelog;
  GetDocParams (doc, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    icp = GetChargeInfo (scp, k);
    if (icp == NULL) continue;
    if (StringHasNoText (icp->name)) continue;
    buf [0] = '\0';
    ExportDocCell (doc, k, 1, buf);
    if (icp->type == 2) {
      FormatCell ("*", buf, 1, expChgPosns, expChgJusts);
    }
    ExportDocCell (doc, k, 3, tmp);
    FormatCell (tmp, buf, 2, expChgPosns, expChgJusts);
    StringCpy (tmp, "$");
    ExportDocCell (doc, k, 2, tmp + 1);
    FormatCell (tmp, buf, 3, expChgPosns, expChgJusts);
    ValNodeCopyStr (head, k + 102, buf);
  }
}

static CharPtr archiveChargesLabel = "Name                      Charge    Reason";

extern void ArchiveChargesDialog (
  DialoG d,
  FILE *fp
)

{
  IntChargePtr   icp;
  Int2           k, n, numItems;
  SscChargesPtr  scp;

  scp = (SscChargesPtr) GetObjectExtra (d);
  if (scp == NULL || fp == NULL) return;

  GetDocParams (scp->chargelog, &numItems, NULL);
  for (k = 1, n = 0; k <= numItems; k++) {
    icp = GetChargeInfo (scp, k);
    if (icp == NULL) continue;
    if (StringHasNoText (icp->name)) continue;
    n++;
  }
  if (n < 1) return;

  fprintf (fp, "CHARGES -----\n\n%s\n\n", archiveChargesLabel);

  for (k = 1; k <= numItems; k++) {
    icp = GetChargeInfo (scp, k);
    if (icp == NULL) continue;
    if (StringHasNoText (icp->name)) continue;
    SaveDocumentItem (scp->chargelog, fp, k);
  }
  fprintf (fp, "\n");
}

static void ChargeDataPtrToChargePage (
  DialoG d,
  Pointer data
)

{
  ChargeDataPtr    charges, mcp;
  DollarAmountPtr  dap;
  Int2             i;
  IntChargePtr     icp;
  Int2             k;
  SscChargesPtr    scp;
  SscTablesPtr     stp;

  scp = (SscChargesPtr) GetObjectExtra (d);
  if (scp == NULL) return;
  stp = scp->tables;
  if (stp == NULL) return;
  charges = (ChargeDataPtr) data;

  Reset (scp->chargelog);
  scp->numCharges = 0;
  scp->currItem = 1;

  for (k = 0; k < 128; k++) {
    icp = scp->charges [k];
    IntChargeInfoFree (icp);
    scp->charges [k] = NULL;
  }

  if (charges == NULL) {
    Reset (scp->chargelog);
    SetValue (scp->name, 0);
    SetDollarAmount (scp->charge, -1, -1);
    SetValue (scp->reason, 0);
    SetTitle (scp->comment, "");
    SetValue (scp->alt_payer, 0);
    scp->numCharges = 0;
    scp->currItem = 1;
    for (i = 0; i < NUM_CHARGES_LINES; i++) {
      AppendItem (scp->chargelog, PrintChargeTable, scp, FALSE, 1,
                  &chargeParFmt, chargeColFmt, systemFont);
    }
    SetDocHighlight (scp->chargelog, 1, 1);
    return;
  }

  for (mcp = charges, k = 1; mcp != NULL; mcp = mcp->next, k++) {
    icp = GetChargeInfo (scp, k);
    if (icp == NULL) continue;

    icp->name = StringSaveForAlist (mcp->name);
    icp->type = mcp->type;
    icp->comment = StringSaveNotEmpty (mcp->comment);
    icp->alt_payer = StringSaveForAlist (mcp->alt_payer);

    icp->reason = mcp->reason;

    dap = mcp->amount;
    if (dap != NULL) {
      icp->charges.dollars = dap->dollars;
      icp->charges.cents = dap->cents;
    }

    AppendItem (scp->chargelog, PrintChargeTable, scp, FALSE, 1,
                &chargeParFmt, chargeColFmt, systemFont);

    (scp->numCharges)++;
  }

  AppendItem (scp->chargelog, PrintChargeTable, scp, FALSE, 1,
              &chargeParFmt, chargeColFmt, systemFont);
  k++;

  while (k <= NUM_CHARGES_LINES) {
    AppendItem (scp->chargelog, PrintChargeTable, scp, FALSE, 1,
                &chargeParFmt, chargeColFmt, systemFont);
    k++;
  }

  UpdateDocument (scp->chargelog, 0, 0);

  ChangeChargeTableSelect (scp->chargelog, 1, 1, 1, FALSE);

  Update ();
}

static Pointer ChargePageToChargeDataPtr (
  DialoG d
)

{
  ChargeDataPtr    charges, mcp, lastmcp;
  DollarAmountPtr  dap;
  IntChargePtr     icp;
  Int2             k, numItems;
  SscChargesPtr    scp;
  SscTablesPtr     stp;

  scp = (SscChargesPtr) GetObjectExtra (d);
  if (scp == NULL) return NULL;
  stp = scp->tables;
  if (stp == NULL) return NULL;

  charges = NULL;

  lastmcp = NULL;
  GetDocParams (scp->chargelog, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    icp = GetChargeInfo (scp, k);
    if (icp == NULL) continue;
    if (StringHasNoText (icp->name)) continue;
    mcp = ChargeDataNew ();
    if (mcp == NULL) continue;

    mcp->name = StringSaveNotEmpty (icp->name);
    mcp->type = icp->type;
    mcp->comment = StringSaveNotEmpty (icp->comment);
    mcp->alt_payer = StringSaveNotEmpty (icp->alt_payer);

    dap = DollarAmountNew ();
    if (dap != NULL) {
      dap->dollars = icp->charges.dollars;
      dap->cents = icp->charges.cents;
      mcp->amount = dap;
    }

    mcp->reason = icp->reason;

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

extern void AddChargeItem (
  DialoG sfpcharges,
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
  ChargeDataPtr    charges, mcp, lastmcp = NULL;
  DollarAmountPtr  dap;

  if (StringHasNoText (name)) return;
  if (dollars < 0 && cents < 0) return;

  dap = DollarAmountNew ();
  if (dap == NULL) return;
  dap->dollars = dollars;
  dap->cents = cents;

  charges = DialogToPointer (sfpcharges);
  if (charges != NULL) {
    for (mcp = charges; mcp != NULL; mcp = mcp->next) {
      lastmcp = mcp;
    }
  }
  mcp = ChargeDataNew ();
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
  PointerToDialog (sfpcharges, mcp);
  ChargeListFree (charges);
}

extern DialoG CreateChargeDialog (
  GrouP prnt,
  SscTablesPtr stp
)

{
  GrouP          cts, tbl, g0, g1, g2, g3, p;
  FonT           fnt;
  Int2           i, hgt, wid;
  SscChargesPtr  scp;

  scp = (SscChargesPtr) MemNew (sizeof (SscCharges));
  if (scp == NULL) return NULL;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, scp, StdCleanupExtraProc);
  scp->dialog = (DialoG) p;
  scp->todialog = ChargeDataPtrToChargePage;
  scp->fromdialog = ChargePageToChargeDataPtr;

  scp->tables = stp;

  SelectFont (systemFont);
  hgt = LineHeight ();
  chargeColFmt [0].pixWidth = 25 * StringWidth ("X") + 5;
  chargeColFmt [1].pixWidth = StringWidth ("$100.00 ") + 5;
  chargeColFmt [2].pixWidth = StringWidth ("Merchandise Sale ") + 5;
  SelectFont (systemFont);

  wid = 0;
  for (i = 0; i < 3; i++) {
    wid += chargeColFmt [i].pixWidth;
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
  StaticPrompt (g0, "Name", chargeColFmt [0].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Amount", chargeColFmt [1].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Item", chargeColFmt [2].pixWidth, 0, fnt, 'c');

  scp->chargelog = DocumentPanel (tbl, wid + 2, NUM_CHARGES_LINES * hgt + 2);
  SetObjectExtra (scp->chargelog, scp, NULL);
  SetDocCache (scp->chargelog, NULL, NULL, NULL);
  SetDocNotify (scp->chargelog, ChangeChargeTableSelect);
  scp->numCharges = 0;

  for (i = 0; i < NUM_CHARGES_LINES; i++) {
    AppendItem (scp->chargelog, PrintChargeTable, scp, FALSE, 1,
                &chargeParFmt, chargeColFmt, systemFont);
  }

  cts = HiddenGroup (p, -1, 0, NULL);
  SetGroupSpacing (cts, 10, 10);
  SetGroupMargins (cts, 5, 5);

  g1 = HiddenGroup (cts, -10, 0, NULL);
  SetGroupSpacing (g1, 10, 5);

  StaticPrompt (g1, "Name", 0, popupMenuHeight, programFont, 'l');
  scp->name = CreateEnumPopupDialog (g1, TRUE, ChangeName, stp->payers.alist, (UIEnum) 0, scp);

  StaticPrompt (g1, "Charge $", 0, dialogTextHeight, programFont, 'l');
  scp->charge = CreateDollarAmountDialog (g1, (Pointer) scp, ChangeCharge);

  StaticPrompt (g1, "Item", 0, popupMenuHeight, programFont, 'l');
  scp->reason = CreateEnumPopupDialog (g1, TRUE, ChangeReason, reason_alist, (UIEnum) 0, scp);
  SetObjectExtra (scp->reason, scp, NULL);

  g2 = HiddenGroup (cts, -10, 0, NULL);
  SetGroupSpacing (g2, 10, 5);

  StaticPrompt (g2, "Comment", 0, dialogTextHeight, programFont, 'l');
  scp->comment = DialogText (g2, "", 20, ChangeComment);
  SetObjectExtra (scp->comment, scp, NULL);

  g3 = HiddenGroup (cts, -10, 0, NULL);
  SetGroupSpacing (g3, 10, 5);

  StaticPrompt (g3, "Alternative Payer", 0, dialogTextHeight, programFont, 'l');
  scp->alt_payer = CreateEnumPopupDialog (g3, TRUE, ChangePayer, stp->payers.alist, (UIEnum) 0, scp);

  AlignObjects (ALIGN_CENTER, (HANDLE) tbl, (HANDLE) cts, NULL);

  scp->numCharges = 0;
  scp->currItem = 1;

  return (DialoG) p;
}

