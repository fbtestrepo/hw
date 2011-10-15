/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Current introductory members page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* dialog controls */

typedef struct ssccurr {
  DIALOG_MESSAGE_BLOCK

  DoC           tempmemblog;

  SscTablesPtr  tables;
} SscCurr, PNTR SscCurrPtr;

#define NUM_TEMPMEMB_LINES 10

static ParData  membParFmt = { FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0 };

static ColData  membColFmt [] = {
  {0, 5, 25, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE}, /* name       */
  {0, 5, 12, 2, NULL, 'l', FALSE, TRUE,  FALSE, FALSE, TRUE}   /* expiration */
};

static CharPtr PrintMembTable (
  DoC d,
  Int2 item,
  Pointer data
)

{
  Char             buf [256], date [32];
  CalendarDatePtr  cdp;
  Int2             i;
  SscCurrPtr       scp;
  SscTablesPtr     stp;
  MemberInfoPtr    tmp;

  scp = (SscCurrPtr) GetObjectExtra (d);
  if (scp == NULL || item < 1 || item > 127) return NULL;
  stp = scp->tables;
  if (stp == NULL) return NULL;

  buf [0] = '\0';

  for (tmp = stp->intro_member_table, i = 1; tmp != NULL; tmp = tmp->next, i++) {
    if (item == i) break;
  }
  if (tmp == NULL) return StringSave ("\t \n");

  if (StringDoesHaveText (tmp->name)) {
    StringCat (buf, tmp->name);
  } else {
    StringCat (buf, " ");
  }

  StringCat (buf, "\t");
  cdp = tmp->expires;
  if (PrintCalendarDate (cdp, date, DATE_WITH_SLASHES)) {
    StringCat (buf, date);
  } else {
    StringCat (buf, " ");
  }

  StringCat (buf, "\n");
  return StringSave (buf);
}

static void TempMemberPtrToMembPage (
  DialoG d,
  Pointer data
)

{
  Int2           i, k;
  SscCurrPtr     scp;
  SscTablesPtr   stp;
  MemberInfoPtr  tempmembs, tmp;

  scp = (SscCurrPtr) GetObjectExtra (d);
  if (scp == NULL) return;
  stp = scp->tables;
  if (stp == NULL) return;
  tempmembs = (MemberInfoPtr) data;

  if (tempmembs == NULL) {
    Reset (scp->tempmemblog);
    for (i = 0; i < NUM_TEMPMEMB_LINES; i++) {
      AppendItem (scp->tempmemblog, PrintMembTable, scp, FALSE, 1,
                  &membParFmt, membColFmt, systemFont);
    }

    UpdateDocument (scp->tempmemblog, 0, 0);
    Update ();

    return;
  }

  Reset (scp->tempmemblog);

  for (tmp = tempmembs, k = 1; tmp != NULL; tmp = tmp->next, k++) {
    AppendItem (scp->tempmemblog, PrintMembTable, scp, FALSE, 1,
                &membParFmt, membColFmt, systemFont);
  }

  while (k <= NUM_TEMPMEMB_LINES) {
    AppendItem (scp->tempmemblog, PrintMembTable, scp, FALSE, 1,
                &membParFmt, membColFmt, systemFont);
    k++;
  }

  UpdateDocument (scp->tempmemblog, 0, 0);
  Update ();
}

static Int4 daysPerMonth [] = {
  0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

extern Boolean AddToTempMemberList (
  SscTablesPtr stp,
  CharPtr name,
  Int2 days
)

{
  MsgAnswer        ans;
  CalendarDatePtr  cdp = NULL;
  Char             exps [32];
  Char             mssg [256];
  CharPtr          str;
  MemberInfoPtr    tmp, lasttmp = NULL, head, next, PNTR prev;

  if (stp == NULL || StringHasNoText (name)) return FALSE;

  for (tmp = stp->intro_member_table; tmp != NULL; tmp = tmp->next) {
    lasttmp = tmp;
    str = (CharPtr) tmp->name;
    if (StringHasNoText (str)) continue;
    if (StringICmp (str, name) != 0) continue;

    if (tmp->expires != NULL && PrintCalendarDate (tmp->expires, exps, DATE_FOR_DISPLAY)) {
      sprintf (mssg, "This person is currently an active temporary member until %s.  Do you wish to add anyway?", exps);
    } else {
      sprintf (mssg, "This person is currently an active temporary.  Do you wish to add anyway?");
    }
    ans = Message (MSG_YN, "%s", mssg);
    if (ans == ANS_NO) return FALSE;
  }

  ClearDirtyFlags (stp);

  AddNameToExtras (&(stp->members), name);
  AddNameToExtras (&(stp->payers), name);

  UpdateFlightControls (stp->glider_flights);
  UpdateFeesControls (stp->fees);
  UpdateChargesControls (stp->charges);
  UpdateAdjustmentsControls (stp->adjustments);

  ClearDirtyFlags (stp);

  if (days > 0) {
    tmp = MemberInfoNew ();
    if (tmp != NULL) {
      tmp->name = StringSave (name);
      cdp = CalendarDateNew ();
      if (cdp != NULL) {
        cdp->month = stp->currentDate.month;
        cdp->day = stp->currentDate.day;
        cdp->year = stp->currentDate.year;
        if (cdp->month >= 1 && cdp->month <= 12) {
          cdp->day += days;
          while (cdp->day > daysPerMonth [cdp->month]) {
            cdp->day -= daysPerMonth [cdp->month];
            (cdp->month)++;
            if (cdp->month > 12) {
              (cdp->year)++;
              cdp->month = 1;
            }
          }
        }
      }
      tmp->expires = cdp;
    }
  }

  if (lasttmp != NULL) {
    lasttmp->next = tmp;
  }
  if (stp->intro_member_table == NULL) {
    stp->intro_member_table = tmp;
  }

  AppendTempMemberTable ("tempmemb.txt", name, cdp);

  head = stp->intro_member_table;
  if (head != NULL && head->next != NULL) {
    for (lasttmp = head; lasttmp != NULL; lasttmp = lasttmp->next) {
      for (tmp = lasttmp->next; tmp != NULL; tmp = tmp->next) {
        if (StringICmp (lasttmp->name, tmp->name) == 0) {
          lasttmp->name = MemFree (lasttmp->name);
        }
      }
    }
    prev = &(stp->intro_member_table);
    tmp = stp->intro_member_table;
    while (tmp != NULL) {
      next = tmp->next;
      if (StringHasNoText (tmp->name)) {
        *prev = tmp->next;
        tmp->next = NULL;
        MemberInfoFree (tmp);
      } else {
        prev = (MemberInfoPtr PNTR) &(tmp->next);
      }
      tmp = next;
    }
  }

  TempMemberPtrToMembPage (stp->intro_members, stp->intro_member_table);

  return TRUE;
}

extern DialoG CreateCurrentIntroDialog (
  GrouP prnt,
  SscTablesPtr stp
)

{
  FonT        fnt;
  GrouP       g, h, p;
  Int2        i, hgt, wid;
  SscCurrPtr  scp;

  if (stp == NULL) return NULL;

  scp = (SscCurrPtr) MemNew (sizeof (SscCurr));
  if (scp == NULL) return NULL;

  p = HiddenGroup (prnt, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, scp, StdCleanupExtraProc);

  scp->dialog = (DialoG) p;
  scp->todialog = TempMemberPtrToMembPage;

  scp->tables = stp;

  SelectFont (systemFont);
  hgt = LineHeight ();
  membColFmt [0].pixWidth = StringWidth ("XXXXXXXXXXXXXXXXXXXXXXXXX") + 5;
  membColFmt [1].pixWidth = StringWidth ("99/99/9999 ") + 5;
  SelectFont (systemFont);

  wid = 0;
  for (i = 0; i < 2; i++) {
    wid += membColFmt [i].pixWidth;
  }

  h = HiddenGroup (p, -1, 0, NULL);
  SetGroupSpacing (h, 10, 5);
  SetGroupMargins (h, 5, 5);

  g = HiddenGroup (h, 15, 0, NULL);
  SetGroupSpacing (g, 0, 3);
#ifdef WIN_MSWIN
  fnt = systemFont;
#else
  fnt = programFont;
#endif
  StaticPrompt (g, "Name", membColFmt [0].pixWidth, 0, fnt, 'c');
  StaticPrompt (g, "Expires", membColFmt [1].pixWidth, 0, fnt, 'c');

  scp->tempmemblog = DocumentPanel (h, wid + 2, NUM_TEMPMEMB_LINES * hgt + 2);
  SetObjectExtra (scp->tempmemblog, scp, NULL);
  SetDocCache (scp->tempmemblog, NULL, NULL, NULL);

  TempMemberPtrToMembPage ((DialoG) p, stp->intro_member_table);

  return (DialoG) p;
}

