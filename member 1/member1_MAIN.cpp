#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <algorithm>

#include "../common/Graph_222582_222448_222452.h"
#include "../member 1/BellmanFord_222582_222448_222452.h"
#include "../member 1/Dijkstra_222582_222448_222452.h"
#include "../member 1/Diameter_222582_222448_222452.h"

using namespace std;




void dijkstraProblem_demo(DirectedGraph<long long>& g) {
    dijkstra(g); //COMPLETED THIS FUNCTION
}

void bellmanFordProblem_demo(DirectedGraph<long long>& g) {
    bellmanFord(g); //COMPLETED THIS FUNCTION
}

void diameterProblem_demo(DirectedGraph<long long>& g) {
    // run the dijkstra algorithm to get the correct result file:
    dijkstra(g);
    // clear terminal screen:
    system("clear");
    // pause the terminal screen:
    cout << "Dijkstra Algorithm Completed...result file updated." << endl;
    cout << "Press Enter to continue...";
    cin.get();
    calculateDiameter();
}


int main() {
    int choice;

    system("clear");
    cout << "----------------------------------------" << endl;
    cout << "BUILDING GRAPH FROM DATASET...." << endl;
    DirectedGraph<long long> g = BuildDirectedGraphFromCSV();
    cout << "GRAPH BUILT SUCCESSFULLY!" << endl;
    cout << "----------------------------------------" << endl;
    // pause the program for a few seconds
    cout << "Press Enter to continue...";
    cin.get();

    // do {
        system("clear");
        cout << "----------------------------------------" << endl;
        cout << "MEMBER 1 WORK : Affan Hameed" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Choose a Problem to Run:" << endl;
        cout << "1. Dijkstra Problem" << endl;
        cout << "2. Bellman-Ford Problem" << endl;
        cout << "3. Diameter Problem" << endl;
        cout << "4. Exit" << endl;
        cin >> choice;

        switch (choice) {
            case 1:
                dijkstraProblem_demo(g);
                break;
            case 2:
                bellmanFordProblem_demo(g);
                break;
            case 3:
                diameterProblem_demo(g);
                break;
            case 4:
                cout << "Exiting the program..." << endl;
                cout << "----------------------------------------" << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
                // pause the program for a few seconds
                cout << "Press Enter to continue...";
                cin.ignore();
                cin.get();
                break;
        }
    // } while (choice != 4);
    return 0;
}