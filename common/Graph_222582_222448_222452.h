#pragma once

#include <vector>
#include <utility>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <map>


using namespace std;


const string filename = "../data/data.csv";
const string directed_traceFile = "../common/directedGraph_trace.txt";
const string biDirected_traceFile = "../common/biDirectedGraph_trace.txt";

//this file will contain the code to build the graph from the csv file.
//this file will be used by other files multiple times, when called it has to build the graph from the csv file everytime.

template <typename T>
class DirectedGraph {
private:
    vector<list<pair<T, long long>>> adjList; // Adjacency list: each node points to a list of (neighbor, weight)

public:
    DirectedGraph(int n) {
        adjList = vector<list<pair<T, long long>>>(n);
    }

    void addEdge(T u, T v, long long weight) {
        adjList[u].emplace_back(v, weight);
    }

    void removeEdge(T u, T v) {
        adjList[u].remove_if([v](const pair<T, long long>& edge) {
            return edge.first == v;
        });
    }

    bool hasEdge(T u, T v) {
        for (const auto& edge : adjList[u]) {
            if (edge.first == v) {
                return true;
            }
        }
        return false;
    }

    int getWeight(T u, T v) {
        for (const auto& edge : adjList[u]) {
            if (edge.first == v) {
                return edge.second;
            }
        }
        return 0; // Return 0 if no edge exists
    }

    vector<T> getNeighbors(T u) {
        vector<T> neighbors;
        for (const auto& edge : adjList[u]) {
            neighbors.push_back(edge.first);
        }
        return neighbors;
    }

    int getNumberOfVertices() const {
        return adjList.size();
    }

    int getNumberOfEdges() {
        int count = 0;
        for (const auto& neighbors : adjList) {
            count += neighbors.size();
        }
        return count;
    }

    vector<list<pair<T, long long>>> getAdjList() const {
        return adjList;
    }

    void printGraph() {
        for (int i = 0; i < adjList.size(); i++) {
            cout << i << ": ";
            for (const auto& edge : adjList[i]) {
                cout << "(" << edge.first << ", " << edge.second << ") ";
            }
            cout << endl;
        }
    }

    void printGraphToFile(const string& outputFilename) {
        ofstream outFile(outputFilename);
        if (!outFile.is_open()) {
            cerr << "Error: Could not open file " << outputFilename << endl;
            return;
        }

        for (int i = 0; i < adjList.size(); i++) {
            outFile << i << ": ";
            for (const auto& edge : adjList[i]) {
                outFile << "(" << edge.first << ", " << edge.second << ") ";
            }
            outFile << endl;
        }

        outFile.close();
        cout << "Directed Graph written to " << outputFilename << " in adjacency list format." << endl;
    }
};


DirectedGraph<long long> BuildDirectedGraphFromCSV() {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could Not Open File " << filename << endl;
        return DirectedGraph<long long>(0);
    }

    string line;
    vector<tuple<long long, long long, long long>> edges; // To temporarily store edges (u, v, weight)
    set<long long> uniqueNodes; // To track unique node IDs

    // Read each line and extract source, destination, and weight
    while (getline(file, line)) {
        stringstream ss(line);
        string srcStr, destStr, weightStr;

        if (!getline(ss, srcStr, ',')) continue;
        if (!getline(ss, destStr, ',')) continue;
        if (!getline(ss, weightStr, ',')) continue;

        long long u = stoll(srcStr);
        long long v = stoll(destStr);
        long long weight = stoll(weightStr);

        edges.push_back({u, v, weight});
        uniqueNodes.insert(u); // Add source node to the set
        uniqueNodes.insert(v); // Add destination node to the set
    }

    file.close();

    // Determine the minimum and maximum node IDs
    long long minNodeId = *uniqueNodes.begin();
    long long maxNodeId = *uniqueNodes.rbegin();

    // Create a graph with the actual number of unique nodes
    DirectedGraph<long long> graph(maxNodeId + 1);

    // Add edges to the graph
    for (const auto& [u, v, weight] : edges) {
        graph.addEdge(u, v, weight);
    }

    // Print the graph to a file
    graph.printGraphToFile(directed_traceFile);

    return graph;
}






template <typename T>
class BiDirectedGraph {
private:
    vector<list<pair<T, long long>>> adjList; // Adjacency list: each node points to a list of (neighbor, weight)

public:
    BiDirectedGraph(int n) {
        adjList = vector<list<pair<T, long long>>>(n);
    }

    void addEdge(T u, T v, long long weight) {
        adjList[u].emplace_back(v, weight); // Add edge u -> v
        adjList[v].emplace_back(u, weight); // Add edge v -> u (bidirectional)
    }

    void removeEdge(T u, T v) {
        adjList[u].remove_if([v](const pair<T, long long>& edge) {
            return edge.first == v;
        });
        adjList[v].remove_if([u](const pair<T, long long>& edge) {
            return edge.first == u;
        });
    }

    bool hasEdge(T u, T v) {
        for (const auto& edge : adjList[u]) {
            if (edge.first == v) {
                return true;
            }
        }
        return false;
    }

    int getWeight(T u, T v) {
        for (const auto& edge : adjList[u]) {
            if (edge.first == v) {
                return edge.second;
            }
        }
        return 0; // Return 0 if no edge exists
    }

    vector<T> getNeighbors(T u) {
        vector<T> neighbors;
        for (const auto& edge : adjList[u]) {
            neighbors.push_back(edge.first);
        }
        return neighbors;
    }

    int getNumberOfVertices() const {
        return adjList.size();
    }

    int getNumberOfEdges() {
        int count = 0;
        for (const auto& neighbors : adjList) {
            count += neighbors.size();
        }
        return count / 2; // Each edge is counted twice in an undirected graph
    }

    vector<list<pair<T, long long>>> getAdjList() const {
        return adjList;
    }

    void printGraph() {
        for (int i = 0; i < adjList.size(); i++) {
            cout << i << ": ";
            for (const auto& edge : adjList[i]) {
                cout << "(" << edge.first << ", " << edge.second << ") ";
            }
            cout << endl;
        }
    }

    void printGraphToFile(const string& outputFilename) {
        ofstream outFile(outputFilename);
        if (!outFile.is_open()) {
            cerr << "Error: Could not open file " << outputFilename << endl;
            return;
        }

        for (int i = 0; i < adjList.size(); i++) {
            outFile << i << ": ";
            for (const auto& edge : adjList[i]) {
                outFile << "(" << edge.first << ", " << edge.second << ") ";
            }
            outFile << endl;
        }

        outFile.close();
        cout << "Bi Directed Graph written to " << outputFilename << " in adjacency list format." << endl;
    }
};

BiDirectedGraph<long long> BuildBiDirectedGraphFromCSV() {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could Not Open File " << filename << endl;
        return BiDirectedGraph<long long>(0);
    }

    string line;
    vector<tuple<long long, long long, long long>> edges; // To temporarily store edges (u, v, weight)
    set<long long> uniqueNodes; // To track unique node IDs

    // Read each line and extract source, destination, and weight
    while (getline(file, line)) {
        stringstream ss(line);
        string srcStr, destStr, weightStr;

        if (!getline(ss, srcStr, ',')) continue;
        if (!getline(ss, destStr, ',')) continue;
        if (!getline(ss, weightStr, ',')) continue;

        long long u = stoll(srcStr);
        long long v = stoll(destStr);
        long long weight = stoll(weightStr);

        edges.push_back({u, v, weight});
        uniqueNodes.insert(u); // Add source node to the set
        uniqueNodes.insert(v); // Add destination node to the set
    }

    file.close();

    // Determine the minimum and maximum node IDs
    long long minNodeId = *uniqueNodes.begin();
    long long maxNodeId = *uniqueNodes.rbegin();

    // Create a graph with the actual number of unique nodes
    BiDirectedGraph<long long> graph(maxNodeId + 1);

    // Add edges to the graph
    for (const auto& [u, v, weight] : edges) {
        graph.addEdge(u, v, weight);
    }

    // Print the graph to a file
    graph.printGraphToFile(biDirected_traceFile);

    return graph;
}


