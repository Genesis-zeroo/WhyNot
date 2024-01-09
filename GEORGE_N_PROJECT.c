#include <stdio.h>
#include <stdlib.h>


typedef struct {
    int burstTime;
    int arrivalTime;
    int priority;
} Job;


void simulateCPU(Job *jobs, int numJobs, FILE *outputFile) {
    int currentTime = 0;
	int i;
    for (i = 0; i < numJobs; i++) {
        fprintf(outputFile, "Job %d starts at time %d\n", i + 1, currentTime);
        currentTime += jobs[i].burstTime;
        fprintf(outputFile, "Job %d finishes at time %d\n", i + 1, currentTime);
    }
}


int readJobsFromFile(char *filename, Job **jobs) {
    FILE *inputFile = fopen(filename, "r");
    if (!inputFile) {
        perror("Error opening input file");
        return -1;
    }

    int numJobs;
    fscanf(inputFile, "%d", &numJobs);

    *jobs = (Job *)malloc(numJobs * sizeof(Job));
	int i;
    for ( i = 0; i < numJobs; i++) {
        fscanf(inputFile, "%d:%d:%d", &((*jobs)[i].burstTime), &((*jobs)[i].arrivalTime), &((*jobs)[i].priority));
    }

    fclose(inputFile);
    return numJobs;
}

int main(int argc, char *argv[]) {
    if (argc != 5 || (strcmp(argv[1], "-f") != 0) || (strcmp(argv[3], "-o") != 0)) {
        printf("Usage: %s -f input_file -o output_file\n", argv[0]);
        return 1;
    }

    
    Job *jobs;
    int numJobs = readJobsFromFile(argv[2], &jobs);
    if (numJobs == -1) {
        return 1;
    }

   
    FILE *outputFile = fopen(argv[4], "w");
    if (!outputFile) {
        perror("Error opening output file");
        free(jobs);
        return 1;
    }

    
    simulateCPU(jobs, numJobs, outputFile);

    // Print results to the screen
    rewind(outputFile);
    int ch;
    while ((ch = fgetc(outputFile)) != EOF) {
        putchar(ch);
    }

    // Close the output file
    fclose(outputFile);

    
    free(jobs);

    return 0;
}
