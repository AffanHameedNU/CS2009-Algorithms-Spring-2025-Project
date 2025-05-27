#pragma once

#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <iomanip> // For std::setw
#include <chrono>
#include <thread> // For simulating progress (optional)

#include "../common/Graph_222582_222448_222452.h"

#include "../common/commonConstants.h"

using namespace std;

// trace file:
const string bellmanFord_TraceFile = "../member 1/traces/bellman_ford_trace.txt";
// result file:
const string bellmanFord_shortestPath_ResultFile = "../member 1/results/bellmanFord_shortest_path.txt";
//execution time file:
const string bellmanFord_exectuionTime_File = "../member 1/executionTimes/bellmanFord_execution_time.txt";


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



// IMPLEMENTATION: Find single source shortest path using Dijkstra and Bellman Ford algorithm.
void bellmanFord(DirectedGraph<long long>& graph) {
    int nodes = graph.getNumberOfVertices();
    int edges = graph.getNumberOfEdges();

    int src = getStartingNode(graphTraceFile);
    if (src == -1) {
        cerr << "Error: No valid source node found." << endl;
        return;
    }

    ofstream traceFile(bellmanFord_TraceFile);
    if (!traceFile.is_open()) {
        cerr << "Error: Could not open trace file " << bellmanFord_TraceFile << endl;
        return;
    }

    traceFile << "Bellman-Ford Algorithm Trace" << endl;
    traceFile << "Source Node: " << src << endl;
    traceFile << "Number of Nodes: " << nodes << endl;
    traceFile << "Number of Edges: " << edges << endl;
    traceFile << "----------------------------------------" << endl;

    // Print trace header to the terminal
    cout << "Bellman-Ford Algorithm Trace" << endl;
    cout << "Source Node: " << src << endl;
    cout << "Number of Nodes: " << nodes << endl;
    cout << "Number of Edges: " << edges << endl;
    cout << "----------------------------------------" << endl;

    vector<long long> distance(nodes, numeric_limits<long long>::max());
    vector<int> predecessor(nodes, -1); // To store the predecessor of each node for path reconstruction
    distance[src] = 0;

    // Start measuring execution time
    auto startTime = chrono::high_resolution_clock::now();

    // Relax all edges |V|-1 times:
    for (int i = 1; i <= nodes - 1; i++) {
        traceFile << "Iteration " << i << " of " << nodes - 1 << ":" << endl;
        cout << "Iteration " << i << " of " << nodes - 1 << ":" << endl;

        bool updated = false; // Track if any distance was updated in this iteration

        for (int u = 0; u < nodes; u++) {
            vector<long long> neighbors = graph.getNeighbors(u);
            for (long long v : neighbors) {
                long long weight = graph.getWeight(u, v);
                if (distance[u] != numeric_limits<long long>::max() && distance[u] + weight < distance[v]) {
                    traceFile << "  Relaxing edge (" << u << " -> " << v << ") with weight " << weight << ". Updating distance of node " << v << " from " << distance[v] << " to " << distance[u] + weight << "." << endl;
                    cout << "  Relaxing edge (" << u << " -> " << v << ") with weight " << weight << ". Updating distance of node " << v << " from " << distance[v] << " to " << distance[u] + weight << "." << endl;
                    distance[v] = distance[u] + weight;
                    predecessor[v] = u; // Update the predecessor
                    updated = true;
                }
            }
        }

        if (!updated) {
            traceFile << "  No updates in this iteration. Stopping early." << endl;
            cout << "  No updates in this iteration. Stopping early." << endl;
            break;
        }
    }

    // Check for negative-weight cycles:
    traceFile << "Checking for negative-weight cycles..." << endl;
    cout << "Checking for negative-weight cycles..." << endl;
    for (int u = 0; u < nodes; u++) {
        vector<long long> neighbors = graph.getNeighbors(u);
        for (long long v : neighbors) {
            long long weight = graph.getWeight(u, v);
            if (distance[u] != numeric_limits<long long>::max() && distance[u] + weight < distance[v]) {
                traceFile << "Error: Graph contains a negative weight cycle involving edge (" << u << " -> " << v << ")." << endl;
                cout << "Error: Graph contains a negative weight cycle involving edge (" << u << " -> " << v << ")." << endl;
                traceFile.close();
                return;
            }
        }
    }
    traceFile << "No negative-weight cycles detected." << endl;
    cout << "No negative-weight cycles detected." << endl;

    // Stop measuring execution time
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    // Output the execution time to the execution time file
    ofstream executionTimeFile(bellmanFord_exectuionTime_File);
    if (!executionTimeFile.is_open()) {
        cerr << "Error: Could not open execution time file " << bellmanFord_exectuionTime_File << endl;
        traceFile.close();
        return;
    }
    executionTimeFile << "Execution Time: " << duration.count() << " milliseconds" << endl;
    executionTimeFile.close();

    // Output the results to the result file
    ofstream resultFile(bellmanFord_shortestPath_ResultFile);
    if (!resultFile.is_open()) {
        cerr << "Error: Could not open result file " << bellmanFord_shortestPath_ResultFile << endl;
        traceFile.close();
        return;
    }

    resultFile << "Node,Distance From Source,Path" << endl;
    traceFile << "Final Distances and Paths:" << endl;
    cout << "Final Distances and Paths:" << endl;

    for (int i = 0; i < nodes; i++) {
        if (distance[i] == numeric_limits<long long>::max()) {
            resultFile << i << ",INF,No Path" << endl;
            traceFile << "Node " << i << ": Distance = INF, Path = No Path" << endl;
            cout << "Node " << i << ": Distance = INF, Path = No Path" << endl;
        } else {
            // Reconstruct the path from the source to the current node
            vector<int> path;
            for (int at = i; at != -1; at = predecessor[at]) {
                path.push_back(at);
            }
            reverse(path.begin(), path.end());

            // Convert the path to a string
            string pathStr;
            for (size_t j = 0; j < path.size(); j++) {
                pathStr += to_string(path[j]);
                if (j < path.size() - 1) {
                    pathStr += " -> ";
                }
            }

            resultFile << i << "," << distance[i] << "," << pathStr << endl;
            traceFile << "Node " << i << ": Distance = " << distance[i] << ", Path = " << pathStr << endl;
            cout << "Node " << i << ": Distance = " << distance[i] << ", Path = " << pathStr << endl;
        }
    }

    resultFile.close();
    traceFile.close();

    // Display execution time on the terminal
    cout << "Bellman-Ford Results written to " << bellmanFord_shortestPath_ResultFile << endl;
    cout << "Trace written to " << bellmanFord_TraceFile << endl;
    cout << "Execution Time: " << duration.count() << " milliseconds" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Press Enter to continue...";
    cin.get();
}

