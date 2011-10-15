/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Checklist page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* dialog controls */

static CharPtr checklistStrs1 [] = {
  "Call Potomac TRACON at 540 349-7541 and announce flight operations area.  Read text below, ",
  NULL
};

static CharPtr checklistStrs2 [] = {
  "substituting the actual values for your name, start time and stop time (UTC), and expected ",
  "maximum altitude (MSL): ",
  "",
  NULL
};

static CharPtr checklistStrs3 [] = {
  "Good morning, my name is [NAME] and I'm today's Duty Officer at the Skyline ",
  "Soaring Club in Front Royal, Virginia.  We plan to start glider operations ",
  NULL
};

static CharPtr optimeStr =
  "at XX00 zulu today, and terminate by XX00 zulu.  We will be operating ";

static CharPtr opaltStr =
  "between the surface and X,000 feet MSL in the area ten miles North and West ";

static CharPtr checklistStrs4 [] = {
  "of the Linden VOR.  If we cease operations earlier, or if the operational ",
  "ceiling changes, I will give you a call. ",
  NULL
};

typedef struct ssccheck {
  DIALOG_MESSAGE_BLOCK

  PopuP    start_time;
  PopuP    stop_time;
  PopuP    max_altitude;

  PrompT   time_ppt;
  PrompT   alt_ppt;

  Boolean  is_dst;
} SscCheck, PNTR SscCheckPtr;

static ENUM_ALIST(start_time_alist)
  {" ",       0},
  {" 8 AM",   8},
  {" 9 AM",   9},
  {"10 AM",  10},
  {"11 AM",  11},
  {"12 NN",  12},
  {" 1 PM",  13},
  {" 2 PM",  14},
  {" 3 PM",  15},
END_ENUM_ALIST

static ENUM_ALIST(stop_time_alist)
  {" ",       0},
  {" 2 PM",  14},
  {" 3 PM",  15},
  {" 4 PM",  16},
  {" 5 PM",  17},
  {" 6 PM",  18},
  {" 7 PM",  19},
  {" 8 PM",  20},
  {" 9 PM",  21},
END_ENUM_ALIST

static ENUM_ALIST(max_altitude_alist)
  {" ",      0},
  {" 1000",  1},
  {" 2000",  2},
  {" 3000",  3},
  {" 4000",  4},
  {" 5000",  5},
  {" 6000",  6},
  {" 7000",  7},
  {" 8000",  8},
  {" 9000",  9},
END_ENUM_ALIST

static void ChangeZulu (
  PopuP p
)

{
  Char         buf [120];
  Int2         dst_offset = 0, start = 0, stop = 0;
  UIEnum       hour;
  SscCheckPtr  scp;

  scp = (SscCheckPtr) GetObjectExtra (p);
  if (scp == NULL) return;

  if (scp->is_dst) {
    dst_offset = -1;
  }

  if (GetEnumPopup (scp->start_time, start_time_alist, &hour)) {
    if (hour > 0) {
      start = hour + 5 + dst_offset;
    }
  }
  if (GetEnumPopup (scp->stop_time, stop_time_alist, &hour)) {
    if (hour > 0) {
      stop = hour + 5 + dst_offset;
    }
  }

  if (start == 0 || stop == 0) {
    SetTitle (scp->time_ppt, optimeStr);
    return;
  }

  if (start > 24) {
    start -= 24;
  }
  if (stop > 24) {
    stop -= 24;
  }

  sprintf (buf,
           "at %2d00 zulu today, and terminate by %2d00 zulu.  We will be operating ",
           (int) start, (int) stop);
  SetTitle (scp->time_ppt, buf);
}

static void ChangeMaxAlt (
  PopuP p
)

{
  Char         buf [120];
  UIEnum       maxalt;
  Int2         release = 0;
  SscCheckPtr  scp;

  scp = (SscCheckPtr) GetObjectExtra (p);
  if (scp == NULL) return;

  if (GetEnumPopup (scp->max_altitude, max_altitude_alist, &maxalt)) {
    if (maxalt > 0) {
      release = maxalt;
    }
  }

  if (release == 0) {
    SetTitle (scp->alt_ppt, opaltStr);
    return;
  }

  sprintf (buf,
           "between the surface and %d,000 feet MSL in the area ten miles North and West ",
           (int) release);
  SetTitle (scp->alt_ppt, buf);
}

extern DialoG CreateChecklistDialog (
  GrouP prnt,
  SscTablesPtr stp
)

{
  Char         buf [80];
  FonT         fnt;
  GrouP        g, h, k, p;
  Int2         i;
  PrompT       ppt;
  SscCheckPtr  scp;
  CharPtr      zone = "EST";

  scp = (SscCheckPtr) MemNew (sizeof (SscCheck));
  if (scp == NULL) return NULL;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, scp, StdCleanupExtraProc);
  scp->dialog = (DialoG) p;

  scp->is_dst = stp->is_dst;

  if (scp->is_dst) {
    zone = "EDT";
  }

#ifdef WIN_MSWIN
  fnt = systemFont;
#else
  fnt = programFont;
#endif

  sprintf (buf, "%s%s%s",
           "Fill in local start and stop time to convert ",
           zone, " to UTC in the text below.");
  ppt = StaticPrompt (p, buf, 0, 0, fnt, 'c');

  g = HiddenGroup (p, 8, 0, NULL);
  SetGroupSpacing (g, 10, 5);
  SetGroupMargins (g, 5, 5);

  StaticPrompt (g, "Start Time", 0, popupMenuHeight, fnt, 'l');
  scp->start_time =  PopupList (g, TRUE, ChangeZulu);
  SetObjectExtra (scp->start_time, (Pointer) scp, NULL);
  InitEnumPopup (scp->start_time, start_time_alist, NULL);
  SetValue (scp->start_time, 1);

  StaticPrompt (g, "Stop Time", 0, popupMenuHeight, fnt, 'l');
  scp->stop_time = PopupList (g, TRUE, ChangeZulu);
  SetObjectExtra (scp->stop_time, (Pointer) scp, NULL);
  InitEnumPopup (scp->stop_time, stop_time_alist, NULL);
  SetValue (scp->stop_time, 1);

  StaticPrompt (g, "Maximum Altitude", 0, popupMenuHeight, fnt, 'l');
  scp->max_altitude = PopupList (g, TRUE, ChangeMaxAlt);
  SetObjectExtra (scp->max_altitude, (Pointer) scp, NULL);
  InitEnumPopup (scp->max_altitude, max_altitude_alist, NULL);
  SetValue (scp->max_altitude, 1);
  StaticPrompt (g, "(MSL)", 0, popupMenuHeight, fnt, 'l');

  k = HiddenGroup (p, 1, 0, NULL);
  SetGroupSpacing (k, 5, 3);

  for (i = 0; checklistStrs1 [i] != NULL; i++) {
    StaticPrompt (k, checklistStrs1 [i], 0, 0, fnt, 'l');
  }

  for (i = 0; checklistStrs2 [i] != NULL; i++) {
    StaticPrompt (k, checklistStrs2 [i], 0, 0, fnt, 'l');
  }

  h = HiddenGroup (p, 1, 0, NULL);
  SetGroupSpacing (h, 5, 3);

  for (i = 0; checklistStrs3 [i] != NULL; i++) {
    StaticPrompt (h, checklistStrs3 [i], 0, 0, fnt, 'l');
  }

  scp->time_ppt = StaticPrompt (h, optimeStr, 0, 0, fnt, 'l');

  scp->alt_ppt = StaticPrompt (h, opaltStr, 0, 0, fnt, 'l');

  for (i = 0; checklistStrs4 [i] != NULL; i++) {
    StaticPrompt (h, checklistStrs4 [i], 0, 0, fnt, 'l');
  }

  AlignObjects (ALIGN_CENTER, (HANDLE) ppt, (HANDLE) g, (HANDLE) h, (HANDLE) k, NULL);

  return (DialoG) p;
}

