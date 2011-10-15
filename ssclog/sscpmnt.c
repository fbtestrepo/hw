/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Payments page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* internal version of PaymentInfoPtr */

typedef struct intpayinfo {
  CharPtr       payer;
  Int4          type;
  DollarAmount  charges;
  DollarAmount  check_payment;
  DollarAmount  cash_payment;
  DollarAmount  account_payment;
  DollarAmount  voucher_payment;
  CharPtr       voucher_serial;
  CharPtr       comment;
} IntPaymentInfo, PNTR IntPaymentPtr;

/* dialog controls */

typedef struct sscpayments {
  DIALOG_MESSAGE_BLOCK

  DoC            paymentlog;
  Int2           currItem;

  PrompT         payer;
  PrompT         charges;
  DialoG         check;
  DialoG         cash;
  ButtoN         acct_button;
  DialoG         account;
  ButtoN         use_voucher;
  GrouP          vouch_ser_group;
  TexT           voucher_serial;
  GrouP          add_adv_group;
  ButtoN         add_adv_button;
  TexT           comment;

  PrompT         ttl_charges;
  PrompT         ttl_check;
  PrompT         ttl_cash;
  PrompT         ttl_account;
  PrompT         ttl_voucher;
  PrompT         ttl_payments;

  DollarAmount   tot_chg;
  DollarAmount   tot_check;
  DollarAmount   tot_cash;
  DollarAmount   tot_account;
  DollarAmount   tot_voucher;
  DollarAmount   tot_pay;

  Int2           numPayments;
  IntPaymentPtr  payments [128];

  SscTablesPtr   tables;
} SscPayments, PNTR SscPaymentsPtr;

#define NUM_PAYMENT_LINES 8

static IntPaymentPtr IntPaymentInfoNew (
  void
)

{
  DollarAmountPtr  dap;
  IntPaymentPtr    ipp;

  ipp = (IntPaymentPtr) MemNew (sizeof (IntPaymentInfo));
  if (ipp == NULL) return NULL;

  dap = &(ipp->charges);
  dap->dollars = -1;
  dap->cents = -1;

  dap = &(ipp->check_payment);
  dap->dollars = -1;
  dap->cents = -1;

  dap = &(ipp->cash_payment);
  dap->dollars = -1;
  dap->cents = -1;

  dap = &(ipp->account_payment);
  dap->dollars = -1;
  dap->cents = -1;

  dap = &(ipp->voucher_payment);
  dap->dollars = -1;
  dap->cents = -1;

  return ipp;
}

static IntPaymentPtr IntPaymentInfoFree (
  IntPaymentPtr ipp
)

{
  if (ipp == NULL) return NULL;

  MemFree (ipp->payer);
  MemFree (ipp->voucher_serial);
  MemFree (ipp->comment);

  return MemFree (ipp);
}

static IntPaymentPtr GetPaymentInfo (
  SscPaymentsPtr spp,
  Int2 item
)

{
  IntPaymentPtr  ipp;

  if (spp == NULL || item < 0 || item > 127) return NULL;
  ipp = spp->payments [item];
  if (ipp != NULL) return ipp;

  ipp = IntPaymentInfoNew ();
  if (ipp != NULL) {
    ipp->payer = StringSave (" ");
  }
  spp->payments [item] = ipp;
  return ipp;
}

static ParData  paymentParFmt = { FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0 };

static ColData  paymentColFmt [] = {
  {0, 5, 25, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE},  /* payee   */
  {0, 5, 10, 2, NULL, 'r', FALSE, TRUE,  FALSE, FALSE, FALSE},  /* charges */
  {0, 5, 10, 2, NULL, 'r', FALSE, TRUE,  FALSE, FALSE, FALSE},  /* check   */
  {0, 5, 10, 2, NULL, 'r', FALSE, TRUE,  FALSE, FALSE, FALSE},  /* cash    */
  {0, 5, 10, 2, NULL, 'r', FALSE, TRUE,  FALSE, FALSE, FALSE},  /* account */
  {0, 5, 10, 2, NULL, 'r', FALSE, TRUE,  FALSE, FALSE, FALSE},  /* voucher */
  {0, 5, 10, 2, NULL, 'r', FALSE, TRUE,  FALSE, FALSE, TRUE}    /* total   */
};

extern void SumPayments (
  DollarAmountPtr check_payment,
  DollarAmountPtr cash_payment,
  DollarAmountPtr account_payment,
  DollarAmountPtr voucher_payment,
  DollarAmountPtr amount_paid
)

{
  Int4             cents, dollars;
  DollarAmountPtr  dap;

  if (amount_paid == NULL) return;
  amount_paid->dollars = 0;
  amount_paid->cents = 0;

  dollars = 0;
  cents = 0;
  dap = check_payment;
  if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
    dollars += dap->dollars;
    cents += dap->cents;
  }
  dap = cash_payment;
  if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
    dollars += dap->dollars;
    cents += dap->cents;
  }
  dap = account_payment;
  if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
    dollars += dap->dollars;
    cents += dap->cents;
  }
  dap = voucher_payment;
  if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
    dollars += dap->dollars;
    cents += dap->cents;
  }
  if (cents >= 100) {
    dollars += cents / 100;
    cents = cents % 100;
  }
  dap = amount_paid;
  if (dap != NULL) {
    dap->dollars = dollars;
    dap->cents = cents;
  }
}

static void DiffPayments (
  DollarAmountPtr charge,
  DollarAmountPtr payment,
  DollarAmountPtr overpaid
)

{
  Int4  chg, pyd, delta;

  if (overpaid == NULL) return;
  overpaid->dollars = 0;
  overpaid->cents = 0;
  if (charge == NULL || payment == NULL) return;

  chg = charge->dollars * 100 + charge->cents;
  pyd = payment->dollars * 100 + payment->cents;
  if (pyd <= chg) return;

  delta = pyd - chg;
  overpaid->dollars = delta / 100;
  overpaid->cents = delta % 100;
}

#define NOCHARGE   0
#define UNPAID     1
#define PAID       2
#define UNDERPAID  3
#define OVERPAID  4

static Int2 CalculatePaymentStatus (
  DollarAmountPtr charges,
  DollarAmountPtr check_payment,
  DollarAmountPtr cash_payment,
  DollarAmountPtr account_payment,
  DollarAmountPtr voucher_payment,
  DollarAmountPtr overpayment
)

{
  DollarAmount  amount_paid;

  if (charges == NULL) return NOCHARGE;

  SumPayments (check_payment, cash_payment, account_payment, voucher_payment, &amount_paid);

  if ((charges->dollars <= 0 && charges->cents <= 0) ||
      (charges->dollars == 0 && charges->cents == 0)) {
    if (amount_paid.dollars >= 0 && amount_paid.cents >= 0) {
      if (amount_paid.dollars > 0 || amount_paid.cents > 0) {
        DiffPayments (charges, &amount_paid, overpayment);
        return OVERPAID;
      }
    }
    return NOCHARGE;
  }
  /*
  if (charges->dollars <= 0 && charges->cents <= 0) return NOCHARGE;
  if (charges->dollars == 0 && charges->cents == 0) return NOCHARGE;
  */

  if (amount_paid.dollars <= 0 && amount_paid.cents <= 0) {
    return UNPAID;
  } else if (amount_paid.dollars == charges->dollars &&
             amount_paid.cents == charges->cents) {
    return PAID;
  } else if (amount_paid.dollars < charges->dollars) {
    return UNDERPAID;
  } else if (amount_paid.dollars == charges->dollars &&
             amount_paid.cents < charges->cents) {
    return UNDERPAID;
  } else if (amount_paid.dollars > charges->dollars) {
    DiffPayments (charges, &amount_paid, overpayment);
    return OVERPAID;
  } else if (amount_paid.dollars == charges->dollars &&
             amount_paid.cents > charges->cents) {
    DiffPayments (charges, &amount_paid, overpayment);
    return OVERPAID;
  }

  return NOCHARGE;
}

static Boolean ColorPaymentTable (
  DoC d,
  Int2 item,
  Int2 row,
  Int2 col
)

{
  IntPaymentPtr   ipp;
  SscPaymentsPtr  spp;
  Int2            status;

  spp = (SscPaymentsPtr) GetObjectExtra (d);
  if (spp == NULL || item < 1 || item > 127) return FALSE;
  ipp = GetPaymentInfo (spp, item);
  if (ipp == NULL) return FALSE;

  if (col != 7) return FALSE;

  status = CalculatePaymentStatus (&(ipp->charges), &(ipp->check_payment),
                                   &(ipp->cash_payment), &(ipp->account_payment),
                                   &(ipp->voucher_payment), NULL);
  switch (status) {
    case NOCHARGE :
      break;
    case UNPAID :
      break;
    case PAID :
      break;
    case UNDERPAID :
      Red ();
      return TRUE;
    case OVERPAID :
      Blue ();
      return TRUE;
    default :
      break;
  }

  return FALSE;
}

static CharPtr PrintPaymentTable (
  DoC d,
  Int2 item,
  Pointer data
)

{
  DollarAmount     amount_paid;
  Char             buf [256], tmp [64];
  DollarAmountPtr  dap;
  IntPaymentPtr    ipp;
  SscPaymentsPtr   spp;

  spp = (SscPaymentsPtr) GetObjectExtra (d);
  if (spp == NULL || item < 1 || item > 127) return NULL;
  ipp = GetPaymentInfo (spp, item);
  if (ipp == NULL) return NULL;

  buf [0] = '\0';

  if (StringDoesHaveText (ipp->payer)) {
    StringCat (buf, ipp->payer);
  } else {
    StringCat (buf, " ");
  }

  StringCat (buf, "\t");
  dap = &(ipp->charges);
  if (PrintDollarAmount (dap, tmp)) {
    StringCat (buf, tmp);
  }

  StringCat (buf, "\t");
  dap = &(ipp->check_payment);
  if (PrintDollarAmount (dap, tmp)) {
    StringCat (buf, tmp);
  }

  StringCat (buf, "\t");
  dap = &(ipp->cash_payment);
  if (PrintDollarAmount (dap, tmp)) {
    StringCat (buf, tmp);
  }

  StringCat (buf, "\t");
  dap = &(ipp->account_payment);
  if (PrintDollarAmount (dap, tmp)) {
    StringCat (buf, tmp);
  }

  StringCat (buf, "\t");
  dap = &(ipp->voucher_payment);
  if (PrintDollarAmount (dap, tmp)) {
    StringCat (buf, tmp);
  }

  StringCat (buf, "\t");
  SumPayments (&(ipp->check_payment), &(ipp->cash_payment),
               &(ipp->account_payment), &(ipp->voucher_payment),
               &amount_paid);
  if ((amount_paid.dollars > 0 || amount_paid.cents > 0) &&
      PrintDollarAmount (&amount_paid, tmp)) {
    StringCat (buf, tmp);
  } else {
    StringCat (buf, " ");
  }

  StringCat (buf, "\n");
  return StringSave (buf);
}

static void UpdateAddToAccountButton (
  SscPaymentsPtr spp
)

{
  Char             amt [64], buf [32];
  DollarAmountPtr  dap;
  IntPaymentPtr    ipp;
  DollarAmount     overpaid;
  Int2             status;

  if (spp == NULL) return;
  ipp = GetPaymentInfo (spp, spp->currItem);
  if (ipp == NULL) {
    SafeHide (spp->add_adv_group);
    SafeHide (spp->vouch_ser_group);
    return;
  }

  if (ipp->type == FAST_MEMBER) {
    SafeDisable (spp->acct_button);
  } else {
    SafeEnable (spp->acct_button);
  }

  dap = &(ipp->voucher_payment);
  if (dap != NULL && (dap->dollars > 0 || dap->cents > 0)) {
    SafeHide (spp->add_adv_group);
    Update ();
    SetStatus (spp->use_voucher, TRUE);
    SafeShow (spp->vouch_ser_group);
  } else {
    SetStatus (spp->use_voucher, FALSE);
    SafeHide (spp->vouch_ser_group);
    Update ();
    status = CalculatePaymentStatus (&(ipp->charges), &(ipp->check_payment),
                                     &(ipp->cash_payment), &(ipp->account_payment),
                                     &(ipp->voucher_payment), &overpaid);
    if (status == OVERPAID) {
      sprintf (buf, "Add Excess $? to Account");
      if (PrintDollarAmount (&overpaid, amt)) {
        if (ipp->type == FAST_MEMBER) {
          sprintf (buf, "Excess $%s Paid", amt);
        } else {
          sprintf (buf, "Add Excess $%s to Account", amt);
        }
      }
      SetTitle (spp->add_adv_button, buf);
      if (ipp->type == FAST_MEMBER) {
        SafeDisable (spp->add_adv_button);
      } else {
        SafeEnable (spp->add_adv_button);
      }
      SafeShow (spp->add_adv_group);
    } else {
      SafeHide (spp->add_adv_group);
    }
  }
}

static void ChangePaymentTableSelect (
  DoC d,
  Int2 item,
  Int2 row,
  Int2 col,
  Boolean dblClck
)

{
  Char             buf [32], cts [8];
  Int4             cents, dollars;
  DollarAmountPtr  dap;
  IntPaymentPtr    ipp;
  Int2             itemOld1, itemOld2;
  SscPaymentsPtr   spp;

  spp = (SscPaymentsPtr) GetObjectExtra (d);
  if (spp == NULL) return;
  if (item == 0 || row == 0 || col == 0) return;

  GetDocHighlight (d, &itemOld1, &itemOld2);
  SetDocHighlight (d, item, item);
  UpdateDocument (d, itemOld1, itemOld2);
  UpdateDocument (d, item, item);
  spp->currItem = item;

  ResetClip ();

  ipp = GetPaymentInfo (spp, spp->currItem);
  if (ipp != NULL) {
    SetTitle (spp->payer, ipp->payer);

    dap = &(ipp->charges);
    if (dap != NULL && (dap->dollars > 0 || dap->cents > 0)) {
      sprintf (cts, "%2ld", (long) dap->cents);
      if (cts [0] == ' ') {
        cts [0] = '0';
      }
      sprintf (buf, "%ld.%s", (long) dap->dollars, cts);
      SetTitle (spp->charges, buf);
    } else {
      SetTitle (spp->charges, "");
    }
    SetDollarAmount (spp->check, ipp->check_payment.dollars, ipp->check_payment.cents);
    SetDollarAmount (spp->cash, ipp->cash_payment.dollars, ipp->cash_payment.cents);
    SetDollarAmount (spp->account, ipp->account_payment.dollars, ipp->account_payment.cents);

    UpdateAddToAccountButton (spp);

    SetTitle (spp->voucher_serial, ipp->voucher_serial);

    SetTitle (spp->comment, ipp->comment);
  }

  GetDollarAmount (spp->check, &dollars, &cents);
  if (dollars < 0) {
    GetDollarAmount (spp->cash, &dollars, &cents);
    if (dollars < 0) {
      SelectDollars (spp->check);
    } else {
      SelectCents (spp->cash);
    }
  } else {
    SelectCents (spp->check);
  }

  Update ();
}

static void RecalculatePaymentTotals (
  SscPaymentsPtr spp
)

{
  DollarAmount     amount_paid;
  Char             buf0 [12], buf1 [32];
  Int4             tot_cents, tot_dollars;
  DollarAmountPtr  dap;
  Int2             i;
  IntPaymentPtr    ipp;

  if (spp == NULL) return;

  tot_cents = 0;
  tot_dollars = 0;
  for (i = 1; i < 128; i++) {
    ipp = spp->payments [i];
    if (ipp == NULL) continue;
    dap = &(ipp->charges);
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
  SetTitle (spp->ttl_charges, buf1);

  dap = &(spp->tot_chg);
  if (dap != NULL) {
    dap->dollars = tot_dollars;
    dap->cents = tot_cents;
  }

  tot_cents = 0;
  tot_dollars = 0;
  for (i = 1; i < 128; i++) {
    ipp = spp->payments [i];
    if (ipp == NULL) continue;
    dap = &(ipp->check_payment);
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
  SetTitle (spp->ttl_check, buf1);

  dap = &(spp->tot_check);
  if (dap != NULL) {
    dap->dollars = tot_dollars;
    dap->cents = tot_cents;
  }

  tot_cents = 0;
  tot_dollars = 0;
  for (i = 1; i < 128; i++) {
    ipp = spp->payments [i];
    if (ipp == NULL) continue;
    dap = &(ipp->cash_payment);
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
  SetTitle (spp->ttl_cash, buf1);

  dap = &(spp->tot_cash);
  if (dap != NULL) {
    dap->dollars = tot_dollars;
    dap->cents = tot_cents;
  }

  tot_cents = 0;
  tot_dollars = 0;
  for (i = 1; i < 128; i++) {
    ipp = spp->payments [i];
    if (ipp == NULL) continue;
    dap = &(ipp->account_payment);
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
  SetTitle (spp->ttl_account, buf1);

  dap = &(spp->tot_account);
  if (dap != NULL) {
    dap->dollars = tot_dollars;
    dap->cents = tot_cents;
  }

  tot_cents = 0;
  tot_dollars = 0;
  for (i = 1; i < 128; i++) {
    ipp = spp->payments [i];
    if (ipp == NULL) continue;
    dap = &(ipp->voucher_payment);
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
  SetTitle (spp->ttl_voucher, buf1);

  dap = &(spp->tot_voucher);
  if (dap != NULL) {
    dap->dollars = tot_dollars;
    dap->cents = tot_cents;
  }

  tot_cents = 0;
  tot_dollars = 0;
  for (i = 1; i < 128; i++) {
    ipp = spp->payments [i];
    if (ipp == NULL) continue;
    SumPayments (&(ipp->check_payment), &(ipp->cash_payment),
                 &(ipp->account_payment), &(ipp->voucher_payment),
                 &amount_paid);
    if (amount_paid.dollars < 0 || amount_paid.cents < 0) continue;
    tot_dollars += amount_paid.dollars;
    tot_cents += amount_paid.cents;
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
  SetTitle (spp->ttl_payments, buf1);

  dap = &(spp->tot_pay);
  if (dap != NULL) {
    dap->dollars = tot_dollars;
    dap->cents = tot_cents;
  }

  SetDirtyFlag (spp->tables);
}

static void AddExcess (
  ButtoN b
)

{
  IntPaymentPtr   ipp;
  DollarAmount    overpaid;
  SscPaymentsPtr  spp;
  Int2            status;
  SscTablesPtr    stp;

  spp = (SscPaymentsPtr) GetObjectExtra (b);
  if (spp == NULL) return;
  stp = spp->tables;
  if (stp == NULL) return;
  ipp = GetPaymentInfo (spp, spp->currItem);
  if (ipp == NULL) return;

  status = CalculatePaymentStatus (&(ipp->charges), &(ipp->check_payment),
                                   &(ipp->cash_payment), &(ipp->account_payment),
                                   &(ipp->voucher_payment), &overpaid);
  if (status != OVERPAID) return;

  if (ipp->type == FAST_MEMBER) {
    if (Message (MSG_YN, "Only regular members can have an account. Do you wish to override?") == ANS_NO) return;
  }

  SafeHide (spp->dialog);
  Update ();

  AddAdjustItem (stp->adjustments, ipp->payer, PAY_IN_ADVANCE, overpaid.dollars, overpaid.cents);

  RecalculateCharges (stp->glider_flights, stp->fees, stp->charges, stp->adjustments, stp->payments);

  RecalculatePaymentTotals (spp);

  UpdateAddToAccountButton (spp);

  UpdateDocument (spp->paymentlog, spp->currItem, spp->currItem);
  SafeShow (spp->dialog);
  Update ();
}

static void ChangeCheckPayment (
  DialoG d
)

{
  BaseDialogPtr    bdp;
  Int4             cents, dollars;
  DollarAmountPtr  dap;
  IntPaymentPtr    ipp;
  SscPaymentsPtr   spp;

  bdp = (BaseDialogPtr) GetObjectExtra (d);
  if (bdp == NULL) return;
  spp = (SscPaymentsPtr) bdp->userdata;
  if (spp == NULL) return;
  ipp = GetPaymentInfo (spp, spp->currItem);
  if (ipp == NULL) return;

  GetDollarAmount (d, &dollars, &cents);

  dap = &(ipp->check_payment);
  if (dap != NULL) {
    dap->dollars = dollars;
    dap->cents = cents;
  }

  SetDirtyFlag (spp->tables);

  RecalculatePaymentTotals (spp);

  UpdateAddToAccountButton (spp);

  UpdateDocument (spp->paymentlog, spp->currItem, spp->currItem);
  Update ();
}

static void ChangeCashPayment (
  DialoG d
)

{
  BaseDialogPtr    bdp;
  Int4             cents, dollars;
  DollarAmountPtr  dap;
  IntPaymentPtr    ipp;
  SscPaymentsPtr   spp;

  bdp = (BaseDialogPtr) GetObjectExtra (d);
  if (bdp == NULL) return;
  spp = (SscPaymentsPtr) bdp->userdata;
  if (spp == NULL) return;
  ipp = GetPaymentInfo (spp, spp->currItem);
  if (ipp == NULL) return;

  GetDollarAmount (d, &dollars, &cents);

  dap = &(ipp->cash_payment);
  if (dap != NULL) {
    dap->dollars = dollars;
    dap->cents = cents;
  }

  SetDirtyFlag (spp->tables);

  RecalculatePaymentTotals (spp);

  UpdateAddToAccountButton (spp);

  UpdateDocument (spp->paymentlog, spp->currItem, spp->currItem);
  Update ();
}

static void ChangeAccountPayment (
  DialoG d
)

{
  BaseDialogPtr    bdp;
  Int4             cents, dollars;
  DollarAmountPtr  dap;
  IntPaymentPtr    ipp;
  SscPaymentsPtr   spp;

  bdp = (BaseDialogPtr) GetObjectExtra (d);
  if (bdp == NULL) return;
  spp = (SscPaymentsPtr) bdp->userdata;
  if (spp == NULL) return;
  ipp = GetPaymentInfo (spp, spp->currItem);
  if (ipp == NULL) return;

  GetDollarAmount (d, &dollars, &cents);

  dap = &(ipp->account_payment);
  if (dap != NULL) {
    dap->dollars = dollars;
    dap->cents = cents;
  }

  SetDirtyFlag (spp->tables);

  RecalculatePaymentTotals (spp);

  UpdateAddToAccountButton (spp);

  UpdateDocument (spp->paymentlog, spp->currItem, spp->currItem);
  Update ();
}

static void ChangeVoucher (
  ButtoN b
)

{
  Int4             cents, dollars;
  DollarAmountPtr  dap;
  IntPaymentPtr    ipp;
  MiscRatePtr      mrp;
  SscPaymentsPtr   spp;
  SscTablesPtr     stp;

  spp = (SscPaymentsPtr) GetObjectExtra (b);
  if (spp == NULL) return;
  ipp = GetPaymentInfo (spp, spp->currItem);
  if (ipp == NULL) return;
  stp = spp->tables;
  if (stp == NULL) return;
  mrp = stp->miscrate_table;
  if (mrp == NULL) return;

  if (GetStatus (b)) {
    dollars = -1;
    cents = -1;
    dap = mrp->fast_fee;
    if (dap != NULL) {
      dollars = dap->dollars;
      cents = dap->cents;
    }
    dap = &(ipp->voucher_payment);
    if (dap != NULL) {
      dap->dollars = dollars;
      dap->cents = cents;
    }
  } else {
    dollars = -1;
    cents = -1;
    dap = &(ipp->voucher_payment);
    if (dap != NULL) {
      dap->dollars = dollars;
      dap->cents = cents;
    }
  }

  RecalculatePaymentTotals (spp);

  UpdateAddToAccountButton (spp);

  UpdateDocument (spp->paymentlog, spp->currItem, spp->currItem);
  Update ();
}

static void ChangeSerial (
  TexT t
)

{
  IntPaymentPtr   ipp;
  SscPaymentsPtr  spp;

  spp = (SscPaymentsPtr) GetObjectExtra (t);
  if (spp == NULL) return;
  ipp = GetPaymentInfo (spp, spp->currItem);
  if (ipp == NULL) return;

  ipp->voucher_serial = MemFree (ipp->voucher_serial);
  ipp->voucher_serial = SaveStringFromText (t);

  Update ();

  SetDirtyFlag (spp->tables);
}

static void ChangeComment (
  TexT t
)

{
  IntPaymentPtr   ipp;
  SscPaymentsPtr  spp;

  spp = (SscPaymentsPtr) GetObjectExtra (t);
  if (spp == NULL) return;
  ipp = GetPaymentInfo (spp, spp->currItem);
  if (ipp == NULL) return;

  ipp->comment = MemFree (ipp->comment);
  ipp->comment = SaveStringFromText (t);

  Update ();

  SetDirtyFlag (spp->tables);
}

extern void KeyToPaymentsDialog (DialoG d, Char key)

{
  Int2            numItems, offset;
  BaR             sb;
  SscPaymentsPtr  spp;

  spp = (SscPaymentsPtr) GetObjectExtra (d);
  if (spp == NULL) return;

  if (key == NLM_UP) {
    if (spp->currItem > 1) {
      (spp->currItem)--;
    }
  } else if (key == NLM_DOWN) {
    GetDocParams (spp->paymentlog, &numItems, NULL);
    if (spp->currItem < numItems) {
      (spp->currItem)++;
    }
  }

  sb = GetSlateVScrollBar ((SlatE) spp->paymentlog);
  offset = GetValue (sb);

  if (key == NLM_DOWN && offset < spp->currItem - NUM_PAYMENT_LINES && spp->currItem > 0) {
    SetValue (sb, spp->currItem - NUM_PAYMENT_LINES);
  }
  if (key == NLM_UP && spp->currItem <= offset) {
    SetValue (sb, spp->currItem - 1);
  }

  ChangePaymentTableSelect (spp->paymentlog, spp->currItem, 1, 1, FALSE);

  Update ();
}

extern DoC GetPaymentLogDoc (DialoG d)

{
  SscPaymentsPtr  spp;

  spp = (SscPaymentsPtr) GetObjectExtra (d);
  if (spp == NULL) return NULL;

  return spp->paymentlog;
}

extern Int2 GetPaymentCurrItem (DialoG d)

{
  SscPaymentsPtr  spp;

  spp = (SscPaymentsPtr) GetObjectExtra (d);
  if (spp == NULL) return 0;

  return spp->currItem;
}

extern void SetPaymentCurrItem (DialoG d, Int2 item)

{
  SscPaymentsPtr  spp;

  spp = (SscPaymentsPtr) GetObjectExtra (d);
  if (spp == NULL) return;

  spp->currItem = item;

  ChangePaymentTableSelect (spp->paymentlog, spp->currItem, 1, 1, FALSE);

  Update ();
}

static Int2 expPayPosns [] = {0, 22, 31, 41, 51, 61, 71, 81};
static Char expPayJusts [] = "llrrrrrr";

extern void ExportPaymentsDialog (
  DialoG d,
  ValNodePtr PNTR head
)

{
  DollarAmount    amount_paid;
  Char            buf [128], str [32], tmp [32];
  DoC             doc;
  IntPaymentPtr   ipp;
  Int2            k, numItems;
  SscPaymentsPtr  spp;

  spp = (SscPaymentsPtr) GetObjectExtra (d);
  if (spp == NULL || head == NULL) return;

  doc = spp->paymentlog;
  GetDocParams (doc, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    ipp = spp->payments [k];
    if (ipp == NULL) continue;
    SumPayments (&(ipp->check_payment), &(ipp->cash_payment),
                 &(ipp->account_payment), &(ipp->voucher_payment),
                 &amount_paid);
    if (ipp->charges.dollars < 0 || ipp->charges.cents < 0) {
      if (amount_paid.dollars <= 0 && amount_paid.cents <= 0) continue;
    }
    if (StringHasNoText (ipp->payer)) continue;
    buf [0] = '\0';
    ExportDocCell (doc, k, 1, buf);
    if (ipp->type == 2) {
      FormatCell ("*", buf, 1, expPayPosns, expPayJusts);
    }
    StringCpy (tmp, "$");
    ExportDocCell (doc, k, 2, tmp + 1);
    FormatCell (tmp, buf, 2, expPayPosns, expPayJusts);
    if (ipp->check_payment.dollars > 0 || ipp->check_payment.cents > 0) {
      StringCpy (tmp, "$");
      ExportDocCell (doc, k, 3, tmp + 1);
      FormatCell (tmp, buf, 3, expPayPosns, expPayJusts);
    }
    if (ipp->cash_payment.dollars > 0 || ipp->cash_payment.cents > 0) {
      StringCpy (tmp, "$");
      ExportDocCell (doc, k, 4, tmp + 1);
      FormatCell (tmp, buf, 4, expPayPosns, expPayJusts);
    }
    if (ipp->account_payment.dollars > 0 || ipp->account_payment.cents > 0) {
      StringCpy (tmp, "$");
      ExportDocCell (doc, k, 5, tmp + 1);
      FormatCell (tmp, buf, 5, expPayPosns, expPayJusts);
    }
    if (ipp->voucher_payment.dollars > 0 || ipp->voucher_payment.cents > 0) {
      StringCpy (tmp, "$");
      ExportDocCell (doc, k, 6, tmp + 1);
      FormatCell (tmp, buf, 6, expPayPosns, expPayJusts);
    }
    if (amount_paid.dollars > 0 || amount_paid.cents > 0) {
      StringCpy (tmp, "$");
      ExportDocCell (doc, k, 7, tmp + 1);
      FormatCell (tmp, buf, 7, expPayPosns, expPayJusts);
    }
    ValNodeCopyStr (head, k, buf);
  }

  ValNodeCopyStr (head, k + 1, "---------------------------------------------------------------------------------");

  buf [0] = '\0';
  GetTitle (spp->ttl_charges, str, sizeof (str));
  if (StringDoesHaveText (str)) {
    StringCpy (tmp, "$");
    StringCat (tmp, str);
    FormatCell (tmp, buf, 2, expPayPosns, expPayJusts);
  }
  GetTitle (spp->ttl_check, str, sizeof (str));
  if (StringDoesHaveText (str)) {
    StringCpy (tmp, "$");
    StringCat (tmp, str);
    FormatCell (tmp, buf, 3, expPayPosns, expPayJusts);
  }
  GetTitle (spp->ttl_cash, str, sizeof (str));
  if (StringDoesHaveText (str)) {
    StringCpy (tmp, "$");
    StringCat (tmp, str);
    FormatCell (tmp, buf, 4, expPayPosns, expPayJusts);
  }
  GetTitle (spp->ttl_account, str, sizeof (str));
  if (StringDoesHaveText (str)) {
    StringCpy (tmp, "$");
    StringCat (tmp, str);
    FormatCell (tmp, buf, 5, expPayPosns, expPayJusts);
  }
  GetTitle (spp->ttl_voucher, str, sizeof (str));
  if (StringDoesHaveText (str)) {
    StringCpy (tmp, "$");
    StringCat (tmp, str);
    FormatCell (tmp, buf, 6, expPayPosns, expPayJusts);
  }
  GetTitle (spp->ttl_payments, str, sizeof (str));
  if (StringDoesHaveText (str)) {
    StringCpy (tmp, "$");
    StringCat (tmp, str);
    FormatCell (tmp, buf, 7, expPayPosns, expPayJusts);
  }
  ValNodeCopyStr (head, k + 2, buf);
}

static CharPtr archivePaymentLabel = "Name                       Charge     Check      Cash   Account   Voucher     Total";

static Int2 acvPayPosns [] = {0, 33, 43, 53, 63, 73, 83};
static Char acvPayJusts [] = "lrrrrrr";

extern void ArchivePaymentsDialog (
  DialoG d,
  FILE *fp
)

{
  DollarAmount    amount_paid;
  Char            buf [128], tmp [32];
  IntPaymentPtr   ipp;
  Int2            k, numItems;
  SscPaymentsPtr  spp;

  spp = (SscPaymentsPtr) GetObjectExtra (d);
  if (spp == NULL || fp == NULL) return;

  fprintf (fp, "PAYMENTS -----\n\n%s\n\n", archivePaymentLabel);

  GetDocParams (spp->paymentlog, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    ipp = spp->payments [k];
    if (ipp == NULL) continue;
    if (ipp->charges.dollars < 0 || ipp->charges.cents < 0) {
      SumPayments (&(ipp->check_payment), &(ipp->cash_payment),
                   &(ipp->account_payment), &(ipp->voucher_payment),
                   &amount_paid);
      if (amount_paid.dollars <= 0 && amount_paid.cents <= 0) continue;
    }
    if (StringHasNoText (ipp->payer)) continue;
    SaveDocumentItem (spp->paymentlog, fp, k);
  }
  fprintf (fp, "-----------------------------------------------------------------------------------\n");
  buf [0] = '\0';
  /* StringCpy (buf, "Totals:"); */
  if (PrintDollarAmount (&(spp->tot_chg), tmp)) {
    FormatCell (tmp, buf, 1, acvPayPosns, acvPayJusts);
  }
  if (PrintDollarAmount (&(spp->tot_check), tmp)) {
    FormatCell (tmp, buf, 2, acvPayPosns, acvPayJusts);
  }
  if (PrintDollarAmount (&(spp->tot_cash), tmp)) {
    FormatCell (tmp, buf, 3, acvPayPosns, acvPayJusts);
  }
  if (PrintDollarAmount (&(spp->tot_account), tmp)) {
    FormatCell (tmp, buf, 4, acvPayPosns, acvPayJusts);
  }
  if (PrintDollarAmount (&(spp->tot_voucher), tmp)) {
    FormatCell (tmp, buf, 5, acvPayPosns, acvPayJusts);
  }
  if (PrintDollarAmount (&(spp->tot_pay), tmp)) {
    FormatCell (tmp, buf, 6, acvPayPosns, acvPayJusts);
  }
  fprintf (fp, "%s\n\n", buf);
}

static void PaymentInfoPtrToPaymentPage (DialoG d, Pointer data)

{
  Char             buf1 [32], buf2 [16];
  DollarAmountPtr  dap;
  Int2             i;
  IntPaymentPtr    ipp;
  Int2             k;
  PaymentInfoPtr   pip, payments;
  SscPaymentsPtr   spp;
  DollarAmount     tot_chg, tot_check, tot_cash, tot_account, tot_voucher, tot_pay;

  spp = (SscPaymentsPtr) GetObjectExtra (d);
  if (spp == NULL) return;
  payments = (PaymentInfoPtr) data;

  Reset (spp->paymentlog);
  spp->numPayments = 0;
  spp->currItem = 1;

  for (k = 0; k < 128; k++) {
    ipp = spp->payments [k];
    IntPaymentInfoFree (ipp);
    spp->payments [k] = NULL;
  }

  if (payments == NULL) {
    Reset (spp->paymentlog);
    SetTitle (spp->payer, "");
    SetTitle (spp->charges, "");
    SetDollarAmount (spp->check, -1, -1);
    SetDollarAmount (spp->cash, -1, -1);
    SetDollarAmount (spp->account, -1, -1);
    /*
    SetTitle (spp->check_dollars, "");
    SetTitle (spp->check_cents, "");
    SetTitle (spp->cash_dollars, "");
    SetTitle (spp->cash_cents, "");
    */
    SetTitle (spp->ttl_charges, "");
    SetTitle (spp->ttl_check, "");
    SetTitle (spp->ttl_cash, "");
    SetTitle (spp->ttl_account, "");
    SetTitle (spp->ttl_voucher, "");
    SetTitle (spp->ttl_payments, "");
    SetStatus (spp->use_voucher, FALSE);
    SafeHide (spp->vouch_ser_group);
    SetTitle (spp->voucher_serial, "");
    SafeHide (spp->add_adv_group);
    SetTitle (spp->add_adv_button, "Add Excess $00000.00 to Account");
    SetTitle (spp->comment, "");
    spp->tot_chg.dollars = 0;
    spp->tot_chg.cents = 0;
    spp->tot_check.dollars = 0;
    spp->tot_check.cents = 0;
    spp->tot_cash.dollars = 0;
    spp->tot_cash.cents = 0;
    spp->tot_account.dollars = 0;
    spp->tot_account.cents = 0;
    spp->tot_voucher.dollars = 0;
    spp->tot_voucher.cents = 0;
    spp->tot_pay.dollars = 0;
    spp->tot_pay.cents = 0;
    spp->numPayments = 0;
    spp->currItem = 1;
    for (i = 0; i < NUM_PAYMENT_LINES; i++) {
      AppendItem (spp->paymentlog, PrintPaymentTable, spp, FALSE, 1,
                  &paymentParFmt, paymentColFmt, systemFont);
    }
    SetDocHighlight (spp->paymentlog, 1, 1);
    return;
  }

  tot_chg.dollars = 0;
  tot_chg.cents = 0;
  tot_check.dollars = 0;
  tot_check.cents = 0;
  tot_cash.dollars = 0;
  tot_cash.cents = 0;
  tot_account.dollars = 0;
  tot_account.cents = 0;
  tot_voucher.dollars = 0;
  tot_voucher.cents = 0;
  tot_pay.dollars = 0;
  tot_pay.cents = 0;

  for (pip = payments, k = 1; pip != NULL; pip = pip->next, k++) {
    ipp = GetPaymentInfo (spp, k);
    if (ipp == NULL) continue;

    ipp->payer = StringSaveForAlist (pip->payer);
    ipp->type = pip->type;

    dap = pip->charges;
    if (dap != NULL) {
      ipp->charges.dollars = dap->dollars;
      ipp->charges.cents = dap->cents;

      tot_chg.dollars += dap->dollars;
      tot_chg.cents += dap->cents;
      if (tot_chg.cents >= 100) {
        tot_chg.dollars += tot_chg.cents / 100;
        tot_chg.cents = tot_chg.cents % 100;
      }
    }
    dap = pip->check_payment;
    if (dap != NULL) {
      ipp->check_payment.dollars = dap->dollars;
      ipp->check_payment.cents = dap->cents;

      tot_check.dollars += dap->dollars;
      tot_check.cents += dap->cents;
      if (tot_check.cents >= 100) {
        tot_check.dollars += tot_check.cents / 100;
        tot_check.cents = tot_check.cents % 100;
      }

      tot_pay.dollars += dap->dollars;
      tot_pay.cents += dap->cents;
      if (tot_pay.cents >= 100) {
        tot_pay.dollars += tot_pay.cents / 100;
        tot_pay.cents = tot_pay.cents % 100;
      }
    }
    dap = pip->cash_payment;
    if (dap != NULL) {
      ipp->cash_payment.dollars = dap->dollars;
      ipp->cash_payment.cents = dap->cents;

      tot_cash.dollars += dap->dollars;
      tot_cash.cents += dap->cents;
      if (tot_cash.cents >= 100) {
        tot_cash.dollars += tot_cash.cents / 100;
        tot_cash.cents = tot_cash.cents % 100;
      }

      tot_pay.dollars += dap->dollars;
      tot_pay.cents += dap->cents;
      if (tot_pay.cents >= 100) {
        tot_pay.dollars += tot_pay.cents / 100;
        tot_pay.cents = tot_pay.cents % 100;
      }
    }
    dap = pip->account_payment;
    if (dap != NULL) {
      ipp->account_payment.dollars = dap->dollars;
      ipp->account_payment.cents = dap->cents;

      tot_account.dollars += dap->dollars;
      tot_account.cents += dap->cents;
      if (tot_account.cents >= 100) {
        tot_account.dollars += tot_account.cents / 100;
        tot_account.cents = tot_account.cents % 100;
      }

      tot_pay.dollars += dap->dollars;
      tot_pay.cents += dap->cents;
      if (tot_pay.cents >= 100) {
        tot_pay.dollars += tot_pay.cents / 100;
        tot_pay.cents = tot_pay.cents % 100;
      }
    }
    dap = pip->voucher_payment;
    if (dap != NULL) {
      ipp->voucher_payment.dollars = dap->dollars;
      ipp->voucher_payment.cents = dap->cents;

      tot_voucher.dollars += dap->dollars;
      tot_voucher.cents += dap->cents;
      if (tot_voucher.cents >= 100) {
        tot_voucher.dollars += tot_voucher.cents / 100;
        tot_voucher.cents = tot_voucher.cents % 100;
      }

      tot_pay.dollars += dap->dollars;
      tot_pay.cents += dap->cents;
      if (tot_pay.cents >= 100) {
        tot_pay.dollars += tot_pay.cents / 100;
        tot_pay.cents = tot_pay.cents % 100;
      }
    }

    ipp->voucher_serial = StringSaveNotEmpty (pip->voucher_serial);
    ipp->comment = StringSaveNotEmpty (pip->comment);

    AppendItem (spp->paymentlog, PrintPaymentTable, spp, FALSE, 1,
                &paymentParFmt, paymentColFmt, systemFont);

    (spp->numPayments)++;
  }

  /*
  AppendItem (spp->paymentlog, PrintPaymentTable, spp, FALSE, 1,
              &paymentParFmt, paymentColFmt, systemFont);
  k++;
  */

  while (k <= NUM_PAYMENT_LINES) {
    AppendItem (spp->paymentlog, PrintPaymentTable, spp, FALSE, 1,
                &paymentParFmt, paymentColFmt, systemFont);
    k++;
  }

  UpdateDocument (spp->paymentlog, 0, 0);

  sprintf (buf2, "%2ld", (long) tot_chg.cents);
  if (buf2 [0] == ' ') {
    buf2 [0] = '0';
  }
  sprintf (buf1, "%ld.%s", (long) tot_chg.dollars, buf2);
  SetTitle (spp->ttl_charges, buf1);
  spp->tot_chg.dollars = tot_chg.dollars;
  spp->tot_chg.cents = tot_chg.cents;

  sprintf (buf2, "%2ld", (long) tot_check.cents);
  if (buf2 [0] == ' ') {
    buf2 [0] = '0';
  }
  sprintf (buf1, "%ld.%s", (long) tot_check.dollars, buf2);
  SetTitle (spp->ttl_check, buf1);
  spp->tot_check.dollars = tot_check.dollars;
  spp->tot_check.cents = tot_check.cents;

  sprintf (buf2, "%2ld", (long) tot_cash.cents);
  if (buf2 [0] == ' ') {
    buf2 [0] = '0';
  }
  sprintf (buf1, "%ld.%s", (long) tot_cash.dollars, buf2);
  SetTitle (spp->ttl_cash, buf1);
  spp->tot_cash.dollars = tot_cash.dollars;
  spp->tot_cash.cents = tot_cash.cents;

  sprintf (buf2, "%2ld", (long) tot_account.cents);
  if (buf2 [0] == ' ') {
    buf2 [0] = '0';
  }
  sprintf (buf1, "%ld.%s", (long) tot_account.dollars, buf2);
  SetTitle (spp->ttl_account, buf1);
  spp->tot_account.dollars = tot_account.dollars;
  spp->tot_account.cents = tot_account.cents;

  sprintf (buf2, "%2ld", (long) tot_voucher.cents);
  if (buf2 [0] == ' ') {
    buf2 [0] = '0';
  }
  sprintf (buf1, "%ld.%s", (long) tot_voucher.dollars, buf2);
  SetTitle (spp->ttl_voucher, buf1);
  spp->tot_voucher.dollars = tot_voucher.dollars;
  spp->tot_voucher.cents = tot_voucher.cents;

  sprintf (buf2, "%2ld", (long) tot_pay.cents);
  if (buf2 [0] == ' ') {
    buf2 [0] = '0';
  }
  sprintf (buf1, "%ld.%s", (long) tot_pay.dollars, buf2);
  SetTitle (spp->ttl_payments, buf1);
  spp->tot_pay.dollars = tot_pay.dollars;
  spp->tot_pay.cents = tot_pay.cents;

  ChangePaymentTableSelect (spp->paymentlog, 1, 1, 1, FALSE);

  Update ();
}

static Pointer PaymentPageToPaymentInfoPtr (DialoG d)

{
  DollarAmount     amount_paid;
  DollarAmountPtr  dap;
  IntPaymentPtr    ipp;
  Int2             k, numItems;
  PaymentInfoPtr   pip, payments, lastpip;
  SscPaymentsPtr   spp;

  spp = (SscPaymentsPtr) GetObjectExtra (d);
  if (spp == NULL) return NULL;

  payments = NULL;

  lastpip = NULL;
  GetDocParams (spp->paymentlog, &numItems, NULL);
  for (k = 1; k <= numItems; k++) {
    ipp = spp->payments [k];
    if (ipp == NULL) continue;
    if (ipp->charges.dollars < 0 || ipp->charges.cents < 0) {
      SumPayments (&(ipp->check_payment), &(ipp->cash_payment),
                   &(ipp->account_payment), &(ipp->voucher_payment),
                   &amount_paid);
      if (amount_paid.dollars <= 0 && amount_paid.cents <= 0) continue;
    }
    if (StringHasNoText (ipp->payer)) continue;
    pip = PaymentInfoNew ();
    if (pip == NULL) continue;

    pip->payer = StringSaveNotEmpty (ipp->payer);
    pip->type = ipp->type;

    dap = DollarAmountNew ();
    if (dap != NULL) {
      if (ipp->charges.dollars >= 0 && ipp->charges.cents >= 0) {
        dap->dollars = ipp->charges.dollars;
        dap->cents = ipp->charges.cents;
      } else {
        dap->dollars = 0;
        dap->cents = 0;
      }
      pip->charges = dap;
    }
    if (ipp->check_payment.dollars > 0 || ipp->check_payment.cents > 0) {
      dap = DollarAmountNew ();
      if (dap != NULL) {
        dap->dollars = ipp->check_payment.dollars;
        dap->cents = ipp->check_payment.cents;
        pip->check_payment = dap;
      }
    }
    if (ipp->cash_payment.dollars > 0 || ipp->cash_payment.cents > 0) {
      dap = DollarAmountNew ();
      if (dap != NULL) {
        dap->dollars = ipp->cash_payment.dollars;
        dap->cents = ipp->cash_payment.cents;
        pip->cash_payment = dap;
      }
    }
    if (ipp->account_payment.dollars > 0 || ipp->account_payment.cents > 0) {
      dap = DollarAmountNew ();
      if (dap != NULL) {
        dap->dollars = ipp->account_payment.dollars;
        dap->cents = ipp->account_payment.cents;
        pip->account_payment = dap;
      }
    }
    if (ipp->voucher_payment.dollars > 0 || ipp->voucher_payment.cents > 0) {
      dap = DollarAmountNew ();
      if (dap != NULL) {
        dap->dollars = ipp->voucher_payment.dollars;
        dap->cents = ipp->voucher_payment.cents;
        pip->voucher_payment = dap;
      }

      pip->voucher_serial = StringSaveNotEmpty (ipp->voucher_serial);
    }

    pip->comment = StringSaveNotEmpty (ipp->comment);

    if (payments == NULL) {
      payments = pip;
    }
    if (lastpip != NULL) {
      lastpip->next = pip;
    }
    lastpip = pip;
  }

  return (Pointer) payments;
}

static Boolean PaymentAlreadySet (
  DollarAmountPtr pmt1,
  DollarAmountPtr pmt2,
  DollarAmountPtr pmt3
)

{
  if (pmt1 != NULL) {
    if (pmt1->dollars >= 0 && pmt1->cents >= 0) return TRUE;
  }
  if (pmt2 != NULL) {
    if (pmt2->dollars >= 0 && pmt2->cents >= 0) return TRUE;
  }
  if (pmt3 != NULL) {
    if (pmt3->dollars >= 0 && pmt3->cents >= 0) return TRUE;
  }

  return FALSE;
}

static void PaidByProc (
  SscTablesPtr stp,
  Int2 which
)

{
  Boolean          already_set = FALSE;
  DollarAmountPtr  chg;
  Int4             chg_cents, voucher_cents;
  DialoG           d;
  DollarAmountPtr  dap;
  IntPaymentPtr    ipp;
  SscPaymentsPtr   spp;

  if (stp == NULL) return;

  d = stp->payments;
  if (! Visible (d)) return;
  if (! AllParentsVisible (d)) return;

  spp = (SscPaymentsPtr) GetObjectExtra (d);
  if (spp == NULL) return;
  ipp = GetPaymentInfo (spp, spp->currItem);
  if (ipp == NULL) return;

  if (ipp->type == FAST_MEMBER && which == 3) {
    if (Message (MSG_YN, "Only regular members can pay by account. Do you wish to override?") == ANS_NO) return;
  }

  chg = &(ipp->charges);
  if (chg != NULL && chg->dollars >= 0 && chg->cents >= 0) {
    switch (which) {
      case 1 :
        already_set = PaymentAlreadySet(&(ipp->cash_payment), &(ipp->account_payment), &(ipp->voucher_payment));
        break;
      case 2 :
        already_set = PaymentAlreadySet(&(ipp->check_payment), &(ipp->account_payment), &(ipp->voucher_payment));
        break;
      case 3 :
        already_set = PaymentAlreadySet(&(ipp->cash_payment), &(ipp->check_payment), &(ipp->voucher_payment));
        break;
      default :
        break;
    }
    if (already_set) {
      if (Message (MSG_YN, "Do you want to override the existing payment?") == ANS_NO) return;
    }
    dap = &(ipp->check_payment);
    if (dap != NULL) {
      dap->dollars = -1;
      dap->cents = -1;
    }
    dap = &(ipp->cash_payment);
    if (dap != NULL) {
      dap->dollars = -1;
      dap->cents = -1;
    }
    dap = &(ipp->account_payment);
    if (dap != NULL) {
      dap->dollars = -1;
      dap->cents = -1;
    }
    dap = &(ipp->voucher_payment);
    if (dap != NULL && dap->dollars >= 0 && dap->cents >= 0) {
      if (dap->dollars >= 0 && dap->cents >= 0) {
        voucher_cents = dap->dollars * 100 + dap->cents;
        chg_cents = chg->dollars * 100 + chg->cents;
        if (chg_cents > voucher_cents) {
          chg_cents -= voucher_cents;
          chg->dollars = chg_cents / 100;
          chg->cents = chg_cents % 100;
        }
      }
    }
    dap = NULL;
    switch (which) {
      case 1 :
        dap = &(ipp->check_payment);
        break;
      case 2 :
        dap = &(ipp->cash_payment);
        break;
      case 3 :
        dap = &(ipp->account_payment);
        break;
      default :
        break;
    }
    if (dap != NULL) {
      dap->dollars = chg->dollars;
      dap->cents = chg->cents;
    }
    SetDollarAmount (spp->check, ipp->check_payment.dollars, ipp->check_payment.cents);
    SetDollarAmount (spp->cash, ipp->cash_payment.dollars, ipp->cash_payment.cents);
    SetDollarAmount (spp->account, ipp->account_payment.dollars, ipp->account_payment.cents);

    RecalculatePaymentTotals (spp);

    UpdateAddToAccountButton (spp);

    UpdateDocument (spp->paymentlog, spp->currItem, spp->currItem);
    Update ();
  }
}

static void PaidByMenuProc (
  IteM i,
  Int2 which
)

{
  BaseFormPtr      bfp;
  SscTablesPtr     stp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  stp = GetTablesPointer (bfp);
  if (stp == NULL) return;

  PaidByProc (stp, which);
}

static void PaidByButtonProc (
  ButtoN b,
  Int2 which
)

{
  SscPaymentsPtr  spp;
  SscTablesPtr    stp;

  spp = (SscPaymentsPtr) GetObjectExtra (b);
  if (spp == NULL) return;
  stp = spp->tables;
  if (stp == NULL) return;

  PaidByProc (stp, which);
}

static void PaidByCheckProc (
  IteM i
)

{
  PaidByMenuProc (i, 1);
}

static void PaidByCashProc (
  IteM i
)

{
  PaidByMenuProc (i, 2);
}

static void PaidByAccountProc (
  IteM i
)

{
  PaidByMenuProc (i, 3);
}

static void PaidByCheckBtn (
  ButtoN b
)

{
  PaidByButtonProc (b, 1);
}

static void PaidByCashBtn (
  ButtoN b
)

{
  PaidByButtonProc (b, 2);
}

static void PaidByAccountBtn (
  ButtoN b
)

{
  PaidByButtonProc (b, 3);
}

extern void SetupPaymentMenus (
  MenU m,
  BaseFormPtr bfp
)

{
#ifdef WIN_MAC
  CommandItem (m, "Paid in Full by Check/Y", PaidByCheckProc);
  CommandItem (m, "Paid in Full by Cash/J", PaidByCashProc);
  CommandItem (m, "Paid in Full from Account/A", PaidByAccountProc);
#else
  IteM  i;

  i = CommandItem (m, "Paid in Full by Check/  Y", PaidByCheckProc);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (m, "Paid in Full by Cash/  J", PaidByCashProc);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (m, "Paid in Full from Account/  A", PaidByAccountProc);
  SetObjectExtra (i, bfp, NULL);
#endif
}

extern DialoG CreatePaymentsDialog (
  GrouP prnt,
  SscTablesPtr stp
)

{
  ButtoN          b;
  Char            buf [64];
  GrouP           cts, tbl, g0, g1, g2, g3, g4, g5, p, x;
  FonT            fnt;
  Int2            i, hgt, wid;
  MiscRatePtr     mrp;
  CharPtr         ptr;
  SscPaymentsPtr  spp;
  CharPtr         str;
  Char            tmp [16];

  if (stp == NULL) return NULL;
  spp = (SscPaymentsPtr) MemNew (sizeof (SscPayments));
  if (spp == NULL) return NULL;
  mrp = stp->miscrate_table;
  if (mrp == NULL) return NULL;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, spp, StdCleanupExtraProc);
  spp->dialog = (DialoG) p;
  spp->todialog = PaymentInfoPtrToPaymentPage;
  spp->fromdialog = PaymentPageToPaymentInfoPtr;

  spp->tables = stp;

  SelectFont (systemFont);
  hgt = LineHeight ();
  paymentColFmt [0].pixWidth = 25 * StringWidth ("X") + 5;
  paymentColFmt [1].pixWidth = StringWidth ("$1000.00 ") + 5;
  paymentColFmt [2].pixWidth = StringWidth ("$1000.00 ") + 5;
  paymentColFmt [3].pixWidth = StringWidth ("$1000.00 ") + 5;
  paymentColFmt [4].pixWidth = StringWidth ("$1000.00 ") + 5;
  paymentColFmt [5].pixWidth = StringWidth ("$1000.00 ") + 5;
  paymentColFmt [6].pixWidth = StringWidth ("$1000.00 ") + 5;
  SelectFont (systemFont);

  wid = 0;
  for (i = 0; i < 7; i++) {
    wid += paymentColFmt [i].pixWidth;
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
  StaticPrompt (g0, "Name", paymentColFmt [0].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Amount", paymentColFmt [1].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Check", paymentColFmt [2].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Cash", paymentColFmt [3].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Account", paymentColFmt [4].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Voucher", paymentColFmt [5].pixWidth, 0, fnt, 'c');
  StaticPrompt (g0, "Total", paymentColFmt [6].pixWidth, 0, fnt, 'c');

  spp->paymentlog = DocumentPanel (tbl, wid + 2, NUM_PAYMENT_LINES * hgt + 2);
  SetObjectExtra (spp->paymentlog, spp, NULL);
  SetDocCache (spp->paymentlog, NULL, NULL, NULL);
  SetDocNotify (spp->paymentlog, ChangePaymentTableSelect);
  spp->numPayments = 0;

  SetDocShade (spp->paymentlog, NULL, NULL, NULL, ColorPaymentTable);

  g5 = HiddenGroup (tbl, -10, 0, NULL);
  SetGroupSpacing (g5, 0, 3);
  StaticPrompt (g5, "", paymentColFmt [0].pixWidth, 0, fnt, 'c');
  spp->ttl_charges = StaticPrompt (g5, "", paymentColFmt [1].pixWidth, 0, systemFont, 'r');
  spp->ttl_check = StaticPrompt (g5, "", paymentColFmt [2].pixWidth, 0, systemFont, 'r');
  spp->ttl_cash = StaticPrompt (g5, "", paymentColFmt [3].pixWidth, 0, systemFont, 'r');
  spp->ttl_account = StaticPrompt (g5, "", paymentColFmt [4].pixWidth, 0, systemFont, 'r');
  spp->ttl_voucher = StaticPrompt (g5, "", paymentColFmt [5].pixWidth, 0, systemFont, 'r');
  spp->ttl_payments = StaticPrompt (g5, "", paymentColFmt [6].pixWidth, 0, systemFont, 'r');

  cts = HiddenGroup (p, -1, 0, NULL);
  SetGroupSpacing (cts, 10, 10);
  SetGroupMargins (cts, 5, 5);

  g1 = HiddenGroup (cts, -10, 0, NULL);
  SetGroupSpacing (g1, 10, 5);

  StaticPrompt (g1, "Name", 0, dialogTextHeight, programFont, 'l');
  str = WidestAlist (stp->members.alist);
  spp->payer = StaticPrompt (g1, str, 0, dialogTextHeight, systemFont, 'l');
  SetTitle (spp->payer, "");

  StaticPrompt (g1, "Amount Due", 0, dialogTextHeight, programFont, 'l');
  spp->charges = StaticPrompt (g1, "1000.00", 0, dialogTextHeight, systemFont, 'l');
  SetTitle (spp->charges, "");

  g2 = HiddenGroup (cts, -15, 0, NULL);
  SetGroupSpacing (g2, 15, 5);

  b = PushButton (g2, "Check", PaidByCheckBtn);
  SetObjectExtra (b, (Pointer) spp, NULL);
  StaticPrompt (g2, "$", 0, dialogTextHeight, programFont, 'l');
  spp->check = CreateDollarAmountDialog (g2, (Pointer) spp, ChangeCheckPayment);
  StaticPrompt (g2, "  ", 0, dialogTextHeight, programFont, 'l');

  b = PushButton (g2, "Cash", PaidByCashBtn);
  SetObjectExtra (b, (Pointer) spp, NULL);
  StaticPrompt (g2, "$", 0, dialogTextHeight, programFont, 'l');
  spp->cash = CreateDollarAmountDialog (g2, (Pointer) spp, ChangeCashPayment);
  StaticPrompt (g2, "  ", 0, dialogTextHeight, programFont, 'l');

  spp->acct_button = PushButton (g2, "Account", PaidByAccountBtn);
  SetObjectExtra (spp->acct_button, (Pointer) spp, NULL);
  StaticPrompt (g2, "$", 0, dialogTextHeight, programFont, 'l');
  spp->account = CreateDollarAmountDialog (g2, (Pointer) spp, ChangeAccountPayment);

  g3 = HiddenGroup (cts, -10, 0, NULL);
  SetGroupSpacing (g3, 10, 5);

  if (mrp != NULL && PrintDollarAmount (mrp->fast_fee, tmp)) {
    TrimSpacesAroundString (tmp);
    ptr = StringStr (tmp, ".00");
    if (ptr != NULL) {
      *ptr = '\0';
    }
    sprintf (buf, "$%s FAST Voucher Payment", tmp);
  } else {
    sprintf (buf, "FAST Voucher Payment");
  }
  spp->use_voucher = CheckBox (g3, buf, ChangeVoucher);
  SetObjectExtra (spp->use_voucher, spp, NULL);
  SetStatus (spp->use_voucher, FALSE);

  x = HiddenGroup (g3, 0, 0, NULL);
  spp->vouch_ser_group = HiddenGroup (x, -4, 0, NULL);
  StaticPrompt (spp->vouch_ser_group, "Serial Number", 0, dialogTextHeight, programFont, 'l');
  spp->voucher_serial = DialogText (spp->vouch_ser_group, "000000000000", 0, ChangeSerial);
  SetObjectExtra (spp->voucher_serial, spp, NULL);
  SetTitle (spp->voucher_serial, "");
  Hide (spp->vouch_ser_group);
  spp->add_adv_group = HiddenGroup (x, -4, 0, NULL);
  spp->add_adv_button = PushButton (spp->add_adv_group, "Add Excess $00000.00 to Account", AddExcess);
  SetObjectExtra (spp->add_adv_button, spp, NULL);
  SetTitle (spp->add_adv_button, "");
  Hide (spp->add_adv_group);

  g4 = HiddenGroup (cts, -10, 0, NULL);
  SetGroupSpacing (g4, 10, 5);

  StaticPrompt (g4, "Comment", 0, dialogTextHeight, programFont, 'l');
  spp->comment = DialogText (g4, "", 20, ChangeComment);
  SetObjectExtra (spp->comment, spp, NULL);

  AlignObjects (ALIGN_CENTER, (HANDLE) tbl, (HANDLE) cts, NULL);
  AlignObjects (ALIGN_RIGHT, (HANDLE) spp->voucher_serial, (HANDLE) spp->comment, NULL);

  spp->numPayments = 0;
  spp->currItem = 1;

  return (DialoG) p;
}

