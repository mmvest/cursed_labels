#include "../src/cursed_label.h"

#include <stdlib.h>     // exit()
#include <stdio.h>      // printf()
#include <unistd.h>     // sleep()
#include <signal.h>     // signal()

int main()
{
    signal(SIGINT, LABEL_TO_FUNCTION(exit_label));
    printf( "[+] Sleeping for 10 seconds -- interrupt during this "
            "sleep to see the label called as the signal handler.\n");
    sleep(10);

    exit_label:
        printf("\n[+] We hit the label!\n");
        printf("[+] Press ENTER to exit...\n");
        getchar();
        exit(1);
    
    return 0;
}