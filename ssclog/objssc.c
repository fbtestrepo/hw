#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <objssc.h>

static Boolean loaded = FALSE;

#include <asnssc.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
objsscAsnLoad(void)
{

   if ( ! loaded) {
      NLM_EXTERN_LOADS

      if ( ! AsnLoad ())
      return FALSE;
      loaded = TRUE;
   }

   return TRUE;
}



/**************************************************
*    Generated object loaders for Module SSC-General
*    Generated using ASNCODE Revision: 6.17 at Jul 4, 2011  9:18 PM
*
**************************************************/


/**************************************************
*
*    CalendarDateNew()
*
**************************************************/
NLM_EXTERN 
CalendarDatePtr LIBCALL
CalendarDateNew(void)
{
   CalendarDatePtr ptr = MemNew((size_t) sizeof(CalendarDate));

   return ptr;

}


/**************************************************
*
*    CalendarDateFree()
*
**************************************************/
NLM_EXTERN 
CalendarDatePtr LIBCALL
CalendarDateFree(CalendarDatePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    CalendarDateAsnRead()
*
**************************************************/
NLM_EXTERN 
CalendarDatePtr LIBCALL
CalendarDateAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CalendarDatePtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CalendarDate ::= (self contained) */
      atp = AsnReadId(aip, amp, CALENDARDATE);
   } else {
      atp = AsnLinkType(orig, CALENDARDATE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = CalendarDateNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CALENDARDATE_month) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> month = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CALENDARDATE_day) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> day = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CALENDARDATE_year) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> year = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CALENDARDATE_weekday) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> weekday = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = CalendarDateFree(ptr);
   goto ret;
}



/**************************************************
*
*    CalendarDateAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CalendarDateAsnWrite(CalendarDatePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CALENDARDATE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> month;
   retval = AsnWrite(aip, CALENDARDATE_month,  &av);
   av.intvalue = ptr -> day;
   retval = AsnWrite(aip, CALENDARDATE_day,  &av);
   av.intvalue = ptr -> year;
   retval = AsnWrite(aip, CALENDARDATE_year,  &av);
   if (ptr -> weekday || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> weekday;
      retval = AsnWrite(aip, CALENDARDATE_weekday,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    ClockTimeNew()
*
**************************************************/
NLM_EXTERN 
ClockTimePtr LIBCALL
ClockTimeNew(void)
{
   ClockTimePtr ptr = MemNew((size_t) sizeof(ClockTime));

   return ptr;

}


/**************************************************
*
*    ClockTimeFree()
*
**************************************************/
NLM_EXTERN 
ClockTimePtr LIBCALL
ClockTimeFree(ClockTimePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    ClockTimeAsnRead()
*
**************************************************/
NLM_EXTERN 
ClockTimePtr LIBCALL
ClockTimeAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ClockTimePtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ClockTime ::= (self contained) */
      atp = AsnReadId(aip, amp, CLOCKTIME);
   } else {
      atp = AsnLinkType(orig, CLOCKTIME);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ClockTimeNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CLOCKTIME_hour) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> hour = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CLOCKTIME_minute) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> minute = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = ClockTimeFree(ptr);
   goto ret;
}



/**************************************************
*
*    ClockTimeAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ClockTimeAsnWrite(ClockTimePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CLOCKTIME);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> hour;
   retval = AsnWrite(aip, CLOCKTIME_hour,  &av);
   av.intvalue = ptr -> minute;
   retval = AsnWrite(aip, CLOCKTIME_minute,  &av);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    FlightTimeNew()
*
**************************************************/
NLM_EXTERN 
FlightTimePtr LIBCALL
FlightTimeNew(void)
{
   FlightTimePtr ptr = MemNew((size_t) sizeof(FlightTime));

   return ptr;

}


/**************************************************
*
*    FlightTimeFree()
*
**************************************************/
NLM_EXTERN 
FlightTimePtr LIBCALL
FlightTimeFree(FlightTimePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    FlightTimeAsnRead()
*
**************************************************/
NLM_EXTERN 
FlightTimePtr LIBCALL
FlightTimeAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   FlightTimePtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* FlightTime ::= (self contained) */
      atp = AsnReadId(aip, amp, FLIGHTTIME);
   } else {
      atp = AsnLinkType(orig, FLIGHTTIME);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = FlightTimeNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == FLIGHTTIME_hours) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> hours = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == FLIGHTTIME_minutes) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> minutes = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = FlightTimeFree(ptr);
   goto ret;
}



/**************************************************
*
*    FlightTimeAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
FlightTimeAsnWrite(FlightTimePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, FLIGHTTIME);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> hours;
   retval = AsnWrite(aip, FLIGHTTIME_hours,  &av);
   av.intvalue = ptr -> minutes;
   retval = AsnWrite(aip, FLIGHTTIME_minutes,  &av);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    DollarAmountNew()
*
**************************************************/
NLM_EXTERN 
DollarAmountPtr LIBCALL
DollarAmountNew(void)
{
   DollarAmountPtr ptr = MemNew((size_t) sizeof(DollarAmount));

   return ptr;

}


/**************************************************
*
*    DollarAmountFree()
*
**************************************************/
NLM_EXTERN 
DollarAmountPtr LIBCALL
DollarAmountFree(DollarAmountPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    DollarAmountAsnRead()
*
**************************************************/
NLM_EXTERN 
DollarAmountPtr LIBCALL
DollarAmountAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   DollarAmountPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* DollarAmount ::= (self contained) */
      atp = AsnReadId(aip, amp, DOLLARAMOUNT);
   } else {
      atp = AsnLinkType(orig, DOLLARAMOUNT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = DollarAmountNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == DOLLARAMOUNT_dollars) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> dollars = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DOLLARAMOUNT_cents) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> cents = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = DollarAmountFree(ptr);
   goto ret;
}



/**************************************************
*
*    DollarAmountAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
DollarAmountAsnWrite(DollarAmountPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, DOLLARAMOUNT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> dollars;
   retval = AsnWrite(aip, DOLLARAMOUNT_dollars,  &av);
   av.intvalue = ptr -> cents;
   retval = AsnWrite(aip, DOLLARAMOUNT_cents,  &av);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    FullNameNew()
*
**************************************************/
NLM_EXTERN 
FullNamePtr LIBCALL
FullNameNew(void)
{
   FullNamePtr ptr = MemNew((size_t) sizeof(FullName));

   return ptr;

}


/**************************************************
*
*    FullNameFree()
*
**************************************************/
NLM_EXTERN 
FullNamePtr LIBCALL
FullNameFree(FullNamePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> last_name);
   MemFree(ptr -> first_name);
   MemFree(ptr -> middle_initials);
   MemFree(ptr -> suffix);
   return MemFree(ptr);
}


/**************************************************
*
*    FullNameAsnRead()
*
**************************************************/
NLM_EXTERN 
FullNamePtr LIBCALL
FullNameAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   FullNamePtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* FullName ::= (self contained) */
      atp = AsnReadId(aip, amp, FULLNAME);
   } else {
      atp = AsnLinkType(orig, FULLNAME);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = FullNameNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == FULLNAME_last_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> last_name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == FULLNAME_first_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> first_name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == FULLNAME_middle_initials) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> middle_initials = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == FULLNAME_suffix) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> suffix = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = FullNameFree(ptr);
   goto ret;
}



/**************************************************
*
*    FullNameAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
FullNameAsnWrite(FullNamePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, FULLNAME);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> last_name != NULL) {
      av.ptrvalue = ptr -> last_name;
      retval = AsnWrite(aip, FULLNAME_last_name,  &av);
   }
   if (ptr -> first_name != NULL) {
      av.ptrvalue = ptr -> first_name;
      retval = AsnWrite(aip, FULLNAME_first_name,  &av);
   }
   if (ptr -> middle_initials != NULL) {
      av.ptrvalue = ptr -> middle_initials;
      retval = AsnWrite(aip, FULLNAME_middle_initials,  &av);
   }
   if (ptr -> suffix != NULL) {
      av.ptrvalue = ptr -> suffix;
      retval = AsnWrite(aip, FULLNAME_suffix,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    ClubJobsNew()
*
**************************************************/
NLM_EXTERN 
ClubJobsPtr LIBCALL
ClubJobsNew(void)
{
   ClubJobsPtr ptr = MemNew((size_t) sizeof(ClubJobs));

   return ptr;

}


/**************************************************
*
*    ClubJobsFree()
*
**************************************************/
NLM_EXTERN 
ClubJobsPtr LIBCALL
ClubJobsFree(ClubJobsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    ClubJobsAsnRead()
*
**************************************************/
NLM_EXTERN 
ClubJobsPtr LIBCALL
ClubJobsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ClubJobsPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ClubJobs ::= (self contained) */
      atp = AsnReadId(aip, amp, CLUBJOBS);
   } else {
      atp = AsnLinkType(orig, CLUBJOBS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ClubJobsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CLUBJOBS_fly_glider) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> fly_glider = av.boolvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CLUBJOBS_instructor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> instructor = av.boolvalue;
      ptr -> OBbits__ |= 1<<1;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CLUBJOBS_tow_pilot) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> tow_pilot = av.boolvalue;
      ptr -> OBbits__ |= 1<<2;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CLUBJOBS_demo_pilot) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> demo_pilot = av.boolvalue;
      ptr -> OBbits__ |= 1<<3;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CLUBJOBS_duty_officer) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> duty_officer = av.boolvalue;
      ptr -> OBbits__ |= 1<<4;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CLUBJOBS_assistant_d_o) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> assistant_d_o = av.boolvalue;
      ptr -> OBbits__ |= 1<<5;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CLUBJOBS_examiner) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> examiner = av.boolvalue;
      ptr -> OBbits__ |= 1<<6;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CLUBJOBS_payer) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> payer = av.boolvalue;
      ptr -> OBbits__ |= 1<<7;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = ClubJobsFree(ptr);
   goto ret;
}



/**************************************************
*
*    ClubJobsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ClubJobsAsnWrite(ClubJobsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CLUBJOBS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> fly_glider || (ptr -> OBbits__ & (1<<0) )){   av.boolvalue = ptr -> fly_glider;
      retval = AsnWrite(aip, CLUBJOBS_fly_glider,  &av);
   }
   if (ptr -> instructor || (ptr -> OBbits__ & (1<<1) )){   av.boolvalue = ptr -> instructor;
      retval = AsnWrite(aip, CLUBJOBS_instructor,  &av);
   }
   if (ptr -> tow_pilot || (ptr -> OBbits__ & (1<<2) )){   av.boolvalue = ptr -> tow_pilot;
      retval = AsnWrite(aip, CLUBJOBS_tow_pilot,  &av);
   }
   if (ptr -> demo_pilot || (ptr -> OBbits__ & (1<<3) )){   av.boolvalue = ptr -> demo_pilot;
      retval = AsnWrite(aip, CLUBJOBS_demo_pilot,  &av);
   }
   if (ptr -> duty_officer || (ptr -> OBbits__ & (1<<4) )){   av.boolvalue = ptr -> duty_officer;
      retval = AsnWrite(aip, CLUBJOBS_duty_officer,  &av);
   }
   if (ptr -> assistant_d_o || (ptr -> OBbits__ & (1<<5) )){   av.boolvalue = ptr -> assistant_d_o;
      retval = AsnWrite(aip, CLUBJOBS_assistant_d_o,  &av);
   }
   if (ptr -> examiner || (ptr -> OBbits__ & (1<<6) )){   av.boolvalue = ptr -> examiner;
      retval = AsnWrite(aip, CLUBJOBS_examiner,  &av);
   }
   if (ptr -> payer || (ptr -> OBbits__ & (1<<7) )){   av.boolvalue = ptr -> payer;
      retval = AsnWrite(aip, CLUBJOBS_payer,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    MemberInfoNew()
*
**************************************************/
NLM_EXTERN 
MemberInfoPtr LIBCALL
MemberInfoNew(void)
{
   MemberInfoPtr ptr = MemNew((size_t) sizeof(MemberInfo));

   return ptr;

}


/**************************************************
*
*    MemberInfoFree()
*
**************************************************/
NLM_EXTERN 
MemberInfoPtr LIBCALL
MemberInfoFree(MemberInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   ClubJobsFree(ptr -> jobs);
   CalendarDateFree(ptr -> expires);
   return MemFree(ptr);
}


/**************************************************
*
*    MemberInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
MemberInfoPtr LIBCALL
MemberInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MemberInfoPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MemberInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, MEMBERINFO);
   } else {
      atp = AsnLinkType(orig, MEMBERINFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MemberInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MEMBERINFO_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MEMBERINFO_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MEMBERINFO_jobs) {
      ptr -> jobs = ClubJobsAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MEMBERINFO_expires) {
      ptr -> expires = CalendarDateAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = MemberInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    MemberInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MemberInfoAsnWrite(MemberInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MEMBERINFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, MEMBERINFO_name,  &av);
   }
   if (ptr -> type || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> type;
      retval = AsnWrite(aip, MEMBERINFO_type,  &av);
   }
   if (ptr -> jobs != NULL) {
      if ( ! ClubJobsAsnWrite(ptr -> jobs, aip, MEMBERINFO_jobs)) {
         goto erret;
      }
   }
   if (ptr -> expires != NULL) {
      if ( ! CalendarDateAsnWrite(ptr -> expires, aip, MEMBERINFO_expires)) {
         goto erret;
      }
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    MemberListFree()
*
**************************************************/
NLM_EXTERN 
MemberListPtr LIBCALL
MemberListFree(MemberListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) MemberInfoFree);
   return NULL;
}


/**************************************************
*
*    MemberListAsnRead()
*
**************************************************/
NLM_EXTERN 
MemberListPtr LIBCALL
MemberListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MemberListPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MemberList ::= (self contained) */
      atp = AsnReadId(aip, amp, MEMBERLIST);
   } else {
      atp = AsnLinkType(orig, MEMBERLIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MemberInfoAsnRead, (AsnOptFreeFunc) MemberInfoFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = MemberListFree(ptr);
   goto ret;
}



/**************************************************
*
*    MemberListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MemberListAsnWrite(MemberListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MEMBERLIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) MemberInfoAsnWrite, aip, atp, MEMBERLIST_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    ContactInfoNew()
*
**************************************************/
NLM_EXTERN 
ContactInfoPtr LIBCALL
ContactInfoNew(void)
{
   ContactInfoPtr ptr = MemNew((size_t) sizeof(ContactInfo));

   return ptr;

}


/**************************************************
*
*    ContactInfoFree()
*
**************************************************/
NLM_EXTERN 
ContactInfoPtr LIBCALL
ContactInfoFree(ContactInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   FullNameFree(ptr -> full_name);
   ClubJobsFree(ptr -> jobs);
   CalendarDateFree(ptr -> expires);
   MemFree(ptr -> address);
   MemFree(ptr -> city);
   MemFree(ptr -> state);
   MemFree(ptr -> postal_code);
   MemFree(ptr -> country);
   MemFree(ptr -> home_phone);
   MemFree(ptr -> work_phone);
   MemFree(ptr -> cell_phone);
   MemFree(ptr -> e_mail);
   MemFree(ptr -> pilot_ratings);
   MemFree(ptr -> occupation);
   MemFree(ptr -> comment);
   MemFree(ptr -> ssa_number);
   CalendarDateFree(ptr -> ssa_expiration);
   MemFree(ptr -> fast_voucher);
   return MemFree(ptr);
}


/**************************************************
*
*    ContactInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
ContactInfoPtr LIBCALL
ContactInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ContactInfoPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ContactInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, CONTACTINFO);
   } else {
      atp = AsnLinkType(orig, CONTACTINFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ContactInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CONTACTINFO_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_full_name) {
      ptr -> full_name = FullNameAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_jobs) {
      ptr -> jobs = ClubJobsAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_expires) {
      ptr -> expires = CalendarDateAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_address) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> address = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_city) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> city = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_state) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> state = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_postal_code) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> postal_code = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_country) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> country = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_home_phone) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> home_phone = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_work_phone) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> work_phone = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_cell_phone) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> cell_phone = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_e_mail) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> e_mail = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_pilot_ratings) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> pilot_ratings = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_occupation) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> occupation = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_comment) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> comment = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_ssa_number) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> ssa_number = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_ssa_expiration) {
      ptr -> ssa_expiration = CalendarDateAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CONTACTINFO_fast_voucher) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> fast_voucher = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = ContactInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    ContactInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ContactInfoAsnWrite(ContactInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CONTACTINFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, CONTACTINFO_name,  &av);
   }
   if (ptr -> full_name != NULL) {
      if ( ! FullNameAsnWrite(ptr -> full_name, aip, CONTACTINFO_full_name)) {
         goto erret;
      }
   }
   if (ptr -> type || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> type;
      retval = AsnWrite(aip, CONTACTINFO_type,  &av);
   }
   if (ptr -> jobs != NULL) {
      if ( ! ClubJobsAsnWrite(ptr -> jobs, aip, CONTACTINFO_jobs)) {
         goto erret;
      }
   }
   if (ptr -> expires != NULL) {
      if ( ! CalendarDateAsnWrite(ptr -> expires, aip, CONTACTINFO_expires)) {
         goto erret;
      }
   }
   if (ptr -> address != NULL) {
      av.ptrvalue = ptr -> address;
      retval = AsnWrite(aip, CONTACTINFO_address,  &av);
   }
   if (ptr -> city != NULL) {
      av.ptrvalue = ptr -> city;
      retval = AsnWrite(aip, CONTACTINFO_city,  &av);
   }
   if (ptr -> state != NULL) {
      av.ptrvalue = ptr -> state;
      retval = AsnWrite(aip, CONTACTINFO_state,  &av);
   }
   if (ptr -> postal_code != NULL) {
      av.ptrvalue = ptr -> postal_code;
      retval = AsnWrite(aip, CONTACTINFO_postal_code,  &av);
   }
   if (ptr -> country != NULL) {
      av.ptrvalue = ptr -> country;
      retval = AsnWrite(aip, CONTACTINFO_country,  &av);
   }
   if (ptr -> home_phone != NULL) {
      av.ptrvalue = ptr -> home_phone;
      retval = AsnWrite(aip, CONTACTINFO_home_phone,  &av);
   }
   if (ptr -> work_phone != NULL) {
      av.ptrvalue = ptr -> work_phone;
      retval = AsnWrite(aip, CONTACTINFO_work_phone,  &av);
   }
   if (ptr -> cell_phone != NULL) {
      av.ptrvalue = ptr -> cell_phone;
      retval = AsnWrite(aip, CONTACTINFO_cell_phone,  &av);
   }
   if (ptr -> e_mail != NULL) {
      av.ptrvalue = ptr -> e_mail;
      retval = AsnWrite(aip, CONTACTINFO_e_mail,  &av);
   }
   if (ptr -> pilot_ratings != NULL) {
      av.ptrvalue = ptr -> pilot_ratings;
      retval = AsnWrite(aip, CONTACTINFO_pilot_ratings,  &av);
   }
   if (ptr -> occupation != NULL) {
      av.ptrvalue = ptr -> occupation;
      retval = AsnWrite(aip, CONTACTINFO_occupation,  &av);
   }
   if (ptr -> comment != NULL) {
      av.ptrvalue = ptr -> comment;
      retval = AsnWrite(aip, CONTACTINFO_comment,  &av);
   }
   if (ptr -> ssa_number != NULL) {
      av.ptrvalue = ptr -> ssa_number;
      retval = AsnWrite(aip, CONTACTINFO_ssa_number,  &av);
   }
   if (ptr -> ssa_expiration != NULL) {
      if ( ! CalendarDateAsnWrite(ptr -> ssa_expiration, aip, CONTACTINFO_ssa_expiration)) {
         goto erret;
      }
   }
   if (ptr -> fast_voucher != NULL) {
      av.ptrvalue = ptr -> fast_voucher;
      retval = AsnWrite(aip, CONTACTINFO_fast_voucher,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    ContactListFree()
*
**************************************************/
NLM_EXTERN 
ContactListPtr LIBCALL
ContactListFree(ContactListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) ContactInfoFree);
   return NULL;
}


/**************************************************
*
*    ContactListAsnRead()
*
**************************************************/
NLM_EXTERN 
ContactListPtr LIBCALL
ContactListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ContactListPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ContactList ::= (self contained) */
      atp = AsnReadId(aip, amp, CONTACTLIST);
   } else {
      atp = AsnLinkType(orig, CONTACTLIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ContactInfoAsnRead, (AsnOptFreeFunc) ContactInfoFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = ContactListFree(ptr);
   goto ret;
}



/**************************************************
*
*    ContactListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ContactListAsnWrite(ContactListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CONTACTLIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) ContactInfoAsnWrite, aip, atp, CONTACTLIST_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    StaffDataNew()
*
**************************************************/
NLM_EXTERN 
StaffDataPtr LIBCALL
StaffDataNew(void)
{
   StaffDataPtr ptr = MemNew((size_t) sizeof(StaffData));

   return ptr;

}


/**************************************************
*
*    StaffDataFree()
*
**************************************************/
NLM_EXTERN 
StaffDataPtr LIBCALL
StaffDataFree(StaffDataPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> dutyofficer);
   MemFree(ptr -> assistant);
   MemFree(ptr -> timekeeper);
   MemFree(ptr -> instructor);
   MemFree(ptr -> towpilot);
   MemFree(ptr -> am_towpilot);
   MemFree(ptr -> pm_towpilot);
   MemFree(ptr -> comment);
   return MemFree(ptr);
}


/**************************************************
*
*    StaffDataAsnRead()
*
**************************************************/
NLM_EXTERN 
StaffDataPtr LIBCALL
StaffDataAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   StaffDataPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* StaffData ::= (self contained) */
      atp = AsnReadId(aip, amp, STAFFDATA);
   } else {
      atp = AsnLinkType(orig, STAFFDATA);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = StaffDataNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == STAFFDATA_dutyofficer) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> dutyofficer = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == STAFFDATA_assistant) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> assistant = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == STAFFDATA_timekeeper) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> timekeeper = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == STAFFDATA_instructor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> instructor = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == STAFFDATA_towpilot) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> towpilot = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == STAFFDATA_am_towpilot) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> am_towpilot = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == STAFFDATA_pm_towpilot) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> pm_towpilot = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == STAFFDATA_comment) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> comment = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = StaffDataFree(ptr);
   goto ret;
}



/**************************************************
*
*    StaffDataAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
StaffDataAsnWrite(StaffDataPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, STAFFDATA);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> dutyofficer != NULL) {
      av.ptrvalue = ptr -> dutyofficer;
      retval = AsnWrite(aip, STAFFDATA_dutyofficer,  &av);
   }
   if (ptr -> assistant != NULL) {
      av.ptrvalue = ptr -> assistant;
      retval = AsnWrite(aip, STAFFDATA_assistant,  &av);
   }
   if (ptr -> timekeeper != NULL) {
      av.ptrvalue = ptr -> timekeeper;
      retval = AsnWrite(aip, STAFFDATA_timekeeper,  &av);
   }
   if (ptr -> instructor != NULL) {
      av.ptrvalue = ptr -> instructor;
      retval = AsnWrite(aip, STAFFDATA_instructor,  &av);
   }
   if (ptr -> towpilot != NULL) {
      av.ptrvalue = ptr -> towpilot;
      retval = AsnWrite(aip, STAFFDATA_towpilot,  &av);
   }
   if (ptr -> am_towpilot != NULL) {
      av.ptrvalue = ptr -> am_towpilot;
      retval = AsnWrite(aip, STAFFDATA_am_towpilot,  &av);
   }
   if (ptr -> pm_towpilot != NULL) {
      av.ptrvalue = ptr -> pm_towpilot;
      retval = AsnWrite(aip, STAFFDATA_pm_towpilot,  &av);
   }
   if (ptr -> comment != NULL) {
      av.ptrvalue = ptr -> comment;
      retval = AsnWrite(aip, STAFFDATA_comment,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    AirplaneInfoNew()
*
**************************************************/
NLM_EXTERN 
AirplaneInfoPtr LIBCALL
AirplaneInfoNew(void)
{
   AirplaneInfoPtr ptr = MemNew((size_t) sizeof(AirplaneInfo));

   return ptr;

}


/**************************************************
*
*    AirplaneInfoFree()
*
**************************************************/
NLM_EXTERN 
AirplaneInfoPtr LIBCALL
AirplaneInfoFree(AirplaneInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> airplane);
   MemFree(ptr -> tail_number);
   DollarAmountFree(ptr -> cost_per_min);
   DollarAmountFree(ptr -> cost_per_flight);
   FlightTimeFree(ptr -> max_rental_time);
   AsnGenericBaseSeqOfFree(ptr -> fly_for_free ,ASNCODE_PTRVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    AirplaneInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
AirplaneInfoPtr LIBCALL
AirplaneInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AirplaneInfoPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AirplaneInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, AIRPLANEINFO);
   } else {
      atp = AsnLinkType(orig, AIRPLANEINFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = AirplaneInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == AIRPLANEINFO_airplane) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> airplane = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEINFO_tail_number) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> tail_number = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEINFO_cost_per_min) {
      ptr -> cost_per_min = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEINFO_cost_per_flight) {
      ptr -> cost_per_flight = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEINFO_number_of_seats) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> number_of_seats = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEINFO_max_rental_time) {
      ptr -> max_rental_time = FlightTimeAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEINFO_fly_for_free) {
      ptr -> fly_for_free = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> fly_for_free == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = AirplaneInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    AirplaneInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AirplaneInfoAsnWrite(AirplaneInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, AIRPLANEINFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> airplane != NULL) {
      av.ptrvalue = ptr -> airplane;
      retval = AsnWrite(aip, AIRPLANEINFO_airplane,  &av);
   }
   if (ptr -> tail_number != NULL) {
      av.ptrvalue = ptr -> tail_number;
      retval = AsnWrite(aip, AIRPLANEINFO_tail_number,  &av);
   }
   if (ptr -> cost_per_min != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> cost_per_min, aip, AIRPLANEINFO_cost_per_min)) {
         goto erret;
      }
   }
   if (ptr -> cost_per_flight != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> cost_per_flight, aip, AIRPLANEINFO_cost_per_flight)) {
         goto erret;
      }
   }
   if (ptr -> number_of_seats || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> number_of_seats;
      retval = AsnWrite(aip, AIRPLANEINFO_number_of_seats,  &av);
   }
   if (ptr -> max_rental_time != NULL) {
      if ( ! FlightTimeAsnWrite(ptr -> max_rental_time, aip, AIRPLANEINFO_max_rental_time)) {
         goto erret;
      }
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> fly_for_free ,ASNCODE_PTRVAL_SLOT, aip, AIRPLANEINFO_fly_for_free, AIRPLANEINFO_fly_for_free_E);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    AirplaneTableFree()
*
**************************************************/
NLM_EXTERN 
AirplaneTablePtr LIBCALL
AirplaneTableFree(AirplaneTablePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) AirplaneInfoFree);
   return NULL;
}


/**************************************************
*
*    AirplaneTableAsnRead()
*
**************************************************/
NLM_EXTERN 
AirplaneTablePtr LIBCALL
AirplaneTableAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AirplaneTablePtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AirplaneTable ::= (self contained) */
      atp = AsnReadId(aip, amp, AIRPLANETABLE);
   } else {
      atp = AsnLinkType(orig, AIRPLANETABLE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) AirplaneInfoAsnRead, (AsnOptFreeFunc) AirplaneInfoFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = AirplaneTableFree(ptr);
   goto ret;
}



/**************************************************
*
*    AirplaneTableAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AirplaneTableAsnWrite(AirplaneTablePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, AIRPLANETABLE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) AirplaneInfoAsnWrite, aip, atp, AIRPLANETABLE_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    GliderInfoNew()
*
**************************************************/
NLM_EXTERN 
GliderInfoPtr LIBCALL
GliderInfoNew(void)
{
   GliderInfoPtr ptr = MemNew((size_t) sizeof(GliderInfo));

   return ptr;

}


/**************************************************
*
*    GliderInfoFree()
*
**************************************************/
NLM_EXTERN 
GliderInfoPtr LIBCALL
GliderInfoFree(GliderInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> glider);
   MemFree(ptr -> tail_number);
   DollarAmountFree(ptr -> cost_per_min);
   DollarAmountFree(ptr -> cost_per_flight);
   FlightTimeFree(ptr -> max_rental_time);
   AsnGenericBaseSeqOfFree(ptr -> fly_for_free ,ASNCODE_PTRVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    GliderInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
GliderInfoPtr LIBCALL
GliderInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   GliderInfoPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* GliderInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, GLIDERINFO);
   } else {
      atp = AsnLinkType(orig, GLIDERINFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = GliderInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == GLIDERINFO_glider) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> glider = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERINFO_tail_number) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> tail_number = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERINFO_cost_per_min) {
      ptr -> cost_per_min = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERINFO_cost_per_flight) {
      ptr -> cost_per_flight = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERINFO_number_of_seats) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> number_of_seats = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERINFO_max_rental_time) {
      ptr -> max_rental_time = FlightTimeAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERINFO_fly_for_free) {
      ptr -> fly_for_free = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> fly_for_free == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = GliderInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    GliderInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
GliderInfoAsnWrite(GliderInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, GLIDERINFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> glider != NULL) {
      av.ptrvalue = ptr -> glider;
      retval = AsnWrite(aip, GLIDERINFO_glider,  &av);
   }
   if (ptr -> tail_number != NULL) {
      av.ptrvalue = ptr -> tail_number;
      retval = AsnWrite(aip, GLIDERINFO_tail_number,  &av);
   }
   if (ptr -> cost_per_min != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> cost_per_min, aip, GLIDERINFO_cost_per_min)) {
         goto erret;
      }
   }
   if (ptr -> cost_per_flight != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> cost_per_flight, aip, GLIDERINFO_cost_per_flight)) {
         goto erret;
      }
   }
   if (ptr -> number_of_seats || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> number_of_seats;
      retval = AsnWrite(aip, GLIDERINFO_number_of_seats,  &av);
   }
   if (ptr -> max_rental_time != NULL) {
      if ( ! FlightTimeAsnWrite(ptr -> max_rental_time, aip, GLIDERINFO_max_rental_time)) {
         goto erret;
      }
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> fly_for_free ,ASNCODE_PTRVAL_SLOT, aip, GLIDERINFO_fly_for_free, GLIDERINFO_fly_for_free_E);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    GliderTableFree()
*
**************************************************/
NLM_EXTERN 
GliderTablePtr LIBCALL
GliderTableFree(GliderTablePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) GliderInfoFree);
   return NULL;
}


/**************************************************
*
*    GliderTableAsnRead()
*
**************************************************/
NLM_EXTERN 
GliderTablePtr LIBCALL
GliderTableAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   GliderTablePtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* GliderTable ::= (self contained) */
      atp = AsnReadId(aip, amp, GLIDERTABLE);
   } else {
      atp = AsnLinkType(orig, GLIDERTABLE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) GliderInfoAsnRead, (AsnOptFreeFunc) GliderInfoFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = GliderTableFree(ptr);
   goto ret;
}



/**************************************************
*
*    GliderTableAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
GliderTableAsnWrite(GliderTablePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, GLIDERTABLE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) GliderInfoAsnWrite, aip, atp, GLIDERTABLE_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    TowPlaneInfoNew()
*
**************************************************/
NLM_EXTERN 
TowPlaneInfoPtr LIBCALL
TowPlaneInfoNew(void)
{
   TowPlaneInfoPtr ptr = MemNew((size_t) sizeof(TowPlaneInfo));

   return ptr;

}


/**************************************************
*
*    TowPlaneInfoFree()
*
**************************************************/
NLM_EXTERN 
TowPlaneInfoPtr LIBCALL
TowPlaneInfoFree(TowPlaneInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> towplane);
   MemFree(ptr -> tail_number);
   return MemFree(ptr);
}


/**************************************************
*
*    TowPlaneInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
TowPlaneInfoPtr LIBCALL
TowPlaneInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TowPlaneInfoPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* TowPlaneInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, TOWPLANEINFO);
   } else {
      atp = AsnLinkType(orig, TOWPLANEINFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = TowPlaneInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TOWPLANEINFO_towplane) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> towplane = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TOWPLANEINFO_tail_number) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> tail_number = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = TowPlaneInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    TowPlaneInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
TowPlaneInfoAsnWrite(TowPlaneInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TOWPLANEINFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> towplane != NULL) {
      av.ptrvalue = ptr -> towplane;
      retval = AsnWrite(aip, TOWPLANEINFO_towplane,  &av);
   }
   if (ptr -> tail_number != NULL) {
      av.ptrvalue = ptr -> tail_number;
      retval = AsnWrite(aip, TOWPLANEINFO_tail_number,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    TowPlaneTableFree()
*
**************************************************/
NLM_EXTERN 
TowPlaneTablePtr LIBCALL
TowPlaneTableFree(TowPlaneTablePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) TowPlaneInfoFree);
   return NULL;
}


/**************************************************
*
*    TowPlaneTableAsnRead()
*
**************************************************/
NLM_EXTERN 
TowPlaneTablePtr LIBCALL
TowPlaneTableAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TowPlaneTablePtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* TowPlaneTable ::= (self contained) */
      atp = AsnReadId(aip, amp, TOWPLANETABLE);
   } else {
      atp = AsnLinkType(orig, TOWPLANETABLE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) TowPlaneInfoAsnRead, (AsnOptFreeFunc) TowPlaneInfoFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = TowPlaneTableFree(ptr);
   goto ret;
}



/**************************************************
*
*    TowPlaneTableAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
TowPlaneTableAsnWrite(TowPlaneTablePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TOWPLANETABLE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) TowPlaneInfoAsnWrite, aip, atp, TOWPLANETABLE_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    TowRateItemNew()
*
**************************************************/
NLM_EXTERN 
TowRateItemPtr LIBCALL
TowRateItemNew(void)
{
   TowRateItemPtr ptr = MemNew((size_t) sizeof(TowRateItem));

   return ptr;

}


/**************************************************
*
*    TowRateItemFree()
*
**************************************************/
NLM_EXTERN 
TowRateItemPtr LIBCALL
TowRateItemFree(TowRateItemPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   DollarAmountFree(ptr -> cost_of_tow);
   return MemFree(ptr);
}


/**************************************************
*
*    TowRateItemAsnRead()
*
**************************************************/
NLM_EXTERN 
TowRateItemPtr LIBCALL
TowRateItemAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TowRateItemPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* TowRateItem ::= (self contained) */
      atp = AsnReadId(aip, amp, TOWRATEITEM);
   } else {
      atp = AsnLinkType(orig, TOWRATEITEM);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = TowRateItemNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TOWRATEITEM_altitude) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> altitude = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TOWRATEITEM_cost_of_tow) {
      ptr -> cost_of_tow = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = TowRateItemFree(ptr);
   goto ret;
}



/**************************************************
*
*    TowRateItemAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
TowRateItemAsnWrite(TowRateItemPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TOWRATEITEM);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> altitude;
   retval = AsnWrite(aip, TOWRATEITEM_altitude,  &av);
   if (ptr -> cost_of_tow != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> cost_of_tow, aip, TOWRATEITEM_cost_of_tow)) {
         goto erret;
      }
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    TowRateTableFree()
*
**************************************************/
NLM_EXTERN 
TowRateTablePtr LIBCALL
TowRateTableFree(TowRateTablePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) TowRateItemFree);
   return NULL;
}


/**************************************************
*
*    TowRateTableAsnRead()
*
**************************************************/
NLM_EXTERN 
TowRateTablePtr LIBCALL
TowRateTableAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TowRateTablePtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* TowRateTable ::= (self contained) */
      atp = AsnReadId(aip, amp, TOWRATETABLE);
   } else {
      atp = AsnLinkType(orig, TOWRATETABLE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) TowRateItemAsnRead, (AsnOptFreeFunc) TowRateItemFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = TowRateTableFree(ptr);
   goto ret;
}



/**************************************************
*
*    TowRateTableAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
TowRateTableAsnWrite(TowRateTablePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TOWRATETABLE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) TowRateItemAsnWrite, aip, atp, TOWRATETABLE_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    TowPlaneListNew()
*
**************************************************/
NLM_EXTERN 
TowPlaneListPtr LIBCALL
TowPlaneListNew(void)
{
   TowPlaneListPtr ptr = MemNew((size_t) sizeof(TowPlaneList));

   return ptr;

}


/**************************************************
*
*    TowPlaneListFree()
*
**************************************************/
NLM_EXTERN 
TowPlaneListPtr LIBCALL
TowPlaneListFree(TowPlaneListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> towplane);
   MemFree(ptr -> tail_number);
   TowRateTableFree(ptr -> rates);
   return MemFree(ptr);
}


/**************************************************
*
*    TowPlaneListAsnRead()
*
**************************************************/
NLM_EXTERN 
TowPlaneListPtr LIBCALL
TowPlaneListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TowPlaneListPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* TowPlaneList ::= (self contained) */
      atp = AsnReadId(aip, amp, TOWPLANELIST);
   } else {
      atp = AsnLinkType(orig, TOWPLANELIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = TowPlaneListNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TOWPLANELIST_towplane) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> towplane = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TOWPLANELIST_tail_number) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> tail_number = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TOWPLANELIST_rates) {
      ptr -> rates = TowRateTableAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = TowPlaneListFree(ptr);
   goto ret;
}



/**************************************************
*
*    TowPlaneListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
TowPlaneListAsnWrite(TowPlaneListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TOWPLANELIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> towplane != NULL) {
      av.ptrvalue = ptr -> towplane;
      retval = AsnWrite(aip, TOWPLANELIST_towplane,  &av);
   }
   if (ptr -> tail_number != NULL) {
      av.ptrvalue = ptr -> tail_number;
      retval = AsnWrite(aip, TOWPLANELIST_tail_number,  &av);
   }
   if (ptr -> rates != NULL) {
      if ( ! TowRateTableAsnWrite(ptr -> rates, aip, TOWPLANELIST_rates)) {
         goto erret;
      }
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    TowPlaneSetFree()
*
**************************************************/
NLM_EXTERN 
TowPlaneSetPtr LIBCALL
TowPlaneSetFree(TowPlaneSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) TowPlaneListFree);
   return NULL;
}


/**************************************************
*
*    TowPlaneSetAsnRead()
*
**************************************************/
NLM_EXTERN 
TowPlaneSetPtr LIBCALL
TowPlaneSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TowPlaneSetPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* TowPlaneSet ::= (self contained) */
      atp = AsnReadId(aip, amp, TOWPLANESET);
   } else {
      atp = AsnLinkType(orig, TOWPLANESET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) TowPlaneListAsnRead, (AsnOptFreeFunc) TowPlaneListFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = TowPlaneSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    TowPlaneSetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
TowPlaneSetAsnWrite(TowPlaneSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TOWPLANESET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) TowPlaneListAsnWrite, aip, atp, TOWPLANESET_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    AirportInfoNew()
*
**************************************************/
NLM_EXTERN 
AirportInfoPtr LIBCALL
AirportInfoNew(void)
{
   AirportInfoPtr ptr = MemNew((size_t) sizeof(AirportInfo));

   return ptr;

}


/**************************************************
*
*    AirportInfoFree()
*
**************************************************/
NLM_EXTERN 
AirportInfoPtr LIBCALL
AirportInfoFree(AirportInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> airport);
   MemFree(ptr -> code);
   MemFree(ptr -> suffix);
   return MemFree(ptr);
}


/**************************************************
*
*    AirportInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
AirportInfoPtr LIBCALL
AirportInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AirportInfoPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AirportInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, AIRPORTINFO);
   } else {
      atp = AsnLinkType(orig, AIRPORTINFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = AirportInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == AIRPORTINFO_airport) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> airport = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPORTINFO_code) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> code = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPORTINFO_suffix) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> suffix = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPORTINFO_altitude) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> altitude = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = AirportInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    AirportInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AirportInfoAsnWrite(AirportInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, AIRPORTINFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> airport != NULL) {
      av.ptrvalue = ptr -> airport;
      retval = AsnWrite(aip, AIRPORTINFO_airport,  &av);
   }
   if (ptr -> code != NULL) {
      av.ptrvalue = ptr -> code;
      retval = AsnWrite(aip, AIRPORTINFO_code,  &av);
   }
   if (ptr -> suffix != NULL) {
      av.ptrvalue = ptr -> suffix;
      retval = AsnWrite(aip, AIRPORTINFO_suffix,  &av);
   }
   if (ptr -> altitude || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> altitude;
      retval = AsnWrite(aip, AIRPORTINFO_altitude,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    AirportListFree()
*
**************************************************/
NLM_EXTERN 
AirportListPtr LIBCALL
AirportListFree(AirportListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) AirportInfoFree);
   return NULL;
}


/**************************************************
*
*    AirportListAsnRead()
*
**************************************************/
NLM_EXTERN 
AirportListPtr LIBCALL
AirportListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AirportListPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AirportList ::= (self contained) */
      atp = AsnReadId(aip, amp, AIRPORTLIST);
   } else {
      atp = AsnLinkType(orig, AIRPORTLIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) AirportInfoAsnRead, (AsnOptFreeFunc) AirportInfoFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = AirportListFree(ptr);
   goto ret;
}



/**************************************************
*
*    AirportListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AirportListAsnWrite(AirportListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, AIRPORTLIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) AirportInfoAsnWrite, aip, atp, AIRPORTLIST_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    GliderFlightNew()
*
**************************************************/
NLM_EXTERN 
GliderFlightPtr LIBCALL
GliderFlightNew(void)
{
   GliderFlightPtr ptr = MemNew((size_t) sizeof(GliderFlight));

   ptr -> release_altitude = -1;
   return ptr;

}


/**************************************************
*
*    GliderFlightFree()
*
**************************************************/
NLM_EXTERN 
GliderFlightPtr LIBCALL
GliderFlightFree(GliderFlightPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   MemFree(ptr -> alt_payer);
   MemFree(ptr -> passenger);
   MemFree(ptr -> glider);
   MemFree(ptr -> airport);
   MemFree(ptr -> towplane);
   MemFree(ptr -> instructor);
   MemFree(ptr -> towpilot);
   ClockTimeFree(ptr -> takeoff);
   ClockTimeFree(ptr -> landing);
   FlightTimeFree(ptr -> flight_time);
   DollarAmountFree(ptr -> flight_cost);
   DollarAmountFree(ptr -> tow_cost);
   DollarAmountFree(ptr -> total_cost);
   MemFree(ptr -> comment);
   return MemFree(ptr);
}


/**************************************************
*
*    GliderFlightAsnRead()
*
**************************************************/
NLM_EXTERN 
GliderFlightPtr LIBCALL
GliderFlightAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   GliderFlightPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* GliderFlight ::= (self contained) */
      atp = AsnReadId(aip, amp, GLIDERFLIGHT);
   } else {
      atp = AsnLinkType(orig, GLIDERFLIGHT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = GliderFlightNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == GLIDERFLIGHT_flight_number) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> flight_number = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      ptr -> OBbits__ |= 1<<1;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_alt_payer) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> alt_payer = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_split_payment) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> split_payment = av.intvalue;
      ptr -> OBbits__ |= 1<<2;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_passenger) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> passenger = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_glider) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> glider = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_category) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> category = av.intvalue;
      ptr -> OBbits__ |= 1<<3;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_airport) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> airport = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_towplane) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> towplane = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_instructor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> instructor = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_towpilot) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> towpilot = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_takeoff) {
      ptr -> takeoff = ClockTimeAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_landing) {
      ptr -> landing = ClockTimeAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_exception) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> exception = av.intvalue;
      ptr -> OBbits__ |= 1<<4;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_landed_out) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> landed_out = av.boolvalue;
      ptr -> OBbits__ |= 1<<5;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_flight_time) {
      ptr -> flight_time = FlightTimeAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_release_altitude) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> release_altitude = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_flight_cost) {
      ptr -> flight_cost = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_tow_cost) {
      ptr -> tow_cost = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_total_cost) {
      ptr -> total_cost = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLIDERFLIGHT_comment) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> comment = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = GliderFlightFree(ptr);
   goto ret;
}



/**************************************************
*
*    GliderFlightAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
GliderFlightAsnWrite(GliderFlightPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, GLIDERFLIGHT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> flight_number || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> flight_number;
      retval = AsnWrite(aip, GLIDERFLIGHT_flight_number,  &av);
   }
   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, GLIDERFLIGHT_name,  &av);
   }
   if (ptr -> type || (ptr -> OBbits__ & (1<<1) )){   av.intvalue = ptr -> type;
      retval = AsnWrite(aip, GLIDERFLIGHT_type,  &av);
   }
   if (ptr -> alt_payer != NULL) {
      av.ptrvalue = ptr -> alt_payer;
      retval = AsnWrite(aip, GLIDERFLIGHT_alt_payer,  &av);
   }
   if (ptr -> split_payment || (ptr -> OBbits__ & (1<<2) )){   av.intvalue = ptr -> split_payment;
      retval = AsnWrite(aip, GLIDERFLIGHT_split_payment,  &av);
   }
   if (ptr -> passenger != NULL) {
      av.ptrvalue = ptr -> passenger;
      retval = AsnWrite(aip, GLIDERFLIGHT_passenger,  &av);
   }
   if (ptr -> glider != NULL) {
      av.ptrvalue = ptr -> glider;
      retval = AsnWrite(aip, GLIDERFLIGHT_glider,  &av);
   }
   if (ptr -> category || (ptr -> OBbits__ & (1<<3) )){   av.intvalue = ptr -> category;
      retval = AsnWrite(aip, GLIDERFLIGHT_category,  &av);
   }
   if (ptr -> airport != NULL) {
      av.ptrvalue = ptr -> airport;
      retval = AsnWrite(aip, GLIDERFLIGHT_airport,  &av);
   }
   if (ptr -> towplane != NULL) {
      av.ptrvalue = ptr -> towplane;
      retval = AsnWrite(aip, GLIDERFLIGHT_towplane,  &av);
   }
   if (ptr -> instructor != NULL) {
      av.ptrvalue = ptr -> instructor;
      retval = AsnWrite(aip, GLIDERFLIGHT_instructor,  &av);
   }
   if (ptr -> towpilot != NULL) {
      av.ptrvalue = ptr -> towpilot;
      retval = AsnWrite(aip, GLIDERFLIGHT_towpilot,  &av);
   }
   if (ptr -> takeoff != NULL) {
      if ( ! ClockTimeAsnWrite(ptr -> takeoff, aip, GLIDERFLIGHT_takeoff)) {
         goto erret;
      }
   }
   if (ptr -> landing != NULL) {
      if ( ! ClockTimeAsnWrite(ptr -> landing, aip, GLIDERFLIGHT_landing)) {
         goto erret;
      }
   }
   if (ptr -> exception || (ptr -> OBbits__ & (1<<4) )){   av.intvalue = ptr -> exception;
      retval = AsnWrite(aip, GLIDERFLIGHT_exception,  &av);
   }
   if (ptr -> landed_out || (ptr -> OBbits__ & (1<<5) )){   av.boolvalue = ptr -> landed_out;
      retval = AsnWrite(aip, GLIDERFLIGHT_landed_out,  &av);
   }
   if (ptr -> flight_time != NULL) {
      if ( ! FlightTimeAsnWrite(ptr -> flight_time, aip, GLIDERFLIGHT_flight_time)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> release_altitude;
   retval = AsnWrite(aip, GLIDERFLIGHT_release_altitude,  &av);
   if (ptr -> flight_cost != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> flight_cost, aip, GLIDERFLIGHT_flight_cost)) {
         goto erret;
      }
   }
   if (ptr -> tow_cost != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> tow_cost, aip, GLIDERFLIGHT_tow_cost)) {
         goto erret;
      }
   }
   if (ptr -> total_cost != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> total_cost, aip, GLIDERFLIGHT_total_cost)) {
         goto erret;
      }
   }
   if (ptr -> comment != NULL) {
      av.ptrvalue = ptr -> comment;
      retval = AsnWrite(aip, GLIDERFLIGHT_comment,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    GliderFlightListFree()
*
**************************************************/
NLM_EXTERN 
GliderFlightListPtr LIBCALL
GliderFlightListFree(GliderFlightListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) GliderFlightFree);
   return NULL;
}


/**************************************************
*
*    GliderFlightListAsnRead()
*
**************************************************/
NLM_EXTERN 
GliderFlightListPtr LIBCALL
GliderFlightListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   GliderFlightListPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* GliderFlightList ::= (self contained) */
      atp = AsnReadId(aip, amp, GLIDERFLIGHTLIST);
   } else {
      atp = AsnLinkType(orig, GLIDERFLIGHTLIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) GliderFlightAsnRead, (AsnOptFreeFunc) GliderFlightFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = GliderFlightListFree(ptr);
   goto ret;
}



/**************************************************
*
*    GliderFlightListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
GliderFlightListAsnWrite(GliderFlightListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, GLIDERFLIGHTLIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) GliderFlightAsnWrite, aip, atp, GLIDERFLIGHTLIST_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    FlightListFree()
*
**************************************************/
NLM_EXTERN 
FlightListPtr LIBCALL
FlightListFree(FlightListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) GliderFlightFree);
   return NULL;
}


/**************************************************
*
*    FlightListAsnRead()
*
**************************************************/
NLM_EXTERN 
FlightListPtr LIBCALL
FlightListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   FlightListPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* FlightList ::= (self contained) */
      atp = AsnReadId(aip, amp, FLIGHTLIST);
   } else {
      atp = AsnLinkType(orig, FLIGHTLIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) GliderFlightAsnRead, (AsnOptFreeFunc) GliderFlightFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = FlightListFree(ptr);
   goto ret;
}



/**************************************************
*
*    FlightListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
FlightListAsnWrite(FlightListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, FLIGHTLIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) GliderFlightAsnWrite, aip, atp, FLIGHTLIST_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    AirplaneFlightNew()
*
**************************************************/
NLM_EXTERN 
AirplaneFlightPtr LIBCALL
AirplaneFlightNew(void)
{
   AirplaneFlightPtr ptr = MemNew((size_t) sizeof(AirplaneFlight));

   return ptr;

}


/**************************************************
*
*    AirplaneFlightFree()
*
**************************************************/
NLM_EXTERN 
AirplaneFlightPtr LIBCALL
AirplaneFlightFree(AirplaneFlightPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   MemFree(ptr -> alt_payer);
   MemFree(ptr -> passenger);
   MemFree(ptr -> airplane);
   MemFree(ptr -> airport);
   MemFree(ptr -> instructor);
   ClockTimeFree(ptr -> takeoff);
   ClockTimeFree(ptr -> landing);
   FlightTimeFree(ptr -> flight_time);
   DollarAmountFree(ptr -> flight_cost);
   MemFree(ptr -> comment);
   return MemFree(ptr);
}


/**************************************************
*
*    AirplaneFlightAsnRead()
*
**************************************************/
NLM_EXTERN 
AirplaneFlightPtr LIBCALL
AirplaneFlightAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AirplaneFlightPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AirplaneFlight ::= (self contained) */
      atp = AsnReadId(aip, amp, AIRPLANEFLIGHT);
   } else {
      atp = AsnLinkType(orig, AIRPLANEFLIGHT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = AirplaneFlightNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == AIRPLANEFLIGHT_flight_number) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> flight_number = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEFLIGHT_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEFLIGHT_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      ptr -> OBbits__ |= 1<<1;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEFLIGHT_alt_payer) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> alt_payer = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEFLIGHT_split_payment) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> split_payment = av.intvalue;
      ptr -> OBbits__ |= 1<<2;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEFLIGHT_passenger) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> passenger = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEFLIGHT_airplane) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> airplane = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEFLIGHT_category) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> category = av.intvalue;
      ptr -> OBbits__ |= 1<<3;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEFLIGHT_airport) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> airport = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEFLIGHT_instructor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> instructor = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEFLIGHT_takeoff) {
      ptr -> takeoff = ClockTimeAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEFLIGHT_landing) {
      ptr -> landing = ClockTimeAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEFLIGHT_exception) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> exception = av.intvalue;
      ptr -> OBbits__ |= 1<<4;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEFLIGHT_flight_time) {
      ptr -> flight_time = FlightTimeAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEFLIGHT_flight_cost) {
      ptr -> flight_cost = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AIRPLANEFLIGHT_comment) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> comment = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = AirplaneFlightFree(ptr);
   goto ret;
}



/**************************************************
*
*    AirplaneFlightAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AirplaneFlightAsnWrite(AirplaneFlightPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, AIRPLANEFLIGHT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> flight_number || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> flight_number;
      retval = AsnWrite(aip, AIRPLANEFLIGHT_flight_number,  &av);
   }
   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, AIRPLANEFLIGHT_name,  &av);
   }
   if (ptr -> type || (ptr -> OBbits__ & (1<<1) )){   av.intvalue = ptr -> type;
      retval = AsnWrite(aip, AIRPLANEFLIGHT_type,  &av);
   }
   if (ptr -> alt_payer != NULL) {
      av.ptrvalue = ptr -> alt_payer;
      retval = AsnWrite(aip, AIRPLANEFLIGHT_alt_payer,  &av);
   }
   if (ptr -> split_payment || (ptr -> OBbits__ & (1<<2) )){   av.intvalue = ptr -> split_payment;
      retval = AsnWrite(aip, AIRPLANEFLIGHT_split_payment,  &av);
   }
   if (ptr -> passenger != NULL) {
      av.ptrvalue = ptr -> passenger;
      retval = AsnWrite(aip, AIRPLANEFLIGHT_passenger,  &av);
   }
   if (ptr -> airplane != NULL) {
      av.ptrvalue = ptr -> airplane;
      retval = AsnWrite(aip, AIRPLANEFLIGHT_airplane,  &av);
   }
   if (ptr -> category || (ptr -> OBbits__ & (1<<3) )){   av.intvalue = ptr -> category;
      retval = AsnWrite(aip, AIRPLANEFLIGHT_category,  &av);
   }
   if (ptr -> airport != NULL) {
      av.ptrvalue = ptr -> airport;
      retval = AsnWrite(aip, AIRPLANEFLIGHT_airport,  &av);
   }
   if (ptr -> instructor != NULL) {
      av.ptrvalue = ptr -> instructor;
      retval = AsnWrite(aip, AIRPLANEFLIGHT_instructor,  &av);
   }
   if (ptr -> takeoff != NULL) {
      if ( ! ClockTimeAsnWrite(ptr -> takeoff, aip, AIRPLANEFLIGHT_takeoff)) {
         goto erret;
      }
   }
   if (ptr -> landing != NULL) {
      if ( ! ClockTimeAsnWrite(ptr -> landing, aip, AIRPLANEFLIGHT_landing)) {
         goto erret;
      }
   }
   if (ptr -> exception || (ptr -> OBbits__ & (1<<4) )){   av.intvalue = ptr -> exception;
      retval = AsnWrite(aip, AIRPLANEFLIGHT_exception,  &av);
   }
   if (ptr -> flight_time != NULL) {
      if ( ! FlightTimeAsnWrite(ptr -> flight_time, aip, AIRPLANEFLIGHT_flight_time)) {
         goto erret;
      }
   }
   if (ptr -> flight_cost != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> flight_cost, aip, AIRPLANEFLIGHT_flight_cost)) {
         goto erret;
      }
   }
   if (ptr -> comment != NULL) {
      av.ptrvalue = ptr -> comment;
      retval = AsnWrite(aip, AIRPLANEFLIGHT_comment,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    AirplaneFlightListFree()
*
**************************************************/
NLM_EXTERN 
AirplaneFlightListPtr LIBCALL
AirplaneFlightListFree(AirplaneFlightListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) AirplaneFlightFree);
   return NULL;
}


/**************************************************
*
*    AirplaneFlightListAsnRead()
*
**************************************************/
NLM_EXTERN 
AirplaneFlightListPtr LIBCALL
AirplaneFlightListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AirplaneFlightListPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AirplaneFlightList ::= (self contained) */
      atp = AsnReadId(aip, amp, AIRPLANEFLIGHTLIST);
   } else {
      atp = AsnLinkType(orig, AIRPLANEFLIGHTLIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) AirplaneFlightAsnRead, (AsnOptFreeFunc) AirplaneFlightFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = AirplaneFlightListFree(ptr);
   goto ret;
}



/**************************************************
*
*    AirplaneFlightListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AirplaneFlightListAsnWrite(AirplaneFlightListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, AIRPLANEFLIGHTLIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) AirplaneFlightAsnWrite, aip, atp, AIRPLANEFLIGHTLIST_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    TowRequestNew()
*
**************************************************/
NLM_EXTERN 
TowRequestPtr LIBCALL
TowRequestNew(void)
{
   TowRequestPtr ptr = MemNew((size_t) sizeof(TowRequest));

   return ptr;

}


/**************************************************
*
*    TowRequestFree()
*
**************************************************/
NLM_EXTERN 
TowRequestPtr LIBCALL
TowRequestFree(TowRequestPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   ClockTimeFree(ptr -> request_time);
   MemFree(ptr -> glider);
   MemFree(ptr -> towplane);
   MemFree(ptr -> instructor);
   return MemFree(ptr);
}


/**************************************************
*
*    TowRequestAsnRead()
*
**************************************************/
NLM_EXTERN 
TowRequestPtr LIBCALL
TowRequestAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TowRequestPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* TowRequest ::= (self contained) */
      atp = AsnReadId(aip, amp, TOWREQUEST);
   } else {
      atp = AsnLinkType(orig, TOWREQUEST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = TowRequestNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TOWREQUEST_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TOWREQUEST_request_time) {
      ptr -> request_time = ClockTimeAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TOWREQUEST_glider) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> glider = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TOWREQUEST_towplane) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> towplane = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TOWREQUEST_instructor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> instructor = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TOWREQUEST_result) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> result = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = TowRequestFree(ptr);
   goto ret;
}



/**************************************************
*
*    TowRequestAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
TowRequestAsnWrite(TowRequestPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TOWREQUEST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, TOWREQUEST_name,  &av);
   }
   if (ptr -> request_time != NULL) {
      if ( ! ClockTimeAsnWrite(ptr -> request_time, aip, TOWREQUEST_request_time)) {
         goto erret;
      }
   }
   if (ptr -> glider != NULL) {
      av.ptrvalue = ptr -> glider;
      retval = AsnWrite(aip, TOWREQUEST_glider,  &av);
   }
   if (ptr -> towplane != NULL) {
      av.ptrvalue = ptr -> towplane;
      retval = AsnWrite(aip, TOWREQUEST_towplane,  &av);
   }
   if (ptr -> instructor != NULL) {
      av.ptrvalue = ptr -> instructor;
      retval = AsnWrite(aip, TOWREQUEST_instructor,  &av);
   }
   if (ptr -> result || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> result;
      retval = AsnWrite(aip, TOWREQUEST_result,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    TowQueueFree()
*
**************************************************/
NLM_EXTERN 
TowQueuePtr LIBCALL
TowQueueFree(TowQueuePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) TowRequestFree);
   return NULL;
}


/**************************************************
*
*    TowQueueAsnRead()
*
**************************************************/
NLM_EXTERN 
TowQueuePtr LIBCALL
TowQueueAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TowQueuePtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* TowQueue ::= (self contained) */
      atp = AsnReadId(aip, amp, TOWQUEUE);
   } else {
      atp = AsnLinkType(orig, TOWQUEUE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) TowRequestAsnRead, (AsnOptFreeFunc) TowRequestFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = TowQueueFree(ptr);
   goto ret;
}



/**************************************************
*
*    TowQueueAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
TowQueueAsnWrite(TowQueuePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TOWQUEUE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) TowRequestAsnWrite, aip, atp, TOWQUEUE_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    AwardDataNew()
*
**************************************************/
NLM_EXTERN 
AwardDataPtr LIBCALL
AwardDataNew(void)
{
   AwardDataPtr ptr = MemNew((size_t) sizeof(AwardData));

   return ptr;

}


/**************************************************
*
*    AwardDataFree()
*
**************************************************/
NLM_EXTERN 
AwardDataPtr LIBCALL
AwardDataFree(AwardDataPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   MemFree(ptr -> comment);
   return MemFree(ptr);
}


/**************************************************
*
*    AwardDataAsnRead()
*
**************************************************/
NLM_EXTERN 
AwardDataPtr LIBCALL
AwardDataAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AwardDataPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AwardData ::= (self contained) */
      atp = AsnReadId(aip, amp, AWARDDATA);
   } else {
      atp = AsnLinkType(orig, AWARDDATA);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = AwardDataNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == AWARDDATA_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AWARDDATA_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AWARDDATA_reason) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> reason = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == AWARDDATA_comment) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> comment = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = AwardDataFree(ptr);
   goto ret;
}



/**************************************************
*
*    AwardDataAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AwardDataAsnWrite(AwardDataPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, AWARDDATA);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, AWARDDATA_name,  &av);
   }
   if (ptr -> type || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> type;
      retval = AsnWrite(aip, AWARDDATA_type,  &av);
   }
   av.intvalue = ptr -> reason;
   retval = AsnWrite(aip, AWARDDATA_reason,  &av);
   if (ptr -> comment != NULL) {
      av.ptrvalue = ptr -> comment;
      retval = AsnWrite(aip, AWARDDATA_comment,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    AwardListFree()
*
**************************************************/
NLM_EXTERN 
AwardListPtr LIBCALL
AwardListFree(AwardListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) AwardDataFree);
   return NULL;
}


/**************************************************
*
*    AwardListAsnRead()
*
**************************************************/
NLM_EXTERN 
AwardListPtr LIBCALL
AwardListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AwardListPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AwardList ::= (self contained) */
      atp = AsnReadId(aip, amp, AWARDLIST);
   } else {
      atp = AsnLinkType(orig, AWARDLIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) AwardDataAsnRead, (AsnOptFreeFunc) AwardDataFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = AwardListFree(ptr);
   goto ret;
}



/**************************************************
*
*    AwardListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AwardListAsnWrite(AwardListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, AWARDLIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) AwardDataAsnWrite, aip, atp, AWARDLIST_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    TowPlaneDataNew()
*
**************************************************/
NLM_EXTERN 
TowPlaneDataPtr LIBCALL
TowPlaneDataNew(void)
{
   TowPlaneDataPtr ptr = MemNew((size_t) sizeof(TowPlaneData));

   return ptr;

}


/**************************************************
*
*    TowPlaneDataFree()
*
**************************************************/
NLM_EXTERN 
TowPlaneDataPtr LIBCALL
TowPlaneDataFree(TowPlaneDataPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> towplane);
   MemFree(ptr -> start_tach);
   MemFree(ptr -> end_tach);
   MemFree(ptr -> tach_time);
   MemFree(ptr -> gas_added);
   MemFree(ptr -> comment);
   return MemFree(ptr);
}


/**************************************************
*
*    TowPlaneDataAsnRead()
*
**************************************************/
NLM_EXTERN 
TowPlaneDataPtr LIBCALL
TowPlaneDataAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TowPlaneDataPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* TowPlaneData ::= (self contained) */
      atp = AsnReadId(aip, amp, TOWPLANEDATA);
   } else {
      atp = AsnLinkType(orig, TOWPLANEDATA);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = TowPlaneDataNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TOWPLANEDATA_towplane) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> towplane = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TOWPLANEDATA_start_tach) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> start_tach = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TOWPLANEDATA_end_tach) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> end_tach = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TOWPLANEDATA_tach_time) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> tach_time = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TOWPLANEDATA_gas_added) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gas_added = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TOWPLANEDATA_comment) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> comment = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = TowPlaneDataFree(ptr);
   goto ret;
}



/**************************************************
*
*    TowPlaneDataAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
TowPlaneDataAsnWrite(TowPlaneDataPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TOWPLANEDATA);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> towplane != NULL) {
      av.ptrvalue = ptr -> towplane;
      retval = AsnWrite(aip, TOWPLANEDATA_towplane,  &av);
   }
   if (ptr -> start_tach != NULL) {
      av.ptrvalue = ptr -> start_tach;
      retval = AsnWrite(aip, TOWPLANEDATA_start_tach,  &av);
   }
   if (ptr -> end_tach != NULL) {
      av.ptrvalue = ptr -> end_tach;
      retval = AsnWrite(aip, TOWPLANEDATA_end_tach,  &av);
   }
   if (ptr -> tach_time != NULL) {
      av.ptrvalue = ptr -> tach_time;
      retval = AsnWrite(aip, TOWPLANEDATA_tach_time,  &av);
   }
   if (ptr -> gas_added != NULL) {
      av.ptrvalue = ptr -> gas_added;
      retval = AsnWrite(aip, TOWPLANEDATA_gas_added,  &av);
   }
   if (ptr -> comment != NULL) {
      av.ptrvalue = ptr -> comment;
      retval = AsnWrite(aip, TOWPLANEDATA_comment,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    TowPlaneDataListFree()
*
**************************************************/
NLM_EXTERN 
TowPlaneDataListPtr LIBCALL
TowPlaneDataListFree(TowPlaneDataListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) TowPlaneDataFree);
   return NULL;
}


/**************************************************
*
*    TowPlaneDataListAsnRead()
*
**************************************************/
NLM_EXTERN 
TowPlaneDataListPtr LIBCALL
TowPlaneDataListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TowPlaneDataListPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* TowPlaneDataList ::= (self contained) */
      atp = AsnReadId(aip, amp, TOWPLANEDATALIST);
   } else {
      atp = AsnLinkType(orig, TOWPLANEDATALIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) TowPlaneDataAsnRead, (AsnOptFreeFunc) TowPlaneDataFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = TowPlaneDataListFree(ptr);
   goto ret;
}



/**************************************************
*
*    TowPlaneDataListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
TowPlaneDataListAsnWrite(TowPlaneDataListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TOWPLANEDATALIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) TowPlaneDataAsnWrite, aip, atp, TOWPLANEDATALIST_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    MerchandiseInfoNew()
*
**************************************************/
NLM_EXTERN 
MerchandiseInfoPtr LIBCALL
MerchandiseInfoNew(void)
{
   MerchandiseInfoPtr ptr = MemNew((size_t) sizeof(MerchandiseInfo));

   return ptr;

}


/**************************************************
*
*    MerchandiseInfoFree()
*
**************************************************/
NLM_EXTERN 
MerchandiseInfoPtr LIBCALL
MerchandiseInfoFree(MerchandiseInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> item);
   DollarAmountFree(ptr -> cost);
   return MemFree(ptr);
}


/**************************************************
*
*    MerchandiseInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
MerchandiseInfoPtr LIBCALL
MerchandiseInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MerchandiseInfoPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MerchandiseInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, MERCHANDISEINFO);
   } else {
      atp = AsnLinkType(orig, MERCHANDISEINFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MerchandiseInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MERCHANDISEINFO_item) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> item = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MERCHANDISEINFO_category) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> category = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MERCHANDISEINFO_cost) {
      ptr -> cost = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = MerchandiseInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    MerchandiseInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MerchandiseInfoAsnWrite(MerchandiseInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MERCHANDISEINFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> item != NULL) {
      av.ptrvalue = ptr -> item;
      retval = AsnWrite(aip, MERCHANDISEINFO_item,  &av);
   }
   if (ptr -> category || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> category;
      retval = AsnWrite(aip, MERCHANDISEINFO_category,  &av);
   }
   if (ptr -> cost != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> cost, aip, MERCHANDISEINFO_cost)) {
         goto erret;
      }
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    MerchandiseListFree()
*
**************************************************/
NLM_EXTERN 
MerchandiseListPtr LIBCALL
MerchandiseListFree(MerchandiseListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) MerchandiseInfoFree);
   return NULL;
}


/**************************************************
*
*    MerchandiseListAsnRead()
*
**************************************************/
NLM_EXTERN 
MerchandiseListPtr LIBCALL
MerchandiseListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MerchandiseListPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MerchandiseList ::= (self contained) */
      atp = AsnReadId(aip, amp, MERCHANDISELIST);
   } else {
      atp = AsnLinkType(orig, MERCHANDISELIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MerchandiseInfoAsnRead, (AsnOptFreeFunc) MerchandiseInfoFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = MerchandiseListFree(ptr);
   goto ret;
}



/**************************************************
*
*    MerchandiseListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MerchandiseListAsnWrite(MerchandiseListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MERCHANDISELIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) MerchandiseInfoAsnWrite, aip, atp, MERCHANDISELIST_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    FeeDataNew()
*
**************************************************/
NLM_EXTERN 
FeeDataPtr LIBCALL
FeeDataNew(void)
{
   FeeDataPtr ptr = MemNew((size_t) sizeof(FeeData));

   return ptr;

}


/**************************************************
*
*    FeeDataFree()
*
**************************************************/
NLM_EXTERN 
FeeDataPtr LIBCALL
FeeDataFree(FeeDataPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   MemFree(ptr -> alt_payer);
   DollarAmountFree(ptr -> amount);
   MemFree(ptr -> item);
   MemFree(ptr -> comment);
   return MemFree(ptr);
}


/**************************************************
*
*    FeeDataAsnRead()
*
**************************************************/
NLM_EXTERN 
FeeDataPtr LIBCALL
FeeDataAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   FeeDataPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* FeeData ::= (self contained) */
      atp = AsnReadId(aip, amp, FEEDATA);
   } else {
      atp = AsnLinkType(orig, FEEDATA);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = FeeDataNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == FEEDATA_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == FEEDATA_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == FEEDATA_alt_payer) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> alt_payer = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == FEEDATA_amount) {
      ptr -> amount = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == FEEDATA_reason) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> reason = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == FEEDATA_item) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> item = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == FEEDATA_comment) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> comment = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = FeeDataFree(ptr);
   goto ret;
}



/**************************************************
*
*    FeeDataAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
FeeDataAsnWrite(FeeDataPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, FEEDATA);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, FEEDATA_name,  &av);
   }
   if (ptr -> type || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> type;
      retval = AsnWrite(aip, FEEDATA_type,  &av);
   }
   if (ptr -> alt_payer != NULL) {
      av.ptrvalue = ptr -> alt_payer;
      retval = AsnWrite(aip, FEEDATA_alt_payer,  &av);
   }
   if (ptr -> amount != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> amount, aip, FEEDATA_amount)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> reason;
   retval = AsnWrite(aip, FEEDATA_reason,  &av);
   if (ptr -> item != NULL) {
      av.ptrvalue = ptr -> item;
      retval = AsnWrite(aip, FEEDATA_item,  &av);
   }
   if (ptr -> comment != NULL) {
      av.ptrvalue = ptr -> comment;
      retval = AsnWrite(aip, FEEDATA_comment,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    FeeListFree()
*
**************************************************/
NLM_EXTERN 
FeeListPtr LIBCALL
FeeListFree(FeeListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) FeeDataFree);
   return NULL;
}


/**************************************************
*
*    FeeListAsnRead()
*
**************************************************/
NLM_EXTERN 
FeeListPtr LIBCALL
FeeListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   FeeListPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* FeeList ::= (self contained) */
      atp = AsnReadId(aip, amp, FEELIST);
   } else {
      atp = AsnLinkType(orig, FEELIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) FeeDataAsnRead, (AsnOptFreeFunc) FeeDataFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = FeeListFree(ptr);
   goto ret;
}



/**************************************************
*
*    FeeListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
FeeListAsnWrite(FeeListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, FEELIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) FeeDataAsnWrite, aip, atp, FEELIST_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    ChargeDataNew()
*
**************************************************/
NLM_EXTERN 
ChargeDataPtr LIBCALL
ChargeDataNew(void)
{
   ChargeDataPtr ptr = MemNew((size_t) sizeof(ChargeData));

   return ptr;

}


/**************************************************
*
*    ChargeDataFree()
*
**************************************************/
NLM_EXTERN 
ChargeDataPtr LIBCALL
ChargeDataFree(ChargeDataPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   MemFree(ptr -> alt_payer);
   DollarAmountFree(ptr -> amount);
   MemFree(ptr -> item);
   MemFree(ptr -> comment);
   return MemFree(ptr);
}


/**************************************************
*
*    ChargeDataAsnRead()
*
**************************************************/
NLM_EXTERN 
ChargeDataPtr LIBCALL
ChargeDataAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ChargeDataPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ChargeData ::= (self contained) */
      atp = AsnReadId(aip, amp, CHARGEDATA);
   } else {
      atp = AsnLinkType(orig, CHARGEDATA);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ChargeDataNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CHARGEDATA_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CHARGEDATA_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CHARGEDATA_alt_payer) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> alt_payer = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CHARGEDATA_amount) {
      ptr -> amount = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CHARGEDATA_reason) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> reason = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CHARGEDATA_item) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> item = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CHARGEDATA_comment) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> comment = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = ChargeDataFree(ptr);
   goto ret;
}



/**************************************************
*
*    ChargeDataAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ChargeDataAsnWrite(ChargeDataPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CHARGEDATA);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, CHARGEDATA_name,  &av);
   }
   if (ptr -> type || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> type;
      retval = AsnWrite(aip, CHARGEDATA_type,  &av);
   }
   if (ptr -> alt_payer != NULL) {
      av.ptrvalue = ptr -> alt_payer;
      retval = AsnWrite(aip, CHARGEDATA_alt_payer,  &av);
   }
   if (ptr -> amount != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> amount, aip, CHARGEDATA_amount)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> reason;
   retval = AsnWrite(aip, CHARGEDATA_reason,  &av);
   if (ptr -> item != NULL) {
      av.ptrvalue = ptr -> item;
      retval = AsnWrite(aip, CHARGEDATA_item,  &av);
   }
   if (ptr -> comment != NULL) {
      av.ptrvalue = ptr -> comment;
      retval = AsnWrite(aip, CHARGEDATA_comment,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    ChargeListFree()
*
**************************************************/
NLM_EXTERN 
ChargeListPtr LIBCALL
ChargeListFree(ChargeListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) ChargeDataFree);
   return NULL;
}


/**************************************************
*
*    ChargeListAsnRead()
*
**************************************************/
NLM_EXTERN 
ChargeListPtr LIBCALL
ChargeListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ChargeListPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ChargeList ::= (self contained) */
      atp = AsnReadId(aip, amp, CHARGELIST);
   } else {
      atp = AsnLinkType(orig, CHARGELIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ChargeDataAsnRead, (AsnOptFreeFunc) ChargeDataFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = ChargeListFree(ptr);
   goto ret;
}



/**************************************************
*
*    ChargeListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ChargeListAsnWrite(ChargeListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CHARGELIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) ChargeDataAsnWrite, aip, atp, CHARGELIST_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    MiscChargeNew()
*
**************************************************/
NLM_EXTERN 
MiscChargePtr LIBCALL
MiscChargeNew(void)
{
   MiscChargePtr ptr = MemNew((size_t) sizeof(MiscCharge));

   return ptr;

}


/**************************************************
*
*    MiscChargeFree()
*
**************************************************/
NLM_EXTERN 
MiscChargePtr LIBCALL
MiscChargeFree(MiscChargePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   MemFree(ptr -> alt_payer);
   DollarAmountFree(ptr -> amount);
   MemFree(ptr -> item);
   MemFree(ptr -> comment);
   return MemFree(ptr);
}


/**************************************************
*
*    MiscChargeAsnRead()
*
**************************************************/
NLM_EXTERN 
MiscChargePtr LIBCALL
MiscChargeAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MiscChargePtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MiscCharge ::= (self contained) */
      atp = AsnReadId(aip, amp, MISCCHARGE);
   } else {
      atp = AsnLinkType(orig, MISCCHARGE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MiscChargeNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MISCCHARGE_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCCHARGE_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCCHARGE_alt_payer) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> alt_payer = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCCHARGE_amount) {
      ptr -> amount = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCCHARGE_reason) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> reason = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCCHARGE_item) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> item = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCCHARGE_comment) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> comment = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = MiscChargeFree(ptr);
   goto ret;
}



/**************************************************
*
*    MiscChargeAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MiscChargeAsnWrite(MiscChargePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MISCCHARGE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, MISCCHARGE_name,  &av);
   }
   if (ptr -> type || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> type;
      retval = AsnWrite(aip, MISCCHARGE_type,  &av);
   }
   if (ptr -> alt_payer != NULL) {
      av.ptrvalue = ptr -> alt_payer;
      retval = AsnWrite(aip, MISCCHARGE_alt_payer,  &av);
   }
   if (ptr -> amount != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> amount, aip, MISCCHARGE_amount)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> reason;
   retval = AsnWrite(aip, MISCCHARGE_reason,  &av);
   if (ptr -> item != NULL) {
      av.ptrvalue = ptr -> item;
      retval = AsnWrite(aip, MISCCHARGE_item,  &av);
   }
   if (ptr -> comment != NULL) {
      av.ptrvalue = ptr -> comment;
      retval = AsnWrite(aip, MISCCHARGE_comment,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    MiscListFree()
*
**************************************************/
NLM_EXTERN 
MiscListPtr LIBCALL
MiscListFree(MiscListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) MiscChargeFree);
   return NULL;
}


/**************************************************
*
*    MiscListAsnRead()
*
**************************************************/
NLM_EXTERN 
MiscListPtr LIBCALL
MiscListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MiscListPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MiscList ::= (self contained) */
      atp = AsnReadId(aip, amp, MISCLIST);
   } else {
      atp = AsnLinkType(orig, MISCLIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MiscChargeAsnRead, (AsnOptFreeFunc) MiscChargeFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = MiscListFree(ptr);
   goto ret;
}



/**************************************************
*
*    MiscListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MiscListAsnWrite(MiscListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MISCLIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) MiscChargeAsnWrite, aip, atp, MISCLIST_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    AdjustDataNew()
*
**************************************************/
NLM_EXTERN 
AdjustDataPtr LIBCALL
AdjustDataNew(void)
{
   AdjustDataPtr ptr = MemNew((size_t) sizeof(AdjustData));

   return ptr;

}


/**************************************************
*
*    AdjustDataFree()
*
**************************************************/
NLM_EXTERN 
AdjustDataPtr LIBCALL
AdjustDataFree(AdjustDataPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   MemFree(ptr -> alt_payer);
   DollarAmountFree(ptr -> amount);
   MemFree(ptr -> comment);
   return MemFree(ptr);
}


/**************************************************
*
*    AdjustDataAsnRead()
*
**************************************************/
NLM_EXTERN 
AdjustDataPtr LIBCALL
AdjustDataAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AdjustDataPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AdjustData ::= (self contained) */
      atp = AsnReadId(aip, amp, ADJUSTDATA);
   } else {
      atp = AsnLinkType(orig, ADJUSTDATA);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = AdjustDataNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ADJUSTDATA_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ADJUSTDATA_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ADJUSTDATA_alt_payer) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> alt_payer = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ADJUSTDATA_amount) {
      ptr -> amount = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ADJUSTDATA_reason) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> reason = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ADJUSTDATA_comment) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> comment = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = AdjustDataFree(ptr);
   goto ret;
}



/**************************************************
*
*    AdjustDataAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AdjustDataAsnWrite(AdjustDataPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ADJUSTDATA);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, ADJUSTDATA_name,  &av);
   }
   if (ptr -> type || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> type;
      retval = AsnWrite(aip, ADJUSTDATA_type,  &av);
   }
   if (ptr -> alt_payer != NULL) {
      av.ptrvalue = ptr -> alt_payer;
      retval = AsnWrite(aip, ADJUSTDATA_alt_payer,  &av);
   }
   if (ptr -> amount != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> amount, aip, ADJUSTDATA_amount)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> reason;
   retval = AsnWrite(aip, ADJUSTDATA_reason,  &av);
   if (ptr -> comment != NULL) {
      av.ptrvalue = ptr -> comment;
      retval = AsnWrite(aip, ADJUSTDATA_comment,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    AdjustmentListFree()
*
**************************************************/
NLM_EXTERN 
AdjustmentListPtr LIBCALL
AdjustmentListFree(AdjustmentListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) AdjustDataFree);
   return NULL;
}


/**************************************************
*
*    AdjustmentListAsnRead()
*
**************************************************/
NLM_EXTERN 
AdjustmentListPtr LIBCALL
AdjustmentListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AdjustmentListPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AdjustmentList ::= (self contained) */
      atp = AsnReadId(aip, amp, ADJUSTMENTLIST);
   } else {
      atp = AsnLinkType(orig, ADJUSTMENTLIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) AdjustDataAsnRead, (AsnOptFreeFunc) AdjustDataFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = AdjustmentListFree(ptr);
   goto ret;
}



/**************************************************
*
*    AdjustmentListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AdjustmentListAsnWrite(AdjustmentListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ADJUSTMENTLIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) AdjustDataAsnWrite, aip, atp, ADJUSTMENTLIST_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    PaymentInfoNew()
*
**************************************************/
NLM_EXTERN 
PaymentInfoPtr LIBCALL
PaymentInfoNew(void)
{
   PaymentInfoPtr ptr = MemNew((size_t) sizeof(PaymentInfo));

   return ptr;

}


/**************************************************
*
*    PaymentInfoFree()
*
**************************************************/
NLM_EXTERN 
PaymentInfoPtr LIBCALL
PaymentInfoFree(PaymentInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> payer);
   DollarAmountFree(ptr -> charges);
   DollarAmountFree(ptr -> check_payment);
   DollarAmountFree(ptr -> cash_payment);
   DollarAmountFree(ptr -> credit_payment);
   DollarAmountFree(ptr -> credit_surcharge);
   DollarAmountFree(ptr -> account_payment);
   DollarAmountFree(ptr -> voucher_payment);
   MemFree(ptr -> voucher_serial);
   MemFree(ptr -> comment);
   return MemFree(ptr);
}


/**************************************************
*
*    PaymentInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
PaymentInfoPtr LIBCALL
PaymentInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   PaymentInfoPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* PaymentInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, PAYMENTINFO);
   } else {
      atp = AsnLinkType(orig, PAYMENTINFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = PaymentInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == PAYMENTINFO_payer) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> payer = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PAYMENTINFO_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PAYMENTINFO_charges) {
      ptr -> charges = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PAYMENTINFO_check_payment) {
      ptr -> check_payment = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PAYMENTINFO_cash_payment) {
      ptr -> cash_payment = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PAYMENTINFO_credit_payment) {
      ptr -> credit_payment = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PAYMENTINFO_credit_surcharge) {
      ptr -> credit_surcharge = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PAYMENTINFO_account_payment) {
      ptr -> account_payment = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PAYMENTINFO_voucher_payment) {
      ptr -> voucher_payment = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PAYMENTINFO_voucher_serial) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> voucher_serial = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PAYMENTINFO_comment) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> comment = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = PaymentInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    PaymentInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
PaymentInfoAsnWrite(PaymentInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, PAYMENTINFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> payer != NULL) {
      av.ptrvalue = ptr -> payer;
      retval = AsnWrite(aip, PAYMENTINFO_payer,  &av);
   }
   if (ptr -> type || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> type;
      retval = AsnWrite(aip, PAYMENTINFO_type,  &av);
   }
   if (ptr -> charges != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> charges, aip, PAYMENTINFO_charges)) {
         goto erret;
      }
   }
   if (ptr -> check_payment != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> check_payment, aip, PAYMENTINFO_check_payment)) {
         goto erret;
      }
   }
   if (ptr -> cash_payment != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> cash_payment, aip, PAYMENTINFO_cash_payment)) {
         goto erret;
      }
   }
   if (ptr -> credit_payment != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> credit_payment, aip, PAYMENTINFO_credit_payment)) {
         goto erret;
      }
   }
   if (ptr -> credit_surcharge != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> credit_surcharge, aip, PAYMENTINFO_credit_surcharge)) {
         goto erret;
      }
   }
   if (ptr -> account_payment != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> account_payment, aip, PAYMENTINFO_account_payment)) {
         goto erret;
      }
   }
   if (ptr -> voucher_payment != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> voucher_payment, aip, PAYMENTINFO_voucher_payment)) {
         goto erret;
      }
   }
   if (ptr -> voucher_serial != NULL) {
      av.ptrvalue = ptr -> voucher_serial;
      retval = AsnWrite(aip, PAYMENTINFO_voucher_serial,  &av);
   }
   if (ptr -> comment != NULL) {
      av.ptrvalue = ptr -> comment;
      retval = AsnWrite(aip, PAYMENTINFO_comment,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    PaymentListFree()
*
**************************************************/
NLM_EXTERN 
PaymentListPtr LIBCALL
PaymentListFree(PaymentListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) PaymentInfoFree);
   return NULL;
}


/**************************************************
*
*    PaymentListAsnRead()
*
**************************************************/
NLM_EXTERN 
PaymentListPtr LIBCALL
PaymentListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   PaymentListPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* PaymentList ::= (self contained) */
      atp = AsnReadId(aip, amp, PAYMENTLIST);
   } else {
      atp = AsnLinkType(orig, PAYMENTLIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) PaymentInfoAsnRead, (AsnOptFreeFunc) PaymentInfoFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = PaymentListFree(ptr);
   goto ret;
}



/**************************************************
*
*    PaymentListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
PaymentListAsnWrite(PaymentListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, PAYMENTLIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) PaymentInfoAsnWrite, aip, atp, PAYMENTLIST_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    MiscRateNew()
*
**************************************************/
NLM_EXTERN 
MiscRatePtr LIBCALL
MiscRateNew(void)
{
   MiscRatePtr ptr = MemNew((size_t) sizeof(MiscRate));

   return ptr;

}


/**************************************************
*
*    MiscRateFree()
*
**************************************************/
NLM_EXTERN 
MiscRatePtr LIBCALL
MiscRateFree(MiscRatePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   DollarAmountFree(ptr -> initiation_fee);
   DollarAmountFree(ptr -> monthly_dues);
   DollarAmountFree(ptr -> ssa_regular);
   DollarAmountFree(ptr -> ssa_family);
   DollarAmountFree(ptr -> fast_fee);
   DollarAmountFree(ptr -> fast_extra);
   MemFree(ptr -> fast_list);
   DollarAmountFree(ptr -> intro_fee);
   DollarAmountFree(ptr -> intro_extra);
   MemFree(ptr -> intro_list);
   DollarAmountFree(ptr -> visiting_fee);
   DollarAmountFree(ptr -> affiliated_fee);
   DollarAmountFree(ptr -> transient_fee);
   DollarAmountFree(ptr -> retrieve_cost_per_min);
   DollarAmountFree(ptr -> minimum_retrieve_cost);
   DollarAmountFree(ptr -> credit_card_percent);
   DollarAmountFree(ptr -> credit_card_flat);
   return MemFree(ptr);
}


/**************************************************
*
*    MiscRateAsnRead()
*
**************************************************/
NLM_EXTERN 
MiscRatePtr LIBCALL
MiscRateAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MiscRatePtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MiscRate ::= (self contained) */
      atp = AsnReadId(aip, amp, MISCRATE);
   } else {
      atp = AsnLinkType(orig, MISCRATE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MiscRateNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MISCRATE_year) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> year = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_initiation_fee) {
      ptr -> initiation_fee = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_monthly_dues) {
      ptr -> monthly_dues = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_ssa_regular) {
      ptr -> ssa_regular = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_ssa_family) {
      ptr -> ssa_family = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_intro_memb_days) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> intro_memb_days = av.intvalue;
      ptr -> OBbits__ |= 1<<1;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_fast_fee) {
      ptr -> fast_fee = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_fast_extra) {
      ptr -> fast_extra = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_fast_list) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> fast_list = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_intro_fee) {
      ptr -> intro_fee = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_intro_extra) {
      ptr -> intro_extra = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_intro_list) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> intro_list = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_special_memb_days) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> special_memb_days = av.intvalue;
      ptr -> OBbits__ |= 1<<2;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_visiting_fee) {
      ptr -> visiting_fee = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_affiliated_fee) {
      ptr -> affiliated_fee = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_transient_memb_days) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> transient_memb_days = av.intvalue;
      ptr -> OBbits__ |= 1<<3;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_transient_fee) {
      ptr -> transient_fee = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_retrieve_cost_per_min) {
      ptr -> retrieve_cost_per_min = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_minimum_retrieve_cost) {
      ptr -> minimum_retrieve_cost = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_credit_card_percent) {
      ptr -> credit_card_percent = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MISCRATE_credit_card_flat) {
      ptr -> credit_card_flat = DollarAmountAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = MiscRateFree(ptr);
   goto ret;
}



/**************************************************
*
*    MiscRateAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MiscRateAsnWrite(MiscRatePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MISCRATE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> year || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> year;
      retval = AsnWrite(aip, MISCRATE_year,  &av);
   }
   if (ptr -> initiation_fee != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> initiation_fee, aip, MISCRATE_initiation_fee)) {
         goto erret;
      }
   }
   if (ptr -> monthly_dues != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> monthly_dues, aip, MISCRATE_monthly_dues)) {
         goto erret;
      }
   }
   if (ptr -> ssa_regular != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> ssa_regular, aip, MISCRATE_ssa_regular)) {
         goto erret;
      }
   }
   if (ptr -> ssa_family != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> ssa_family, aip, MISCRATE_ssa_family)) {
         goto erret;
      }
   }
   if (ptr -> intro_memb_days || (ptr -> OBbits__ & (1<<1) )){   av.intvalue = ptr -> intro_memb_days;
      retval = AsnWrite(aip, MISCRATE_intro_memb_days,  &av);
   }
   if (ptr -> fast_fee != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> fast_fee, aip, MISCRATE_fast_fee)) {
         goto erret;
      }
   }
   if (ptr -> fast_extra != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> fast_extra, aip, MISCRATE_fast_extra)) {
         goto erret;
      }
   }
   if (ptr -> fast_list != NULL) {
      av.ptrvalue = ptr -> fast_list;
      retval = AsnWrite(aip, MISCRATE_fast_list,  &av);
   }
   if (ptr -> intro_fee != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> intro_fee, aip, MISCRATE_intro_fee)) {
         goto erret;
      }
   }
   if (ptr -> intro_extra != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> intro_extra, aip, MISCRATE_intro_extra)) {
         goto erret;
      }
   }
   if (ptr -> intro_list != NULL) {
      av.ptrvalue = ptr -> intro_list;
      retval = AsnWrite(aip, MISCRATE_intro_list,  &av);
   }
   if (ptr -> special_memb_days || (ptr -> OBbits__ & (1<<2) )){   av.intvalue = ptr -> special_memb_days;
      retval = AsnWrite(aip, MISCRATE_special_memb_days,  &av);
   }
   if (ptr -> visiting_fee != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> visiting_fee, aip, MISCRATE_visiting_fee)) {
         goto erret;
      }
   }
   if (ptr -> affiliated_fee != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> affiliated_fee, aip, MISCRATE_affiliated_fee)) {
         goto erret;
      }
   }
   if (ptr -> transient_memb_days || (ptr -> OBbits__ & (1<<3) )){   av.intvalue = ptr -> transient_memb_days;
      retval = AsnWrite(aip, MISCRATE_transient_memb_days,  &av);
   }
   if (ptr -> transient_fee != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> transient_fee, aip, MISCRATE_transient_fee)) {
         goto erret;
      }
   }
   if (ptr -> retrieve_cost_per_min != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> retrieve_cost_per_min, aip, MISCRATE_retrieve_cost_per_min)) {
         goto erret;
      }
   }
   if (ptr -> minimum_retrieve_cost != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> minimum_retrieve_cost, aip, MISCRATE_minimum_retrieve_cost)) {
         goto erret;
      }
   }
   if (ptr -> credit_card_percent != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> credit_card_percent, aip, MISCRATE_credit_card_percent)) {
         goto erret;
      }
   }
   if (ptr -> credit_card_flat != NULL) {
      if ( ! DollarAmountAsnWrite(ptr -> credit_card_flat, aip, MISCRATE_credit_card_flat)) {
         goto erret;
      }
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    LogSheetNew()
*
**************************************************/
NLM_EXTERN 
LogSheetPtr LIBCALL
LogSheetNew(void)
{
   LogSheetPtr ptr = MemNew((size_t) sizeof(LogSheet));

   return ptr;

}


/**************************************************
*
*    LogSheetFree()
*
**************************************************/
NLM_EXTERN 
LogSheetPtr LIBCALL
LogSheetFree(LogSheetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   CalendarDateFree(ptr -> date);
   MemFree(ptr -> version);
   MemFree(ptr -> original_version);
   MemFree(ptr -> club);
   MemFree(ptr -> airport);
   MemFree(ptr -> towplane);
   StaffDataFree(ptr -> staff);
   FlightListFree(ptr -> flights);
   GliderFlightListFree(ptr -> glider_flights);
   AirplaneFlightListFree(ptr -> airplane_flights);
   TowQueueFree(ptr -> tow_queue);
   MiscListFree(ptr -> other_charges);
   FeeListFree(ptr -> fees);
   ChargeListFree(ptr -> charges);
   AdjustmentListFree(ptr -> adjustments);
   PaymentListFree(ptr -> payments);
   AwardListFree(ptr -> awards);
   TowPlaneDataListFree(ptr -> towplane_data);
   ContactListFree(ptr -> new_contacts);
   GliderTableFree(ptr -> new_gliders);
   MemberListFree(ptr -> new_staff);
   MemFree(ptr -> expenses);
   MemFree(ptr -> comment);
   return MemFree(ptr);
}


/**************************************************
*
*    LogSheetAsnRead()
*
**************************************************/
NLM_EXTERN 
LogSheetPtr LIBCALL
LogSheetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   LogSheetPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* LogSheet ::= (self contained) */
      atp = AsnReadId(aip, amp, LOGSHEET);
   } else {
      atp = AsnLinkType(orig, LOGSHEET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = LogSheetNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == LOGSHEET_date) {
      ptr -> date = CalendarDateAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_version) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> version = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_original_version) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> original_version = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_club) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> club = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_airport) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> airport = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_towplane) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> towplane = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_no_operations) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> no_operations = av.boolvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_staff) {
      ptr -> staff = StaffDataAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_flights) {
      ptr -> flights = FlightListAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_glider_flights) {
      ptr -> glider_flights = GliderFlightListAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_airplane_flights) {
      ptr -> airplane_flights = AirplaneFlightListAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_tow_queue) {
      ptr -> tow_queue = TowQueueAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_other_charges) {
      ptr -> other_charges = MiscListAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_fees) {
      ptr -> fees = FeeListAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_charges) {
      ptr -> charges = ChargeListAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_adjustments) {
      ptr -> adjustments = AdjustmentListAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_payments) {
      ptr -> payments = PaymentListAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_awards) {
      ptr -> awards = AwardListAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_towplane_data) {
      ptr -> towplane_data = TowPlaneDataListAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_new_contacts) {
      ptr -> new_contacts = ContactListAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_new_gliders) {
      ptr -> new_gliders = GliderTableAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_new_staff) {
      ptr -> new_staff = MemberListAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_expenses) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> expenses = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_comment) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> comment = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOGSHEET_passed_validation) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> passed_validation = av.boolvalue;
      ptr -> OBbits__ |= 1<<1;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = LogSheetFree(ptr);
   goto ret;
}



/**************************************************
*
*    LogSheetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
LogSheetAsnWrite(LogSheetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, LOGSHEET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> date != NULL) {
      if ( ! CalendarDateAsnWrite(ptr -> date, aip, LOGSHEET_date)) {
         goto erret;
      }
   }
   if (ptr -> version != NULL) {
      av.ptrvalue = ptr -> version;
      retval = AsnWrite(aip, LOGSHEET_version,  &av);
   }
   if (ptr -> original_version != NULL) {
      av.ptrvalue = ptr -> original_version;
      retval = AsnWrite(aip, LOGSHEET_original_version,  &av);
   }
   if (ptr -> club != NULL) {
      av.ptrvalue = ptr -> club;
      retval = AsnWrite(aip, LOGSHEET_club,  &av);
   }
   if (ptr -> airport != NULL) {
      av.ptrvalue = ptr -> airport;
      retval = AsnWrite(aip, LOGSHEET_airport,  &av);
   }
   if (ptr -> towplane != NULL) {
      av.ptrvalue = ptr -> towplane;
      retval = AsnWrite(aip, LOGSHEET_towplane,  &av);
   }
   if (ptr -> no_operations || (ptr -> OBbits__ & (1<<0) )){   av.boolvalue = ptr -> no_operations;
      retval = AsnWrite(aip, LOGSHEET_no_operations,  &av);
   }
   if (ptr -> staff != NULL) {
      if ( ! StaffDataAsnWrite(ptr -> staff, aip, LOGSHEET_staff)) {
         goto erret;
      }
   }
   if (ptr -> flights != NULL) {
      if ( ! FlightListAsnWrite(ptr -> flights, aip, LOGSHEET_flights)) {
         goto erret;
      }
   }
   if (ptr -> glider_flights != NULL) {
      if ( ! GliderFlightListAsnWrite(ptr -> glider_flights, aip, LOGSHEET_glider_flights)) {
         goto erret;
      }
   }
   if (ptr -> airplane_flights != NULL) {
      if ( ! AirplaneFlightListAsnWrite(ptr -> airplane_flights, aip, LOGSHEET_airplane_flights)) {
         goto erret;
      }
   }
   if (ptr -> tow_queue != NULL) {
      if ( ! TowQueueAsnWrite(ptr -> tow_queue, aip, LOGSHEET_tow_queue)) {
         goto erret;
      }
   }
   if (ptr -> other_charges != NULL) {
      if ( ! MiscListAsnWrite(ptr -> other_charges, aip, LOGSHEET_other_charges)) {
         goto erret;
      }
   }
   if (ptr -> fees != NULL) {
      if ( ! FeeListAsnWrite(ptr -> fees, aip, LOGSHEET_fees)) {
         goto erret;
      }
   }
   if (ptr -> charges != NULL) {
      if ( ! ChargeListAsnWrite(ptr -> charges, aip, LOGSHEET_charges)) {
         goto erret;
      }
   }
   if (ptr -> adjustments != NULL) {
      if ( ! AdjustmentListAsnWrite(ptr -> adjustments, aip, LOGSHEET_adjustments)) {
         goto erret;
      }
   }
   if (ptr -> payments != NULL) {
      if ( ! PaymentListAsnWrite(ptr -> payments, aip, LOGSHEET_payments)) {
         goto erret;
      }
   }
   if (ptr -> awards != NULL) {
      if ( ! AwardListAsnWrite(ptr -> awards, aip, LOGSHEET_awards)) {
         goto erret;
      }
   }
   if (ptr -> towplane_data != NULL) {
      if ( ! TowPlaneDataListAsnWrite(ptr -> towplane_data, aip, LOGSHEET_towplane_data)) {
         goto erret;
      }
   }
   if (ptr -> new_contacts != NULL) {
      if ( ! ContactListAsnWrite(ptr -> new_contacts, aip, LOGSHEET_new_contacts)) {
         goto erret;
      }
   }
   if (ptr -> new_gliders != NULL) {
      if ( ! GliderTableAsnWrite(ptr -> new_gliders, aip, LOGSHEET_new_gliders)) {
         goto erret;
      }
   }
   if (ptr -> new_staff != NULL) {
      if ( ! MemberListAsnWrite(ptr -> new_staff, aip, LOGSHEET_new_staff)) {
         goto erret;
      }
   }
   if (ptr -> expenses != NULL) {
      av.ptrvalue = ptr -> expenses;
      retval = AsnWrite(aip, LOGSHEET_expenses,  &av);
   }
   if (ptr -> comment != NULL) {
      av.ptrvalue = ptr -> comment;
      retval = AsnWrite(aip, LOGSHEET_comment,  &av);
   }
   if (ptr -> passed_validation || (ptr -> OBbits__ & (1<<1) )){   av.boolvalue = ptr -> passed_validation;
      retval = AsnWrite(aip, LOGSHEET_passed_validation,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    LogSheetSetFree()
*
**************************************************/
NLM_EXTERN 
LogSheetSetPtr LIBCALL
LogSheetSetFree(LogSheetSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) LogSheetFree);
   return NULL;
}


/**************************************************
*
*    LogSheetSetAsnRead()
*
**************************************************/
NLM_EXTERN 
LogSheetSetPtr LIBCALL
LogSheetSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   LogSheetSetPtr ptr;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* LogSheetSet ::= (self contained) */
      atp = AsnReadId(aip, amp, LOGSHEETSET);
   } else {
      atp = AsnLinkType(orig, LOGSHEETSET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) LogSheetAsnRead, (AsnOptFreeFunc) LogSheetFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = LogSheetSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    LogSheetSetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
LogSheetSetAsnWrite(LogSheetSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objsscAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, LOGSHEETSET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) LogSheetAsnWrite, aip, atp, LOGSHEETSET_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

