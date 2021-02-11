#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions
void printPuzzle(char** arr, int n);
void searchPuzzle(char** arr, int n, char** list, int listSize);
void searchLefttoRight(char** arr, int n, char** list, int listSize);
void searchToptoBottom(char** arr, int n, char** list, int listSize);
void searchBottomtoTop(char** arr, int n, char** list, int listSize);
void searchToptoBottomDiagonal(char** arr, int n, char** list, int listSize);
void searchBottomtoTopDiagonal(char** arr, int n, char** list, int listSize);

// Main function, DO NOT MODIFY!!!
int main(int argc, char **argv) {
    int bSize = 15;
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
		return 2;
	}
    int i, j;
    FILE *fptr;
    char **block = (char**)malloc(bSize * sizeof(char*));
	char **words = (char**)malloc(50 * sizeof(char*));

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

	// Read puzzle block into 2D arrays
    for(i=0; i<bSize; i++){
        *(block+i) = (char*)malloc(bSize * sizeof(char));

        fscanf(fptr, "%c %c %c %c %c %c %c %c %c %c %c %c %c %c %c\n", *(block+i), *(block+i)+1, *(block+i)+2, *(block+i)+3, *(block+i)+4, *(block+i)+5, *(block+i)+6, *(block+i)+7, *(block+i)+8, *(block+i)+9, *(block+i)+10, *(block+i)+11, *(block+i)+12, *(block+i)+13, *(block+i)+14 );
    }
	fclose(fptr);

	// Open file for reading word list
	fptr = fopen("states.txt", "r");
	if (fptr == NULL) {
        printf("Cannot Open Words File!\n");
        return 0;
    }

	// Save words into arrays
	for(i=0; i<50; i++){
		*(words+i) = (char*)malloc(20 * sizeof(char));
		fgets(*(words+i), 20, fptr);
	}

	// Remove newline characters from each word (except for the last word)
	for(i=0; i<49; i++){
		*(*(words+i) + strlen(*(words+i))-2) = '\0';
	}

	// Print out word list
	printf("Printing list of words:\n");
	for(i=0; i<50; i++){
		printf("%s\n", *(words + i));
	}
	printf("\n");

	// Print out original puzzle grid
    printf("Printing puzzle before search:\n");
    printPuzzle(block, bSize);
	printf("\n");

	// Call searchPuzzle to find all words in the puzzle
	searchPuzzle(block, bSize, words, 50);
	printf("\n");

	// Print out final puzzle grid with found words in lower case
    printf("Printing puzzle after search:\n");
    printPuzzle(block, bSize);
	printf("\n");

    return 0;
}

void printPuzzle(char** arr, int n){
	// This function will print out the complete puzzle grid (arr). It must produce the output in the SAME format as the samples in the instructions.
	// Your implementation her
  int i, j;
  for(i = 0; i < n; i++){
    for(j = 0; j < n; j++){
      printf("%c", *(*(arr + i) + j));
    }
    printf("\n");
  }
}

void searchPuzzle(char** arr, int n, char** list, int listSize){
	// This function checks if arr contains words from list. If a word appears in arr, it will print out that word and then convert that word entry in arr into lower case.
	// Your implementation here

  printf("searching from Left to Right...\n");
  searchLefttoRight(arr, n, list, listSize);
  printf("\nsearching from Top to Bottom...\n");
  searchToptoBottom(arr, n, list, listSize);
  printf("\nsearching from Bottom to Top...\n");
  searchBottomtoTop(arr, n, list, listSize);
  printf("\nsearching from Top to Bottom Diagonally...\n");
  searchToptoBottomDiagonal(arr, n, list, listSize);
  printf("\nsearching from Bottom to Top Diagonally...\n");
  searchBottomtoTopDiagonal(arr, n, list, listSize);
}

bool ifEqual(char* s1, char* s2){
  int i;
  if(strlen(s1) == strlen(s2)){
    for(i = 0; i < strlen(s2); i++){
      if(*(s1 + i) != *(s2 + i)){
        return false;
      }
    }
    return true;
  }
  return false;
}

char* checkRow(int startAt,char* row, int rowSize,char** list, int listSize){
  int i, j, k;
  int indexCount; //holds the index for each hold
  char* hold;
  char* listHold;
  int * indexHold;
  int indexStartHold;
  int indexEndHold;
  //check throughout list
  for(i = 0; i < listSize - 1; i++){
    //iterate through the length of word at list[i]
    indexCount = 0;
    hold = malloc(strlen(*(list)) + 1);
    listHold = malloc(strlen(*(list)) + 1);

    //printf("checking if word = %s...\n", *(list + i));
    for(j = startAt; j < rowSize - 1; j++){
      //add (char)row[j] to hold and (char)list[i][j]
      //if hold isn't empty, then it is not the first letter, so lowercase
      if(indexCount == 0){
        indexStartHold = j;
      }
      //if 1st letter and the letter is lowercase then to check make it upper case, if other than 1st letter make lower case if not lower case
      if(indexCount >= 1 && *(row + j) <= 90 && *(row + j) >= 65){
        *(hold + indexCount) = *(row + j) + 32;
      }else if(indexCount == 0 && *(row + j) >= 97 && *(row + j) <= 122){
        *(hold + indexCount) = *(row + j) - 32;
      }else{
        *(hold + indexCount) = *(row + j);
      }

      //add to each one at a time then compare
      *(listHold + indexCount) = *(*(list + i) + indexCount);

      //if they don't equal reset

      //PRINTS TO CHECK
      //
      //printf("list + i = %s, ", *(list + i));
      //printf("listHold = %s, ", listHold);
      //printf("hold = %s \n", hold);

      if(ifEqual(hold, listHold) == false){
        indexCount = 0; //reset index to start from beginning of char*
        hold = malloc(strlen(*(list)) + 1); //resets hold
        listHold = malloc(strlen(*(list)) + 1); //resets listHold
        indexStartHold = 0;
      }else if(ifEqual(hold, *(list + i))){
        indexEndHold = j;
        if(rowSize - j >= 6){
          checkRow(j, row, rowSize, list, listSize);
        }
        break;
      }else{
      indexCount++;
      }
      //continue iterating until length of the word at list[i] then move to next word
    }
    //if found word
    if(ifEqual(hold, *(list + i))){
      int x = 0;
      //check within hold ex: Arkansas -> Arkansas and Ar-[Kansas]
      //x = 2 so it doesn't re check the same word.
        for(x = 2; x <= strlen(hold); x++){
          checkRow(x, hold, strlen(hold) + 1, list, listSize);
        }
      printf("\nFound word: %s", hold);
      //check if any words are within the word
      for(x = indexStartHold; x <= indexEndHold; x++){
        if(*(row + x) >= 65 && *(row + x) <= 90){
          *(row + x) = *(row + x) + 32;
        }
      }
      return row;
    }
  }
  //didn't find word
  //printf("\nFound no words: \n");
  //*(indexHold + 0) = -1;
  //*(indexHold + 1) = -1;
  return row;
  free(hold);
  free(listHold);
}
//5 directions: horizontal left to right (→), vertical top to bottom (), vertical bottom to up (), diagonal top left to bottom right (\), and diagonal bottom left to top right (/).


void searchLefttoRight(char** arr, int n, char** list, int listSize){
  int i, j, x, y;
  for(i = 0; i < n; i++){
    char* row = (char*)malloc(strlen(*(arr + i)));
    for(j = 0; j < n; j++){
      *(row + j) = *(*(arr + i) + j);
    }
    //printf("\ncheck row: %s \n", row);
    char* result = checkRow(0, row, n, list, listSize);
    //printf("\n result = %s", result);
    for(x = 0; x < n; x++){
      *(*(arr + i) + x) = *(result + x);
    }
    free(row);
  }
}

void searchToptoBottom(char** arr, int n, char** list, int listSize){
  int i, j, x;
  for(i = 0 ; i < n; i++){
    char* row = (char*)malloc(strlen(*(arr + i)));
    for(j = 0; j < n; j++){
      *(row + j) = *(*(arr + j) + i);
    }
    //printf("\ncheck row: %s \n", row);
    char* result = checkRow(0, row, n, list, listSize);
    //printf("\nresult = %s", result);
    for(x = 0; x < n; x++){
      *(*(arr + x) + i) = *(result + x);
    }
    free(row);
  }
}

void searchBottomtoTop(char** arr, int n, char** list, int listSize){
  int i, j, k, x;
  for(i = 0; i < n; i++){
    k = n - 1; //counting up while j counts backwards
    char* row = (char*)malloc(strlen(*(arr + i)));
    for(j = 0; j < n; j++){
      *(row + k) = *(*(arr + j) + i);
      k--;
    }
    //printf("\ncheck row: %s \n", row);
    char* result = checkRow(0, row, n, list, listSize);
    //printf("\nresult = %s", result);
    k = n - 1;
    for(x = 0; x < n; x++){
      *(*(arr + x) + i) = *(result + k);
      k--;
    }
    //printf("%s \n", result);
    free(row);
  }
}

void searchToptoBottomDiagonal(char** arr, int n, char** list, int listSize){
  int i, j, k, x;
  //search from left to right
  int rowLength = 0;
    for(i = 0 ; i < n; i++){
      char* row = (char*)malloc(strlen(*(arr + i)));
      k = 0;
      for(j = i; j < n; j++){
        *(row + k) = *(*(arr + k) + j);
        k++;
      }
    if(k > 4){
      //printf("\ncheck row: %s \n", row);
      k = 0;
      char* result = checkRow(0, row, n, list, listSize);
      //printf("\nresult = %s", result);
      for(x = i; x < n; x++){
        *(*(arr + k) + x) = *(result + k);
        k++;
      }
    }
    //char* newRow = rowFound(*(result + 0), *(result + 1), *(arr  + i));
    free(row);
  }
  //search from top to bottom
    for(i = 1 ; i < n; i++){
      char* row = (char*)malloc(strlen(*(arr + i)));
      k = 0;
      for(j = i; j < n; j++){
        *(row + k) = *(*(arr + j) + k);
        k++;
      }
    if(k > 4){
      //printf("\ncheck row: %s \n", row);
      k = 0;
      char* result = checkRow(0, row, n, list, listSize);
      //printf("\nresult = %s", result);
      for(x = i; x < n; x++){
        *(*(arr + x) + k) = *(result + k);
        k++;
      }
    }
    free(row);
  }
}

void searchBottomtoTopDiagonal(char** arr, int n, char** list, int listSize){
  int i, j, k, z;
  //search from left to right
    for(i = n - 1; i >= 0; i--){
      char* row = (char*)malloc(strlen(*(arr + i)));
      k = 0;
      for(j = i; j >= 0; j--){
        *(row + k) = *(*(arr + j) + k);
        k++;
      }
    if(k > 4){
      //printf("\ncheck row: %s \n", row);
      k = 0;
      char* result = checkRow(0, row, n, list, listSize);
      //printf("\nresult = %s", result);
      for(z = i; z >= 0; z--){
        *(*(arr + z) + k) = *(result + k);
        k++;
      }
    }
    //char* newRow = rowFound(*(result + 0), *(result + 1), *(arr  + i));
    free(row);
  }
  //search from top to bottom
  for(i = n - 1; i >= 0; i--){
    char* row = (char*)malloc(strlen(*(arr + i)));
    k = 0;
    int x = 14; //since it should always start at arr[14][j]
    for(j = 0; j < n; j++){
      *(row + k) = *(*(arr + x) + (j + i)); //(j + i) to move it over;
      k++;
      x--;
    }
    if(k > 4){
      //printf("\ncheck row: %s \n", row);
      k = 0;
      char* result = checkRow(0, row, n, list, listSize);
      //printf("\nresult = %s", result);
      x = 14;
      for(z = 0; z < n; z++){
        *(*(arr + x) + (z + i)) = *(result + k);
        k++;
        x--;
      }
    }
    //char* newRow = rowFound(*(result + 0), *(result + 1), *(arr  + i));
    free(row);
  }
}
