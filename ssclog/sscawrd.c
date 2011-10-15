/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Badge page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* internal version of AwardDataPtr*/

typedef struct intbadginfo {
  CharPtr       name;
  Int4          type;
  Int4          reason;
  CharPtr       comment;
} IntBadgInfo, PNTR IntBadgPtr;

/* dialog controls */

typedef struct sscbadg {
  DIALOG_MESSAGE_BLOCK

  DoC           badglog;
  Int2          currItem;

  PopuP         name;
  PopuP         reason;
  TexT          comment;

  Int2          numBadgs;
  IntBadgPtr    badges [128];

  SscTablesPtr  tables;
} SscBadg, PNTR SscBadgPtr;

#define NUM_BADGE_LINES 8

static IntBadgPtr IntBadgInfoNew (
  void
)

{
  IntBadgPtr  ibp;

  ibp = MemNew (sizeof (IntBadgInfo));
  if (ibp == NULL) return NULL;
  return ibp;
}

static IntBadgPtr IntBadgInfoFree (
  IntBadgPtr ibp
)

{
  if (ibp == NULL) return NULL;

  MemFree (ibp->name);
  MemFree (ibp->comment);

  return MemFree (ibp);
}

static IntBadgPtr GetBadgInfo (
  SscBadgPtr sbp,
  Int2 item
)

{
  IntBadgPtr  ibp;

  if (sbp == NULL || item < 0 || item > 127) return NULL;
  ibp = sbp->badges [item];
  if (ibp != NULL) return ibp;

  ibp = IntBadgInfoNew ();
  if (ibp != NULL) {
    ibp->name = StringSave (" ");
    ibp->comment = StringSave (" ");
  }
  sbp->badges [item] = ibp;
  return ibp;
}

static ParData  badgParFmt = { FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0 };

static ColData  badgColFmt [] = {
  {0, 5, 25, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE}, /* payee  */
  {0, 5, 12, 2, NULL, 'l', FALSE, TRUE,  FALSE, FALSE, TRUE}   /* reason */
};

static ENUM_ALIST(reason_alist)
  {" ",                 0},
  {"A Badge",           1},
  {"B Badge",           2},
  {"C Badge",           3},
  {"Bronze Badge",      4},
  {"Silver Altitude",   5},
  {"Silver Duration",   6},
  {"Silver Distance",   7},
  {"Gold Altitude",     8},
  {"Gold Distance",     9},
  {"Diamond Altitude", 10},
  {"Diamond Goal",     11},
  {"Diamond Distance", 12},
  {"1000 K",           13},
  {"2000 K",           14},
  {"PPL-G",            15},
  {"CPL-G",            16},
  {"CFI-G",            17},
  {"Other",            18},
END_ENUM_ALIST

static CharPtr reasonLabel [19] = {
  " ",
  "A Badge",
  "B Badge",
  "C Badge",
  "Bronze Badge",
  "Silver Altitude",
  "Silver Duration",
  "Silver Distance",
  "Gold Altitude",
  "Gold Distance",
  "Diamond Altitude",
  "Diamond Goal",
  "Diamond Distance",
  "1000 K",
  "2000 K",
  "PPL-G",
  "CPL-G",
  "CFI-G",
  "Other"
};

static CharPtr PrintBadgTable (
  DoC d,
  Int2 item,
  Pointer data
)

{
  Char        buf [256];
  IntBadgPtr  ibp;
  Int2        reason;
  SscBadgPtr  sbp;

  sbp = (SscBadgPtr) GetObjectExtra (d);
  if (sbp == NULL || item < 1 || item > 127) return NULL;
  ibp = GetBadgInfo (sbp, item);
  if (ibp == NULL) return NULL;

  buf [0] = '\0';

  if (StringDoesHaveText (ibp->name)) {
    StringCat (buf, ibp->name);
  } else {
    StringCat (buf, " ");
  }

  StringCat (buf, "\t");
  reason = (Int2) ibp->reason;
  if (reason > 0 && reason <= 18) {
    StringCat (buf, reasonLabel [reason]);
  } else {
    StringCat (buf, " ");
  }

  StringCat (buf, "\n");
  return StringSave (buf);
}

static void ChangeBadgTableSelect (
  DoC d,
  Int2 item,
  Int2 row,
  Int2 col,
  Boolean dblClck
)

{
  IntBadgPtr    ibp;
  Int2          itemOld1, itemOld2;
  SscBadgPtr    sbp;
  SscTablesPtr  stp;

  sbp = (SscBadgPtr) GetObjectExtra (d);
  if (sbp == NULL) return;
  stp = sbp->tables;
  if (stp == NULL) return;
  if (item == 0 || row == 0 || col == 0) return;

  GetDocHighlight (d, &itemOld1, &itemOld2);
  SetDocHighlight (d, item, item);
  UpdateDocument (d, itemOld1, itemOld2);
  UpdateDocument (d, item, item);
  sbp->currItem = item;

  ResetClip ();

  ibp = GetBadgInfo (sbp, sbp->currItem);
  if (ibp != NULL) {
    SetEnumPopupByName (sbp->name, stp->payers.alist, ibp->name);
    SetEnumPopup (sbp->reason, reason_alist, (UIEnum) ibp->reason);
    SetTitle (sbp->comment, ibp->comment);
  }

  Update ();
}

static void CheckExtendTable (
  SscBadgPtr sbp
)

{
  Int2  numItems;

  if (sbp == NULL) return;

  GetDocParams (sbp->badglog, &numItems, NULL);
  if (sbp->currItem == numItems) {
    AppendItem (sbp->badglog, PrintBadgTable, sbp, FALSE, 1,
                &badgParFmt, badgColFmt, systemFont);
  }

  Update ();

  SetDirtyFlag (sbp->tables);
}

static void ChangeName (
  PopuP p
)

{
  AlistDialogPtr  adp;
  IntBadgPtr      ibp;
  SscBadgPtr      sbp;
  SscTablesPtr    stp;
  CharPtr         str;

  adp = (AlistDialogPtr) GetObjectExtra (p);
  if (adp == NULL) return;
  sbp = (SscBadgPtr) adp->userdata;
  if (sbp == NULL) return;
  stp = sbp->tables;
  if (stp == NULL) return;
  ibp = GetBadgInfo (sbp, sbp->currItem);
  if (ibp == NULL) return;

  str = GetEnumPopupByName (sbp->name, stp->payers.alist);
  if (StringCmp (str, "-") == 0) {
    str [0] = ' ';
    SetEnumPopupByName (sbp->name, stp->payers.alist, str);
  }
  ibp->name = MemFree (ibp->name);
  ibp->name = str; /* allocated by GetEnumPopupByName */
  ibp->type = GetMemberType (str, stp);

  UpdateDocument (sbp->badglog, sbp->currItem, sbp->currItem);
  Update ();

  CheckExtendTable (sbp);
}

static void ChangeReason (
  PopuP p
)

{
  IntBadgPtr  ibp;
  SscBadgPtr  sbp;
  UIEnum      val;

  sbp = (SscBadgPtr) GetObjectExtra (p);
  if (sbp == NULL) return;
  ibp = GetBadgInfo (sbp, sbp->currItem);
  if (ibp == NULL) return;

  if (GetEnumPopup (sbp->reason, reason_alist, &val)) {
    ibp->reason = (Int4) val;
  }

  UpdateDocument (sbp->badglog, sbp->currItem, sbp->currItem);
  Update ();

  CheckExtendTable (sbp);
}

static void ChangeComment (
  TexT t
)

{
  IntBadgPtr  ibp;
  SscBadgPtr  sbp;

  sbp = (SscBadgPtr) GetObjectExtra (t);
  if (sbp == NULL) return;
  ibp = GetBadgInfo (sbp, sbp->currItem);
  if (ibp == NULL) return;

  ibp->comment = MemFree (ibp->comment);
  ibp->comment = SaveStringFromText (t);

  CheckExtendTable (sbp);
}

extern void KeyToBadgesDialog (
  DialoG d,
  Char key
)

{
  Int2        numItems, offset;
  BaR         sb;
  SscBadgPtr  sbp;

  sbp = (SscBadgPtr) GetObjectExtra (d);
  if (sbp == NULL) return;

  if (key == NLM_UP) {
    if (sbp->currItem > 1) {
      (sbp->currItem)--;
    }
  } else if (key == NLM_DOWN) {
    GetDocParams (sbp->badglog, &numItems, NULL);
    if (sbp->currItem < numItems) {
      (sbp->currItem)++;
    }
  }

  sb = GetSlateVScrollBar ((SlatE) sbp->badglog);
  offset = GetValue (sb);

  if (key == NLM_DOWN && offset < sbp->currItem - NUM_BADGE_LINES && sbp->currItem > 0) {
    SetValue (sb, sbp->currItem - NUM_BADGE_LINES);
  }
  if (key == NLM_UP && sbp->currItem <= offset) {
    SetValue (sb, sbp->currItem - 1);
  }

  ChangeBadgTableSelect (sbp->badglog, sbp->currItem, 1, 1, FALSE);

  Update ();
}

extern void UpdateBadgesControls (
  DialoG d
)

{
  SscBadgPtr    sbp;
  SscTablesPtr  stp;
  Boolean       was_visible = FALSE;

  sbp = (SscBadgPtr) GetObjectExtra (d);
  if (sbp == NULL) return;
  stp = sbp->tables;
  if (stp == NULL) return;

  if (! stp->payers.need_to_update) return;

  if (Visible (d) && AllParentsVisible (d)) {
    Hide (d);
    Update ();
    was_visible = TRUE;
  }

  if (stp->payers.need_to_update) {
    RecreateNameAlist (&(stp->payers));
    Reset (sbp->name);
    InitEnumPopup (sbp->name, stp->payers.alist, NULL);
    SetValue (sbp->name, 0);
  }

  ChangeBadgTableSelect (sbp->badglog, sbp->currItem, 1, 1, FALSE);

  if (was_visible) {
    Show (d);
    Update ();
  }
}

extern void ExportBadgesDialog (
  DialoG d,
  ValNodePtr PNTR head
)

{
  Char        buf [128], tmp [32];
  DoC         doc;
  IntBadgPtr  ibp;
  Int2        k, numItems;
  SscBadgPtr  sbp;

  sbp = (SscBadgPtr) GetObjectExtra (d);
  if (sbp == NULL || head == NULL) return;

  doc = sbp->badglog;
  GetDocParams (doc, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    ibp = GetBadgInfo (sbp, k);
    if (ibp == NULL) continue;
    if (StringHasNoText (ibp->name)) continue;
    buf [0] = '\0';
    ExportDocCell (doc, k, 1, buf);
    TabToCol (buf, 22);
    ExportDocCell (doc, k, 3, tmp);
    StringCat (buf, tmp);
    ValNodeCopyStr (head, k + 100, buf);
  }
}

static CharPtr archiveBadgLabel = "Name                       Award";

extern void ArchiveBadgesDialog (
  DialoG d,
  FILE *fp
)

{
  IntBadgPtr  ibp;
  Int2        k, n, numItems;
  SscBadgPtr  sbp;

  sbp = (SscBadgPtr) GetObjectExtra (d);
  if (sbp == NULL || fp == NULL) return;

  GetDocParams (sbp->badglog, &numItems, NULL);
  for (k = 1, n = 0; k <= numItems; k++) {
    ibp = GetBadgInfo (sbp, k);
    if (ibp == NULL) continue;
    if (StringHasNoText (ibp->name)) continue;
    n++;
  }
  if (n < 1) return;

  fprintf (fp, "AWARDS -----\n\n%s\n\n", archiveBadgLabel);

  for (k = 1; k <= numItems; k++) {
    ibp = GetBadgInfo (sbp, k);
    if (ibp == NULL) continue;
    if (StringHasNoText (ibp->name)) continue;
    SaveDocumentItem (sbp->badglog, fp, k);
  }
  fprintf (fp, "\n");
}

static void AwardDataPtrToAdjPage (DialoG d, Pointer data)

{
  AwardDataPtr  badges, bdp;
  Int2          i;
  IntBadgPtr    ibp;
  Int2          k;
  SscBadgPtr    sbp;
  SscTablesPtr  stp;

  sbp = (SscBadgPtr) GetObjectExtra (d);
  if (sbp == NULL) return;
  stp = sbp->tables;
  if (stp == NULL) return;
  badges = (AwardDataPtr) data;

  Reset (sbp->badglog);
  sbp->numBadgs = 0;
  sbp->currItem = 1;

  for (k = 0; k < 128; k++) {
    ibp = sbp->badges [k];
    IntBadgInfoFree (ibp);
    sbp->badges [k] = NULL;
  }

  if (badges == NULL) {
    Reset (sbp->badglog);
    SetValue (sbp->name, 0);
    SetValue (sbp->reason, 0);
    SetTitle (sbp->comment, "");
    sbp->numBadgs = 0;
    sbp->currItem = 1;
    for (i = 0; i < NUM_BADGE_LINES; i++) {
      AppendItem (sbp->badglog, PrintBadgTable, sbp, FALSE, 1,
                  &badgParFmt, badgColFmt, systemFont);
    }
    SetDocHighlight (sbp->badglog, 1, 1);
    return;
  }

  for (bdp = badges, k = 1; bdp != NULL; bdp = bdp->next, k++) {
    ibp = GetBadgInfo (sbp, k);
    if (ibp == NULL) continue;

    ibp->name = StringSaveForAlist (bdp->name);
    ibp->type = bdp->type;
    ibp->comment = StringSaveNotEmpty (bdp->comment);

    ibp->reason = bdp->reason;

    AppendItem (sbp->badglog, PrintBadgTable, sbp, FALSE, 1,
                &badgParFmt, badgColFmt, systemFont);

    (sbp->numBadgs)++;
  }

  AppendItem (sbp->badglog, PrintBadgTable, sbp, FALSE, 1,
              &badgParFmt, badgColFmt, systemFont);
  k++;

  while (k <= NUM_BADGE_LINES) {
    AppendItem (sbp->badglog, PrintBadgTable, sbp, FALSE, 1,
                &badgParFmt, badgColFmt, systemFont);
    k++;
  }

  UpdateDocument (sbp->badglog, 0, 0);

  ChangeBadgTableSelect (sbp->badglog, 1, 1, 1, FALSE);

  Update ();
}

static Pointer AdjPageToAwardDataPtr(DialoG d)

{
  AwardDataPtr  badges, bdp, lastbdp;
  IntBadgPtr    ibp;
  Int2          k, numItems;
  SscBadgPtr    sbp;
  SscTablesPtr  stp;

  sbp = (SscBadgPtr) GetObjectExtra (d);
  if (sbp == NULL) return NULL;
  stp = sbp->tables;
  if (stp == NULL) return NULL;

  badges = NULL;

  lastbdp = NULL;
  GetDocParams (sbp->badglog, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    ibp = GetBadgInfo (sbp, k);
    if (ibp == NULL) continue;
    if (StringHasNoText (ibp->name)) continue;
    bdp = AwardDataNew ();
    if (bdp == NULL) continue;

    bdp->name = StringSaveNotEmpty (ibp->name);
    bdp->type = ibp->type;
    bdp->comment = StringSaveNotEmpty (ibp->comment);

    bdp->reason = ibp->reason;

    if (badges == NULL) {
      badges = bdp;
    }
    if (lastbdp != NULL) {
      lastbdp->next = bdp;
    }
    lastbdp = bdp;
  }

  return (Pointer) badges;
}

extern DialoG CreateBadgesDialog (
  GrouP prnt,
  SscTablesPtr stp
)

{
  GrouP       cts, tbl, g0, g1, g2, p;
  FonT        fnt;
  Int2        i, hgt, max, wid;
  SscBadgPtr  sbp;

  sbp = (SscBadgPtr) MemNew (sizeof (SscBadg));
  if (sbp == NULL) return NULL;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, sbp, StdCleanupExtraProc);
  sbp->dialog = (DialoG) p;
  sbp->todialog = AwardDataPtrToAdjPage;
  sbp->fromdialog = AdjPageToAwardDataPtr;

  sbp->tables = stp;

  SelectFont (systemFont);
  hgt = LineHeight ();
  badgColFmt [0].pixWidth = 25 * StringWidth ("X") + 5;
  max = StringWidth ("Diamond Distance ");
  badgColFmt [1].pixWidth = max + 5;
  SelectFont (systemFont);

  wid = 0;
  for (i = 0; i < 2; i++) {
    wid += badgColFmt [i].pixWidth;
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
  StaticPrompt (g0, "Name", badgColFmt [0].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Award", badgColFmt [1].pixWidth, 0, fnt, 'c');

  sbp->badglog = DocumentPanel (tbl, wid + 2, NUM_BADGE_LINES * hgt + 2);
  SetObjectExtra (sbp->badglog, sbp, NULL);
  SetDocCache (sbp->badglog, NULL, NULL, NULL);
  SetDocNotify (sbp->badglog, ChangeBadgTableSelect);
  sbp->numBadgs = 0;

  for (i = 0; i < NUM_BADGE_LINES; i++) {
    AppendItem (sbp->badglog, PrintBadgTable, sbp, FALSE, 1,
                &badgParFmt, badgColFmt, systemFont);
  }

  cts = HiddenGroup (p, -1, 0, NULL);
  SetGroupSpacing (cts, 10, 10);
  SetGroupMargins (cts, 5, 5);

  g1 = HiddenGroup (cts, -10, 0, NULL);
  SetGroupSpacing (g1, 10, 5);

  StaticPrompt (g1, "Name", 0, popupMenuHeight, programFont, 'l');
  sbp->name = CreateEnumPopupDialog (g1, TRUE, ChangeName, stp->payers.alist, (UIEnum) 0, sbp);

  StaticPrompt (g1, "Award", 0, popupMenuHeight, programFont, 'l');
  sbp->reason = CreateEnumPopupDialog (g1, TRUE, ChangeReason, reason_alist, (UIEnum) 0, sbp);
  SetObjectExtra (sbp->reason, sbp, NULL);

  g2 = HiddenGroup (cts, -10, 0, NULL);
  SetGroupSpacing (g2, 10, 5);

  StaticPrompt (g2, "Comment", 0, dialogTextHeight, programFont, 'l');
  sbp->comment = DialogText (g2, "", 20, ChangeComment);
  SetObjectExtra (sbp->comment, sbp, NULL);

  AlignObjects (ALIGN_CENTER, (HANDLE) tbl, (HANDLE) cts, NULL);

  sbp->numBadgs = 0;
  sbp->currItem = 1;

  return (DialoG) p;
}

