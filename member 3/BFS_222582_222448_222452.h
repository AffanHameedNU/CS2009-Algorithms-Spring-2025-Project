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

const string bfs_execTionTime_File = "../member 3/executionTimes/bfs_execTime.txt";
const string bfs_TraceFile = "../member 3/traces/bfs_trace.txt";
const string bfs_ResultFile = "../member 3/results/traversal_bfs.txt";


int getStartingNode(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return -1; // Return -1 to indicate an error
    }

    string line;
    while (getline(file, line)) {
        // Find the first node with connections
        size_t colonPos = line.find(':');
        if (colonPos != string::npos) {
            string nodeStr = line.substr(0, colonPos);
            string connections = line.substr(colonPos + 1);

            // Check if the node has connections
            if (!connections.empty() && connections.find('(') != string::npos) {
                file.close();
                return stoi(nodeStr); // Return the first node with connections
            }
        }
    }

    file.close();
    cerr << "Error: No valid source node found in the file." << endl;
    return -1; // Return -1 if no valid source node is found
}



void bfsTraversal(BiDirectedGraph<long long>& graph) {
    int numVertices = graph.getNumberOfVertices();
    if (numVertices == 0) {
        cerr << "Graph is empty." << endl;
        return;
    }

    // Get the starting node from the dataset
    int startNode = getStartingNode(biDirectedGraphTraceFile);
    if (startNode == -1) {
        cerr << "Error: Could not determine a valid starting node for BFS." << endl;
        return;
    }

    vector<bool> visited(numVertices, false);
    vector<long long> bfsResult;
    queue<long long> q;

    // Open the trace file for logging
    ofstream traceFile(bfs_TraceFile);
    if (!traceFile.is_open()) {
        cerr << "Error: Could not open trace file " << bfs_TraceFile << endl;
        return;
    }

    traceFile << "BFS Traversal Trace" << endl;
    traceFile << "Number of Nodes: " << numVertices << endl;
    traceFile << "Starting Node: " << startNode << endl;
    traceFile << "----------------------------------------" << endl;

    // Start measuring execution time
    auto startTime = chrono::high_resolution_clock::now();

    // Start BFS from the determined starting node
    q.push(startNode);
    visited[startNode] = true;
    traceFile << "Node " << startNode << " inserted into the queue." << endl;

    while (!q.empty()) {
        long long currentVertex = q.front();
        q.pop();
        bfsResult.push_back(currentVertex);
        traceFile << "Node " << currentVertex << " removed from the queue." << endl;

        for (const auto& neighbor : graph.getNeighbors(currentVertex)) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
                traceFile << "Node " << neighbor << " inserted into the queue." << endl;
            }
        }
    }

    // Stop measuring execution time
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    // Write the BFS result to the result file
    ofstream resultFile(bfs_ResultFile);
    if (!resultFile.is_open()) {
        cerr << "Error: Could not open result file " << bfs_ResultFile << endl;
        traceFile.close();
        return;
    }

    resultFile << "BFS Traversal Result:" << endl;
    cout << "BFS Traversal Result:" << endl;
    for (const auto& vertex : bfsResult) {
        resultFile << vertex << " ";
        cout << vertex << " ";
    }
    resultFile << endl;
    cout << endl;
    resultFile.close();

    // Log the result to the trace file
    traceFile << "----------------------------------------" << endl;
    traceFile << "BFS Traversal Completed." << endl;
    traceFile << "Execution Time: " << duration.count() << " milliseconds" << endl;
    traceFile.close();

    // Write the execution time to the execution time file
    ofstream executionTimeFile(bfs_execTionTime_File);
    if (!executionTimeFile.is_open()) {
        cerr << "Error: Could not open execution time file " << bfs_execTionTime_File << endl;
        return;
    }

    executionTimeFile << "Execution Time: " << duration.count() << " milliseconds" << endl;
    executionTimeFile.close();

    // Print the execution time to the console
    cout << "Execution Time: " << duration.count() << " milliseconds" << endl;
    cout << "Results written to " << bfs_ResultFile << endl;
    cout << "Trace written to " << bfs_TraceFile << endl;
}