#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <cstdlib>
#include <algorithm>
#include <functional>
#include <fstream>
#include <chrono>
#include <set>

#include "../common/Graph_222582_222448_222452.h"
#include "../common/commonConstants.h"

using namespace std;

const string cycleDetection_execTionTime_File = "../member 3/executionTimes/cycleDetection_execTime.txt";
const string cycleDetection_TraceFile = "../member 3/traces/cycle_detection_trace.txt";
const string cycleDetection_ResultFile = "../member 3/results/cycle_detection_result.txt";


int getSoruceNode(const string& filename) {
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



void cycleDetectionInDirected(DirectedGraph<long long>& graph) {
    int numVertices = graph.getNumberOfVertices();
    if (numVertices == 0) {
        cerr << "Graph is empty." << endl;
        return;
    }

    vector<bool> visited(numVertices, false);
    vector<bool> recursionStack(numVertices, false);
    vector<vector<long long>> allCycles; // To store all detected cycles
    vector<long long> cyclePath;

    // Open the trace file for logging
    ofstream traceFile(cycleDetection_TraceFile);
    if (!traceFile.is_open()) {
        cerr << "Error: Could not open trace file " << cycleDetection_TraceFile << endl;
        return;
    }

    traceFile << "Cycle Detection in Directed Graph Trace" << endl;
    traceFile << "Number of Nodes: " << numVertices << endl;
    traceFile << "----------------------------------------" << endl;

    // Start measuring execution time
    auto startTime = chrono::high_resolution_clock::now();

    // Helper function for DFS-based cycle detection
    function<bool(int)> dfs = [&](int node) {
        visited[node] = true;
        recursionStack[node] = true;
        cyclePath.push_back(node);
        traceFile << "Node " << node << " visited and added to recursion stack." << endl;

        for (const auto& neighbor : graph.getNeighbors(node)) {
            if (!visited[neighbor]) {
                traceFile << "Exploring neighbor " << neighbor << " of node " << node << "." << endl;
                if (dfs(neighbor)) {
                    return true;
                }
            } else if (recursionStack[neighbor]) {
                traceFile << "Cycle detected! Node " << neighbor << " is already in the recursion stack." << endl;

                // Extract the cycle
                auto it = find(cyclePath.begin(), cyclePath.end(), neighbor);
                vector<long long> cycle(it, cyclePath.end());
                cycle.push_back(neighbor); // Complete the cycle

                // Add the cycle to the list of all cycles
                allCycles.push_back(cycle);
            }
        }

        recursionStack[node] = false;
        traceFile << "Node " << node << " removed from recursion stack." << endl;
        cyclePath.pop_back();
        return false;
    };

    // Perform DFS from each unvisited node
    for (int i = 0; i < numVertices; i++) {
        if (!visited[i]) {
            traceFile << "Starting DFS from node " << i << "." << endl;
            dfs(i);
        }
    }

    // Stop measuring execution time
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    // Write the result to the result file
    ofstream resultFile(cycleDetection_ResultFile);
    if (!resultFile.is_open()) {
        cerr << "Error: Could not open result file " << cycleDetection_ResultFile << endl;
        traceFile.close();
        return;
    }

    if (!allCycles.empty()) {
        resultFile << "Cycles Detected in the Directed Graph:" << endl;
        cout << "Cycles Detected in the Directed Graph:" << endl;

        for (const auto& cycle : allCycles) {
            for (const auto& node : cycle) {
                resultFile << node << " ";
                cout << node << " ";
            }
            resultFile << endl;
            cout << endl;
        }
    } else {
        resultFile << "No Cycles Detected in the Directed Graph." << endl;
        cout << "No Cycles Detected in the Directed Graph." << endl;
    }

    resultFile.close();

    // Log the result to the trace file
    traceFile << "----------------------------------------" << endl;
    if (!allCycles.empty()) {
        traceFile << "Cycles Detected in the Directed Graph." << endl;
    } else {
        traceFile << "No Cycles Detected in the Directed Graph." << endl;
    }
    traceFile << "Execution Time: " << duration.count() << " milliseconds" << endl;
    traceFile.close();

    // Write the execution time to the execution time file
    ofstream executionTimeFile(cycleDetection_execTionTime_File);
    if (!executionTimeFile.is_open()) {
        cerr << "Error: Could not open execution time file " << cycleDetection_execTionTime_File << endl;
        return;
    }

    executionTimeFile << "Execution Time: " << duration.count() << " milliseconds" << endl;
    executionTimeFile.close();

    // Print the execution time to the console
    cout << "Execution Time: " << duration.count() << " milliseconds" << endl;
    cout << "Results written to " << cycleDetection_ResultFile << endl;
    cout << "Trace written to " << cycleDetection_TraceFile << endl;
}


void cycleDetectionInUnDirected(BiDirectedGraph<long long>& graph) {
    int numVertices = graph.getNumberOfVertices();
    if (numVertices == 0) {
        cerr << "Graph is empty." << endl;
        return;
    }

    vector<bool> visited(numVertices, false);
    vector<vector<long long>> allCycles; // To store all detected cycles
    set<vector<long long>> uniqueCycles; // To ensure cycles are unique
    bool cycleFound = false;

    // Open the trace file for logging
    ofstream traceFile(cycleDetection_TraceFile);
    if (!traceFile.is_open()) {
        cerr << "Error: Could not open trace file " << cycleDetection_TraceFile << endl;
        return;
    }

    traceFile << "Cycle Detection in Undirected Graph Trace" << endl;
    traceFile << "Number of Nodes: " << numVertices << endl;
    traceFile << "----------------------------------------" << endl;

    // Start measuring execution time
    auto startTime = chrono::high_resolution_clock::now();

    // Helper function for BFS-based cycle detection
    auto bfs = [&](int startNode) {
        queue<pair<int, int>> q; // Pair of (current node, parent node)
        q.push({startNode, -1});
        visited[startNode] = true;
        traceFile << "Starting BFS from node " << startNode << "." << endl;

        while (!q.empty()) {
            int currentNode = q.front().first;
            int parentNode = q.front().second;
            q.pop();
            traceFile << "Node " << currentNode << " dequeued. Parent: " << parentNode << endl;

            for (const auto& neighbor : graph.getNeighbors(currentNode)) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push({neighbor, currentNode});
                    traceFile << "Node " << neighbor << " enqueued. Parent: " << currentNode << endl;
                } else if (neighbor != parentNode) {
                    // Cycle detected
                    traceFile << "Cycle detected! Node " << neighbor << " revisited from node " << currentNode << "." << endl;

                    // Extract the cycle
                    vector<long long> cycle = {currentNode, neighbor};
                    sort(cycle.begin(), cycle.end()); // Ensure consistent order
                    if (uniqueCycles.find(cycle) == uniqueCycles.end()) {
                        uniqueCycles.insert(cycle);
                        allCycles.push_back(cycle);
                        cycleFound = true;
                    }
                }
            }
        }
    };

    // Perform BFS from each unvisited node
    for (int i = 0; i < numVertices; i++) {
        if (!visited[i]) {
            bfs(i);
        }
    }

    // Stop measuring execution time
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    // Write the result to the result file
    ofstream resultFile(cycleDetection_ResultFile);
    if (!resultFile.is_open()) {
        cerr << "Error: Could not open result file " << cycleDetection_ResultFile << endl;
        traceFile.close();
        return;
    }

    if (cycleFound) {
        resultFile << "Cycles Detected in the Undirected Graph:" << endl;
        cout << "Cycles Detected in the Undirected Graph:" << endl;

        for (const auto& cycle : allCycles) {
            for (const auto& node : cycle) {
                resultFile << node << " ";
                cout << node << " ";
            }
            resultFile << endl;
            cout << endl;
        }
    } else {
        resultFile << "No Cycles Detected in the Undirected Graph." << endl;
        cout << "No Cycles Detected in the Undirected Graph." << endl;
    }

    resultFile.close();

    // Log the result to the trace file
    traceFile << "----------------------------------------" << endl;
    if (cycleFound) {
        traceFile << "Cycles Detected in the Undirected Graph." << endl;
    } else {
        traceFile << "No Cycles Detected in the Undirected Graph." << endl;
    }
    traceFile << "Execution Time: " << duration.count() << " milliseconds" << endl;
    traceFile.close();

    // Write the execution time to the execution time file
    ofstream executionTimeFile(cycleDetection_execTionTime_File);
    if (!executionTimeFile.is_open()) {
        cerr << "Error: Could not open execution time file " << cycleDetection_execTionTime_File << endl;
        return;
    }

    executionTimeFile << "Execution Time: " << duration.count() << " milliseconds" << endl;
    executionTimeFile.close();

    // Print the execution time to the console
    cout << "Execution Time: " << duration.count() << " milliseconds" << endl;
    cout << "Results written to " << cycleDetection_ResultFile << endl;
    cout << "Trace written to " << cycleDetection_TraceFile << endl;
}






