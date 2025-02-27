#include <stdio.h>
#include <stdlib>
#include <string.h>
#include <random.h> // ?
#include <time.h> // ?
#define FILE_NAME "wordlist.txt"
using namespace std;

char* arrayCreat(void) {
FILE* file = fopen(FILE_NAME, "r");

if (!file) {
printf("Error opening file");
return -1;
}

char chr; 
int num_lines = 0;
while (chr != EOF) {
if (chr == '\n') {
num_lines += 1;
}
chr = getc(file);
}
rewind(file);
{
char* words[num_lines];
size_t n; // all lines are 5 letter words, so this can be optimized
for (size_t i = 0 ; i < num_lines ; i++) {
words[i] = NULL;
size_t n = 0;

getline(&words[i], &n, file);
  // add check for error getting line
}
}
return words;
}

char* /*???*/ chooseRandomWord(char* words, int number_words) {
int random_index = rand() % number_words;
return words[random_index];
}

int main(void) {
  return 0;
}
