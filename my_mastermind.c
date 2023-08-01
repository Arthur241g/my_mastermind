#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define CODE_LENGTH 4
#define MAX_ATTEMPTS 10

void generateSecretCode(char *secretCode) {
  char pieces[] = "012345678";
  int numPieces = strlen(pieces);
  srand(time(NULL));
  for (int i = 0; i < CODE_LENGTH; i++) {
    int index = rand() % numPieces;
    secretCode[i] = pieces[index];
  }
  secretCode[CODE_LENGTH] = '\0';
}

int isValidGuess(const char *guess) {
  if (strlen(guess) != CODE_LENGTH) {
    return 0;
  }

  for (int i = 0; i < CODE_LENGTH; i++) {
    if (guess[i] < '0' || guess[i] > '8') {
      return 0;
    }
  }

  return 1;
}

void getUserGuess(char *userGuess) {
  char input[CODE_LENGTH + 2]; // +2 to account for newline and null character
  ssize_t bytesRead = read(STDIN_FILENO, input, CODE_LENGTH + 1);

  if (bytesRead > 0) {
    input[bytesRead] = '\0';
    // Remove newline character
    if (input[bytesRead - 1] == '\n') {
      input[bytesRead - 1] = '\0';
    }
    strncpy(userGuess, input, CODE_LENGTH);
    userGuess[CODE_LENGTH] = '\0';

    // Check if the guess is valid
    if (!isValidGuess(userGuess)) {
      printf("Invalid guess.\n");
      // Exit the program
      exit(1);
    }
  } else {
    // Handle read error or end of file
    exit(1);
  }
}


void evaluateGuess(const char *secretCode, const char *userGuess, int *wellPlaced, int *misplaced) {
  *wellPlaced = 0;
  *misplaced = 0;

  // Create temporary arrays for marking visited pieces
  int secretCount[10] = {0};
  int guessCount[10] = {0};

  // Count well-placed pieces and fill arrays
  for (int i = 0; i < CODE_LENGTH; i++) {
    if (userGuess[i] == secretCode[i]) {
      (*wellPlaced)++;
    }
    secretCount[secretCode[i] - '0']++;
    guessCount[userGuess[i] - '0']++;
  }

  // Count misplaced pieces
  for (int i = 0; i < 10; i++) {
    *misplaced += (secretCount[i] < guessCount[i]) ? secretCount[i] : guessCount[i];
  }

  *misplaced -= *wellPlaced;

}

// Check if the user has an error in their combination
  void badguessuser(){
  
    printf("Wrong Input.\n");
  
}

void printResult(int wellPlaced, int misplaced) {
  printf("Well placed Pieces: %d\nMisplaced Pieces: %d\n", wellPlaced, misplaced);
}

int main(int argc, char *argv[]) {
  char secretCode[CODE_LENGTH + 1];
  char userGuess[CODE_LENGTH + 1];
  int wellPlaced, misplaced;
  int attempts = MAX_ATTEMPTS;

  // Check if custom code is provided
  if (argc > 1 && strcmp(argv[1], "-c") == 0 && argc >= 3 && strlen(argv[2]) == CODE_LENGTH) {
    strncpy(secretCode, argv[2], CODE_LENGTH);
    secretCode[CODE_LENGTH] = '\0';
  } else {
    // Generate random secret code
    generateSecretCode(secretCode);
  }

  if (argc > 1 && strcmp(argv[1], "-t") == 0 && argc >= 3) {
    attempts = atoi(argv[2]);
    if (attempts <= 0)
      attempts = MAX_ATTEMPTS;
  }

  printf("Will you find the secret code?\nPlease enter a valid guess\n");

  // Game loop
    for (int i = 0; i < attempts; i++) {
    printf("---\nRound %d\n", i);

    // Get user's guess
    getUserGuess(userGuess);

    // Evaluate the guess
    evaluateGuess(secretCode, userGuess, &wellPlaced, &misplaced);

    // Check if the user found the secret code
    if (wellPlaced == CODE_LENGTH) {
      printf("Congratz! You did it!\n");
      return 0;
    }

    // Print the result
    printResult(wellPlaced, misplaced);

    if (misplaced == 0 && wellPlaced == 0) {
        badguessuser();
    }
  }

  printf("---\nYou failed to find the secret code! The code was %s\n", secretCode);
  return 0;
}