#include "../src/cursed_label.h"

#include <stdlib.h>     // exit()
#include <stdio.h>      // printf()
#include <unistd.h>     // sleep()
#include <signal.h>     // signal()

int main()
{
    unsigned const sleep_time = 10;
    unsigned *test_buffer = malloc(4);
    signal(SIGINT, LABEL_TO_FUNCTION(exit_label));
    printf("[+] Label set as signal handler\n");
    test_buffer[0] = 100;
    test_buffer[1] = 200;
    test_buffer[2] = 300;
    test_buffer[3] = 400;

    for (unsigned i = 0; i < 4; i++)
    {
        printf("[+] test_buffer[%u]: %u\n", i, test_buffer[i]);
    }

    printf( "[+] Sleeping for %d seconds -- interrupt during this "
            "sleep to see the label called as the signal handler.\n", sleep_time);
    sleep(sleep_time);

    CURSED_LABEL(main, exit_label)
        printf("\n[+] We hit the label!\n");
        free(test_buffer);
        printf("[+] test_buffer freed\n");
        printf("[+] Press ENTER to exit...");
        getchar();
        exit(1);
    
    return 0;
}