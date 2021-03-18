///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        (n_in_a_row.c)
// This File:        (n_in_a_row.c)
// Other Files:      (NONE)
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
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
     
char *COMMA = ",";  
     
/* COMPLETED:       
 * Retrieves from the first line of the input file,
 * the size of the board (number of rows and columns).
 * 
 * fp: file pointer for input file
 * size: pointer to size
 */
void get_dimensions(FILE *fp, int *size) {      
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, fp) == -1) {
        printf("Error in reading the file.\n");
        exit(1);
    }

    char *token = NULL;
    token = strtok(line, COMMA);
    *size = atoi(token);
}       
 
   
  
/* TODO:
 * Returns 1 if and only if the board is in a valid state.
 * Otherwise returns 0.
 * 
 * board: heap allocated 2D board
 * size: number of rows and columns
 * return 1 if and only the board is in a valid state, and 0 otherwise.
 */
int n_in_a_row(int **board, int size) {
    // the size of a valid board can only be odd 
    if (size % 2 == 1) {
        int xNumber = 0; // the number of Xs
        int oNumber = 0; // the number of Os
        int xRowWinner = 0;
        int oRowWinner = 0;
        int xColumnWinner = 0;
        int oColumnWinner = 0;
        int isWinnerFound = 0;
        int elementInFirstRowFirstColumn = **board; // the element in the first row first column
        int elementInFirstRowLastColumn = *(*board + size - 1); // the element in the first row last column
        int xLeftWinner = 0;
        int oLeftWinner = 0;
        int xRightWinner = 0;
        int oRightWinner = 0; 
        int xWinner = 0; // the total number of X winners
        int oWinner = 0; // the total number of O winners

        // iterate through the board to compute the numbers of Xs and Os
        for (int i = 0; i < size; i ++) {
            for (int j = 0; j < size; j ++) {
                if (*(*(board + i) + j) == 1) {    
                    xNumber ++;
                } else if (*(*(board + i) + j) == 2) {
                    oNumber ++;
                }
            }
        }

        // the number of Xs can only be equal to that of Os or 1 more X than O 
        if (xNumber == oNumber || xNumber == (oNumber + 1)) {
            // look for the winning row
            for (int i = 0; i < size; i ++) {
                int rowFirstElement = **(board + i);
                isWinnerFound = 1;
                for (int j = 0; j < size; j ++) {
                    if (*(*(board + i) + j) == rowFirstElement) {
                        continue;
                    } else {
                        isWinnerFound = 0;
                        break;
                    }
                }
                // compute the numbers of X winners and O winners in rows 
                if (isWinnerFound == 1) {
                    if (rowFirstElement == 1) {
                        xRowWinner ++;
                    } else if (rowFirstElement == 2) {
                        oRowWinner ++;
                    }
                }
            }

            // either no winner or one winner, X and O cannot both be winners
            if (xRowWinner <= 1 && oRowWinner <= 1 && xRowWinner * oRowWinner == 0) {
                // look for the winning column
                for (int j = 0; j < size; j ++) {
                    isWinnerFound = 1;
                    int columnFirstElement = *(*board + j);
                    for (int i = 0; i < size; i ++) {
                        if (*(*(board + i) + j) == columnFirstElement) {
                            continue;
                        } else {
                            isWinnerFound = 0;
                            break;
                        }		
                    }
                    // computer the numbers of X winners and O winners in columns
                    if (isWinnerFound == 1) {
                        if (columnFirstElement == 1) {
                            xColumnWinner ++;
                        } else if (columnFirstElement == 2) {
                            oColumnWinner ++;
                        }
                    }
                }

                // either no winner or one winner, X and O cannot both be winners 
                if ((xRowWinner == 1 && oRowWinner == 0 && xColumnWinner <= 1 && oColumnWinner == 0) ||
                        (xRowWinner == 0 && oRowWinner == 1 && xColumnWinner == 0 && oColumnWinner <= 1) ||
                        (xRowWinner == 0 && oRowWinner == 0 && xColumnWinner <= 1 && oColumnWinner <= 1 &&
                            xColumnWinner * oColumnWinner == 0)) {
                    isWinnerFound = 1;

                    // look for the winning left diagonal
                    for (int i = 0; i < size; i ++) {
                        if (*(*(board + i) + i) == elementInFirstRowFirstColumn) {
                            continue;
                        } else {
                            isWinnerFound = 0;	
                            break;
                        }
                    }
                    if (isWinnerFound == 1) {
                        if (elementInFirstRowFirstColumn == 1) {
                            xLeftWinner = 1;
                        } else if (elementInFirstRowFirstColumn == 2) {
                            oLeftWinner = 1;
                        }
                    } 

                    isWinnerFound = 1;
                    // look for the winning right diagonal
                    for (int i = 0; i < size; i ++) {
                        for (int j = size - 1; j >= 0; j --) {
                            if (*(*(board + i) + j) == elementInFirstRowLastColumn) {
                                continue;
                            } else {
                                isWinnerFound = 0;	
                            }
                        }
                    }
                    if (isWinnerFound == 1) {
                        if (elementInFirstRowLastColumn == 1) {
                            xRightWinner = 1;
                        } else if (elementInFirstRowLastColumn == 2) {
                            oRightWinner = 1;
                        }
                    } 

                    // computer the overall number of X winners and O winners
                    xWinner = xRowWinner + xColumnWinner + xLeftWinner + xRightWinner;
                    oWinner = oRowWinner + oColumnWinner + oLeftWinner + oRightWinner;

                    // either no winner or one winner, X and O cannot both be winners
                    if ((xWinner <= 2 && oWinner == 0) || (xWinner == 0 && oWinner <= 2)) {
                        return 1;
                    }
                }
            }
        }	
    }

    return 0;   
}    
  
 
   
/* PARTIALLY COMPLETED:
 * This program prints Valid if the input file contains
 * a game board with either 1 or no winners; and where
 * there is at most 1 more X than O.
 * 
 * argc: CLA count
 * argv: CLA value
 */
int main(int argc, char *argv[]) {              
     
    //TODO: Check if number of command-line arguments is correct.
    if (argc != 2) {
        printf("The number of command-line arguments is wrong.\n");
        exit(1);
    }

    //Open the file and check if it opened successfully.
    FILE *fp = fopen(*(argv + 1), "r");
    if (fp == NULL) {
        printf("Can't open file for reading.\n");
        exit(1);
    }

    //Declare local variables.
	//size is in the range of 3 to 99 inclusive
    int size;

    //TODO: Call get_dimensions to retrieve the board size.
    get_dimensions(fp, &size);

    //TODO: Dynamically allocate a 2D array of dimensions retrieved above.
    int **array = (int**)malloc(sizeof(int*) * size);
    if (array == NULL) {
        printf("Can't allocate memory for array.\n");
        exit(1);
    }

    for (int i = 0; i < size; i ++) {
        *(array + i) = (int*)malloc(sizeof(int) * size); 
        if (*(array + i) == NULL) {
            printf("Can't allocate memory for array.\n");
            exit(1);
        }
    }

    //Read the file line by line.
    //Tokenize each line wrt comma to store the values in your 2D array.
    char *line = NULL;
    size_t len = 0;
    char *token = NULL;
    for (int i = 0; i < size; i++) {

        if (getline(&line, &len, fp) == -1) {
            printf("Error while reading the file.\n");
            exit(1);
        }

        token = strtok(line, COMMA);
        for (int j = 0; j < size; j++) {
            //TODO: Complete the line of code below
            //to initialize your 2D array.
            *(*(array + i) + j) = atoi(token);
            token = strtok(NULL, COMMA);
        }
    }

    //TODO: Call the function n_in_a_row and print the appropriate
    //output depending on the function's return value.
    if (n_in_a_row(array, size) == 1) {
        printf("valid\n");	
    } else if (n_in_a_row(array, size) == 0) {
        printf("invalid\n");
    }


    //TODO: Free all dynamically allocated memory.
    for (int i = 0; i < size; i ++) {
        free(*(array + i));
        *(array + i) = NULL;
    }
    free(array);
    array = NULL;

    //Close the file.
    if (fclose(fp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
    } 
     
    return 0;       
}
