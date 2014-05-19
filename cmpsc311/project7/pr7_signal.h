#ifndef PR6_SIGNAL_H
#define PR6_SIGNAL_H

/* CMPSC 311 Project 7 toolkit 
 *
 * Authors: Aaron Goodermuth, Brian Golden
 * Emails:  ajg5353@psu.edu,  bwg5079@psu.edu
 *
 * 10 April 2012
 */

/*----------------------------------------------------------------------------*/

#include <signal.h>

typedef void sighandler_t(int);
    /* the type of a function that has one int argument, and returns nothing */

int install_signal_handler(int sig, sighandler_t func);
    /* returns 0 if successful, -1 if not */

void reinstall_signal_handler(int sig, sighandler_t func);

/*----------------------------------------------------------------------------*/

#endif
