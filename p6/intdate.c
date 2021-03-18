////////////////////////////////////////////////////////////////////////////////
// Main File:        (intdate.c)
// This File:        (intdate.c)
// Other Files:      (sendsig.c, division.c)
// Semester:         CS 354 Spring 2020
//
// Author:           (Xunwei Ye)
// Email:            (xye53@wisc.edu)
// CS Login:         (xunwei)
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          NONE
//                  
//
// Online sources:   NONE
//                    
//                   
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>

int alarm_counter = 3; // seconds to send a alarm
int user1_counter = 0; // counter of times of SIGUSR1 caught  

/**
 * Signal handler of SIGALRM
 */
void handler_SIGALRM() {
    
    time_t curtime;
    if (time(&curtime) == ((time_t) -1)) {
        printf("Error getting current time!\n");
        exit(1);
    }

    printf("PID: %d | Current Time: %s", getpid(), ctime(&curtime));
    alarm(alarm_counter);
}

/**
 * Signal handler of SIGUSR1
 */ 
void handler_SIGUSR1() {
    printf("SIGUSR1 caught!\n");
    user1_counter ++;
}

/**
 * Signal handler of SIGINT
 */
void handler_SIGINT() {
    printf("\nSIGINT received.\n");
    printf("SIGUSR1 was received %d times. Exiting now.\n", user1_counter);
    exit(0);
}

/**
 * Main function to trigger signals
 */
int main() {
    struct sigaction sa;
    struct sigaction sa_usr1;
    struct sigaction sa_int;

    // initialize sigaction structs
    memset(&sa, 0, sizeof(sa));
    memset(&sa_usr1, 0, sizeof(sa_usr1));
    memset(&sa_int, 0, sizeof(sa_int));
    
    sa.sa_handler = handler_SIGALRM;
    sa_usr1.sa_handler = handler_SIGUSR1;
    sa_int.sa_handler = handler_SIGINT;

    // bind handler to SIGALRM
    if (sigaction(SIGALRM, &sa, NULL) != 0) {
        printf("Error binding SIGALRM handler.\n");
        exit(1);
    }
    
    // bind handler to SIGUSR1
    if (sigaction(SIGUSR1, &sa_usr1, NULL) != 0) {
        printf("Error binding SIGUSR1 handler.\n");
        exit(1);
    }

    // bind handler to SIGINT
    if (sigaction(SIGINT, &sa_int, NULL) != 0) {
        printf("Errot binding SIGINT handler.\n");
        exit(1);
    }
 
    printf("Pid and time will be printed every 3 seconds.\n");
    printf("Enter ^C to end the program.\n");
    alarm(alarm_counter);
    while (1) {
    }
    
    return 0;
}
