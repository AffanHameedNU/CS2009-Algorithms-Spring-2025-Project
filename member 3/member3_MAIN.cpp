#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <algorithm>

#include "../common/Graph_222582_222448_222452.h"

#include "../member 3/CycleDetection_222582_222448_222452.h"
#include "../member 3/DFS_222582_222448_222452.h"
#include "../member 3/BFS_222582_222448_222452.h"

using namespace std;


void bfsTraversalProblem_demo(BiDirectedGraph<long long>& g) {
    bfsTraversal(g); //COMPLETED THIS FUNCTION
    
}

void dfsTraversalProblem_demo(BiDirectedGraph<long long>& g) {
    dfsTraversal(g); //COMPLETED THIS FUNCTION
}

void cycleDetectionProblem_demo(BiDirectedGraph<long long>& g, DirectedGraph<long long>& g2) {
    int choice;
    cout << "----------------------------------------" << endl;
    cout << "Choose a Cycle Detection Algorithm:" << endl;
    cout << "1. Cycle Detection in Directed Graph" << endl;
    cout << "2. Cycle Detection in Undirected Graph" << endl;
    cin >> choice;
    switch (choice) {
        case 1:
            cycleDetectionInDirected(g2); //COMPLETED THIS FUNCTION
            break;
        case 2:
            cycleDetectionInUnDirected(g); //COMPLETED THIS FUNCTION
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
    }

}


int main() {
    int choice;
    system("clear");
    cout << "----------------------------------------" << endl;
    cout << "BUILDING GRAPH FROM DATASET...." << endl;

    BiDirectedGraph<long long> g = BuildBiDirectedGraphFromCSV();
    DirectedGraph<long long> g2 = BuildDirectedGraphFromCSV();
    
    cout << "GRAPH BUILT SUCCESSFULLY!" << endl;
    cout << "----------------------------------------" << endl;
    // pause the program for a few seconds
    cout << "Press Enter to continue...";
    cin.get();

    // do {
        system("clear");
        cout << "----------------------------------------" << endl;
        cout << "MEMBER 3 WORK : Shoaib Mehmood" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Choose a Problem to Run:" << endl;
        cout << "1. BFS Traversal Problem" << endl;
        cout << "2. DFS Traversal Problem" << endl;
        cout << "3. Cycle Detection Problem" << endl;
        cout << "4. Exit" << endl;
        cin >> choice;

        switch (choice) {
            case 1:
                bfsTraversalProblem_demo(g);
                break;
            case 2:
                dfsTraversalProblem_demo(g);
                break;
            case 3:
                cycleDetectionProblem_demo(g,g2);
                break;
            case 4:
                cout << "Exiting the program..." << endl;
                cout << "----------------------------------------" << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
                // pause the program for a few seconds
                cout << "Press Enter to continue..." << endl;
                cin.ignore();
                cin.get();
                break;
        }
    // } while (choice != 4);
    return 0;
}