#ifdef __MWERKS__
#include <ncbilcl.met>
#else
#ifdef __MINGW32__
#include <ncbilcl.mgw>
#else
#ifdef __linux__
#include <ncbilcl.lnx>
#else
#include <ncbilcl.dwn>
#endif
#endif
#endif

