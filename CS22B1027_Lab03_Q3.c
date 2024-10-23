#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 512
char *sender(char *input)
{
    char *output = (char *)malloc(MAX * sizeof(char));

    char seq[] = "01111110";
    int j = 0;
    int it = 0;
    int seq_flag = 0;
    int add_zero = 0;
    int i;
    for (i = 0;; i++)
    {
        if (i < 8 || input[it] == '\0')
        {
            output[i] = seq[j++];
            if (j == 8)
            {
                j = 0;
                seq_flag = (seq_flag + 1) % 2;
                if (!seq_flag)
                {
                    break;
                }
            }
        }
        else
        {
            if (input[it] == '1')
            {
                add_zero++;
            }
            else if (add_zero && input[it] == '0')
            {
                add_zero = 0;
            }

            output[i] = input[it++];

            if (add_zero == 5)
            {
                output[++i] = '0';
                add_zero = 0;
            }
        }
    }
    output[++i] = '\0';
    return output;
}

char *receiver(char *input)
{
    int i=0;
    int count=0;
    char *output = (char *)malloc(MAX * sizeof(char));
    int rem_zero = 0;
    int it=8;
    for (i = 0;it<strlen(input)-8; i++)
    {
        if (input[it] == '1')
        {
            rem_zero++;
        }
        else if (rem_zero && input[it] == '0')
        {
            rem_zero = 0;
        }

        output[i] = input[it++];

        if (rem_zero == 5 && input[it] == '0')
        {
            it++;
            count++;
            rem_zero=0;
        }
    }
    output[it-8-count] = '\0';
    return output;
}
int main()
{
    char input[MAX] ;
    printf("Enter string:");
    scanf("%s", input);

    char *output = sender(input);

    printf("\nStuffed output: %s\n\n", output);

    output = receiver(output);
    printf("\nDestuffed output: %s", output);

    return 0;
}