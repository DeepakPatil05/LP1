#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void show(const vector<int>& procs, int coord) {
    cout << "Processes: ";
    for (int p: procs) cout << p << ' ';
    cout << "\nCoordinator: " << (coord == -1 ? string("None") : to_string(coord)) << "\n";
}

int findIndex(const vector<int>& v, int x){
    for (size_t i=0;i<v.size();++i) if (v[i]==x) return (int)i;
    return -1;
}

int ringElection(vector<int>& procs, int initiator) {
    if (procs.empty()) { cout << "No alive processes.\n"; return -1; }
    int idx = findIndex(procs, initiator);
    if (idx == -1) { cout << "Initiator " << initiator << " not found, using first process.\n"; idx = 0; initiator = procs[0]; }
    cout << "\nRing election started by " << initiator << '\n';
    vector<int> seen;
    int n = (int)procs.size();
    int cur = idx;
    do {
        cout << "Process " << procs[cur] << " receives and forwards the election message.\n";
        seen.push_back(procs[cur]);
        cur = (cur + 1) % n;
    } while (cur != idx);
    int winner = *max_element(seen.begin(), seen.end());
    cout << "Ring election complete. Elected coordinator: " << winner << "\n\n";
    return winner;
}

int bullyElection(const vector<int>& procs, int initiator) {
    if (procs.empty()) { cout << "No alive processes.\n"; return -1; }
    if (findIndex(procs, initiator) == -1) {
        cout << "Initiator " << initiator << " not found, using highest process to start.\n";
        return *max_element(procs.begin(), procs.end());
    }
    cout << "\nBully election started by " << initiator << '\n';
    bool higherResponded = false;
    for (int p : procs) {
        if (p > initiator) {
            cout << "  Sent ELECTION to " << p << " -> (responds)\n";
            higherResponded = true;
        }
    }
    if (!higherResponded) {
        cout << "No higher process responded. " << initiator << " becomes coordinator.\n\n";
        return initiator;
    }
    int winner = *max_element(procs.begin(), procs.end());
    cout << "Higher process will take over. Final coordinator: " << winner << "\n\n";
    return winner;
}

int main(){
    vector<int> defaultProcs = {1,2,3,4,5};
    vector<int> procs = defaultProcs;
    int coordinator = procs.empty() ? -1 : *max_element(procs.begin(), procs.end());

    while (true) {
        cout << "\nMenu:\n1) Show\n2) Fail process\n3) Start election\n4) Reset\n0) Exit\nChoice: ";
        int ch; if (!(cin >> ch)) { cin.clear(); cin.ignore(10000,'\n'); continue; }

        if (ch == 0) break;
        if (ch == 1) { show(procs, coordinator); continue; }

        if (ch == 2) {
            cout << "Enter ID to fail: "; int id; cin >> id;
            int i = findIndex(procs, id);
            if (i == -1) { cout << "Process not alive.\n"; }
            else {
                procs.erase(procs.begin() + i);
                cout << "Process " << id << " failed.\n";
                if (id == coordinator) {
                    cout << "Coordinator failed. No coordinator until election.\n";
                    coordinator = -1;
                }
            }
            continue;
        }

        if (ch == 3) {
            cout << "Enter starter ID: "; int starter; cin >> starter;
            cout << "Choose algorithm: 1) Ring  2) Bully  : "; int alg; cin >> alg;
            if (alg == 1) coordinator = ringElection(procs, starter);
            else if (alg == 2) coordinator = bullyElection(procs, starter);
            else cout << "Invalid algorithm choice.\n";
            continue;
        }

        if (ch == 4) {
            procs = defaultProcs;
            coordinator = *max_element(procs.begin(), procs.end());
            cout << "Reset to default processes.\n";
            continue;
        }

        cout << "Invalid choice.\n";
    }

    cout << "Exiting.\n";
    return 0;
}
