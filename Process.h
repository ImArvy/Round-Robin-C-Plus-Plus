#ifndef PROCESS_H
#define PROCESS_H

class Process {
public:
    int pid;
    int burst;
    int quantum;
    bool finished;
    void printProcess(Process*);
};

#endif // PROCESS_H
