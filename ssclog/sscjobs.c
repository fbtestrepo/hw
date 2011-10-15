/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Jobs page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* dialog controls */

typedef struct jobspage {
  DIALOG_MESSAGE_BLOCK

  ButtoN  dutyofficer;
  ButtoN  assistant;
  ButtoN  instructor;
  ButtoN  towpilot;
} JobsPage, PNTR JobsPagePtr;

extern void GetClubJobs (
  DialoG d,
  BoolPtr dutyofficer,
  BoolPtr assistant,
  BoolPtr instructor,
  BoolPtr towpilot
)

{
  JobsPagePtr  jpp;

  if (d == NULL) return;

  jpp = (JobsPagePtr) GetObjectExtra (d);
  if (jpp == NULL) return;

  if (dutyofficer != NULL) {
    *dutyofficer = GetStatus (jpp->dutyofficer);
  }
  if (assistant != NULL) {
    *assistant = GetStatus (jpp->assistant);
  }
  if (instructor != NULL) {
    *instructor = GetStatus (jpp->instructor);
  }
  if (towpilot != NULL) {
    *towpilot = GetStatus (jpp->towpilot);
  }
}

extern void SetClubJobs (
  DialoG d,
  Boolean dutyofficer,
  Boolean assistant,
  Boolean instructor,
  Boolean towpilot
)

{
  JobsPagePtr  jpp;

  if (d == NULL) return;
  jpp = (JobsPagePtr) GetObjectExtra (d);
  if (jpp == NULL) return;

  SetStatus (jpp->dutyofficer, dutyofficer);
  SetStatus (jpp->assistant, assistant);
  SetStatus (jpp->instructor, instructor);
  SetStatus (jpp->towpilot, towpilot);
}

extern DialoG CreateClubJobsDialog (
  GrouP prnt
)

{
  GrouP        d, p;
  JobsPagePtr  jpp;

  p = HiddenGroup (prnt, 1, 0, NULL);

  jpp = (JobsPagePtr) MemNew (sizeof (JobsPage));
  if (jpp != NULL) {

    SetObjectExtra (p, jpp, StdCleanupExtraProc);
    jpp->dialog = (DialoG) p;

    d = HiddenGroup (p, 5, 0, NULL);
    SetGroupSpacing (d, 5, 3);
    jpp->dutyofficer = CheckBox (d, "Duty Officer", NULL);
    SetStatus (jpp->dutyofficer, FALSE);
    jpp->assistant = CheckBox (d, "Assistant", NULL);
    SetStatus (jpp->assistant, FALSE);
    jpp->instructor = CheckBox (d, "Instructor", NULL);
    SetStatus (jpp->instructor, FALSE);
    jpp->towpilot = CheckBox (d, "Tow Pilot", NULL);
    SetStatus (jpp->towpilot, FALSE);
  }

  return (DialoG) p;
}

