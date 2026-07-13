#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

const char ALIVE = '@';
const char DEAD  = '.';

void clearScreen() {
    printf("\033[H\033[2J");
}

int readInt(int min, int max) {
    char buffer[100];
    int number;
    while (true) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input. Try again.\n");
            return 1;
        }
        if (sscanf(buffer, "%d", &number) != 1) {
            printf("Invalid data type! Please enter a valid number. Try again.\n");
            continue; 
        }
        if (number < min || number > max) {
            printf("Out of bounds! Number must be between %d and %d. Try again.\n", min, max);
            continue;
        }
        break;
    }
    return number;
}

char readChar() {
    char buffer[100];
    char c;
    while (true) {
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            return buffer[0];
        } else {
            printf("Invalid char, try again!\n");
            continue;
        }
    }    
}

void randomTable(char* table, int size) {
    for (int i=0; i<size*size; i++) {
        if (rand() % 2 == 0)
            table[i] = ALIVE;
        else
            table[i] = DEAD;
    }
}

void printTable(char* table, int size) {
    for (int i=0; i<size; i++){
        for (int j=0; j<size; j++){
            printf("%c ", table[i*size+j]);
        }
        printf("\n");
    }
}


int checkPositionLife(char* table, int size, int position) {
    if (position < 0 || position > (size*size)-1) // Invalid position
        return 0;
    if (table[position] == ALIVE)
        return 1;
    return 0;
 
}

int checkAliveNeighbors(char* table, int size, int position) {
    if (size == 1)
        return 0;

    int neighbors = 0;
    bool leftBorder = false;
    bool rightBorder = false;

    /* 
    Checking if a cell is in the first or last column, as to not check some of their neighbors.
    Cells above the first row or below the last row are already invalid in
    checkPositionLife(), so there's no need to check for that here.
    */
    int rowPosition = position % size;
    if (rowPosition == 0) 
        leftBorder = true;
    else if (rowPosition == size-1)
        rightBorder = true;

    // North neighbors
    neighbors += checkPositionLife(table, size, position-size);
    // South neighbors
    neighbors += checkPositionLife(table, size, position+size);

    // East, northeast, southeast neighbors
    if (!rightBorder) {
        neighbors += checkPositionLife(table, size, position+1);
        neighbors += checkPositionLife(table, size, position+size+1);
        neighbors += checkPositionLife(table, size, position-size+1);
    }
    // West, northwest, southwest neighbors
    if (!leftBorder) {
        neighbors += checkPositionLife(table, size, position-1);
        neighbors += checkPositionLife(table, size, position+size-1);
        neighbors += checkPositionLife(table, size, position-size-1);
    }

    return neighbors;
}

bool shouldDie(char* table, int size, int position) {
    int neighbors = checkAliveNeighbors(table, size, position);
    if (neighbors == 2 || neighbors == 3)
        return false;
    else return true;
}

bool shouldBirth(char* table, int size, int position) {
    int neighbors = checkAliveNeighbors(table, size, position);
    if (neighbors == 3)
        return true;
    else return false;
}

void updateTable(char* table, int size) {
    char* nextTable = (char*)malloc(size*size*sizeof(char));
    if (nextTable == NULL) {
        printf("Unable to alloc memory for table.\n");
        exit(1);
    }
    for (int i=0; i<size*size; i++)
        nextTable[i] = ' ';

    for (int i=0; i<size*size; i++) {
        if (table[i] == ALIVE) {
            if (shouldDie(table, size, i))
                nextTable[i] = DEAD;
            else nextTable[i] = ALIVE;
        }
        else {
            if (shouldBirth(table, size, i))
                nextTable[i] = ALIVE;
            else nextTable[i] = DEAD;
        }
    }

    for (int i=0; i<size*size; i++)
        table[i] = nextTable[i];

    free(nextTable);
}





int main() {
    srand(time(NULL));
    printf("Write the table size n x n (between 1 and 50):\n");
    int n = readInt(1, 50);

    printf("Table size: (%d x %d)", n, n);
    printf("\n\n\n");

    char* table = (char*)malloc(n*n*sizeof(char));
    if (table == NULL) {
        printf("Unable to alloc memory for table.\n");
        exit(1);
    }

    randomTable(table, n);

    char c = ' ';
    unsigned int generation = 1;

    // Main game loop
    while (true) {
        clearScreen();
        printf("===GAME OF LIFE===\n\n\n");
        printTable(table, n);
        printf("\nCurrent generation: %d", generation++);
        printf("\n\n\n");
        printf("- [ENTER] - next generation /// [X] - exit -\n");
        c = readChar();
        if (c == 'x' || c == 'X')
            break;
        else {
            updateTable(table, n);
            continue;
        }
    }

    free(table);

    return 0;
}
