#define _POSIX_C_SOURCE 202405L

#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>

#define NormalizeToShort(a) a %= 0x1'00'00
#define NormalizeToInt(a)   a %= 0x1'00'00'00'00
#define ConvertToMB(a)      a / 100'000
#define WARNING_TRIGGER     70'000

int main(int argc, char **argv){
    char option;
    char separator = '\n';
    bool bigMax = false;
    bool askQuantity = true;
    FILE *outputFile = stdout;
    uint64_t upperBound = ULONG_MAX;
    size_t quantity;

    while((option = getopt(argc, argv, "Miq:f:u:h?")) != -1){
        switch (option){
            case 'h':
                printf("Options are:\n");
                printf("\t-M:\t\tallows user to print up to %u numbers\n", UINT_MAX);
                printf("\t-f [FILENAME]:\tprint numbers to [FILENAME]\n");
                printf("\t-q [QUANTITY]:\tset amount of numbers to print\n");
                printf("\t-i:\t\tprint numbers in a single line spaced by a single character\n");
                printf("\t-u [MAX_VAL]:\tset a max integer value\n");
                printf("\t-h:\t\tprint this message and exit\n");
                return 0;
            case 'M':
                bigMax = true;
                quantity = (uint32_t) quantity;
                break;
            case 'i': 
                separator = ' ';
                break;
            case 'q':
                if (bigMax){ 
                    printf("option -q disabled with large max enabled\n"); 
                    break; 
                }
                sscanf(optarg, "%zu", &quantity);
                askQuantity = false;
                NormalizeToShort(quantity);
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
    if (askQuantity){
        if (!bigMax) {
            printf("Quantity of numbers to print (max %u): ", USHRT_MAX);
            scanf("%zu", &quantity);
            NormalizeToShort(quantity);
        }else {
            printf("WARNING: Large limit enabled, be careful\n");
            printf("Quantity of numbers to print (max %u): ", UINT_MAX);
            scanf("%zu", &quantity);
            NormalizeToInt(quantity);
            if (quantity > WARNING_TRIGGER){
            warning:
                char key;
                printf("WARNING: This will print a file of ~%zuMB\n", ConvertToMB(quantity));
                printf("Are you REALLY sure you need %zu numbers? [Y/n] ", quantity);
                getchar();
                scanf("%c", &key);
                switch (key){
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
        }

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
