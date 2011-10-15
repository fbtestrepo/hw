/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Expenses page

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#include "ssclog.h"

/* dialog controls */

typedef struct sscexp {
  DIALOG_MESSAGE_BLOCK

  TexT          expenses;

  SscTablesPtr  tables;
} SscExpense, PNTR SscExpensePtr;

static CharPtr SaveStringFromTextAndWrap (
  TexT t
)

{
  Char     ch;
  Int2     i;
  CharPtr  last, ptr, str;

  str = SaveStringFromText (t);
  if (StringHasNoText (str)) return str;
  TrimSpacesAroundString (str);

  last = NULL;
  i = 0;

  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    if (IS_WHITESP (ch)) {
      last = ptr;
    }
    i++;
    if (i > 76 && last != NULL) {
      *last = '\n';
      i = 0;
      last = NULL;
    }
    ptr++;
    ch = *ptr;
  }

  return str;
}

extern void ExportExpensesDialog (
  DialoG d,
  ValNodePtr PNTR head
)

{
  SscExpensePtr  sep;
  CharPtr        str;

  sep = (SscExpensePtr) GetObjectExtra (d);
  if (sep == NULL || head == NULL) return;

  str = SaveStringFromTextAndWrap (sep->expenses);
  if (StringDoesHaveText (str)) {
    ValNodeCopyStr (head, 0, str);
  }
  MemFree (str);
}

extern void ArchiveExpensesDialog (
  DialoG d,
  FILE *fp
)

{
  SscExpensePtr  sep;
  CharPtr        str;

  sep = (SscExpensePtr) GetObjectExtra (d);
  if (sep == NULL || fp == NULL) return;

  str = SaveStringFromTextAndWrap (sep->expenses);
  if (StringDoesHaveText (str)) {
    fprintf (fp, "EXPENSES -----\n\n");

    fprintf (fp,"%s\n\n", str);
  }
  MemFree (str);
}

static void CharPtrToExpensePage (
  DialoG d,
  Pointer data
)

{
  SscExpensePtr  sep;
  CharPtr        str;

  sep = (SscExpensePtr) GetObjectExtra (d);
  if (sep == NULL) return;
  str = (CharPtr) data;

  SetTitle (sep->expenses, str);
}

static Pointer ExpensePageToCharPtr (
  DialoG d
)

{
  SscExpensePtr  sep;
  CharPtr        str;

  sep = (SscExpensePtr) GetObjectExtra (d);
  if (sep == NULL) return NULL;

  str = SaveStringFromText (sep->expenses);

  return (Pointer) str;
}

extern DialoG CreateAddExpensesDialog (
  GrouP prnt,
  SscTablesPtr stp
)

{
  GrouP          g, p;
  SscExpensePtr  sep;

  sep = (SscExpensePtr) MemNew (sizeof (SscExpense));
  if (sep == NULL) return NULL;

  p = HiddenGroup (prnt, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  SetObjectExtra (p, sep, StdCleanupExtraProc);
  sep->dialog = (DialoG) p;
  sep->todialog = CharPtrToExpensePage;
  sep->fromdialog = ExpensePageToCharPtr;

  sep->tables = stp;

  g = HiddenGroup (p, 1, 0, NULL);
  StaticPrompt (g, "Reimbursable Expenses", 0, dialogTextHeight, programFont, 'c');
  sep->expenses = ScrollText (g, 20, 5, programFont, TRUE, NULL);
  StaticPrompt (g, "Do not subtract expenses from payment - treasurer will reimburse purchaser",
                0, dialogTextHeight, programFont, 'c');

  return (DialoG) p;
}

