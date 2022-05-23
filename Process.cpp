#include <iostream>
#include "Process.h"

using namespace std;

void Process::printProcess(Process* process) {
	cout << "ID : " << process->pid << " Burst : " << process->burst << " Quantum : " << process->quantum << "\n";
}
