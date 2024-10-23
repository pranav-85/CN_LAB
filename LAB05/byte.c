#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void byteStuffing(unsigned char *originalData, unsigned char *stuffedData, int originalLength)
{
    int stuffedLength = 0;


    for (int i = 0; i < originalLength; i++)
    {
        if (originalData[i] == 0x7E)
        {
            stuffedData[stuffedLength++] = 0x7D; // Escape character
            stuffedData[stuffedLength++] = 0x5E; // Replaced 0x7E
        }
        else if (originalData[i] == 0x7D)
        {
            stuffedData[stuffedLength++] = 0x7D; // Escape character
            stuffedData[stuffedLength++] = 0x5D; // Replaced 0x7D
        }
        else
        {
            stuffedData[stuffedLength++] = originalData[i];
        }
    }


    // Null-terminate stuffed data for string representation (optional)
    stuffedData[stuffedLength] = '\0';
}


void byteDestuffing(unsigned char *stuffedData, unsigned char *originalData, int stuffedLength)
{
    int originalLength = 0;


    for (int i = 0; i < stuffedLength; i++)
    {
        if (stuffedData[i] == 0x7D)
        {
            i++; // Skip the escape character
            if (stuffedData[i] == 0x5E)
            {
                originalData[originalLength++] = 0x7E; // Replaced back to 0x7E
            }
            else if (stuffedData[i] == 0x5D)
            {
                originalData[originalLength++] = 0x7D; // Replaced back to 0x7D
            }
        }
        else
        {
            originalData[originalLength++] = stuffedData[i];
        }
    }


    // Null-terminate original data for string representation 
    originalData[originalLength] = '\0';
}


int main()
{
    unsigned char originalData[100];
    unsigned char stuffedData[200];
    unsigned char destuffedData[100];
    char input[300];


    printf("Enter the Original Data as hex values (e.g., 45 7E 56 7D 78): ");
    fgets(input, sizeof(input), stdin);


    // Convert input to byte array
    int originalLength = 0;
    char *token = strtok(input, " ");
    while (token != NULL)
    {
        originalData[originalLength++] = (unsigned char)strtol(token, NULL, 16);
        token = strtok(NULL, " ");
    }
    for(int i=0;i<originalLength;i++){
    printf("%02X\n",originalData[i]);
    }


    byteStuffing(originalData, stuffedData, originalLength);


    printf("Stuffed Data: ");
    for (int i = 0; i < 200 && stuffedData[i] != '\0'; i++)
    {
        printf("0x%02X ", stuffedData[i]);
    }
    printf("\n");


    byteDestuffing(stuffedData, destuffedData, strlen((const char *)stuffedData));
    printf("Destuffed Data: ");
    for (int i = 0; i < originalLength; i++)
    {
        printf("0x%02X ", destuffedData[i]);
    }
    printf("\n");


    return 0;
}
