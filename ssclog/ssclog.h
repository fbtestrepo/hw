/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Header file

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#ifndef _SSCLOG_
#define _SSCLOG_

#include <vibrant.h>
#include <document.h>
#include <asn.h>
#include <urlquery.h>
#define NLM_GENERATED_CODE_PROTO 1
#include "objssc.h"
#include "sscconv.h"

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern CharPtr program_version;

typedef struct namelist {
  ValNodePtr         names;
  ValNodePtr         extras;
  EnumFieldAssocPtr  alist;
  Boolean            need_to_update;
} NameList, PNTR NameListPtr;

typedef struct ssctables {
  NameList           members;
  NameList           justfull;
  NameList           instructors;
  NameList           justcfigs;
  NameList           towpilots;
  NameList           dutyofficers;
  NameList           assistants;
  NameList           payers;

  EnumFieldAssocPtr  field_alist;
  AirportListPtr     field_list;
  EnumFieldAssocPtr  glider_alist;
  GliderTablePtr     glider_table;
  EnumFieldAssocPtr  towplane_alist;
  TowRateTablePtr    towrate_table;
  TowPlaneSetPtr     towset_table;
  EnumFieldAssocPtr  airplane_alist;
  AirplaneTablePtr   airplane_table;
  MiscRatePtr        miscrate_table;

  CharPtr            club;
  CharPtr            airport;
  CharPtr            traconname;
  CharPtr            traconphone;
  CharPtr            submission_host;
  CharPtr            submission_path;

  MemberInfoPtr      intro_member_table;
  ValNodePtr         intro_member_this_year;

  Boolean            need_to_update;
  Boolean            need_to_auto_save;
  Boolean            allow_auto_save;
  Int2               auto_cycle;
  Int2               auto_delay;

  Char               date_string [32];
  CalendarDate       workingDate;
  CalendarDate       currentDate;
  Boolean            is_dst;

  ClockTime          currentTime;

  ForM               form;
  DialoG             staff;
  DialoG             date;
  DialoG             field;
  DialoG             towplane;
  PopuP              flash_drive;
  DialoG             checklist;
  DialoG             glider_flights;
  DialoG             airplane_flights;
  DialoG             pilotsummary;
  DialoG             instrsummary;
  DialoG             payments;
  DialoG             fees;
  DialoG             charges;
  DialoG             adjustments;
  DialoG             awards;
  DialoG             expenses;
  DialoG             towplane_data;
  DialoG             new_contacts;
  TexT               comment;
  ButtoN             no_ops;

  DialoG             intro_members;
  DialoG             guest_members;
} SscTables, PNTR SscTablesPtr;

extern DialoG CreateGliderFlightsDialog (
  GrouP prnt,
  SscTablesPtr stp,
  BaseFormPtr bfp
);
extern void SetupFlightMenus (
  MenU m,
  BaseFormPtr bfp
);

extern DialoG CreateAirplaneFlightsDialog (
  GrouP prnt,
  SscTablesPtr stp,
  BaseFormPtr bfp
);
extern DialoG CreatePilotSummaryDialog (
  GrouP prnt,
  SscTablesPtr stp,
  BaseFormPtr bfp
);
extern DialoG CreateInstrSummaryDialog (
  GrouP prnt,
  SscTablesPtr stp,
  BaseFormPtr bfp
);

extern DialoG CreatePaymentsDialog (
  GrouP prnt,
  SscTablesPtr stp
);
extern void SetupPaymentMenus (
  MenU m,
  BaseFormPtr bfp
);

#define REG_MEMB_FEE         1
#define SSC_DUES_FEE         2
#define SSA_DUES_FEE         3
#define INTRO_MEMB_FEE       4
#define VISITING_MEMB_FEE    5
#define AFFIL_MEMB_FEE       6
#define TRANS_MEMB_FEE       7
#define AERIAL_RETRIEVE_FEE  8
#define OTHER_FEE            9

extern DialoG CreateFeeDialog (
  GrouP prnt,
  SscTablesPtr stp
);

#define VOUCHER_SALE_CHARGE      1
#define BOOK_SALE_CHARGE         2
#define VIDEO_SALE_CHARGE        3
#define CLOTHING_SALE_CHARGE     4
#define PACKET_SALE_CHARGE       5
#define MERCHANDISE_SALE_CHARGE  6
#define OTHER_CHARGE             7

extern DialoG CreateChargeDialog (
  GrouP prnt,
  SscTablesPtr stp
);

#define PAY_FOR_PREVIOUS  1
#define PAY_IN_ADVANCE    2

extern DialoG CreateAdjustmentsDialog (
  GrouP prnt,
  SscTablesPtr stp
);

extern DialoG CreateStaffDialog (
  GrouP prnt,
  SscTablesPtr stp
);
extern DialoG CreateMainTowDialog (
  GrouP prnt,
  SscTablesPtr stp
);
extern DialoG CreateAirportDialog (
  GrouP prnt,
  SscTablesPtr stp
);

extern DialoG CreateBadgesDialog (
  GrouP prnt,
  SscTablesPtr stp
);
extern DialoG CreateTowplaneDialog (
  GrouP prnt,
  SscTablesPtr stp
);
extern DialoG CreateChecklistDialog (
  GrouP prnt,
  SscTablesPtr stp
);
extern DoC CreateHelpDialog (
  GrouP prnt,
  Int2 wid
);
extern void PopulateHelpDoc (
  DoC d,
  CharPtr filename
);

#define FULL_MEMBER       1
#define FAST_MEMBER       2
#define INTRO_MEMBER      3
#define VISITING_MEMBER   4
#define AFFIL_MEMBER      5
#define TRANSIENT_MEMBER  6
#define SPECIAL_MEMBER    7  /* obsolete, maps to TRANSIENT_MEMBER */
#define SERVICE_MEMBER    8
#define GUEST_MEMBER      9
#define PAYER_MEMBER     10
#define OTHER_MEMBER     11

extern DialoG CreateCurrentIntroDialog (
  GrouP prnt,
  SscTablesPtr stp
);
extern void CreateAddIntroDialog (
  GrouP prnt,
  SscTablesPtr stp
);
extern void CreateAddSpecialDialog (
  GrouP prnt,
  SscTablesPtr stp
);
extern void CreateAddFullDialog (
  GrouP prnt,
  SscTablesPtr stp
);
extern DialoG CreateAddGuestDialog (
  GrouP prnt,
  SscTablesPtr stp
);
extern void CreateAddPayerDialog (
  GrouP prnt,
  SscTablesPtr stp
);
extern void CreateAddStaffDialog (
  GrouP prnt,
  SscTablesPtr stp
);

extern DialoG CreateContactDialog (
  GrouP prnt,
  SscTablesPtr stp
);

extern DialoG CreateAddExpensesDialog (
  GrouP prnt,
  SscTablesPtr stp
);

extern SscTablesPtr GetTablesPointer (
  BaseFormPtr bfp
);

extern void UpdateFlightControls (
  DialoG d
);
extern void UpdateStaffControls (
  DialoG d
);
extern void UpdateFeesControls (
  DialoG d
);
extern void UpdateChargesControls (
  DialoG d
);
extern void UpdateAdjustmentsControls (
  DialoG d
);
extern void UpdateGuestControls (
  DialoG d
);
extern void UpdateBadgesControls (
  DialoG d
);

extern void FlightControlsTimerProc (
  DialoG d
);
extern void AirplaneControlsTimerProc (
  DialoG d
);

extern void ExportFlightsDialog (
  DialoG d,
  ValNodePtr PNTR head
);
extern void ExportAirplaneDialog (
  DialoG d,
  ValNodePtr PNTR head
);
extern void ExportFeesDialog (
  DialoG d,
  ValNodePtr PNTR head
);
extern void ExportChargesDialog (
  DialoG d,
  ValNodePtr PNTR head
);
extern void ExportAdjustmentsDialog (
  DialoG d,
  ValNodePtr PNTR head
);
extern void ExportExpensesDialog (
  DialoG d,
  ValNodePtr PNTR head
);
extern void ExportPaymentsDialog (
  DialoG d,
  ValNodePtr PNTR head
);
extern void ExportBadgesDialog (
  DialoG d,
  ValNodePtr PNTR head
);

extern void ArchiveFlightsDialog (
  DialoG d,
  FILE *fp
);
extern void ArchiveAirplaneDialog (
  DialoG d,
  FILE *fp
);
extern void ArchiveFeesDialog (
  DialoG d,
  FILE *fp
);
extern void ArchiveChargesDialog (
  DialoG d,
  FILE *fp
);
extern void ArchiveAdjustmentsDialog (
  DialoG d,
  FILE *fp
);
extern void ArchiveExpensesDialog (
  DialoG d,
  FILE *fp
);
extern void ArchivePaymentsDialog (
  DialoG d,
  FILE *fp
);
extern void ArchiveBadgesDialog (
  DialoG d,
  FILE *fp
);
extern void ArchiveTowplaneDialog (
  DialoG d,
  FILE *fp
);

extern void PrintDocCell (
  DoC d,
  Int2 item,
  Int2 col,
  Boolean tab,
  FILE *fp
);
extern void ExportDocCell (
  DoC d,
  Int2 item,
  Int2 col,
  CharPtr buf
);
extern void TabToCol (
 CharPtr buf,
 Int2 col
);
extern void FormatCell (
  CharPtr str,
  CharPtr buf,
  Int2 col,
  Int2Ptr posns,
  CharPtr justs
);

extern void KeyToFlightsDialog (
  DialoG d,
  Char key
);
extern void KeyToAirplaneDialog (
  DialoG d,
  Char key
);
extern void KeyToPaymentsDialog (
  DialoG d,
  Char key
);
extern void KeyToFeesDialog (
  DialoG d,
  Char key
);
extern void KeyToChargesDialog (
  DialoG d,
  Char key
);
extern void KeyToAdjustmentsDialog (
  DialoG d,
  Char key
);
extern void KeyToBadgesDialog (
  DialoG d,
  Char key
);
extern void KeyToContactsDialog (
  DialoG d,
  Char key
);

extern DoC GetPaymentLogDoc (
  DialoG d
);
extern Int2 GetPaymentCurrItem (
  DialoG d
);
extern void SetPaymentCurrItem (
  DialoG d,
  Int2 item
);

extern FILE* OpenAFile (
  CharPtr filename,
  CharPtr actualpath
);

extern Boolean ReadMemberTable (
  CharPtr filename,
  SscTablesPtr stp
);
extern Boolean ReadAirportTable (
  CharPtr filename,
  SscTablesPtr stp
);
extern Boolean ReadGliderTable (
  CharPtr filename,
  SscTablesPtr stp
);
extern Boolean ReadTowRateTable (
  CharPtr filename,
  SscTablesPtr stp
);
extern Boolean ReadMiscRateTable (
  CharPtr filename,
  SscTablesPtr stp
);
extern Boolean ReadFieldTable (
  CharPtr filename,
  SscTablesPtr stp
);
extern Boolean ReadTempMemberTable (
  CharPtr filename,
  SscTablesPtr stp,
  CalendarDatePtr currdate
);
extern ValNodePtr ReadPayersTable (
  CharPtr filename
);

extern void AppendTempMemberTable (
  CharPtr filename,
  CharPtr name,
  CalendarDatePtr expires
);
extern void AppendFullMemberTable (
  CharPtr filename,
  CharPtr name,
  CharPtr jobs
);

extern void RecreateNameAlist (
  NameListPtr nlp
);

extern Boolean AddNameToList (
  NameListPtr nlp,
  CharPtr name
);
extern Boolean AddNameToExtras (
  NameListPtr nlp,
  CharPtr name
);

extern Int4 GetMemberType (
  CharPtr name,
  SscTablesPtr stp
);

extern Boolean AddToTempMemberList (
  SscTablesPtr stp,
  CharPtr name,
  Int2 days
);
extern Boolean AddToFullMemberList (
  SscTablesPtr stp,
  CharPtr name,
  DialoG jobs
);

extern void AddToContactList (
  DialoG sfpcontacts,
  CharPtr name,
  Int4 type
);

extern void AddToContactListEx (
  DialoG sfpcontacts,
  CharPtr name,
  CharPtr home_phone,
  CharPtr work_phone,
  CharPtr cell_phone,
  CharPtr e_mail,
  Int4 type
);

extern void AddFeeItem (
  DialoG sfpfees,
  CharPtr name,
  Int4 type,
  CharPtr alt_payer,
  Int4 dollars,
  Int4 cents,
  Int4 reason,
  CharPtr item,
  CharPtr comment
);

extern void AddChargeItem (
  DialoG sfpcharges,
  CharPtr name,
  Int4 type,
  CharPtr alt_payer,
  Int4 dollars,
  Int4 cents,
  Int4 reason,
  CharPtr item,
  CharPtr comment
);

extern void AddAdjustItem (
  DialoG sfpadjust,
  CharPtr name,
  Int4 reason,
  Int4 dollars,
  Int4 cents
);

extern void RecalculateCharges (
  DialoG sfpflights,
  DialoG sfpfees,
  DialoG sfpcharges,
  DialoG sfpadjustments,
  DialoG sfppayments
);
extern void RecalculatePilotSummary (
  DialoG sfpflights,
  DialoG pilotsummary
);
extern void RecalculateInstrSummary (
  DialoG sfpflights,
  DialoG instrsummary
);

extern void SumPayments (
  DollarAmountPtr check_payment,
  DollarAmountPtr cash_payment,
  DollarAmountPtr account_payment,
  DollarAmountPtr voucher_payment,
  DollarAmountPtr amount_paid
);

extern Int2 ParseALine (
  CharPtr str,
  CharPtr PNTR array
);

extern CharPtr StringSaveNotEmpty (
  CharPtr str
);
extern CharPtr StringSaveForAlist (
  CharPtr str
);
extern void FixNonPrintableChars (
  CharPtr str
);
extern CharPtr FindLastName (
  CharPtr str
);

extern Boolean NormalizeAuthorCaps (
  CharPtr buf
);

extern DialoG CreateClubJobsDialog (
  GrouP prnt
);
extern void GetClubJobs (
  DialoG d,
  BoolPtr dutyofficer,
  BoolPtr assistant,
  BoolPtr instructor,
  BoolPtr towpilot
);
extern void SetClubJobs (
  DialoG d,
  Boolean dutyofficer,
  Boolean assistant,
  Boolean instructor,
  Boolean towpilot
);

extern DialoG CreateCalendarDateDialog (
  GrouP prnt,
  Pointer userdata,
  DialogActnFunc actproc
);
extern void GetCalendarDate (
  DialoG d,
  Int4Ptr month,
  Int4Ptr day,
  Int4Ptr year
);
extern void SetCalendarDate (
  DialoG d,
  Int4 month,
  Int4 day,
  Int4 year
);

extern DialoG CreateClockTimeDialog (
  GrouP prnt,
  Pointer userdata,
  DialogActnFunc actproc
);
extern void GetClockTime (
  DialoG d,
  Int4Ptr hour,
  Int4Ptr minute
);
extern void SetClockTime (
  DialoG d,
  Int4 hour,
  Int4 minute
);

extern DialoG CreateReleaseHeightDialog (
  GrouP prnt,
  Pointer userdata,
  DialogActnFunc actproc
);
extern void GetReleaseHeight (
  DialoG d,
  Int4Ptr height
);
extern void SetReleaseHeight (
  DialoG d,
  Int4 height
);

extern DialoG CreateDollarAmountDialog (
  GrouP prnt,
  Pointer userdata,
  DialogActnFunc actproc
);
extern void GetDollarAmount (
  DialoG d,
  Int4Ptr dollars,
  Int4Ptr cents
);
extern void SetDollarAmount (
  DialoG d,
  Int4 dollars,
  Int4 cents
);
extern void SelectDollars (
  DialoG d
);
extern void SelectCents (
  DialoG d
);

extern Int4 ParseInteger (
  CharPtr str,
  Int4 dfault
);
extern DollarAmountPtr ParseDollar (
  CharPtr str
);
extern FlightTimePtr ParseTime (
  CharPtr str
);
extern ClockTimePtr ParseClock (
  CharPtr str
);
extern CalendarDatePtr ParseDate (
  CharPtr str
);

#define DATE_FOR_FILE      1
#define DATE_WITH_SLASHES  2
#define DATE_FOR_DISPLAY   3
#define DATE_WITH_DAY      4

extern Boolean PrintDollarAmount (
  DollarAmountPtr dap,
  CharPtr buf
);
extern Boolean PrintWholeDollar (
  DollarAmountPtr dap,
  CharPtr buf
);
extern Boolean PrintFlightTime (
  FlightTimePtr ftp,
  CharPtr buf
);
extern Boolean PrintClockTime (
  ClockTimePtr ctp,
  CharPtr buf
);
extern Boolean PrintCalendarDate (
  CalendarDatePtr cdp,
  CharPtr buf,
  Int2 type
);

extern void ProrateDollarAmount (
  DollarAmountPtr dap,
  Int4 base_month,
  Boolean rate_is_per_year,
  SscTablesPtr stp,
  DollarAmount PNTR result
);

extern void ClearDirtyFlags (
  SscTablesPtr stp
);
extern void SetDirtyFlag (
  SscTablesPtr stp
);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* ndef _SSCLOG_ */

