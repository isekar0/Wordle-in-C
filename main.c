#include <stdio.h>
#include <stdlib>
#include <string.h>
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
for (int i = 0 ; i < num_lines ; i++) {
char* words[i] = NULL;
}
}

}
