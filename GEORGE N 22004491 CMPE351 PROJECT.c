#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_JOBS 100

// Structure to represent a job
struct Job {
    int id;
    int arrivalTime;
    int executionTime;
};

// Function to simulate CPU service
void simulateCPU(struct Job jobs[], int numJobs, FILE *outputFile) {
    int currentTime = 0;
    int totalExecutionTime = 0;

    fprintf(outputFile, "Job ID\tArrival Time\tExecution Time\tCompletion Time\n");

    for (int i = 0; i < numJobs; i++) {
        // Simulate job execution
        if (currentTime < jobs[i].arrivalTime) {
            currentTime = jobs[i].arrivalTime;
        }

        fprintf(outputFile, "%d\t%d\t\t%d\t\t%d\n", jobs[i].id, jobs[i].arrivalTime,
                jobs[i].executionTime, currentTime + jobs[i].executionTime);

        currentTime += jobs[i].executionTime;
        totalExecutionTime += currentTime - jobs[i].arrivalTime;
    }

    fprintf(outputFile, "Average Turnaround Time: %.2f\n",
            (float)totalExecutionTime / numJobs);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s input_file output_file\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    FILE *outputFile = fopen(argv[2], "w");

    if (!inputFile || !outputFile) {
        perror("Error opening file");
        return 1;
    }

    struct Job jobs[MAX_JOBS];
    int numJobs = 0;

    // Read job information from the input file
    while (fscanf(inputFile, "%d %d %d", &jobs[numJobs].id, &jobs[numJobs].arrivalTime,
                  &jobs[numJobs].executionTime) == 3) {
        numJobs++;
        if (numJobs >= MAX_JOBS) {
            printf("Too many jobs. Increase MAX_JOBS.\n");
            return 1;
        }
    }

    // Call the function to simulate CPU service
    simulateCPU(jobs, numJobs, outputFile);

    // Close files
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
