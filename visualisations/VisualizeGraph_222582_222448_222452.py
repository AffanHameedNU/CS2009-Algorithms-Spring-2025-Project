import networkx as nx
import matplotlib.pyplot as plt

def load_graph_from_txt(filename, max_edges=25000):
    G = nx.DiGraph()
    edge_count = 0

    with open(filename, 'r') as f:
        for line in f:
            if ':' not in line:
                continue
            src, rest = line.strip().split(':')
            src = int(src)
            if not rest.strip():
                continue
            neighbors = rest.strip().split(') ')
            for neighbor in neighbors:
                if neighbor:
                    neighbor = neighbor.replace('(', '').replace(')', '').strip()
                    if neighbor:
                        dst, weight = map(int, neighbor.split(','))
                        G.add_edge(src, dst, weight=weight)
                        edge_count += 1
                        if edge_count >= max_edges:
                            return G
    return G

if __name__ == "__main__":
    filename = "../common/directedGraph_trace.txt"
    G = load_graph_from_txt(filename)

    print(f"Visualizing graph with {G.number_of_nodes()} nodes and {G.number_of_edges()} edges.")

    plt.figure(figsize=(12, 12))
    pos = nx.spring_layout(G, k=0.1)  # Layout for better spacing
    nx.draw(G, pos, with_labels=True, node_size=30, font_size=6, arrows=True)
    plt.title("Graph Visualization (Subset of Edges)")
    plt.savefig("../visualisations/plots/graph_visualization.png")  # Save the plot as an image
    print("Graph visualization saved as 'graph_visualization.png'.")
