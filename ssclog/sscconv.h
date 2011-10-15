/*
    SKYLINE SOARING CLUB DUTY LOGSHEET PROGRAM - Converter header file

    Copyright (c) 2004-2011 Jonathan A. Kans. All rights reserved.
*/

#ifndef _SSCCONV_
#define _SSCCONV_

#include <ncbi.h>
#include <asn.h>
#include <urlquery.h>
#define NLM_GENERATED_CODE_PROTO 1
#include "objssc.h"

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif


extern CharPtr data_spec_version;

extern void ModernizeLogSheet (
  LogSheetPtr lsp
);

extern void ExpandLogSheet (
  LogSheetPtr lsp
);

extern void ContractLogSheet (
  LogSheetPtr lsp
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

#endif /* ndef _SSCCONV_ */

