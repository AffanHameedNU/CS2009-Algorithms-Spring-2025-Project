#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <cstdlib>
#include <stack>
#include <algorithm>
#include <fstream>
#include <chrono>

#include "../common/Graph_222582_222448_222452.h"
#include "../common/commonConstants.h"

using namespace std;

const string dfs_execTionTime_File = "../member 3/executionTimes/dfs_execTime.txt";
const string dfs_TraceFile = "../member 3/traces/dfs_trace.txt";
const string dfs_ResultFile = "../member 3/results/traversal_dfs.txt";



int getStartNode(const string& filename) {
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



void dfsTraversal(BiDirectedGraph<long long>& graph) {
    int numVertices = graph.getNumberOfVertices();
    if (numVertices == 0) {
        cerr << "Graph is empty." << endl;
        return;
    }

    // Get the starting node from the dataset
    int startNode = getStartNode(biDirectedGraphTraceFile); // Update the path to your dataset file
    if (startNode == -1) {
        cerr << "Error: Could not determine a valid starting node for DFS." << endl;
        return;
    }

    vector<bool> visited(numVertices, false);
    vector<long long> dfsResult;
    stack<long long> s;

    // Open the trace file for logging
    ofstream traceFile(dfs_TraceFile);
    if (!traceFile.is_open()) {
        cerr << "Error: Could not open trace file " << dfs_TraceFile << endl;
        return;
    }

    traceFile << "DFS Traversal Trace" << endl;
    traceFile << "Number of Nodes: " << numVertices << endl;
    traceFile << "Starting Node: " << startNode << endl;
    traceFile << "----------------------------------------" << endl;

    // Start measuring execution time
    auto startTime = chrono::high_resolution_clock::now();

    // Start DFS from the determined starting node
    s.push(startNode);
    traceFile << "Node " << startNode << " pushed onto the stack." << endl;

    while (!s.empty()) {
        long long currentVertex = s.top();
        s.pop();
        traceFile << "Node " << currentVertex << " popped from the stack." << endl;

        if (!visited[currentVertex]) {
            visited[currentVertex] = true;
            dfsResult.push_back(currentVertex);
            traceFile << "Node " << currentVertex << " marked as visited." << endl;

            // Push all unvisited neighbors onto the stack
            for (const auto& neighbor : graph.getNeighbors(currentVertex)) {
                if (!visited[neighbor]) {
                    s.push(neighbor);
                    traceFile << "Node " << neighbor << " pushed onto the stack." << endl;
                }
            }
        }
    }

    // Stop measuring execution time
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    // Write the DFS result to the result file
    ofstream resultFile(dfs_ResultFile);
    if (!resultFile.is_open()) {
        cerr << "Error: Could not open result file " << dfs_ResultFile << endl;
        traceFile.close();
        return;
    }

    resultFile << "DFS Traversal Result:" << endl;
    cout << "DFS Traversal Result:" << endl;
    for (const auto& vertex : dfsResult) {
        resultFile << vertex << " ";
        cout << vertex << " ";
    }
    resultFile << endl;
    cout << endl;
    resultFile.close();

    // Log the result to the trace file
    traceFile << "----------------------------------------" << endl;
    traceFile << "DFS Traversal Completed." << endl;
    traceFile << "Execution Time: " << duration.count() << " milliseconds" << endl;
    traceFile.close();

    // Write the execution time to the execution time file
    ofstream executionTimeFile(dfs_execTionTime_File);
    if (!executionTimeFile.is_open()) {
        cerr << "Error: Could not open execution time file " << dfs_execTionTime_File << endl;
        return;
    }

    executionTimeFile << "Execution Time: " << duration.count() << " milliseconds" << endl;
    executionTimeFile.close();

    // Print the execution time to the console
    cout << "Execution Time: " << duration.count() << " milliseconds" << endl;
    cout << "Results written to " << dfs_ResultFile << endl;
    cout << "Trace written to " << dfs_TraceFile << endl;
}

