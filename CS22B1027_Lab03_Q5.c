#include <stdio.h>
#include <string.h>

#define POLY_LEN strlen(poly)

char data[28];
char crcRem[28];  
char poly[10] = "10001101";
int dataLength, i, j;

void performXOR() {
    int polyLength = POLY_LEN;
    for (j = 1; j < polyLength; j++) {
        crcRem[j] = (crcRem[j] == poly[j]) ? '0' : '1';
    }
}

void calculateCRC() {
    int polyLength = POLY_LEN;
    for (i = 0; i < polyLength; i++) {
        crcRem[i] = data[i];
    }
    do {
        if (crcRem[0] == '1') {
            performXOR();
        }
        for (j = 0; j < polyLength - 1; j++) {
            crcRem[j] = crcRem[j + 1];
        }
        crcRem[j] = data[i++];
    } while (i <= dataLength + polyLength - 1);
}

int main() {
    printf("Enter the data to transmit: ");
    scanf("%s", data);
    
    dataLength = strlen(data);
    int polyLength = POLY_LEN;

    for (i = dataLength; i < dataLength + polyLength - 1; i++) {
        data[i] = '0';
    }
    
    calculateCRC();
    printf("\nCRC or Checksum value is: %s", crcRem);
    
    for (i = dataLength; i < dataLength + polyLength - 1; i++) {
        data[i] = crcRem[i - dataLength];
    }
    
    printf("\nData to be transmitted: %s", data);
    calculateCRC();

    for (i = 0; (i < polyLength - 1) && (crcRem[i] != '1'); i++);
    if (i < polyLength - 1)
        printf("\nError detected during transmission.\n\n");
    else
        printf("\nNo error detected during transmission.\n\n");
    
    return 0;
}
