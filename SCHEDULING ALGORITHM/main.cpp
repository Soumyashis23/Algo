#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority; // For Priority Scheduling
    int waitingTime;
    int turnaroundTime;
};

// Function to calculate waiting time and turnaround time
void calculateTimes(vector<Process>& processes) {
    for (auto& p : processes) {
        p.turnaroundTime = p.waitingTime + p.burstTime;
    }
}
 
// Function to display the result
void displayProcesses(const vector<Process>& processes) {
    cout << left << setw(10) << "Process" << setw(15) << "Arrival Time" << setw(15) << "Burst Time"
         << setw(15) << "Waiting Time" << setw(15) << "Turnaround Time" << "\n";

    for (const auto& p : processes) {
        cout << left << setw(10) << p.id << setw(15) << p.arrivalTime << setw(15) << p.burstTime
             << setw(15) << p.waitingTime << setw(15) << p.turnaroundTime << "\n";
    }
}

// First Come First Serve (FCFS)
void fcfs(vector<Process>& processes) {
    int currentTime = 0;
    for (auto& p : processes) {
        if (currentTime < p.arrivalTime) {
            currentTime = p.arrivalTime;
        }
        p.waitingTime = currentTime - p.arrivalTime;
        currentTime += p.burstTime;
    }
    calculateTimes(processes);
}

// Shortest Job First (SJF) Non-Preemptive
void sjf(vector<Process>& processes) {
    int currentTime = 0;
    vector<bool> visited(processes.size(), false);
    int completed = 0;

    while (completed < processes.size()) {
        int idx = -1;
        for (int i = 0; i < processes.size(); ++i) {
            if (!visited[i] && processes[i].arrivalTime <= currentTime) {
                if (idx == -1 || processes[i].burstTime < processes[idx].burstTime) {
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            visited[idx] = true;
            processes[idx].waitingTime = currentTime - processes[idx].arrivalTime;
            currentTime += processes[idx].burstTime;
            completed++;
        } else {
            currentTime++;
        }
    }
    calculateTimes(processes);
}

// Priority Scheduling (Non-Preemptive)
void priorityScheduling(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.priority < b.priority || (a.priority == b.priority && a.arrivalTime < b.arrivalTime);
    });

    fcfs(processes);
}

// Round Robin
void roundRobin(vector<Process>& processes, int quantum) {
    int currentTime = 0;
    vector<int> remainingBurst(processes.size());
    for (int i = 0; i < processes.size(); ++i) {
        remainingBurst[i] = processes[i].burstTime;
    }

    bool done = false;
    while (!done) {
        done = true;
        for (int i = 0; i < processes.size(); ++i) {
            if (remainingBurst[i] > 0) {
                done = false;
                if (remainingBurst[i] > quantum) {
                    currentTime += quantum;
                    remainingBurst[i] -= quantum;
                } else {
                    currentTime += remainingBurst[i];
                    processes[i].waitingTime = currentTime - processes[i].arrivalTime - processes[i].burstTime;
                    remainingBurst[i] = 0;
                }
            }
        }
    }
    calculateTimes(processes);
}

// Main Function
int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    for (int i = 0; i < n; ++i) {
        cout << "Enter Arrival Time, Burst Time, and Priority for Process " << i + 1 << ":\n";
        cin >> processes[i].arrivalTime >> processes[i].burstTime >> processes[i].priority;
        processes[i].id = i + 1;
    }

    cout << "\nSelect Scheduling Algorithm:\n";
    cout << "1. First Come First Serve (FCFS)\n";
    cout << "2. Shortest Job First (SJF)\n";
    cout << "3. Priority Scheduling\n";
    cout << "4. Round Robin\n";
    int choice;
    cin >> choice;

    vector<Process> processesCopy = processes; // Copy for reuse
    switch (choice) {
        case 1:
            sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
                return a.arrivalTime < b.arrivalTime;
            });
            fcfs(processes);
            break;

        case 2:
            sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
                return a.arrivalTime < b.arrivalTime;
            });
            sjf(processes);
            break;

        case 3:
            priorityScheduling(processes);
            break;

        case 4:
            int quantum;
            cout << "Enter Time Quantum: ";
            cin >> quantum;
            roundRobin(processes, quantum);
            break;

        default:
            cout << "Invalid choice.\n";
            return 0;
    }

    displayProcesses(processes);
    return 0;
}
