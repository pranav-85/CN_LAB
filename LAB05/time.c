#include <stdio.h>
#include <time.h>
#include <unistd.h>  // for sleep()

int main() {
    // 1. Get the current time using time()
    time_t current_time = time(NULL);
    printf("Current time (time_t): %ld\n", current_time);

    // 2. Convert time_t to local time using localtime()
    struct tm *local = localtime(&current_time);
    printf("Local time: %s", asctime(local));  // asctime() converts struct tm to human-readable string

    // 3. Convert time_t to UTC time using gmtime()
    struct tm *utc = gmtime(&current_time);
    printf("UTC time: %s", asctime(utc));

    // 4. Format time using strftime()
    char buffer[100];
    strftime(buffer, sizeof(buffer), "Formatted local time: %Y-%m-%d %H:%M:%S\n", local);
    printf("%s", buffer);

    // 5. Get difference between two times using difftime()
    time_t future_time = current_time + 60;  // 60 seconds later
    double diff = difftime(future_time, current_time);
    printf("Difference between times: %.f seconds\n", diff);

    // 6. Measure CPU time used using clock()
    clock_t start = clock();
    // Simulate some processing with sleep
    sleep(2);
    clock_t end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("CPU time used: %.2f seconds\n", cpu_time_used);

    // 7. Convert struct tm back to time_t using mktime()
    time_t reconstructed_time = mktime(local);
    printf("Reconstructed time (time_t): %ld\n", reconstructed_time);
    printf("Reconstructed local time: %s", ctime(&reconstructed_time));  // ctime() directly converts time_t to human-readable string

    return 0;
}
