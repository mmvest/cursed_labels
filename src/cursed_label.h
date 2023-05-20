#include <stdint.h>

#define CURSED_FLAG "cursedlabelcursedlabelcursedlab"   // 32 bytes including null terminator
#define CURSED_FLAG_SIZE 32                             // bytes

static uintptr_t *cur_scan_loc;
static uint8_t correct_context = 1;

struct __attribute__((__packed__)) cursed_flag
{
    uint8_t flag[CURSED_FLAG_SIZE];
};

#define LABEL_TO_FUNCTION(label) (void(*)())&&label

#define SET_STACK_FLAG struct cursed_flag stack_flag = {.flag = CURSED_FLAG};

#define SET_CONTEXT                                                 \
                    uintptr_t *correct_rbp = (cur_scan_loc - 1);    \
                    uintptr_t *correct_rsp = (cur_scan_loc + 1);    \
                    asm volatile(                                   \
                        "mov %%rsp, %0\n"                           \
                        "mov %%rbp, %1\n"                           \
                        :                                           \
                        : "r" (*correct_rsp), "r" (*correct_rbp)    \
                    );                                              \

#define SCAN_STACK_AND_SET_CONTEXT(func, label)                                 \
                    while(1)                                                    \
                    {                                                           \
                        if(*cur_scan_loc > &func || *cur_scan_loc < &label)     \
                        {                                                       \
                            SET_CONTEXT                                         \
                        }                                                       \
                        cur_scan_loc++;                                         \
                    }                                                           \

#define CHECK_FOR_FLAG                                                              \
                    struct cursed_flag *cf = (struct cursed_flag*)*cur_scan_loc;    \
                    for(uint8_t i = 0; i < CURSED_FLAG_SIZE; i++)                   \
                    {                                                               \
                        if(cf.flag[i] != CURSED_FLAG[i])                            \
                        {                                                           \
                            correct_context = 0;                                    \
                            break;                                                  \
                        }                                                           \
                    }                                                               \

#define CURSED_LABEL(func, label)                       \
                    SET_STACK_FLAG                      \
                    label:                              \
                        asm volatile(                   \
                            "mov %0, %rsp\n"            \
                            : "=r" (cur_scan_loc)       \
                        );                              \
                        CHECK_FOR_FLAG                  \
                        if(!correct_context)            \
                        {                               \
                            SCAN_STACK_AND_SET_CONTEXT  \
                        }                               \
