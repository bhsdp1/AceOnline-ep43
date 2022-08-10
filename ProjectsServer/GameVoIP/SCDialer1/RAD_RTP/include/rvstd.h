/*

NOTICE:
This document contains information that is proprietary to RADVision LTD..
No part of this publication may be reproduced in any form whatsoever without
written prior approval by RADVision LTD..

RADVision LTD. reserves the right to revise this publication and make changes
without obligation to notify any person of such revisions or changes.

*/

#define IN
#define OUT
#define INOUT
#define OUTIN
#define OPTIONAL

#ifdef RVERROR
#undef RVERROR
#endif
#define RVERROR -1

#ifdef SUCCESS
#undef SUCCESS
#endif
#define SUCCESS 1

#ifdef OK
#undef OK
#endif
#define OK 0

#ifdef RESOURCES_PROBLEM
#undef RESOURCES_PROBLEM
#endif
#define RESOURCES_PROBLEM -2


#ifdef OBJECT_WAS_NOT_FOUND
#undef OBJECT_WAS_NOT_FOUND
#endif
#define OBJECT_WAS_NOT_FOUND -3

#ifndef DECLARE_OPAQUE
#define DECLARE_OPAQUE(name)    typedef struct { int unused; } name##__ ; \
				typedef const name##__ * name; \
				typedef name*  LP##name

typedef char* rHandler;
#define nprn(s) ((s)?(s):"(null)")
#define nprnd(d) ((d)?(*d):RVERROR)
#endif



