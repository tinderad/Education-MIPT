#include "onegin.h"
#include <string.h>

char TESTS_lexicographical_compare[6][2][5] = {

{ "acde",    "abcd" },
{ " ,ab",    "::de" },
{ "abc ",    "abcd" },
{ "beef",    "DEAD" },
{ "zzzz",    "-__-" },
{ "DING",    "DING" }};


int TEST_RESULTS_lexicographical_compare[6]{
  GREATER,
  LESS,
  LESS,
  GREATER,
  GREATER,
  EQUAL };


char TESTS_reversed[4][2][5] = {

{ "abcd",    "dcba" },
{ "I go",    "og I" },
{ "meow",    "woem" },
{ "o__-",    "-__o" }};


int TEST_lexicographicalCompare(){

    int result = 0;

    for(int i = 0; i < 6; i++){
        if (LexicographicalCompare(TESTS_lexicographical_compare[i][0],TESTS_lexicographical_compare[i][1],1) != TEST_RESULTS_lexicographical_compare[i]){
            printf ("Test #%d failed. Input: %s %s | Result: %d | Expected: %d\n", i+1,
            TESTS_lexicographical_compare[i][0],TESTS_lexicographical_compare[i][1],
            (LexicographicalCompare(TESTS_lexicographical_compare[i][0],TESTS_lexicographical_compare[i][1],1)),
            TEST_RESULTS_lexicographical_compare[i]);

            result++;
        }
    }
    for(int i = 0; i < 6; i++){
        if (LexicographicalCompare(TESTS_lexicographical_compare[i][0] + 3,TESTS_lexicographical_compare[i][1] + 3, -1) != TEST_RESULTS_lexicographical_compare[i]){
            printf ("Test #%d failed. Input: %s %s | Result: %d | Expected: %d\n", i + 1,
            TESTS_lexicographical_compare[i][0],TESTS_lexicographical_compare[i][1],
            (LexicographicalCompare(TESTS_lexicographical_compare[i][0] + 3,TESTS_lexicographical_compare[i][1] + 3, -1)),
            TEST_RESULTS_lexicographical_compare[i]);

            result++;
        }
    }
    return result;
}


int TEST_myQsort(){

    char* name = (char*)calloc(9, sizeof(char));
    int result = 0;
    int number_of_lines;

    Comparator* comparators = (Comparator*)calloc(2,sizeof(Comparator));
    comparators[0] = ReversedLGComparator;
    comparators[1] = LGComparator;

    name[0] = 't';
    name[1] = '/';
    name[4] = '.';
    name[5] = 't';
    name[6] = 'x';
    name[7] = 't';
    name[8] = '\0';

    for(int i = 0; i < 2; i++){

        name[2] = (char)(i+48);
        name[3] = 'i';

        char* buffer = ReadFile(name);
        MyStr* lines = GetLines(buffer, &number_of_lines);
        MyQSort((void*)&lines, number_of_lines, comparators[i]);
        name[3] = 't';
        PrintFile(name, lines, number_of_lines);

        char* buffer_1 = ReadFile(name);
        name[3] = 'r';
        char* buffer_2 = ReadFile(name);

        int j = 0;
        char current_1 = buffer_1[j];
        char current_2 = buffer_2[j];
        bool failed = false;
        while (current_1 != '\0' && current_2 != '\0') {

            if (current_1 != current_2){
                result++;
                printf("First mismatch in test #%d at char #%d. See %dt.txt\n", i+1, j+1, i);
                failed = true;
                break;
            }
            j++;
            current_1 = buffer_1[j];
            current_2 = buffer_2[j];
        }

        if (!failed){
        name[3] = 't';
        remove(name);
        }

    }
    return result;
}
