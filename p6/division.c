////////////////////////////////////////////////////////////////////////////////
// Main File:        (division.c)
// This File:        (division.c)
// Other Files:      (sendsig.c, intdate.c)
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

#define MAX_LENGTH 30

int completed = 0; // times that division is completed

/**
 * Signal handler of SIGFPE
 */
void handler_SIGFPE() {
    printf("Error: a division by 0 operation was attempted.\n");
    printf("Total number of operations completed successfully: %d\n", completed);
    printf("The program will be terminated.\n"); 
    exit(0); 
}

/**
 * Signal handler of SIGINT
 */
void handler_SIGINT() {
    printf("\nTotal number of operations successfully completed: %d\n", completed);
    printf("The program will be terminated.\n");
    exit(0);
}

/**
 * Main function to do divison
 */
int main() {
    struct sigaction sa;
    struct sigaction sa_int;

    memset(&sa, 0, sizeof(sa));
    memset(&sa_int, 0, sizeof(sa_int));

    sa.sa_handler = handler_SIGFPE;
    sa_int.sa_handler = handler_SIGINT;

    if (sigaction(SIGFPE, &sa, NULL) != 0) {
        printf("Error binding SIGFPE handler.\n");
        exit(1);
    }

    if (sigaction(SIGINT, &sa_int, NULL) != 0) {
        printf("Error binding SIGINT handler.\n");
        exit(1);
    }
 
    while (1) {
        int int1;
        int int2;
        int result;
        int remainder;
        char str[MAX_LENGTH + 1];
 
        printf("Enter first integer: ");
        fgets(str, MAX_LENGTH + 1, stdin);
        int1 = atoi(str);

        printf("Enter second integer: ");
        fgets(str, MAX_LENGTH + 1, stdin);
        int2 = atoi(str);
        
        result = int1 / int2; 
        remainder = int1 - int2 * (int1 / int2);

        printf("%d / %d is %d with a remainder of %d\n", int1, int2, result, remainder);
        completed ++;

    }

    return 0;
}
