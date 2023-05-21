#include <stdint.h>

#define CURSED_FLAG "cursedlabel"
#define FLAG_SIZE   (uint8_t)sizeof(uintptr_t) * 4 // bytes

static uintptr_t *cur_scan_loc;
static uint8_t correct_context = 1;

struct __attribute__((__packed__)) cursed_flag
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
                        "mov %0, %%rsp\n"                           \
                        "mov %1, %%rbp\n"                           \
                        :                                           \
                        : "r" (*correct_rsp), "r" (*correct_rbp)    \
                    );                                              

#define SCAN_STACK_AND_SET_CONTEXT(func, label)                                 \
                    while(1)                                                    \
                    {                                                           \
                        if((void *)*cur_scan_loc > (void*)&func || (void *)*cur_scan_loc < (void *)&label)     \
                        {                                                       \
                            SET_CONTEXT                                         \
                        }                                                       \
                        cur_scan_loc++;                                         \
                    }                                                           

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
                    SET_STACK_FLAG                                      \
                    label:                                              \
                        asm volatile(                                   \
                            "mov %%rsp, %0\n"                           \
                            : "=r" (cur_scan_loc)                       \
                        );                                              \
                        CHECK_FOR_FLAG                                  \
                        if(!correct_context)                            \
                        {                                               \
                            SCAN_STACK_AND_SET_CONTEXT(func, label)     \
                        }                                               
