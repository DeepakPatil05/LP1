#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono_literals;

class Bully
{
    int n;
    vector<int> processes;
    int coordinator;

public:
    void input()
    {
        cout << "\n=== Bully Algorithm ===\n";
        cout << "Enter number of processes: ";
        cin >> n;

        processes.resize(n);
        cout << "Enter process IDs: ";
        for (int i = 0; i < n; i++)
            cin >> processes[i];

        coordinator = *max_element(processes.begin(), processes.end());
        cout << "Initial Coordinator is Process " << coordinator << endl;
    }

    void startElection(int initiator)
    {
        cout << "\n⚠️  Process " << initiator << " detected coordinator failure!\n";
        cout << "Election initiated by Process " << initiator << "...\n";
        this_thread::sleep_for(1s);

        vector<int> higher;
        for (int p : processes)
            if (p > initiator)
                higher.push_back(p);

        if (higher.empty())
        {
            coordinator = initiator;
            cout << "No higher process found.\n✅ Process " << initiator << " becomes the new Coordinator.\n";
            return;
        }

        cout << "Process " << initiator << " sends ELECTION to: ";
        for (int h : higher)
            cout << h << " ";
        cout << endl;
        this_thread::sleep_for(1s);

        cout << "Processes ";
        for (int h : higher)
            cout << h << " ";
        cout << "respond with OK.\n";
        this_thread::sleep_for(1s);

        coordinator = *max_element(higher.begin(), higher.end());
        cout << "\n🏆 Process " << coordinator << " becomes the new Coordinator.\n";
    }

    void failCoordinator()
    {
        cout << "\nEnter the failed coordinator ID: ";
        int failed;
        cin >> failed;

        if (failed != coordinator)
        {
            cout << "Error: Process " << failed << " is not the current coordinator!\n";
            return;
        }

        cout << "💀 Coordinator Process " << coordinator << " has failed.\n";
        processes.erase(remove(processes.begin(), processes.end(), failed), processes.end());

        cout << "Enter process ID that will start election: ";
        int initiator;
        cin >> initiator;

        if (find(processes.begin(), processes.end(), initiator) == processes.end())
        {
            cout << "Invalid initiator process ID!\n";
            return;
        }

        startElection(initiator);
    }
};

class Ring
{
    int n;
    vector<int> processes;
    int coordinator;

public:
    void input()
    {
        cout << "\n=== Ring Algorithm ===\n";
        cout << "Enter number of processes: ";
        cin >> n;

        processes.resize(n);
        cout << "Enter process IDs in ring order: ";
        for (int i = 0; i < n; i++)
            cin >> processes[i];

        coordinator = *max_element(processes.begin(), processes.end());
        cout << "Initial Coordinator is Process " << coordinator << endl;
    }

    void startElection(int initiator)
    {
        cout << "\n⚠️  Process " << initiator << " detected coordinator failure!\n";
        cout << "Election initiated by Process " << initiator << "...\n";
        this_thread::sleep_for(1s);

        vector<int> electionList;
        int startIndex = find(processes.begin(), processes.end(), initiator) - processes.begin();
        int index = (startIndex + 1) % n;

        electionList.push_back(initiator);

        cout << "🔁 Passing ELECTION message around the ring:\n";
        while (processes[index] != initiator)
        {
            this_thread::sleep_for(800ms);
            cout << "   " << electionList.back() << " → " << processes[index] << endl;
            electionList.push_back(processes[index]);
            index = (index + 1) % n;
        }

        this_thread::sleep_for(1s);
        cout << "\nAll processes participated in election: ";
        for (int id : electionList)
            cout << id << " ";
        cout << endl;

        coordinator = *max_element(electionList.begin(), electionList.end());
        cout << "\n🏆 Process " << coordinator << " becomes the new Coordinator.\n";
        this_thread::sleep_for(1s);

        cout << "🔁 Circulating COORDINATOR message: ";
        for (int id : electionList)
        {
            cout << id << " ";
            this_thread::sleep_for(300ms);
        }
        cout << "\nCOORDINATOR (" << coordinator << ") message delivered to all.\n";
    }

    void failCoordinator()
    {
        cout << "\nEnter the failed coordinator ID: ";
        int failed;
        cin >> failed;

        if (failed != coordinator)
        {
            cout << "Error: Process " << failed << " is not the current coordinator!\n";
            return;
        }

        cout << "💀 Coordinator Process " << coordinator << " has failed.\n";
        processes.erase(remove(processes.begin(), processes.end(), failed), processes.end());
        n = processes.size();

        cout << "Enter process ID that will start election: ";
        int initiator;
        cin >> initiator;

        if (find(processes.begin(), processes.end(), initiator) == processes.end())
        {
            cout << "Invalid initiator process ID!\n";
            return;
        }

        startElection(initiator);
    }
};

int main()
{
    while (true)
    {
        cout << "\n==============================";
        cout << "\n  Leader Election Simulation";
        cout << "\n==============================";
        cout << "\n1. Bully Algorithm";
        cout << "\n2. Ring Algorithm";
        cout << "\n3. Exit";
        cout << "\nEnter your choice: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            Bully b;
            b.input();
            b.failCoordinator();
            break;
        }
        case 2:
        {
            Ring r;
            r.input();
            r.failCoordinator();
            break;
        }
        case 3:
            cout << "\n👋 Exiting program. Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice! Please try again.\n";
        }
    }
}
