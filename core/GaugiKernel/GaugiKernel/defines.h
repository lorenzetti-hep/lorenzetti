#ifndef defines_h
#define defines_h


//#if !defined(__CINT__) && !defined(__CLING__)
//# define USING_MULTI_THREAD
//#endif

/**
 * Do not change this macro
 **/
#if (defined(_OPENMP) && defined(USING_MULTI_THREAD))
# define USE_OMP
#endif

#ifdef USE_OMP
# include <omp.h>
#endif


//the following are UBUNTU/LINUX ONLY terminal color codes.
#define GAUGI_RESET       "\033[0m"
#define GAUGI_NORMAL      GAUGI_RESET
#define GAUGI_BLACK       "\033[90m"             /* Black */
#define GAUGI_RED         "\033[31m"             /* Red */
#define GAUGI_GREEN       "\033[32m"             /* Green */
#define GAUGI_YELLOW      "\033[33m"             /* Yellow */
#define GAUGI_BLUE        "\033[34m"             /* Blue */
#define GAUGI_MAGENTA     "\033[35m"             /* Magenta */
#define GAUGI_CYAN        "\033[36m"             /* Cyan */
#define GAUGI_DARKCYAN    "\033[1m\033[36m"      /* Dark Cyan */
#define GAUGI_GRAY        "\033[30m"             /* Gray */
#define GAUGI_WHITE       "\033[97m"             /* White */
#define GAUGI_BOLDBLACK   "\033[1m\033[90m"      /* Bold Black */
#define GAUGI_BOLDRED     "\033[1m\033[91m"      /* Bold Red */
#define GAUGI_BOLDGREEN   "\033[1m\033[92m"      /* Bold Green */
#define GAUGI_BOLDYELLOW  "\033[1m\033[93m"      /* Bold Yellow */
#define GAUGI_BOLDBLUE    "\033[1m\033[94m"      /* Bold Blue */
#define GAUGI_BOLDMAGENTA "\033[1m\033[95m"      /* Bold Magenta */
#define GAUGI_BOLDCYAN    "\033[1m\033[96m"      /* Bold Cyan */
#define GAUGI_BOLDWHITE   "\033[1m\033[97m"      /* Bold White */


#endif // 
