#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define POLY "1001"  // The polynomial used for CRC
#define POLY_SIZE strlen(POLY)
#define SIZE 100

// Function to perform XOR operation between two bits
char xor(char a, char b){
    return (a == b) ? '0' : '1';
}

// Function to calculate checksum using polynomial division
void calculateCheckSum(char* input, int size){
    char checksum[POLY_SIZE];  // To store the checksum

    // Create an intermediate buffer to store the input with appended zeros
    char inter[size + POLY_SIZE];
    
    // Copy the input into the intermediate buffer
    strcpy(inter, input);

    // Append POLY_SIZE-1 zeros at the end of the input
    for(int i = size; i < size + POLY_SIZE - 1; i++){
        inter[i] = '0';
    }
    inter[size + POLY_SIZE - 1] = '\0';  // Null-terminate the string

    printf("Transformed input: %s\n", inter);

    // Perform XOR division
    for(int i = 0; i <= strlen(inter) - POLY_SIZE; i++){
        if(inter[i] == '1'){  // Only apply XOR if the current bit is '1'
            for(int j = 0; j < POLY_SIZE; j++){
                inter[i + j] = xor(inter[i + j], POLY[j]);
            }
        }
    }

    // Extract the remainder (checksum) from the last POLY_SIZE-1 bits
    int j = 0;
    for(int i = strlen(inter) - (POLY_SIZE - 1); i < strlen(inter); i++){
        checksum[j++] = inter[i];
    }
    checksum[j] = '\0';  // Null-terminate the checksum

    // Output the final checksum
    printf("Checksum: %s\n", checksum);
}

int main(){
    // Example input string
    char input[] = "11001001";  
    int size = strlen(input);  // Get the correct size of the input

    // Calculate and display the checksum
    calculateCheckSum(input, size);

    return 0;
}
