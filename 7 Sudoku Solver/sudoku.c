#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


void print_puzzle(int puzzle[9][9]);
int valid_move(int puzzle[9][9], int x, int y, int val);
int solve_puzzle(int puzzle[9][9], int x, int y);
int valid_puzzle(int puzzle[9][9]);
void clear_screen();

int sudoku[9][9] = {
  {3,0,0,0,2,0,0,7,0},
  {9,0,0,5,0,0,0,1,4},
  {0,1,6,3,7,0,0,0,8},
  {2,0,0,8,0,0,0,0,1},
  {5,0,0,0,4,1,8,0,0},
  {0,8,9,0,0,0,0,5,0},
  {0,0,5,0,1,0,2,8,0},
  {0,4,0,0,0,6,0,9,3},
  {7,3,1,0,8,2,0,0,0},
};

struct timespec ts;

int main(void)
{
    ts.tv_sec  = 0;
    ts.tv_nsec = 50000000;

    printf("Original Puzzle : \n");
    print_puzzle(sudoku);
    if (valid_puzzle(sudoku))
    {
        if (solve_puzzle(sudoku, 0, 0))
        {
            printf("The solved puzzle is : \n");
            print_puzzle(sudoku);
        }
        else
        {
            printf("OOPS..The puzzle is not solvable :(\n");
        }
    }
    else
    {
        printf("\nSorry, the given puzzle itself is wrong :)\n");
    }

    return 0;
}


int solve_puzzle(int puzzle[9][9], int x, int y)
{
    nanosleep(&ts, NULL);
    clear_screen();
    print_puzzle(puzzle);
    if (x == 9)
    {
        if (y == 8)
        {
            return 1;
        }
        y = y + 1;
        x = 0;
    }

    if (puzzle[y][x] > 0)
    {
        return solve_puzzle(puzzle, x + 1, y);
    }

    for (int i = 1; i <= 9; i++)
    {
        if (valid_move(sudoku, x, y, i))
        {
            sudoku[y][x] = i;
            if (solve_puzzle(sudoku, x + 1, y))
            {
                return 1;
            }
            sudoku[y][x] = 0;
        }
    }
    return 0;
}


int valid_move(int puzzle[9][9], int x, int y, int val)
{
    for (int i = 0; i < 9; i++)
    {
        if (puzzle[y][i] == val)
        {
            return 0;
        }

        if (puzzle[i][x] == val)
        {
            return 0;
        }
    }

    int c = x - x % 3;
    int r = y - y % 3;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (puzzle[r + i][c + j] == val)
            {
                return 0;
            }
        }
    }
    return 1;
}


int valid_puzzle(int puzzle[9][9])
{
    for (int i =0; i<9; i++)
    {
        for (int j = 0; j<9; j++)
        {
            if (puzzle[i][j]==0)
            {
                continue;
            }
            else
            {
                int temp = puzzle[i][j];
                puzzle[i][j] = 0;

                if (valid_move(puzzle, j, i, temp) == 0)
                {
                    return 0;
                }

                puzzle[i][j] = temp;
            }
        }
    }
    return 1;
}


void print_puzzle(int puzzle[9][9])
{
    printf("\n+-------+-------+-------+\n");
    for (int i = 0; i < 9; i++)
    {
        if (i == 3 || i == 6)
        {
            printf("|-------+-------+-------|\n");
        }
        for (int j = 0; j < 9; j++)
        {
            if (j % 3 == 0)
            {
                printf("| ");
            }
            if (puzzle[i][j] != 0)
            {
                printf("%d ", puzzle[i][j]);
            }
            else
            {
                printf("  ");
            }
        }
        printf("|");
        printf("\n");
    }
    printf("+-------+-------+-------+\n");
}

void clear_screen()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
