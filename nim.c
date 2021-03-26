/***  Include Files ***********************************************************/
#include <stdio.h>  /* Function library: Standard input/output */
#include <string.h> /* String Library */
#include <time.h> /* Initialisation of the random generator */.

/*** Global declarations and definitions **********************************/
#define MAX_ZEILEN 4 /* Maximum number of rows */
#define MAX_STAEBCHEN 7 /* Maximum sticks */
#define GAME_OBJ '|' /* Representation of sticks */
#define MINIMUM 1
#define FLAG 1
#define ERROR_MSG "\n\nThis entry is not valid.\n"
#define ROW_CHOICE "\n\nFrom which row should sticks be taken: "
#define REMOVE_COUNT "\nHow many sticks to remove: "

/***  Funktions-Deklarationen *************************************************/
void introGame(); /* Game title */
void initGame(int *zeilearray); /* Create initialisation of the board */
void displayGame(const int *zeilearray, const char gameObj); /* Game display */
void playWithYoda(); /* NIM-Logic */
int checkInput(const int *zeilearray, const int maxLimit, const char* msg, const int rowFlag);
short endOfGame(const int *zeilearray); /* Block which determines end of game */
void showWinner(const char *winner); /* Winner display */
void AI(int *zeilearray, int *zug); /* Computer-Game */

/*******************************************************************************
******************************* MAIN PROGRAM ***********************************
*******************************************************************************/
int main()
{
    // local variables
    char answer[100];

    /* Intro -------------------------------- */
    introGame();

    /* Inout -------------------------------- */
    printf("\nDo you want to play with me? yes/no: ");
    scanf("%s", answer);

    // Evaluation input
    if ((strcmp(answer, "yes") == 0) || (strcmp(answer, "y") == 0))
    {
        playWithYoda();
    }
    else if ((strcmp(answer, "no") == 0) || (strcmp(answer, "n") == 0))
    {
        printf("\nGoodbye...\n");
    }
    else{
        printf("\nInvalid answer...\n");
    }

    /* Only during the development phase, delete later! */
    system ("PAUSE");
    return 0;
}

void introGame()
{
    printf("**************************************************\n");
    printf("*                  NIM GAME                      *\n");
    printf("**************************************************\n");
}

void initGame(int *zeilearray)
{
    int i;
    // Initialization, should only be called once.
    srand(time(NULL));
    for(i = 0;i < MAX_ZEILEN;i++)
    {
        // random int between 0 and 7
        zeilearray[i] = rand() % MAX_STAEBCHEN + 1;
    }
}

void displayGame(const int *zeilearray, const char gameObj)
{
    int i, j;
    introGame();
    // Game displays
    for(i = 0;i < MAX_ZEILEN;i++)
    {
        printf("\nZeile %d (%d): ", i + 1, zeilearray[i]);
        for(j = 0;j < zeilearray[i];j++)
        {
            printf("%c  ", gameObj);
        }
    }
    printf("\n\n**************************************************\n");
    printf("\n**************************************************\n");
}

void showWinner(const char *winner)
{
    system("cls");
    printf("The Winner: %s\n", winner);
}

// End of game determination
short endOfGame(const int *zeilearray )
{
    int i;
    for(i = 0;i < MAX_ZEILEN; i++)
    {
        if(zeilearray[i] > 0)
        {
            return 0;
        }
    }
    return 1;
}

void playWithYoda()
{
    int zeilearray[MAX_ZEILEN];
    int rowChoice;
    int removeCount;
    short player = 1;
    char gameObj = GAME_OBJ;
    initGame(zeilearray);
    /* Processing ------------------------- */
    do
    {
        int zug[2]; // zug[0]: removed row, zug[1]: removed sticks
        system("cls");
        displayGame(zeilearray, gameObj);
        // Select row
        rowChoice = checkInput(zeilearray, MAX_ZEILEN, ROW_CHOICE, 1); 
        // remove Sticks
        removeCount = checkInput(zeilearray, zeilearray[rowChoice], REMOVE_COUNT, 0);  
        zeilearray[rowChoice] -= removeCount;
        player = (player == 1)? 2 : 1;
        // Yoda's turn...
        if(player == 2 && endOfGame(zeilearray) != 1)
        {
            system("cls");
            displayGame(zeilearray, gameObj);
            AI(zeilearray, zug);
            printf("\n\nYoda has decided the move! Press enter to find what Yoda is going to play...");
            fflush(stdin);
            getchar();
            fflush(stdin);
            player = (player == 1)? 2 : 1;
        }
    } while(endOfGame(zeilearray) != 1);
    
    /* Output -------------------------------- */
    if(player == 2)
    {
        showWinner("You");
    }
    else
    {
        showWinner("Yoda");
    }
    
}

int checkInput(const int *zeilearray, const int max, const char* msg, const int rowFlag)
{
    int input;
    short invalid = 1;
    do
    {
            printf(msg);
            fflush(stdin);
            scanf("%d", &input);
            if(input < MINIMUM || input > max || (rowFlag == FLAG && zeilearray[--input] < MINIMUM))
            {
                printf(ERROR_MSG);
            }
            else
            {
                invalid = 0;
            }
    }while(invalid == 1);
    return input;
}

void AI(int *zeilearray, int *zug)
{
    int i;
    int zeroes = 0;
    int oneRows = 0;
    int zielzeile = 0;
    int maxCount = 0;
    short bitArray[MAX_ZEILEN][3] = {0};
    int powersCount[3] = {0};
    int index = 0;

    for(i = 0;i < MAX_ZEILEN;i++)
    {
        int temp = zeilearray[i];
        // Find the row that has no stick
        if(zeilearray[i] == 0)
        {
            zeroes++;
        }
        // Find the row with the maximum sticks
        if(zeilearray[i] > maxCount)
        {
            maxCount = zeilearray[i];
            zielzeile = i;
        }
        // Find the number of rowarray that have exactly 1 sticks
        if(zeilearray[i] == MINIMUM)
        {
            oneRows++;
        }

        // It returns the number of powers of 2 in each line.
        // e.g. the number 7 = 111 has a 4, a 2 and a 1
        // e.g. the number 5 = 101 has a 4 and a 1
        if(temp >= 4)
        {
            bitArray[i][2] = 1;
            temp -= 4;
            powersCount[2]++;
        }
        if(temp >= 2)
        {
            bitArray[i][1] = 1;
            temp -= 2;
            powersCount[1]++;
        }
        if(temp >= 1)
        {
            bitArray[i][0] = 1;
            powersCount[0]++;
        }
    }

    if((powersCount[2] % 2) == 1)
    {
        index = 2;
    }
    else if((powersCount[1] % 2) == 1)
    {
        index = 1;
    }

    for(i=MAX_ZEILEN-1; i>=0; i--)
    {
        if(bitArray[i][index] == 1)
        {
            zielzeile = i;
            bitArray[i][index] = 0;
            powersCount[index]--;
            index--;
            break;
        }
    }

    while(index >= 0)
    {
        if((powersCount[index] % 2) == 1)
        {
            bitArray[zielzeile][index] = !bitArray[zielzeile][index];
        }
        index--;
    }

    zug[1] = zeilearray[zielzeile];
    
    zeilearray[zielzeile] = bitArray[i][0] * 1 + bitArray[i][1] * 2 + bitArray[i][2] * 4;

    if(maxCount == 1 && zeilearray[zielzeile] != MINIMUM)
    {
        oneRows--;
    }
    else if(zeilearray[zielzeile] == MINIMUM)
    {
        oneRows++;
    }
    
    if(zeilearray[zielzeile] == 0)
    {
        zeroes++;
    }

    if((zeroes + oneRows) == MAX_ZEILEN && oneRows % 2 == 1 && maxCount != MINIMUM)
    {
        zeilearray[zielzeile] == 0 ? ++zeilearray[zielzeile] : --zeilearray[zielzeile];
    }

    // Return the removed row
    zug[0] = zielzeile;
    // Return the number of sticks removed
    zug[1] = zug[1] - zeilearray[zielzeile];
}
