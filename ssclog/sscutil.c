/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Utilities page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

extern Int2 ParseALine (
  CharPtr str,
  CharPtr PNTR array
)

{
  Char     ch;
  Int2     i = 0;
  CharPtr  ptr;

  if (str == NULL || array == NULL) return 0;
  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    while (ch != '\0' && ch != '\t') {
      ptr++;
      ch = *ptr;
    }
    *ptr = '\0';
    array [i] = str;
    i++;
    if (ch == '\t') {
      ptr++;
      ch = *ptr;
      str = ptr;
    }
  }
  return i;
}

extern CharPtr StringSaveNotEmpty (
  CharPtr str
)

{
  if (StringHasNoText (str)) return NULL;
  return StringSave (str);
}

extern CharPtr StringSaveForAlist (
  CharPtr str
)

{
  if (StringHasNoText (str)) return StringSave (" ");
  return StringSave (str);
}

extern void FixNonPrintableChars (
  CharPtr str
)

{
  Char     ch;
  CharPtr  ptr;

  if (StringHasNoText (str)) return;
  ptr = str;
  if (ptr != NULL) {
    /* remove unprintable characters from ASN.1 VisibleString */
    ch = *ptr;
    while (ch != '\0') {
      if (IS_WHITESP (ch)) {
        *ptr = ' ';
      }
      ptr++;
      ch = *ptr;
    }
  }
}

extern Int4 ParseInteger (
  CharPtr str,
  Int4 dfault
)

{
  long int  val;

  if (StringHasNoText (str)) return dfault;

  if (sscanf (str, "%ld", &val) == 1) return val;

  return dfault;
}

extern DollarAmountPtr ParseDollar (
  CharPtr str
)

{
  DollarAmountPtr  dap;
  CharPtr          ptr;
  Char             tmp [64];

  if (StringHasNoText (str)) return NULL;
  dap = DollarAmountNew ();
  if (dap == NULL) return NULL;
  dap->dollars = -1;
  dap->cents = -1;

  StringNCpy_0 (tmp, str, sizeof (tmp));
  ptr = StringChr (tmp, '.');
  if (ptr != NULL) {
    *ptr = '\0';
    ptr++;
    dap->cents = ParseInteger (ptr, -1);
    ptr = tmp;
    if (ptr [0] == '$') {
      ptr++;
    }
    dap->dollars = ParseInteger (ptr, -1);
  }

  return dap;
}

extern ClockTimePtr ParseClock (
  CharPtr str
)

{
  ClockTimePtr  ctp;
  CharPtr       ptr;
  Char          tmp [64];

  if (StringHasNoText (str)) return NULL;
  ctp = ClockTimeNew ();
  if (ctp == NULL) return NULL;
  ctp->hour = -1;
  ctp->minute = -1;

  StringNCpy_0 (tmp, str, sizeof (tmp));
  ptr = StringChr (tmp, ':');
  if (ptr != NULL) {
    *ptr = '\0';
    ptr++;
    ctp->hour = ParseInteger (tmp, -1);
    ctp->minute = ParseInteger (ptr, -1);
  }

  return ctp;
}

extern FlightTimePtr ParseTime (
  CharPtr str
)

{
  FlightTimePtr  ftp;
  CharPtr        ptr;
  Char           tmp [64];

  if (StringHasNoText (str)) return NULL;
  ftp = FlightTimeNew ();
  if (ftp == NULL) return NULL;
  ftp->hours = -1;
  ftp->minutes = -1;

  StringNCpy_0 (tmp, str, sizeof (tmp));
  ptr = StringChr (tmp, ':');
  if (ptr != NULL) {
    *ptr = '\0';
    ptr++;
    ftp->hours = ParseInteger (tmp, -1);
    ftp->minutes = ParseInteger (ptr, -1);
  }

  return ftp;
}

extern CalendarDatePtr ParseDate (
  CharPtr str
)

{
  CalendarDatePtr  cdp;
  CharPtr          ptr;
  Char             tmp [64];

  if (StringHasNoText (str)) return NULL;
  cdp = CalendarDateNew ();
  if (cdp == NULL) return NULL;
  cdp->month = -1;
  cdp->day = -1;
  cdp->year = -1;

  StringNCpy_0 (tmp, str, sizeof (tmp));
  ptr = StringChr (tmp, '/');
  if (ptr != NULL) {
    *ptr = '\0';
    ptr++;
    cdp->month = ParseInteger (tmp, -1);
    str = StringChr (ptr, '/');
    if (str != NULL) {
      *str = '\0';
      str++;
      cdp->day = ParseInteger (ptr, -1);
      cdp->year = ParseInteger (str, -1);
    }
  }

  return cdp;
}

extern Boolean PrintDollarAmount (
  DollarAmountPtr dap,
  CharPtr buf
)

{
  if (dap == NULL || buf == NULL) return FALSE;
  *buf = '\0';

  if (dap->dollars >= 0 && dap->cents >= 0) {
    sprintf (buf, "%4ld.%2ld", (long) dap->dollars, (long) dap->cents);
    if (buf [5] == ' ') {
      buf [5] = '0';
    }
    TrimSpacesAroundString (buf);
    return TRUE;
  }

  return FALSE;
}

extern Boolean PrintWholeDollar (
  DollarAmountPtr dap,
  CharPtr buf
)

{
  CharPtr  ptr;

  if (dap == NULL || buf == NULL) return FALSE;
  *buf = '\0';

  if (dap->dollars >= 0 && dap->cents >= 0) {
    sprintf (buf, "%4ld.%2ld", (long) dap->dollars, (long) dap->cents);
    if (buf [5] == ' ') {
      buf [5] = '0';
    }
    TrimSpacesAroundString (buf);
    ptr = StringStr (buf, ".00");
    if (ptr != NULL) {
      *ptr = '\0';
    }
    return TRUE;
  }

  return FALSE;
}

extern Boolean PrintClockTime (
  ClockTimePtr ctp,
  CharPtr buf
)

{
  if (ctp == NULL || buf == NULL) return FALSE;
  *buf = '\0';

  if (ctp->hour > 2 && ctp->hour <= 24 &&
      ctp->minute >= 0 && ctp->minute <= 59) {
    sprintf (buf, "%2ld:%2ld", (long) ctp->hour, (long) ctp->minute);
    if (buf [3] == ' ') {
      buf [3] = '0';
    }
    return TRUE;
  }

  return FALSE;
}

extern Boolean PrintFlightTime (
  FlightTimePtr ftp,
  CharPtr buf
)

{
  if (ftp == NULL || buf == NULL) return FALSE;
  *buf = '\0';

  if (ftp->hours >= 0 && ftp->minutes >= 0) {
    sprintf (buf, "%2ld:%2ld", (long) ftp->hours, (long) ftp->minutes);
    if (buf [3] == ' ') {
      buf [3] = '0';
    }
    return TRUE;
  }

  return FALSE;
}

static CharPtr month_strings [] = {
  "?", "January", "February", "March", "April", "May", "June",
  "July", "August", "September", "October", "November", "December"
};

static CharPtr weekday_strings [] = {
  "?", "Sunday", "Monday", "Tuesday", "Wednesday",
  "Thursday", "Friday","Saturday"
};

static Int2 in_leap_months_table [] = {
  0, 6, 2, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5
};

static Int2 non_leap_months_table [] = {
  0, 0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5
};

static Int2 days_per_month_table [] = {
  0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static Int2 CalculateDay (
  CalendarDatePtr cdp
)

{
  Int2     century_value, days_per_month, month_value, sum, year_in_century;
  Boolean  is_leap_year;

  if (cdp == NULL) return 0;
  if (cdp->month < 1 || cdp->month > 12) return 0;

  if ((cdp->year % 400) == 0) {
    is_leap_year = TRUE;
  } else if ((cdp->year % 100) == 0) {
    is_leap_year = FALSE;
  } else if ((cdp->year % 4) == 0) {
    is_leap_year = TRUE;
  } else {
    is_leap_year = FALSE;
  }

  days_per_month = days_per_month_table [(int) cdp->month];
  if (cdp->month == 2 && is_leap_year) {
    days_per_month = 29;
  }
  if (cdp->day < 1 || cdp->day > days_per_month) return 0;

  century_value = 6; /* 1752-1799 = 4, 1800s = 2, 1900s = 0, 2000s = 6, 2100s = 4 */
  year_in_century = (cdp->year % 100);

  if (is_leap_year) {
    month_value = in_leap_months_table [(int) cdp->month];
  } else {
    month_value = non_leap_months_table [(int) cdp->month];
  }

  sum = century_value + year_in_century + year_in_century / 4 +
        month_value + cdp->day;

  return (sum % 7) + 1;
}

extern Boolean PrintCalendarDate (
  CalendarDatePtr cdp,
  CharPtr buf,
  Int2 type
)

{
  Int2  weekday;

  if (cdp == NULL || buf == NULL) return FALSE;
  *buf = '\0';

  switch (type) {
    case DATE_FOR_FILE :
      if (cdp->month > 0 && cdp->month <= 12 &&
          cdp->day > 0 && cdp->year > 0) {
        sprintf (buf, "%2ld%2ld%4ld",
                 (long) cdp->month, (long) cdp->day, (long) cdp->year);
        if (buf [0] == ' ') {
          buf [0] = '0';
        }
        if (buf [3] == ' ') {
          buf [3] = '0';
        }
        return TRUE;
      }
      break;
    case DATE_WITH_SLASHES :
      if (cdp->month > 0 && cdp->month <= 12 &&
          cdp->day > 0 && cdp->year > 0) {
        sprintf (buf, "%2ld/%2ld/%4ld",
                 (long) cdp->month, (long) cdp->day, (long) cdp->year);
        if (buf [0] == ' ') {
          buf [0] = '0';
        }
        if (buf [3] == ' ') {
          buf [3] = '0';
        }
        return TRUE;
      }
      break;
    case DATE_FOR_DISPLAY :
      if (cdp->month >= 1 && cdp->month <= 12) {
        sprintf (buf, "%s %ld, %ld",
                 month_strings [ (int) cdp->month],
                 (long) cdp->day, (long) cdp->year);
        return TRUE;
      }
      break;
    case DATE_WITH_DAY :
      if (cdp->month >= 1 && cdp->month <= 12) {
        weekday = CalculateDay (cdp);
        if (weekday >= 1 && weekday <= 7) {
          sprintf (buf, "%s, %s %ld, %ld",
                   weekday_strings [ (int) weekday],
                   month_strings [ (int) cdp->month],
                   (long) cdp->day, (long) cdp->year);
        } else {
          sprintf (buf, "%s %ld, %ld",
                   month_strings [ (int) cdp->month],
                   (long) cdp->day, (long) cdp->year);
        }
        return TRUE;
      }
      break;
    default :
      break;
  }


  return FALSE;
}

extern void ProrateDollarAmount (
  DollarAmountPtr dap,
  Int4 base_month,
  Boolean rate_is_per_year,
  SscTablesPtr stp,
  DollarAmount PNTR result
)

{
  Int4          cents, currMonth;
  DollarAmount  da;

  if (result == NULL) return;

  result->dollars = -1;
  result->cents = -1;

  if (dap == NULL || stp == NULL) return;
  if (base_month < 1 || base_month > 12) return;

  currMonth = stp->currentDate.month;
  if (stp->currentDate.day > 15) {
    currMonth++;
  }

  da.dollars = -1;
  da.cents = -1;

  cents = dap->dollars * 100 + dap->cents;

  cents *= base_month + 12 - currMonth;

  if (rate_is_per_year) {
    cents /= 12;
  }

  da.dollars = cents / 100;
  da.cents = cents % 100;

  if (da.cents > 50 && da.cents < 100) {
    (da.dollars)++;
    da.cents = 0;
  } else {
    da.cents = 0;
  }

  result->dollars = da.dollars;
  result->cents = da.cents;
}

extern void PrintDocCell (
  DoC d,
  Int2 item,
  Int2 col,
  Boolean tab,
  FILE *fp
)

{
  CharPtr  str;

  str = GetDocText (d, item, 1, col);
  if (tab) {
    fprintf (fp, "\t");
  }
  if (StringDoesHaveText (str)) {
    TrimSpacesAroundString (str);
    fprintf (fp, "%s", str);
  }
  MemFree (str);
}

extern void ExportDocCell (
  DoC d,
  Int2 item,
  Int2 col,
  CharPtr buf
)

{
  CharPtr  str;

  str = GetDocText (d, item, 1, col);
  if (StringDoesHaveText (str)) {
    TrimSpacesAroundString (str);
    StringCpy (buf, str);
  }
  MemFree (str);
}

extern void TabToCol (
  CharPtr buf,
  Int2 col
)

{
  size_t   len;
  CharPtr  ptr;

  if (buf == NULL) return;
  len = StringLen (buf);
  if (len >= col) return;

  ptr = buf + len;
  while (len < col) {
    *ptr = ' ';
    ptr++;
    len++;
  }
  *ptr = '\0';
}

extern void FormatCell (
  CharPtr str,
  CharPtr buf,
  Int2 col,
  Int2Ptr posns,
  CharPtr justs
)

{
  Char  just;
  Int2  posn;

  if (StringHasNoText (str)) return;
  if (buf == NULL || posns == NULL || justs == NULL) return;

  posn = posns [col];
  just = justs [col];

  if (just == 'l') {
    TabToCol (buf, posn);
  } else if (just == 'r') {
    TabToCol (buf, posn - StringLen (str));
  }

  StringCat (buf, str);
}

extern FILE* OpenAFile (
  CharPtr filename,
  CharPtr actualpath
)

{
  FILE     *fp;
  Char     path [PATH_MAX];
  CharPtr  ptr;

  if (StringHasNoText (filename)) return NULL;
  if (actualpath != NULL) {
    *actualpath = '\0';
  }

  ProgramPath (path, sizeof (path));
  ptr = StringRChr (path, DIRDELIMCHR);
  if (ptr != NULL) {
    *ptr = '\0';
  }
  FileBuildPath (path, "tables", NULL);
  FileBuildPath (path, NULL, filename);
  fp = FileOpen (path, "r");
  if (fp != NULL) {
    if (actualpath != NULL) {
      StringCpy (actualpath, path);
    }
    return fp;
  }

#ifdef WIN_MAC
  ProgramPath (path, sizeof (path));
  ptr = StringStr (path, "ssc");
  if (ptr != NULL) {
    *ptr = '\0';
  }
  FileBuildPath (path, "ssc", NULL);
  FileBuildPath (path, "tables", NULL);
  FileBuildPath (path, NULL, filename);
  fp = FileOpen (path, "r");
  if (fp != NULL) {
    if (actualpath != NULL) {
      StringCpy (actualpath, path);
    }
    return fp;
  }

  ProgramPath (path, sizeof (path));
  ptr = StringStr (path, "Developer/Xcode");
  if (ptr == NULL) return NULL;
  ptr = StringStr (path, "Xcode");
  if (ptr != NULL) {
    *ptr = '\0';
  }
  FileBuildPath (path, "tables", NULL);
  FileBuildPath (path, NULL, filename);
  fp = FileOpen (path, "r");
  if (fp != NULL) {
    if (actualpath != NULL) {
      StringCpy (actualpath, path);
    }
    return fp;
  }
#endif

  return NULL;
}

extern Boolean NormalizeAuthorCaps (
  CharPtr buf
)

{
  MsgAnswer  ans;
  Char       ch;
  Char       chfirst;
  Char       chsecond;
  Char       first [64];
  Char       last [64];
  Boolean    is_first;
  Boolean    is_second;
  Boolean    is_third;
  Boolean    make_lower_case;
  Boolean    make_upper_case;
  CharPtr    ptr;
  CharPtr    str;
  Char       tmp [128];

  StringNCpy_0 (tmp, buf, sizeof (tmp));

  ptr = StringChr (tmp, ',');
  if (ptr != NULL) {
    str = ptr + 1;
    ch = *str;
    while (IS_WHITESP (ch)) {
      str++;
      ch = *str;
    }
    if (StringCmp (str, "Jr") == 0 ||
        StringCmp (str, "Jr.") == 0 ||
        StringCmp (str, "Sr") == 0 ||
        StringCmp (str, "Sr.") == 0 ||
        StringCmp (str, "II") == 0 ||
        StringCmp (str, "II.") == 0 ||
        StringCmp (str, "III") == 0 ||
        StringCmp (str, "III.") == 0 ||
        StringCmp (str, "IV") == 0 ||
        StringCmp (str, "IV.") == 0) {
      *ptr = ' ';
    } else {
      *ptr = '\0';
      StringNCpy_0 (first, str, sizeof (first));
      StringNCpy_0 (last, tmp, sizeof (last));
      TrimSpacesAroundString (first);
      TrimSpacesAroundString (last);
      sprintf (tmp, "%s %s", first, last);
    }
  }

  ptr = tmp;
  ch = *ptr;
  make_lower_case = FALSE;
  make_upper_case = TRUE;

  while (ch != '\0') {
    if (IS_UPPER (ch)) {
      if (make_lower_case) {
        *ptr = TO_LOWER (ch);
      }
      make_lower_case = TRUE;
      make_upper_case = FALSE;
    } else if (IS_LOWER (ch)) {
      if (make_upper_case) {
        *ptr = TO_UPPER (ch);
      }
      make_lower_case = TRUE;
      make_upper_case = FALSE;
    } else if (IS_WHITESP (ch)) {
      make_lower_case = FALSE;
      make_upper_case = TRUE;
    }

    ptr++;
    ch = *ptr;
  }

  ptr = tmp;
  ch = *ptr;
  is_first = TRUE;
  is_second = FALSE;
  is_third = FALSE;
  chfirst = '\0';
  chsecond = '\0';

  while (ch != '\0') {
    if (IS_WHITESP (ch)) {
      is_first = TRUE;
      is_second = FALSE;
      is_third = FALSE;
      chfirst = '\0';
      chsecond = '\0';
    } else if (is_first) {
      chfirst = ch;
      is_first = FALSE;
      is_second = TRUE;
      is_third = FALSE;
    } else if (is_second) {
      chsecond = ch;
      is_first = FALSE;
      is_second = FALSE;
      is_third = TRUE;
    } else if (is_third) {
      if (chfirst == 'M' && chsecond == 'c') {
        *ptr = TO_UPPER (ch);
      } else if (chfirst == 'O' && chsecond == '\'') {
        *ptr = TO_UPPER (ch);
      }
      is_first = FALSE;
      is_second = FALSE;
      is_third = FALSE;
      chfirst = '\0';
      chsecond = '\0';
    }

    ptr++;
    ch = *ptr;
  }

  if (StringCmp (buf, tmp) != 0) {
    ans = Message (MSG_YNC, "Capitalize as '%s'?", tmp);
    if (ans == ANS_CANCEL) return FALSE;
    if (ans == ANS_YES) {
      StringCpy (buf, tmp);
    }
  }

  return TRUE;
}

extern CharPtr FindLastName (
  CharPtr str
)

{
  Char     ch;
  CharPtr  last, prev, ptr;

  if (StringHasNoText (str)) return str;

  ptr = str;
  last = ptr;
  prev = ptr;

  ch = *ptr;
  while (ch != '\0') {
    if (ch == ' ') {
      prev = last;
      last = ptr;
    }
    ptr++;
    ch = *ptr;
  }

  if (StringCmp (last, " Jr") == 0 ||
      StringCmp (last, " Jr.") == 0 ||
      StringCmp (last, " Sr") == 0 ||
      StringCmp (last, " Sr.") == 0 ||
      StringCmp (last, " II") == 0 ||
      StringCmp (last, " II.") == 0 ||
      StringCmp (last, " III") == 0 ||
      StringCmp (last, " III.") == 0 ||
      StringCmp (last, " IV") == 0 ||
      StringCmp (last, " IV.") == 0) {
    last = prev;
  }

  if (*last == ' ') {
    last++;
  }
  return last;
}

static int LIBCALLBACK SortNameList (
  VoidPtr vp1, VoidPtr vp2
)

{
  int         compare;
  CharPtr     name1, name2;
  CharPtr     str1 = NULL, str2 = NULL;
  ValNodePtr  vnp1, vnp2;

  if (vp1 == NULL || vp2 == NULL) return 0;
  vnp1 = *((ValNodePtr PNTR) vp1);
  vnp2 = *((ValNodePtr PNTR) vp2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;

  if (vnp1->choice > vnp2->choice) {
    return 1;
  } else if (vnp1->choice < vnp2->choice) {
    return -1;
  }

  name1 = (CharPtr) vnp1->data.ptrvalue;
  name2 = (CharPtr) vnp2->data.ptrvalue;
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

  return 0;
}

static ValNodePtr ValNodeStrListDup (
  ValNodePtr vnp
)

{
  ValNodePtr  head = NULL, ths, last = NULL;

  if (vnp == NULL) return NULL;
  while (vnp != NULL) {
    ths = ValNodeNew (NULL);
    if (head == NULL) {
      head = ths;
    }
    if (last != NULL) {
      last->next = ths;
    }
    if (ths != NULL) {
      ths->choice = vnp->choice;
      ths->data.ptrvalue = StringSaveNoNull (vnp->data.ptrvalue);
    }
    last = ths;
    vnp = vnp->next;
  }
  return head;
}

extern Boolean ReadMemberTable (
  CharPtr filename,
  SscTablesPtr stp
)

{
  CharPtr     array [256];
  Int2        count;
  ValNodePtr  demopilots = NULL, examiners = NULL, head, last = NULL, vnp;
  FileCache   fc;
  FILE        *fp;
  CharPtr     job, name, str;
  Char        line [512];

  if (stp == NULL) return FALSE;
  fp = OpenAFile (filename, NULL);
  if (fp == NULL) return FALSE;
  FileCacheSetup (&fc, fp);

  vnp = ValNodeCopyStr (&last, 0, " ");
  stp->members.names = vnp;
  last = vnp;

  ValNodeCopyStr (&(stp->instructors.names), 0, " ");
  ValNodeCopyStr (&(stp->justcfigs.names), 0, " ");
  ValNodeCopyStr (&(stp->towpilots.names), 0, " ");
  ValNodeCopyStr (&(stp->dutyofficers.names), 0, " ");
  ValNodeCopyStr (&(stp->assistants.names), 0, " ");

  str = FileCacheGetString (&fc, line, sizeof (line));
  while (str != NULL) {
    TrimSpacesAroundString (str);
    if (StringDoesHaveText (str) && str [0] != '-') {
      job = NULL;
      count = ParseALine (line, array);
      if (count >= 2) {
        job = array [1];
      }
      if (count >= 1) {
        name = array [0];
        TrimSpacesAroundString (name);
        if (job == NULL || (StringChr (job, 'S') == NULL && StringChr (job, 'P') == NULL)) {
          vnp = ValNodeCopyStr (&last, 1, name);
          last = vnp;
        }
        if (job != NULL) {
          if (StringChr (job, 'I')) {
            ValNodeCopyStr (&(stp->instructors.names), 1, name);
            ValNodeCopyStr (&(stp->justcfigs.names), 1, name);
          } else if (StringChr (job, 'X')) {
            ValNodeCopyStr (&examiners, 5, name);
          }
          if (StringChr (job, 'T')) {
            ValNodeCopyStr (&(stp->towpilots.names), 1, name);
          }
          if (StringChr (job, 'C')) {
            ValNodeCopyStr (&demopilots, 3, name);
          }
          if (StringChr (job, 'D')) {
            ValNodeCopyStr (&(stp->dutyofficers.names), 1, name);
          }
          if (StringChr (job, 'A')) {
            ValNodeCopyStr (&(stp->assistants.names), 1, name);
          }
          if (StringChr (job, 'P')) {
            ValNodeCopyStr (&(stp->members.extras), 1, name);
          }
        }
      }
    }

    str = FileCacheGetString (&fc, line, sizeof (line));
  }

  stp->members.names = ValNodeSort (stp->members.names, SortNameList);
  stp->members.extras = ValNodeSort (stp->members.extras, SortNameList);
  head = ValNodeStrListDup (stp->members.names);
  if (stp->members.extras != NULL) {
    vnp = ValNodeStrListDup (stp->members.extras);
    ValNodeCopyStr (&head, 6, "-");
    ValNodeLink (&head, vnp);
  }
  stp->members.alist = MakeEnumFieldAlistFromValNodeList (head);
  ValNodeFreeData (head);

  stp->justfull.names = ValNodeStrListDup (stp->members.names);
  stp->justfull.names = ValNodeSort (stp->justfull.names, SortNameList);
  stp->justfull.alist = MakeEnumFieldAlistFromValNodeList (stp->justfull.names);

  stp->instructors.names = ValNodeSort (stp->instructors.names, SortNameList);
  if (demopilots != NULL) {
    ValNodeCopyStr (&demopilots, 2, "-");
    demopilots = ValNodeSort (demopilots, SortNameList);
    ValNodeLink (&stp->instructors.names, demopilots);
  }
  if (examiners != NULL) {
    ValNodeCopyStr (&examiners, 4, "-");
    examiners = ValNodeSort (examiners, SortNameList);
    ValNodeLink (&stp->instructors.names, examiners);
  }
  stp->instructors.alist = MakeEnumFieldAlistFromValNodeList (stp->instructors.names);

  stp->justcfigs.names = ValNodeSort (stp->justcfigs.names, SortNameList);
  stp->justcfigs.alist = MakeEnumFieldAlistFromValNodeList (stp->justcfigs.names);

  stp->towpilots.names = ValNodeSort (stp->towpilots.names, SortNameList);
  stp->towpilots.alist = MakeEnumFieldAlistFromValNodeList (stp->towpilots.names);

  stp->dutyofficers.names = ValNodeSort (stp->dutyofficers.names, SortNameList);
  stp->dutyofficers.alist = MakeEnumFieldAlistFromValNodeList (stp->dutyofficers.names);

  stp->assistants.names = ValNodeSort (stp->assistants.names, SortNameList);
  stp->assistants.alist = MakeEnumFieldAlistFromValNodeList (stp->assistants.names);

  stp->payers.names = ValNodeStrListDup (stp->members.names);
  stp->payers.names = ValNodeSort (stp->payers.names, SortNameList);
  stp->payers.extras = ValNodeStrListDup (stp->members.extras);
  stp->payers.extras = ValNodeSort (stp->payers.extras, SortNameList);
  head = ValNodeStrListDup (stp->payers.names);
  if (stp->payers.extras != NULL) {
    vnp = ValNodeStrListDup (stp->payers.extras);
    ValNodeCopyStr (&head, 6, "-");
    ValNodeLink (&head, vnp);
  }
  stp->payers.alist = MakeEnumFieldAlistFromValNodeList (head);
  ValNodeFreeData (head);

  FileClose (fp);

  return TRUE;
}

static EnumFieldAssocPtr CreateAlistFromAirportList (
  AirportListPtr table
)

{
  AirportInfoPtr     aip;
  EnumFieldAssocPtr  alist;
  ValNodePtr         head, last = NULL, vnp;
  CharPtr            str;

  if (table == NULL) return NULL;

  vnp = ValNodeCopyStr (&last, 0, " ");
  head = vnp;
  last = vnp;

  for (aip = table; aip != NULL; aip = aip->next) {
    str = aip->airport;
    if (StringHasNoText (str)) continue;
    vnp = ValNodeCopyStr (&last, 0, str);

    if (head == NULL) {
      head = vnp;
    }
    last = vnp;
  }

  alist = MakeEnumFieldAlistFromValNodeList (head);
  ValNodeFreeData (head);

  return alist;
}

extern Boolean ReadAirportTable (
  CharPtr filename,
  SscTablesPtr stp
)

{
  AirportInfoPtr  aip, last = NULL;
  FileCache       fc;
  FILE            *fp;
  AirportListPtr  head = NULL;
  Char            line [512];
  CharPtr         ptr;
  CharPtr         str;

  if (stp == NULL) return FALSE;
  fp = OpenAFile (filename, NULL);
  if (fp == NULL) return FALSE;
  FileCacheSetup (&fc, fp);

  str = FileCacheGetString (&fc, line, sizeof (line));
  while (str != NULL) {
    TrimSpacesAroundString (str);
    if (StringDoesHaveText (str) && str [0] != '-') {
      aip = (AirportInfoPtr) MemNew (sizeof (AirportInfo));
      if (aip != NULL) {
        ptr = StringChr (str, '\t');
        if (ptr != NULL) {
          *ptr = '\0';
          ptr++;
        }
        aip->airport = StringSave (str);
        str = ptr;
        if (str != NULL) {
          ptr = StringChr (str, '\t');
          if (ptr != NULL) {
            *ptr = '\0';
            ptr++;
          }
          aip->code = StringSaveNoNull (str);
          str = ptr;
          if (str != NULL) {
            ptr = StringChr (str, '\t');
            if (ptr != NULL) {
              *ptr = '\0';
              ptr++;
            }
            aip->suffix = StringSaveNoNull (str);
            if (ptr != NULL) {
              aip->altitude = ParseInteger (ptr, -1);
            }
          }
        }
        if (head == NULL) {
          head = aip;
        }
        if (last != NULL) {
          last->next = aip;
        }
        last = aip;
      }
    }

    str = FileCacheGetString (&fc, line, sizeof (line));
  }

  stp->field_list = head;
  stp->field_alist = CreateAlistFromAirportList (stp->field_list);

  FileClose (fp);

  return TRUE;
}

static EnumFieldAssocPtr CreateAlistFromGliderTable (
  GliderTablePtr table
)

{
  EnumFieldAssocPtr  alist;
  GliderInfoPtr      gip;
  ValNodePtr         head, last = NULL, vnp;

  if (table == NULL) return NULL;

  vnp = ValNodeCopyStr (&last, 0, " ");
  head = vnp;
  last = vnp;

  for (gip = table; gip != NULL; gip = gip->next) {
    vnp = ValNodeCopyStr (&last, 0, gip->glider);

    if (head == NULL) {
      head = vnp;
    }
    last = vnp;
  }

  alist = MakeEnumFieldAlistFromValNodeList (head);
  ValNodeFreeData (head);

  return alist;
}

/*
glider name, registration, rental per minute, rental per flight, number of seats, max time charged per day, fly for free owners
*/

extern Boolean ReadGliderTable (
  CharPtr filename,
  SscTablesPtr stp
)

{
  CharPtr         array [32];
  Int2            count, i;
  FileCache       fc;
  FILE            *fp;
  GliderInfoPtr   gip, last = NULL;
  GliderTablePtr  head = NULL;
  Char            line [512];
  CharPtr         str;

  if (stp == NULL) return FALSE;
  fp = OpenAFile (filename, NULL);
  if (fp == NULL) return FALSE;
  FileCacheSetup (&fc, fp);

  str = FileCacheGetString (&fc, line, sizeof (line));
  while (str != NULL) {
    TrimSpacesAroundString (str);
    if (StringDoesHaveText (str) && str [0] != '-') {
      MemSet ((Pointer) array, 0, sizeof (array));
      count = ParseALine (line, array);
      if (count >= 1) {
        gip = GliderInfoNew ();
        if (gip != NULL) {
          gip->glider = StringSave (array [0]);
          TrimSpacesAroundString (gip->glider);
          if (count >= 2) {
            gip->tail_number = StringSaveNotEmpty (array [1]);
          }
          if (count >= 3) {
            gip->cost_per_min = ParseDollar (array [2]);
          }
          if (count >= 4) {
            gip->cost_per_flight = ParseDollar (array [3]);
          }
          if (count >= 5) {
            gip->number_of_seats = ParseInteger (array [4], -1);
          }
          if (count >= 6) {
            gip->max_rental_time = ParseTime (array [5]);
          }
          for (i = 6; i < count; i++) {
            if (StringDoesHaveText (array [i])) {
              ValNodeCopyStr (&(gip->fly_for_free), 0, array [i]);
            }
          }
          if (head == NULL) {
            head = gip;
          }
          if (last != NULL) {
            last->next = gip;
          }
          last = gip;
        }
      }
    }

    str = FileCacheGetString (&fc, line, sizeof (line));
  }

  stp->glider_table = head;
  stp->glider_alist = CreateAlistFromGliderTable (stp->glider_table);

  FileClose (fp);

  return TRUE;
}

static EnumFieldAssocPtr CreateAlistFromTowplaneTable (
  TowPlaneSetPtr table
)

{
  EnumFieldAssocPtr  alist;
  ValNodePtr         head, last = NULL, vnp;
  TowPlaneListPtr    tlp;

  if (table == NULL) return NULL;

  vnp = ValNodeCopyStr (&last, 0, " ");
  head = vnp;
  last = vnp;

  for (tlp = table; tlp != NULL; tlp = tlp->next) {
    vnp = ValNodeCopyStr (&last, 0, tlp->towplane);

    if (head == NULL) {
      head = vnp;
    }
    last = vnp;
  }

  alist = MakeEnumFieldAlistFromValNodeList (head);
  ValNodeFreeData (head);

  return alist;
}

extern Boolean ReadTowRateTable (
  CharPtr filename,
  SscTablesPtr stp
)

{
  CharPtr          array [256];
  Int2             count;
  FileCache        fc;
  FILE             *fp;
  Char             line [512];
  CharPtr          str;
  TowPlaneListPtr  tlp = NULL, lasttlp = NULL;
  TowRateItemPtr   trp, lasttrp = NULL;
  TowPlaneSetPtr   tsp = NULL;

  if (stp == NULL) return FALSE;
  fp = OpenAFile (filename, NULL);
  if (fp == NULL) return FALSE;
  FileCacheSetup (&fc, fp);

  str = FileCacheGetString (&fc, line, sizeof (line));
  while (str != NULL) {
    if (StringDoesHaveText (str) && str [0] != '-') {
      count = ParseALine (line, array);
      if (count >= 1 && StringDoesHaveText (array [0])) {
        tlp = TowPlaneListNew ();
        if (tlp != NULL) {
          TrimSpacesAroundString (array [0]);
          tlp->towplane = StringSave (array [0]);
          if (StringDoesHaveText (array [1])) {
            TrimSpacesAroundString (array [1]);
            tlp->tail_number = StringSave (array [1]);
          }
          tlp->rates = NULL;
          lasttrp = NULL;
          if (tsp == NULL) {
            tsp = tlp;
          }
          if (lasttlp != NULL) {
            lasttlp->next = tlp;
          }
          lasttlp = tlp;
        }
      } else if (count >= 3 && StringHasNoText (array [0])) {
        trp = TowRateItemNew ();
        if (trp != NULL) {
          TrimSpacesAroundString (array [1]);
          trp->altitude = ParseInteger (array [1], -1);
          TrimSpacesAroundString (array [2]);
          trp->cost_of_tow = ParseDollar (array [2]);
          if (tlp != NULL && tlp->rates == NULL) {
            tlp->rates = trp;
          }
          if (lasttrp != NULL) {
            lasttrp->next = trp;
          }
          lasttrp = trp;
        }
      }
    }

    str = FileCacheGetString (&fc, line, sizeof (line));
  }

  stp->towset_table = tsp;
  stp->towplane_alist = CreateAlistFromTowplaneTable (stp->towset_table);

  if (tsp != NULL) {
    stp->towrate_table = tsp->rates;
  }

  FileClose (fp);

  return TRUE;
}

extern Boolean ReadMiscRateTable (
  CharPtr filename,
  SscTablesPtr stp
)

{
  CharPtr      array [256];
  Int2         count;
  FileCache    fc;
  FILE         *fp;
  Int2         k;
  Char         line [512];
  MiscRatePtr  mrp = NULL;
  CharPtr      str;

  if (stp == NULL) return FALSE;
  fp = OpenAFile (filename, NULL);
  if (fp == NULL) return FALSE;
  FileCacheSetup (&fc, fp);

  mrp = MiscRateNew ();
  if (mrp == NULL) return FALSE;

  k = 0;
  str = FileCacheGetString (&fc, line, sizeof (line));
  while (str != NULL) {
    TrimSpacesAroundString (str);
    if (StringDoesHaveText (str) && str [0] != '-') {
      k++;
      count = ParseALine (line, array);
      if (count >= 1) {
        switch (k) {
          case 1 :
            mrp->year = ParseInteger (array [0], -1);
            break;
          case 2 :
            mrp->initiation_fee = ParseDollar (array [0]);
            break;
          case 3 :
            mrp->monthly_dues = ParseDollar (array [0]);
            break;
          case 4 :
            mrp->ssa_regular = ParseDollar (array [0]);
            break;
          case 5 :
            mrp->ssa_family = ParseDollar (array [0]);
            break;
          case 6 :
            mrp->intro_memb_days = ParseInteger (array [0], -1);
            break;
          case 7 :
            mrp->fast_fee = ParseDollar (array [0]);
            break;
          case 8 :
            mrp->fast_extra = ParseDollar (array [0]);
            break;
          case 9 :
            mrp->fast_list = StringSaveNotEmpty (array [0]);
            break;
          case 10 :
            mrp->intro_fee = ParseDollar (array [0]);
            break;
          case 11 :
            mrp->intro_extra = ParseDollar (array [0]);
            break;
          case 12 :
            mrp->intro_list = StringSaveNotEmpty (array [0]);
            break;
          case 13 :
            mrp->special_memb_days = ParseInteger (array [0], -1);
            break;
          case 14 :
            mrp->visiting_fee = ParseDollar (array [0]);
            break;
          case 15 :
            mrp->affiliated_fee = ParseDollar (array [0]);
            break;
          case 16 :
            mrp->transient_memb_days = ParseInteger (array [0], -1);
            break;
          case 17 :
            mrp->transient_fee = ParseDollar (array [0]);
            break;
          case 18 :
            mrp->retrieve_cost_per_min = ParseDollar (array [0]);
            break;
          case 19 :
            mrp->minimum_retrieve_cost = ParseDollar (array [0]);
            break;
          default :
            break;
        }
      }
    }

    str = FileCacheGetString (&fc, line, sizeof (line));
  }

  stp->miscrate_table = mrp;

  FileClose (fp);

  if (mrp->year < stp->workingDate.year) {
    Message (MSG_OK, "Miscellaneous Rate Table appears to be out of date");
  }

  return TRUE;
}

extern Boolean ReadFieldTable (
  CharPtr filename,
  SscTablesPtr stp
)

{
  CharPtr       array [256];
  Int2          count;
  FileCache     fc;
  FILE          *fp;
  Int2          k;
  Char          line [512];
  CharPtr       ptr;
  CharPtr       str;
  CharPtr       tmp;

  if (stp == NULL) return FALSE;
  fp = OpenAFile (filename, NULL);
  if (fp == NULL) return FALSE;
  FileCacheSetup (&fc, fp);

  k = 0;
  str = FileCacheGetString (&fc, line, sizeof (line));
  while (str != NULL) {
    TrimSpacesAroundString (str);
    if (StringDoesHaveText (str) && str [0] != '-') {
      k++;
      count = ParseALine (line, array);
      if (count >= 1) {
        switch (k) {
          case 1 :
            stp->club = StringSave (array [0]);
            break;
          case 2 :
            stp->airport = StringSave (array [0]);
            break;
          case 3 :
            stp->traconname = StringSave (array [0]);
            break;
          case 4 :
            stp->traconphone = StringSave (array [0]);
            break;
          case 5 :
            tmp = StringSave (array [0]);
            ptr = tmp;
            if (StringNICmp (ptr, "http://", 7) == 0) {
              ptr += 7;
            }
            ptr = StringChr (ptr, '/');
            if (ptr != NULL) {
              stp->submission_path = StringSave (ptr);
              *ptr = '\0';
              stp->submission_host = StringSave (tmp);
            }
            MemFree (tmp);
            break;
          default :
            break;
        }
      }
    }

    str = FileCacheGetString (&fc, line, sizeof (line));
  }

  FileClose (fp);

  return TRUE;
}

extern Boolean ReadTempMemberTable (
  CharPtr filename,
  SscTablesPtr stp,
  CalendarDatePtr currdate
)

{
  ValNodePtr       alltempmembs = NULL;
  CharPtr          array [256];
  CalendarDatePtr  cdp;
  Int2             count;
  Boolean          expired;
  FileCache        fc;
  FILE             *fp;
  Char             line [512];
  CharPtr          name, str;
  MemberInfoPtr    tmp, last = NULL, head = NULL, next, PNTR prev;

  if (stp == NULL || currdate == NULL) return FALSE;
  fp = OpenAFile (filename, NULL);
  if (fp == NULL) return FALSE;
  FileCacheSetup (&fc, fp);

  str = FileCacheGetString (&fc, line, sizeof (line));
  while (str != NULL) {
    TrimSpacesAroundString (str);
    if (StringDoesHaveText (str) && str [0] != '-') {
      count = ParseALine (line, array);
      if (count >= 2) {
        name = array [0];
        TrimSpacesAroundString (name);
        if (StringDoesHaveText (name)) {
          cdp = ParseDate (array [1]);
          expired = FALSE;
          if (cdp != NULL) {
            if (cdp->year < currdate->year) {
              expired = TRUE;
            } else if (cdp->year == currdate->year) {
              if (cdp->month < currdate->month) {
                expired = TRUE;
              } else if (cdp->month == currdate->month) {
                if (cdp->day < currdate->day) {
                  expired = TRUE;
                }
              }
            }
          }
          if (cdp != NULL) {
            if (cdp->year == currdate->year) {
              ValNodeCopyStr (&alltempmembs, 0, name);
            } else if (cdp->year == currdate->year + 1) {
              ValNodeCopyStr (&alltempmembs, 0, name);
            }
          }
          if (! expired) {
            tmp = MemberInfoNew ();
            if (tmp != NULL) {
              tmp->name = StringSave (name);
              tmp->expires = cdp;
              if (head == NULL) {
                head = tmp;
              }
              if (last != NULL) {
                last->next = tmp;
              }
              last = tmp;
            }
          } else {
            cdp = CalendarDateFree (cdp);
          }
        }
      }
    }

    str = FileCacheGetString (&fc, line, sizeof (line));
  }

  stp->intro_member_table =  head;

  FileClose (fp);

  alltempmembs = ValNodeSort (alltempmembs, SortNameList);
  stp->intro_member_this_year = alltempmembs;

  /* use last entry if person was extended before original period expired */

  if (head != NULL && head->next != NULL) {
    for (last = head; last != NULL; last = last->next) {
      for (tmp = last->next; tmp != NULL; tmp = tmp->next) {
        if (StringICmp (last->name, tmp->name) == 0) {
          last->name = MemFree (last->name);
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

  return TRUE;
}

extern ValNodePtr ReadPayersTable (
  CharPtr filename
)

{
  ValNodePtr  head = NULL, last = NULL, vnp;
  FileCache   fc;
  FILE        *fp;
  Char        line [512];
  CharPtr     str;

  fp = OpenAFile (filename, NULL);
  if (fp == NULL) return NULL;
  FileCacheSetup (&fc, fp);

  str = FileCacheGetString (&fc, line, sizeof (line));
  while (str != NULL) {
    TrimSpacesAroundString (str);
    if (StringDoesHaveText (str) && str [0] != '-') {
      vnp = ValNodeCopyStr (&last, 1, str);
      if (head == NULL) {
        head = vnp;
      }
      last = vnp;
    }
    str = FileCacheGetString (&fc, line, sizeof (line));
  }

  FileClose (fp);

  return head;
}

extern void AppendTempMemberTable (
  CharPtr filename,
  CharPtr name,
  CalendarDatePtr expires
)

{
  FILE  *fp;
  Char  path [PATH_MAX];
  Char  str [32];

  if (StringHasNoText (name) || expires == NULL) return;
  fp = OpenAFile (filename, path);
  if (fp == NULL) return;
  FileClose (fp);

  fp = FileOpen (path, "a");
  if (fp == NULL) return;

  if (PrintCalendarDate (expires, str, DATE_WITH_SLASHES)) {
    fprintf (fp, "%s\t%s\n", name, str);
  }

  FileClose (fp);
}

extern void AppendFullMemberTable (
  CharPtr filename,
  CharPtr name,
  CharPtr jobs
)

{
  FILE  *fp;
  Char  path [PATH_MAX];

  if (StringHasNoText (name)) return;
  fp = OpenAFile (filename, path);
  if (fp == NULL) return;
  FileClose (fp);

  fp = FileOpen (path, "a");
  if (fp == NULL) return;

  fprintf (fp, "%s\t%s\n", name, jobs);

  FileClose (fp);
}

extern void RecreateNameAlist (
  NameListPtr  nlp
)

{
  ValNodePtr  head, vnp;

  if (nlp == NULL) return;

  head = ValNodeStrListDup (nlp->names);
  head = ValNodeSort (head, SortNameList);

  vnp = ValNodeStrListDup (nlp->extras);
  if (vnp != NULL) {
    vnp = ValNodeSort (vnp, SortNameList);
    ValNodeCopyStr (&head, 6, "-");
    ValNodeLink (&head, vnp);
  }

  nlp->alist = FreeEnumFieldAlist (nlp->alist);
  nlp->alist = MakeEnumFieldAlistFromValNodeList (head);
  ValNodeFreeData (head);
}

extern Boolean AddNameToList (
  NameListPtr  nlp,
  CharPtr name
)

{
  EnumFieldAssocPtr  al;
  Int2               i;
  CharPtr            str;
  ValNodePtr         vnp;

  if (nlp == NULL || StringHasNoText (name)) return FALSE;

  al = nlp->alist;
  if (al == NULL) return FALSE;

  for (i = 1; al->name != NULL; i++, al++) {
    if (StringICmp (al->name, name) == 0) return FALSE;
  }

  for (vnp = nlp->extras; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    if (StringICmp (str, name) == 0) return FALSE;
  }

  ValNodeCopyStr (&(nlp->names), 1, name);

  nlp->need_to_update = TRUE;

  return TRUE;
}

extern Boolean AddNameToExtras (
  NameListPtr  nlp,
  CharPtr name
)

{
  EnumFieldAssocPtr  al;
  Int2               i;
  CharPtr            str;
  ValNodePtr         vnp;

  if (nlp == NULL || StringHasNoText (name)) return FALSE;

  al = nlp->alist;
  if (al == NULL) return FALSE;

  for (i = 1; al->name != NULL; i++, al++) {
    if (StringICmp (al->name, name) == 0) return FALSE;
  }

  for (vnp = nlp->extras; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    if (StringICmp (str, name) == 0) return FALSE;
  }

  ValNodeCopyStr (&(nlp->extras), 7, name);

  nlp->need_to_update = TRUE;

  return TRUE;
}

extern Int4 GetMemberType (
  CharPtr name,
  SscTablesPtr stp
)

{
  MemberInfoPtr  mip;
  CharPtr        str;
  Int4           type = 0;
  ValNodePtr     vnp;

  if (StringHasNoText (name) || stp == NULL) return 0;

  /* look in intro member list first */

  for (mip = stp->intro_member_table; mip != NULL; mip = mip->next) {
    str = (CharPtr) mip->name;
    if (StringICmp (name, str) == 0) {
      type = 2;
    }
  }

  /* then look in full member list */

  for (vnp = stp->justfull.names; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    if (StringICmp (name, str) == 0) {
      type = 1;
    }
  }

  return type;
}

static int LIBCALLBACK SortCharges (
  VoidPtr vp1,
  VoidPtr vp2
)

{
  int              compare;
  PaymentInfoPtr   pip1, pip2;
  CharPtr          str1 = NULL, str2 = NULL;
  ValNodePtr       vnp1, vnp2;

  if (vp1 == NULL || vp2 == NULL) return 0;
  vnp1 = *((ValNodePtr PNTR) vp1);
  vnp2 = *((ValNodePtr PNTR) vp2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;

  pip1 = (PaymentInfoPtr) vnp1->data.ptrvalue;
  pip2 = (PaymentInfoPtr) vnp2->data.ptrvalue;
  if (pip1 == NULL || pip2 == NULL) return 0;

  str1 = FindLastName (pip1->payer);
  str2 = FindLastName (pip2->payer);

  compare = StringICmp (str1, str2);

  if (compare > 0) {
    return 1;
  } else if (compare < 0) {
    return -1;
  }

  str1 = pip1->payer;
  str2 = pip2->payer;

  compare = StringICmp (str1, str2);

  if (compare > 0) {
    return 1;
  } else if (compare < 0) {
    return -1;
  }

  if (pip2->check_payment == NULL && pip2->check_payment == NULL) {
    return 1;
  } else if (pip1->check_payment == NULL && pip1->check_payment == NULL) {
    return -1;
  }

  return 0;
}

static void AddCharge (
  ValNodePtr PNTR head,
  CharPtr name,
  Int4 type,
  Int4 dollars,
  Int4 cents
)

{
  DollarAmountPtr  dap;
  Boolean          keep = FALSE;
  PaymentInfoPtr   pip;

  if (StringHasNoText (name)) return;

  pip = PaymentInfoNew ();
  if (pip == NULL) return;
  pip->payer = StringSave (name);
  pip->type = type;

  if (dollars > 0 || cents > 0) {
    dap = pip->charges;
    if (dap == NULL) {
      dap = DollarAmountNew ();
      pip->charges = dap;
    }
    if (dap != NULL) {
      dap->dollars = dollars;
      dap->cents = cents;
      keep = TRUE;
    }
  }

  if (! keep) {
    pip = PaymentInfoFree (pip);
    return;
  }

  ValNodeAddPointer (head, 0, (Pointer) pip);
}

static void AddPayment (
  ValNodePtr PNTR head,
  CharPtr name,
  Int4 type,
  DollarAmountPtr check_payment,
  DollarAmountPtr cash_payment,
  DollarAmountPtr account_payment,
  DollarAmountPtr voucher_payment,
  CharPtr voucher_serial,
  CharPtr comment
)

{
  DollarAmountPtr  dap;
  Boolean          keep = FALSE;
  PaymentInfoPtr   pip;

  if (StringHasNoText (name)) return;

  pip = PaymentInfoNew ();
  if (pip == NULL) return;
  pip->payer = StringSave (name);
  pip->type = type;

  if (check_payment != NULL &&
      (check_payment->dollars > 0 || check_payment->cents > 0)) {
    dap = pip->check_payment;
    if (dap == NULL) {
      dap = DollarAmountNew ();
      pip->check_payment = dap;
    }
    if (dap != NULL) {
      dap->dollars = check_payment->dollars;
      dap->cents = check_payment->cents;
      keep = TRUE;
    }
  }

  if (cash_payment != NULL &&
      (cash_payment->dollars > 0 || cash_payment->cents > 0)) {
    dap = pip->cash_payment;
    if (dap == NULL) {
      dap = DollarAmountNew ();
      pip->cash_payment = dap;
    }
    if (dap != NULL && cash_payment != NULL) {
      dap->dollars = cash_payment->dollars;
      dap->cents = cash_payment->cents;
      keep = TRUE;
    }
  }

  if (account_payment != NULL &&
      (account_payment->dollars > 0 || account_payment->cents > 0)) {
    dap = pip->account_payment;
    if (dap == NULL) {
      dap = DollarAmountNew ();
      pip->account_payment = dap;
    }
    if (dap != NULL && account_payment != NULL) {
      dap->dollars = account_payment->dollars;
      dap->cents = account_payment->cents;
      keep = TRUE;
    }
  }

  if (voucher_payment != NULL &&
      (voucher_payment->dollars > 0 || voucher_payment->cents > 0)) {
    dap = pip->voucher_payment;
    if (dap == NULL) {
      dap = DollarAmountNew ();
      pip->voucher_payment = dap;
    }
    if (dap != NULL && voucher_payment != NULL) {
      dap->dollars = voucher_payment->dollars;
      dap->cents = voucher_payment->cents;
      keep = TRUE;
    }
  }

  pip->voucher_serial = StringSaveNotEmpty (voucher_serial);
  pip->comment = StringSaveNotEmpty (comment);

  if (! keep) {
    pip = PaymentInfoFree (pip);
    return;
  }

  ValNodeAddPointer (head, 0, (Pointer) pip);
}

static void AddRecalcPayment (
  ValNodePtr PNTR head,
  CharPtr name,
  Int4 type,
  Int4 dollars,
  Int4 cents,
  DollarAmountPtr check_payment,
  DollarAmountPtr cash_payment,
  DollarAmountPtr account_payment,
  DollarAmountPtr voucher_payment,
  CharPtr voucher_serial,
  CharPtr comment
)

{
  DollarAmountPtr  dap;
  Boolean          keep = FALSE;
  PaymentInfoPtr   pip;

  if (StringHasNoText (name)) return;

  pip = PaymentInfoNew ();
  if (pip == NULL) return;
  pip->payer = StringSave (name);
  pip->type = type;

  if (dollars > 0 || cents > 0) {
    dap = pip->charges;
    if (dap == NULL) {
      dap = DollarAmountNew ();
      pip->charges = dap;
    }
    if (dap != NULL) {
      dap->dollars = dollars;
      dap->cents = cents;
      keep = TRUE;
    }
  }

  if (check_payment != NULL &&
      (check_payment->dollars > 0 || check_payment->cents > 0)) {
    dap = pip->check_payment;
    if (dap == NULL) {
      dap = DollarAmountNew ();
      pip->check_payment = dap;
    }
    if (dap != NULL) {
      dap->dollars = check_payment->dollars;
      dap->cents = check_payment->cents;
      keep = TRUE;
    }
  }

  if (cash_payment != NULL &&
      (cash_payment->dollars > 0 || cash_payment->cents > 0)) {
    dap = pip->cash_payment;
    if (dap == NULL) {
      dap = DollarAmountNew ();
      pip->cash_payment = dap;
    }
    if (dap != NULL) {
      dap->dollars = cash_payment->dollars;
      dap->cents = cash_payment->cents;
      keep = TRUE;
    }
  }

  if (account_payment != NULL &&
      (account_payment->dollars > 0 || account_payment->cents > 0)) {
    dap = pip->account_payment;
    if (dap == NULL) {
      dap = DollarAmountNew ();
      pip->account_payment = dap;
    }
    if (dap != NULL) {
      dap->dollars = account_payment->dollars;
      dap->cents = account_payment->cents;
      keep = TRUE;
    }
  }

  if (voucher_payment != NULL &&
      (voucher_payment->dollars > 0 || voucher_payment->cents > 0)) {
    dap = pip->voucher_payment;
    if (dap == NULL) {
      dap = DollarAmountNew ();
      pip->voucher_payment = dap;
    }
    if (dap != NULL && voucher_payment != NULL) {
      dap->dollars = voucher_payment->dollars;
      dap->cents = voucher_payment->cents;
      keep = TRUE;
    }
  }

  pip->voucher_serial = StringSaveNotEmpty (voucher_serial);
  pip->comment = StringSaveNotEmpty (comment);

  if (! keep) {
    pip = PaymentInfoFree (pip);
    return;
  }

  ValNodeAddPointer (head, 0, (Pointer) pip);
}

static ValNodePtr FreeVnpChgList (
  ValNodePtr head
)

{
  PaymentInfoPtr  pip;
  ValNodePtr      vnp;

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    pip = (PaymentInfoPtr) vnp->data.ptrvalue;
    if (pip == NULL) continue;
    PaymentInfoFree (pip);
  }

  return ValNodeFree (head);
}

extern void RecalculateCharges (
  DialoG sfpflights,
  DialoG sfpfees,
  DialoG sfpcharges,
  DialoG sfpadjustments,
  DialoG sfppayments
)

{
  AdjustDataPtr    adjustments, fap;
  Int4             cents, dollars, split, tot_cents, tot_dollars, type;
  ChargeDataPtr    charges, mcp;
  ValNodePtr       chghead, sumhead, vnp, nxt;
  CharPtr          comment, last, voucher_serial;
  Int2             currItem;
  DollarAmountPtr  dap, check_payment, cash_payment, account_payment, voucher_payment;
  FeeDataPtr       fees, fdp;
  GliderFlightPtr  fip, flights;
  DoC              paymentlog;
  PaymentInfoPtr   pip, pipn, payments, lastpay;

  chghead = NULL;

  flights = DialogToPointer (sfpflights);

  for (fip = flights; fip != NULL; fip = fip->next) {
    dap = fip->total_cost;
    if (dap == NULL) continue;
    if (dap->dollars <= 0 && dap->cents <= 0) continue;

    dollars = dap->dollars;
    cents = dap->cents;
    if (StringDoesHaveText (fip->alt_payer)) {
      split = fip->split_payment;
      if (split > 0 && StringDoesHaveText (fip->name)) {
        switch (split) {
          case 1:
            AddCharge (&chghead, fip->alt_payer, fip->type, dollars, cents);
            break;
          case 2 :
            cents /= 2;
            if ((dollars % 2) == 1) {
              cents += 50;
            }
            dollars /= 2;
            AddCharge (&chghead, fip->alt_payer, fip->type, dollars, cents);
            AddCharge (&chghead, fip->name, fip->type, dollars, cents);
            break;
          case 3 :
            dap = fip->flight_cost;
            if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
              AddCharge (&chghead, fip->alt_payer, fip->type, dap->dollars, dap->cents);
            }
            dap = fip->tow_cost;
            if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
              AddCharge (&chghead, fip->name, fip->type, dap->dollars, dap->cents);
            }
            break;
          case 4 :
            dap = fip->tow_cost;
            if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
              AddCharge (&chghead, fip->alt_payer, fip->type, dap->dollars, dap->cents);
            }
            dap = fip->flight_cost;
            if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
              AddCharge (&chghead, fip->name, fip->type, dap->dollars, dap->cents);
            }
            break;
          default :
            AddCharge (&chghead, fip->alt_payer, fip->type, dollars, cents);
            break;
        }
      } else {
        AddCharge (&chghead, fip->alt_payer, fip->type, dollars, cents);
      }
    } else {
      AddCharge (&chghead, fip->name, fip->type, dollars, cents);
    }
  }

  flights = FlightListFree (flights);

  fees = DialogToPointer (sfpfees);

  for (fdp = fees; fdp != NULL; fdp = fdp->next) {
    dap = fdp->amount;
    if (dap == NULL || dap->dollars < 0 || dap->cents < 0) continue;

    dollars = dap->dollars;
    cents = dap->cents;
    if (StringDoesHaveText (fdp->alt_payer)) {
      AddCharge (&chghead, fdp->alt_payer, fdp->type, dollars, cents);
    } else {
      AddCharge (&chghead, fdp->name, fdp->type, dollars, cents);
    }
  }

  fees = FeeListFree (fees);

  charges = DialogToPointer (sfpcharges);

  for (mcp = charges; mcp != NULL; mcp = mcp->next) {
    dap = mcp->amount;
    if (dap == NULL || dap->dollars < 0 || dap->cents < 0) continue;

    dollars = dap->dollars;
    cents = dap->cents;
    if (StringDoesHaveText (mcp->alt_payer)) {
      AddCharge (&chghead, mcp->alt_payer, mcp->type, dollars, cents);
    } else {
      AddCharge (&chghead, mcp->name, mcp->type, dollars, cents);
    }
  }

  charges = ChargeListFree (charges);

  adjustments = DialogToPointer (sfpadjustments);

  for (fap = adjustments; fap != NULL; fap = fap->next) {
    dap = fap->amount;
    if (dap == NULL || dap->dollars < 0 || dap->cents < 0) continue;

    dollars = dap->dollars;
    cents = dap->cents;
    if (StringDoesHaveText (fap->alt_payer)) {
      AddCharge (&chghead, fap->alt_payer, fap->type, dollars, cents);
    } else {
      AddCharge (&chghead, fap->name, fap->type, dollars, cents);
    }
  }

  adjustments = AdjustmentListFree (adjustments);

  paymentlog = GetPaymentLogDoc (sfppayments);
  currItem = GetPaymentCurrItem (sfppayments);

  payments = DialogToPointer (sfppayments);

  SafeHide (paymentlog);
  Update ();

  for (pip = payments; pip != NULL; pip = pip->next) {
    AddPayment (&chghead, pip->payer, pip->type, pip->check_payment,
                pip->cash_payment, pip->account_payment, pip->voucher_payment,
                pip->voucher_serial, pip->comment);
  }

  payments = PaymentListFree (payments);

  chghead = ValNodeSort (chghead, SortCharges);

  sumhead = NULL;
  last = NULL;
  type = 0;
  tot_cents = 0;
  tot_dollars = 0;
  check_payment = NULL;
  cash_payment = NULL;
  account_payment = NULL;
  voucher_payment = NULL;
  voucher_serial = NULL;
  comment = NULL;

  for (vnp = chghead; vnp != NULL; vnp = vnp->next) {
    pip = (PaymentInfoPtr) vnp->data.ptrvalue;
    if (pip == NULL || StringHasNoText (pip->payer)) continue;

    if (last != NULL && StringICmp (last, pip->payer) != 0) {

      AddRecalcPayment (&sumhead, last, type, tot_dollars, tot_cents, check_payment,
                        cash_payment, account_payment, voucher_payment, voucher_serial,
                        comment);

      type = 0;
      tot_cents = 0;
      tot_dollars = 0;
      check_payment = NULL;
      cash_payment = NULL;
      account_payment = NULL;
      voucher_payment = NULL;
      voucher_serial = NULL;
      comment = NULL;
    }
    last = pip->payer;
    if (type == 0) {
      type = pip->type;
    }

    dap = pip->charges;
    if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
      tot_dollars += dap->dollars;
      tot_cents += dap->cents;
      if (tot_cents >= 100) {
        tot_dollars += tot_cents / 100;
        tot_cents = tot_cents % 100;
      }
    }
    dap = pip->check_payment;
    if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
      check_payment = dap;
    }
    dap = pip->cash_payment;
    if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
      cash_payment = dap;
    }
    dap = pip->account_payment;
    if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
      account_payment = dap;
    }
    dap = pip->voucher_payment;
    if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
      voucher_payment = dap;
    }
    if (StringDoesHaveText (pip->voucher_serial)) {
      voucher_serial = pip->voucher_serial;
    }
    if (StringDoesHaveText (pip->comment)) {
      comment = pip->comment;
    }
  }

  AddRecalcPayment (&sumhead, last, type, tot_dollars, tot_cents, check_payment,
                    cash_payment, account_payment, voucher_payment, voucher_serial,
                    comment);

  chghead = FreeVnpChgList (chghead);

  sumhead = ValNodeSort (sumhead, SortCharges);

  for (vnp = sumhead; vnp != NULL; vnp = vnp->next) {
    pip = (PaymentInfoPtr) vnp->data.ptrvalue;
    if (pip == NULL) continue;
    nxt = vnp->next;
    if (nxt != NULL) {
      pipn = (PaymentInfoPtr) nxt->data.ptrvalue;
      if (pipn == NULL) continue;
      if (StringICmp (pip->payer, pipn->payer) == 0) {

        dap = pip->charges;
        if (dap != NULL) {
          if (dap->dollars <= 0 && dap->cents <= 0) {
            dap = pipn->check_payment;
            if (dap == NULL || (dap->dollars <= 0 &&  dap->cents <= 0)) {
              dap = pipn->cash_payment;
              if (dap == NULL || (dap->dollars <= 0 &&  dap->cents <= 0)) {
                dap = pipn->account_payment;
                if (dap == NULL || (dap->dollars <= 0 &&  dap->cents <= 0)) {
                  dap = pipn->voucher_payment;
                  if (dap == NULL || (dap->dollars <= 0 &&  dap->cents <= 0)) continue;
                }
              }
            }
          }
        }

        dollars = 0;
        cents = 0;

        dap = pipn->check_payment;
        if (dap != NULL) {
          if (dap->dollars >= 0 && dap->cents >= 0) {
            dollars = dap->dollars;
            cents = dap->cents;
          }
        }

        dap = pip->check_payment;
        if (dap == NULL) {
          dap = DollarAmountNew ();
          pip->check_payment = dap;
        }
        if (dap != NULL) {
          dap->dollars = dollars;
          dap->cents = cents;
        }

        dollars = 0;
        cents = 0;

        dap = pipn->cash_payment;
        if (dap != NULL) {
          if (dap->dollars >= 0 && dap->cents >= 0) {
            dollars = dap->dollars;
            cents = dap->cents;
          }
        }

        dap = pip->cash_payment;
        if (dap == NULL) {
          dap = DollarAmountNew ();
          pip->cash_payment = dap;
        }
        if (dap != NULL) {
          dap->dollars = dollars;
          dap->cents = cents;
        }

        dollars = 0;
        cents = 0;

        dap = pipn->account_payment;
        if (dap != NULL) {
          if (dap->dollars >= 0 && dap->cents >= 0) {
            dollars = dap->dollars;
            cents = dap->cents;
          }
        }

        dap = pip->account_payment;
        if (dap == NULL) {
          dap = DollarAmountNew ();
          pip->account_payment = dap;
        }
        if (dap != NULL) {
          dap->dollars = dollars;
          dap->cents = cents;
        }

        dollars = 0;
        cents = 0;

        dap = pipn->voucher_payment;
        if (dap != NULL) {
          if (dap->dollars >= 0 && dap->cents >= 0) {
            dollars = dap->dollars;
            cents = dap->cents;
          }
        }

        dap = pip->voucher_payment;
        if (dap == NULL) {
          dap = DollarAmountNew ();
          pip->voucher_payment = dap;
        }
        if (dap != NULL) {
          dap->dollars = dollars;
          dap->cents = cents;
        }

        if (pip->type == 0) {
          pip->type = pipn->type;
        }

        vnp->next = nxt->next;
        nxt->next = NULL;
        PaymentInfoFree (pipn);
        ValNodeFree (nxt);
      }
    }
  }

  payments = NULL;
  lastpay = NULL;

  for (vnp = sumhead; vnp != NULL; vnp = vnp->next) {
    pip = (PaymentInfoPtr) vnp->data.ptrvalue;
    if (pip == NULL) continue;
    if (payments == NULL) {
      payments = pip;
    }
    if (lastpay != NULL) {
      lastpay->next = pip;
    }
    lastpay = pip;
  }

  PointerToDialog (sfppayments, payments);

  SetPaymentCurrItem (sfppayments, currItem);

  payments = PaymentListFree (payments);
  sumhead = ValNodeFree (sumhead);

  SafeShow (paymentlog);
  Update ();
}

extern void ClearDirtyFlags (
  SscTablesPtr stp
)

{
  if (stp == NULL) return;
  stp->members.need_to_update = FALSE;
  stp->justfull.need_to_update = FALSE;
  stp->instructors.need_to_update = FALSE;
  stp->justcfigs.need_to_update = FALSE;
  stp->towpilots.need_to_update = FALSE;
  stp->dutyofficers.need_to_update = FALSE;
  stp->assistants.need_to_update = FALSE;
  stp->payers.need_to_update = FALSE;
}

extern void SetDirtyFlag (
  SscTablesPtr stp
)

{
  if (stp == NULL) return;
  stp->need_to_update = TRUE;
  stp->need_to_auto_save = TRUE;
  stp->auto_delay = 180;
}

