#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#define GREATER 1
#define LESS -1
#define EQUAL 0


//------------------------------------------------------------------------------------
char* readFile(char* name);
void addChar(char** line_ptr, int* length_ptr, char to_add);
char** getLines(char* buffer, int* number_of_lines);
char* next_letter(char* a);
int lexicographicalCompare(char* a, char* b);
char* reversed(char* str);
int reversedLGComparator(const void* a, const void* b);
int LGComparator(const void* a, const void* b);
void bubbleSort (char** base, size_t num, size_t sz, int(*compare)(char* a, char* b));
void arrayPrint(char** arr, int length);
void printFile(char* name, char** lines, int length);
void* concat(char** a, char** b, char** c, int a_size, int b_size, int c_size);
void myQSort(void* lines_ptr, int length, int(*compare)(const void* a, const void* b));
//------------------------------------------------------------------------------------


char* readFile(char* name){

    FILE* fp = fopen(name, "r");
    char* buffer = nullptr;

    if (fp==nullptr){
        printf("Error: unable to open file");
        exit(1);
    }

    assert(fseek(fp, 0L, SEEK_END)==0);
    long int filesize = ftell(fp);
    buffer = (char*)realloc(buffer, filesize);
    rewind(fp);
    int read = fread(buffer, sizeof(char), filesize, fp);
    fclose(fp);

    printf("File read successfully\n");
    return buffer;
}


void addChar(char** line_ptr, int* length_ptr, char to_add){ //Не работает(

    *line_ptr = (char*)realloc(*line_ptr, (*length_ptr)+1);
    (*line_ptr)[*length_ptr] = to_add;
    (*length_ptr)++;
}


char** getLines(char* buffer, int* number_of_lines){

    char* line = nullptr;
    char** lines  = nullptr;
    int char_counter = 0;
    int line_counter = 0;

    int i = 0;
    char current = buffer[i];

    while (current != '\0'){
        if (current == '\n'){
            if (char_counter > 0){

                addChar(&line, &char_counter, '\0');
                lines = (char**)realloc(lines, (line_counter+1) * sizeof(char*));
                lines[line_counter] = line;
                line_counter++;
            }
            line = nullptr;
            char_counter = 0;
        }

        else {
            addChar(&line, &char_counter, current);
        }

        i++;
        current = buffer[i];
    }

    *number_of_lines = line_counter;
    return lines;
}


char* next_letter(char* a){

    if (*a == '\0'){
        return nullptr;
    }

    else if (isalpha(*a)){
        return a;
    }
    else{
        return next_letter(a+1);
    }
}


int lexicographicalCompare(char* a, char* b){

    a = next_letter(a);
    b = next_letter(b);

    if (a == nullptr || b == nullptr){

        if (a == nullptr && b == nullptr){
            return EQUAL;
        }

        if (a == nullptr && b != nullptr){
            return LESS;
        }

        if (a != nullptr && b == nullptr){
            return GREATER;
        }

    }

    if (*a > *b) {
        return GREATER;
    }

    else if (*a < *b) {
        return LESS;
    }

    else {
        return lexicographicalCompare(a+1,b+1);
    }
}


char* reversed(char* str){

    int length=0;
    char current = '\0';

    do{

        current = str[length];
        length++;

    }while (current != '\0');

    char* rev_str = (char*)calloc(length,1);
    int i = 0;

    do{
    current = str[i];

        if(i>0){
            rev_str[i-1] = str[length - i -1];
        }

        i++;

    }while (current != '\0');

    rev_str[length-1] = '\0';
    return rev_str;
}


int reversedLGComparator(const void* a, const void* b){
    return lexicographicalCompare(reversed((char*)a), reversed((char*)b));
}


int LGComparator(const void* a, const void* b){
    return lexicographicalCompare((char*)a, (char*)b);
}


void arrayPrint(char** arr, int length){

    for (int i = 0; i<length; i++){
        printf("%s\n", arr[i]);
    }
}


void printFile(char* name, char** lines, int length){

    FILE* fp;
    fp = fopen(name, "w");

    for (int i = 0; i < length; i++){
        fprintf(fp, "%s\n", lines[i]);
    }

    fclose(fp);
}


void* concat(char** a, char** b, char** c, int a_size, int b_size, int c_size){

    char** out = (char**)calloc(1, (sizeof(char*) * (a_size+b_size+c_size)) );

    for (int m = 0; m < a_size; m++){
        out[m] = a[m];
    }
    for (int m = 0; m < b_size; m++){
        out[m+a_size] = b[m];
    }
    for (int m = 0; m < c_size; m++){
        out[m+a_size+b_size] = c[m];
    }

    return out;
}


void myQSort(void* lines_ptr, int length, int(*compare)(const void* a, const void* b)){

    char** lines = *((char***)lines_ptr);
    char** ltp;
    char** gtp;
    char** etp;

    int ltp_length = 0;
    int gtp_length = 0;
    int etp_length = 0;

    char* pivot = lines[length-1];

    for (int i = 0; i<length; i++){

        char* element = lines[i];

        if ((*compare)((const void*)element, (const void*)pivot) < 0){
            ltp_length++;
        }
        if ((*compare)((const void*)element, (const void*)pivot) == 0){
            etp_length++;
        }
        if ((*compare)((const void*)element, (const void*)pivot) > 0){
            gtp_length++;
        }
    }

    ltp = (char**)calloc(ltp_length, sizeof(char*));
    etp = (char**)calloc(etp_length, sizeof(char*));
    gtp = (char**)calloc(gtp_length, sizeof(char*));

    int ltp_counter = 0;
    int etp_counter = 0;
    int gtp_counter = 0;

    for(int i = 0; i<length; i++){

        char* element = lines[i];

        if ((*compare)((const void*)element, (const void*)pivot) < 0){
            ltp[ltp_counter] = lines[i];
            ltp_counter++;
        }
        if ((*compare)((const void*)element, (const void*)pivot) == 0){
            etp[etp_counter] = lines[i];
            etp_counter++;
        }
        if ((*compare)((const void*)element, (const void*)pivot) > 0){
            gtp[gtp_counter] = lines[i];
            gtp_counter++;
        }
    }

    if (gtp_length != 0 || ltp_length != 0){
    myQSort(&ltp, ltp_length, (*compare));
    myQSort(&gtp, gtp_length, (*compare));
    }

    *((char***)lines_ptr) = (char**)concat(ltp, etp, gtp, ltp_length, etp_length, gtp_length);
}


int main(){

    char* buffer = readFile("hamlet.txt");
    int number_of_lines;
    char** lines = getLines(buffer, &number_of_lines);
    printf("Processing started, wait please\n");
    myQSort((void*)&lines, number_of_lines, reversedLGComparator);
    printf("Processing ended, writing to a file\n");
    printFile("out.txt", lines, number_of_lines);
    printf("Writed successfully");
    return 0;

}
