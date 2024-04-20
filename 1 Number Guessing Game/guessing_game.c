#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  int random, guess;
  int no_of_guess = 0;
  srand(time(NULL));
  
  printf("Welcome to the world of Guessing Numbers\n");
  random = rand() % 100 + 1; // Generating between 0 to 100

  do {
    printf("\nPlease enter your Guess between(1 to 100): ");
    scanf("%d", &guess);
    no_of_guess++;

    if (guess < random) {
      printf("Guess a larger number. \n");
    } else if (guess > random) {
      printf("Guess a smaller number. \n");
    } else {
      printf("\nCongratulations !!!You have successfully guessed the Number in %d attempts", no_of_guess);
    }

  } while (guess != random);

  printf("\nBye Bye, Thanks for Playing.");
  printf("\nDeveloped by: KGCoding");
}