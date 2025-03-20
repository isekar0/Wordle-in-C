#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#define FILE_NAME "wordlist.txt"
// #define NUM_WORDS 5195
#define INIT_SIZE 256
#define WORD_LENGTH 5  

typedef struct ListOfWords {
    char **words;
    int num_words;
} ListOfWords;

// LettersStatus.letters[0] = 'A', LettersStatus.status[0] = 'u'
typedef struct LettersStatus {
    char letters[26];
    char status[26]; // default unguessed = 'u', red = 'r', yellow = 'y', green = 'g'
                     // max #green = 5
} LettersStatus;

void freeArray(char ***, const int);
ListOfWords *createWordList(void);
void destroyWordList(ListOfWords **);
char *getRandomWord(const ListOfWords);
LettersStatus *initLettersStatus(void);

void printLetterStatus(const LettersStatus, int, int);
void printAvailableLetters(const LettersStatus);
void printRandomWord(const LettersStatus);
void updateAvailableLetters(LettersStatus *, const char *, const char *);
int checkWin(const char *, const char *);
void gameLoop(const ListOfWords);

int main() {
    srand(time(0));
    ListOfWords *list_of_words = createWordList();
    // for (int i = 0; i < list_of_words->num_words; i++) {
    //     printf("%s", list_of_words->words[i]);
    // }
    char *random_word = getRandomWord(*list_of_words);
    printf("The random word is: %s\n", random_word);
    
    destroyWordList(&list_of_words);

    printf("About to initialize letters_status\n");
    LettersStatus *letters_status = initLettersStatus();
    // printf("intialized letters_status successfully\n");

    // printAvailableLetters(*letters_status);
    
    
    return 0;
}

void freeArray(char ***arr, const int SIZE) {
    if (arr != NULL && *arr != NULL) {
        for (int i = 0; i < SIZE; i++) {
            free(*arr[i]);
            *arr[i] = NULL;
        }
        free(*arr);
        *arr = NULL;
    }
}

ListOfWords *createWordList(void) {
    ListOfWords *ptr_list_of_words = malloc(sizeof(ListOfWords));
    if (ptr_list_of_words == NULL) {
        fprintf(stderr, "Error allocating memory to ptr_list_of_words");
    }
    // opening file
    FILE *ptr_file = fopen(FILE_NAME, "r");
    if (ptr_file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", FILE_NAME);
        exit(EXIT_FAILURE);
    }
    // initialize words
    int arr_size = INIT_SIZE;
    ptr_list_of_words->words = malloc(sizeof(char *) * arr_size);
    if (ptr_list_of_words->words == NULL) {
        fprintf(stderr, "Error allocating initial memory to words\n");
        fclose(ptr_file);
        exit(EXIT_FAILURE);
    }
    
    // begin reading from file
    int i = 0;
    char buffer[WORD_LENGTH + 1];
    while (fgets(buffer, WORD_LENGTH + 1, ptr_file) != NULL) {
        // check if words is full, if so, expand
        if (i >= arr_size) {
            int new_arr_size = arr_size * 2; // double memory each time
            char **temp_arr = realloc(ptr_list_of_words->words, sizeof(char *) * new_arr_size);
            if (temp_arr == NULL) {
                fprintf(stderr, "Error allocating memory to temp_arr, i = %d, arr_size = %d\n", i, arr_size);
                freeArray(&ptr_list_of_words->words, i);
                fclose(ptr_file);
                exit(EXIT_FAILURE);
            }
            else {
                arr_size = new_arr_size;
                ptr_list_of_words->words = temp_arr;
            }
        }
        // reading line into words[i]
        ptr_list_of_words->words[i] = malloc(WORD_LENGTH + 1);
        if (ptr_list_of_words->words[i] == NULL) {
            fprintf(stderr, "Error allocating memory to words[%d], arr_size = %d\n", i, arr_size);
            freeArray(&ptr_list_of_words->words, i);
            fclose(ptr_file);
            exit(EXIT_FAILURE);
        }
        else {
            strncpy(ptr_list_of_words->words[i], buffer, WORD_LENGTH + 1);
            i++;
        }
    }
    fclose(ptr_file);
    // deallocate memory if actual number of words is less than allocated number of words
    if (i < arr_size) {
        char **temp_arr = realloc(ptr_list_of_words->words, sizeof(char *) * i);
            if (temp_arr == NULL) {
                fprintf(stderr, "Error allocating memory to temp_arr, i = %d, arr_size = %d\n", i, arr_size);
                freeArray(&ptr_list_of_words->words, i);
                fclose(ptr_file);
                exit(EXIT_FAILURE);
            }
            else {
                ptr_list_of_words->words = temp_arr;
            }
    }
    ptr_list_of_words->num_words = i;
    return ptr_list_of_words;
}

void destroyWordList(ListOfWords **list_of_words) {
    if (list_of_words != NULL && *list_of_words != NULL) {
            for (int i = 0; i < (*list_of_words)->num_words; i++) {
                free((*list_of_words)->words[i]);
                (*list_of_words)->words[i] = NULL;
            }
        free((*list_of_words)->words);
        (*list_of_words)->words = NULL;
        free(*list_of_words);
        *list_of_words = NULL;
    }
}

char *getRandomWord(const ListOfWords list_of_words) {
    int rand_int = rand() % list_of_words.num_words;
    char *rand_word = list_of_words.words[rand_int];
    // something went wrong and some words can be whitespace
    if (rand_word == NULL || rand_word[0] == '\0' || rand_word[0] == '\n') {
        rand_word = getRandomWord(list_of_words);
    }
    return rand_word;
}

LettersStatus *initLettersStatus(void) {
    printf("Entered initLettersStatus\n");
    LettersStatus *ptr_letters_status;
    for (int i = 0; i < 26; i++) {
        ptr_letters_status->letters[i] = 'A' + i;
        ptr_letters_status->status[i] = 'u';
    }
    printf("Exited for loop in initLettersStatus\n");
    return ptr_letters_status;
}

void printLetterStatus(const LettersStatus letters_status, int i, int j) {
    printf("Entered printLetterStatus with j + i * 7 = %d\n", j + i * 7);
    switch (letters_status.status[j + i * 7]) {
        case 'r': printf("\033[31m%c | \033[0m", letters_status.letters[j + i * 7]); break;
        case 'y': printf("\033[33m%c | \033[0m", letters_status.letters[j + i * 7]); break;
        case 'g': printf("\033[32m%c | \033[0m", letters_status.letters[j + i * 7]); break;
        default: printf("%c | ", letters_status.letters[j + i * 7]);
    }
}

void printAvailableLetters(const LettersStatus letters_status) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 7; j++) {
            // printf("%c | ", 'A' + j + i * 7);
            printLetterStatus(letters_status, i, j);
        }
        printf("\n");
    }
    for (int i = 0; i < 5; i++) {
        // printf("%c | ", 'A' + 21 + i);
        printLetterStatus(letters_status, 21 + i, 0);
    }
    printf("\n");
}
