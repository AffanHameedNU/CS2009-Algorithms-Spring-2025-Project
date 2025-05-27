#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <algorithm>

#include "../common/Graph_222582_222448_222452.h"
#include "../member 2/Kruskals_222582_222448_222452.h"
#include "../member 2/Prims_222582_222448_222452.h"
#include "../member 2/AverageDegree_222582_222448_222452.h"

using namespace std;


void primsProblem_demo(BiDirectedGraph<long long>& g) {
    prims_MST(g); //COMPLETED THIS FUNCTION
    
}

void kruskalsProblem_demo(BiDirectedGraph<long long>& g) {
    kruskals_MST(g); //COMPLETED THIS FUNCTION
    
}

void averageDegreeProblem_demo(DirectedGraph<long long>& g) {
    calculateAverageDegree(g); //COMPLETED THIS FUNCTION
}


int main() {
    int choice;

    system("clear");
    cout << "----------------------------------------" << endl;
    cout << "BUILDING GRAPH FROM DATASET...." << endl;

    BiDirectedGraph<long long> g = BuildBiDirectedGraphFromCSV();
    DirectedGraph<long long> g2 = BuildDirectedGraphFromCSV();
    
    cout << "GRAPHS BUILT SUCCESSFULLY!" << endl;
    cout << "----------------------------------------" << endl;
    // pause the program for a few seconds
    cout << "Press Enter to continue...";
    cin.get();

    // do {
        system("clear");
        cout << "----------------------------------------" << endl;
        cout << "MEMBER 2 WORK : Bilal Mehmood" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Choose a Problem to Run:" << endl;
        cout << "1. Prims Problem" << endl;
        cout << "2. Kruskals Problem" << endl;
        cout << "3. Average Degree Problem" << endl;
        cout << "4. Exit" << endl;
        cin >> choice;

        switch (choice) {
            case 1:
                primsProblem_demo(g); 
                break;
            case 2:
                kruskalsProblem_demo(g); 
                break;
            case 3:
                averageDegreeProblem_demo(g2);
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