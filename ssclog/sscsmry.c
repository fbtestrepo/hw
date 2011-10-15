/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Summary page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* dialog controls */

typedef struct pilotsummary {
  DIALOG_MESSAGE_BLOCK

  DoC           pilotlog;

  SscTablesPtr  tables;
  BaseFormPtr   bfp;
} PilotSummary, PNTR PilotSummaryPtr;

#define NUM_PILOT_LINES 10

static ParData  pilotParFmt = { FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0 };

static ColData  pilotColFmt [] = {
  {0, 5, 25, 3, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE},  /* name    */
  {0, 5, 12, 2, NULL, 'l', FALSE, TRUE,  FALSE, FALSE, FALSE},  /* glider  */
  {0, 5,  7, 2, NULL, 'r', FALSE, TRUE,  FALSE, FALSE, FALSE},  /* time    */
  {0, 5,  9, 2, NULL, 'r', FALSE, FALSE, FALSE, FALSE, FALSE},  /* release */
  {0, 5,  9, 2, NULL, 'r', FALSE, FALSE, FALSE, FALSE, TRUE}    /* total   */
};

static int LIBCALLBACK SortVnpByPilot (
  VoidPtr ptr1,
  VoidPtr ptr2
)

{
  int              compare;
  GliderFlightPtr  fip1, fip2;
  CharPtr          name1, name2;
  CharPtr          str1, str2;
  ValNodePtr       vnp1, vnp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;

  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;

  fip1 = (GliderFlightPtr) vnp1->data.ptrvalue;
  fip2 = (GliderFlightPtr) vnp2->data.ptrvalue;
  if (fip1 == NULL || fip2 == NULL) return 0;

  name1 = fip1->name;
  name2 = fip2->name;
  if (name1 == NULL || name2 == NULL) return 0;

  str1 = FindLastName (name1);
  str2 = FindLastName (name2);

  compare = StringICmp (str1, str2);

  if (compare > 0) {
    return 1;
  } else if (compare < 0) {
    return -1;
  }

  str1 = name1;
  str2 = name2;

  compare = StringICmp (str1, str2);

  if (compare > 0) {
    return 1;
  } else if (compare < 0) {
    return -1;
  }

  if (fip1->flight_number > fip2->flight_number) return 1;
  if (fip1->flight_number < fip2->flight_number) return -1;

  return 0;
}

static void FlightInfoPtrToPilotSummaryPage (
  DialoG d,
  Pointer data
)

{
  Char             buf [256], tmp1 [32], tmp2 [32], tmp3 [32];
  GliderFlightPtr  fip, flights;
  ValNodePtr       head = NULL, last = NULL, vnp;
  Int2             i = 0;
  CharPtr          nm, gl, rl, tm, tt;
  Int4             num, rel_hund, rel_thou;
  PilotSummaryPtr  psp;

  psp = (PilotSummaryPtr) GetObjectExtra (d);
  if (psp == NULL) return;
  flights = (GliderFlightPtr) data;

  Reset (psp->pilotlog);

  if (flights == NULL) {
    Reset (psp->pilotlog);
    for (i = 0; i < NUM_PILOT_LINES; i++) {
      AppendText (psp->pilotlog, " \t \t \t \t \n",
                  &pilotParFmt, pilotColFmt, systemFont);
    }
    return;
  }

  for (fip = flights, num = 1; fip != NULL; fip = fip->next, num++) {
    fip->flight_number = num;
    vnp = ValNodeAddPointer (&last, 0, (Pointer) fip);
    if (head == NULL) {
      head = vnp;
    }
    last = vnp;
  }

  head = ValNodeSort (head, SortVnpByPilot);

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    fip = (GliderFlightPtr) vnp->data.ptrvalue;
    if (fip == NULL) continue;

    nm = fip->name;
    if (StringHasNoText (nm)) {
      nm = " ";
    }

    gl = fip->glider;
    if (StringHasNoText (gl)) {
      gl = " ";
    }

    tm = NULL;
    if (PrintFlightTime (fip->flight_time, tmp1)) {
      tm = tmp1;
    }
    if (StringHasNoText (tm)) {
      tm = " ";
    }

    rl = NULL;
    if (fip->release_altitude >= 0) {
      sprintf (tmp2, "%ld", (long) fip->release_altitude);
      rel_thou = fip->release_altitude / 1000;
      rel_hund = fip->release_altitude % 1000;
      sprintf (tmp2, "%1ld,%3ld", (long) rel_thou, (long) rel_hund);
      if (tmp2 [2] == ' ') {
        tmp2 [2] = '0';
      }
      if (tmp2 [3] == ' ') {
        tmp2 [3] = '0';
      }
      rl = tmp2;
    }
    if (StringHasNoText (rl)) {
      rl = " ";
    }

    tt = NULL;
    if (PrintDollarAmount (fip->total_cost, tmp3)) {
      tt = tmp3;
    }
    if (StringHasNoText (tt)) {
      tt = " ";
    }

    sprintf (buf, "%s\t%s\t%s\t%s\t%s\n",
             nm, gl, tm, rl, tt);
    AppendText (psp->pilotlog, buf, &pilotParFmt, pilotColFmt, systemFont);

    i++;
  }

  ValNodeFree (head);

  while (i < NUM_PILOT_LINES) {
    AppendText (psp->pilotlog, " \t \t \t \t \n",
                &pilotParFmt, pilotColFmt, systemFont);
    i++;
  }

  Update ();
}

extern void RecalculatePilotSummary (
  DialoG sfpflights,
  DialoG pilotsummary
)

{
  GliderFlightPtr  flights;
  PilotSummaryPtr  psp;

  psp = (PilotSummaryPtr) GetObjectExtra (pilotsummary);
  if (psp == NULL) return;

  SafeHide (psp->pilotlog);
  Update ();

  flights = DialogToPointer (sfpflights);

  PointerToDialog (pilotsummary, (Pointer) flights);

  flights = FlightListFree (flights);

  SafeShow (psp->pilotlog);
  Update ();
}

extern DialoG CreatePilotSummaryDialog (
  GrouP prnt,
  SscTablesPtr stp,
  BaseFormPtr bfp
)

{
  FonT             fnt;
  Int2             i, hgt, wid;
  GrouP            p, g0, tbl;
  PilotSummaryPtr  psp;

  psp = (PilotSummaryPtr) MemNew (sizeof (PilotSummary));
  if (psp == NULL) return NULL;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, psp, StdCleanupExtraProc);
  psp->dialog = (DialoG) p;
  psp->todialog = FlightInfoPtrToPilotSummaryPage;
  psp->fromdialog = NULL;

  psp->tables = stp;
  psp->bfp = bfp;

  tbl = HiddenGroup (p, -1, 0, NULL);
  SetGroupSpacing (tbl, 10, 5);
  SetGroupMargins (tbl, 5, 5);

  SelectFont (systemFont);
  hgt = LineHeight ();
  pilotColFmt [0].pixWidth = 25 * StringWidth ("X") + 5;
  pilotColFmt [1].pixWidth = StringWidth ("GROB 103 ") + 5;
  pilotColFmt [2].pixWidth = StringWidth ("99:99 ") + 5;
  pilotColFmt [3].pixWidth = StringWidth ("10000 ") + 5;
  pilotColFmt [4].pixWidth = StringWidth ("$100.00 ") + 5;
  SelectFont (systemFont);

  wid = 0;
  for (i = 0; i < 5; i++) {
    wid += pilotColFmt [i].pixWidth;
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
  StaticPrompt (g0, "Name", pilotColFmt [0].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Glider", pilotColFmt [1].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Time", pilotColFmt [2].pixWidth, 0, fnt, 'r');
  StaticPrompt (g0, "Alt", pilotColFmt [3].pixWidth, 0, fnt, 'r');
  StaticPrompt (g0, "Total", pilotColFmt [4].pixWidth, 0, fnt, 'r');

  psp->pilotlog = DocumentPanel (tbl, wid + 2, NUM_PILOT_LINES * hgt + 2);
  SetObjectExtra (psp->pilotlog, psp, NULL);
  SetDocCache (psp->pilotlog, NULL, NULL, NULL);

  return (DialoG) p;
}

/* dialog controls */

typedef struct instrsummary {
  DIALOG_MESSAGE_BLOCK

  DoC           instrlog;

  SscTablesPtr  tables;
  BaseFormPtr   bfp;
} InstrSummary, PNTR InstrSummaryPtr;

#define NUM_INSTR_LINES 10

static ParData  instrParFmt = { FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0 };

static ColData  instrColFmt [] = {
  {0, 5, 25, 3, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE},  /* instructor */
  {0, 5, 25, 3, NULL, 'l', FALSE, TRUE,  FALSE, FALSE, FALSE},  /* name       */
  {0, 5, 12, 2, NULL, 'l', FALSE, TRUE,  FALSE, FALSE, FALSE},  /* glider     */
  {0, 5,  7, 2, NULL, 'r', FALSE, TRUE,  FALSE, FALSE, FALSE},  /* time       */
  {0, 5,  9, 2, NULL, 'r', FALSE, FALSE, FALSE, FALSE, TRUE}    /* release    */
};

static int LIBCALLBACK SortVnpByInstr (
  VoidPtr ptr1,
  VoidPtr ptr2
)

{
  int              compare;
  GliderFlightPtr  fip1, fip2;
  CharPtr          name1, name2;
  CharPtr          str1, str2;
  ValNodePtr       vnp1, vnp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;

  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;

  fip1 = (GliderFlightPtr) vnp1->data.ptrvalue;
  fip2 = (GliderFlightPtr) vnp2->data.ptrvalue;
  if (fip1 == NULL || fip2 == NULL) return 0;

  name1 = fip1->instructor;
  name2 = fip2->instructor;
  if (name1 == NULL || name2 == NULL) return 0;

  str1 = FindLastName (name1);
  str2 = FindLastName (name2);

  compare = StringICmp (str1, str2);

  if (compare > 0) {
    return 1;
  } else if (compare < 0) {
    return -1;
  }

  str1 = name1;
  str2 = name2;

  compare = StringICmp (str1, str2);

  if (compare > 0) {
    return 1;
  } else if (compare < 0) {
    return -1;
  }

  if (fip1->flight_number > fip2->flight_number) return 1;
  if (fip1->flight_number < fip2->flight_number) return -1;

  return 0;
}

static void FlightInfoPtrToInstrSummaryPage (
  DialoG d,
  Pointer data
)

{
  Char             buf [256], tmp1 [32], tmp2 [32];
  GliderFlightPtr  fip, flights;
  ValNodePtr       head = NULL, last = NULL, vnp;
  Int2             i = 0;
  CharPtr          in, nm, gl, rl, tm;
  InstrSummaryPtr  isp;
  Int4             num, rel_hund, rel_thou;

  isp = (InstrSummaryPtr) GetObjectExtra (d);
  if (isp == NULL) return;
  flights = (GliderFlightPtr) data;

  Reset (isp->instrlog);

  if (flights == NULL) {
    Reset (isp->instrlog);
    for (i = 0; i < NUM_INSTR_LINES; i++) {
      AppendText (isp->instrlog, " \t \t \t \t \n",
                  &instrParFmt, instrColFmt, systemFont);
    }
    return;
  }

  for (fip = flights, num = 1; fip != NULL; fip = fip->next, num++) {
    fip->flight_number = num;
		if (StringHasNoText (fip->instructor)) continue;
    vnp = ValNodeAddPointer (&last, 0, (Pointer) fip);
    if (head == NULL) {
      head = vnp;
    }
    last = vnp;
  }

  head = ValNodeSort (head, SortVnpByInstr);

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    fip = (GliderFlightPtr) vnp->data.ptrvalue;
    if (fip == NULL) continue;

    in = fip->instructor;
    if (StringHasNoText (in)) {
      in = " ";
    }

    nm = fip->name;
    if (StringHasNoText (nm)) {
      nm = " ";
    }

    gl = fip->glider;
    if (StringHasNoText (gl)) {
      gl = " ";
    }

    tm = NULL;
    if (PrintFlightTime (fip->flight_time, tmp1)) {
      tm = tmp1;
    }
    if (StringHasNoText (tm)) {
      tm = " ";
    }

    rl = NULL;
    if (fip->release_altitude >= 0) {
      sprintf (tmp2, "%ld", (long) fip->release_altitude);
      rel_thou = fip->release_altitude / 1000;
      rel_hund = fip->release_altitude % 1000;
      sprintf (tmp2, "%1ld,%3ld", (long) rel_thou, (long) rel_hund);
      if (tmp2 [2] == ' ') {
        tmp2 [2] = '0';
      }
      if (tmp2 [3] == ' ') {
        tmp2 [3] = '0';
      }
      rl = tmp2;
    }
    if (StringHasNoText (rl)) {
      rl = " ";
    }

    sprintf (buf, "%s\t%s\t%s\t%s\t%s\n",
             in, nm, gl, tm, rl);
    AppendText (isp->instrlog, buf, &instrParFmt, instrColFmt, systemFont);

    i++;
  }

  ValNodeFree (head);

  while (i < NUM_INSTR_LINES) {
    AppendText (isp->instrlog, " \t \t \t \t \n",
                &instrParFmt, instrColFmt, systemFont);
    i++;
  }

  Update ();
}

extern void RecalculateInstrSummary (
  DialoG sfpflights,
  DialoG instrsummary
)

{
  GliderFlightPtr  flights;
  InstrSummaryPtr  isp;

  isp = (InstrSummaryPtr) GetObjectExtra (instrsummary);
  if (isp == NULL) return;

  SafeHide (isp->instrlog);
  Update ();

  flights = DialogToPointer (sfpflights);

  PointerToDialog (instrsummary, (Pointer) flights);

  flights = FlightListFree (flights);

  SafeShow (isp->instrlog);
  Update ();
}

extern DialoG CreateInstrSummaryDialog (
  GrouP prnt,
  SscTablesPtr stp,
  BaseFormPtr bfp
)

{
  FonT             fnt;
  Int2             i, hgt, wid;
  InstrSummaryPtr  isp;
  GrouP            p, g0, tbl;

  isp = (InstrSummaryPtr) MemNew (sizeof (InstrSummary));
  if (isp == NULL) return NULL;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, isp, StdCleanupExtraProc);
  isp->dialog = (DialoG) p;
  isp->todialog = FlightInfoPtrToInstrSummaryPage;
  isp->fromdialog = NULL;

  isp->tables = stp;
  isp->bfp = bfp;

  tbl = HiddenGroup (p, -1, 0, NULL);
  SetGroupSpacing (tbl, 10, 5);
  SetGroupMargins (tbl, 5, 5);

  SelectFont (systemFont);
  hgt = LineHeight ();
  instrColFmt [0].pixWidth = 25 * StringWidth ("X") + 5;
  instrColFmt [1].pixWidth = 25 * StringWidth ("X") + 5;
  instrColFmt [2].pixWidth = StringWidth ("GROB 103 ") + 5;
  instrColFmt [3].pixWidth = StringWidth ("99:99 ") + 5;
  instrColFmt [4].pixWidth = StringWidth ("10000 ") + 5;
  SelectFont (systemFont);

  wid = 0;
  for (i = 0; i < 5; i++) {
    wid += instrColFmt [i].pixWidth;
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
  StaticPrompt (g0, "Instructor", instrColFmt [0].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Name", instrColFmt [1].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Glider", instrColFmt [2].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Time", instrColFmt [3].pixWidth, 0, fnt, 'r');
  StaticPrompt (g0, "Alt", instrColFmt [4].pixWidth, 0, fnt, 'r');

  isp->instrlog = DocumentPanel (tbl, wid + 2, NUM_INSTR_LINES * hgt + 2);
  SetObjectExtra (isp->instrlog, isp, NULL);
  SetDocCache (isp->instrlog, NULL, NULL, NULL);

  return (DialoG) p;
}

