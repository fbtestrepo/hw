/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Contact information page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* internal version of ContactInfoPtr */

typedef struct intctcinfo {
  CharPtr          name;
  Int4             type;
  CharPtr          address;
  CharPtr          city;
  CharPtr          state;
  CharPtr          postal_code;
  CharPtr          country;
  CharPtr          home_phone;
  CharPtr          work_phone;
  CharPtr          cell_phone;
  CharPtr          e_mail;
  CharPtr          pilot_ratings;
  CharPtr          occupation;
  CharPtr          comment;
  CharPtr          ssanumber;
  CalendarDatePtr  ssaexpdate;
} IntCtcInfo, PNTR IntCtcPtr;

/* dialog controls */

typedef struct sscctc {
  DIALOG_MESSAGE_BLOCK

  DoC           ctclog;
  Int2          currItem;

  TexT          name;
  PopuP         type;
  TexT          address;
  TexT          city;
  TexT          state;
  TexT          postal_code;
  TexT          country;
  TexT          home_phone;
  TexT          work_phone;
  TexT          cell_phone;
  TexT          e_mail;
  TexT          pilot_ratings;
  TexT          occupation;
  TexT          comment;
  TexT          ssanumber;
  DialoG        ssaexpdate;

  Int2          numCtcs;
  IntCtcPtr     contacts [128];

  SscTablesPtr  tables;
} SscCtc, PNTR SscCtcPtr;

#define NUM_CONTACT_LINES 4

static IntCtcPtr IntCtcInfoNew (
  void
)

{
  IntCtcPtr  icp;

  icp = MemNew (sizeof (IntCtcInfo));
  if (icp == NULL) return NULL;

  return icp;
}

static IntCtcPtr IntCtcInfoFree (
  IntCtcPtr icp
)

{
  if (icp == NULL) return NULL;

  MemFree (icp->name);
  MemFree (icp->address);
  MemFree (icp->city);
  MemFree (icp->state);
  MemFree (icp->postal_code);
  MemFree (icp->country);
  MemFree (icp->home_phone);
  MemFree (icp->work_phone);
  MemFree (icp->cell_phone);
  MemFree (icp->e_mail);
  MemFree (icp->pilot_ratings);
  MemFree (icp->occupation);
  MemFree (icp->ssanumber);
  MemFree (icp->comment);

  return MemFree (icp);
}

static IntCtcPtr GetCtcInfo (
  SscCtcPtr scp,
  Int2 item
)

{
  IntCtcPtr  icp;

  if (scp == NULL || item < 0 || item > 127) return NULL;
  icp = scp->contacts [item];
  if (icp != NULL) return icp;

  icp = IntCtcInfoNew ();
  if (icp != NULL) {
    icp->name = StringSave (" ");
    /*
    icp->comment = StringSave (" ");
    */
  }
  scp->contacts [item] = icp;
  return icp;
}

static ParData  ctcParFmt = { FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0 };

static ColData  ctcColFmt [] = {
  {0, 5, 25, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, TRUE}   /* name */
};

static ENUM_ALIST(member_type_alist)
  {" ",          0},
  {"Regular",    FULL_MEMBER},
  {"FAST",       FAST_MEMBER},
  {"Intro",      INTRO_MEMBER},
  {"Visiting",   VISITING_MEMBER},
  {"Affiliated", AFFIL_MEMBER},
  {"Transient",  TRANSIENT_MEMBER},
  {"Service",    SERVICE_MEMBER},
  {"Guest",      GUEST_MEMBER},
  {"Payer",      PAYER_MEMBER},
  {"Other",      OTHER_MEMBER},
END_ENUM_ALIST

static CharPtr PrintCtcTable (
  DoC d,
  Int2 item,
  Pointer data
)

{
  Char       buf [256];
  IntCtcPtr  icp;
  SscCtcPtr  scp;

  scp = (SscCtcPtr) GetObjectExtra (d);
  if (scp == NULL || item < 1 || item > 127) return NULL;
  icp = GetCtcInfo (scp, item);
  if (icp == NULL) return NULL;

  buf [0] = '\0';

  if (StringDoesHaveText (icp->name)) {
    StringCat (buf, icp->name);
  } else {
    StringCat (buf, " ");
  }

  StringCat (buf, "\n");
  return StringSave (buf);
}

static void ChangeCtcTableSelect (
  DoC d,
  Int2 item,
  Int2 row,
  Int2 col,
  Boolean dblClck
)

{
  IntCtcPtr     icp;
  Int2          itemOld1, itemOld2;
  SscCtcPtr     scp;
  SscTablesPtr  stp;

  scp = (SscCtcPtr) GetObjectExtra (d);
  if (scp == NULL) return;
  stp = scp->tables;
  if (stp == NULL) return;
  if (item == 0 || row == 0 || col == 0) return;

  GetDocHighlight (d, &itemOld1, &itemOld2);
  SetDocHighlight (d, item, item);
  UpdateDocument (d, itemOld1, itemOld2);
  UpdateDocument (d, item, item);
  scp->currItem = item;

  ResetClip ();

  icp = GetCtcInfo (scp, scp->currItem);
  if (icp != NULL) {
    SetTitle (scp->name, icp->name);
    SetEnumPopup (scp->type, member_type_alist, (UIEnum) icp->type);
    SetTitle (scp->address, icp->address);
    SetTitle (scp->city, icp->city);
    SetTitle (scp->state, icp->state);
    SetTitle (scp->postal_code, icp->postal_code);
    SetTitle (scp->country, icp->country);
    SetTitle (scp->home_phone, icp->home_phone);
    SetTitle (scp->work_phone, icp->work_phone);
    SetTitle (scp->cell_phone, icp->cell_phone);
    SetTitle (scp->e_mail, icp->e_mail);
    SetTitle (scp->pilot_ratings, icp->pilot_ratings);
    SetTitle (scp->occupation, icp->occupation);
    SetTitle (scp->ssanumber, icp->ssanumber);
    PointerToDialog (scp->ssaexpdate, icp->ssaexpdate);
    SetTitle (scp->comment, icp->comment);
  }

  Update ();
}

static void CheckExtendTable (
  SscCtcPtr scp
)

{
  Int2  numItems;

  if (scp == NULL) return;

  GetDocParams (scp->ctclog, &numItems, NULL);
  if (scp->currItem == numItems) {
    AppendItem (scp->ctclog, PrintCtcTable, scp, FALSE, 1,
                &ctcParFmt, ctcColFmt, systemFont);
  }

  Update ();

  SetDirtyFlag (scp->tables);
}

static void ChangeName (
  TexT t
)

{
  IntCtcPtr  icp;
  SscCtcPtr  scp;

  scp = (SscCtcPtr) GetObjectExtra (t);
  if (scp == NULL) return;
  icp = GetCtcInfo (scp, scp->currItem);
  if (icp == NULL) return;

  icp->name = MemFree (icp->name);
  icp->name = SaveStringFromText (t);

  UpdateDocument (scp->ctclog, scp->currItem, scp->currItem);
  Update ();

  CheckExtendTable (scp);
}

static void ChangeType (
  PopuP p
)

{
  IntCtcPtr  icp;
  SscCtcPtr  scp;
  UIEnum     val;

  scp = (SscCtcPtr) GetObjectExtra (p);
  if (scp == NULL) return;
  icp = GetCtcInfo (scp, scp->currItem);
  if (icp == NULL) return;

  if (GetEnumPopup (scp->type, member_type_alist, &val)) {
    icp->type = (Int4) val;
  }

  CheckExtendTable (scp);
}

static void ChangeAddress (
  TexT t
)

{
  IntCtcPtr  icp;
  SscCtcPtr  scp;

  scp = (SscCtcPtr) GetObjectExtra (t);
  if (scp == NULL) return;
  icp = GetCtcInfo (scp, scp->currItem);
  if (icp == NULL) return;

  icp->address = MemFree (icp->address);
  icp->address = SaveStringFromText (t);

  CheckExtendTable (scp);
}

static void ChangeCity (
  TexT t
)

{
  IntCtcPtr  icp;
  SscCtcPtr  scp;

  scp = (SscCtcPtr) GetObjectExtra (t);
  if (scp == NULL) return;
  icp = GetCtcInfo (scp, scp->currItem);
  if (icp == NULL) return;

  icp->city = MemFree (icp->city);
  icp->city = SaveStringFromText (t);

  CheckExtendTable (scp);
}

static void ChangeState (
  TexT t
)

{
  IntCtcPtr  icp;
  SscCtcPtr  scp;

  scp = (SscCtcPtr) GetObjectExtra (t);
  if (scp == NULL) return;
  icp = GetCtcInfo (scp, scp->currItem);
  if (icp == NULL) return;

  icp->state = MemFree (icp->state);
  icp->state = SaveStringFromText (t);

  CheckExtendTable (scp);
}

static void ChangeZipCode (
  TexT t
)

{
  IntCtcPtr  icp;
  SscCtcPtr  scp;

  scp = (SscCtcPtr) GetObjectExtra (t);
  if (scp == NULL) return;
  icp = GetCtcInfo (scp, scp->currItem);
  if (icp == NULL) return;

  icp->postal_code = MemFree (icp->postal_code);
  icp->postal_code = SaveStringFromText (t);

  CheckExtendTable (scp);
}

static void ChangeCountry (
  TexT t
)

{
  IntCtcPtr  icp;
  SscCtcPtr  scp;

  scp = (SscCtcPtr) GetObjectExtra (t);
  if (scp == NULL) return;
  icp = GetCtcInfo (scp, scp->currItem);
  if (icp == NULL) return;

  icp->country = MemFree (icp->country);
  icp->country = SaveStringFromText (t);

  CheckExtendTable (scp);
}

static void ChangeHomePhone (
  TexT t
)

{
  IntCtcPtr  icp;
  SscCtcPtr  scp;

  scp = (SscCtcPtr) GetObjectExtra (t);
  if (scp == NULL) return;
  icp = GetCtcInfo (scp, scp->currItem);
  if (icp == NULL) return;

  icp->home_phone = MemFree (icp->home_phone);
  icp->home_phone = SaveStringFromText (t);

  CheckExtendTable (scp);
}

static void ChangeWorkPhone (
  TexT t
)

{
  IntCtcPtr  icp;
  SscCtcPtr  scp;

  scp = (SscCtcPtr) GetObjectExtra (t);
  if (scp == NULL) return;
  icp = GetCtcInfo (scp, scp->currItem);
  if (icp == NULL) return;

  icp->work_phone = MemFree (icp->work_phone);
  icp->work_phone = SaveStringFromText (t);

  CheckExtendTable (scp);
}

static void ChangeCellPhone (
  TexT t
)

{
  IntCtcPtr  icp;
  SscCtcPtr  scp;

  scp = (SscCtcPtr) GetObjectExtra (t);
  if (scp == NULL) return;
  icp = GetCtcInfo (scp, scp->currItem);
  if (icp == NULL) return;

  icp->cell_phone = MemFree (icp->cell_phone);
  icp->cell_phone = SaveStringFromText (t);

  CheckExtendTable (scp);
}

static void ChangeEMail (
  TexT t
)

{
  IntCtcPtr  icp;
  SscCtcPtr  scp;

  scp = (SscCtcPtr) GetObjectExtra (t);
  if (scp == NULL) return;
  icp = GetCtcInfo (scp, scp->currItem);
  if (icp == NULL) return;

  icp->e_mail = MemFree (icp->e_mail);
  icp->e_mail = SaveStringFromText (t);

  CheckExtendTable (scp);
}

static void ChangePilotRatings (
  TexT t
)

{
  IntCtcPtr  icp;
  SscCtcPtr  scp;

  scp = (SscCtcPtr) GetObjectExtra (t);
  if (scp == NULL) return;
  icp = GetCtcInfo (scp, scp->currItem);
  if (icp == NULL) return;

  icp->pilot_ratings = MemFree (icp->pilot_ratings);
  icp->pilot_ratings = SaveStringFromText (t);

  CheckExtendTable (scp);
}

static void ChangeOccupation (
  TexT t
)

{
  IntCtcPtr  icp;
  SscCtcPtr  scp;

  scp = (SscCtcPtr) GetObjectExtra (t);
  if (scp == NULL) return;
  icp = GetCtcInfo (scp, scp->currItem);
  if (icp == NULL) return;

  icp->occupation = MemFree (icp->occupation);
  icp->occupation = SaveStringFromText (t);

  CheckExtendTable (scp);
}

static void ChangeSSANumber (
  TexT t
)

{
  IntCtcPtr  icp;
  SscCtcPtr  scp;

  scp = (SscCtcPtr) GetObjectExtra (t);
  if (scp == NULL) return;
  icp = GetCtcInfo (scp, scp->currItem);
  if (icp == NULL) return;

  icp->ssanumber = MemFree (icp->ssanumber);
  icp->ssanumber = SaveStringFromText (t);

  CheckExtendTable (scp);
}

static void ChangeSSADate (
  DialoG d
)

{
  BaseDialogPtr    bdp;
  CalendarDatePtr  cdp;
  IntCtcPtr        icp;
  SscCtcPtr        scp;

  bdp = (BaseDialogPtr) GetObjectExtra (d);
  if (bdp == NULL) return;
  scp = (SscCtcPtr) bdp->userdata;
  if (scp == NULL) return;
  icp = GetCtcInfo (scp, scp->currItem);
  if (icp == NULL) return;

  icp->ssaexpdate = CalendarDateFree (icp->ssaexpdate);
  cdp = DialogToPointer (d);
  if (cdp != NULL) {
    if (cdp->month == -1 || cdp->year == -1) {
      cdp = CalendarDateFree (cdp);
    }
  }
  icp->ssaexpdate = cdp;

  CheckExtendTable (scp);
}

static void ChangeComment (
  TexT t
)

{
  IntCtcPtr  icp;
  SscCtcPtr  scp;

  scp = (SscCtcPtr) GetObjectExtra (t);
  if (scp == NULL) return;
  icp = GetCtcInfo (scp, scp->currItem);
  if (icp == NULL) return;

  icp->comment = MemFree (icp->comment);
  icp->comment = SaveStringFromText (t);

  CheckExtendTable (scp);
}

extern void KeyToContactsDialog (DialoG d, Char key)

{
  Int2       numItems, offset;
  BaR        sb;
  SscCtcPtr  scp;

  scp = (SscCtcPtr) GetObjectExtra (d);
  if (scp == NULL) return;

  if (key == NLM_UP) {
    if (scp->currItem > 1) {
      (scp->currItem)--;
    }
  } else if (key == NLM_DOWN) {
    GetDocParams (scp->ctclog, &numItems, NULL);
    if (scp->currItem < numItems) {
      (scp->currItem)++;
    }
  }

  sb = GetSlateVScrollBar ((SlatE) scp->ctclog);
  offset = GetValue (sb);

  if (key == NLM_DOWN && offset < scp->currItem - NUM_CONTACT_LINES && scp->currItem > 0) {
    SetValue (sb, scp->currItem - NUM_CONTACT_LINES);
  }
  if (key == NLM_UP && scp->currItem <= offset) {
    SetValue (sb, scp->currItem - 1);
  }

  ChangeCtcTableSelect (scp->ctclog, scp->currItem, 1, 1, FALSE);

  Update ();
}

static void ContactListPtrToContactPage (DialoG d, Pointer data)

{
  ContactInfoPtr  cip, contacts;
  Int2            i;
  IntCtcPtr       icp;
  Int2            k;
  SscCtcPtr       scp;
  SscTablesPtr    stp;

  scp = (SscCtcPtr) GetObjectExtra (d);
  if (scp == NULL) return;
  stp = scp->tables;
  if (stp == NULL) return;
  contacts = (ContactInfoPtr) data;

  Reset (scp->ctclog);
  scp->numCtcs = 0;
  scp->currItem = 1;

  for (k = 0; k < 128; k++) {
    icp = scp->contacts [k];
    IntCtcInfoFree (icp);
    scp->contacts [k] = NULL;
  }

  if (contacts == NULL) {
    Reset (scp->ctclog);
    SetTitle (scp->name, "");
    SetValue (scp->type, 0);
    SetTitle (scp->address, "");
    SetTitle (scp->city, "");
    SetTitle (scp->state, "");
    SetTitle (scp->postal_code, "");
    SetTitle (scp->country, "");
    SetTitle (scp->home_phone, "");
    SetTitle (scp->work_phone, "");
    SetTitle (scp->cell_phone, "");
    SetTitle (scp->e_mail, "");
    SetTitle (scp->pilot_ratings, "");
    SetTitle (scp->occupation, "");
    SetTitle (scp->ssanumber, "");
    PointerToDialog (scp->ssaexpdate, NULL);
    SetTitle (scp->comment, "");
    scp->numCtcs = 0;
    scp->currItem = 1;
    for (i = 0; i < NUM_CONTACT_LINES; i++) {
      AppendItem (scp->ctclog, PrintCtcTable, scp, FALSE, 1,
                  &ctcParFmt, ctcColFmt, systemFont);
    }
    SetDocHighlight (scp->ctclog, 1, 1);
    return;
  }

  for (cip = contacts, k = 1; cip != NULL; cip = cip->next, k++) {
    icp = GetCtcInfo (scp, k);
    if (icp == NULL) continue;

    icp->name = StringSaveNotEmpty (cip->name);
    icp->type = cip->type;
    icp->address = StringSaveNotEmpty (cip->address);
    icp->city = StringSaveNotEmpty (cip->city);
    icp->state = StringSaveNotEmpty (cip->state);
    icp->postal_code = StringSaveNotEmpty (cip->postal_code);
    icp->country = StringSaveNotEmpty (cip->country);
    icp->home_phone = StringSaveNotEmpty (cip->home_phone);
    icp->work_phone = StringSaveNotEmpty (cip->work_phone);
    icp->cell_phone = StringSaveNotEmpty (cip->cell_phone);
    icp->e_mail = StringSaveNotEmpty (cip->e_mail);
    icp->pilot_ratings = StringSaveNotEmpty (cip->pilot_ratings);
    icp->occupation = StringSaveNotEmpty (cip->occupation);
    icp->ssanumber = StringSaveNotEmpty (cip->ssa_number);
    if (cip->ssa_expiration != NULL) {
      icp->ssaexpdate= AsnIoMemCopy (cip->ssa_expiration,
                                            (AsnReadFunc) CalendarDateAsnRead,
                                            (AsnWriteFunc) CalendarDateAsnWrite);
    }
    icp->comment = StringSaveNotEmpty (cip->comment);

    AppendItem (scp->ctclog, PrintCtcTable, scp, FALSE, 1,
                &ctcParFmt, ctcColFmt, systemFont);

    (scp->numCtcs)++;
  }

  AppendItem (scp->ctclog, PrintCtcTable, scp, FALSE, 1,
              &ctcParFmt, ctcColFmt, systemFont);
  k++;

  while (k <= NUM_CONTACT_LINES) {
    AppendItem (scp->ctclog, PrintCtcTable, scp, FALSE, 1,
                &ctcParFmt, ctcColFmt, systemFont);
    k++;
  }

  UpdateDocument (scp->ctclog, 0, 0);

  ChangeCtcTableSelect (scp->ctclog, 1, 1, 1, FALSE);

  Update ();
}

static Pointer ContactPageToContactListPtr (DialoG d)

{
  CalendarDatePtr  cdp;
  ContactInfoPtr   contacts, cip, lastcip;
  IntCtcPtr        icp;
  Int2             k, numItems;
  SscCtcPtr        scp;
  SscTablesPtr     stp;

  scp = (SscCtcPtr) GetObjectExtra (d);
  if (scp == NULL) return NULL;
  stp = scp->tables;
  if (stp == NULL) return NULL;

  contacts = NULL;

  lastcip = NULL;
  GetDocParams (scp->ctclog, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    icp = GetCtcInfo (scp, k);
    if (icp == NULL) continue;
    if (StringHasNoText (icp->name)) continue;
    cip = ContactInfoNew ();
    if (cip == NULL) continue;

    cip->name = StringSaveNotEmpty (icp->name);
    cip->type = icp->type;
    cip->address = StringSaveNotEmpty (icp->address);
    cip->city = StringSaveNotEmpty (icp->city);
    cip->state = StringSaveNotEmpty (icp->state);
    cip->postal_code = StringSaveNotEmpty (icp->postal_code);
    cip->country = StringSaveNotEmpty (icp->country);
    cip->home_phone = StringSaveNotEmpty (icp->home_phone);
    cip->work_phone = StringSaveNotEmpty (icp->work_phone);
    cip->cell_phone = StringSaveNotEmpty (icp->cell_phone);
    cip->e_mail = StringSaveNotEmpty (icp->e_mail);
    cip->pilot_ratings = StringSaveNotEmpty (icp->pilot_ratings);
    cip->occupation = StringSaveNotEmpty (icp->occupation);
    cip->ssa_number = StringSaveNotEmpty (icp->ssanumber);
    cdp = icp->ssaexpdate;
    if (cdp != NULL && cdp->month != -1 && cdp->year != -1) {
      cip->ssa_expiration = AsnIoMemCopy (icp->ssaexpdate,
                                          (AsnReadFunc) CalendarDateAsnRead,
                                          (AsnWriteFunc) CalendarDateAsnWrite);
    }
    cip->comment = StringSaveNotEmpty (icp->comment);

    if (contacts == NULL) {
      contacts = cip;
    }
    if (lastcip != NULL) {
      lastcip->next = cip;
    }
    lastcip = cip;
  }

  return (Pointer) contacts;
}

extern void AddToContactList (
  DialoG sfpcontacts,
  CharPtr name,
  Int4 type
)

{
  ContactInfoPtr  cip, contacts;
  SscCtcPtr       scp;
  SscTablesPtr    stp;

  scp = (SscCtcPtr) GetObjectExtra (sfpcontacts);
  if (scp == NULL) return;
  stp = scp->tables;
  if (stp == NULL) return;

  contacts = DialogToPointer (stp->new_contacts);
  cip = ContactInfoNew ();
  if (cip != NULL) {
    cip->name = StringSave (name);
    cip->type = type;
  }
  if (contacts != NULL) {
    if (cip != NULL) {
      cip->next = contacts;
    }
    contacts = cip;
  } else {
    contacts = cip;
  }
  PointerToDialog (stp->new_contacts, contacts);
}

extern void AddToContactListEx (
  DialoG sfpcontacts,
  CharPtr name,
  CharPtr home_phone,
  CharPtr work_phone,
  CharPtr cell_phone,
  CharPtr e_mail,
  Int4 type
)

{
  ContactInfoPtr  cip, contacts;
  SscCtcPtr       scp;
  SscTablesPtr    stp;

  scp = (SscCtcPtr) GetObjectExtra (sfpcontacts);
  if (scp == NULL) return;
  stp = scp->tables;
  if (stp == NULL) return;

  contacts = DialogToPointer (stp->new_contacts);
  cip = ContactInfoNew ();
  if (cip != NULL) {
    cip->name = StringSave (name);
    cip->home_phone = StringSaveNoNull (home_phone);
    cip->work_phone = StringSaveNoNull (work_phone);
    cip->cell_phone = StringSaveNoNull (cell_phone);
    cip->e_mail = StringSaveNoNull (e_mail);
    cip->type = type;
  }
  if (contacts != NULL) {
    if (cip != NULL) {
      cip->next = contacts;
    }
    contacts = cip;
  } else {
    contacts = cip;
  }
  PointerToDialog (stp->new_contacts, contacts);
}

extern DialoG CreateContactDialog (
  GrouP prnt,
  SscTablesPtr stp
)

{
  GrouP      cts, tbl, g0, g1, g2, g3, g4, g5, g6, g9, p;
  FonT       fnt;
  Int2       i, hgt;
  SscCtcPtr  scp;

  scp = (SscCtcPtr) MemNew (sizeof (SscCtc));
  if (scp == NULL) return NULL;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, scp, StdCleanupExtraProc);
  scp->dialog = (DialoG) p;
  scp->todialog = ContactListPtrToContactPage;
  scp->fromdialog = ContactPageToContactListPtr;

  scp->tables = stp;

  SelectFont (systemFont);
  hgt = LineHeight ();
  ctcColFmt [0].pixWidth = 45 * StringWidth ("X") + 5;
  SelectFont (systemFont);

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
  StaticPrompt (g0, "Name", ctcColFmt [0].pixWidth, 0, fnt, 'c');

  scp->ctclog = DocumentPanel (tbl, ctcColFmt [0].pixWidth + 2, NUM_CONTACT_LINES * hgt + 2);
  SetObjectExtra (scp->ctclog, scp, NULL);
  SetDocCache (scp->ctclog, NULL, NULL, NULL);
  SetDocNotify (scp->ctclog, ChangeCtcTableSelect);
  scp->numCtcs = 0;

  for (i = 0; i < NUM_CONTACT_LINES; i++) {
    AppendItem (scp->ctclog, PrintCtcTable, scp, FALSE, 1,
                &ctcParFmt, ctcColFmt, systemFont);
  }

  cts = HiddenGroup (p, -1, 0, NULL);
  SetGroupSpacing (cts, 10, 10);
  SetGroupMargins (cts, 5, 5);

  g1 = HiddenGroup (cts, 2, 0, NULL);
  SetGroupSpacing (g1, 10, 5);

  StaticPrompt (g1, "Name", 0, dialogTextHeight, programFont, 'l');
  g9 = HiddenGroup (g1, 5, 0, NULL);
  scp->name = DialogText (g9, "", 21, ChangeName);
  SetObjectExtra (scp->name, scp, NULL);
  StaticPrompt (g9, "Type", 0, popupMenuHeight, programFont, 'l');
  scp->type = CreateEnumPopupDialog (g9, TRUE, ChangeType, member_type_alist, (UIEnum) 0, scp);
  SetObjectExtra (scp->type, scp, NULL);

  StaticPrompt (g1, "Address", 0, dialogTextHeight, programFont, 'l');
  scp->address = DialogText (g1, "", 12, ChangeAddress);
  SetObjectExtra (scp->address, scp, NULL);

  StaticPrompt (g1, "City", 0, dialogTextHeight, programFont, 'l');
  g2 = HiddenGroup (g1, -6, 0, NULL);
  scp->city = DialogText (g2, "", 13, ChangeCity);
  SetObjectExtra (scp->city, scp, NULL);
  StaticPrompt (g2, "State", 0, dialogTextHeight, programFont, 'l');
  scp->state = DialogText (g2, "", 3, ChangeState);
  SetObjectExtra (scp->state, scp, NULL);
  StaticPrompt (g2, "Zip Code", 0, dialogTextHeight, programFont, 'l');
  scp->postal_code = DialogText (g2, "", 5, ChangeZipCode);
  SetObjectExtra (scp->postal_code, scp, NULL);

  StaticPrompt (g1, "Country", 0, dialogTextHeight, programFont, 'l');
  g5 = HiddenGroup (g1, -6, 0, NULL);
  scp->country = DialogText (g5, "", 10, ChangeCountry);
  SetObjectExtra (scp->country, scp, NULL);
  StaticPrompt (g5, "E-Mail", 0, dialogTextHeight, programFont, 'l');
  scp->e_mail = DialogText (g5, "", 11, ChangeEMail);
  SetObjectExtra (scp->e_mail, scp, NULL);

  StaticPrompt (g1, "Phones", 0, dialogTextHeight, programFont, 'l');
  g3 = HiddenGroup (g1, -6, 0, NULL);
  StaticPrompt (g3, "Home", 0, dialogTextHeight, programFont, 'l');
  scp->home_phone = DialogText (g3, "", 7, ChangeHomePhone);
  SetObjectExtra (scp->home_phone, scp, NULL);
  StaticPrompt (g3, "Work", 0, dialogTextHeight, programFont, 'l');
  scp->work_phone = DialogText (g3, "", 7, ChangeWorkPhone);
  SetObjectExtra (scp->work_phone, scp, NULL);
  StaticPrompt (g3, "Cell", 0, dialogTextHeight, programFont, 'l');
  scp->cell_phone = DialogText (g3, "", 7, ChangeCellPhone);
  SetObjectExtra (scp->cell_phone, scp, NULL);

  StaticPrompt (g1, "Ratings", 0, dialogTextHeight, programFont, 'l');
  g4 = HiddenGroup (g1, -6, 0, NULL);
  scp->pilot_ratings = DialogText (g4, "", 12, ChangePilotRatings);
  SetObjectExtra (scp->pilot_ratings, scp, NULL);
  StaticPrompt (g4, "Occupation", 0, dialogTextHeight, programFont, 'l');
  scp->occupation = DialogText (g4, "", 9, ChangeOccupation);
  SetObjectExtra (scp->occupation, scp, NULL);

  StaticPrompt (g1, "SSA", 0, dialogTextHeight, programFont, 'l');
  g6 = HiddenGroup (g1, -5, 0, NULL);
  StaticPrompt (g6, "Number", 0, popupMenuHeight, programFont, 'l');
  scp->ssanumber = DialogText (g6, "", 5, ChangeSSANumber);
  SetObjectExtra (scp->ssanumber, scp, NULL);
  StaticPrompt (g6, "Expiration Date", 0, popupMenuHeight, programFont, 'l');
  scp->ssaexpdate = CreateCalendarDateDialog (g6, (Pointer) scp, ChangeSSADate);
  PointerToDialog (scp->ssaexpdate, NULL);

  StaticPrompt (g1, "Comment", 0, dialogTextHeight, programFont, 'l');
  scp->comment = DialogText (g1, "", 12, ChangeComment);
  SetObjectExtra (scp->comment, scp, NULL);

  AlignObjects (ALIGN_CENTER, (HANDLE) tbl, (HANDLE) cts, NULL);

  AlignObjects (ALIGN_RIGHT, (HANDLE) scp->address, (HANDLE) scp->postal_code,
                (HANDLE) scp->e_mail, (HANDLE) scp->cell_phone, (HANDLE) scp->occupation,
                (HANDLE) scp->comment, NULL);

  scp->numCtcs = 0;
  scp->currItem = 1;

  return (DialoG) p;
}

