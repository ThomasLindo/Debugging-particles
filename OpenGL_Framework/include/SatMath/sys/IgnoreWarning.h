#ifndef __SAT_IGNORE_WARNING__
#define __SAT_IGNORE_WARNING__

#define WARNING_NAMELESS_STRUCT_UNION		4201	 // Warning	C4201	nonstandard extension used : nameless struct / union

#define IGNORE_WARNING_NAMELESS_UNION_STRUCT  __pragma (warning( disable: 4201 )) // Warning	C4201	nonstandard extension used : nameless struct / union
#define IGNORE_WARNING_PUSH		__pragma (warning( push ))
#define IGNORE_WARNING_POP		__pragma (warning( pop ))

#endif // !__SAT_IGNORE_WARNING__
