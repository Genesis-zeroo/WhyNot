#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>


typedef struct {
    int burstTime;
    int arrivalTime;
    int priority;
    int waitingTime;
} Job;


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

// Function to simulate CPU scheduling using FCFS algorithm
void simulateCPU_FCFS(Job *jobs, int numJobs, FILE *outputFile) {
    int currentTime = 0;
	int i;
    for ( i = 0; i < numJobs; i++) {
        fprintf(outputFile, "Job %d starts at time %d\n", i + 1, currentTime);
        jobs[i].waitingTime = currentTime - jobs[i].arrivalTime;
        currentTime += jobs[i].burstTime;
        fprintf(outputFile, "Job %d finishes at time %d\n", i + 1, currentTime);
    }
}


// Function to simulate CPU scheduling using SJF algorithm (Non-Preemptive)
void simulateCPU_SJF(Job *jobs, int numJobs, FILE *outputFile) {

    int i;
    for ( i = 0; i < numJobs - 1; i++) {
    	int j;
        for ( j = 0; j < numJobs - i - 1; j++) {
            if (jobs[j].burstTime > jobs[j + 1].burstTime) {
               
                Job temp = jobs[j];
                jobs[j] = jobs[j + 1];
                jobs[j + 1] = temp;
            }
        }
    }

    int currentTime = 0;
    int totalWaitingTime = 0;

    fprintf(outputFile, "Scheduling Method: Shortest Job First – Non-Preemptive\n");
    fprintf(outputFile, "Process Waiting Times:\n");
	
    for ( i = 0; i < numJobs; i++) {
        fprintf(outputFile, "P%d: %d ms\n", i + 1, currentTime - jobs[i].arrivalTime);
        totalWaitingTime += currentTime - jobs[i].arrivalTime;

        currentTime += jobs[i].burstTime;
    }

    fprintf(outputFile, "Average Waiting Time: %.2f ms\n", (float)totalWaitingTime / numJobs);
}









// Function to simulate CPU scheduling using SJF algorithm (Preemptive)
void simulateCPU_Priority(Job *jobs, int numJobs, int preemptive, FILE *outputFile)
/*void simulateCPU_SJF(Job *jobs, int numJobs, FILE *outputFile)*/ {
    int *remainingTime = (int *)malloc(numJobs * sizeof(int));
    int currentTime = 0;
    int totalWaitingTime = 0;

   
    int i;
    for ( i = 0; i < numJobs; i++) {
        remainingTime[i] = jobs[i].burstTime;
    }

    fprintf(outputFile, "Scheduling Method: Shortest Job First –Preemptive\n");
    fprintf(outputFile, "Process Waiting Times:\n");

    int done = 0;

    while (!done) {
        done = 1;
		int i;
        for ( i = 0; i < numJobs; i++) {
            if (remainingTime[i] > 0) {
                done = 0;

                fprintf(outputFile, "P%d: %d ms\n", i + 1, currentTime - jobs[i].arrivalTime);
                totalWaitingTime += currentTime - jobs[i].arrivalTime;

                if (remainingTime[i] > 1) {
                    currentTime++;
                    remainingTime[i]--;
                } else {
                    currentTime += remainingTime[i];
                    remainingTime[i] = 0;
                }
            }
        }
    }

    fprintf(outputFile, "Average Waiting Time: %.2f ms\n", (float)totalWaitingTime / numJobs);

    free(remainingTime);
}





// Function to simulate CPU scheduling using Round Robin scheduling
void simulateCPU_RoundRobin(Job *jobs, int numJobs, int timeQuantum, FILE *outputFile) {
    int *remainingTime = (int *)malloc(numJobs * sizeof(int));
    int *arrivalOrder = (int *)malloc(numJobs * sizeof(int));
    int currentTime = 0;
    int totalWaitingTime = 0;

    
    int i;
    for ( i = 0; i < numJobs; i++) {
        remainingTime[i] = jobs[i].burstTime;
        arrivalOrder[i] = i;  
    }

    fprintf(outputFile, "Scheduling Method: Round Robin Scheduling – time_quantum=%d\n", timeQuantum);
    fprintf(outputFile, "Process Waiting Times:\n");

    while (1) {
        int done = 1;  

        for ( i = 0; i < numJobs; i++) {
            if (remainingTime[i] > 0) {
                done = 0;  

                int executeTime = (remainingTime[i] > timeQuantum) ? timeQuantum : remainingTime[i];

                fprintf(outputFile, "P%d: %d ms\n", arrivalOrder[i] + 1, currentTime);
                totalWaitingTime += currentTime - jobs[arrivalOrder[i]].arrivalTime;

                currentTime += executeTime;
                remainingTime[i] -= executeTime;

                
                int tempOrder = arrivalOrder[i];
                int j;
                for (j = i; j < numJobs - 1; j++) {
                    arrivalOrder[j] = arrivalOrder[j + 1];
                }
                arrivalOrder[numJobs - 1] = tempOrder;
            }
        }

        if (done) {
            break;  
        }
    }

    fprintf(outputFile, "Average Waiting Time: %.2f ms\n", (float)totalWaitingTime / numJobs);

    free(remainingTime);
    free(arrivalOrder);
}









// Function to calculate and display waiting times
void calculateWaitingTimes(Job *jobs, int numJobs, FILE *outputFile) {
    int totalWaitingTime = 0;

    fprintf(outputFile, "Process Waiting Times:\n");
    int i;
    for ( i = 0; i < numJobs; i++) {
        fprintf(outputFile, "P%d: %d ms\n", i + 1, jobs[i].waitingTime);
        totalWaitingTime += jobs[i].waitingTime;
    }

    fprintf(outputFile, "Average Waiting Time: %.2f ms\n", (float)totalWaitingTime / numJobs);
}

int main(int argc, char *argv[]) {
    char *inputFileName = NULL;
    char *outputFileName = NULL;
    int schedulingMethod = 0; // 0: None, 1: FCFS, 2: SJF, 3: Priority, 4: Round Robin
    int preemptive = 0;
    int timeQuantum = 0;

    int opt;
    while ((opt = getopt(argc, argv, "f:o:")) != -1) {
        switch (opt) {
            case 'f':
                inputFileName = optarg;
                break;
            case 'o':
                outputFileName = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s -f input_file -o output_file\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (inputFileName == NULL || outputFileName == NULL) {
        fprintf(stderr, "Usage: %s -f input_file -o output_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Read jobs from the input file
    Job *jobs;
    int numJobs = readJobsFromFile(inputFileName, &jobs);
    if (numJobs == -1) {
        exit(EXIT_FAILURE);
    }

    // Open the output file
    FILE *outputFile = fopen(outputFileName, "w");
    if (!outputFile) {
        perror("Error opening output file");
        free(jobs);
        exit(EXIT_FAILURE);
    }

    do {
        printf("CPU Scheduler Simulator\n");
        printf("1) Scheduling Method (None)\n");
        printf("2) Preemptive Mode (Off)\n");
        printf("3) Show Result\n");
        printf("4) End Program\n");
        printf("Option > ");
        scanf("%d", &opt);

        switch (opt) {
            case 1:
                printf("Choose Scheduling Method:\n");
                printf("1. None\n");
                printf("2. First Come, First Served Scheduling\n");
                printf("3. Shortest-Job-First Scheduling\n");
                printf("4. Priority Scheduling\n");
                printf("5. Round-Robin Scheduling\n");
                printf("Option > ");
                scanf("%d", &schedulingMethod);

                if (schedulingMethod == 5) {
                    printf("Enter time quantum for Round Robin: ");
                    scanf("%d", &timeQuantum);
                }

                break;

            case 2:
                printf("Choose Preemptive Mode:\n");
                printf("1. Preemptive Mode\n");
                printf("2. Non-preemptive Mode\n");
                printf("Option > ");
                scanf("%d", &preemptive);
                break;

            case 3:
                switch (schedulingMethod) {
                    case 1:
                        fprintf(outputFile, "Scheduling Method: None\n");
                        break;
                    case 2:
                        fprintf(outputFile, "Scheduling Method: First Come First Served\n");
                        simulateCPU_FCFS(jobs, numJobs, outputFile);
                        break;
                    case 3:
                        fprintf(outputFile, "Scheduling Method: Shortest Job First – Non-Preemptive\n");
                        simulateCPU_SJF(jobs, numJobs, 0, outputFile); // Non-preemptive
                        break;
                    case 4:
                        fprintf(outputFile, "Scheduling Method: Priority Scheduling – ");
                        fprintf(outputFile, preemptive ? "Preemptive\n" : "Non-Preemptive\n");
                        simulateCPU_Priority(jobs, numJobs, preemptive, outputFile);
                        break;
                    case 5:
                        fprintf(outputFile, "Scheduling Method: Round Robin Scheduling – time_quantum=%d\n", timeQuantum);
                        simulateCPU_RoundRobin(jobs, numJobs, timeQuantum, outputFile);
                        break;
                    default:
                        fprintf(outputFile, "Invalid scheduling method. Please choose a valid option.\n");
                        break;
                }

                calculateWaitingTimes(jobs, numJobs, outputFile);
                break;

            case 4:
                fprintf(outputFile, "Program Ended\n");
                break;

            default:
                printf("Invalid option. Please choose a valid option.\n");
                break;
        }

    } while (opt != 4);

    fclose(outputFile);

    free(jobs);

    return 0;
}
