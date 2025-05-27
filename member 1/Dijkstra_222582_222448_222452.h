#pragma once

#include <vector>
#include <queue>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <limits>
#include <set>

#include "../common/Graph_222582_222448_222452.h"

#include "../common/commonConstants.h"

using namespace std;

// trace file:
const string dijkstra_TraceFile = "../member 1/traces/dijkstra_trace.txt";
// result file:
const string dijkstra_shortestPath_ResultFile = "../member 1/results/dijkstra_shortest_path.txt";
// exection time file:
const string dijkstra_exectuionTime_File = "../member 1/executionTimes/dijkstra_execution_time.txt";



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

// IMPLEMENTATION: Find single source shortest path using Dijkstra and Bellman Ford algorithm.
void dijkstra(DirectedGraph<long long>& graph) {
    // Get the number of nodes and edges
    int nodes = graph.getNumberOfVertices();
    int edges = graph.getNumberOfEdges();

    // Get the start node
    int src = getStartNode(graphTraceFile);
    if (src == -1) {
        cerr << "Error: No valid source node found." << endl;
        return;
    }

    // Get the adjacency list
    vector<list<pair<long long, long long>>> adjList = graph.getAdjList();

    // Initialize distance vector and predecessor vector
    vector<long long> dist(nodes, numeric_limits<long long>::max());
    vector<int> predecessor(nodes, -1);
    dist[src] = 0;

    // Set to store (distance, node) pairs
    set<pair<long long, long long>> st; // Acts as a priority queue
    st.insert({0, src}); // Insert the source node with distance 0 into the "queue".

    // Open trace file
    ofstream traceFile(dijkstra_TraceFile);
    if (!traceFile.is_open()) {
        cerr << "Error: Could not open trace file " << dijkstra_TraceFile << endl;
        return;
    }

    traceFile << "Dijkstra Algorithm Trace" << endl;
    traceFile << "Source Node: " << src << endl;
    traceFile << "Number of Nodes: " << nodes << endl;
    traceFile << "Number of Edges: " << edges << endl;
    traceFile << "----------------------------------------" << endl;

    cout << "Dijkstra Algorithm Trace" << endl;
    cout << "Source Node: " << src << endl;
    cout << "Number of Nodes: " << nodes << endl;
    cout << "Number of Edges: " << edges << endl;
    cout << "----------------------------------------" << endl;

    // Start measuring execution time
    auto startTime = chrono::high_resolution_clock::now();

    // Main loop
    while (!st.empty()) {
        // Fetch the top record (node with the smallest distance)
        auto top = *(st.begin());
        st.erase(st.begin()); // Remove the processed node from the "queue"

        int nodeDistance = top.first;
        int topNode = top.second;

        // Log the dequeue operation
        traceFile << "Dequeued node " << topNode << " with current distance " << nodeDistance << endl;
        cout << "Dequeued node " << topNode << " with current distance " << nodeDistance << endl;

        // Traverse neighbors
        for (const auto& neighbour : adjList[topNode]) {
            int neighborNode = neighbour.first;
            long long weight = neighbour.second;

            // Relaxation step
            if (nodeDistance + weight < dist[neighborNode]) {
                // If the neighbor is already in the "queue", remove it
                auto record = st.find({dist[neighborNode], neighborNode});
                if (record != st.end()) {
                    st.erase(record); // Remove the outdated record from the "queue"
                    traceFile << "Removed outdated record for node " << neighborNode << " from the queue." << endl;
                }

                // Update distance and predecessor
                dist[neighborNode] = nodeDistance + weight;
                predecessor[neighborNode] = topNode;

                // Insert the updated record into the "queue"
                st.insert({dist[neighborNode], neighborNode});
                traceFile << "Enqueued node " << neighborNode << " with updated distance " << dist[neighborNode] << endl;
                cout << "Enqueued node " << neighborNode << " with updated distance " << dist[neighborNode] << endl;
            }
        }
    }

    // Stop measuring execution time
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    // Write execution time to file
    ofstream executionTimeFile(dijkstra_exectuionTime_File);
    if (!executionTimeFile.is_open()) {
        cerr << "Error: Could not open execution time file " << dijkstra_exectuionTime_File << endl;
        traceFile.close();
        return;
    }
    executionTimeFile << "Execution Time: " << duration.count() << " milliseconds" << endl;
    executionTimeFile.close();

    // Write results to file
    ofstream resultFile(dijkstra_shortestPath_ResultFile);
    if (!resultFile.is_open()) {
        cerr << "Error: Could not open result file " << dijkstra_shortestPath_ResultFile << endl;
        traceFile.close();
        return;
    }

    resultFile << "Node,Distance From Source,Path" << endl;
    traceFile << "Final Distances and Paths:" << endl;
    cout << "Final Distances and Paths:" << endl;

    for (int i = 0; i < nodes; i++) {
        if (dist[i] == numeric_limits<long long>::max()) {
            resultFile << i << ",INF,No Path" << endl;
            traceFile << "Node " << i << ": Distance = INF, Path = No Path" << endl;
            cout << "Node " << i << ": Distance = INF, Path = No Path" << endl;
        } else {
            // Reconstruct the path
            vector<int> path;
            for (int at = i; at != -1; at = predecessor[at]) {
                path.push_back(at);
            }
            reverse(path.begin(), path.end());

            // Convert path to string
            string pathStr;
            for (size_t j = 0; j < path.size(); j++) {
                pathStr += to_string(path[j]);
                if (j < path.size() - 1) {
                    pathStr += " -> ";
                }
            }

            resultFile << i << "," << dist[i] << "," << pathStr << endl;
            traceFile << "Node " << i << ": Distance = " << dist[i] << ", Path = " << pathStr << endl;
            cout << "Node " << i << ": Distance = " << dist[i] << ", Path = " << pathStr << endl;
        }
    }

    resultFile.close();
    traceFile.close();

    // Display execution time on terminal
    cout << "Dijkstra Results written to " << dijkstra_shortestPath_ResultFile << endl;
    cout << "Trace written to " << dijkstra_TraceFile << endl;
    cout << "Execution Time: " << duration.count() << " milliseconds" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Press Enter to continue..." << endl;
    cin.get();
}
