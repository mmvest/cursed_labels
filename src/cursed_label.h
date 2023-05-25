#include <stdint.h>

#define CURSED_FLAG "cursedlabel"
#define FLAG_SIZE   (uint8_t)sizeof(uintptr_t) * 4 // bytes

uintptr_t *cur_scan_loc;
uint8_t correct_context = 1;

struct cursed_flag
{
    uint8_t flag[FLAG_SIZE];
};

#define LABEL_TO_FUNCTION(label) (void(*)())&&label

#define SET_STACK_FLAG                                                                      \
                    struct cursed_flag stack_flag = { 0 };                                  \
                    for (uint8_t i = 0; i < FLAG_SIZE; i++)                   \
                    {                                                                       \
                        stack_flag.flag[i] = (uint8_t)CURSED_FLAG[i%sizeof(CURSED_FLAG)];   \
                    }                                                                       

#define SET_CONTEXT                                                 \
                    uintptr_t *correct_rbp = (cur_scan_loc - 1);    \
                    uintptr_t *correct_rsp = (cur_scan_loc + 1);    \
                    asm volatile(                                   \
                        "mov %0, %%rbp\n"                           \
                        "mov %1, %%rsp\n"                           \
                        :                                           \
                        : "r" (*correct_rbp), "r" (correct_rsp)    \
                    );                                              

#define SCAN_STACK_AND_SET_CONTEXT(func, label)                                 \
                    while(!correct_context)                                                    \
                    {                                                           \
                        if((void *)*cur_scan_loc > (void*)&func && (void *)*cur_scan_loc < (void *)&&label)     \
                        {                                                       \
                            printf("LOL");\
                            fflush(stdout);\
                            correct_context = 1;                                \
                        }                                                       \
                        cur_scan_loc++;                                         \
                    }\
                    SET_CONTEXT                                                                                                

#define CHECK_FOR_FLAG                                                                          \
                    for(uint8_t i = 0; i < FLAG_SIZE; i++)                                      \
                    {                                                                           \
                        if(((uint8_t*)cur_scan_loc)[i] != CURSED_FLAG[i%sizeof(CURSED_FLAG)])  \
                        {                                                                       \
                            correct_context = 0;                                                \
                            break;                                                              \
                        }                                                                       \
                    }                                                                           

#define CURSED_LABEL(func, label)                                       \
                    uintptr_t flag = 0xDEADBEEF;                        \
                    label:                                              \
                        asm volatile(                                   \
                            "mov %%rsp, %0\n"                           \
                            : "=r" (cur_scan_loc)                       \
                        );                                              \
                        if((uintptr_t)*cur_scan_loc != (uintptr_t)0xDEADBEEF && (uintptr_t)*(cur_scan_loc + 1) != (uintptr_t)0xDEADBEEF)      \
                        {                                               \
                            SCAN_STACK_AND_SET_CONTEXT(func, label)               \
                        }                                               
