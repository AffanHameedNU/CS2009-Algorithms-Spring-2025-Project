#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <fstream>
#include <chrono>


#include "../common/Graph_222582_222448_222452.h"
#include "../common/commonConstants.h"

using namespace std;

const string prims_execTionTime_File = "../member 2/executionTimes/prims_execTime.txt";
const string prims_TraceFile = "../member 2/traces/prims_trace.txt";
const string prims_ResultFile = "../member 2/results/mst_prims.txt";



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


void prims_MST(BiDirectedGraph<long long>& graph) {
    int nodes = graph.getNumberOfVertices();
    int edges = graph.getNumberOfEdges();

    // Open the trace file for logging
    ofstream traceFile(prims_TraceFile);
    if (!traceFile.is_open()) {
        cerr << "Error: Could not open trace file " << prims_TraceFile << endl;
        return;
    }

    traceFile << "Prim's Algorithm Trace" << endl;
    traceFile << "Number of Nodes: " << nodes << endl;
    traceFile << "Number of Edges: " << edges << endl;
    traceFile << "----------------------------------------" << endl;

    // Start measuring execution time
    auto startTime = chrono::high_resolution_clock::now();

    // Adjacency list
    vector<vector<pair<long long, long long>>> adjList(nodes);
    for (int i = 0; i < nodes; i++) {
        vector<long long> neighbors = graph.getNeighbors(i);
        for (long long neighbor : neighbors) {
            long long weight = graph.getWeight(i, neighbor);
            adjList[i].push_back({neighbor, weight});
        }
    }

    // Key, inMST, and parent arrays
    vector<long long> key(nodes, numeric_limits<long long>::max());
    vector<bool> inMST(nodes, false);
    vector<long long> parent(nodes, -1);

    // Get the starting node
    int src = getStartingNode(biDirectedGraphTraceFile);
    if (src == -1) {
        cerr << "Error: No valid source node found." << endl;
        traceFile << "Error: No valid source node found." << endl;
        traceFile.close();
        return;
    }

    key[src] = 0;
    parent[src] = -1;

    for (int i = 0; i < nodes - 1; i++) {
        long long minKey = numeric_limits<long long>::max();
        long long minIndex = -1;

        // Find the minimum key
        for (int j = 0; j < nodes; j++) {
            if (!inMST[j] && key[j] < minKey) {
                minKey = key[j];
                minIndex = j;
            }
        }

        // Check if a valid node was found
        if (minIndex == -1) {
            traceFile << "Error: No valid node found to include in MST. Continuing with partial MST." << endl;
            cerr << "Error: No valid node found to include in MST. Continuing with partial MST." << endl;
            break; // Exit the loop but continue to construct the MST with processed nodes
        }

        // Mark the selected node as true
        inMST[minIndex] = true;

        traceFile << "Selected Node: " << minIndex << " with Key: " << minKey << endl;

        // Update the key value and parent index of the adjacent nodes
        for (const auto& neighbor : adjList[minIndex]) {
            long long v = neighbor.first;
            long long weight = neighbor.second;

            if (!inMST[v] && weight < key[v]) {
                parent[v] = minIndex;
                key[v] = weight;

                traceFile << "  Updated Node: " << v << ", Parent: " << minIndex << ", Key: " << weight << endl;
            }
        }
    }

    // Stop measuring execution time
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    // Write the MST to the result file
    ofstream resultFile(prims_ResultFile);
    if (!resultFile.is_open()) {
        cerr << "Error: Could not open result file " << prims_ResultFile << endl;
        traceFile.close();
        return;
    }

    resultFile << "Minimum Spanning Tree (MST) using Prim's Algorithm:" << endl;
    cout << "Minimum Spanning Tree (MST) using Prim's Algorithm:" << endl;

    long long totalWeight = 0;
    for (int i = 1; i < nodes; i++) {
        if (parent[i] != -1) {
            resultFile << "Edge: " << parent[i] << " - " << i << ", Weight: " << key[i] << endl;
            cout << "Edge: " << parent[i] << " - " << i << ", Weight: " << key[i] << endl;
            totalWeight += key[i];
        }
    }

    resultFile << "Total Weight of MST: " << totalWeight << endl;
    cout << "Total Weight of MST: " << totalWeight << endl;
    resultFile.close();

    // Log the execution time to the trace file
    traceFile << "----------------------------------------" << endl;
    traceFile << "Total Weight of MST: " << totalWeight << endl;
    traceFile << "Execution Time: " << duration.count() << " milliseconds" << endl;
    traceFile.close();

    // Write the execution time to the execution time file
    ofstream executionTimeFile(prims_execTionTime_File);
    if (!executionTimeFile.is_open()) {
        cerr << "Error: Could not open execution time file " << prims_execTionTime_File << endl;
        return;
    }

    executionTimeFile << "Execution Time: " << duration.count() << " milliseconds" << endl;
    executionTimeFile.close();

    // Print the execution time to the console
    cout << "Execution Time: " << duration.count() << " milliseconds" << endl;
    cout << "Results written to " << prims_ResultFile << endl;
    cout << "Trace written to " << prims_TraceFile << endl;
}
