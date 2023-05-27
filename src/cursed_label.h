#include <stdint.h>

uintptr_t   *cur_scan_loc;
uint8_t     correct_context = 1;
uintptr_t   flag = 0xDEADBEEF;
uintptr_t   *correct_rbp;
uintptr_t   *correct_rsp;


#define LABEL_TO_FUNCTION(label) (void(*)())&&label

#define CURSED_LABEL(func, label)                                                                                           \
   asm volatile(                                                                                                            \
            "push %0"                                                                                                       \
            :                                                                                                               \
            :"r" (flag)                                                                                                     \
   );                                                                                                                       \
    label:                                                                                                                  \
        asm volatile(                                                                                                       \
            "mov %%rsp, %0\n"                                                                                               \
            : "=r" (cur_scan_loc)                                                                                           \
        );                                                                                                                  \
                                                                                                                            \
        if((uintptr_t)*cur_scan_loc != (uintptr_t)0xDEADBEEF && (uintptr_t)*(cur_scan_loc + 1) != (uintptr_t)0xDEADBEEF)    \
        {                                                                                                                   \
            correct_context = 0;                                                                                            \
        }                                                                                                                   \
                                                                                                                            \
        while(!correct_context)                                                                                             \
        {                                                                                                                   \
            if((void *)*cur_scan_loc > (void*)&func && (void *)*cur_scan_loc < (void *)&&label)                             \
            {                                                                                                               \
                correct_rbp = (cur_scan_loc - 1);                                                                           \
                correct_rsp = (cur_scan_loc + 1);                                                                           \
                asm volatile(                                                                                               \
                    "mov %0, %%rbp\n"                                                                                       \
                    "mov %1, %%rsp\n"                                                                                       \
                    :                                                                                                       \
                    : "r" (*correct_rbp), "r" (correct_rsp)                                                                 \
                );                                                                                                          \
                correct_context = 1;                                                                                        \
                break;                                                                                                      \
            }                                                                                                               \
            cur_scan_loc++;                                                                                                 \
        }                                                                                                               
