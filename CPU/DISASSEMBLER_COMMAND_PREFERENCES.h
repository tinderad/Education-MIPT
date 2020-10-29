
#define CBITS_0(name)\
        fprintf(fp, "%s ", #name);\
        rip++;\

#define CBITS_1(name)\
        CBITS_0(name)\
        fprintf(fp, "%lf", *((double*)(buffer + rip)));\
        rip += sizeof(double);

#define CBITS_2(name)\
        CBITS_0(name)\
        fprintf(fp, "%s", GET_KEYWORD_NAME(buffer[rip]));\
        rip++;

#define CBITS_3(name)\
        CBITS_0(name)\
        fprintf(fp, "%s + ", GET_KEYWORD_NAME(buffer[rip]));\
        rip++;\
        fprintf(fp, "%lf", *((double*)(buffer + rip)));\
        rip+=sizeof(double);

#define CBITS_4(name)\
        CBITS_0(name)\
        fprintf(fp, "[ ]");\

#define CBITS_5(name)\
        CBITS_0(name)\
        fprintf(fp, "[ %lf ]", *((double*)(buffer + rip)));\
        rip += sizeof(double);

#define CBITS_6(name)\
        CBITS_0(name)\
        fprintf(fp, "[ %s ]", GET_KEYWORD_NAME(buffer[rip]));\
        rip++;


#define CBITS_7(name)\
        CBITS_0(name)\
        fprintf(fp, "[ %s + ", GET_KEYWORD_NAME(buffer[rip]));\
        rip++;\
        fprintf(fp, "%lf ]", *((double*)(buffer + rip)));\
        rip+=sizeof(double);

//-----------------------------------------------------------------

#define CBITS_0_PUSH\
        printf("Error: code corrupted, wrong use of \"PUSH\"\n");\
        exit(1);

#define CBITS_1_PUSH\
        CBITS_1(PUSH)\
        fprintf(fp, "\n");

#define CBITS_2_PUSH\
        CBITS_2(PUSH)\
        fprintf(fp, "\n");

#define CBITS_3_PUSH\
        CBITS_3(PUSH)\
        fprintf(fp, "\n");

#define CBITS_4_PUSH\
        CBITS_0_PUSH\

#define CBITS_5_PUSH\
        CBITS_5(PUSH)\
        fprintf(fp, "\n");

#define CBITS_6_PUSH\
        CBITS_6(PUSH)\
        fprintf(fp, "\n");

#define CBITS_7_PUSH\
        CBITS_7(PUSH)\
        fprintf(fp, "\n");

//-----------------------------------------------------------------

#define CBITS_0_POP\
        CBITS_0(POP)\
        fprintf(fp, "\n");

#define CBITS_1_POP\
        printf("Error: code corrupted, wrong use of \"POP\"\n");\
        exit(1);

#define CBITS_2_POP\
        CBITS_2(POP)\
        fprintf(fp, "\n");

#define CBITS_3_POP\
        printf("Error: code corrupted, wrong use of \"POP\"\n");\
        exit(1);

#define CBITS_4_POP\
        CBITS_3_POP\

#define CBITS_5_POP\
        CBITS_5(POP)\
        fprintf(fp, "\n");

#define CBITS_6_POP\
        CBITS_6(POP)\
        fprintf(fp, "\n");

#define CBITS_7_POP\
        CBITS_7(POP)\
        fprintf(fp, "\n");

//-------------------------------------------------------------------

#define DISASM_JMP(name)\
        CBITS_0(name)\
        fprintf(fp, "LABEL_%d\n", *((LABEL_TYPE*)(buffer + rip)));\
        rip+=sizeof(LABEL_TYPE);

//--------------------------------------------------------------------

#include "GENERATED_CMDS_CBIT_DISASSEMBLER.h" //Generated by python script