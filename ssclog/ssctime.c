/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Time page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* dialog controls */

typedef struct timepage {
  DIALOG_MESSAGE_BLOCK
  PopuP  hour;
  PopuP  minute;
} TimePage, PNTR TimePagePtr;

static ENUM_ALIST(hours_alist)
  {" ",   -1},
  {"00",   0},
  {"01",   1},
  {"02",   2},
  {"03",   3},
  {"04",   4},
  {"05",   5},
  {"06",   6},
  {"07",   7},
  {"08",   8},
  {"09",   9},
  {"10",  10},
  {"11",  11},
  {"12",  12},
  {"13",  13},
  {"14",  14},
  {"15",  15},
  {"16",  16},
  {"17",  17},
  {"18",  18},
  {"19",  19},
  {"20",  20},
  {"21",  21},
  {"22",  22},
  {"23",  23},
END_ENUM_ALIST

static ENUM_ALIST(minutes_alist)
  {" ",   -1},
  {"00",   0},
  {"01",   1},
  {"02",   2},
  {"03",   3},
  {"04",   4},
  {"05",   5},
  {"06",   6},
  {"07",   7},
  {"08",   8},
  {"09",   9},
  {"10",  10},
  {"11",  11},
  {"12",  12},
  {"13",  13},
  {"14",  14},
  {"15",  15},
  {"16",  16},
  {"17",  17},
  {"18",  18},
  {"19",  19},
  {"20",  20},
  {"21",  21},
  {"22",  22},
  {"23",  23},
  {"24",  24},
  {"25",  25},
  {"26",  26},
  {"27",  27},
  {"28",  28},
  {"29",  29},
  {"30",  30},
  {"31",  31},
  {"32",  32},
  {"33",  33},
  {"34",  34},
  {"35",  35},
  {"36",  36},
  {"37",  37},
  {"38",  38},
  {"39",  39},
  {"40",  40},
  {"41",  41},
  {"42",  42},
  {"43",  43},
  {"44",  44},
  {"45",  45},
  {"46",  46},
  {"47",  47},
  {"48",  48},
  {"49",  49},
  {"50",  50},
  {"51",  51},
  {"52",  52},
  {"53",  53},
  {"54",  54},
  {"55",  55},
  {"56",  56},
  {"57",  57},
  {"58",  58},
  {"59",  59},
END_ENUM_ALIST

static void ChangeTimeProc (
  PopuP p
)

{
  DialogActnFunc  actproc;
  TimePagePtr     tpp;

  tpp = (TimePagePtr) GetObjectExtra (p);
  if (tpp == NULL) return;

  actproc = tpp->actproc;
  if (actproc != NULL) {
    actproc (tpp->dialog);
  }
}

static void ClockTimePtrToTimePage (
  DialoG d,
  Pointer data
)

{
  ClockTimePtr  ctp;
  TimePagePtr   tpp;

  tpp = (TimePagePtr) GetObjectExtra (d);
  if (tpp == NULL) return;
  ctp = (ClockTimePtr) data;
  if (ctp != NULL) {
    SetEnumPopup (tpp->hour, hours_alist, (UIEnum) ctp->hour);
    SetEnumPopup (tpp->minute, minutes_alist, (UIEnum) ctp->minute);
    return;
  }

  SafeSetValue (tpp->hour, 1);
  SafeSetValue (tpp->minute, 1);
}

static Pointer TimePageToClockTimePtr (
  DialoG d
)

{
  ClockTimePtr  ctp;
  UIEnum        hour;
  UIEnum        minute;
  TimePagePtr   tpp;

  ctp = NULL;
  tpp = (TimePagePtr) GetObjectExtra (d);
  if (tpp == NULL) return NULL;
  ctp = ClockTimeNew ();
  if (ctp != NULL) {

    if (GetEnumPopup (tpp->hour, hours_alist, &hour)) {
      ctp->hour = (Int4) hour;
    } else {
      ctp->hour = -1;
    }

    if (GetEnumPopup (tpp->minute, minutes_alist, &minute)) {
      ctp->minute = (Int4) minute;
    } else {
      ctp->minute = -1;
    }

    /*
    if (ctp->hour == -1 || ctp->minute == -1) {
      ctp = ClockTimeFree (ctp);
    }
    */
  }
  return (Pointer) ctp;
}

extern void GetClockTime (
  DialoG d,
  Int4Ptr hour,
  Int4Ptr minute
)

{
  ClockTimePtr  ctp;

  if (d == NULL || hour == NULL || minute == NULL) return;

  ctp = (ClockTimePtr) DialogToPointer (d);
  if (ctp != NULL) {
    *hour = ctp->hour;
    *minute = ctp->minute;
    ctp = ClockTimeFree (ctp);
    return;
  }

  *hour = -1;
  *minute = -1;
}

extern void SetClockTime (
  DialoG d,
  Int4 hour,
  Int4 minute
)

{
  ClockTime  ct;

  if (d == NULL) return;

  ct.hour = hour;
  ct.minute = minute;

  PointerToDialog (d, (Pointer) &ct);
}

extern DialoG CreateClockTimeDialog (
  GrouP prnt,
  Pointer userdata,
  DialogActnFunc actproc
)

{
  TimePagePtr  tpp;
  GrouP        f;
  GrouP        p;

  p = HiddenGroup (prnt, 1, 0, NULL);

  tpp = (TimePagePtr) MemNew (sizeof (TimePage));
  if (tpp) {

    SetObjectExtra (p, tpp, StdCleanupExtraProc);
    tpp->dialog = (DialoG) p;
    tpp->todialog = ClockTimePtrToTimePage;
    tpp->fromdialog = TimePageToClockTimePtr;
    tpp->testdialog = NULL;

    tpp->actproc = actproc;
    tpp->userdata = userdata;

    f = HiddenGroup (p, -6, 0, NULL);

    /*
    tpp->hour = CreateEnumPopupDialog (f, TRUE, ChangeTime, hours_alist, 0, (Pointer) tpp);
    StaticPrompt (f, ":", 0, popupMenuHeight, programFont, 'l');
    tpp->minute = CreateEnumPopupDialog (f, TRUE, ChangeTime, minutes_alist, 0, (Pointer) tpp);
    */

    tpp->hour = PopupList (f, TRUE, ChangeTimeProc);
    SetObjectExtra (tpp->hour, (Pointer) tpp, NULL);
    InitEnumPopup (tpp->hour, hours_alist, NULL);
    SetValue (tpp->hour, 1);

    StaticPrompt (f, ":", 0, popupMenuHeight, programFont, 'l');

    tpp->minute = PopupList (f, TRUE, ChangeTimeProc);
    SetObjectExtra (tpp->minute, (Pointer) tpp, NULL);
    InitEnumPopup (tpp->minute, minutes_alist, NULL);
    SetValue (tpp->minute, 1);
  }

  return (DialoG) p;
}

