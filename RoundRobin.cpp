#include <queue>
#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>
#include "Process.h"

using namespace std;
using namespace this_thread; // this_thread::sleep_for
using namespace chrono; // chrono::milliseconds(100)

#define NUM_PROCESSES 10
#define TIME_SLICE 10
int arrivalTimes[] = { 0, 3, 6, 10, 11, 15, 19, 22, 25, 90 };
int burstTimes[] = { 8, 4, 5, 3, 10, 9, 10, 18, 11, 5 };
int turnaroundTimes[NUM_PROCESSES];

void roundRobin() {
    int completed = 0;
    int timeStep = 0;
    int i = 0;

    Process* running = new Process();
    running->pid = -1;
    running->burst = -1;
    running->quantum = TIME_SLICE;
    running->finished = false;

    queue<Process*> readyQueue;

    while (completed < NUM_PROCESSES) {
        cout << "T = " << timeStep << "\n";
        //running->printProcess(running);
        if (running->burst >= 1) {
            running->burst--;
        }
        if (running->quantum >= 1) {
            running->quantum--; // Decrement the quantum
        }
        if (running->burst == 0 && !running->finished) { // If process burst time is up and has not yet been marked as finished
            cout << "Completed Process-" << running->pid << " at Time Step-" << timeStep << "\n";
            turnaroundTimes[running->pid] = timeStep - arrivalTimes[running->pid]; // Turnaround Time = Timestep - Arrival Time
            completed = running->pid;
            if (running->pid == (NUM_PROCESSES - 1)) {
                completed = NUM_PROCESSES;
            }
            running->finished = true;
        }
        while (i < NUM_PROCESSES && arrivalTimes[i] <= timeStep) { // While there are still processes to push to the queue
            cout << "New Process-" << i << " at Time Step-" << timeStep << "\n";
            Process* newProcess = new Process();
            newProcess->pid = i;
            newProcess->burst = burstTimes[i];
            newProcess->quantum = TIME_SLICE;
            newProcess->finished = false;
            readyQueue.push(newProcess);
            i++;
        }
        if (!readyQueue.empty()) {
            if (running->burst > 0 && running->quantum == 0) { // If running process still has burst time but its time quantum is up
                running->quantum = TIME_SLICE; // Set quantum back to ten
                readyQueue.push(running); // Push this one back to the queue so it can finish processing
                running = readyQueue.front(); // Set the next process in queue to running
                readyQueue.pop(); // Take running process out of the queue
            }
            if (running->burst <= 0 && running->quantum == 0) { // If running process has no more burst time and its time quantum is up
                running->quantum = TIME_SLICE; // Set quantum back to ten
                running = readyQueue.front(); // Set the next process in queue to running
                readyQueue.pop(); // Take running process out of the queue
            }
            if (running->burst <= 0 && running->quantum > 0) { // If running process has no more burst time but its time quantum has not expired
                running->quantum = TIME_SLICE; // Set quantum back to ten
                running = readyQueue.front(); // Set the next process in queue to running
                readyQueue.pop(); // Take running process out of the queue
            }
        }
        sleep_for(milliseconds(100)); // Sleep for 100 milliseconds, used this_thread:: and chrono:: namespaces
        timeStep++; // Increment the time step
    }
}

int main(int argc, const char* argv[]) {
    roundRobin();
    double averageWaitingTime = 0;
    int waitingTime = 0;
    for (int i = 0; i < NUM_PROCESSES; i++) {
        waitingTime = abs(turnaroundTimes[i] - burstTimes[i]); // Waiting Time = Turnaround Time - Burst 
        averageWaitingTime += waitingTime;
        cout << "Waiting Time for Process-" << i << " : " << waitingTime << "\n";
    }
    averageWaitingTime /= NUM_PROCESSES;
    cout << "Average = " << averageWaitingTime << "\n";
    return EXIT_SUCCESS;
}