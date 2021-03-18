////////////////////////////////////////////////////////////////////////////////
// Main File:        (sendsig.c)
// This File:        (sendsig.c)
// Other Files:      (intdate.c, division.c)
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
#include <signal.h>
#include <string.h>
#include <sys/types.h>

/**
 * Main function to send signals
 */
int main(int argc, char *argv[]) {
    if (argc != 3 || (strcmp(argv[1], "-u") != 0 && strcmp(argv[1], "-i")!= 0)) {
        printf("Usage: <signal type> <pid>\n");
        exit(1);
    }
    
    pid_t pid = atoi(argv[2]);
 
    if (strcmp(argv[1], "-u") == 0) {
        if (kill(pid, SIGUSR1) != 0) {
            printf("Error sending SIGUSR1.\n");
            exit(1);
        }
    } else {
        if (kill(pid, SIGINT) != 0) {
            printf("Error sending SIGINT.\n");
            exit(1);
        }
    }
    return 0;
}


