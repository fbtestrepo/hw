/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Glider flights page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* internal version of FlightInfoPtr/GliderFlightPtr */

typedef struct intgfpinfo {
  CharPtr       name;
  Int4          type;
  CharPtr       alt_payer;
  Int4          split_payment;
  CharPtr       glider;
  Int4          category;
  CharPtr       instructor;
  CharPtr       towpilot;
  CharPtr       towplane;
  CharPtr       passenger;
  ClockTime     takeoff;
  ClockTime     landing;
  Int4          exception;
  Boolean       aloft;
  FlightTime    flight_time;
  Int4          release_altitude;
  DollarAmount  flight_cost;
  DollarAmount  tow_cost;
  DollarAmount  total_cost;
  CharPtr       comment;
  Boolean       warn_on_name_change;
} IntFlightInfo, PNTR IntFlightPtr;

/* dialog controls */

typedef struct sscflights {
  DIALOG_MESSAGE_BLOCK

  DoC           flightlog;
  Int2          currItem;

  PopuP         name;
  GrouP         aftername;
  PopuP         glider;
  PopuP         fclass;
  PopuP         instructor;
  PopuP         towpilot;
  PopuP         towplane;
  TexT          passenger;
  DialoG        takeoff;
  DialoG        landing;
  PopuP         exception;
  DialoG        release;
  PopuP         alt_payer;
  PopuP         split_payment;

  PrompT        ttl_rent;
  PrompT        ttl_tow;
  PrompT        ttl_cost;

  DollarAmount  tot_rent;
  DollarAmount  tot_tow;
  DollarAmount  tot_cost;

  Int2          numFlights;
  IntFlightPtr  glider_flights [128];

  ValNodePtr    recentName;
  ValNodePtr    latestName;
  ValNodePtr    recentInst;
  ValNodePtr    latestInst;
  ValNodePtr    recentTowpilot;
  ValNodePtr    latestTowpilot;
  ValNodePtr    recentTowplane;
  ValNodePtr    latestTowplane;
  ValNodePtr    recentGlider;
  ValNodePtr    latestGlider;
  ValNodePtr    recentClass;
  ValNodePtr    latestClass;
  Boolean       changeAfterTakeoff;

  SscTablesPtr  tables;
  BaseFormPtr   bfp;
} SscFlights, PNTR SscFlightsPtr;

#define NUM_FLIGHT_LINES 10

static IntFlightPtr IntFlightInfoNew (
  void
)

{
  ClockTimePtr     ctp;
  DollarAmountPtr  dap;
  FlightTimePtr    ftp;
  IntFlightPtr     ifp;

  ifp = (IntFlightPtr) MemNew (sizeof (IntFlightInfo));
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

  dap = &(ifp->tow_cost);
  dap->dollars = -1;
  dap->cents = -1;

  dap = &(ifp->total_cost);
  dap->dollars = -1;
  dap->cents = -1;

  return ifp;
}

static IntFlightPtr IntFlightInfoFree (
  IntFlightPtr ifp
)

{
  if (ifp == NULL) return NULL;

  MemFree (ifp->name);
  MemFree (ifp->alt_payer);
  MemFree (ifp->glider);
  MemFree (ifp->instructor);
  MemFree (ifp->towpilot);
  MemFree (ifp->towplane);
  MemFree (ifp->passenger);

  return MemFree (ifp);
}

static IntFlightPtr GetFlightInfo (
  SscFlightsPtr sfp,
  Int2 item
)

{
  IntFlightPtr  ifp;

  if (sfp == NULL || item < 0 || item > 127) return NULL;
  ifp = sfp->glider_flights [item];
  if (ifp != NULL) return ifp;

  ifp = IntFlightInfoNew ();
  if (ifp != NULL) {
    ifp->name = StringSave (" ");
    ifp->alt_payer = StringSave (" ");
    ifp->glider = StringSave (" ");
    ifp->instructor = StringSave (" ");
    ifp->towpilot = StringSave (" ");
    ifp->towplane = StringSave (" ");
    ifp->release_altitude = -1;
  }
  sfp->glider_flights [item] = ifp;
  return ifp;
}

static ParData  flightParFmt = { FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0 };

static ColData  flightColFmt [] = {
  {0, 5,  2, 0, NULL, 'r', FALSE, FALSE, FALSE, FALSE, FALSE},  /* number  */
  {0, 5, 25, 3, NULL, 'l', FALSE, TRUE,  FALSE, FALSE, FALSE},  /* name    */
  {0, 5, 12, 2, NULL, 'l', FALSE, TRUE,  FALSE, FALSE, FALSE},  /* glider  */
  {0, 5,  7, 2, NULL, 'r', FALSE, TRUE,  FALSE, FALSE, FALSE},  /* takeoff */
  {0, 5,  7, 2, NULL, 'r', FALSE, FALSE, FALSE, FALSE, FALSE},  /* landing */
  {0, 5,  7, 2, NULL, 'r', FALSE, FALSE, FALSE, FALSE, FALSE},  /* time    */
  {0, 5,  9, 2, NULL, 'r', FALSE, FALSE, FALSE, FALSE, FALSE},  /* release */
  {0, 5,  9, 2, NULL, 'r', FALSE, TRUE,  FALSE, FALSE, FALSE},  /* rental  */
  {0, 5,  9, 2, NULL, 'r', FALSE, FALSE, FALSE, FALSE, FALSE},  /* towcost */
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
  {"FAST",  7},
END_ENUM_ALIST

static ENUM_ALIST(except_alist)
  {" ",                0},
  {"Landed Out",       1},
  {"Aborted Takeoff",  2},
  {"Simulated PTT",    3},
  {"Actual PTT",       4},
  {"Other",            5},
END_ENUM_ALIST

static ENUM_ALIST(ratio_alist)
  {" ",       0},
  {"All",     1},
  {"Half",    2},
  {"Rental",  3},
  {"Tow",     4},
END_ENUM_ALIST

static void RecalculateTime (
  SscFlightsPtr sfp,
  FlightTimePtr ftp,
  ClockTimePtr lnd,
  Int2 item
)

{
  ClockTimePtr  ctp;
  IntFlightPtr  ifp;
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
    } else if (to_min == ld_min && to_hour == ld_hour) {
      ftp->hours = 0;
      ftp->minutes = 0;
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
  IntFlightPtr   ifp;
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
  IntFlightPtr   ifp;
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
  Int4             hundreds, thousands;
  IntFlightPtr     ifp;
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
  if (StringDoesHaveText (ifp->glider)) {
    StringCat (buf, ifp->glider);
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
  if (ifp->release_altitude >= 0) {
    thousands = ifp->release_altitude / 1000;
    hundreds = ifp->release_altitude % 1000;
    sprintf (tmp, "%1ld,%3ld", (long) thousands, (long) hundreds);
    if (tmp [2] == ' ') {
      tmp [2] = '0';
    }
    if (tmp [3] == ' ') {
      tmp [3] = '0';
    }
    StringCat (buf, tmp);
  }

  StringCat (buf, "\t");
  dap = &(ifp->flight_cost);
  if (PrintDollarAmount (dap, tmp)) {
    StringCat (buf, tmp);
  }

  StringCat (buf, "\t");
  dap = &(ifp->tow_cost);
  if (PrintDollarAmount (dap, tmp)) {
    StringCat (buf, tmp);
  }

  StringCat (buf, "\t");
  dap = &(ifp->total_cost);
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
  IntFlightPtr   ifp;
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
    SetEnumPopupByName (sfp->glider, stp->glider_alist, ifp->glider);
    SetEnumPopupByName (sfp->instructor, stp->instructors.alist, ifp->instructor);
    SetEnumPopupByName (sfp->towpilot, stp->towpilots.alist, ifp->towpilot);
    SetEnumPopupByName (sfp->towplane, stp->towplane_alist, ifp->towplane);
    SetEnumPopup (sfp->fclass, fclass_alist, (UIEnum) ifp->category);
    SetEnumPopup (sfp->exception, except_alist, (UIEnum) ifp->exception);
    SetTitle (sfp->passenger, ifp->passenger);
    SetEnumPopupByName (sfp->alt_payer, stp->payers.alist, ifp->alt_payer);
    SetEnumPopup (sfp->split_payment, ratio_alist, (UIEnum) ifp->split_payment);

    SetClockTime (sfp->takeoff, ifp->takeoff.hour, ifp->takeoff.minute);

    SetClockTime (sfp->landing, ifp->landing.hour, ifp->landing.minute);

    SetReleaseHeight (sfp->release, ifp->release_altitude);
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
  IntFlightPtr     ifp;

  if (sfp == NULL) return;

  tot_cents = 0;
  tot_dollars = 0;
  for (i = 1; i < 128; i++) {
    ifp = sfp->glider_flights [i];
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
  SetTitle (sfp->ttl_rent, buf1);

  dap = &(sfp->tot_rent);
  if (dap != NULL) {
    dap->dollars = tot_dollars;
    dap->cents = tot_cents;
  }

  tot_cents = 0;
  tot_dollars = 0;
  for (i = 1; i < 128; i++) {
    ifp = sfp->glider_flights [i];
    if (ifp == NULL) continue;
    dap = &(ifp->tow_cost);
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
  SetTitle (sfp->ttl_tow, buf1);

  dap = &(sfp->tot_tow);
  if (dap != NULL) {
    dap->dollars = tot_dollars;
    dap->cents = tot_cents;
  }

  tot_cents = 0;
  tot_dollars = 0;
  for (i = 1; i < 128; i++) {
    ifp = sfp->glider_flights [i];
    if (ifp == NULL) continue;
    dap = &(ifp->total_cost);
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

static GliderInfoPtr GetGliderByName (
  GliderTablePtr gtp,
  CharPtr str
)

{
  GliderInfoPtr  gip;

  if (gtp == NULL || StringHasNoText (str)) return NULL;

  for (gip = gtp; gip != NULL; gip = gip->next) {
    if (StringICmp (str, gip->glider) == 0) return gip;
  }

  return NULL;
}

static void RecalculateGlidersAloft (
  SscFlightsPtr sfp
)

{
  BaseFormPtr   bfp;
  ClockTimePtr  ctp;
  Int2          gliders_aloft = 0;
  IntFlightPtr  ifp;
  Int2          k, numItems;
  SscTablesPtr  stp;
  Char          tmp [32];
  Char          ttl [128];

  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;

  GetDocParams (sfp->flightlog, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    ifp = sfp->glider_flights [k];
    if (ifp == NULL) continue;

    ctp = &(ifp->takeoff);
    if (ctp != NULL && ctp->hour > 2 && ctp->hour <= 24 &&
        ctp->minute >= 0 && ctp->minute <= 59) {
      gliders_aloft++;
      ctp = &(ifp->landing);
      if (ctp != NULL && ctp->hour > 2 && ctp->hour <= 24 &&
          ctp->minute >= 0 && ctp->minute <= 59) {
        gliders_aloft--;
      }
    }
  }

  bfp = sfp->bfp;
  if (bfp == NULL) return;
  tmp [0] = '\0';
  if (gliders_aloft == 1) {
    sprintf (tmp, " - 1 glider aloft");
  } else if (gliders_aloft > 1) {
    sprintf (tmp, " - %d gliders aloft", (int) gliders_aloft);
  }
  sprintf (ttl, "Skyline Soaring Logsheet %s - %s%s", program_version, stp->date_string, tmp);
  SetTitle (bfp->form, ttl);
}

static void RecalculateCosts (
  SscFlightsPtr sfp,
  Int2 item
)

{
  Int4             cents, dollars, pennies, minutes, maxminutes,
                   rental_cost, tot_cents, tot_dollars;
  DollarAmountPtr  dap, fee;
  FlightTimePtr    ftp, maxtime;
  GliderInfoPtr    gip;
  IntFlightPtr     ifp;
  MiscRatePtr      mrp;
  SscTablesPtr     stp;
  TowPlaneListPtr  tlp;
  CharPtr          tmp;
  TowRateItemPtr   towrate;
  TowRateItemPtr   trp;
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
    dap = &(ifp->tow_cost);
    if (dap != NULL) {
      dap->dollars = 0;
      dap->cents = 0;
    }
    dap = &(ifp->flight_cost);
    if (dap != NULL) {
      dap->dollars = 0;
      dap->cents = 0;
    }
    dap = &(ifp->total_cost);
    if (dap != NULL) {
      dap->dollars = 0;
      dap->cents = 0;
    }

    RecalculateFlightTotals (sfp);

    return;
  }

  /* FAST lesson, SSA-designated fixed price */

  if (ifp->category == 7) {
    dap = &(ifp->tow_cost);
    if (dap != NULL) {
      dap->dollars = 0;
      dap->cents = 0;
    }
    dap = &(ifp->flight_cost);
    if (dap != NULL) {
      dap->dollars = 0;
      dap->cents = 0;
    }
    fee = mrp->fast_fee;
    dap = &(ifp->total_cost);
    if (dap != NULL) {
      dap->dollars = fee->dollars;
      dap->cents = fee->cents;
    }

    RecalculateFlightTotals (sfp);

    return;
  }

  /* normal flight charges */

  dollars = -1;
  cents = -1;

  if (ifp->release_altitude >= 0) {
    towrate = stp->towrate_table;
    if (StringDoesHaveText (ifp->towplane)) {
      for (tlp = stp->towset_table; tlp != NULL; tlp = tlp->next) {
        if (StringICmp (tlp->towplane, ifp->towplane) == 0) {
          if (tlp->rates != NULL) {
            towrate = tlp->rates;
          }
        }
      }
    }
    for (trp = towrate; trp != NULL; trp = trp->next) {
      if (trp->altitude == ifp->release_altitude) {
        dap = trp->cost_of_tow;
        if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
          dollars = dap->dollars;
          cents = dap->cents;
        }
      }
    }
  }

  dap = &(ifp->tow_cost);
  if (dap != NULL) {
    dap->dollars = dollars;
    dap->cents = cents;
  }

  tot_dollars = dollars;
  tot_cents = cents;

  dollars = -1;
  cents = -1;

  gip = GetGliderByName (stp->glider_table, ifp->glider);
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

  dap = &(ifp->total_cost);
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
  IntFlightPtr  ifp;
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

  RecalculateGlidersAloft (sfp);

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
  DayTime  dt;
  Int4     new_hour, new_min, old_hour, old_min;

  if (! GetDayTime (&dt)) return;
  new_hour = (Int4) dt.tm_hour;
  new_min = (Int4) dt.tm_min;

  GetClockTime (d, &old_hour, &old_min);

  if (new_hour == old_hour && new_min == old_min) return;
  if (old_hour != -1 || old_min != -1) {
    if (Message (MSG_YN, "Do you want to override the existing time?") == ANS_NO) return;
  }

  SetClockTime (d, new_hour, new_min);
}

static void SetTakeoffTime (
  ButtoN b
)

{
  IntFlightPtr   ifp;
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

static void TakeoffItemProc (
  IteM i
)

{
  BaseFormPtr    bfp;
  DialoG         d;
  IntFlightPtr   ifp;
  SscFlightsPtr  sfp;
  SscTablesPtr   stp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  stp = GetTablesPointer (bfp);
  if (stp == NULL) return;

  d = stp->glider_flights;
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

  d = stp->glider_flights;
  if (! Visible (d)) return;
  if (! AllParentsVisible (d)) return;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;

  SetATime (sfp->landing);

  ImposeTimes (sfp, sfp->currItem, TRUE);
}

static Boolean AreThereTooManyPeople (
  SscTablesPtr stp,
  IntFlightPtr ifp,
  Int4Ptr num_seatsP
)

{
  Int2           count;
  GliderInfoPtr  gip;
  Int4           number_of_seats;

  if (num_seatsP != NULL) {
    *num_seatsP = 0;
  }

  count = 1;
  gip = GetGliderByName (stp->glider_table, ifp->glider);
  if (gip != NULL) {
    number_of_seats = gip->number_of_seats;
    if (number_of_seats == 0) {
      number_of_seats = 1;
    }
    if (StringDoesHaveText (ifp->instructor)) {
      count++;
    }
    if (StringDoesHaveText (ifp->passenger)) {
      count++;
    }
    if (count > number_of_seats) {
      if (num_seatsP != NULL) {
        *num_seatsP = number_of_seats;
      }
      return TRUE;
    }
  }

  return FALSE;
}

static void TooManyPeopleWarning (
  SscFlightsPtr sfp,
  IntFlightPtr ifp,
  Int4 num_seats,
  Boolean ask_to_delete
)

{
  MsgAnswer  ans;
  CharPtr    occupants = "people";

  if (num_seats == 1) {
    occupants = "person";
  }

  if (ask_to_delete) {
    ans = Message (MSG_YN, "The glider can only hold %ld %s\nDo you wish to remove the passenger name?",
                   (long) num_seats, occupants);
    if (ans == ANS_YES && sfp != NULL && ifp != NULL) {
      ifp->passenger = MemFree (ifp->passenger);
      SetTitle (sfp->passenger, "");
    }
  } else {
    Message (MSG_OK, "The glider can only hold %ld %s", (long) num_seats, occupants);
  }
}

static void ChangeName (
  PopuP p
)

{
  AlistDialogPtr  adp;
  ClockTimePtr    ctp;
  IntFlightPtr    ifp;
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

  if (StringDoesHaveText (ifp->instructor) && StringICmp (str, ifp->instructor) == 0) {
    if (Message (MSG_YN, "Same name as instructor. Do you still want to change the name?") == ANS_NO) {
      SetEnumPopupByName (sfp->name, stp->members.alist, ifp->name);
      MemFree (str);
      return;
    }
  }

  if (StringDoesHaveText (ifp->towpilot) && StringICmp (str, ifp->towpilot) == 0) {
    if (Message (MSG_YN, "Same name as tow pilot. Do you still want to change the name?") == ANS_NO) {
      SetEnumPopupByName (sfp->name, stp->members.alist, ifp->name);
      MemFree (str);
      return;
    }
  }

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

static void ChangeGlider (
  PopuP p
)

{
  AlistDialogPtr  adp;
  ClockTimePtr    ctp;
  IntFlightPtr    ifp;
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
    if (StringDoesHaveText (ifp->glider)) {
      if (Message (MSG_YN, "Do you want to override the existing glider?") == ANS_NO) {
        SetEnumPopupByName (sfp->glider, stp->glider_alist, ifp->glider);
        return;
      }
    }
  }

  str = GetEnumPopupByName (sfp->glider, stp->glider_alist);
  ifp->glider = MemFree (ifp->glider);
  ifp->glider = str; /* allocated by GetEnumPopupByName */

  if (ifp->glider == NULL) {
    ifp->glider = StringSave (" ");
  }

  RecalculateCosts (sfp, sfp->currItem);

  UpdateDocument (sfp->flightlog, sfp->currItem, sfp->currItem);
  Update ();

  CheckExtendTable (sfp);

  if (StringDoesHaveText (ifp->name)) {
    ifp->warn_on_name_change = TRUE;
  }

  if (AreThereTooManyPeople (stp, ifp, &num_seats)) {
    TooManyPeopleWarning (sfp, ifp, num_seats, FALSE);
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

static void SameGliderProc (
  IteM i
)

{
  BaseFormPtr    bfp;
  ClockTimePtr   ctp;
  DialoG         d;
  IntFlightPtr   ifp;
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

  d = stp->glider_flights;
  if (! Visible (d)) return;
  if (! AllParentsVisible (d)) return;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;

  if (sfp->currItem <= 1) {
    Beep ();
    return;
  }

  if (sfp->recentGlider == NULL) {
    str = NULL;
    for (j = sfp->currItem - 1; j > 0; j--) {
      ifp = GetFlightInfo (sfp, j);
      if (ifp == NULL) continue;
      str = ifp->glider;
      if (StringHasNoText (str)) continue;
      if (StringCmp (str, "-") == 0) continue;
      if (StringInValNodeList (sfp->recentGlider, str)) continue;
      ValNodeCopyStr (&(sfp->recentGlider), 0, str);
    }
    sfp->latestGlider = sfp->recentGlider;
  }

  str = NULL;
  if (sfp->latestGlider != NULL) {
    vnp = sfp->latestGlider;
    str = (CharPtr) vnp->data.ptrvalue;
    sfp->latestGlider = vnp->next;
  } else if (sfp->recentGlider != NULL) {
    sfp->latestGlider = sfp->recentGlider;
  } else {
    return;
  }

  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  ctp = &(ifp->takeoff);
  if (ctp != NULL && ctp->hour > 2 && ctp->hour <= 24 &&
      ctp->minute >= 0 && ctp->minute <= 59 && (! sfp->changeAfterTakeoff)) {
    if (StringDoesHaveText (ifp->glider) && StringICmp (ifp->glider, str) != 0) {
      if (Message (MSG_YN, "Do you want to override the existing glider?") == ANS_NO) return;
    }
  }

  ifp->glider = MemFree (ifp->glider);
  ifp->glider = StringSave (str);

  if (ifp->glider == NULL) {
    ifp->glider = StringSave (" ");
  }

  SetEnumPopupByName (sfp->glider, stp->glider_alist, ifp->glider);

  RecalculateCosts (sfp, sfp->currItem);

  UpdateDocument (sfp->flightlog, sfp->currItem, sfp->currItem);
  Update ();

  CheckExtendTable (sfp);

  if (StringDoesHaveText (ifp->name)) {
    ifp->warn_on_name_change = TRUE;
  }

  if (AreThereTooManyPeople (stp, ifp, &num_seats)) {
    /*
    TooManyPeopleWarning (sfp, ifp, num_seats, FALSE);
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
  IntFlightPtr   ifp;
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

  d = stp->glider_flights;
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
  IntFlightPtr   ifp;
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

  d = stp->glider_flights;
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
          TooManyPeopleWarning (sfp, ifp, num_seats, TRUE);
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
    TooManyPeopleWarning (sfp, ifp, num_seats, TRUE);
  }
}

static void SameTowPilotProc (
  IteM i
)

{
  BaseFormPtr    bfp;
  ClockTimePtr   ctp;
  DialoG         d;
  IntFlightPtr   ifp;
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

  d = stp->glider_flights;
  if (! Visible (d)) return;
  if (! AllParentsVisible (d)) return;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;

  if (sfp->currItem <= 1) {
    Beep ();
    return;
  }

  if (sfp->recentTowpilot == NULL) {
    for (j = sfp->currItem - 1; j > 0; j--) {
      ifp = GetFlightInfo (sfp, j);
      if (ifp == NULL) continue;
      str = ifp->towpilot;
      if (StringHasNoText (str)) continue;
      if (StringCmp (str, "-") == 0) continue;
      if (StringInValNodeList (sfp->recentTowpilot, str)) continue;
      ValNodeCopyStr (&(sfp->recentTowpilot), 0, str);
    }
    sfp->latestTowpilot = sfp->recentTowpilot;
  }

  str = NULL;
  if (sfp->latestTowpilot != NULL) {
    vnp = sfp->latestTowpilot;
    str = (CharPtr) vnp->data.ptrvalue;
    sfp->latestTowpilot = vnp->next;
  } else if (sfp->recentTowpilot != NULL) {
    sfp->latestTowpilot = sfp->recentTowpilot;
  } else {
    return;
  }

  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  ctp = &(ifp->takeoff);
  if (ctp != NULL && ctp->hour > 2 && ctp->hour <= 24 &&
      ctp->minute >= 0 && ctp->minute <= 59 && (! sfp->changeAfterTakeoff)) {
    if (StringDoesHaveText (ifp->towpilot) && StringICmp (ifp->towpilot, str) != 0) {
      if (Message (MSG_YN, "Do you want to override the existing tow pilot?") == ANS_NO) {
        return;
      }
    }
  }

  ifp->towpilot = MemFree (ifp->towpilot);
  ifp->towpilot = StringSave (str);

  if (ifp->towpilot == NULL) {
    ifp->towpilot = StringSave (" ");
  }

  SetEnumPopupByName (sfp->towpilot, stp->towpilots.alist, ifp->towpilot);

  Update ();

  CheckExtendTable (sfp);

  if (StringDoesHaveText (ifp->name)) {
    ifp->warn_on_name_change = TRUE;
  }
}

static void SameTowPlaneProc (
  IteM i
)

{
  BaseFormPtr    bfp;
  ClockTimePtr   ctp;
  DialoG         d;
  IntFlightPtr   ifp;
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

  d = stp->glider_flights;
  if (! Visible (d)) return;
  if (! AllParentsVisible (d)) return;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;

  if (sfp->currItem <= 1) {
    Beep ();
    return;
  }

  if (sfp->recentTowplane == NULL) {
    for (j = sfp->currItem - 1; j > 0; j--) {
      ifp = GetFlightInfo (sfp, j);
      if (ifp == NULL) continue;
      str = ifp->towplane;
      if (StringHasNoText (str)) continue;
      if (StringCmp (str, "-") == 0) continue;
      if (StringInValNodeList (sfp->recentTowplane, str)) continue;
      ValNodeCopyStr (&(sfp->recentTowplane), 0, str);
    }
    sfp->latestTowplane = sfp->recentTowplane;
  }

  str = NULL;
  if (sfp->latestTowplane != NULL) {
    vnp = sfp->latestTowplane;
    str = (CharPtr) vnp->data.ptrvalue;
    sfp->latestTowplane = vnp->next;
  } else if (sfp->recentTowplane != NULL) {
    sfp->latestTowplane = sfp->recentTowplane;
  } else {
    return;
  }

  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  ctp = &(ifp->takeoff);
  if (ctp != NULL && ctp->hour > 2 && ctp->hour <= 24 &&
      ctp->minute >= 0 && ctp->minute <= 59 && (! sfp->changeAfterTakeoff)) {
    if (StringDoesHaveText (ifp->towplane) && StringICmp (ifp->towplane, str) != 0) {
      if (Message (MSG_YN, "Do you want to override the existing towplane?") == ANS_NO) {
        return;
      }
    }
  }

  ifp->towplane = MemFree (ifp->towplane);
  ifp->towplane = StringSave (str);

  if (ifp->towplane == NULL) {
    ifp->towplane = StringSave (" ");
  }

  SetEnumPopupByName (sfp->towplane, stp->towplane_alist, ifp->towplane);

  RecalculateCosts (sfp, sfp->currItem);

  UpdateDocument (sfp->flightlog, sfp->currItem, sfp->currItem);
  Update ();

  CheckExtendTable (sfp);

  if (StringDoesHaveText (ifp->name)) {
    ifp->warn_on_name_change = TRUE;
  }
}

static void SameClassProc (
  IteM i
)

{
  BaseFormPtr    bfp;
  ClockTimePtr   ctp;
  DialoG         d;
  IntFlightPtr   ifp;
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

  d = stp->glider_flights;
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
  IntFlightPtr    ifp;
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

static void ChangeExcept (
  PopuP p
)

{
  AlistDialogPtr  adp;
  IntFlightPtr    ifp;
  Int4            old_hour, old_min, to_hour, to_min;
  Int4            rel_hgt;
  SscFlightsPtr   sfp;
  UIEnum          val;

  adp = (AlistDialogPtr) GetObjectExtra (p);
  if (adp == NULL) return;
  sfp = (SscFlightsPtr) adp->userdata;
  if (sfp == NULL) return;
  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  if (GetEnumPopup (sfp->exception, except_alist, &val)) {
    if (val > 0) {
      if (Message (MSG_YN, "Are you sure you want to set an exception status for this flight?") == ANS_NO) {
        SetEnumPopup (sfp->exception, except_alist, (UIEnum) ifp->exception);
        return;
      }
    }

    ifp->exception = (Int4) val;

    if (val > 0) {
      GetClockTime (sfp->landing, &old_hour, &old_min);
      if (old_hour == -1 && old_min == -1) {
        if (val == 2) {
          GetClockTime (sfp->takeoff, &to_hour, &to_min);
          if (old_hour != to_hour || old_min != to_min || ifp->release_altitude != 0) {
            if (Message (MSG_YN, "Do you want to set the landing time and release altitude for this flight?") == ANS_YES) {
              SetClockTime (sfp->landing, to_hour, to_min);
              ImposeTimes (sfp, sfp->currItem, TRUE);
              ifp->release_altitude = 0;
              SetReleaseHeight (sfp->release, ifp->release_altitude);
            }
          }
        } else {
          if (Message (MSG_YN, "Do you want to set the landing time for this flight?") == ANS_YES) {
            SetATime (sfp->landing);
            ImposeTimes (sfp, sfp->currItem, TRUE);
            GetReleaseHeight (sfp->release, &rel_hgt);
            if (rel_hgt == -1) {
              Message (MSG_OK, "Please set the release altitude for this flight");
            }
          }
        }
      } else {
        if (val == 2) {
          GetClockTime (sfp->takeoff, &to_hour, &to_min);
          if (old_hour != to_hour || old_min != to_min || ifp->release_altitude != 0) {
            if (Message (MSG_YN, "Do you want to set the landing time and release altitude for this flight?") == ANS_YES) {
              SetClockTime (sfp->landing, to_hour, to_min);
              ImposeTimes (sfp, sfp->currItem, TRUE);
              ifp->release_altitude = 0;
              SetReleaseHeight (sfp->release, ifp->release_altitude);
            }
          }
        } else {
          if (Message (MSG_YN, "Do you want to override the existing landing time for this flight?") == ANS_YES) {
            SetATime (sfp->landing);
            ImposeTimes (sfp, sfp->currItem, TRUE);
          }
        }
      }
    }
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
  IntFlightPtr   ifp;
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
    TooManyPeopleWarning (sfp, ifp, num_seats, TRUE);
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
  IntFlightPtr     ifp;
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

  if (StringDoesHaveText (ifp->name) && StringCmp (ifp->name, ifp->alt_payer) == 0) {
    if (Message (MSG_YN,
                 "Alternative payer is same as pilot.  Do you want to clear special payment information?") == ANS_YES) {
      clear_alt_payer = TRUE;
    }
  } else if (ifp->category == 5 || ifp->category == 6) {
    if (Message (MSG_YN,
                 "Free flights do not need alternative payer.  Do you want to clear special payment information?") == ANS_YES) {
      clear_alt_payer = TRUE;
    }
  } else if (StringHasNoText (ifp->alt_payer) && ifp->split_payment > 0) {
    if (Message (MSG_YN,
                 "Do you want to clear special payment split?") == ANS_YES) {
      clear_alt_payer = TRUE;
    }
  }
  if (clear_alt_payer) {
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
  IntFlightPtr   ifp;
  SscFlightsPtr  sfp;
  SscTablesPtr   stp;
  UIEnum         val;

  sfp = (SscFlightsPtr) GetObjectExtra (p);
  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;
  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  if (GetEnumPopup (sfp->split_payment, ratio_alist, &val)) {
    ifp->split_payment = (Int4) val;
    if (ifp->split_payment == 0 && StringDoesHaveText (ifp->alt_payer)) {
      if (Message (MSG_YN,
                   "Do you want to clear special payment payer?") == ANS_YES) {
        ifp->alt_payer = MemFree (ifp->alt_payer);
        ifp->alt_payer = StringSave (" ");
        SetEnumPopupByName (sfp->alt_payer, stp->payers.alist, " ");
      }
    }
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
  IntFlightPtr    ifp;
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
        if (AreThereTooManyPeople (stp, ifp, &num_seats)) {
          TooManyPeopleWarning (sfp, ifp, num_seats, FALSE);
        }
        return;
      }
    }
  }

  str = GetEnumPopupByName (sfp->instructor, stp->instructors.alist);

  if (StringDoesHaveText (ifp->name) && StringICmp (str, ifp->name) == 0) {
    if (Message (MSG_YN, "Same name as glider pilot. Do you still want to change the instructor?") == ANS_NO) {
      SetEnumPopupByName (sfp->instructor, stp->instructors.alist, ifp->instructor);
      MemFree (str);
      return;
    }
  }

  if (StringDoesHaveText (ifp->towpilot) && StringICmp (str, ifp->towpilot) == 0) {
    if (Message (MSG_YN, "Same name as tow pilot. Do you still want to change the instructor?") == ANS_NO) {
      SetEnumPopupByName (sfp->instructor, stp->instructors.alist, ifp->instructor);
      MemFree (str);
      return;
    }
  }

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
    TooManyPeopleWarning (sfp, ifp, num_seats, FALSE);
  }
}

static void ChangeTowPilot (
  PopuP p
)

{
  AlistDialogPtr  adp;
  ClockTimePtr    ctp;
  IntFlightPtr    ifp;
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
    if (StringDoesHaveText (ifp->towpilot)) {
      if (Message (MSG_YN, "Do you want to override the existing tow pilot?") == ANS_NO) {
        SetEnumPopupByName (sfp->towpilot, stp->towpilots.alist, ifp->towpilot);
        return;
      }
    }
  }

  str = GetEnumPopupByName (sfp->towpilot, stp->towpilots.alist);

  if (StringDoesHaveText (ifp->name) && StringICmp (str, ifp->name) == 0) {
    if (Message (MSG_YN, "Same name as glider pilot. Do you still want to change the tow pilot?") == ANS_NO) {
      SetEnumPopupByName (sfp->towpilot, stp->towpilots.alist, ifp->towpilot);
      MemFree (str);
      return;
    }
  }

  if (StringDoesHaveText (ifp->instructor) && StringICmp (str, ifp->instructor) == 0) {
    if (Message (MSG_YN, "Same name as instructor. Do you still want to change the tow pilot?") == ANS_NO) {
      SetEnumPopupByName (sfp->towpilot, stp->towpilots.alist, ifp->towpilot);
      MemFree (str);
      return;
    }
  }

  ifp->towpilot = MemFree (ifp->towpilot);
  ifp->towpilot = str; /* allocated by GetEnumPopupByName */

  if (ifp->towpilot == NULL) {
    ifp->towpilot = StringSave (" ");
  }

  CheckExtendTable (sfp);

  if (StringDoesHaveText (ifp->name)) {
    ifp->warn_on_name_change = TRUE;
  }
}

static void ChangeTowPlane (
  PopuP p
)

{
  AlistDialogPtr  adp;
  ClockTimePtr    ctp;
  IntFlightPtr    ifp;
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
    if (StringDoesHaveText (ifp->towplane)) {
      if (Message (MSG_YN, "Do you want to override the existing tow plane?") == ANS_NO) {
        SetEnumPopupByName (sfp->towplane, stp->towplane_alist, ifp->towplane);
        return;
      }
    }
  }

  str = GetEnumPopupByName (sfp->towplane, stp->towplane_alist);
  ifp->towplane = MemFree (ifp->towplane);
  ifp->towplane = str; /* allocated by GetEnumPopupByName */

  if (ifp->towplane == NULL) {
    ifp->towplane = StringSave (" ");
  }

  RecalculateCosts (sfp, sfp->currItem);

  UpdateDocument (sfp->flightlog, sfp->currItem, sfp->currItem);
  Update ();

  CheckExtendTable (sfp);

  if (StringDoesHaveText (ifp->name)) {
    ifp->warn_on_name_change = TRUE;
  }
}

static void ChangeRelease (
  DialoG d
)

{
  BaseDialogPtr  bdp;
  IntFlightPtr   ifp;
  Int4           rel_height;
  SscFlightsPtr  sfp;

  bdp = (BaseDialogPtr) GetObjectExtra (d);
  if (bdp == NULL) return;
  sfp = (SscFlightsPtr) bdp->userdata;
  if (sfp == NULL) return;
  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  GetReleaseHeight (d, &rel_height);

  ifp->release_altitude = rel_height;

  RecalculateCosts (sfp, sfp->currItem);

  UpdateDocument (sfp->flightlog, sfp->currItem, sfp->currItem);
  Update ();

  CheckExtendTable (sfp);

  if (StringDoesHaveText (ifp->name)) {
    ifp->warn_on_name_change = TRUE;
  }
}

static void CommonReleaseItemProc (
  IteM i,
  Int4 thousands,
  Int4 hundreds
)

{
  BaseFormPtr    bfp;
  ClockTimePtr   ctp;
  DialoG         d;
  IntFlightPtr   ifp;
  Int4           rel_height;
  SscFlightsPtr  sfp;
  SscTablesPtr   stp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  stp = GetTablesPointer (bfp);
  if (stp == NULL) return;

  d = stp->glider_flights;
  if (! Visible (d)) return;
  if (! AllParentsVisible (d)) return;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;
  ifp = GetFlightInfo (sfp, sfp->currItem);
  if (ifp == NULL) return;

  GetReleaseHeight (sfp->release, &rel_height);

  ctp = &(ifp->takeoff);
  if (ctp != NULL && ctp->hour > 2 && ctp->hour <= 23 &&
      ctp->minute >= 0 && ctp->minute <= 59) {

    if (rel_height > 0) {
      if ((rel_height % 1000) == 0 && hundreds > 0) {
        /* allow increase by 500 feet if hundreds popup not yet set */
      } else {
        if (Message (MSG_YN, "Do you want to override the existing release altitude?") == ANS_NO) return;
      }
    }
  }

  if (thousands == 0 && hundreds != 0) {
    thousands = (rel_height / 1000);
  }
  /*
  if (rel_height > 0 && (rel_height % 1000) == 0 && hundreds > 0) {
    thousands = (rel_height % 1000);
  }
  */

  rel_height = thousands * 1000 + hundreds * 100;
  SetReleaseHeight (sfp->release, rel_height);

  ifp->release_altitude = rel_height;

  RecalculateCosts (sfp, sfp->currItem);

  UpdateDocument (sfp->flightlog, sfp->currItem, sfp->currItem);
  Update ();

  CheckExtendTable (sfp);

  if (StringDoesHaveText (ifp->name)) {
    ifp->warn_on_name_change = TRUE;
  }
}

static void Release0ItemProc (
  IteM i
)

{
  CommonReleaseItemProc (i, 0, 0);
}

static void Release1ItemProc (
  IteM i
)

{
  CommonReleaseItemProc (i, 1, 0);
}

static void Release2ItemProc (
  IteM i
)

{
  CommonReleaseItemProc (i, 2, 0);
}

static void Release3ItemProc (
  IteM i
)

{
  CommonReleaseItemProc (i, 3, 0);
}

static void Release4ItemProc (
  IteM i
)

{
  CommonReleaseItemProc (i, 4, 0);
}

static void Release5ItemProc (
  IteM i
)

{
  CommonReleaseItemProc (i, 0, 5);
}

extern void UpdateFlightControls (
  DialoG d
)

{
  Int2           left, right, offset;
  RecT           r;
  SscFlightsPtr  sfp;
  SscTablesPtr   stp;
  Boolean        was_visible = FALSE;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;

  if ((! stp->members.need_to_update) &&
      (! stp->instructors.need_to_update) &&
      (! stp->towpilots.need_to_update) &&
      (! stp->payers.need_to_update)) return;

  if (Visible (d) && AllParentsVisible (d)) {
    Hide (d);
    Update ();
    was_visible = TRUE;
  }

  if (stp->members.need_to_update) {
    RecreateNameAlist (&(stp->members));
    Reset (sfp->name);
    InitEnumPopup (sfp->name, stp->members.alist, NULL);
    SetValue (sfp->name, 0);
  }

  if (stp->instructors.need_to_update) {
    RecreateNameAlist (&(stp->instructors));
    Reset (sfp->instructor);
    InitEnumPopup (sfp->instructor, stp->instructors.alist, NULL);
    SetValue (sfp->instructor, 0);
  }

  if (stp->towpilots.need_to_update) {
    RecreateNameAlist (&(stp->towpilots));
    Reset (sfp->towpilot);
    InitEnumPopup (sfp->towpilot, stp->towpilots.alist, NULL);
    SetValue (sfp->towpilot, 0);
  }

  if (stp->payers.need_to_update) {
    RecreateNameAlist (&(stp->payers));
    Reset (sfp->alt_payer);
    InitEnumPopup (sfp->alt_payer, stp->payers.alist, NULL);
    SetValue (sfp->alt_payer, 0);
  }

  ChangeFlightTableSelect (sfp->flightlog, sfp->currItem, 1, 1, FALSE);

  GetPosition (sfp->name, &r);
  right = r.right;
  GetPosition (sfp->aftername, &r);
  left = r.left;
  offset = right - left + 10;
  OffsetRect (&r, offset, 0);
  SetPosition (sfp->aftername, &r);

  GetPosition (sfp->name, &r);
  AdjustPrnt (sfp->name, &r, FALSE);
  GetPosition (sfp->aftername, &r);
  AdjustPrnt (sfp->aftername, &r, FALSE);
  GetPosition (sfp->instructor, &r);
  AdjustPrnt (sfp->instructor, &r, FALSE);
  GetPosition (sfp->towpilot, &r);
  AdjustPrnt (sfp->towpilot, &r, FALSE);

  if (was_visible) {
    Show (d);
    Update ();
  }
}

static Int2 expFltPosns [] = {0, 22, 30, 35, 48, 71, 78, 101};
static Char expFltJusts [] = "llrllrlr";

extern void ExportFlightsDialog (
  DialoG d,
  ValNodePtr PNTR head
)

{
  Char            buf [128], tmp [32];
  DoC             doc;
  IntFlightPtr    ifp;
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
    ifp = sfp->glider_flights [k];
    if (ifp == NULL) continue;
    if (StringHasNoText (ifp->name)) continue;
    if (StringHasNoText (ifp->glider)) continue;
    buf [0] = '\0';
    ExportDocCell (doc, k, 2, buf);
    if (ifp->category == 7) {
      FormatCell ("*", buf, 1, expFltPosns, expFltJusts);
    }
    sprintf (tmp, "%4ld", (long) ifp->release_altitude);
    FormatCell (tmp, buf, 2, expFltPosns, expFltJusts);
    ExportDocCell (doc, k, 3, tmp);
    FormatCell (tmp, buf, 3, expFltPosns, expFltJusts);
    StringCpy (tmp, ifp->towplane);
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

extern void ArchiveFlightsDialog (
  DialoG d,
  FILE *fp
)

{
  Char            buf [128], tmp [32];
  IntFlightPtr    ifp;
  Int2            k, numItems;
  SscFlightsPtr   sfp;
  SscTablesPtr    stp;
  TowPlaneSetPtr  tsp;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL || fp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;

  fprintf (fp, "FLIGHT OPERATIONS -----\n\n");
  fprintf (fp, "Flt  Name                Glider       TowPlane         Tkoff");
  fprintf (fp, "   Land   Time     Alt    Rental     Tow     Total\n\n");

  GetDocParams (sfp->flightlog, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    ifp = sfp->glider_flights [k];
    if (ifp == NULL) continue;
    if (StringHasNoText (ifp->name)) continue;
    if (StringHasNoText (ifp->glider)) continue;
    buf [0] = '\0';
    ExportDocCell (sfp->flightlog, k, 1, tmp);
    StringCat (buf, tmp);
    ExportDocCell (sfp->flightlog, k, 2, tmp);
    FormatCell (tmp, buf, 1, acvFltPosns, acvFltJusts);
    ExportDocCell (sfp->flightlog, k, 3, tmp);
    FormatCell (tmp, buf, 2, acvFltPosns, acvFltJusts);
    StringCpy (tmp, ifp->towplane);
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
  if (PrintDollarAmount (&(sfp->tot_rent), tmp)) {
    FormatCell (tmp, buf, 8, acvFltPosns, acvFltJusts);
  }
  if (PrintDollarAmount (&(sfp->tot_tow), tmp)) {
    FormatCell (tmp, buf, 9, acvFltPosns, acvFltJusts);
  }
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
  sfp->recentTowpilot = ValNodeFreeData (sfp->recentTowpilot);
  sfp->recentTowplane = ValNodeFreeData (sfp->recentTowplane);
  sfp->recentGlider = ValNodeFreeData (sfp->recentGlider);
  sfp->recentClass = ValNodeFree (sfp->recentClass);
  sfp->latestName = NULL;
  sfp->latestInst = NULL;
  sfp->latestTowpilot = NULL;
  sfp->latestTowplane = NULL;
  sfp->latestGlider = NULL;
  sfp->latestClass = NULL;
  sfp->changeAfterTakeoff = FALSE;

  ChangeFlightTableSelect (d, item, row, col, dblClick);
}

extern void KeyToFlightsDialog (
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
    sfp->recentTowpilot = ValNodeFreeData (sfp->recentTowpilot);
    sfp->recentTowplane = ValNodeFreeData (sfp->recentTowplane);
    sfp->recentGlider = ValNodeFreeData (sfp->recentGlider);
    sfp->recentClass = ValNodeFree (sfp->recentClass);
    sfp->latestName = NULL;
    sfp->latestInst = NULL;
    sfp->latestTowpilot = NULL;
    sfp->latestTowplane = NULL;
    sfp->latestGlider = NULL;
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

extern void FlightControlsTimerProc (
  DialoG d
)

{
  ClockTimePtr   ctp;
  FlightTimePtr  ftp;
  IntFlightPtr   ifp;
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
    ifp = sfp->glider_flights [k];
    if (ifp == NULL) continue;
    /*
    if (StringHasNoText (ifp->name)) continue;
    if (StringHasNoText (ifp->glider)) continue;
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

  d = stp->glider_flights;
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

  sprintf (buf2, "%2ld", (long) sfp->tot_rent.cents);
  if (buf2 [0] == ' ') {
    buf2 [0] = '0';
  }
  sprintf (buf1, "%ld.%s", (long) sfp->tot_rent.dollars, buf2);
  SetTitle (sfp->ttl_rent, buf1);

  sprintf (buf2, "%2ld", (long) sfp->tot_tow.cents);
  if (buf2 [0] == ' ') {
    buf2 [0] = '0';
  }
  sprintf (buf1, "%ld.%s", (long) sfp->tot_tow.dollars, buf2);
  SetTitle (sfp->ttl_tow, buf1);

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
  GliderFlightPtr  fip, flights;
  FlightTimePtr    ftp;
  Int2             i;
  IntFlightPtr     ifp;
  Int2             k;
  SscFlightsPtr    sfp;
  SscTablesPtr     stp;
  DollarAmount     tot_rent, tot_tow, tot_cost;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return;
  stp = sfp->tables;
  if (stp == NULL) return;
  flights = (GliderFlightPtr) data;

  sfp->recentName = ValNodeFreeData (sfp->recentName);
  sfp->recentInst = ValNodeFreeData (sfp->recentInst);
  sfp->recentTowpilot = ValNodeFreeData (sfp->recentTowpilot);
  sfp->recentTowplane = ValNodeFreeData (sfp->recentTowplane);
  sfp->recentGlider = ValNodeFreeData (sfp->recentGlider);
  sfp->recentClass = ValNodeFree (sfp->recentClass);
  sfp->latestName = NULL;
  sfp->latestInst = NULL;
  sfp->latestTowpilot = NULL;
  sfp->latestTowplane = NULL;
  sfp->latestGlider = NULL;
  sfp->latestClass = NULL;
  sfp->changeAfterTakeoff = FALSE;

  Reset (sfp->flightlog);
  sfp->numFlights = 0;
  sfp->currItem = 1;

  for (k = 0; k < 128; k++) {
    ifp = sfp->glider_flights [k];
    IntFlightInfoFree (ifp);
    sfp->glider_flights [k] = NULL;
  }

  if (flights == NULL) {
    Reset (sfp->flightlog);
    SetValue (sfp->name, 0);
    SetValue (sfp->glider, 0);
    SetTitle (sfp->passenger, "");
    SetValue (sfp->alt_payer, 0);
    SetValue (sfp->split_payment, 0);
    SetValue (sfp->fclass, 0);
    SetValue (sfp->instructor, 0);
    SetValue (sfp->towpilot, 0);
    SetValue (sfp->towplane, 0);
    SetClockTime (sfp->takeoff, -1, -1);
    SetClockTime (sfp->landing, -1, -1);
    SetValue (sfp->exception, 0);
    SetReleaseHeight (sfp->release, -1);
    SetTitle (sfp->ttl_rent, "");
    SetTitle (sfp->ttl_tow, "");
    SetTitle (sfp->ttl_cost, "");
    sfp->tot_rent.dollars = 0;
    sfp->tot_rent.cents = 0;
    sfp->tot_tow.dollars = 0;
    sfp->tot_tow.cents = 0;
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

  tot_rent.dollars = 0;
  tot_rent.cents = 0;
  tot_tow.dollars = 0;
  tot_tow.cents = 0;
  tot_cost.dollars = 0;
  tot_cost.cents = 0;

  for (fip = flights, k = 1; fip != NULL; fip = fip->next, k++) {
    ifp = GetFlightInfo (sfp, k);
    if (ifp == NULL) continue;

    ifp->name = StringSaveForAlist (fip->name);
    AddNameToExtras (&(stp->members), ifp->name);
    ifp->alt_payer = StringSaveForAlist (fip->alt_payer);
    AddNameToExtras (&(stp->payers), ifp->alt_payer);
    ifp->glider = StringSaveForAlist (fip->glider);
    ifp->instructor = StringSaveForAlist (fip->instructor);
    AddNameToExtras (&(stp->instructors), ifp->instructor);
    AddNameToExtras (&(stp->justcfigs), ifp->instructor);
    ifp->towpilot = StringSaveForAlist (fip->towpilot);
    AddNameToExtras (&(stp->towpilots), ifp->towpilot);
    ifp->towplane = StringSaveForAlist (fip->towplane);
    ifp->passenger = StringSaveForAlist (fip->passenger);

    ifp->type = fip->type;
    ifp->split_payment = fip->split_payment;
    ifp->category = fip->category;
    ifp->exception = fip->exception;
    ifp->release_altitude = fip->release_altitude;

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
    dap = fip->tow_cost;
    if (dap != NULL) {
      ifp->tow_cost.dollars = dap->dollars;
      ifp->tow_cost.cents = dap->cents;

      tot_tow.dollars += dap->dollars;
      tot_tow.cents += dap->cents;
      if (tot_tow.cents >= 100) {
        tot_tow.dollars += tot_tow.cents / 100;
        tot_tow.cents = tot_tow.cents % 100;
      }
    }
    dap = fip->total_cost;
    if (dap != NULL) {
      ifp->total_cost.dollars = dap->dollars;
      ifp->total_cost.cents = dap->cents;

      tot_cost.dollars += dap->dollars;
      tot_cost.cents += dap->cents;
      if (tot_cost.cents >= 100) {
        tot_cost.dollars += tot_cost.cents / 100;
        tot_cost.cents = tot_cost.cents % 100;
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

  if (tot_rent.cents < 0) {
    tot_rent.cents = 0;
  }
  sprintf (buf2, "%2ld", (long) tot_rent.cents);
  if (buf2 [0] == ' ') {
    buf2 [0] = '0';
  }
  sprintf (buf1, "%ld.%s", (long) tot_rent.dollars, buf2);
  SetTitle (sfp->ttl_rent, buf1);
  sfp->tot_rent.dollars = tot_rent.dollars;
  sfp->tot_rent.cents = tot_rent.cents;

  if (tot_tow.cents < 0) {
    tot_tow.cents = 0;
  }
  sprintf (buf2, "%2ld", (long) tot_tow.cents);
  if (buf2 [0] == ' ') {
    buf2 [0] = '0';
  }
  sprintf (buf1, "%ld.%s", (long) tot_tow.dollars, buf2);
  SetTitle (sfp->ttl_tow, buf1);
  sfp->tot_tow.dollars = tot_tow.dollars;
  sfp->tot_tow.cents = tot_tow.cents;

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

  RecalculateGlidersAloft (sfp);
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
  /* if (ftp->hours == 0 && ftp->minutes == 0) return FALSE; */

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
  IntFlightPtr     ifp;
  GliderFlightPtr  fip, flights, lastfip;
  Int2             k, numItems;
  SscFlightsPtr    sfp;

  sfp = (SscFlightsPtr) GetObjectExtra (d);
  if (sfp == NULL) return NULL;

  flights = NULL;

  lastfip = NULL;
  GetDocParams (sfp->flightlog, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    ifp = sfp->glider_flights [k];
    if (ifp == NULL) continue;
    if (StringHasNoText (ifp->name)) continue;
    if (StringCmp (ifp->name, "-") == 0) continue;
    if (StringHasNoText (ifp->glider)) continue;
    fip = GliderFlightNew ();
    if (fip == NULL) continue;

    fip->name = StringSaveNotEmpty (ifp->name);
    fip->alt_payer = StringSaveNotEmpty (ifp->alt_payer);
    fip->glider = StringSaveNotEmpty (ifp->glider);
    fip->instructor = StringSaveNotEmpty (ifp->instructor);
    if (StringCmp (fip->instructor, "-") == 0) {
      fip->instructor = MemFree (fip->instructor);
    }
    fip->towpilot = StringSaveNotEmpty (ifp->towpilot);
    if (StringCmp (fip->towpilot, "-") == 0) {
      fip->towpilot = MemFree (fip->towpilot);
    }
    fip->towplane = StringSaveNotEmpty (ifp->towplane);
    fip->passenger = StringSaveNotEmpty (ifp->passenger);

    fip->type = ifp->type;
    fip->split_payment = ifp->split_payment;
    fip->category = ifp->category;
    fip->exception = ifp->exception;
    fip->release_altitude = ifp->release_altitude;

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

    if (DollarAmountOkay (&(ifp->flight_cost))) {
      dap = DollarAmountNew ();
      if (dap != NULL) {
        dap->dollars = ifp->flight_cost.dollars;
        dap->cents = ifp->flight_cost.cents;
        fip->flight_cost = dap;
      }
    }
    if (DollarAmountOkay (&(ifp->tow_cost))) {
      dap = DollarAmountNew ();
      if (dap != NULL) {
        dap->dollars = ifp->tow_cost.dollars;
        dap->cents = ifp->tow_cost.cents;
        fip->tow_cost = dap;
      }
    }
    if (DollarAmountOkay (&(ifp->total_cost))) {
      dap = DollarAmountNew ();
      if (dap != NULL) {
        dap->dollars = ifp->total_cost.dollars;
        dap->cents = ifp->total_cost.cents;
        fip->total_cost = dap;
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

extern void SetupFlightMenus (
  MenU m,
  BaseFormPtr bfp
)

{
#ifdef WIN_MAC
  CommandItem (m, "Takeoff/T", TakeoffItemProc);
  CommandItem (m, "Landing/L", LandingItemProc);
  SeparatorItem (m);
  CommandItem (m, "Release 0/0", Release0ItemProc);
  CommandItem (m, "Release 1000/1", Release1ItemProc);
  CommandItem (m, "Release 2000/2", Release2ItemProc);
  CommandItem (m, "Release 3000/3", Release3ItemProc);
  CommandItem (m, "Release 4000/4", Release4ItemProc);
  CommandItem (m, "Release + 500/5", Release5ItemProc);
  SeparatorItem (m);
  CommandItem (m, "Previous Glider/G", SameGliderProc);
  SeparatorItem (m);
  CommandItem (m, "Previous Pilot Name/N", SameNameProc);
  CommandItem (m, "Previous Instructor/U", SameInstructorProc);
  CommandItem (m, "Previous Tow Pilot/W", SameTowPilotProc);
  CommandItem (m, "Previous Towplane/M", SameTowPlaneProc);
  CommandItem (m, "Previous Class/K", SameClassProc);
  SeparatorItem (m);
  CommandItem (m, "Recalculate Charges", RecalculateAllFlightCosts);
#else
  IteM  i;

  i = CommandItem (m, "Takeoff/  T", TakeoffItemProc);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (m, "Landing/  L", LandingItemProc);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (m);
  i = CommandItem (m, "Release 0/  0", Release0ItemProc);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (m, "Release 1000/  1", Release1ItemProc);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (m, "Release 2000/  2", Release2ItemProc);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (m, "Release 3000/  3", Release3ItemProc);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (m, "Release 4000/  4", Release4ItemProc);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (m, "Release + 500/  5", Release5ItemProc);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (m);
  i = CommandItem (m, "Previous Glider/  G", SameGliderProc);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (m);
  i = CommandItem (m, "Previous Pilot Name/  N", SameNameProc);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (m, "Previous Instructor/  U", SameInstructorProc);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (m, "Previous Tow Pilot/  W", SameTowPilotProc);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (m, "Previous Towplane/  M", SameTowPlaneProc);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (m, "Previous Class/  K", SameClassProc);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (m);
  i = CommandItem (m, "Recalculate Charges", RecalculateAllFlightCosts);
  SetObjectExtra (i, bfp, NULL);
#endif
}

extern DialoG CreateGliderFlightsDialog (
  GrouP prnt,
  SscTablesPtr stp,
  BaseFormPtr bfp
)

{
  GrouP          cts, tbl, g0, g1, g2, g3, g4, g5, gx, p;
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
  flightColFmt [6].pixWidth = StringWidth ("10000 ") + 5;
  flightColFmt [7].pixWidth = StringWidth ("$100.00 ") + 5;
  flightColFmt [8].pixWidth = StringWidth ("$100.00 ") + 5;
  flightColFmt [9].pixWidth = StringWidth ("$100.00 ") + 5;
  SelectFont (systemFont);

  wid = 0;
  for (i = 0; i < 10; i++) {
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
  StaticPrompt (g0, "Glider", flightColFmt [2].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Tkoff", flightColFmt [3].pixWidth, 0, fnt, 'r');
  StaticPrompt (g0, "Land", flightColFmt [4].pixWidth, 0, fnt, 'r');
  StaticPrompt (g0, "Time", flightColFmt [5].pixWidth, 0, fnt, 'r');
  StaticPrompt (g0, "Alt", flightColFmt [6].pixWidth, 0, fnt, 'r');
  StaticPrompt (g0, "Rental", flightColFmt [7].pixWidth, 0, fnt, 'r');
  StaticPrompt (g0, "Tow", flightColFmt [8].pixWidth, 0, fnt, 'r');
  StaticPrompt (g0, "Total", flightColFmt [9].pixWidth, 0, fnt, 'r');

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
  sfp->ttl_rent = StaticPrompt (g5, "", flightColFmt [7].pixWidth, 0, systemFont, 'r');
  sfp->ttl_tow = StaticPrompt (g5, "", flightColFmt [8].pixWidth, 0, systemFont, 'r');
  sfp->ttl_cost = StaticPrompt (g5, "", flightColFmt [9].pixWidth, 0, systemFont, 'r');

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

  StaticPrompt (g4, "Tow Pilot", 0, popupMenuHeight, programFont, 'l');
  sfp->towpilot = CreateEnumPopupDialog (g4, TRUE, ChangeTowPilot, stp->towpilots.alist, (UIEnum) 0, sfp);

  g2 = HiddenGroup (cts, -20, 0, NULL);
  SetGroupSpacing (g2, 10, 5);

  StaticPrompt (g2, "Glider", 0, popupMenuHeight, programFont, 'l');
  sfp->glider = CreateEnumPopupDialog (g2, TRUE, ChangeGlider, stp->glider_alist, (UIEnum) 0, sfp);

  StaticPrompt (g2, "Class", 0, popupMenuHeight, programFont, 'l');
  sfp->fclass = CreateEnumPopupDialog (g2, TRUE, ChangeFclass, fclass_alist, (UIEnum) 0, sfp);

  StaticPrompt (g2, "Tow Plane", 0, popupMenuHeight, programFont, 'l');
  sfp->towplane = CreateEnumPopupDialog (g2, TRUE, ChangeTowPlane, stp->towplane_alist, (UIEnum) 0, sfp);

  StaticPrompt (g2, "Release", 0, popupMenuHeight, programFont, 'l');
  sfp->release = CreateReleaseHeightDialog (g2, (Pointer) sfp, ChangeRelease);

  g3 = HiddenGroup (cts, -20, 0, NULL);
  SetGroupSpacing (g3, 10, 5);

  to = PushButton (g3, "Takeoff", SetTakeoffTime);
  SetObjectExtra (to, sfp, NULL);

  sfp->takeoff = CreateClockTimeDialog (g3, (Pointer) sfp, ChangeTakeoff);

  ld = PushButton (g3, "Landing", SetLandingTime);
  SetObjectExtra (ld, sfp, NULL);

  sfp->landing = CreateClockTimeDialog (g3, (Pointer) sfp, ChangeLanding);

  StaticPrompt (g3, "Exception", 0, popupMenuHeight, programFont, 'l');
  sfp->exception = CreateEnumPopupDialog (g3, TRUE, ChangeExcept, except_alist, (UIEnum) 0, sfp);

  gx = HiddenGroup (cts, -20, 0, NULL);
  SetGroupSpacing (gx, 10, 5);

  StaticPrompt (gx, "Passenger", 0, dialogTextHeight, programFont, 'l');
  str = WidestAlist (stp->members.alist);
  sfp->passenger = DialogText (gx, str, 0, ChangePassenger);
  SetObjectExtra (sfp->passenger, sfp, NULL);
  SetTitle (sfp->passenger, "");

  StaticPrompt (gx, "Alternative Payer", 0, dialogTextHeight, programFont, 'l');
  sfp->alt_payer = CreateEnumPopupDialog (gx, TRUE, ChangePayer, stp->payers.alist, (UIEnum) 0, sfp);

  StaticPrompt (gx, "Pays", 0, popupMenuHeight, programFont, 'l');
  sfp->split_payment = CreateEnumPopupDialog (gx, TRUE, ChangeSplit, ratio_alist, (UIEnum) 0, sfp);
  SetObjectExtra (sfp->split_payment, sfp, NULL);

  AlignObjects (ALIGN_CENTER, (HANDLE) tbl, (HANDLE) cts, NULL);

  sfp->numFlights = 0;
  sfp->currItem = 1;

  return (DialoG) p;
}


