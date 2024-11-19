#define _POSIX_C_SOURCE 202405L

#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv){
    char option;
    char separator = '\n';
    bool bigMax = false;
    FILE *outputFile = stdout;
    size_t quantity;
    size_t upperBound = 0xFFFFFFFFFFFFFFFF;

    while((option = getopt(argc, argv, "Mif:u:h?")) != -1){
        switch (option){
            case 'h':
                printf("Options are:\n");
                printf("\t-M:\t\tallows user to print up to %u numbers\n", ~0);
                printf("\t-f [FILENAME]:\tprint numbers to [FILENAME]\n");
                printf("\t-i:\t\tprint numbers in a single line spaced by a single character\n");
                printf("\t-u:\t\tset a max integer value\n");
                printf("\t-h:\t\tprint this message and exit\n");
                return 0;
            case 'M':
                bigMax = true;
                quantity = (uint32_t) quantity;
                break;
            case 'i': 
                separator = ' ';
                break;
            case 'f':
                outputFile = fopen(optarg, "w");
                if (outputFile == NULL){
                    printf("Couldn't create or open file '%s'\n", optarg);
                    return 1;
                }
                break;
            case 'u':
                sscanf(optarg, "%zu", &upperBound);
                upperBound++;
                break;
            case '?':
                printf("Try %s -h\n", argv[0]);
                return 1;
            default:
                printf("Try %s -h\n", argv[0]);
                break;
        }
    }

    if (bigMax){
        printf("WARNING: Large limit enabled, be careful\n");
        printf("Quantity of numbers to print (max %u): ", ~0);
        scanf("%zu", &quantity);
        quantity %= 0x100000000;
        if (quantity > 0xFFFF){
        warning:
            char planC;
            printf("WARNING: This will print a file of ~%zuMB\n", quantity / 100000);
            printf("Are you REALLY sure you need %zu numbers? [Y/n] ", quantity);
            getchar();
            scanf("%c", &planC);
            switch (planC){
                case 'y':
                case 'Y':
                    break;
                case 'n':
                case 'N':
                    return 1;
                default:
                    goto warning;
                    break;

            }
        }
    }else {
        printf("Quantity of numbers to print (max %u): ", 0xFFFF);
        scanf("%zu", &quantity);
        quantity %= 0x10000;
    }


    srand(time(0));

    for (uint32_t i = 0; i < quantity; i++){
        int32_t randomNumber = rand() % upperBound;
        fprintf(outputFile, "%i", randomNumber);
        fputc(separator, outputFile);
    }
    if (separator != '\n'){
        fputc('\n', outputFile);
    }

    fclose(outputFile);

    return 0;
}
