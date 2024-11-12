#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>


#define SIZE 16


bool isValidIP(char *ip_addr) {
    int last_dot = -1;
    int first = 1;
    int num_dots = 0;
    int digits[4] = {0};
    int it = 0;


    for (int i = 0; i <= strlen(ip_addr); i++) {
        if (ip_addr[i] == '.' || ip_addr[i] == '\0') {
            num_dots++;
            if (num_dots > 4) {
                return false;
            }
            if (last_dot == i) {
                return false;
            }


            int num = 0;
            int mul = 1;
            for (int j = i - 1; j > last_dot; j--) {
                if (!isdigit(ip_addr[j])) {
                    return false;
                }
                num += (mul * (ip_addr[j] - '0'));
                mul *= 10;
            }


            digits[it++] = num;


            if (first && (num < 0 || num > 255)) {
                return false;
            } else if (num < 0 || num > 255) {
                return false;
            }


            if (first) {
                first = 0;
            }
            last_dot = i;
        }
    }


    if (num_dots != 4) {
        return false;
    }


    if (digits[0] <= 127) {
        printf("Class A\n");
    } else if (digits[0] <= 191) {
        printf("Class B\n");
    } else if (digits[0] <= 223) {
        printf("Class C\n");
    } else if (digits[0] <= 239) {
        printf("Class D\n");
    } else if (digits[0] <= 255) {
        printf("Class E\n");
    } else {
        return false;
    }


    return true;
}


int main() {
    char ip_addr[SIZE];
    printf("IP Address: ");
    scanf("%s", ip_addr);


    if (isValidIP(ip_addr)) {
        printf("Valid IP Address!\n");
    } else {
        printf("Invalid IP Address\n");
    }
}


