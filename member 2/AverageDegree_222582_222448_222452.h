#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <chrono>

#include "../common/Graph_222582_222448_222452.h"
#include "../common/commonConstants.h"

using namespace std;

const string averageDeagree_execTionTime_File = "../member 2/executionTimes/averageDegree_execTime.txt";
const string averageDegree_TraceFile = "../member 2/traces/averageDegree_trace.txt";
const string averageDegree_ResultFile = "../member 2/results/average_degree.txt";

void calculateAverageDegree(DirectedGraph<long long>& graph) {
    int nodes = graph.getNumberOfVertices(); //the number of nodes from dataset are correctly identified.
    if (nodes == 0) {
        cerr << "Graph is empty." << endl;
        return;
    }

    // Open the trace file for logging
    ofstream traceFile(averageDegree_TraceFile);
    if (!traceFile.is_open()) {
        cerr << "Error: Could not open trace file " << averageDegree_TraceFile << endl;
        return;
    }

    traceFile << "Average Degree Calculation Trace" << endl;
    traceFile << "Number of Nodes: " << nodes << endl;
    traceFile << "----------------------------------------" << endl;

    // Start measuring execution time
    auto startTime = chrono::high_resolution_clock::now();

    const auto& adjList = graph.getAdjList();
    long long totalDegree = 0;

    // Degree of each node is the size of its adjacency list
    for (int i = 0; i < adjList.size(); i++) {
        long long degree = adjList[i].size();
        totalDegree += degree;

        traceFile << "Node " << i << " Degree: " << degree << endl;
    }

    double avgDegree = static_cast<double>(totalDegree) / nodes;

    // Stop measuring execution time
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    // Write the average degree to the result file
    ofstream resultFile(averageDegree_ResultFile);
    if (!resultFile.is_open()) {
        cerr << "Error: Could not open result file " << averageDegree_ResultFile << endl;
        traceFile.close();
        return;
    }

    resultFile << "Average Degree of the Graph: " << avgDegree << endl;
    resultFile.close();

    // Log the result to the trace file
    traceFile << "----------------------------------------" << endl;
    traceFile << "Total Degree: " << totalDegree << endl;
    traceFile << "Average Degree: " << avgDegree << endl;
    traceFile << "Execution Time: " << duration.count() << " milliseconds" << endl;
    traceFile.close();

    // Write the execution time to the execution time file
    ofstream executionTimeFile(averageDeagree_execTionTime_File);
    if (!executionTimeFile.is_open()) {
        cerr << "Error: Could not open execution time file " << averageDeagree_execTionTime_File << endl;
        return;
    }

    executionTimeFile << "Execution Time: " << duration.count() << " milliseconds" << endl;
    executionTimeFile.close();

    // Print the average degree and execution time to the console
    cout << "Total Degree: " << totalDegree << endl;
    cout << "Number of Nodes: " << nodes << endl;
    cout << "Average Degree of the Graph: " << avgDegree << endl;
    cout << "Execution Time: " << duration.count() << " milliseconds" << endl;
    cout << "Results written to " << averageDegree_ResultFile << endl;
    cout << "Trace written to " << averageDegree_TraceFile << endl;
}

