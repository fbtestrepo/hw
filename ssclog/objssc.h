#ifndef _objssc_ 
#define _objssc_ 

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" { /* } */
#endif


/**************************************************
*
*    Generated objects for Module SSC-General
*    Generated using ASNCODE Revision: 6.17 at Jul 4, 2011  9:18 PM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objsscAsnLoad PROTO((void));


/**************************************************
*
*    CalendarDate
*
**************************************************/
typedef struct struct_CalendarDate {
   Uint4 OBbits__;
   Int4   month;
   Int4   day;
   Int4   year;
#define OB__CalendarDate_weekday 0

   Int4   weekday;
} CalendarDate, PNTR CalendarDatePtr;


NLM_EXTERN CalendarDatePtr LIBCALL CalendarDateFree PROTO ((CalendarDatePtr ));
NLM_EXTERN CalendarDatePtr LIBCALL CalendarDateNew PROTO (( void ));
NLM_EXTERN CalendarDatePtr LIBCALL CalendarDateAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CalendarDateAsnWrite PROTO (( CalendarDatePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ClockTime
*
**************************************************/
typedef struct struct_ClockTime {
   Uint4 OBbits__;
   Int4   hour;
   Int4   minute;
} ClockTime, PNTR ClockTimePtr;


NLM_EXTERN ClockTimePtr LIBCALL ClockTimeFree PROTO ((ClockTimePtr ));
NLM_EXTERN ClockTimePtr LIBCALL ClockTimeNew PROTO (( void ));
NLM_EXTERN ClockTimePtr LIBCALL ClockTimeAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ClockTimeAsnWrite PROTO (( ClockTimePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    FlightTime
*
**************************************************/
typedef struct struct_FlightTime {
   Uint4 OBbits__;
   Int4   hours;
   Int4   minutes;
} FlightTime, PNTR FlightTimePtr;


NLM_EXTERN FlightTimePtr LIBCALL FlightTimeFree PROTO ((FlightTimePtr ));
NLM_EXTERN FlightTimePtr LIBCALL FlightTimeNew PROTO (( void ));
NLM_EXTERN FlightTimePtr LIBCALL FlightTimeAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL FlightTimeAsnWrite PROTO (( FlightTimePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    DollarAmount
*
**************************************************/
typedef struct struct_DollarAmount {
   Uint4 OBbits__;
   Int4   dollars;
   Int4   cents;
} DollarAmount, PNTR DollarAmountPtr;


NLM_EXTERN DollarAmountPtr LIBCALL DollarAmountFree PROTO ((DollarAmountPtr ));
NLM_EXTERN DollarAmountPtr LIBCALL DollarAmountNew PROTO (( void ));
NLM_EXTERN DollarAmountPtr LIBCALL DollarAmountAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL DollarAmountAsnWrite PROTO (( DollarAmountPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    FullName
*
**************************************************/
typedef struct struct_FullName {
   Uint4 OBbits__;
   CharPtr   last_name;
   CharPtr   first_name;
   CharPtr   middle_initials;
   CharPtr   suffix;
} FullName, PNTR FullNamePtr;


NLM_EXTERN FullNamePtr LIBCALL FullNameFree PROTO ((FullNamePtr ));
NLM_EXTERN FullNamePtr LIBCALL FullNameNew PROTO (( void ));
NLM_EXTERN FullNamePtr LIBCALL FullNameAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL FullNameAsnWrite PROTO (( FullNamePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ClubJobs
*
**************************************************/
typedef struct struct_ClubJobs {
   Uint4 OBbits__;
#define OB__ClubJobs_fly_glider 0

   Uint1   fly_glider;
#define OB__ClubJobs_instructor 1

   Uint1   instructor;
#define OB__ClubJobs_tow_pilot 2

   Uint1   tow_pilot;
#define OB__ClubJobs_demo_pilot 3

   Uint1   demo_pilot;
#define OB__ClubJobs_duty_officer 4

   Uint1   duty_officer;
#define OB__ClubJobs_assistant_d_o 5

   Uint1   assistant_d_o;
#define OB__ClubJobs_examiner 6

   Uint1   examiner;
#define OB__ClubJobs_payer 7

   Uint1   payer;
} ClubJobs, PNTR ClubJobsPtr;


NLM_EXTERN ClubJobsPtr LIBCALL ClubJobsFree PROTO ((ClubJobsPtr ));
NLM_EXTERN ClubJobsPtr LIBCALL ClubJobsNew PROTO (( void ));
NLM_EXTERN ClubJobsPtr LIBCALL ClubJobsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ClubJobsAsnWrite PROTO (( ClubJobsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    MemberInfo
*
**************************************************/
typedef struct struct_MemberInfo {
   struct struct_MemberInfo PNTR next;
   Uint4 OBbits__;
   CharPtr   name;
#define OB__MemberInfo_type 0

   Int4   type;
   struct struct_ClubJobs PNTR   jobs;
   struct struct_CalendarDate PNTR   expires;
} MemberInfo, PNTR MemberInfoPtr;


NLM_EXTERN MemberInfoPtr LIBCALL MemberInfoFree PROTO ((MemberInfoPtr ));
NLM_EXTERN MemberInfoPtr LIBCALL MemberInfoNew PROTO (( void ));
NLM_EXTERN MemberInfoPtr LIBCALL MemberInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MemberInfoAsnWrite PROTO (( MemberInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    MemberList
*
**************************************************/
typedef struct struct_MemberInfo MemberList;
typedef struct struct_MemberInfo PNTR MemberListPtr;
#define MemberListNew() MemberInfoNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN MemberListPtr LIBCALL MemberListFree PROTO ((MemberListPtr ));
NLM_EXTERN MemberListPtr LIBCALL MemberListNew PROTO (( void ));
NLM_EXTERN MemberListPtr LIBCALL MemberListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MemberListAsnWrite PROTO (( MemberListPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    ContactInfo
*
**************************************************/
typedef struct struct_ContactInfo {
   struct struct_ContactInfo PNTR next;
   Uint4 OBbits__;
   CharPtr   name;
   struct struct_FullName PNTR   full_name;
#define OB__ContactInfo_type 0

   Int4   type;
   struct struct_ClubJobs PNTR   jobs;
   struct struct_CalendarDate PNTR   expires;
   CharPtr   address;
   CharPtr   city;
   CharPtr   state;
   CharPtr   postal_code;
   CharPtr   country;
   CharPtr   home_phone;
   CharPtr   work_phone;
   CharPtr   cell_phone;
   CharPtr   e_mail;
   CharPtr   pilot_ratings;
   CharPtr   occupation;
   CharPtr   comment;
   CharPtr   ssa_number;
   struct struct_CalendarDate PNTR   ssa_expiration;
   CharPtr   fast_voucher;
} ContactInfo, PNTR ContactInfoPtr;


NLM_EXTERN ContactInfoPtr LIBCALL ContactInfoFree PROTO ((ContactInfoPtr ));
NLM_EXTERN ContactInfoPtr LIBCALL ContactInfoNew PROTO (( void ));
NLM_EXTERN ContactInfoPtr LIBCALL ContactInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ContactInfoAsnWrite PROTO (( ContactInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ContactList
*
**************************************************/
typedef struct struct_ContactInfo ContactList;
typedef struct struct_ContactInfo PNTR ContactListPtr;
#define ContactListNew() ContactInfoNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN ContactListPtr LIBCALL ContactListFree PROTO ((ContactListPtr ));
NLM_EXTERN ContactListPtr LIBCALL ContactListNew PROTO (( void ));
NLM_EXTERN ContactListPtr LIBCALL ContactListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ContactListAsnWrite PROTO (( ContactListPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    StaffData
*
**************************************************/
typedef struct struct_StaffData {
   Uint4 OBbits__;
   CharPtr   dutyofficer;
   CharPtr   assistant;
   CharPtr   timekeeper;
   CharPtr   instructor;
   CharPtr   towpilot;
   CharPtr   am_towpilot;
   CharPtr   pm_towpilot;
   CharPtr   comment;
} StaffData, PNTR StaffDataPtr;


NLM_EXTERN StaffDataPtr LIBCALL StaffDataFree PROTO ((StaffDataPtr ));
NLM_EXTERN StaffDataPtr LIBCALL StaffDataNew PROTO (( void ));
NLM_EXTERN StaffDataPtr LIBCALL StaffDataAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL StaffDataAsnWrite PROTO (( StaffDataPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AirplaneInfo
*
**************************************************/
typedef struct struct_AirplaneInfo {
   struct struct_AirplaneInfo PNTR next;
   Uint4 OBbits__;
   CharPtr   airplane;
   CharPtr   tail_number;
   struct struct_DollarAmount PNTR   cost_per_min;
   struct struct_DollarAmount PNTR   cost_per_flight;
#define OB__AirplaneInfo_number_of_seats 0

   Int4   number_of_seats;
   struct struct_FlightTime PNTR   max_rental_time;
   ValNodePtr   fly_for_free;
} AirplaneInfo, PNTR AirplaneInfoPtr;


NLM_EXTERN AirplaneInfoPtr LIBCALL AirplaneInfoFree PROTO ((AirplaneInfoPtr ));
NLM_EXTERN AirplaneInfoPtr LIBCALL AirplaneInfoNew PROTO (( void ));
NLM_EXTERN AirplaneInfoPtr LIBCALL AirplaneInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AirplaneInfoAsnWrite PROTO (( AirplaneInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AirplaneTable
*
**************************************************/
typedef struct struct_AirplaneInfo AirplaneTable;
typedef struct struct_AirplaneInfo PNTR AirplaneTablePtr;
#define AirplaneTableNew() AirplaneInfoNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN AirplaneTablePtr LIBCALL AirplaneTableFree PROTO ((AirplaneTablePtr ));
NLM_EXTERN AirplaneTablePtr LIBCALL AirplaneTableNew PROTO (( void ));
NLM_EXTERN AirplaneTablePtr LIBCALL AirplaneTableAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AirplaneTableAsnWrite PROTO (( AirplaneTablePtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    GliderInfo
*
**************************************************/
typedef struct struct_GliderInfo {
   struct struct_GliderInfo PNTR next;
   Uint4 OBbits__;
   CharPtr   glider;
   CharPtr   tail_number;
   struct struct_DollarAmount PNTR   cost_per_min;
   struct struct_DollarAmount PNTR   cost_per_flight;
#define OB__GliderInfo_number_of_seats 0

   Int4   number_of_seats;
   struct struct_FlightTime PNTR   max_rental_time;
   ValNodePtr   fly_for_free;
} GliderInfo, PNTR GliderInfoPtr;


NLM_EXTERN GliderInfoPtr LIBCALL GliderInfoFree PROTO ((GliderInfoPtr ));
NLM_EXTERN GliderInfoPtr LIBCALL GliderInfoNew PROTO (( void ));
NLM_EXTERN GliderInfoPtr LIBCALL GliderInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL GliderInfoAsnWrite PROTO (( GliderInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    GliderTable
*
**************************************************/
typedef struct struct_GliderInfo GliderTable;
typedef struct struct_GliderInfo PNTR GliderTablePtr;
#define GliderTableNew() GliderInfoNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN GliderTablePtr LIBCALL GliderTableFree PROTO ((GliderTablePtr ));
NLM_EXTERN GliderTablePtr LIBCALL GliderTableNew PROTO (( void ));
NLM_EXTERN GliderTablePtr LIBCALL GliderTableAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL GliderTableAsnWrite PROTO (( GliderTablePtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    TowPlaneInfo
*
**************************************************/
typedef struct struct_TowPlaneInfo {
   struct struct_TowPlaneInfo PNTR next;
   Uint4 OBbits__;
   CharPtr   towplane;
   CharPtr   tail_number;
} TowPlaneInfo, PNTR TowPlaneInfoPtr;


NLM_EXTERN TowPlaneInfoPtr LIBCALL TowPlaneInfoFree PROTO ((TowPlaneInfoPtr ));
NLM_EXTERN TowPlaneInfoPtr LIBCALL TowPlaneInfoNew PROTO (( void ));
NLM_EXTERN TowPlaneInfoPtr LIBCALL TowPlaneInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL TowPlaneInfoAsnWrite PROTO (( TowPlaneInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    TowPlaneTable
*
**************************************************/
typedef struct struct_TowPlaneInfo TowPlaneTable;
typedef struct struct_TowPlaneInfo PNTR TowPlaneTablePtr;
#define TowPlaneTableNew() TowPlaneInfoNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN TowPlaneTablePtr LIBCALL TowPlaneTableFree PROTO ((TowPlaneTablePtr ));
NLM_EXTERN TowPlaneTablePtr LIBCALL TowPlaneTableNew PROTO (( void ));
NLM_EXTERN TowPlaneTablePtr LIBCALL TowPlaneTableAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL TowPlaneTableAsnWrite PROTO (( TowPlaneTablePtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    TowRateItem
*
**************************************************/
typedef struct struct_TowRateItem {
   struct struct_TowRateItem PNTR next;
   Uint4 OBbits__;
   Int4   altitude;
   struct struct_DollarAmount PNTR   cost_of_tow;
} TowRateItem, PNTR TowRateItemPtr;


NLM_EXTERN TowRateItemPtr LIBCALL TowRateItemFree PROTO ((TowRateItemPtr ));
NLM_EXTERN TowRateItemPtr LIBCALL TowRateItemNew PROTO (( void ));
NLM_EXTERN TowRateItemPtr LIBCALL TowRateItemAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL TowRateItemAsnWrite PROTO (( TowRateItemPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    TowRateTable
*
**************************************************/
typedef struct struct_TowRateItem TowRateTable;
typedef struct struct_TowRateItem PNTR TowRateTablePtr;
#define TowRateTableNew() TowRateItemNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN TowRateTablePtr LIBCALL TowRateTableFree PROTO ((TowRateTablePtr ));
NLM_EXTERN TowRateTablePtr LIBCALL TowRateTableNew PROTO (( void ));
NLM_EXTERN TowRateTablePtr LIBCALL TowRateTableAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL TowRateTableAsnWrite PROTO (( TowRateTablePtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    TowPlaneList
*
**************************************************/
typedef struct struct_TowPlaneList {
   struct struct_TowPlaneList PNTR next;
   Uint4 OBbits__;
   CharPtr   towplane;
   CharPtr   tail_number;
   struct struct_TowRateItem PNTR   rates;
} TowPlaneList, PNTR TowPlaneListPtr;


NLM_EXTERN TowPlaneListPtr LIBCALL TowPlaneListFree PROTO ((TowPlaneListPtr ));
NLM_EXTERN TowPlaneListPtr LIBCALL TowPlaneListNew PROTO (( void ));
NLM_EXTERN TowPlaneListPtr LIBCALL TowPlaneListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL TowPlaneListAsnWrite PROTO (( TowPlaneListPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    TowPlaneSet
*
**************************************************/
typedef struct struct_TowPlaneList TowPlaneSet;
typedef struct struct_TowPlaneList PNTR TowPlaneSetPtr;
#define TowPlaneSetNew() TowPlaneListNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN TowPlaneSetPtr LIBCALL TowPlaneSetFree PROTO ((TowPlaneSetPtr ));
NLM_EXTERN TowPlaneSetPtr LIBCALL TowPlaneSetNew PROTO (( void ));
NLM_EXTERN TowPlaneSetPtr LIBCALL TowPlaneSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL TowPlaneSetAsnWrite PROTO (( TowPlaneSetPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    AirportInfo
*
**************************************************/
typedef struct struct_AirportInfo {
   struct struct_AirportInfo PNTR next;
   Uint4 OBbits__;
   CharPtr   airport;
   CharPtr   code;
   CharPtr   suffix;
#define OB__AirportInfo_altitude 0

   Int4   altitude;
} AirportInfo, PNTR AirportInfoPtr;


NLM_EXTERN AirportInfoPtr LIBCALL AirportInfoFree PROTO ((AirportInfoPtr ));
NLM_EXTERN AirportInfoPtr LIBCALL AirportInfoNew PROTO (( void ));
NLM_EXTERN AirportInfoPtr LIBCALL AirportInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AirportInfoAsnWrite PROTO (( AirportInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AirportList
*
**************************************************/
typedef struct struct_AirportInfo AirportList;
typedef struct struct_AirportInfo PNTR AirportListPtr;
#define AirportListNew() AirportInfoNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN AirportListPtr LIBCALL AirportListFree PROTO ((AirportListPtr ));
NLM_EXTERN AirportListPtr LIBCALL AirportListNew PROTO (( void ));
NLM_EXTERN AirportListPtr LIBCALL AirportListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AirportListAsnWrite PROTO (( AirportListPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    GliderFlight
*
**************************************************/
typedef struct struct_GliderFlight {
   struct struct_GliderFlight PNTR next;
   Uint4 OBbits__;
#define OB__GliderFlight_flight_number 0

   Int4   flight_number;
   CharPtr   name;
#define OB__GliderFlight_type 1

   Int4   type;
   CharPtr   alt_payer;
#define OB__GliderFlight_split_payment 2

   Int4   split_payment;
   CharPtr   passenger;
   CharPtr   glider;
#define OB__GliderFlight_category 3

   Int4   category;
   CharPtr   airport;
   CharPtr   towplane;
   CharPtr   instructor;
   CharPtr   towpilot;
   struct struct_ClockTime PNTR   takeoff;
   struct struct_ClockTime PNTR   landing;
#define OB__GliderFlight_exception 4

   Int4   exception;
#define OB__GliderFlight_landed_out 5

   Uint1   landed_out;
   struct struct_FlightTime PNTR   flight_time;
   Int4   release_altitude;
   struct struct_DollarAmount PNTR   flight_cost;
   struct struct_DollarAmount PNTR   tow_cost;
   struct struct_DollarAmount PNTR   total_cost;
   CharPtr   comment;
} GliderFlight, PNTR GliderFlightPtr;


NLM_EXTERN GliderFlightPtr LIBCALL GliderFlightFree PROTO ((GliderFlightPtr ));
NLM_EXTERN GliderFlightPtr LIBCALL GliderFlightNew PROTO (( void ));
NLM_EXTERN GliderFlightPtr LIBCALL GliderFlightAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL GliderFlightAsnWrite PROTO (( GliderFlightPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    GliderFlightList
*
**************************************************/
typedef struct struct_GliderFlight GliderFlightList;
typedef struct struct_GliderFlight PNTR GliderFlightListPtr;
#define GliderFlightListNew() GliderFlightNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN GliderFlightListPtr LIBCALL GliderFlightListFree PROTO ((GliderFlightListPtr ));
NLM_EXTERN GliderFlightListPtr LIBCALL GliderFlightListNew PROTO (( void ));
NLM_EXTERN GliderFlightListPtr LIBCALL GliderFlightListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL GliderFlightListAsnWrite PROTO (( GliderFlightListPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    FlightList
*
**************************************************/
typedef struct struct_GliderFlight FlightList;
typedef struct struct_GliderFlight PNTR FlightListPtr;
#define FlightListNew() GliderFlightNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN FlightListPtr LIBCALL FlightListFree PROTO ((FlightListPtr ));
NLM_EXTERN FlightListPtr LIBCALL FlightListNew PROTO (( void ));
NLM_EXTERN FlightListPtr LIBCALL FlightListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL FlightListAsnWrite PROTO (( FlightListPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    AirplaneFlight
*
**************************************************/
typedef struct struct_AirplaneFlight {
   struct struct_AirplaneFlight PNTR next;
   Uint4 OBbits__;
#define OB__AirplaneFlight_flight_number 0

   Int4   flight_number;
   CharPtr   name;
#define OB__AirplaneFlight_type 1

   Int4   type;
   CharPtr   alt_payer;
#define OB__AirplaneFlight_split_payment 2

   Int4   split_payment;
   CharPtr   passenger;
   CharPtr   airplane;
#define OB__AirplaneFlight_category 3

   Int4   category;
   CharPtr   airport;
   CharPtr   instructor;
   struct struct_ClockTime PNTR   takeoff;
   struct struct_ClockTime PNTR   landing;
#define OB__AirplaneFlight_exception 4

   Int4   exception;
   struct struct_FlightTime PNTR   flight_time;
   struct struct_DollarAmount PNTR   flight_cost;
   CharPtr   comment;
} AirplaneFlight, PNTR AirplaneFlightPtr;


NLM_EXTERN AirplaneFlightPtr LIBCALL AirplaneFlightFree PROTO ((AirplaneFlightPtr ));
NLM_EXTERN AirplaneFlightPtr LIBCALL AirplaneFlightNew PROTO (( void ));
NLM_EXTERN AirplaneFlightPtr LIBCALL AirplaneFlightAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AirplaneFlightAsnWrite PROTO (( AirplaneFlightPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AirplaneFlightList
*
**************************************************/
typedef struct struct_AirplaneFlight AirplaneFlightList;
typedef struct struct_AirplaneFlight PNTR AirplaneFlightListPtr;
#define AirplaneFlightListNew() AirplaneFlightNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN AirplaneFlightListPtr LIBCALL AirplaneFlightListFree PROTO ((AirplaneFlightListPtr ));
NLM_EXTERN AirplaneFlightListPtr LIBCALL AirplaneFlightListNew PROTO (( void ));
NLM_EXTERN AirplaneFlightListPtr LIBCALL AirplaneFlightListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AirplaneFlightListAsnWrite PROTO (( AirplaneFlightListPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    TowRequest
*
**************************************************/
typedef struct struct_TowRequest {
   struct struct_TowRequest PNTR next;
   Uint4 OBbits__;
   CharPtr   name;
   struct struct_ClockTime PNTR   request_time;
   CharPtr   glider;
   CharPtr   towplane;
   CharPtr   instructor;
#define OB__TowRequest_result 0

   Int4   result;
} TowRequest, PNTR TowRequestPtr;


NLM_EXTERN TowRequestPtr LIBCALL TowRequestFree PROTO ((TowRequestPtr ));
NLM_EXTERN TowRequestPtr LIBCALL TowRequestNew PROTO (( void ));
NLM_EXTERN TowRequestPtr LIBCALL TowRequestAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL TowRequestAsnWrite PROTO (( TowRequestPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    TowQueue
*
**************************************************/
typedef struct struct_TowRequest TowQueue;
typedef struct struct_TowRequest PNTR TowQueuePtr;
#define TowQueueNew() TowRequestNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN TowQueuePtr LIBCALL TowQueueFree PROTO ((TowQueuePtr ));
NLM_EXTERN TowQueuePtr LIBCALL TowQueueNew PROTO (( void ));
NLM_EXTERN TowQueuePtr LIBCALL TowQueueAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL TowQueueAsnWrite PROTO (( TowQueuePtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    AwardData
*
**************************************************/
typedef struct struct_AwardData {
   struct struct_AwardData PNTR next;
   Uint4 OBbits__;
   CharPtr   name;
#define OB__AwardData_type 0

   Int4   type;
   Int4   reason;
   CharPtr   comment;
} AwardData, PNTR AwardDataPtr;


NLM_EXTERN AwardDataPtr LIBCALL AwardDataFree PROTO ((AwardDataPtr ));
NLM_EXTERN AwardDataPtr LIBCALL AwardDataNew PROTO (( void ));
NLM_EXTERN AwardDataPtr LIBCALL AwardDataAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AwardDataAsnWrite PROTO (( AwardDataPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AwardList
*
**************************************************/
typedef struct struct_AwardData AwardList;
typedef struct struct_AwardData PNTR AwardListPtr;
#define AwardListNew() AwardDataNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN AwardListPtr LIBCALL AwardListFree PROTO ((AwardListPtr ));
NLM_EXTERN AwardListPtr LIBCALL AwardListNew PROTO (( void ));
NLM_EXTERN AwardListPtr LIBCALL AwardListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AwardListAsnWrite PROTO (( AwardListPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    TowPlaneData
*
**************************************************/
typedef struct struct_TowPlaneData {
   struct struct_TowPlaneData PNTR next;
   Uint4 OBbits__;
   CharPtr   towplane;
   CharPtr   start_tach;
   CharPtr   end_tach;
   CharPtr   tach_time;
   CharPtr   gas_added;
   CharPtr   comment;
} TowPlaneData, PNTR TowPlaneDataPtr;


NLM_EXTERN TowPlaneDataPtr LIBCALL TowPlaneDataFree PROTO ((TowPlaneDataPtr ));
NLM_EXTERN TowPlaneDataPtr LIBCALL TowPlaneDataNew PROTO (( void ));
NLM_EXTERN TowPlaneDataPtr LIBCALL TowPlaneDataAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL TowPlaneDataAsnWrite PROTO (( TowPlaneDataPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    TowPlaneDataList
*
**************************************************/
typedef struct struct_TowPlaneData TowPlaneDataList;
typedef struct struct_TowPlaneData PNTR TowPlaneDataListPtr;
#define TowPlaneDataListNew() TowPlaneDataNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN TowPlaneDataListPtr LIBCALL TowPlaneDataListFree PROTO ((TowPlaneDataListPtr ));
NLM_EXTERN TowPlaneDataListPtr LIBCALL TowPlaneDataListNew PROTO (( void ));
NLM_EXTERN TowPlaneDataListPtr LIBCALL TowPlaneDataListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL TowPlaneDataListAsnWrite PROTO (( TowPlaneDataListPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    MerchandiseInfo
*
**************************************************/
typedef struct struct_MerchandiseInfo {
   struct struct_MerchandiseInfo PNTR next;
   Uint4 OBbits__;
   CharPtr   item;
#define OB__MerchandiseInfo_category 0

   Int4   category;
   struct struct_DollarAmount PNTR   cost;
} MerchandiseInfo, PNTR MerchandiseInfoPtr;


NLM_EXTERN MerchandiseInfoPtr LIBCALL MerchandiseInfoFree PROTO ((MerchandiseInfoPtr ));
NLM_EXTERN MerchandiseInfoPtr LIBCALL MerchandiseInfoNew PROTO (( void ));
NLM_EXTERN MerchandiseInfoPtr LIBCALL MerchandiseInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MerchandiseInfoAsnWrite PROTO (( MerchandiseInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    MerchandiseList
*
**************************************************/
typedef struct struct_MerchandiseInfo MerchandiseList;
typedef struct struct_MerchandiseInfo PNTR MerchandiseListPtr;
#define MerchandiseListNew() MerchandiseInfoNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN MerchandiseListPtr LIBCALL MerchandiseListFree PROTO ((MerchandiseListPtr ));
NLM_EXTERN MerchandiseListPtr LIBCALL MerchandiseListNew PROTO (( void ));
NLM_EXTERN MerchandiseListPtr LIBCALL MerchandiseListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MerchandiseListAsnWrite PROTO (( MerchandiseListPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    FeeData
*
**************************************************/
typedef struct struct_FeeData {
   struct struct_FeeData PNTR next;
   Uint4 OBbits__;
   CharPtr   name;
#define OB__FeeData_type 0

   Int4   type;
   CharPtr   alt_payer;
   struct struct_DollarAmount PNTR   amount;
   Int4   reason;
   CharPtr   item;
   CharPtr   comment;
} FeeData, PNTR FeeDataPtr;


NLM_EXTERN FeeDataPtr LIBCALL FeeDataFree PROTO ((FeeDataPtr ));
NLM_EXTERN FeeDataPtr LIBCALL FeeDataNew PROTO (( void ));
NLM_EXTERN FeeDataPtr LIBCALL FeeDataAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL FeeDataAsnWrite PROTO (( FeeDataPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    FeeList
*
**************************************************/
typedef struct struct_FeeData FeeList;
typedef struct struct_FeeData PNTR FeeListPtr;
#define FeeListNew() FeeDataNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN FeeListPtr LIBCALL FeeListFree PROTO ((FeeListPtr ));
NLM_EXTERN FeeListPtr LIBCALL FeeListNew PROTO (( void ));
NLM_EXTERN FeeListPtr LIBCALL FeeListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL FeeListAsnWrite PROTO (( FeeListPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    ChargeData
*
**************************************************/
typedef struct struct_ChargeData {
   struct struct_ChargeData PNTR next;
   Uint4 OBbits__;
   CharPtr   name;
#define OB__ChargeData_type 0

   Int4   type;
   CharPtr   alt_payer;
   struct struct_DollarAmount PNTR   amount;
   Int4   reason;
   CharPtr   item;
   CharPtr   comment;
} ChargeData, PNTR ChargeDataPtr;


NLM_EXTERN ChargeDataPtr LIBCALL ChargeDataFree PROTO ((ChargeDataPtr ));
NLM_EXTERN ChargeDataPtr LIBCALL ChargeDataNew PROTO (( void ));
NLM_EXTERN ChargeDataPtr LIBCALL ChargeDataAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ChargeDataAsnWrite PROTO (( ChargeDataPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ChargeList
*
**************************************************/
typedef struct struct_ChargeData ChargeList;
typedef struct struct_ChargeData PNTR ChargeListPtr;
#define ChargeListNew() ChargeDataNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN ChargeListPtr LIBCALL ChargeListFree PROTO ((ChargeListPtr ));
NLM_EXTERN ChargeListPtr LIBCALL ChargeListNew PROTO (( void ));
NLM_EXTERN ChargeListPtr LIBCALL ChargeListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ChargeListAsnWrite PROTO (( ChargeListPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    MiscCharge
*
**************************************************/
typedef struct struct_MiscCharge {
   struct struct_MiscCharge PNTR next;
   Uint4 OBbits__;
   CharPtr   name;
#define OB__MiscCharge_type 0

   Int4   type;
   CharPtr   alt_payer;
   struct struct_DollarAmount PNTR   amount;
   Int4   reason;
   CharPtr   item;
   CharPtr   comment;
} MiscCharge, PNTR MiscChargePtr;


NLM_EXTERN MiscChargePtr LIBCALL MiscChargeFree PROTO ((MiscChargePtr ));
NLM_EXTERN MiscChargePtr LIBCALL MiscChargeNew PROTO (( void ));
NLM_EXTERN MiscChargePtr LIBCALL MiscChargeAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MiscChargeAsnWrite PROTO (( MiscChargePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    MiscList
*
**************************************************/
typedef struct struct_MiscCharge MiscList;
typedef struct struct_MiscCharge PNTR MiscListPtr;
#define MiscListNew() MiscChargeNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN MiscListPtr LIBCALL MiscListFree PROTO ((MiscListPtr ));
NLM_EXTERN MiscListPtr LIBCALL MiscListNew PROTO (( void ));
NLM_EXTERN MiscListPtr LIBCALL MiscListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MiscListAsnWrite PROTO (( MiscListPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    AdjustData
*
**************************************************/
typedef struct struct_AdjustData {
   struct struct_AdjustData PNTR next;
   Uint4 OBbits__;
   CharPtr   name;
#define OB__AdjustData_type 0

   Int4   type;
   CharPtr   alt_payer;
   struct struct_DollarAmount PNTR   amount;
   Int4   reason;
   CharPtr   comment;
} AdjustData, PNTR AdjustDataPtr;


NLM_EXTERN AdjustDataPtr LIBCALL AdjustDataFree PROTO ((AdjustDataPtr ));
NLM_EXTERN AdjustDataPtr LIBCALL AdjustDataNew PROTO (( void ));
NLM_EXTERN AdjustDataPtr LIBCALL AdjustDataAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AdjustDataAsnWrite PROTO (( AdjustDataPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AdjustmentList
*
**************************************************/
typedef struct struct_AdjustData AdjustmentList;
typedef struct struct_AdjustData PNTR AdjustmentListPtr;
#define AdjustmentListNew() AdjustDataNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN AdjustmentListPtr LIBCALL AdjustmentListFree PROTO ((AdjustmentListPtr ));
NLM_EXTERN AdjustmentListPtr LIBCALL AdjustmentListNew PROTO (( void ));
NLM_EXTERN AdjustmentListPtr LIBCALL AdjustmentListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AdjustmentListAsnWrite PROTO (( AdjustmentListPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    PaymentInfo
*
**************************************************/
typedef struct struct_PaymentInfo {
   struct struct_PaymentInfo PNTR next;
   Uint4 OBbits__;
   CharPtr   payer;
#define OB__PaymentInfo_type 0

   Int4   type;
   struct struct_DollarAmount PNTR   charges;
   struct struct_DollarAmount PNTR   check_payment;
   struct struct_DollarAmount PNTR   cash_payment;
   struct struct_DollarAmount PNTR   credit_payment;
   struct struct_DollarAmount PNTR   credit_surcharge;
   struct struct_DollarAmount PNTR   account_payment;
   struct struct_DollarAmount PNTR   voucher_payment;
   CharPtr   voucher_serial;
   CharPtr   comment;
} PaymentInfo, PNTR PaymentInfoPtr;


NLM_EXTERN PaymentInfoPtr LIBCALL PaymentInfoFree PROTO ((PaymentInfoPtr ));
NLM_EXTERN PaymentInfoPtr LIBCALL PaymentInfoNew PROTO (( void ));
NLM_EXTERN PaymentInfoPtr LIBCALL PaymentInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL PaymentInfoAsnWrite PROTO (( PaymentInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    PaymentList
*
**************************************************/
typedef struct struct_PaymentInfo PaymentList;
typedef struct struct_PaymentInfo PNTR PaymentListPtr;
#define PaymentListNew() PaymentInfoNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN PaymentListPtr LIBCALL PaymentListFree PROTO ((PaymentListPtr ));
NLM_EXTERN PaymentListPtr LIBCALL PaymentListNew PROTO (( void ));
NLM_EXTERN PaymentListPtr LIBCALL PaymentListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL PaymentListAsnWrite PROTO (( PaymentListPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    MiscRate
*
**************************************************/
typedef struct struct_MiscRate {
   Uint4 OBbits__;
#define OB__MiscRate_year 0

   Int4   year;
   struct struct_DollarAmount PNTR   initiation_fee;
   struct struct_DollarAmount PNTR   monthly_dues;
   struct struct_DollarAmount PNTR   ssa_regular;
   struct struct_DollarAmount PNTR   ssa_family;
#define OB__MiscRate_intro_memb_days 1

   Int4   intro_memb_days;
   struct struct_DollarAmount PNTR   fast_fee;
   struct struct_DollarAmount PNTR   fast_extra;
   CharPtr   fast_list;
   struct struct_DollarAmount PNTR   intro_fee;
   struct struct_DollarAmount PNTR   intro_extra;
   CharPtr   intro_list;
#define OB__MiscRate_special_memb_days 2

   Int4   special_memb_days;
   struct struct_DollarAmount PNTR   visiting_fee;
   struct struct_DollarAmount PNTR   affiliated_fee;
#define OB__MiscRate_transient_memb_days 3

   Int4   transient_memb_days;
   struct struct_DollarAmount PNTR   transient_fee;
   struct struct_DollarAmount PNTR   retrieve_cost_per_min;
   struct struct_DollarAmount PNTR   minimum_retrieve_cost;
   struct struct_DollarAmount PNTR   credit_card_percent;
   struct struct_DollarAmount PNTR   credit_card_flat;
} MiscRate, PNTR MiscRatePtr;


NLM_EXTERN MiscRatePtr LIBCALL MiscRateFree PROTO ((MiscRatePtr ));
NLM_EXTERN MiscRatePtr LIBCALL MiscRateNew PROTO (( void ));
NLM_EXTERN MiscRatePtr LIBCALL MiscRateAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MiscRateAsnWrite PROTO (( MiscRatePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    LogSheet
*
**************************************************/
typedef struct struct_LogSheet {
   struct struct_LogSheet PNTR next;
   Uint4 OBbits__;
   struct struct_CalendarDate PNTR   date;
   CharPtr   version;
   CharPtr   original_version;
   CharPtr   club;
   CharPtr   airport;
   CharPtr   towplane;
#define OB__LogSheet_no_operations 0

   Uint1   no_operations;
   struct struct_StaffData PNTR   staff;
   struct struct_GliderFlight PNTR   flights;
   struct struct_GliderFlight PNTR   glider_flights;
   struct struct_AirplaneFlight PNTR   airplane_flights;
   struct struct_TowRequest PNTR   tow_queue;
   struct struct_MiscCharge PNTR   other_charges;
   struct struct_FeeData PNTR   fees;
   struct struct_ChargeData PNTR   charges;
   struct struct_AdjustData PNTR   adjustments;
   struct struct_PaymentInfo PNTR   payments;
   struct struct_AwardData PNTR   awards;
   struct struct_TowPlaneData PNTR   towplane_data;
   struct struct_ContactInfo PNTR   new_contacts;
   struct struct_GliderInfo PNTR   new_gliders;
   struct struct_MemberInfo PNTR   new_staff;
   CharPtr   expenses;
   CharPtr   comment;
#define OB__LogSheet_passed_validation 1

   Uint1   passed_validation;
} LogSheet, PNTR LogSheetPtr;


NLM_EXTERN LogSheetPtr LIBCALL LogSheetFree PROTO ((LogSheetPtr ));
NLM_EXTERN LogSheetPtr LIBCALL LogSheetNew PROTO (( void ));
NLM_EXTERN LogSheetPtr LIBCALL LogSheetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL LogSheetAsnWrite PROTO (( LogSheetPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    LogSheetSet
*
**************************************************/
typedef struct struct_LogSheet LogSheetSet;
typedef struct struct_LogSheet PNTR LogSheetSetPtr;
#define LogSheetSetNew() LogSheetNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN LogSheetSetPtr LIBCALL LogSheetSetFree PROTO ((LogSheetSetPtr ));
NLM_EXTERN LogSheetSetPtr LIBCALL LogSheetSetNew PROTO (( void ));
NLM_EXTERN LogSheetSetPtr LIBCALL LogSheetSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL LogSheetSetAsnWrite PROTO (( LogSheetSetPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objssc_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

