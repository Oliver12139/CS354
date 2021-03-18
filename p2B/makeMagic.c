///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        (makeMagic.c)
// This File:        (makeMagic.c)
// Other Files:      (makeMagic.c)
// Semester:         CS 354 Spring 2020
//
// Author:           (Xunwei Ye)
// Email:            (xye53@wisc.edu)
// CS Login:         (xunwei)
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   Avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
    int size;     // dimension of the square
    int **msquare; // pointer to heap allocated magic square
} MSquare;

/* TODO:
 * Prompts the user for the magic square's size, reads it,
 * checks if it's an odd number >= 3 (if not display the required
 * error message and exit), and returns the valid number.
 */
int getSize() {
    int size;
    char inputStr[20];
    
    printf("Enter magic square's size (odd integer >= 3)\n");
    
    // get the user's input for the magic square's size
    fgets(inputStr, 20, stdin);    
    size = atoi(inputStr);
    if (size % 2 == 0) {
        printf("Size must be odd.\n");
        exit(1);
    }

    if (size < 3) {
        printf("Size must be >= 3.\n");
        exit(1);
    }
    
    return size;   
} 
   
/* TODO:
 * Makes a magic square of size n using the Siamese algorithm
 * and returns a pointer to the completed MSquare struct.
 *
 * n the number of rosw and columns
 */
MSquare *generateMSquare(int n) {
    MSquare *square = (MSquare*)malloc(sizeof(MSquare));
    square->size = n;
    int numberOfRemaining = n * n;
    int nextNumber;
    int row;
    int column;
   
    // dynamically allocate a 2D array of n rows and n columns 
    square->msquare = (int**)malloc(sizeof(int*) * n);
    if (square->msquare == NULL) {
        printf("Can't allocate memory for array.\n");
        exit(1);
    }
 
    for (int i = 0; i < n; i ++) {
        *(square->msquare + i) = (int*)malloc(sizeof(int) * n);
        if (*(square->msquare + i) == NULL) {
            printf("Can't allocate memory for array.\n");
            exit(1);
        }
    }  

    // initialize all the elements of the 2D array to 0
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < n; j ++) {
            *(*(square->msquare + i) + j) = 0;
        }
    }     
       
    // place 1 at the center column of the topmost row in the square
    *(*(square->msquare) + n / 2) = 1;
    nextNumber = 2;
    row = 0;
    column = n / 2;
    numberOfRemaining --;
    
    // while the magic square have not been fully filled with numbers
    while (numberOfRemaining != 0) {
        // if it is currently at the first row, last column
        if (row == 0 && (column == n - 1)) {
            *(*(square->msquare + 1) + n - 1) = nextNumber;
            row = 1;
            column = n - 1;
            nextNumber ++;
            numberOfRemaining --;
            continue; 
        // if it is currently at the first row, but not last column
        } else if (row == 0 && (column != (n - 1))) {
            // check if the next position is already filled with a number
            if(*(*(square->msquare + n - 1) + column + 1) != 0) {
                *(*(square->msquare + 1) + column) = nextNumber;
                row = 1;
                column = column;
                nextNumber ++;
                numberOfRemaining --;
            } else { 
                *(*(square->msquare + n - 1) + column + 1) = nextNumber;
                row = n - 1;
                column ++;
                nextNumber ++;
                numberOfRemaining --;
            }
            continue; 
        // if it is currently at the last column     
        } else if (row != 0 && column == n - 1) {          
            **(square->msquare + row - 1) = nextNumber;
            row --;
            column = 0;
            nextNumber ++;
            numberOfRemaining --;
            continue;
        } else {
            // check if the next position is already filled with a number
            if (*(*(square->msquare + row - 1) + column + 1) != 0) {
                *(*(square->msquare + row + 1) + column) = nextNumber;
                row ++;
                column = column;
                nextNumber ++;
                numberOfRemaining --;
            } else {
                *(*(square->msquare + row - 1) + column + 1) = nextNumber;
                row --;
                column ++;
                nextNumber ++;
                numberOfRemaining --;
            } 
            continue;
        }             
    }
       
    return square;    
} 

/* TODO:  
 * Opens a new file (or overwrites the existing file)
 * and writes the square in the specified format.
 *
 * msquare the magic square to output
 * filename the name of the output file
 */
void outputMSquare(MSquare *msquare, char *filename) {
    // open the file and check if it opened successfully
    FILE *fp = fopen(filename, "w");   
    if (fp == NULL) {
        printf("Can't open the file for writing.\n");
        exit(1);
    }
    
    int size = msquare->size;
    // print the size in the first line of file
    fprintf(fp, "%d\n", msquare->size);

    for (int i = 0; i < size; i ++) {
        for (int j = 0; j < size - 1; j ++) {
            fprintf(fp, "%d,", *(*(msquare->msquare + i) + j));
        }
        fprintf(fp, "%d\n", *(*(msquare->msquare + i) + size - 1));
    }

    // Close the file
    if (fclose(fp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
    }
}

/* TODO:
 * Generates a magic square of the user specified size and
 * output the quare to the output filename
 */
int main(int argc, char *argv[]) {
    // TODO: Check input arguments to get output filename
    if (argc != 2) {
        printf("The number of command-line arguments is wrong.\n");
        exit(1);
    }
       
    char *filename = *(argv + 1);

    // TODO: Get magin square's size from user
    int size = getSize();

    // TODO: Generate the magic square
    MSquare *msquare = generateMSquare(size);       

    // TODO: Output the magic square
    outputMSquare(msquare, filename);
    
    // free all dynamically allocated memory
    for (int i = 0; i < size; i ++) {
        free(*(msquare->msquare + i));
        *(msquare->msquare + i) = NULL;
    }
    free(msquare->msquare);
    msquare->msquare = NULL;

    return 0;
} 


   
