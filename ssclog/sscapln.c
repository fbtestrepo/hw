/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Airplane flights page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* internal version of AirplaneFlightPtr */

typedef struct intafpinfo {
  CharPtr       name;
  Int4          type;
  CharPtr       alt_payer;
  Int4          split_payment;
  CharPtr       airplane;
  Int4          category;
  CharPtr       instructor;
  CharPtr       passenger;
  ClockTime     takeoff;
  ClockTime     landing;
  Boolean       landed_out;
  Boolean       aloft;
  FlightTime    flight_time;
  DollarAmount  flight_cost;
  CharPtr       comment;
  Boolean       warn_on_name_change;
} IntAirfltInfo, PNTR IntAirfltPtr;

/* dialog controls */

typedef struct sscflights {
  DIALOG_MESSAGE_BLOCK

  DoC           flightlog;
  Int2          currItem;

  PopuP         name;
  GrouP         aftername;
  PopuP         airplane;
  PopuP         fclass;
  PopuP         instructor;
  TexT          passenger;
  ButtoN        special_pay;
  DialoG        takeoff;
  DialoG        landing;
  ButtoN        landout;
  GrouP         special_group;
  PopuP         alt_payer;
  PopuP         split_payment;

  PrompT        ttl_cost;

  DollarAmount  tot_cost;

  Int2          numFlights;
  IntAirfltPtr  airplane_flights [128];

  ValNodePtr    recentName;
  ValNodePtr    latestName;
  ValNodePtr    recentInst;
  ValNodePtr    latestInst;
  ValNodePtr    recentAirplane;
  ValNodePtr    latestAirplane;
  ValNodePtr    recentClass;
  ValNodePtr    latestClass;
  Boolean       changeAfterTakeoff;

  SscTablesPtr  tables;
  BaseFormPtr   bfp;
} SscFlights, PNTR SscFlightsPtr;

#define NUM_FLIGHT_LINES 10

static IntAirfltPtr IntAirfltInfoNew (
  void
)

{
  ClockTimePtr     ctp;
  DollarAmountPtr  dap;
  FlightTimePtr    ftp;
  IntAirfltPtr     ifp;

  ifp = (IntAirfltPtr) MemNew (sizeof (IntAirfltInfo));
  if (ifp == NULL) return NULL;

  ctp = &(ifp->takeoff);
  ctp->hour = -1;
  ctp->minute = -1;

  ctp = &(ifp->landing);
  ctp->hour = -1;
  ctp->minute = -1;

  ftp = &(ifp->flight_time);
  ftp->hours = -1;
  ftp->minutes = -1;

  dap = &(ifp->flight_cost);
  dap->dollars = -1;
  dap->cents = -1;

  return ifp;
}

static IntAirfltPtr IntAirfltInfoFree (
  IntAirfltPtr ifp
)

{
  if (ifp == NULL) return NULL;

  MemFree (ifp->name);
  MemFree (ifp->alt_payer);
  MemFree (ifp->airplane);
  MemFree (ifp->instructor);
  MemFree (ifp->passenger);

  return MemFree (ifp);
}

static IntAirfltPtr GetFlightInfo (
  SscFlightsPtr sfp,
  Int2 item
)

{
  IntAirfltPtr  ifp;

  if (sfp == NULL || item < 0 || item > 127) return NULL;
  ifp = sfp->airplane_flights [item];
  if (ifp != NULL) return ifp;

  ifp = IntAirfltInfoNew ();
  if (ifp != NULL) {
    ifp->name = StringSave (" ");
    ifp->alt_payer = StringSave (" ");
    ifp->airplane = StringSave (" ");
    ifp->instructor = StringSave (" ");
  }
  sfp->airplane_flights [item] = ifp;
  return ifp;
}

static ParData  flightParFmt = { FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0 };

static ColData  flightColFmt [] = {
  {0, 5,  2, 0, NULL, 'r', FALSE, FALSE, FALSE, FALSE, FALSE},  /* number  */
  {0, 5, 25, 3, NULL, 'l', FALSE, TRUE,  FALSE, FALSE, FALSE},  /* name    */
  {0, 5, 12, 2, NULL, 'l', FALSE, TRUE,  FALSE, FALSE, FALSE},  /* airplane  */
  {0, 5,  7, 2, NULL, 'r', FALSE, TRUE,  FALSE, FALSE, FALSE},  /* takeoff */
  {0, 5,  7, 2, NULL, 'r', FALSE, FALSE, FALSE, FALSE, FALSE},  /* landing */
  {0, 5,  7, 2, NULL, 'r', FALSE, FALSE, FALSE, FALSE, FALSE},  /* time    */
  {0, 5,  9, 2, NULL, 'r', FALSE, FALSE, FALSE, FALSE, TRUE}    /* total   */
};

static ENUM_ALIST(fclass_alist)
  {" ",     0},
  {"Norm",  1},
  {"Inst",  2},
  {"Demo",  3},
  {"Exam",  4},
  {"Test",  5},
  {"Free",  6},
END_ENUM_ALIST

static ENUM_ALIST(ratio_alist)
  {" ",       0},
  {"All",     1},
  {"Half",    2},
END_ENUM_ALIST

static void RecalculateTime (
  SscFlightsPtr sfp,
  FlightTimePtr ftp,
  ClockTimePtr lnd,
  Int2 item
)

{
  ClockTimePtr  ctp;
  IntAirfltPtr  ifp;
  Int4          ld_hour, ld_min, to_hour, to_min;

  if (sfp == NULL) return;
  ifp = GetFlightInfo (sfp, item);
  if (ifp == NULL) return;

  to_hour = -1;
  to_min = -1;

  ctp = &(ifp->takeoff);
  if (ctp != NULL) {
    to_hour = ctp->hour;
    to_min = ctp->minute;
  }

  ld_hour = -1;
  ld_min = -1;

  if (lnd != NULL) {
    ld_hour = lnd->hour;
    ld_min = lnd->minute;
  }

  if (ftp != NULL) {
    if (to_hour == -1 || to_min == -1 || ld_hour == -1 || ld_min == -1) {
      ftp->hours = -1;
      ftp->minutes = -1;
    } else if (to_hour > ld_hour) {
      ftp->hours = -1;
      ftp->minutes = -1;
    } else if (to_min > ld_min && to_hour == ld_hour) {
      ftp->hours = -1;
      ftp->minutes = -1;
    } else if (to_min > ld_min) {
      ftp->hours = ld_hour - to_hour - 1;
      ftp->minutes = 60 - to_min + ld_min;
    } else {
      ftp->hours = ld_hour - to_hour;
      ftp->minutes = ld_min - to_min;
    }
  }
}

static Boolean IsSameDay (
  SscTablesPtr stp
)

{
  if (stp == NULL) return FALSE;

  if (stp->workingDate.month != stp->currentDate.month) return FALSE;
  if (stp->workingDate.day != stp->currentDate.day) return FALSE;
  if (stp->workingDate.year != stp->currentDate.year) return FALSE;

  return TRUE;
}

#ifdef WIN_MAC
static Boolean ColorFlightTable (
  DoC d,
  Int2 item,
  Int2 row,
  Int2 col
)

{
  IntAirfltPtr   ifp;
  SscFlightsPtr  sfp;
  SscTablesPtr   stp;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL || item < 1 || item > 127) return FALSE;
  stp = sfp->tables;
  if (stp == NULL) return FALSE;

  ifp = GetFlightInfo (sfp, item);
  if (ifp == NULL) return FALSE;

  if (ifp->aloft && col == 6 && IsSameDay (stp)) {
    Gray ();
    return TRUE;
  }

  return FALSE;
}
#endif

static Boolean GrayFlightTable (
  DoC d,
  Int2 item,
  Int2 row, 
  Int2 col
)

{
  IntAirfltPtr   ifp;
  SscFlightsPtr  sfp;
  SscTablesPtr   stp;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL || item < 1 || item > 127) return FALSE;
  stp = sfp->tables;
  if (stp == NULL) return FALSE;

  ifp = GetFlightInfo (sfp, item);
  if (ifp == NULL) return FALSE;

  if (ifp->aloft && col == 6 && IsSameDay (stp)) return TRUE;

  return FALSE;
}

static CharPtr PrintFlightTable (
  DoC d,
  Int2 item,
  Pointer data
)

{
  Boolean          aloft, landing, takeoff;
  Char             buf [256], tmp [64];
  Char             ch;
  ClockTimePtr     ctp;
  DollarAmountPtr  dap;
  DayTime          dt;
  FlightTime       ft;
  FlightTimePtr    ftp;
  IntAirfltPtr     ifp;
  ClockTime        lnd;
  CharPtr          ptr;
  SscFlightsPtr    sfp;
  SscTablesPtr     stp;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL || item < 1 || item > 127) return NULL;
  stp = sfp->tables;
  if (stp == NULL) return FALSE;
  ifp = GetFlightInfo (sfp, item);
  if (ifp == NULL) return NULL;

  buf [0] = '\0';
  sprintf (tmp, "%d", (int) item);
  StringCat (buf, tmp);

  StringCat (buf, "\t");
  if (StringDoesHaveText (ifp->name)) {
    StringCat (buf, ifp->name);
  }

  StringCat (buf, "\t");
  if (StringDoesHaveText (ifp->airplane)) {
    StringCat (buf, ifp->airplane);
  }

  takeoff = FALSE;
  StringCat (buf, "\t");
  ctp = &(ifp->takeoff);
  if (PrintClockTime (ctp, tmp)) {
    StringCat (buf, tmp);
    takeoff = TRUE;
  }

  landing = FALSE;
  StringCat (buf, "\t");
  ctp = &(ifp->landing);
  if (PrintClockTime (ctp, tmp)) {
    StringCat (buf, tmp);
    landing = TRUE;
  }

  aloft = FALSE;
  StringCat (buf, "\t");
  ftp = &(ifp->flight_time);
  if (PrintFlightTime (ftp, tmp)) {
    StringCat (buf, tmp);
  } else if (takeoff && (! landing) && IsSameDay (stp)) {
    if (GetDayTime (&dt)) {
      if (dt.tm_hour == 0) {
        dt.tm_hour = 24;
      }
      lnd.hour = dt.tm_hour;
      lnd.minute = dt.tm_min;
      RecalculateTime (sfp, &ft, &lnd, item);
      if (PrintFlightTime (&ft, tmp)) {
        TrimSpacesAroundString (tmp);
        ptr = tmp;
        ch = *ptr;
        while (ch == '0' || ch == ':') {
          ptr++;
          ch = *ptr;
        }
        if (StringDoesHaveText (ptr)) {
          StringCat (buf, ptr);
          aloft = TRUE;
        }
      }
    }
  }
  ifp->aloft = aloft;

  StringCat (buf, "\t");
  dap = &(ifp->flight_cost);
  if (PrintDollarAmount (dap, tmp)) {
    StringCat (buf, tmp);
  } else {
    StringCat (buf, " ");
  }

  StringCat (buf, "\n");
  return StringSave (buf);
}

static void ChangeFlightTableSelect (
  DoC d,
  Int2 item,
  Int2 row,
  Int2 col,
  Boolean dblClck
)

{
  IntAirfltPtr   ifp;
  Int2           itemOld1, itemOld2;
  SscFlightsPtr  sfp;
  SscTablesPtr   stp;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;
  if (item == 0 || row == 0 || col == 0) return;

  GetDocHighlight (d, &itemOld1, &itemOld2);
  SetDocHighlight (d, item, item);
  UpdateDocument (d, itemOld1, itemOld2);
  UpdateDocument (d, item, item);
  sfp->currItem = item;

  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp != NULL) {
    SetEnumPopupByName (sfp->name, stp->members.alist, ifp->name);
    SetEnumPopupByName (sfp->airplane, stp->towplane_alist, ifp->airplane);
    SetEnumPopupByName (sfp->instructor, stp->instructors.alist, ifp->instructor);
    SetEnumPopup (sfp->fclass, fclass_alist, (UIEnum) ifp->category);
    SetTitle (sfp->passenger, ifp->passenger);
    SetEnumPopupByName (sfp->alt_payer, stp->payers.alist, ifp->alt_payer);
    SetEnumPopup (sfp->split_payment, ratio_alist, (UIEnum) ifp->split_payment);
    if (StringDoesHaveText (ifp->alt_payer) || ifp->split_payment > 0) {
      SafeShow (sfp->special_group);
      SetStatus (sfp->special_pay, TRUE);
    } else {
      SafeHide (sfp->special_group);
      SetStatus (sfp->special_pay, FALSE);
    }

    SetClockTime (sfp->takeoff, ifp->takeoff.hour, ifp->takeoff.minute);

    SetClockTime (sfp->landing, ifp->landing.hour, ifp->landing.minute);

    SetStatus (sfp->landout, ifp->landed_out);
  }

  Update ();
}

static void RecalculateFlightTotals (
  SscFlightsPtr sfp
)

{
  Char             buf0 [12], buf1 [32];
  Int4             tot_cents, tot_dollars;
  DollarAmountPtr  dap;
  Int2             i;
  IntAirfltPtr     ifp;

  if (sfp == NULL) return;

  tot_cents = 0;
  tot_dollars = 0;
  for (i = 1; i < 128; i++) {
    ifp = sfp->airplane_flights [i];
    if (ifp == NULL) continue;
    dap = &(ifp->flight_cost);
    if (dap == NULL || dap->dollars < 0 || dap->cents < 0) continue;
    tot_dollars += dap->dollars;
    tot_cents += dap->cents;
    if (tot_cents >= 100) {
      tot_dollars += tot_cents / 100;
      tot_cents = tot_cents % 100;
    }
  }

  sprintf (buf0, "%2ld", (long) tot_cents);
  if (buf0 [0] == ' ') {
    buf0 [0] = '0';
  }
  sprintf (buf1, "%ld.%s", (long) tot_dollars, buf0);
  SetTitle (sfp->ttl_cost, buf1);

  dap = &(sfp->tot_cost);
  if (dap != NULL) {
    dap->dollars = tot_dollars;
    dap->cents = tot_cents;
  }

  SetDirtyFlag (sfp->tables);
}

static void CheckExtendTable (
  SscFlightsPtr sfp
)

{
  Int2  numItems;

  if (sfp == NULL) return;

  GetDocParams (sfp->flightlog, &numItems, NULL);
  if (sfp->currItem == numItems) {
    AppendItem (sfp->flightlog, PrintFlightTable, sfp, FALSE, 1,
                &flightParFmt, flightColFmt, systemFont);
  }

  Update ();

  SetDirtyFlag (sfp->tables);
}

static AirplaneInfoPtr GetAirplaneByName (
  AirplaneTablePtr atp,
  CharPtr str
)

{
  AirplaneInfoPtr  aip;

  if (atp == NULL || StringHasNoText (str)) return NULL;

  for (aip = atp; aip != NULL; aip = aip->next) {
    if (StringICmp (str, aip->airplane) == 0) return aip;
  }

  return NULL;
}

static void RecalculateCosts (
  SscFlightsPtr sfp,
  Int2 item
)

{
  Int4             cents, dollars, pennies, minutes, maxminutes,
                   rental_cost, tot_cents, tot_dollars;
  DollarAmountPtr  dap;
  FlightTimePtr    ftp, maxtime;
  AirplaneInfoPtr    gip;
  IntAirfltPtr     ifp;
  MiscRatePtr      mrp;
  SscTablesPtr     stp;
  CharPtr          tmp;
  ValNodePtr       vnp;

  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;
  ifp = GetFlightInfo (sfp, item);
  if (ifp == NULL) return;
  mrp = stp->miscrate_table;
  if (mrp == NULL) return;

  /* test flight or free flight, no charge */

  if (ifp->category == 5 || ifp->category == 6) {
    dap = &(ifp->flight_cost);
    if (dap != NULL) {
      dap->dollars = 0;
      dap->cents = 0;
    }

    RecalculateFlightTotals (sfp);

    return;
  }

  /* normal flight charges */

  dollars = -1;
  cents = -1;

  gip = GetAirplaneByName (stp->airplane_table, ifp->airplane);
  if (gip != NULL) {
    for (vnp = gip->fly_for_free; vnp != NULL; vnp = vnp->next) {
      tmp = (CharPtr) vnp->data.ptrvalue;
      if (StringHasNoText (tmp)) continue;
      if (StringICmp (tmp, ifp->name) == 0) {
        gip = NULL;
        dollars = 0;
        cents = 0;
      }
      if (StringICmp (tmp, ifp->alt_payer) == 0) {
        gip = NULL;
        dollars = 0;
        cents = 0;
      }
    }
  }
  if (gip != NULL) {
    dap = gip->cost_per_min;
    if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
      ftp = &(ifp->flight_time);
      if (ftp != NULL && ftp->hours >= 0 && ftp->minutes >= 0) {
        minutes = 60 * ftp->hours + ftp->minutes;
        maxtime = gip->max_rental_time;
        if (maxtime != NULL && maxtime->hours >= 0 && maxtime->minutes >= 0) {
          maxminutes = 60 * maxtime->hours + maxtime->minutes;
          if (minutes > maxminutes && maxminutes > 0) {
            minutes = maxminutes;
          }
        }
        pennies = 100 * dap->dollars + dap->cents;
        rental_cost = pennies * minutes;
        dollars = rental_cost / 100;
        cents = rental_cost % 100;
      }
    } else {
      dap = gip->cost_per_flight;
      if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
        pennies = 100 * dap->dollars + dap->cents;
        rental_cost = pennies;
        dollars = rental_cost / 100;
        cents = rental_cost % 100;
      }
    }
  }

  dap = &(ifp->flight_cost);
  if (dap != NULL) {
    dap->dollars = dollars;
    dap->cents = cents;
  }

  if (dollars >= 0 && cents >= 0) {
    if (tot_dollars >= 0 && tot_cents >= 0) {
      tot_dollars += dollars;
      tot_cents += cents;
    } else {
      tot_dollars = dollars;
      tot_cents = cents;
    }
  }

  if (tot_cents >= 100) {
    tot_dollars += tot_cents / 100;
    tot_cents = tot_cents % 100;
  }

  dap = &(ifp->flight_cost);
  if (dap != NULL) {
    dap->dollars = tot_dollars;
    dap->cents = tot_cents;
  }

  RecalculateFlightTotals (sfp);
}

static void ImposeTimes (
  SscFlightsPtr sfp,
  Int2 item,
  Boolean lockname
)

{
  ClockTimePtr  ctp;
  IntAirfltPtr  ifp;
  Int4          ld_hour, ld_min, to_hour, to_min;

  if (sfp == NULL) return;
  ifp = GetFlightInfo (sfp, item);
  if (ifp == NULL) return;

  GetClockTime (sfp->takeoff, &to_hour, &to_min);

  ctp = &(ifp->takeoff);
  if (ctp != NULL) {
    ctp->hour = to_hour;
    ctp->minute = to_min;
  }

  GetClockTime (sfp->landing, &ld_hour, &ld_min);

  ctp = &(ifp->landing);
  if (ctp != NULL) {
    ctp->hour = ld_hour;
    ctp->minute = ld_min;
  }

  RecalculateTime (sfp, &(ifp->flight_time), &(ifp->landing), item);

  RecalculateCosts (sfp, item);

  UpdateDocument (sfp->flightlog, item, item);
  Update ();

  CheckExtendTable (sfp);

  if (lockname) {
    ifp->warn_on_name_change = TRUE;
  } else if (StringDoesHaveText (ifp->name)) {
    ifp->warn_on_name_change = TRUE;
  }
}

static void ChangeTakeoff (
  DialoG d
)

{
  BaseDialogPtr  bdp;
  SscFlightsPtr  sfp;

  bdp = (BaseDialogPtr) GetObjectExtra (d);
  if (bdp == NULL) return;
  sfp = (SscFlightsPtr) bdp->userdata;
  if (sfp == NULL) return;

  SetDirtyFlag (sfp->tables);

  ImposeTimes (sfp, sfp->currItem, TRUE);
}

static void ChangeLanding (
  DialoG d
)

{
  BaseDialogPtr  bdp;
  SscFlightsPtr  sfp;

  bdp = (BaseDialogPtr) GetObjectExtra (d);
  if (bdp == NULL) return;
  sfp = (SscFlightsPtr) bdp->userdata;
  if (sfp == NULL) return;

  SetDirtyFlag (sfp->tables);

  ImposeTimes (sfp, sfp->currItem, TRUE);
}

static void SetATime (
  DialoG d
)

{
  ClockTimePtr  ctp;
  DayTime       dt;
  Int4          new_hour, new_min, old_hour, old_min;

  if (! GetDayTime (&dt)) return;
  new_hour = (Int4) dt.tm_hour;
  new_min = (Int4) dt.tm_min;

  old_hour = -1;
  old_min = -1;
  ctp = DialogToPointer (d);
  if (ctp != NULL) {
    old_hour = ctp->hour;
    old_min = ctp->minute;
  }
  MemFree (ctp);

  if (new_hour == old_hour && new_min == old_min) return;
  if (old_hour != -1 || old_min != -1) {
    if (Message (MSG_YN, "Do you want to override the existing time?") == ANS_NO) return;
  }

  ctp = ClockTimeNew ();
  if (ctp != NULL) {
    ctp->hour = new_hour;
    ctp->minute = new_min;
    PointerToDialog (d, (Pointer) ctp);
  }
  MemFree (ctp);
}

static void SetTakeoffTime (
  ButtoN b
)

{
  IntAirfltPtr   ifp;
  SscFlightsPtr  sfp;

  sfp = GetObjectExtra (b);
  if (sfp == NULL) return;

  SetATime (sfp->takeoff);

  ImposeTimes (sfp, sfp->currItem, FALSE);

  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;
  if (StringHasNoText (ifp->name)) {
    sfp->changeAfterTakeoff = TRUE;
  }
}

static void SetLandingTime (
  ButtoN b
)

{
  SscFlightsPtr  sfp;

  sfp = GetObjectExtra (b);
  if (sfp == NULL) return;

  SetATime (sfp->landing);

  ImposeTimes (sfp, sfp->currItem, TRUE);
}

static void SetLandout (
  ButtoN b
)

{
  ClockTime      ct;
  ClockTimePtr   ctp;
  IntAirfltPtr   ifp;
  Int4           old_hour, old_min;
  SscFlightsPtr  sfp;

  sfp = GetObjectExtra (b);
  if (sfp == NULL) return;
  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  ifp->landed_out = GetStatus (sfp->landout);

  if (ifp->landed_out) {
    if (Message (MSG_YN, "Are you sure you want to set the Landed Out status for this flight?") == ANS_NO) {
      SetStatus (sfp->landout, FALSE);
      ifp->landed_out = FALSE;
      return;
    }

    old_hour = -1;
    old_min = -1;
    ctp = DialogToPointer (sfp->landing);
    if (ctp != NULL) {
      old_hour = ctp->hour;
      old_min = ctp->minute;
    }
    MemFree (ctp);

    if (old_hour != -1 || old_min != -1) {
      if (Message (MSG_YN, "Do you want to override the existing landing time for this flight?") == ANS_NO) return;
    }

    SetATime (sfp->landing);

  } else {

    if (Message (MSG_YN, "Do you want to clear the existing landing time for this flight?") == ANS_YES) {
      ct.hour = -1;
      ct.minute = -1;
      PointerToDialog (sfp->landing, (Pointer) &ct);
    }
  }

  ImposeTimes (sfp, sfp->currItem, TRUE);
}

static void TakeoffItemProc (
  IteM i
)

{
  BaseFormPtr    bfp;
  DialoG         d;
  IntAirfltPtr   ifp;
  SscFlightsPtr  sfp;
  SscTablesPtr   stp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  stp = GetTablesPointer (bfp);
  if (stp == NULL) return;

  d = stp->airplane_flights;
  if (! Visible (d)) return;
  if (! AllParentsVisible (d)) return;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;

  SetATime (sfp->takeoff);

  ImposeTimes (sfp, sfp->currItem, FALSE);

  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;
  if (StringHasNoText (ifp->name)) {
    sfp->changeAfterTakeoff = TRUE;
  }
}

static void LandingItemProc (
  IteM i
)

{
  BaseFormPtr    bfp;
  DialoG         d;
  SscFlightsPtr  sfp;
  SscTablesPtr   stp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  stp = GetTablesPointer (bfp);
  if (stp == NULL) return;

  d = stp->airplane_flights;
  if (! Visible (d)) return;
  if (! AllParentsVisible (d)) return;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;

  SetATime (sfp->landing);

  ImposeTimes (sfp, sfp->currItem, TRUE);
}

static Boolean AreThereTooManyPeople (
  SscTablesPtr stp,
  IntAirfltPtr ifp,
  Int4Ptr num_seatsP
)

{
  Int2           count;
  AirplaneInfoPtr  gip;

  if (num_seatsP != NULL) {
    *num_seatsP = 0;
  }

  count = 1;
  gip = GetAirplaneByName (stp->airplane_table, ifp->airplane);
  if (gip != NULL) {
    if (gip->number_of_seats == 0) return FALSE;
    if (StringDoesHaveText (ifp->instructor)) {
      count++;
    }
    if (StringDoesHaveText (ifp->passenger)) {
      count++;
    }
    if (count > gip->number_of_seats) {
      *num_seatsP = gip->number_of_seats;
      return TRUE;
    }
  }

  return FALSE;
}

static void TooManyPeopleWarning (
  Int4 num_seats
)

{
  if (num_seats == 1) {
    Message (MSG_OK, "The airplane can only hold %ld person", (long) num_seats);
  } else {
    Message (MSG_OK, "The airplane can only hold %ld people", (long) num_seats);
  }
}

static void ChangeName (
  PopuP p
)

{
  AlistDialogPtr  adp;
  ClockTimePtr    ctp;
  IntAirfltPtr    ifp;
  SscFlightsPtr   sfp;
  SscTablesPtr    stp;
  CharPtr         str;

  adp = (AlistDialogPtr) GetObjectExtra (p);
  if (adp == NULL) return;
  sfp = (SscFlightsPtr) adp->userdata;
  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;
  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  ctp = &(ifp->takeoff);
  if (ctp != NULL && ctp->hour > 2 && ctp->hour <= 24 &&
      ctp->minute >= 0 && ctp->minute <= 59) {
    if (StringDoesHaveText (ifp->name) && ifp->warn_on_name_change) {
      if (Message (MSG_YN, "Do you want to override the existing name?") == ANS_NO) {
        SetEnumPopupByName (sfp->name, stp->members.alist, ifp->name);
        return;
      }
    }
  }

  str = GetEnumPopupByName (sfp->name, stp->members.alist);
  if (StringCmp (str, "-") == 0) {
    str [0] = ' ';
    SetEnumPopupByName (sfp->name, stp->members.alist, str);
  }
  ifp->name = MemFree (ifp->name);
  ifp->name = str; /* allocated by GetEnumPopupByName */
  ifp->type = GetMemberType (str, stp);

  if (ifp->name == NULL) {
    ifp->name = StringSave (" ");
  }

  RecalculateCosts (sfp, sfp->currItem);

  UpdateDocument (sfp->flightlog, sfp->currItem, sfp->currItem);
  Update ();

  CheckExtendTable (sfp);
}

static void ChangeAirplane (
  PopuP p
)

{
  AlistDialogPtr  adp;
  ClockTimePtr    ctp;
  IntAirfltPtr    ifp;
  Int4            num_seats;
  SscFlightsPtr   sfp;
  SscTablesPtr    stp;
  CharPtr         str;

  adp = (AlistDialogPtr) GetObjectExtra (p);
  if (adp == NULL) return;
  sfp = (SscFlightsPtr) adp->userdata;
  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;
  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  ctp = &(ifp->takeoff);
  if (ctp != NULL && ctp->hour > 2 && ctp->hour <= 24 &&
      ctp->minute >= 0 && ctp->minute <= 59) {
    if (StringDoesHaveText (ifp->airplane)) {
      if (Message (MSG_YN, "Do you want to override the existing airplane?") == ANS_NO) {
        SetEnumPopupByName (sfp->airplane, stp->towplane_alist, ifp->airplane);
        return;
      }
    }
  }

  str = GetEnumPopupByName (sfp->airplane, stp->towplane_alist);
  ifp->airplane = MemFree (ifp->airplane);
  ifp->airplane = str; /* allocated by GetEnumPopupByName */

  if (ifp->airplane == NULL) {
    ifp->airplane = StringSave (" ");
  }

  RecalculateCosts (sfp, sfp->currItem);

  UpdateDocument (sfp->flightlog, sfp->currItem, sfp->currItem);
  Update ();

  CheckExtendTable (sfp);

  if (StringDoesHaveText (ifp->name)) {
    ifp->warn_on_name_change = TRUE;
  }

  if (AreThereTooManyPeople (stp, ifp, &num_seats)) {
    TooManyPeopleWarning (num_seats);
  }
}

static Boolean StringInValNodeList (
  ValNodePtr head,
  CharPtr str
)

{
  CharPtr     ptr;
  ValNodePtr  vnp;

  if (head == NULL || StringHasNoText (str)) return FALSE;
  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    ptr = (CharPtr) vnp->data.ptrvalue;
    if (StringHasNoText (ptr)) continue;
    if (StringCmp (str, ptr) == 0) return TRUE;
  }
  return FALSE;
}

static Boolean IntInValNodeList (
  ValNodePtr head,
  Int4 val
)

{
  ValNodePtr  vnp;

  if (head == NULL || val == 0) return FALSE;
  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    if (vnp->data.intvalue == 0) continue;
    if (vnp->data.intvalue == val) return TRUE;
  }
  return FALSE;
}

static void SameAirplaneProc (
  IteM i
)

{
  BaseFormPtr    bfp;
  ClockTimePtr   ctp;
  DialoG         d;
  IntAirfltPtr   ifp;
  Int2           j;
  Int4           num_seats;
  SscFlightsPtr  sfp;
  SscTablesPtr   stp;
  CharPtr        str;
  ValNodePtr     vnp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  stp = GetTablesPointer (bfp);
  if (stp == NULL) return;

  d = stp->airplane_flights;
  if (! Visible (d)) return;
  if (! AllParentsVisible (d)) return;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;

  if (sfp->currItem <= 1) {
    Beep ();
    return;
  }

  if (sfp->recentAirplane == NULL) {
    str = NULL;
    for (j = sfp->currItem - 1; j > 0; j--) {
      ifp = GetFlightInfo (sfp, j);
      if (ifp == NULL) continue;
      str = ifp->airplane;
      if (StringHasNoText (str)) continue;
      if (StringCmp (str, "-") == 0) continue;
      if (StringInValNodeList (sfp->recentAirplane, str)) continue;
      ValNodeCopyStr (&(sfp->recentAirplane), 0, str);
    }
    sfp->latestAirplane = sfp->recentAirplane;
  }

  str = NULL;
  if (sfp->latestAirplane != NULL) {
    vnp = sfp->latestAirplane;
    str = (CharPtr) vnp->data.ptrvalue;
    sfp->latestAirplane = vnp->next;
  } else if (sfp->recentAirplane != NULL) {
    sfp->latestAirplane = sfp->recentAirplane;
  } else {
    return;
  }

  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  ctp = &(ifp->takeoff);
  if (ctp != NULL && ctp->hour > 2 && ctp->hour <= 24 &&
      ctp->minute >= 0 && ctp->minute <= 59 && (! sfp->changeAfterTakeoff)) {
    if (StringDoesHaveText (ifp->airplane) && StringICmp (ifp->airplane, str) != 0) {
      if (Message (MSG_YN, "Do you want to override the existing airplane?") == ANS_NO) return;
    }
  }

  ifp->airplane = MemFree (ifp->airplane);
  ifp->airplane = StringSave (str);

  if (ifp->airplane == NULL) {
    ifp->airplane = StringSave (" ");
  }

  SetEnumPopupByName (sfp->airplane, stp->towplane_alist, ifp->airplane);

  RecalculateCosts (sfp, sfp->currItem);

  UpdateDocument (sfp->flightlog, sfp->currItem, sfp->currItem);
  Update ();

  CheckExtendTable (sfp);

  if (StringDoesHaveText (ifp->name)) {
    ifp->warn_on_name_change = TRUE;
  }

  if (AreThereTooManyPeople (stp, ifp, &num_seats)) {
    /*
    TooManyPeopleWarning (num_seats);
    */
  }
}

static void SameNameProc (
  IteM i
)

{
  BaseFormPtr    bfp;
  ClockTimePtr   ctp;
  DialoG         d;
  IntAirfltPtr   ifp;
  Int2           j;
  SscFlightsPtr  sfp;
  SscTablesPtr   stp;
  CharPtr        str;
  ValNodePtr     vnp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  stp = GetTablesPointer (bfp);
  if (stp == NULL) return;

  d = stp->airplane_flights;
  if (! Visible (d)) return;
  if (! AllParentsVisible (d)) return;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;

  if (sfp->currItem <= 1) {
    Beep ();
    return;
  }

  if (sfp->recentName == NULL) {
    str = NULL;
    for (j = sfp->currItem - 1; j > 0; j--) {
      ifp = GetFlightInfo (sfp, j);
      if (ifp == NULL) continue;
      str = ifp->name;
      if (StringHasNoText (str)) continue;
      if (StringCmp (str, "-") == 0) continue;
      if (StringInValNodeList (sfp->recentName, str)) continue;
      ValNodeCopyStr (&(sfp->recentName), 0, str);
    }
    sfp->latestName = sfp->recentName;
  }

  str = NULL;
  if (sfp->latestName != NULL) {
    vnp = sfp->latestName;
    str = (CharPtr) vnp->data.ptrvalue;
    sfp->latestName = vnp->next;
  } else if (sfp->recentName != NULL) {
    sfp->latestName = sfp->recentName;
  } else {
    return;
  }

  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  ctp = &(ifp->takeoff);
  if (ctp != NULL && ctp->hour > 2 && ctp->hour <= 24 &&
      ctp->minute >= 0 && ctp->minute <= 59 && (! sfp->changeAfterTakeoff)) {
    if (StringDoesHaveText (ifp->name) && StringICmp (ifp->name, str) != 0) {
      if (Message (MSG_YN, "Do you want to override the existing name?") == ANS_NO) {
        return;
      }
    }
  }

  ifp->name = MemFree (ifp->name);
  ifp->name = StringSave (str);

  if (ifp->name == NULL) {
    ifp->name = StringSave (" ");
  }

  SetEnumPopupByName (sfp->name, stp->members.alist, ifp->name);

  RecalculateCosts (sfp, sfp->currItem);

  UpdateDocument (sfp->flightlog, sfp->currItem, sfp->currItem);
  Update ();

  CheckExtendTable (sfp);
}

static void SameInstructorProc (
  IteM i
)

{
  BaseFormPtr    bfp;
  ClockTimePtr   ctp;
  DialoG         d;
  IntAirfltPtr   ifp;
  Int2           j;
  Int4           num_seats;
  SscFlightsPtr  sfp;
  SscTablesPtr   stp;
  CharPtr        str;
  ValNodePtr     vnp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  stp = GetTablesPointer (bfp);
  if (stp == NULL) return;

  d = stp->airplane_flights;
  if (! Visible (d)) return;
  if (! AllParentsVisible (d)) return;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;

  if (sfp->currItem <= 1) {
    Beep ();
    return;
  }

  if (sfp->recentInst == NULL) {
    str = NULL;
    for (j = sfp->currItem - 1; j > 0; j--) {
      ifp = GetFlightInfo (sfp, j);
      if (ifp == NULL) continue;
      str = ifp->instructor;
      if (StringHasNoText (str)) continue;
      if (StringCmp (str, "-") == 0) continue;
      if (StringInValNodeList (sfp->recentInst, str)) continue;
      ValNodeCopyStr (&(sfp->recentInst), 0, str);
    }
    sfp->latestInst = sfp->recentInst;
  }

  str = NULL;
  if (sfp->latestInst != NULL) {
    vnp = sfp->latestInst;
    str = (CharPtr) vnp->data.ptrvalue;
    sfp->latestInst = vnp->next;
  } else if (sfp->recentInst != NULL) {
    sfp->latestInst = sfp->recentInst;
  } else {
    return;
  }

  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  ctp = &(ifp->takeoff);
  if (ctp != NULL && ctp->hour > 2 && ctp->hour <= 24 &&
      ctp->minute >= 0 && ctp->minute <= 59 && (! sfp->changeAfterTakeoff)) {
    if (StringDoesHaveText (ifp->instructor) && StringICmp (ifp->instructor, str) != 0) {
      if (Message (MSG_YN, "Do you want to override the existing instructor?") == ANS_NO) {
        if (AreThereTooManyPeople (stp, ifp, &num_seats)) {
          TooManyPeopleWarning (num_seats);
        }
        return;
      }
    }
  }

  ifp->instructor = MemFree (ifp->instructor);
  ifp->instructor = StringSave (str);

  if (ifp->instructor == NULL) {
    ifp->instructor = StringSave (" ");
  }

  SetEnumPopupByName (sfp->instructor, stp->instructors.alist, ifp->instructor);

  Update ();

  CheckExtendTable (sfp);

  if (StringDoesHaveText (ifp->name)) {
    ifp->warn_on_name_change = TRUE;
  }

  if (AreThereTooManyPeople (stp, ifp, &num_seats)) {
    TooManyPeopleWarning (num_seats);
  }
}

static void SameClassProc (
  IteM i
)

{
  BaseFormPtr    bfp;
  ClockTimePtr   ctp;
  DialoG         d;
  IntAirfltPtr   ifp;
  Int2           j;
  SscFlightsPtr  sfp;
  SscTablesPtr   stp;
  Int4           val;
  ValNodePtr     vnp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  stp = GetTablesPointer (bfp);
  if (stp == NULL) return;

  d = stp->airplane_flights;
  if (! Visible (d)) return;
  if (! AllParentsVisible (d)) return;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;

  if (sfp->currItem <= 1) {
    Beep ();
    return;
  }

  if (sfp->recentClass == NULL) {
    for (j = sfp->currItem - 1; j > 0; j--) {
      ifp = GetFlightInfo (sfp, j);
      if (ifp == NULL) continue;
      val = ifp->category;
      if (val == 0) continue;
      if (IntInValNodeList (sfp->recentClass, val)) continue;
      ValNodeAddInt (&(sfp->recentClass), 0, val);
    }
    sfp->latestClass = sfp->recentClass;
  }

  val = 0;
  if (sfp->latestClass != NULL) {
    vnp = sfp->latestClass;
    val = (Int4) vnp->data.intvalue;
    sfp->latestClass = vnp->next;
  } else if (sfp->recentClass != NULL) {
    sfp->latestClass = sfp->recentClass;
  } else {
    return;
  }

  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  ctp = &(ifp->takeoff);
  if (ctp != NULL && ctp->hour > 2 && ctp->hour <= 24 &&
      ctp->minute >= 0 && ctp->minute <= 59 && (! sfp->changeAfterTakeoff)) {
    if (ifp->category > 0 && ifp->category == val) {
      if (Message (MSG_YN, "Do you want to override the existing class?") == ANS_NO) {
        return;
      }
    }
  }

  ifp->category = val;

  SetEnumPopup (sfp->fclass, fclass_alist, (UIEnum) ifp->category);

  RecalculateCosts (sfp, sfp->currItem);

  UpdateDocument (sfp->flightlog, sfp->currItem, sfp->currItem);
  Update ();

  CheckExtendTable (sfp);

  if (StringDoesHaveText (ifp->name)) {
    ifp->warn_on_name_change = TRUE;
  }
}

static void ChangeFclass (
  PopuP p
)

{
  AlistDialogPtr  adp;
  IntAirfltPtr    ifp;
  SscFlightsPtr   sfp;
  UIEnum          val;

  adp = (AlistDialogPtr) GetObjectExtra (p);
  if (adp == NULL) return;
  sfp = (SscFlightsPtr) adp->userdata;
  if (sfp == NULL) return;
  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  if (GetEnumPopup (sfp->fclass, fclass_alist, &val)) {
    ifp->category = (Int4) val;
  }

  RecalculateCosts (sfp, sfp->currItem);

  UpdateDocument (sfp->flightlog, sfp->currItem, sfp->currItem);
  Update ();

  CheckExtendTable (sfp);

  if (StringDoesHaveText (ifp->name)) {
    ifp->warn_on_name_change = TRUE;
  }
}

static void ChangePassenger (
  TexT t
)

{
  IntAirfltPtr   ifp;
  Int4           num_seats;
  SscFlightsPtr  sfp;
  SscTablesPtr   stp;

  sfp = (SscFlightsPtr) GetObjectExtra (t);
  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;
  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  ifp->passenger = MemFree (ifp->passenger);
  ifp->passenger = SaveStringFromText (t);

  CheckExtendTable (sfp);

  if (StringDoesHaveText (ifp->name)) {
    ifp->warn_on_name_change = TRUE;
  }

  if (AreThereTooManyPeople (stp, ifp, &num_seats)) {
    TooManyPeopleWarning (num_seats);
  }
}

static void ChangeSpecialPay (
  ButtoN b
)

{
  IntAirfltPtr   ifp;
  SscFlightsPtr  sfp;
  SscTablesPtr   stp;

  sfp = (SscFlightsPtr) GetObjectExtra (b);
  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;
  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  if (GetStatus (b)) {
    SafeShow (sfp->special_group);
  } else {
    if (StringDoesHaveText (ifp->alt_payer) || ifp->split_payment > 0) {
      if (Message (MSG_YN, "Do you want to clear the existing special payment information?") == ANS_NO) {
        SetStatus (b, TRUE);
        return;
      }
    }
    SafeHide (sfp->special_group);
    ifp->alt_payer = MemFree (ifp->alt_payer);
    ifp->alt_payer = StringSave (" ");
    ifp->split_payment = 0;
    SetEnumPopupByName (sfp->alt_payer, stp->payers.alist, " ");
    SetValue (sfp->split_payment, 0);
  }
}

static void ChangePayer (
  PopuP p
)

{
  AlistDialogPtr   adp;
  Int4             cents = -1, dollars = -1;
  Boolean          clear_alt_payer = FALSE, update = FALSE;
  DollarAmountPtr  dap;
  IntAirfltPtr     ifp;
  SscFlightsPtr    sfp;
  SscTablesPtr     stp;
  CharPtr          str;

  adp = (AlistDialogPtr) GetObjectExtra (p);
  if (adp == NULL) return;
  sfp = (SscFlightsPtr) adp->userdata;
  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;
  ifp = GetFlightInfo (sfp, sfp->currItem);
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

  if (StringCmp (ifp->name, ifp->alt_payer) == 0) {
    if (Message (MSG_YN,
                 "Alternative payer is same as pilot.  Do you want to clear special payment information?") == ANS_YES) {
      clear_alt_payer = TRUE;
    }
  } else if (ifp->category == 5 || ifp->category == 6) {
    if (Message (MSG_YN,
                 "Free flights do not need alternative payer.  Do you want to clear special payment information?") == ANS_YES) {
      clear_alt_payer = TRUE;
    }
  }
  if (clear_alt_payer) {
    SafeSetStatus (sfp->special_pay, FALSE);
    SafeHide (sfp->special_group);
    ifp->alt_payer = MemFree (ifp->alt_payer);
    ifp->alt_payer = StringSave (" ");
    ifp->split_payment = 0;
    SetEnumPopupByName (sfp->alt_payer, stp->payers.alist, " ");
    SetValue (sfp->split_payment, 0);
  }

  dap = &(ifp->flight_cost);
  if (dap != NULL) {
    dollars = dap->dollars;
    cents = dap->cents;
  }

  RecalculateCosts (sfp, sfp->currItem);

  dap = &(ifp->flight_cost);
  if (dap == NULL) {
    if (dollars != -1 || cents != -1) {
      update = TRUE;
    }
  } else if (dap->dollars != dollars || dap->cents != cents) {
    update = TRUE;
  }
  if (update) {
    UpdateDocument (sfp->flightlog, sfp->currItem, sfp->currItem);
    Update ();
  }

  CheckExtendTable (sfp);
}

static void ChangeSplit (
  PopuP p
)

{
  IntAirfltPtr   ifp;
  SscFlightsPtr  sfp;
  UIEnum         val;

  sfp = (SscFlightsPtr) GetObjectExtra (p);
  if (sfp == NULL) return;
  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  if (GetEnumPopup (sfp->split_payment, ratio_alist, &val)) {
    ifp->split_payment = (Int4) val;
  }

  RecalculateCosts (sfp, sfp->currItem);

  CheckExtendTable (sfp);
}

static void ChangeInstructor (
  PopuP p
)

{
  AlistDialogPtr  adp;
  ClockTimePtr    ctp;
  IntAirfltPtr    ifp;
  Int4            num_seats;
  SscFlightsPtr   sfp;
  SscTablesPtr    stp;
  CharPtr         str;

  adp = (AlistDialogPtr) GetObjectExtra (p);
  if (adp == NULL) return;
  sfp = (SscFlightsPtr) adp->userdata;
  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;
  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  ctp = &(ifp->takeoff);
  if (ctp != NULL && ctp->hour > 2 && ctp->hour <= 24 &&
      ctp->minute >= 0 && ctp->minute <= 59) {
    if (StringDoesHaveText (ifp->instructor)) {
      if (Message (MSG_YN, "Do you want to override the existing instructor?") == ANS_NO) {
        SetEnumPopupByName (sfp->instructor, stp->instructors.alist, ifp->instructor);
        return;
      }
    }
  }

  str = GetEnumPopupByName (sfp->instructor, stp->instructors.alist);
  ifp->instructor = MemFree (ifp->instructor);
  ifp->instructor = str; /* allocated by GetEnumPopupByName */

  if (ifp->instructor == NULL) {
    ifp->instructor = StringSave (" ");
  }

  CheckExtendTable (sfp);

  if (StringDoesHaveText (ifp->name)) {
    ifp->warn_on_name_change = TRUE;
  }

  if (AreThereTooManyPeople (stp, ifp, &num_seats)) {
    TooManyPeopleWarning (num_seats);
  }
}

static Int2 expFltPosns [] = {0, 22, 30, 35, 48, 71, 78, 101};
static Char expFltJusts [] = "llrllrlr";

extern void ExportAirplaneDialog (
  DialoG d,
  ValNodePtr PNTR head
)

{
  Char            buf [128], tmp [32];
  DoC             doc;
  IntAirfltPtr    ifp;
  Int2            k, numItems;
  SscFlightsPtr   sfp;
  SscTablesPtr    stp;
  TowPlaneSetPtr  tsp;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL || head == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;

  doc = sfp->flightlog;
  GetDocParams (doc, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    ifp = sfp->airplane_flights [k];
    if (ifp == NULL) continue;
    if (StringHasNoText (ifp->name)) continue;
    if (StringHasNoText (ifp->airplane)) continue;
    buf [0] = '\0';
    ExportDocCell (doc, k, 2, buf);
    if (ifp->type == 2) {
      FormatCell ("*", buf, 1, expFltPosns, expFltJusts);
    }
    /*
    sprintf (tmp, "%4ld", (long) ifp->release_altitude);
    FormatCell (tmp, buf, 2, expFltPosns, expFltJusts);
    ExportDocCell (doc, k, 3, tmp);
    */
    FormatCell (tmp, buf, 3, expFltPosns, expFltJusts);
    StringCpy (tmp, ifp->airplane);
    if (StringHasNoText (tmp)) {
      tsp = stp->towset_table;
      if (tsp != NULL) {
        StringCpy (tmp, tsp->towplane);
      }
    }
    FormatCell (tmp, buf, 4, expFltPosns, expFltJusts);
    ExportDocCell (doc, k, 6, tmp);
    FormatCell (tmp, buf, 5, expFltPosns, expFltJusts);
    ValNodeCopyStr (head, k, buf);
  }
}

static Int2 acvFltPosns [] = {0, 5, 25, 38, 60, 67, 74, 83, 92, 101, 110};
static Char acvFltJusts [] = "llllrrrrrrr";

extern void ArchiveAirplaneDialog (
  DialoG d,
  FILE *fp
)

{
  Char            buf [128], tmp [32];
  IntAirfltPtr    ifp;
  Int2            k, numItems;
  SscFlightsPtr   sfp;
  SscTablesPtr    stp;
  TowPlaneSetPtr  tsp;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL || fp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;

  fprintf (fp, "FLIGHT OPERATIONS -----\n\n");
  fprintf (fp, "Flt  Name                Airplane       TowPlane         Tkoff");
  fprintf (fp, "   Land   Time     Alt    Rental     Tow     Total\n\n");

  GetDocParams (sfp->flightlog, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    ifp = sfp->airplane_flights [k];
    if (ifp == NULL) continue;
    if (StringHasNoText (ifp->name)) continue;
    if (StringHasNoText (ifp->airplane)) continue;
    buf [0] = '\0';
    ExportDocCell (sfp->flightlog, k, 1, tmp);
    StringCat (buf, tmp);
    ExportDocCell (sfp->flightlog, k, 2, tmp);
    FormatCell (tmp, buf, 1, acvFltPosns, acvFltJusts);
    ExportDocCell (sfp->flightlog, k, 3, tmp);
    FormatCell (tmp, buf, 2, acvFltPosns, acvFltJusts);
    StringCpy (tmp, ifp->airplane);
    if (StringHasNoText (tmp)) {
      tsp = stp->towset_table;
      if (tsp != NULL) {
        StringCpy (tmp, tsp->towplane);
      }
    }
    FormatCell (tmp, buf, 3, acvFltPosns, acvFltJusts);
    ExportDocCell (sfp->flightlog, k, 4, tmp);
    FormatCell (tmp, buf, 4, acvFltPosns, acvFltJusts);
    ExportDocCell (sfp->flightlog, k, 5, tmp);
    FormatCell (tmp, buf, 5, acvFltPosns, acvFltJusts);
    ExportDocCell (sfp->flightlog, k, 6, tmp);
    FormatCell (tmp, buf, 6, acvFltPosns, acvFltJusts);
    ExportDocCell (sfp->flightlog, k, 7, tmp);
    FormatCell (tmp, buf, 7, acvFltPosns, acvFltJusts);
    ExportDocCell (sfp->flightlog, k, 8, tmp);
    FormatCell (tmp, buf, 8, acvFltPosns, acvFltJusts);
    ExportDocCell (sfp->flightlog, k, 9, tmp);
    FormatCell (tmp, buf, 9, acvFltPosns, acvFltJusts);
    ExportDocCell (sfp->flightlog, k, 10, tmp);
    FormatCell (tmp, buf, 10, acvFltPosns, acvFltJusts);
    StringCat (buf, "\n");
    fprintf (fp, "%s", buf);
  }
  fprintf (fp, "--------------------------------------------------------");
  fprintf (fp, "------------------------------------------------------\n");
  buf [0] = '\0';
  /* StringCpy (buf, "     Totals:"); */
  /*
  if (PrintDollarAmount (&(sfp->tot_rent), tmp)) {
    FormatCell (tmp, buf, 8, acvFltPosns, acvFltJusts);
  }
  if (PrintDollarAmount (&(sfp->tot_tow), tmp)) {
    FormatCell (tmp, buf, 9, acvFltPosns, acvFltJusts);
  }
  */
  if (PrintDollarAmount (&(sfp->tot_cost), tmp)) {
    FormatCell (tmp, buf, 10, acvFltPosns, acvFltJusts);
  }
  fprintf (fp, "%s\n\n", buf);
}

static void FlightsDialogNotify (
  DoC d,
  Int2 item,
  Int2 row,
  Int2 col,
  Boolean dblClck
)

{
  SscFlightsPtr  sfp;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;

  ResetClip ();

  sfp->recentName = ValNodeFreeData (sfp->recentName);
  sfp->recentInst = ValNodeFreeData (sfp->recentInst);
  sfp->recentAirplane = ValNodeFreeData (sfp->recentAirplane);
  sfp->recentClass = ValNodeFree (sfp->recentClass);
  sfp->latestName = NULL;
  sfp->latestInst = NULL;
  sfp->latestAirplane = NULL;
  sfp->latestClass = NULL;
  sfp->changeAfterTakeoff = FALSE;

  ChangeFlightTableSelect (d, item, row, col, dblClick);
}

extern void KeyToAirplaneDialog (
  DialoG d,
  Char key
)

{
  Int2           numItems, offset;
  BaR            sb;
  SscFlightsPtr  sfp;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;

  if (key == NLM_UP) {
    if (sfp->currItem > 1) {
      (sfp->currItem)--;
    }
  } else if (key == NLM_DOWN) {
    GetDocParams (sfp->flightlog, &numItems, NULL);
    if (sfp->currItem < numItems) {
      (sfp->currItem)++;
    }
  }

  if (key == NLM_UP || key == NLM_DOWN) {
    sfp->recentName = ValNodeFreeData (sfp->recentName);
    sfp->recentInst = ValNodeFreeData (sfp->recentInst);
    sfp->recentAirplane = ValNodeFreeData (sfp->recentAirplane);
    sfp->recentClass = ValNodeFree (sfp->recentClass);
    sfp->latestName = NULL;
    sfp->latestInst = NULL;
    sfp->latestAirplane = NULL;
    sfp->latestClass = NULL;
    sfp->changeAfterTakeoff = FALSE;
  }

  sb = GetSlateVScrollBar ((SlatE) sfp->flightlog);
  offset = GetValue (sb);

  if (key == NLM_DOWN && offset < sfp->currItem - NUM_FLIGHT_LINES && sfp->currItem > 0) {
    SetValue (sb, sfp->currItem - NUM_FLIGHT_LINES);
  }
  if (key == NLM_UP && sfp->currItem <= offset) {
    SetValue (sb, sfp->currItem - 1);
  }

  ChangeFlightTableSelect (sfp->flightlog, sfp->currItem, 1, 1, FALSE);

  Update ();
}

extern void AirplaneControlsTimerProc (
  DialoG d
)

{
  ClockTimePtr   ctp;
  FlightTimePtr  ftp;
  IntAirfltPtr   ifp;
  Int2           k, numItems;
  Boolean        landing, takeoff;
  SscFlightsPtr  sfp;
  SscTablesPtr   stp;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;

  GetDocParams (sfp->flightlog, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    ifp = sfp->airplane_flights [k];
    if (ifp == NULL) continue;
    /*
    if (StringHasNoText (ifp->name)) continue;
    if (StringHasNoText (ifp->airplane)) continue;
    */

    takeoff = FALSE;
    ctp = &(ifp->takeoff);
    if (ctp != NULL && ctp->hour > 2 && ctp->hour <= 24 &&
        ctp->minute >= 0 && ctp->minute <= 59) {
      takeoff = TRUE;
    }

    landing = FALSE;
    ctp = &(ifp->landing);
    if (ctp != NULL && ctp->hour > 2 && ctp->hour <= 24 &&
        ctp->minute >= 0 && ctp->minute <= 59) {
      landing = TRUE;
    }

    ftp = &(ifp->flight_time);
    if (ftp != NULL && ftp->hours >= 0 && ftp->minutes >= 0) {
    } else if (takeoff && (! landing) && IsSameDay (stp)) {
      UpdateDocument (sfp->flightlog, k, k);
    }
  }
  Update ();
}

static void RecalculateAllFlightCosts (
  IteM i
)

{
  BaseFormPtr    bfp;
  Char           buf1 [32], buf2 [16];
  DialoG         d;
  Int2           k;
  SscFlightsPtr  sfp;
  SscTablesPtr   stp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  stp = GetTablesPointer (bfp);
  if (stp == NULL) return;

  d = stp->airplane_flights;
  if (! Visible (d)) return;
  if (! AllParentsVisible (d)) return;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;

  Hide (sfp->flightlog);
  Update ();

  for (k = 1; k <= sfp->numFlights; k++) {
    RecalculateCosts (sfp, k);
  }

  UpdateDocument (sfp->flightlog, 0, 0);
  Show (sfp->flightlog);

  UpdateFlightControls (d);

  sprintf (buf2, "%2ld", (long) sfp->tot_cost.cents);
  if (buf2 [0] == ' ') {
    buf2 [0] = '0';
  }
  sprintf (buf1, "%ld.%s", (long) sfp->tot_cost.dollars, buf2);
  SetTitle (sfp->ttl_cost, buf1);

  Update ();
}

static void FlightInfoPtrToFlightPage (
  DialoG d,
  Pointer data
)

{
  Char             buf1 [32], buf2 [16];
  ClockTimePtr     ctp;
  DollarAmountPtr  dap;
  AirplaneFlightPtr  fip, flights;
  FlightTimePtr    ftp;
  Int2             i;
  IntAirfltPtr     ifp;
  Int2             k;
  SscFlightsPtr    sfp;
  SscTablesPtr     stp;
  DollarAmount     tot_rent, tot_cost;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;
  flights = (AirplaneFlightPtr) data;

  sfp->recentName = ValNodeFreeData (sfp->recentName);
  sfp->recentInst = ValNodeFreeData (sfp->recentInst);
  sfp->recentAirplane = ValNodeFreeData (sfp->recentAirplane);
  sfp->recentClass = ValNodeFree (sfp->recentClass);
  sfp->latestName = NULL;
  sfp->latestInst = NULL;
  sfp->latestAirplane = NULL;
  sfp->latestClass = NULL;
  sfp->changeAfterTakeoff = FALSE;

  Reset (sfp->flightlog);
  sfp->numFlights = 0;
  sfp->currItem = 1;

  for (k = 0; k < 128; k++) {
    ifp = sfp->airplane_flights [k];
    IntAirfltInfoFree (ifp);
    sfp->airplane_flights [k] = NULL;
  }

  if (flights == NULL) {
    Reset (sfp->flightlog);
    SetValue (sfp->name, 0);
    SetValue (sfp->airplane, 0);
    SetTitle (sfp->passenger, "");
    SetValue (sfp->alt_payer, 0);
    SetValue (sfp->split_payment, 0);
    SetValue (sfp->fclass, 0);
    SetValue (sfp->instructor, 0);
    SetClockTime (sfp->takeoff, -1, -1);
    SetClockTime (sfp->landing, -1, -1);
    SetStatus (sfp->landout, FALSE);
    SetTitle (sfp->ttl_cost, "");
    sfp->tot_cost.dollars = 0;
    sfp->tot_cost.cents = 0;
    sfp->numFlights = 0;
    sfp->currItem = 1;
    for (i = 0; i < NUM_FLIGHT_LINES; i++) {
      AppendItem (sfp->flightlog, PrintFlightTable, sfp, FALSE, 1,
                  &flightParFmt, flightColFmt, systemFont);
    }
    SetDocHighlight (sfp->flightlog, 1, 1);
    return;
  }

  tot_cost.dollars = 0;
  tot_cost.cents = 0;

  for (fip = flights, k = 1; fip != NULL; fip = fip->next, k++) {
    ifp = GetFlightInfo (sfp, k);
    if (ifp == NULL) continue;

    ifp->name = StringSaveForAlist (fip->name);
    AddNameToExtras (&(stp->members), ifp->name);
    ifp->alt_payer = StringSaveForAlist (fip->alt_payer);
    AddNameToExtras (&(stp->payers), ifp->alt_payer);
    ifp->airplane = StringSaveForAlist (fip->airplane);
    ifp->instructor = StringSaveForAlist (fip->instructor);
    AddNameToExtras (&(stp->instructors), ifp->instructor);
    AddNameToExtras (&(stp->justcfigs), ifp->instructor);
    ifp->passenger = StringSaveForAlist (fip->passenger);

    ifp->type = fip->type;
    ifp->split_payment = fip->split_payment;
    ifp->category = fip->category;

    ctp = fip->takeoff;
    if (ctp != NULL) {
      ifp->takeoff.hour = ctp->hour;
      ifp->takeoff.minute = ctp->minute;
    }
    ctp = fip->landing;
    if (ctp != NULL) {
      ifp->landing.hour = ctp->hour;
      ifp->landing.minute = ctp->minute;
    }
    ftp = fip->flight_time;
    if (ftp != NULL) {
      ifp->flight_time.hours = ftp->hours;
      ifp->flight_time.minutes = ftp->minutes;
    }
    /*
    ifp->landed_out = fip->landed_out;
    */
    ifp->aloft = FALSE;

    dap = fip->flight_cost;
    if (dap != NULL) {
      ifp->flight_cost.dollars = dap->dollars;
      ifp->flight_cost.cents = dap->cents;

      tot_rent.dollars += dap->dollars;
      tot_rent.cents += dap->cents;
      if (tot_rent.cents >= 100) {
        tot_rent.dollars += tot_rent.cents / 100;
        tot_rent.cents = tot_rent.cents % 100;
      }
    }

    AppendItem (sfp->flightlog, PrintFlightTable, sfp, FALSE, 1,
                &flightParFmt, flightColFmt, systemFont);

    (sfp->numFlights)++;
  }

  AppendItem (sfp->flightlog, PrintFlightTable, sfp, FALSE, 1,
              &flightParFmt, flightColFmt, systemFont);
  k++;

  while (k <= NUM_FLIGHT_LINES) {
    AppendItem (sfp->flightlog, PrintFlightTable, sfp, FALSE, 1,
                &flightParFmt, flightColFmt, systemFont);
    k++;
  }

  /*
  for (k = 1; k <= sfp->numFlights; k++) {
    RecalculateCosts (sfp, k);
  }
  */

  UpdateDocument (sfp->flightlog, 0, 0);

  UpdateFlightControls (d);

  if (tot_cost.cents < 0) {
    tot_cost.cents = 0;
  }
  sprintf (buf2, "%2ld", (long) tot_cost.cents);
  if (buf2 [0] == ' ') {
    buf2 [0] = '0';
  }
  sprintf (buf1, "%ld.%s", (long) tot_cost.dollars, buf2);
  SetTitle (sfp->ttl_cost, buf1);
  sfp->tot_cost.dollars = tot_cost.dollars;
  sfp->tot_cost.cents = tot_cost.cents;

  ChangeFlightTableSelect (sfp->flightlog, 1, 1, 1, FALSE);

  Update ();
}

static Boolean ClockTimeOkay (
  ClockTimePtr ctp
)

{
  if (ctp == NULL) return FALSE;

  if (ctp->hour < 0 || ctp->minute < 0) return FALSE;

  return TRUE;
}

static Boolean FlightTimeOkay (
  FlightTimePtr ftp
)

{
  if (ftp == NULL) return FALSE;

  if (ftp->hours < 0 || ftp->minutes < 0) return FALSE;
  if (ftp->hours == 0 && ftp->minutes == 0) return FALSE;

  return TRUE;
}

static Boolean DollarAmountOkay (
  DollarAmountPtr dap
)

{
  if (dap == NULL) return FALSE;

  if (dap->dollars < 0 || dap->cents < 0) return FALSE;

  return TRUE;
}

static Pointer FlightPageToFlightInfoPtr (
  DialoG d
)

{
  ClockTimePtr     ctp;
  DollarAmountPtr  dap;
  FlightTimePtr    ftp;
  IntAirfltPtr     ifp;
  AirplaneFlightPtr  fip, flights, lastfip;
  Int2             k, numItems;
  SscFlightsPtr    sfp;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return NULL;

  flights = NULL;

  lastfip = NULL;
  GetDocParams (sfp->flightlog, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    ifp = sfp->airplane_flights [k];
    if (ifp == NULL) continue;
    if (StringHasNoText (ifp->name)) continue;
    if (StringCmp (ifp->name, "-") == 0) continue;
    if (StringHasNoText (ifp->airplane)) continue;
    fip = AirplaneFlightNew ();
    if (fip == NULL) continue;

    fip->name = StringSaveNotEmpty (ifp->name);
    fip->alt_payer = StringSaveNotEmpty (ifp->alt_payer);
    fip->airplane = StringSaveNotEmpty (ifp->airplane);
    fip->instructor = StringSaveNotEmpty (ifp->instructor);
    if (StringCmp (fip->instructor, "-") == 0) {
      fip->instructor = MemFree (fip->instructor);
    }
    fip->passenger = StringSaveNotEmpty (ifp->passenger);

    fip->type = ifp->type;
    fip->split_payment = ifp->split_payment;
    fip->category = ifp->category;

    if (ClockTimeOkay (&(ifp->takeoff))) {
      ctp = ClockTimeNew ();
      if (ctp != NULL) {
        ctp->hour = ifp->takeoff.hour;
        ctp->minute = ifp->takeoff.minute;
        fip->takeoff = ctp;
      }
    }
    if (ClockTimeOkay (&(ifp->landing))) {
      ctp = ClockTimeNew ();
      if (ctp != NULL) {
        ctp->hour = ifp->landing.hour;
        ctp->minute = ifp->landing.minute;
        fip->landing = ctp;
      }
    }
    if (FlightTimeOkay (&(ifp->flight_time))) {
      ftp = FlightTimeNew ();
      if (ftp != NULL) {
        ftp->hours = ifp->flight_time.hours;
        ftp->minutes = ifp->flight_time.minutes;
        fip->flight_time = ftp;
      }
    }
    /*
    fip->landed_out = ifp->landed_out;
    */

    if (DollarAmountOkay (&(ifp->flight_cost))) {
      dap = DollarAmountNew ();
      if (dap != NULL) {
        dap->dollars = ifp->flight_cost.dollars;
        dap->cents = ifp->flight_cost.cents;
        fip->flight_cost = dap;
      }
    }

    if (flights == NULL) {
      flights = fip;
    }
    if (lastfip != NULL) {
      lastfip->next = fip;
    }
    lastfip = fip;
  }

  return (Pointer) flights;
}

extern DialoG CreateAirplaneFlightsDialog (
  GrouP prnt,
  SscTablesPtr stp,
  BaseFormPtr bfp
)

{
  GrouP          cts, tbl, g0, g1, g2, g3, g4, g5, gy, gz, p;
  FonT           fnt;
  Int2           i, hgt, wid;
  SscFlightsPtr  sfp;
  CharPtr        str;
  ButtoN         to, ld;

  sfp = (SscFlightsPtr) MemNew (sizeof (SscFlights));
  if (sfp == NULL) return NULL;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, sfp, StdCleanupExtraProc);
  sfp->dialog = (DialoG) p;
  sfp->todialog = FlightInfoPtrToFlightPage;
  sfp->fromdialog = FlightPageToFlightInfoPtr;

  sfp->tables = stp;
  sfp->bfp = bfp;

  SelectFont (systemFont);
  hgt = LineHeight ();
  flightColFmt [0].pixWidth = StringWidth ("00 ") + 5;
  flightColFmt [1].pixWidth = 25 * StringWidth ("X") + 5;
  flightColFmt [2].pixWidth = StringWidth ("GROB 103 ") + 5;
  flightColFmt [3].pixWidth = StringWidth ("99:99 ") + 5;
  flightColFmt [4].pixWidth = StringWidth ("99:99 ") + 5;
  flightColFmt [5].pixWidth = StringWidth ("99:99 ") + 5;
  flightColFmt [6].pixWidth = StringWidth ("$100.00 ") + 5;
  SelectFont (systemFont);

  wid = 0;
  for (i = 0; i < 7; i++) {
    wid += flightColFmt [i].pixWidth;
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
  StaticPrompt (g0, "Flt", flightColFmt [0].pixWidth, 0, fnt, 'r');
  StaticPrompt (g0, "Name", flightColFmt [1].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Airplane", flightColFmt [2].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Tkoff", flightColFmt [3].pixWidth, 0, fnt, 'r');
  StaticPrompt (g0, "Land", flightColFmt [4].pixWidth, 0, fnt, 'r');
  StaticPrompt (g0, "Time", flightColFmt [5].pixWidth, 0, fnt, 'r');
  StaticPrompt (g0, "Total", flightColFmt [6].pixWidth, 0, fnt, 'r');

  sfp->flightlog = DocumentPanel (tbl, wid + 2, NUM_FLIGHT_LINES * hgt + 2);
  SetObjectExtra (sfp->flightlog, sfp, NULL);
  SetDocCache (sfp->flightlog, NULL, NULL, NULL);
  SetDocNotify (sfp->flightlog, FlightsDialogNotify);
  sfp->numFlights = 0;

#ifdef WIN_MAC
  SetDocShade (sfp->flightlog, NULL, NULL, NULL, ColorFlightTable);
#else
  SetDocShade (sfp->flightlog, NULL, GrayFlightTable, NULL, NULL);
#endif

  for (i = 0; i < NUM_FLIGHT_LINES; i++) {
    AppendItem (sfp->flightlog, PrintFlightTable, sfp, FALSE, 1,
                &flightParFmt, flightColFmt, systemFont);
  }

  g5 = HiddenGroup (tbl, -10, 0, NULL);
  SetGroupSpacing (g5, 0, 3);
  StaticPrompt (g5, "", flightColFmt [0].pixWidth, 0, fnt, 'r');
  StaticPrompt (g5, "", flightColFmt [1].pixWidth, 0, fnt, 'c');
  StaticPrompt (g5, "", flightColFmt [2].pixWidth, 0, fnt, 'c');
  StaticPrompt (g5, "", flightColFmt [3].pixWidth, 0, fnt, 'r');
  StaticPrompt (g5, "", flightColFmt [4].pixWidth, 0, fnt, 'r');
  StaticPrompt (g5, "", flightColFmt [5].pixWidth, 0, fnt, 'r');
  StaticPrompt (g5, "", flightColFmt [6].pixWidth, 0, fnt, 'c');
  sfp->ttl_cost = StaticPrompt (g5, "", flightColFmt [7].pixWidth, 0, systemFont, 'r');

  cts = HiddenGroup (p, -1, 0, NULL);
  SetGroupSpacing (cts, 10, 10);
  SetGroupMargins (cts, 5, 5);

  g1 = HiddenGroup (cts, -10, 0, NULL);
  SetGroupSpacing (g1, 10, 5);

  StaticPrompt (g1, "Name", 0, popupMenuHeight, programFont, 'l');
  sfp->name = CreateEnumPopupDialog (g1, TRUE, ChangeName, stp->members.alist, (UIEnum) 0, sfp);

  g4 = HiddenGroup (g1, -10, 0, NULL);
  SetGroupSpacing (g4, 10, 5);
  sfp->aftername = g4;

  StaticPrompt (g4, "Instructor", 0, popupMenuHeight, programFont, 'l');
  sfp->instructor = CreateEnumPopupDialog (g4, TRUE, ChangeInstructor, stp->instructors.alist, (UIEnum) 0, sfp);

  g2 = HiddenGroup (cts, -20, 0, NULL);
  SetGroupSpacing (g2, 10, 5);

  StaticPrompt (g2, "Airplane", 0, popupMenuHeight, programFont, 'l');
  sfp->airplane = CreateEnumPopupDialog (g2, TRUE, ChangeAirplane, stp->towplane_alist, (UIEnum) 0, sfp);

  StaticPrompt (g2, "Class", 0, popupMenuHeight, programFont, 'l');
  sfp->fclass = CreateEnumPopupDialog (g2, TRUE, ChangeFclass, fclass_alist, (UIEnum) 0, sfp);

  g3 = HiddenGroup (cts, -20, 0, NULL);
  SetGroupSpacing (g3, 10, 5);

  to = PushButton (g3, "Takeoff", SetTakeoffTime);
  SetObjectExtra (to, sfp, NULL);

  sfp->takeoff = CreateClockTimeDialog (g3, (Pointer) sfp, ChangeTakeoff);

  ld = PushButton (g3, "Landing", SetLandingTime);
  SetObjectExtra (ld, sfp, NULL);

  sfp->landing = CreateClockTimeDialog (g3, (Pointer) sfp, ChangeLanding);

  StaticPrompt (g3, "Passenger", 0, dialogTextHeight, programFont, 'l');
  str = WidestAlist (stp->members.alist);
  sfp->passenger = DialogText (g3, str, 0, ChangePassenger);
  SetObjectExtra (sfp->passenger, sfp, NULL);
  SetTitle (sfp->passenger, "");

  gy = HiddenGroup (cts, -20, 0, NULL);
  SetGroupSpacing (gy, 10, 5);

  sfp->landout = CheckBox (gy, "Landed Out", SetLandout);
  SetObjectExtra (sfp->landout, sfp, NULL);

  sfp->special_pay = CheckBox (gy, "Special Payment", ChangeSpecialPay);
  SetObjectExtra (sfp->special_pay, sfp, NULL);

  gz = HiddenGroup (gy, -20, 0, NULL);
  SetGroupSpacing (gz, 10, 5);

  StaticPrompt (gz, "Alternative Payer", 0, dialogTextHeight, programFont, 'l');
  str = WidestAlist (stp->payers.alist);
  sfp->alt_payer = CreateEnumPopupDialog (gz, TRUE, ChangePayer, stp->payers.alist, (UIEnum) 0, sfp);

  StaticPrompt (gz, "Pays", 0, popupMenuHeight, programFont, 'l');
  sfp->split_payment = CreateEnumPopupDialog (gz, TRUE, ChangeSplit, ratio_alist, (UIEnum) 0, sfp);
  SetObjectExtra (sfp->split_payment, sfp, NULL);

  sfp->special_group = gz;
  SafeHide (sfp->special_group);

  AlignObjects (ALIGN_CENTER, (HANDLE) tbl, (HANDLE) cts, NULL);

  sfp->numFlights = 0;
  sfp->currItem = 1;

  return (DialoG) p;
}


