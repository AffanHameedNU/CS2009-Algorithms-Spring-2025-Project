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

const string kruskals_execTionTime_File = "../member 2/executionTimes/kruskals_execTime.txt";
const string kruskals_TraceFile = "../member 2/traces/kruskals_trace.txt";
const string kruskals_ResultFile = "../member 2/results/mst_kruskals.txt";


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


void makeSet(vector<long long>& parent, vector<long long>& rank, int n) {
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank[i] = 0;
    }
}

long long findParent(vector<long long>& parent, long long& node) {
    if (parent[node] == node) {
        return node;
    }
    return parent[node] = findParent(parent, parent[node]); // Path compression logic added here
}

void unionSet(long long u, long long v, vector<long long>& parent, vector<long long>& rank) {
    u = findParent(parent, u);
    v = findParent(parent, v);

    //we need to compare the rank of u and v
    if (rank[u] < rank[v]) {
        parent[u] = v;
    } else if (rank[u] > rank[v]) {
        parent[v] = u;
    } else {
        parent[v] = u;
        rank[u]++;
    } 
}

bool cmp(vector<long long>& a, vector<long long>& b) {
    return a[2] < b[2]; // Compare based on the weight (3rd element)
}

void kruskals_MST(BiDirectedGraph<long long>& graph) {
    int nodes = graph.getNumberOfVertices();
    int edges = graph.getNumberOfEdges();

    // Open the trace file for logging
    ofstream traceFile(kruskals_TraceFile);
    if (!traceFile.is_open()) {
        cerr << "Error: Could not open trace file " << kruskals_TraceFile << endl;
        return;
    }

    traceFile << "Kruskal's Algorithm Trace" << endl;
    traceFile << "Number of Nodes: " << nodes << endl;
    traceFile << "Number of Edges: " << edges << endl;
    traceFile << "----------------------------------------" << endl;

    // Start measuring execution time
    auto startTime = chrono::high_resolution_clock::now();

    // Vector to store the edges
    vector<tuple<long long, long long, long long>> edgesList;
    for (int i = 0; i < nodes; i++) {
        vector<long long> neighbors = graph.getNeighbors(i);
        for (long long neighbor : neighbors) {
            if (i < neighbor) { // To avoid duplicate edges
                long long weight = graph.getWeight(i, neighbor);
                edgesList.emplace_back(weight, i, neighbor);
            }
        }
    }

    // Sort the edgesList based on weight
    sort(edgesList.begin(), edgesList.end());

    // Parent and rank arrays for union-find
    vector<long long> parent(nodes);
    vector<long long> rank(nodes, 0);

    // Initialize the parent and rank arrays
    makeSet(parent, rank, nodes);

    long long minWeight = 0;
    vector<tuple<long long, long long, long long>> mstEdges; // To store the MST edges

    for (const auto& edge : edgesList) {
        long long weight = get<0>(edge);
        long long u = get<1>(edge);
        long long v = get<2>(edge);

        // Check if u and v are in the same set
        if (findParent(parent, u) != findParent(parent, v)) {
            minWeight += weight;
            unionSet(u, v, parent, rank);
            mstEdges.emplace_back(weight, u, v);

            traceFile << "Edge Added: " << u << " - " << v << " with Weight: " << weight << endl;
        }
    }

    // Stop measuring execution time
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    // Write the MST to the result file
    ofstream resultFile(kruskals_ResultFile);
    if (!resultFile.is_open()) {
        cerr << "Error: Could not open result file " << kruskals_ResultFile << endl;
        traceFile.close();
        return;
    }

    resultFile << "Minimum Spanning Tree (MST) using Kruskal's Algorithm:" << endl;
    cout << "Minimum Spanning Tree (MST) using Kruskal's Algorithm:" << endl;

    for (const auto& edge : mstEdges) {
        long long weight = get<0>(edge);
        long long u = get<1>(edge);
        long long v = get<2>(edge);

        resultFile << "Edge: " << u << " - " << v << ", Weight: " << weight << endl;
        cout << "Edge: " << u << " - " << v << ", Weight: " << weight << endl;
    }

    resultFile << "Total Weight of MST: " << minWeight << endl;
    cout << "Total Weight of MST: " << minWeight << endl;
    resultFile.close();

    // Log the execution time to the trace file
    traceFile << "----------------------------------------" << endl;
    traceFile << "Total Weight of MST: " << minWeight << endl;
    traceFile << "Execution Time: " << duration.count() << " milliseconds" << endl;
    traceFile.close();

    // Write the execution time to the execution time file
    ofstream executionTimeFile(kruskals_execTionTime_File);
    if (!executionTimeFile.is_open()) {
        cerr << "Error: Could not open execution time file " << kruskals_execTionTime_File << endl;
        return;
    }

    executionTimeFile << "Execution Time: " << duration.count() << " milliseconds" << endl;
    executionTimeFile.close();

    // Print the execution time to the console
    cout << "Execution Time: " << duration.count() << " milliseconds" << endl;
    cout << "Results written to " << kruskals_ResultFile << endl;
    cout << "Trace written to " << kruskals_TraceFile << endl;
}

