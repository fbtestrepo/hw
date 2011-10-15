/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Introductory members page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* dialog controls */

typedef struct sscintro {
  DIALOG_MESSAGE_BLOCK

  GrouP         whichintro;

  GrouP         namegroup;
  TexT          name;

  TexT          home_phone;
  TexT          work_phone;
  TexT          cell_phone;
  TexT          e_mail;

  GrouP         controls [5];
  Int2          currPage;

  ButtoN        introfee;
  ButtoN        intropacket;

  ButtoN        fastpacket;

  PopuP         extenddays;

  ButtoN        addintro;

  SscTablesPtr  tables;
} SscIntro, PNTR SscIntroPtr;

static ENUM_ALIST(days_alist)
  {" ",   -1},
  {" 1",   1},
  {" 2",   2},
  {" 3",   3},
  {" 4",   4},
  {" 5",   5},
  {" 6",   6},
  {" 7",   7},
  {" 8",   8},
  {" 9",   9},
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
END_ENUM_ALIST

#define EMPTY_PAGE     0
#define ADD_FAST_PAGE  1
#define ADD_SSA_PAGE   2
#define EXTEND_PAGE    3

static void ChangeIntroType (
  GrouP g
)

{
  SscIntroPtr  sip;

  sip = (SscIntroPtr) GetObjectExtra (g);
  if (sip == NULL) return;

  SafeHide (sip->controls [sip->currPage]);
  Update ();

  sip->currPage = GetValue (g);
  SafeShow (sip->controls [sip->currPage]);
  SafeShow (sip->namegroup);
  Select (sip->name);
  Update ();
}

static void ChangeIntroName (
  TexT t
)

{
  SscIntroPtr  sip;

  sip = (SscIntroPtr) GetObjectExtra (t);
  if (sip == NULL) return;

  if (TextHasNoText (t)) {
    SafeDisable (sip->addintro);
  } else {
    SafeEnable (sip->addintro);
  }
}

static void AddIntroMember (
  ButtoN b
)

{
  Char             buf [128];
  CharPtr          cell_phone;
  DollarAmountPtr  dap;
  Int2             days = 0;
  UIEnum           dys;
  CharPtr          e_mail;
  CharPtr          home_phone;
  MiscRatePtr      mrp;
  SscIntroPtr      sip;
  SscTablesPtr     stp;
  Int2             which;
  CharPtr          work_phone;

  sip = (SscIntroPtr) GetObjectExtra (b);
  if (sip == NULL) return;
  stp = sip->tables;
  if (stp == NULL) return;
  mrp = stp->miscrate_table;
  if (mrp == NULL) return;

  GetTitle (sip->name, buf, sizeof (buf));
  if (StringHasNoText (buf)) return;
  TrimSpacesAroundString (buf);

  home_phone = SaveStringFromText (sip->home_phone);
  work_phone = SaveStringFromText (sip->work_phone);
  cell_phone = SaveStringFromText (sip->cell_phone);
  e_mail = SaveStringFromText (sip->e_mail);

  if (! NormalizeAuthorCaps (buf)) return;

  which = GetValue (sip->whichintro);
  if (which < 1) return;

  switch (which) {
    case ADD_FAST_PAGE :
      days = mrp->intro_memb_days;
      if (days < 0) {
        days = 0;
      }
      if (AddToTempMemberList (stp, buf, days)) {
        AddToContactListEx (stp->new_contacts, buf, home_phone, work_phone,
                            cell_phone, e_mail, FAST_MEMBER);

        if (GetStatus (sip->fastpacket)) {
          dap = mrp->fast_extra;
          if (dap != NULL) {
            AddChargeItem (stp->charges, buf, 0, NULL, dap->dollars, dap->cents,
                           PACKET_SALE_CHARGE, "FAST member packet", NULL);
          }
        }
      }
      break;
    case ADD_SSA_PAGE :
      days = mrp->intro_memb_days;
      if (days < 0) {
        days = 0;
      }
      if (AddToTempMemberList (stp, buf, days)) {
        AddToContactList (stp->new_contacts, buf, INTRO_MEMBER);

        if (GetStatus (sip->introfee)) {
          dap = mrp->intro_fee;
          if (dap != NULL) {
            AddFeeItem (stp->fees, buf, 0, NULL, dap->dollars, dap->cents,
                        INTRO_MEMB_FEE, NULL, NULL);
          }
        }

        if (GetStatus (sip->intropacket)) {
          dap = mrp->intro_extra;
          AddChargeItem (stp->charges, buf, 0, NULL, dap->dollars, dap->cents,
                         PACKET_SALE_CHARGE, "Intro member packet", NULL);
        }
      }
      break;
    case EXTEND_PAGE :
      if (GetEnumPopup (sip->extenddays, days_alist, &dys)) {
        days = (Int2) dys;
      }
      if (days < 0) {
        days = 0;
      }
      AddToTempMemberList (stp, buf, days);
      break;
    default :
      break;
  }

  SafeHide (sip->namegroup);

  SafeHide (sip->controls [sip->currPage]);
  SafeSetValue (sip->whichintro, 0);

  sip->currPage = 0;

  SetTitle (sip->name, "");

  SetTitle (sip->home_phone, "");
  SetTitle (sip->work_phone, "");
  SetTitle (sip->cell_phone, "");
  SetTitle (sip->e_mail, "");

  SetStatus (sip->introfee, TRUE);
  SetStatus (sip->intropacket, TRUE);
  SetStatus (sip->fastpacket, FALSE);
  SetValue (sip->extenddays, 1);

  Disable (b);

  MemFree (home_phone);
  MemFree (work_phone);
  MemFree (cell_phone);
  MemFree (e_mail);

  Update ();
}

extern void CreateAddIntroDialog (
  GrouP prnt,
  SscTablesPtr stp
)

{
  Char         buf [64];
  GrouP        g, h, k, p, n1, n2, n3;
  MiscRatePtr  mrp;
  PrompT       ppt;
  SscIntroPtr  sip;
  Char         str [16];

  if (stp == NULL) return;
  mrp = stp->miscrate_table;
  if (mrp == NULL) return;

  sip = (SscIntroPtr) MemNew (sizeof (SscIntro));
  if (sip == NULL) return;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, sip, StdCleanupExtraProc);

  sip->dialog = (DialoG) p;

  sip->tables = stp;

  sip->whichintro = HiddenGroup (p, 5, 0, ChangeIntroType);
  SetObjectExtra (sip->whichintro, sip, NULL);

  RadioButton (sip->whichintro, "FAST Coupon");
  RadioButton (sip->whichintro, "SSA Member");
  RadioButton (sip->whichintro, "Extend");

  sip->namegroup = HiddenGroup (p, 2, 0, NULL);
  SetGroupSpacing (sip->namegroup, 5, 3);

  StaticPrompt (sip->namegroup, "Name ", 0, dialogTextHeight, programFont, 'l');
  n1 = HiddenGroup (sip->namegroup, -8, 0, NULL);
  sip->name = DialogText (n1,
                          "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
                          15, ChangeIntroName);
  SetObjectExtra (sip->name, (Pointer) sip, NULL);
  SetTitle (sip->name, "");

  StaticPrompt (sip->namegroup, "Phones ", 0, dialogTextHeight, programFont, 'l');
  n2 = HiddenGroup (sip->namegroup, -8, 0, NULL);
  StaticPrompt (n2, "Home" , 0, dialogTextHeight, programFont, 'l');
  sip->home_phone = DialogText (n2, "", 7, NULL);
  SetObjectExtra (sip->home_phone, sip, NULL);
  StaticPrompt (n2, "Work", 0, dialogTextHeight, programFont, 'l');
  sip->work_phone = DialogText (n2, "", 7, NULL);
  SetObjectExtra (sip->work_phone, sip, NULL);
  StaticPrompt (n2, "Cell", 0, dialogTextHeight, programFont, 'l');
  sip->cell_phone = DialogText (n2, "", 7, NULL);
  SetObjectExtra (sip->cell_phone, sip, NULL);

  StaticPrompt (sip->namegroup, "E-Mail ", 0, dialogTextHeight, programFont, 'l');
  n3 = HiddenGroup (sip->namegroup, -8, 0, NULL);
  sip->e_mail = DialogText (n3, "", 15, NULL);
  SetObjectExtra (sip->e_mail, sip, NULL);

  StaticPrompt (sip->namegroup, " ", 0, dialogTextHeight, programFont, 'l');
  ppt = StaticPrompt (sip->namegroup, "Add full contact data in SHUTDOWN/Contact Info page",
                      0, dialogTextHeight, systemFont, 'c');

  AlignObjects (ALIGN_RIGHT, (HANDLE) sip->name, (HANDLE) sip->cell_phone,
                (HANDLE) sip->e_mail, (HANDLE) ppt, NULL);

  Hide (sip->namegroup);

  h = HiddenGroup (p, 0, 0, NULL);
  SetGroupMargins (h, 10, 10);

  g = HiddenGroup (h, -1, 0, NULL);

  Hide (g);
  sip->controls [EMPTY_PAGE] = g;

  g = HiddenGroup (h, -1, 0, NULL);

  if (PrintWholeDollar (mrp->fast_extra, str)) {
    sprintf (buf, "Charge $%s FAST Packet", str);
  } else {
    sprintf (buf, "Charge FAST Packet");
  }
  sip->fastpacket = CheckBox (g, buf, NULL);
  SetStatus (sip->fastpacket, FALSE);

  Hide (g);
  sip->controls [ADD_FAST_PAGE] = g;

  g = HiddenGroup (h, -1, 0, NULL);

  if (PrintWholeDollar (mrp->intro_fee, str)) {
    sprintf (buf, "Charge $%s Introductory Fee", str);
  } else {
    sprintf (buf, "Charge Introductory Fee");
  }
  sip->introfee = CheckBox (g, buf, NULL);
  SetStatus (sip->introfee, TRUE);

  if (PrintWholeDollar (mrp->intro_extra, str)) {
    sprintf (buf, "Charge $%s Introductory Packet", str);
  } else {
    sprintf (buf, "Charge Introductory Packet");
  }
  sip->intropacket = CheckBox (g, buf, NULL);
  SetStatus (sip->intropacket, TRUE);

  Hide (g);
  sip->controls [ADD_SSA_PAGE] = g;

  g = HiddenGroup (h, -1, 0, NULL);

  k = HiddenGroup (g, 5, 0, NULL);
  StaticPrompt (k, "Number of Days", 0, popupMenuHeight, programFont, 'l');
  sip->extenddays = PopupList (k, TRUE, NULL);
  SetObjectExtra (sip->extenddays, (Pointer) sip, NULL);
  InitEnumPopup (sip->extenddays, days_alist, NULL);
  SetValue (sip->extenddays, 1);

  Hide (g);
  sip->controls [EXTEND_PAGE] = g;

  sip->addintro = PushButton (p, "Add Intro Member", AddIntroMember);
  SetObjectExtra (sip->addintro, (Pointer) sip, NULL);
  Disable (sip->addintro);

  sip->currPage = 0;

  AlignObjects (ALIGN_CENTER,
                (HANDLE) sip->whichintro,
                (HANDLE) sip->namegroup,
                (HANDLE) sip->controls [EMPTY_PAGE],
                (HANDLE) sip->controls [ADD_FAST_PAGE],
                (HANDLE) sip->controls [ADD_SSA_PAGE],
                (HANDLE) sip->controls [EXTEND_PAGE],
                (HANDLE) sip->addintro,
                NULL);
}

