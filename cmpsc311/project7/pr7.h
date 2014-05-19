#ifndef PR7_H
#define PR7_H

/* CMPSC 311 Project 7 starter kit
 *
 * Authors:  Aaron Goodermuth, Brian Golden
 * Emails:   ajg5353@psu.edu,  bwg5079@psu.edu
 *
 * 10 April 2012
 */

/*----------------------------------------------------------------------------*/

#include <limits.h>

/*
 *  MAX_LINE            input line length
 *  MAX_PATH            directory name length
 *  MAX_CHILDREN        number of child processes
 */

#ifdef _POSIX_C_SOURCE
/* use the minimal value for maximal portability */

#define MAX_LINE        _POSIX_MAX_INPUT
#define MAX_PATH        _POSIX_PATH_MAX
#define MAX_CHILDREN    _POSIX_CHILD_MAX

#else
/* use the default value for this system */

#define MAX_LINE        MAX_INPUT
#define MAX_PATH        PATH_MAX
#define MAX_CHILDREN    CHILD_MAX

/* Alternative versions can be obtained from
 *   #include <unistd.h>
 *   sysconf(_SC_LINE_MAX)
 *   sysconf(_SC_CHILD_MAX)
 */

#endif

/*----------------------------------------------------------------------------*/
// other fun stuff
#define USAGE_STATUS_HELP 1
#define USAGE_STATUS_CMD 2

#endif
