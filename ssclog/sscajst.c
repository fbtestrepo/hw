/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Fee adjustment page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* internal version of AdjustDataPtr*/

typedef struct intadjinfo {
  CharPtr       name;
  Int4          type;
  CharPtr       alt_payer;
  DollarAmount  adjustments;
  Int4          reason;
  CharPtr       comment;
} IntAdjInfo, PNTR IntAdjPtr;

/* dialog controls */

typedef struct sscadj {
  DIALOG_MESSAGE_BLOCK

  DoC           adjlog;
  Int2          currItem;

  PopuP         name;
  TexT          alt_payer;
  DialoG        amount;
  PopuP         reason;
  TexT          comment;

  Int2          numAdjs;
  IntAdjPtr     adjustments [128];

  SscTablesPtr  tables;
} SscAdj, PNTR SscAdjPtr;

#define NUM_ADJUSTMENT_LINES 8

static IntAdjPtr IntAdjInfoNew (
  void
)

{
  DollarAmountPtr  dap;
  IntAdjPtr        iap;

  iap = MemNew (sizeof (IntAdjInfo));
  if (iap == NULL) return NULL;

  dap = &(iap->adjustments);
  dap->dollars = -1;
  dap->cents = -1;

  return iap;
}

static IntAdjPtr IntAdjInfoFree (
  IntAdjPtr iap
)

{
  if (iap == NULL) return NULL;

  MemFree (iap->name);
  MemFree (iap->alt_payer);
  MemFree (iap->comment);

  return MemFree (iap);
}

static IntAdjPtr GetAdjInfo (
  SscAdjPtr sap,
  Int2 item
)

{
  IntAdjPtr  iap;

  if (sap == NULL || item < 0 || item > 127) return NULL;
  iap = sap->adjustments [item];
  if (iap != NULL) return iap;

  iap = IntAdjInfoNew ();
  if (iap != NULL) {
    iap->name = StringSave (" ");
    iap->alt_payer = StringSave (" ");
    /*
    iap->comment = StringSave (" ");
    */
  }
  sap->adjustments [item] = iap;
  return iap;
}

static ParData  adjParFmt = { FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0 };

static ColData  adjColFmt [] = {
  {0, 5, 25, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE}, /* payee       */
  {0, 5,  9, 2, NULL, 'r', FALSE, TRUE,  FALSE, FALSE, FALSE}, /* adjustments */
  {0, 5, 12, 2, NULL, 'l', FALSE, TRUE,  FALSE, FALSE, TRUE}   /* reason      */
};

static ENUM_ALIST(reason_alist)
  {" ",              0},
  {"Pay Old Charge", 1},
  {"Add to Account", 2},
  {"Other",          3},
END_ENUM_ALIST

static CharPtr reasonLabel [4] = {
  " ",
  "Prev Payment",
  "Advc Credit",
  "Other",
};

static CharPtr PrintAdjTable (
  DoC d,
  Int2 item,
  Pointer data
)

{
  Char             buf [256], tmp [64];
  DollarAmountPtr  dap;
  IntAdjPtr        iap;
  Int2             reason;
  SscAdjPtr        sap;

  sap = (SscAdjPtr) GetObjectExtra (d);
  if (sap == NULL || item < 1 || item > 127) return NULL;
  iap = GetAdjInfo (sap, item);
  if (iap == NULL) return NULL;

  buf [0] = '\0';

  if (StringDoesHaveText (iap->name)) {
    StringCat (buf, iap->name);
  } else {
    StringCat (buf, " ");
  }

  StringCat (buf, "\t");
  dap = &(iap->adjustments);
  if (PrintDollarAmount (dap, tmp)) {
    StringCat (buf, tmp);
  } else {
    StringCat (buf, " ");
  }

  StringCat (buf, "\t");
  reason = (Int2) iap->reason;
  if (reason > 0 && reason <= 3) {
    StringCat (buf, reasonLabel [reason]);
  } else {
    StringCat (buf, " ");
  }

  StringCat (buf, "\n");
  return StringSave (buf);
}

static void ChangeAdjTableSelect (
  DoC d,
  Int2 item,
  Int2 row,
  Int2 col,
  Boolean dblClck
)

{
  Int4             cents, dollars;
  IntAdjPtr        iap;
  Int2             itemOld1, itemOld2;
  SscAdjPtr        sap;
  SscTablesPtr     stp;

  sap = (SscAdjPtr) GetObjectExtra (d);
  if (sap == NULL) return;
  stp = sap->tables;
  if (stp == NULL) return;
  if (item == 0 || row == 0 || col == 0) return;

  GetDocHighlight (d, &itemOld1, &itemOld2);
  SetDocHighlight (d, item, item);
  UpdateDocument (d, itemOld1, itemOld2);
  UpdateDocument (d, item, item);
  sap->currItem = item;

  ResetClip ();

  iap = GetAdjInfo (sap, sap->currItem);
  if (iap != NULL) {
    SetEnumPopupByName (sap->name, stp->payers.alist, iap->name);
    SetTitle (sap->alt_payer, iap->alt_payer);
    SetDollarAmount (sap->amount, iap->adjustments.dollars, iap->adjustments.cents);
    SetEnumPopup (sap->reason, reason_alist, (UIEnum) iap->reason);
    SetTitle (sap->comment, iap->comment);
  }

  GetDollarAmount (sap->amount, &dollars, &cents);
  if (dollars < 0) {
    SelectDollars (sap->amount);
  } else {
    SelectCents (sap->amount);
  }

  Update ();
}

static void CheckExtendTable (
  SscAdjPtr sap
)

{
  Int2  numItems;

  if (sap == NULL) return;

  GetDocParams (sap->adjlog, &numItems, NULL);
  if (sap->currItem == numItems) {
    AppendItem (sap->adjlog, PrintAdjTable, sap, FALSE, 1,
                &adjParFmt, adjColFmt, systemFont);
  }

  Update ();

  SetDirtyFlag (sap->tables);
}

static void ChangeName (
  PopuP p
)

{
  AlistDialogPtr   adp;
  DollarAmountPtr  dap;
  IntAdjPtr        iap;
  SscAdjPtr        sap;
  SscTablesPtr     stp;
  CharPtr          str;

  adp = (AlistDialogPtr) GetObjectExtra (p);
  if (adp == NULL) return;
  sap = (SscAdjPtr) adp->userdata;
  if (sap == NULL) return;
  stp = sap->tables;
  if (stp == NULL) return;
  iap = GetAdjInfo (sap, sap->currItem);
  if (iap == NULL) return;

  dap = &(iap->adjustments);
  if (dap != NULL) {
    if (dap->dollars > 0 || dap->cents > 0) {
      if (StringDoesHaveText (iap->name)) {
        if (Message (MSG_YN, "Do you want to override the existing payer?") == ANS_NO) {
          SetEnumPopupByName (sap->name, stp->payers.alist, iap->name);
          return;
        }
      }
    }
  }

  str = GetEnumPopupByName (sap->name, stp->payers.alist);
  if (StringCmp (str, "-") == 0) {
    str [0] = ' ';
    SetEnumPopupByName (sap->name, stp->payers.alist, str);
  }
  iap->name = MemFree (iap->name);
  iap->name = str; /* allocated by GetEnumPopupByName */
  iap->type = GetMemberType (str, stp);

  UpdateDocument (sap->adjlog, sap->currItem, sap->currItem);
  Update ();

  CheckExtendTable (sap);
}

static void ChangeAmount (
  DialoG d
)

{
  BaseDialogPtr    bdp;
  Int4             cents, dollars;
  DollarAmountPtr  dap;
  IntAdjPtr        iap;
  SscAdjPtr        sap;

  bdp = (BaseDialogPtr) GetObjectExtra (d);
  if (bdp == NULL) return;
  sap = (SscAdjPtr) bdp->userdata;
  if (sap == NULL) return;
  iap = GetAdjInfo (sap, sap->currItem);
  if (iap == NULL) return;

  GetDollarAmount (d, &dollars, &cents);

  dap = &(iap->adjustments);
  if (dap != NULL) {
    dap->dollars = dollars;
    dap->cents = cents;
  }

  UpdateDocument (sap->adjlog, sap->currItem, sap->currItem);
  Update ();

  CheckExtendTable (sap);
}

static void ChangeReason (
  PopuP p
)

{
  IntAdjPtr  iap;
  SscAdjPtr  sap;
  UIEnum     val;

  sap = (SscAdjPtr) GetObjectExtra (p);
  if (sap == NULL) return;
  iap = GetAdjInfo (sap, sap->currItem);
  if (iap == NULL) return;

  if (GetEnumPopup (sap->reason, reason_alist, &val)) {
    iap->reason = (Int4) val;
  }

  UpdateDocument (sap->adjlog, sap->currItem, sap->currItem);
  Update ();

  CheckExtendTable (sap);
}

static void ChangeComment (
  TexT t
)

{
  IntAdjPtr  iap;
  SscAdjPtr  sap;

  sap = (SscAdjPtr) GetObjectExtra (t);
  if (sap == NULL) return;
  iap = GetAdjInfo (sap, sap->currItem);
  if (iap == NULL) return;

  iap->comment = MemFree (iap->comment);
  iap->comment = SaveStringFromText (t);

  CheckExtendTable (sap);
}

extern void KeyToAdjustmentsDialog (
  DialoG d,
  Char key
)

{
  Int2       numItems, offset;
  BaR        sb;
  SscAdjPtr  sap;

  sap = (SscAdjPtr) GetObjectExtra (d);
  if (sap == NULL) return;

  if (key == NLM_UP) {
    if (sap->currItem > 1) {
      (sap->currItem)--;
    }
  } else if (key == NLM_DOWN) {
    GetDocParams (sap->adjlog, &numItems, NULL);
    if (sap->currItem < numItems) {
      (sap->currItem)++;
    }
  }

  sb = GetSlateVScrollBar ((SlatE) sap->adjlog);
  offset = GetValue (sb);

  if (key == NLM_DOWN && offset < sap->currItem - NUM_ADJUSTMENT_LINES && sap->currItem > 0) {
    SetValue (sb, sap->currItem - NUM_ADJUSTMENT_LINES);
  }
  if (key == NLM_UP && sap->currItem <= offset) {
    SetValue (sb, sap->currItem - 1);
  }

  ChangeAdjTableSelect (sap->adjlog, sap->currItem, 1, 1, FALSE);

  Update ();
}

extern void UpdateAdjustmentsControls (
  DialoG d
)

{
  SscAdjPtr     sap;
  SscTablesPtr  stp;
  Boolean       was_visible = FALSE;

  sap = (SscAdjPtr) GetObjectExtra (d);
  if (sap == NULL) return;
  stp = sap->tables;
  if (stp == NULL) return;

  if (! stp->payers.need_to_update) return;

  if (Visible (d) && AllParentsVisible (d)) {
    Hide (d);
    Update ();
    was_visible = TRUE;
  }

  if (stp->payers.need_to_update) {
    RecreateNameAlist (&(stp->payers));
    Reset (sap->name);
    InitEnumPopup (sap->name, stp->payers.alist, NULL);
    SetValue (sap->name, 0);
  }

  ChangeAdjTableSelect (sap->adjlog, sap->currItem, 1, 1, FALSE);

  if (was_visible) {
    Show (d);
    Update ();
  }
}

static Int2 expAdjPosns [] = {0, 22, 77, 100};
static Char expAdjJusts [] = "lllr";

extern void ExportAdjustmentsDialog (
  DialoG d,
  ValNodePtr PNTR head
)

{
  Char       buf [128], tmp [32];
  DoC        doc;
  IntAdjPtr  iap;
  Int2       k, numItems;
  SscAdjPtr  sap;

  sap = (SscAdjPtr) GetObjectExtra (d);
  if (sap == NULL || head == NULL) return;

  doc = sap->adjlog;
  GetDocParams (doc, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    iap = GetAdjInfo (sap, k);
    if (iap == NULL) continue;
    if (StringHasNoText (iap->name)) continue;
    buf [0] = '\0';
    ExportDocCell (doc, k, 1, buf);
    if (iap->type == 2) {
      FormatCell ("*", buf, 1, expAdjPosns, expAdjJusts);
    }
    ExportDocCell (doc, k, 3, tmp);
    FormatCell (tmp, buf, 2, expAdjPosns, expAdjJusts);
    StringCpy (tmp, "$");
    ExportDocCell (doc, k, 2, tmp + 1);
    FormatCell (tmp, buf, 3, expAdjPosns, expAdjJusts);
    ValNodeCopyStr (head, k + 102, buf);
  }
}

static CharPtr archiveAdjsLabel = "Name                      Adjust    Reason";

extern void ArchiveAdjustmentsDialog (
  DialoG d,
  FILE *fp
)

{
  IntAdjPtr  iap;
  Int2       k, n, numItems;
  SscAdjPtr  sap;

  sap = (SscAdjPtr) GetObjectExtra (d);
  if (sap == NULL || fp == NULL) return;

  GetDocParams (sap->adjlog, &numItems, NULL);
  for (k = 1, n = 0; k <= numItems; k++) {
    iap = GetAdjInfo (sap, k);
    if (iap == NULL) continue;
    if (StringHasNoText (iap->name)) continue;
    n++;
  }
  if (n < 1) return;

  fprintf (fp, "ADJUSTMENTS -----\n\n%s\n\n", archiveAdjsLabel);

  for (k = 1; k <= numItems; k++) {
    iap = GetAdjInfo (sap, k);
    if (iap == NULL) continue;
    if (StringHasNoText (iap->name)) continue;
    SaveDocumentItem (sap->adjlog, fp, k);
  }
  fprintf (fp, "\n");
}

static void AdjustDataPtrToAdjPage (DialoG d, Pointer data)

{
  AdjustDataPtr    adjustments, fap;
  DollarAmountPtr  dap;
  Int2             i;
  IntAdjPtr        iap;
  Int2             k;
  SscAdjPtr        sap;

  sap = (SscAdjPtr) GetObjectExtra (d);
  if (sap == NULL) return;
  adjustments = (AdjustDataPtr) data;

  Reset (sap->adjlog);
  sap->numAdjs = 0;
  sap->currItem = 1;

  for (k = 0; k < 128; k++) {
    iap = sap->adjustments [k];
    IntAdjInfoFree (iap);
    sap->adjustments [k] = NULL;
  }

  if (adjustments == NULL) {
    Reset (sap->adjlog);
    SetValue (sap->name, 0);
    SetDollarAmount (sap->amount, -1, -1);
    SetValue (sap->reason, 0);
    SetTitle (sap->comment, "");
    sap->numAdjs = 0;
    sap->currItem = 1;
    for (i = 0; i < NUM_ADJUSTMENT_LINES; i++) {
      AppendItem (sap->adjlog, PrintAdjTable, sap, FALSE, 1,
                  &adjParFmt, adjColFmt, systemFont);
    }
    SetDocHighlight (sap->adjlog, 1, 1);
    return;
  }

  for (fap = adjustments, k = 1; fap != NULL; fap = fap->next, k++) {
    iap = GetAdjInfo (sap, k);
    if (iap == NULL) continue;

    iap->name = StringSaveForAlist (fap->name);
    iap->type = fap->type;
    iap->comment = StringSaveNotEmpty (fap->comment);

    iap->reason = fap->reason;

    dap = fap->amount;
    if (dap != NULL) {
      iap->adjustments.dollars = dap->dollars;
      iap->adjustments.cents = dap->cents;
    }

    AppendItem (sap->adjlog, PrintAdjTable, sap, FALSE, 1,
                &adjParFmt, adjColFmt, systemFont);

    (sap->numAdjs)++;
  }

  AppendItem (sap->adjlog, PrintAdjTable, sap, FALSE, 1,
              &adjParFmt, adjColFmt, systemFont);
  k++;

  while (k <= NUM_ADJUSTMENT_LINES) {
    AppendItem (sap->adjlog, PrintAdjTable, sap, FALSE, 1,
                &adjParFmt, adjColFmt, systemFont);
    k++;
  }

  UpdateDocument (sap->adjlog, 0, 0);

  ChangeAdjTableSelect (sap->adjlog, 1, 1, 1, FALSE);

  Update ();
}

static Pointer AdjPageToAdjustDataPtr(DialoG d)

{
  AdjustDataPtr    adjustments, fap, lastfap;
  DollarAmountPtr  dap;
  IntAdjPtr        iap;
  Int2             k, numItems;
  SscAdjPtr        sap;

  sap = (SscAdjPtr) GetObjectExtra (d);
  if (sap == NULL) return NULL;

  adjustments = NULL;

  lastfap = NULL;
  GetDocParams (sap->adjlog, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    iap = GetAdjInfo (sap, k);
    if (iap == NULL) continue;
    if (StringHasNoText (iap->name)) continue;
    fap = AdjustDataNew ();
    if (fap == NULL) continue;

    fap->name = StringSaveNotEmpty (iap->name);
    fap->type = iap->type;
    fap->comment = StringSaveNotEmpty (iap->comment);

    dap = DollarAmountNew ();
    if (dap != NULL) {
      dap->dollars = iap->adjustments.dollars;
      dap->cents = iap->adjustments.cents;
      fap->amount = dap;
    }

    fap->reason = iap->reason;

    if (adjustments == NULL) {
      adjustments = fap;
    }
    if (lastfap != NULL) {
      lastfap->next = fap;
    }
    lastfap = fap;
  }

  return (Pointer) adjustments;
}

extern void AddAdjustItem (
  DialoG sfpadjust,
  CharPtr name,
  Int4 reason,
  Int4 dollars,
  Int4 cents
)

{
  AdjustDataPtr    adjustments, adp, lastadp = NULL;
  DollarAmountPtr  dap;

  if (StringHasNoText (name)) return;
  if (dollars < 0 && cents < 0) return;

  adjustments = DialogToPointer (sfpadjust);
  if (adjustments != NULL) {
    for (adp = adjustments; adp != NULL; adp = adp->next) {
      if (StringICmp (adp->name, name) == 0) {
        if (adp->reason == 0) {
          adp->reason = reason;
        }
        if (adp->reason == reason) {
          if (adp->amount == NULL) {
            adp->amount = DollarAmountNew ();
          }
          if (adp->amount != NULL) {
            dap = adp->amount;
            dap->dollars += dollars;
            dap->cents += cents;
            if (dap->cents >= 100) {
              dap->dollars += dap->cents / 100;
              dap->cents = dap->cents % 100;
            }
            PointerToDialog (sfpadjust, adjustments);
            AdjustmentListFree (adjustments);
            return;
          }
        }
      }
      lastadp = adp;
    }
  }

  adp = AdjustDataNew ();
  if (adp != NULL) {
    adp->name = StringSave (name);
    adp->reason = reason;
    dap = DollarAmountNew ();
    if (dap != NULL) {
      dap->dollars = dollars;
      dap->cents = cents;
    }
    adp->amount = dap;
  }
  if (lastadp != NULL) {
    lastadp->next = adp;
    adp = adjustments;
  }
  PointerToDialog (sfpadjust, adp);
  AdjustmentListFree (adjustments);
}

extern DialoG CreateAdjustmentsDialog (
  GrouP prnt,
  SscTablesPtr stp
)

{
  GrouP      cts, tbl, g0, g1, g2, p;
  FonT       fnt;
  Int2       i, hgt, max, wid;
  SscAdjPtr  sap;

  sap = (SscAdjPtr) MemNew (sizeof (SscAdj));
  if (sap == NULL) return NULL;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, sap, StdCleanupExtraProc);
  sap->dialog = (DialoG) p;
  sap->todialog = AdjustDataPtrToAdjPage;
  sap->fromdialog = AdjPageToAdjustDataPtr;

  sap->tables = stp;

  SelectFont (systemFont);
  hgt = LineHeight ();
  adjColFmt [0].pixWidth = 25 * StringWidth ("X") + 5;
  adjColFmt [1].pixWidth = StringWidth ("$100.00 ") + 5;
  max = MAX (StringWidth ("Prev Payment "), StringWidth ("Advc Credit "));
  adjColFmt [2].pixWidth = max + 5;
  SelectFont (systemFont);

  wid = 0;
  for (i = 0; i < 3; i++) {
    wid += adjColFmt [i].pixWidth;
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
  StaticPrompt (g0, "Name", adjColFmt [0].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Amount", adjColFmt [1].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Item", adjColFmt [2].pixWidth, 0, fnt, 'c');

  sap->adjlog = DocumentPanel (tbl, wid + 2, NUM_ADJUSTMENT_LINES * hgt + 2);
  SetObjectExtra (sap->adjlog, sap, NULL);
  SetDocCache (sap->adjlog, NULL, NULL, NULL);
  SetDocNotify (sap->adjlog, ChangeAdjTableSelect);
  sap->numAdjs = 0;

  for (i = 0; i < NUM_ADJUSTMENT_LINES; i++) {
    AppendItem (sap->adjlog, PrintAdjTable, sap, FALSE, 1,
                &adjParFmt, adjColFmt, systemFont);
  }

  cts = HiddenGroup (p, -1, 0, NULL);
  SetGroupSpacing (cts, 10, 10);
  SetGroupMargins (cts, 5, 5);

  g1 = HiddenGroup (cts, -10, 0, NULL);
  SetGroupSpacing (g1, 10, 5);

  StaticPrompt (g1, "Name", 0, popupMenuHeight, programFont, 'l');
  sap->name = CreateEnumPopupDialog (g1, TRUE, ChangeName, stp->payers.alist, (UIEnum) 0, sap);

  StaticPrompt (g1, "Adjust $", 0, dialogTextHeight, programFont, 'l');
  sap->amount = CreateDollarAmountDialog (g1, (Pointer) sap, ChangeAmount);

  StaticPrompt (g1, "Item", 0, popupMenuHeight, programFont, 'l');
  sap->reason = CreateEnumPopupDialog (g1, TRUE, ChangeReason, reason_alist, (UIEnum) 0, sap);
  SetObjectExtra (sap->reason, sap, NULL);

  g2 = HiddenGroup (cts, -10, 0, NULL);
  SetGroupSpacing (g2, 10, 5);

  StaticPrompt (g2, "Comment", 0, dialogTextHeight, programFont, 'l');
  sap->comment = DialogText (g2, "", 20, ChangeComment);
  SetObjectExtra (sap->comment, sap, NULL);

  AlignObjects (ALIGN_CENTER, (HANDLE) tbl, (HANDLE) cts, NULL);

  sap->numAdjs = 0;
  sap->currItem = 1;

  return (DialoG) p;
}

