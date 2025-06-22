#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <queue>

using namespace std;

struct GraphNode {
    char data;
    vector<weak_ptr<GraphNode>> neighbors;

    GraphNode(char value) : data(value) {}
};
class GraphList {
private:
    map<char, shared_ptr<GraphNode>> nodes;

    // Fungsi helper rekursif untuk DFS
    void DFSHelper(shared_ptr<GraphNode> currentNode, map<char, bool>& visited) {
        // Tandai node saat ini sebagai sudah dikunjungi dan proses
        visited[currentNode->data] = true;
        cout << currentNode->data << " ";

        // Kunjungi semua tetangga yang belum pernah dikunjungi secara rekursif
        for (auto& weakNeighbor : currentNode->neighbors) {
            if (auto sharedNeighbor = weakNeighbor.lock()) { // Ambil shared_ptr dari weak_ptr
                if (!visited[sharedNeighbor->data]) {
                    DFSHelper(sharedNeighbor, visited);
                }
            }
        }
    }
public:
    void addNode(char value) {
        if (nodes.find(value) == nodes.end()) {
            nodes[value] = make_shared<GraphNode>(value);
            cout << "Node '" << value << "' ditambahkan." << endl;
        }
    }

    void addDirectedEdge(char from, char to) {
        if (nodes.count(from) && nodes.count(to)) {
            nodes[from]->neighbors.push_back(nodes[to]);
            cout << "Koneksi " << from << " -> " << to << " dibuat." << endl;
        }
    }

    void addUndirectedEdge(char node1, char node2) {
        if (nodes.count(node1) && nodes.count(node2)) {
            nodes[node1]->neighbors.push_back(nodes[node2]);
            nodes[node2]->neighbors.push_back(nodes[node1]);
            cout << "Koneksi " << node1 << " <-> " << node2 << " dibuat." << endl;
        }
    }

    void printGraph() {
        cout << "\n--- Struktur Graph ---" << endl;
        for (const auto& pair : nodes) {
            cout << "Node " << pair.first << " terhubung ke: ";
            for (const auto& neighbor_ptr : pair.second->neighbors) {
                if (auto neighbor = neighbor_ptr.lock()) {
                    cout << neighbor->data << " ";
                }
            }
            cout << endl;
        }
        cout << "--------------------" << endl;
    }

    void BFS(char startNodeData) {
        cout << "BFS traversal dimulai dari node " << startNodeData << ": ";

        // Cek apakah node awal ada
        if (nodes.find(startNodeData) == nodes.end()) {
            cout << "Node awal tidak ditemukan!" << endl;
            return;
        }

        // Map untuk melacak node yang sudah dikunjungi
        map<char, bool> visited;

        // Buat sebuah queue untuk BFS
        queue<shared_ptr<GraphNode>> q;

        // Tandai node awal sebagai dikunjungi dan masukkan ke queue
        visited[startNodeData] = true;
        q.push(nodes[startNodeData]);

        while (!q.empty()) {
            // Keluarkan node dari depan queue dan proses (print)
            shared_ptr<GraphNode> currentNode = q.front();
            q.pop();
            cout << currentNode->data << " ";

            // Dapatkan semua tetangga dari node yang sedang diproses.
            // Jika tetangga belum dikunjungi, tandai, dan masukkan ke queue.
            for (auto& weakNeighbor : currentNode->neighbors) {
                if (auto sharedNeighbor = weakNeighbor.lock()) { // ubah weak_ptr ke shared_ptr
                    if (!visited[sharedNeighbor->data]) {
                        visited[sharedNeighbor->data] = true;
                        q.push(sharedNeighbor);
                    }
                }
            }
        }
        cout << endl;
    }

    void DFS(char startNodeData) {
        cout << "DFS traversal dimulai dari node " << startNodeData << ": ";

        // Cek apakah node awal ada
        if (nodes.find(startNodeData) == nodes.end()) {
            cout << "Node awal tidak ditemukan!" << endl;
            return;
        }

        // Map untuk melacak node yang sudah dikunjungi, akan di-pass ke helper
        map<char, bool> visited;

        // Panggil fungsi rekursif untuk memulai traversal
        DFSHelper(nodes[startNodeData], visited);

        cout << endl;
    }
};

class GraphMatrix {
private:
    vector<vector<int>> adjMatrix; // Matriks untuk menyimpan koneksi (0 atau 1)
    map<char, int> nodeToIndex;    // Memetakan 'A' -> 0, 'B' -> 1, dst.
    vector<char> indexToNode;      // Memetakan 0 -> 'A', 1 -> 'B', dst.
    int nodeCount;                 // Jumlah node saat ini

    void DFSHelper(int currentIndex, vector<bool>& visited) {
        visited[currentIndex] = true;
        cout << indexToNode[currentIndex] << " "; // Cetak char dari index

        // Untuk menemukan tetangga, kita harus memindai seluruh baris
        for (int neighborIndex = 0; neighborIndex < nodeCount; ++neighborIndex) {
            // Jika ada koneksi (nilai=1) dan belum dikunjungi
            if (adjMatrix[currentIndex][neighborIndex] == 1 && !visited[neighborIndex]) {
                DFSHelper(neighborIndex, visited);
            }
        }
    }

public:
    GraphMatrix() : nodeCount(0) {} // Constructor

    void addNode(char value) {
        if (nodeToIndex.find(value) == nodeToIndex.end()) {
            // Tambahkan pemetaan
            nodeToIndex[value] = nodeCount;
            indexToNode.push_back(value);

            // Perbesar matriks: tambahkan satu kolom ke setiap baris yang ada
            for (int i = 0; i < nodeCount; ++i) {
                adjMatrix[i].push_back(0);
            }
            // Tambahkan satu baris baru di bawah
            adjMatrix.push_back(vector<int>(nodeCount + 1, 0));

            nodeCount++;
        }
    }

    void addDirectedEdge(char from, char to) {
        if (nodeToIndex.count(from) && nodeToIndex.count(to)) {
            int fromIndex = nodeToIndex[from];
            int toIndex = nodeToIndex[to];
            adjMatrix[fromIndex][toIndex] = 1;
        }
    }

    void printGraph() {
        cout << "\n--- Struktur Graph (Adjacency Matrix) ---" << endl;
        cout << "    ";
        for (int i = 0; i < nodeCount; ++i) cout << indexToNode[i] << " ";
        cout << "\n----";
        for (int i = 0; i < nodeCount; ++i) cout << "--";
        cout << endl;

        for (int i = 0; i < nodeCount; ++i) {
            cout << indexToNode[i] << " | ";
            for (int j = 0; j < nodeCount; ++j) {
                cout << adjMatrix[i][j] << " ";
            }
            cout << endl;
        }
        cout << "-----------------------------------------" << endl;
    }

    void BFS(char startNodeData) {
        cout << "BFS (Matrix) dimulai dari node " << startNodeData << ": ";
        if (nodeToIndex.find(startNodeData) == nodeToIndex.end()) { cout << "Node awal tidak ditemukan!" << endl; return; }

        vector<bool> visited(nodeCount, false);
        queue<int> q; // Queue sekarang menyimpan integer (index)

        int startIndex = nodeToIndex[startNodeData];
        visited[startIndex] = true;
        q.push(startIndex);

        while (!q.empty()) {
            int currentIndex = q.front();
            q.pop();
            cout << indexToNode[currentIndex] << " "; // Cetak char dari index

            // Untuk menemukan tetangga, pindai seluruh baris
            for (int neighborIndex = 0; neighborIndex < nodeCount; ++neighborIndex) {
                if (adjMatrix[currentIndex][neighborIndex] == 1 && !visited[neighborIndex]) {
                    visited[neighborIndex] = true;
                    q.push(neighborIndex);
                }
            }
        }
        cout << endl;
    }

    void DFS(char startNodeData) {
        cout << "DFS (Matrix) dimulai dari node " << startNodeData << ": ";
        if (nodeToIndex.find(startNodeData) == nodeToIndex.end()) { cout << "Node awal tidak ditemukan!" << endl; return; }

        vector<bool> visited(nodeCount, false);
        int startIndex = nodeToIndex[startNodeData];
        DFSHelper(startIndex, visited);
        cout << endl;
    }
};

int main() {
    cout << "===== MENGGUNAKAN ADJACENCY LIST =====" << endl;

    GraphList graphList;

    graphList.addNode('A');
    graphList.addNode('B');
    graphList.addNode('C');
    graphList.addNode('D');
    graphList.addNode('E');
    graphList.addNode('F');

    graphList.addDirectedEdge('A', 'B');
    graphList.addDirectedEdge('B', 'C');
    graphList.addDirectedEdge('C', 'D');
    graphList.addDirectedEdge('D', 'E');
    graphList.addDirectedEdge('E', 'F');
    graphList.addDirectedEdge('F', 'A');

    graphList.printGraph();

    graphList.BFS('A');
    graphList.DFS('A');

    cout << "\n\n===== MENGGUNAKAN ADJACENCY MATRIX =====" << endl;

    GraphMatrix graphMatrix;

    graphMatrix.addNode('A');
    graphMatrix.addNode('B');
    graphMatrix.addNode('C');
    graphMatrix.addNode('D');
    graphMatrix.addNode('E');
    graphMatrix.addNode('F');

    graphMatrix.addDirectedEdge('A', 'B');
    graphMatrix.addDirectedEdge('B', 'C');
    graphMatrix.addDirectedEdge('C', 'D');
    graphMatrix.addDirectedEdge('D', 'E');
    graphMatrix.addDirectedEdge('E', 'F');
    graphMatrix.addDirectedEdge('F', 'A');

    graphMatrix.printGraph();

    graphMatrix.BFS('B');
    graphMatrix.DFS('A');

    return 0;
}