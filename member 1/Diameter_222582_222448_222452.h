#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <fstream>
#include <chrono>
#include <sstream>

#include "../common/Graph_222582_222448_222452.h"
#include "../common/commonConstants.h"

using namespace std;

// execution time file :
const string diameter_exectuionTime_File = "../member 1/executionTimes/diameter_execution_time.txt";
// trace file :
const string diameter_TraceFile = "../member 1/traces/diameter_trace.txt";
// result file :
const string diameter_ResultFile = "../member 1/results/diameter_result.txt";
const string dijkstra_ResultFile = "../member 1/results/dijkstra_shortest_path.txt";


int getSourceNode(const string& filename) {
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



void calculateDiameter() {
    // Open the result file for Dijkstra's algorithm
    ifstream resultFile(dijkstra_ResultFile);
    if (!resultFile.is_open()) {
        cerr << "Error: Could not open result file " << dijkstra_ResultFile << endl;
        return;
    }

    // Open the trace file for logging
    ofstream traceFile(diameter_TraceFile);
    if (!traceFile.is_open()) {
        cerr << "Error: Could not open trace file " << diameter_TraceFile << endl;
        resultFile.close();
        return;
    }

    traceFile << "Diameter Calculation Trace" << endl;
    traceFile << "Parsing Dijkstra's result file: " << dijkstra_ResultFile << endl;
    traceFile << "----------------------------------------" << endl;

    cout << "Diameter Calculation Trace" << endl;
    cout << "Parsing Dijkstra's result file: " << dijkstra_ResultFile << endl;
    cout << "----------------------------------------" << endl;

    // Start measuring execution time
    auto startTime = chrono::high_resolution_clock::now();

    string line;
    long long maxDistance = 0; // Variable to store the maximum distance (diameter)
    bool headerSkipped = false; // To skip the header line

    // Parse the file line by line
    while (getline(resultFile, line)) {
        // Skip the header line
        if (!headerSkipped) {
            headerSkipped = true;
            continue;
        }

        // Split the line into components (Node, Distance From Source, Path)
        stringstream ss(line);
        string node, distance, path;

        getline(ss, node, ',');      // Node
        getline(ss, distance, ','); // Distance From Source
        getline(ss, path, ',');     // Path (not used here)

        // Log the parsed line to the trace file
        traceFile << "Processing Node: " << node << ", Distance: " << distance << ", Path: " << path << endl;

        // Check if the distance is "INF" (indicating no path)
        if (distance == "INF") {
            traceFile << "  Skipping Node " << node << " as it has no valid path (Distance = INF)." << endl;
            continue; // Skip nodes with no path
        }

        // Convert the distance to a numeric value
        long long distValue = stoll(distance);

        // Update the maximum distance if the current distance is greater
        if (distValue > maxDistance) {
            traceFile << "  Updating maximum distance. Previous: " << maxDistance << ", New: " << distValue << endl;
            maxDistance = distValue;
        }
    }

    resultFile.close();

    // Stop measuring execution time
    auto endTime = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    // Write the diameter to the result file
    ofstream diameterFile(diameter_ResultFile);
    if (!diameterFile.is_open()) {
        cerr << "Error: Could not open diameter result file " << diameter_ResultFile << endl;
        traceFile.close();
        return;
    }

    diameterFile << "Graph Diameter: " << maxDistance << endl;
    diameterFile.close();

    // Log the result to the trace file
    traceFile << "----------------------------------------" << endl;
    traceFile << "Graph Diameter: " << maxDistance << endl;
    traceFile << "Execution Time: " << duration.count() << " milliseconds" << endl;
    traceFile.close();

    // Write the execution time to the execution time file
    ofstream executionTimeFile(diameter_exectuionTime_File);
    if (!executionTimeFile.is_open()) {
        cerr << "Error: Could not open execution time file " << diameter_exectuionTime_File << endl;
        return;
    }

    executionTimeFile << "Execution Time: " << duration.count() << " milliseconds" << endl;
    executionTimeFile.close();

    // Print the diameter and execution time to the console
    cout << "Graph Diameter: " << maxDistance << endl;
    cout << "Execution Time: " << duration.count() << " milliseconds" << endl;
    cout << "Results written to " << diameter_ResultFile << endl;
    cout << "Trace written to " << diameter_TraceFile << endl;
    cout << "----------------------------------------" << endl;
}
