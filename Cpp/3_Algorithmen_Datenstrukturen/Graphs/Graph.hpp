#include "../HashBasedStructures/Hashtable.hpp"
#include "graphAlgorithms.cpp"




struct Edge
{
    int to;
    double weight = 1.0;
};

template<typename V>
struct  Vertex
{
    int id;
    V value;
};

enum RoutingType{
    DIJKSTRA,
    BELLMAN_FORD,
    BFS_PATH
};



template<typename V>
class Graph{
    int nextId;
    bool directed;
    bool weighted;
    HashTableChain<int, Vertex<V>, SortedBucket> vertices;
    HashTableChain<int, std::vector<Edge>, SortedBucket> edges;
    public:
        Graph(bool directed, bool weighted);
        Graph(const Graph& other);
        Graph& operator=(const Graph& other);
        ~Graph(){};
        Edge* _getEdge(const int& from, const int& to);
        const Edge* _getEdge(const int& from, const int& to) const;
        bool addVertex(V value);
        bool addEdge(int from, int to, double weightTo=1.0, double weightBack=1.0);
        bool removeVertex(int id);
        bool removeEdge(int from, int to);
        bool updateVertex(int id, V value);
        bool updateEdgeWeight(int from, int to, double weight=1.0);
        int getNextId() const;
        std::vector<int> getIds() const;
        std::vector<Edge> getNeighbours(int id) const;
        std::vector<std::tuple<int, int, double>> getAllEdges() const;
        std::vector<std::vector<double>> getAdjMatrix()const;
        void printAdjMatrix()const;
        void printAdjList()const;
        V getVertex(int id)const;
        std::optional<std::pair<std::vector<int>, double>> BFS(const int& startId, const int& targetId) const;
        std::vector<Edge> BFSTraverse(const int& startId) const;
        std::optional<std::pair<std::vector<int>, double>> DFS(const int& startId, const int& targetId) const;
        std::vector<Edge> DFSTraverse(const int& startId) const;
        std::optional<std::pair<std::vector<int>, double>> getPath(const int& start, const int& end, RoutingType type);
        std::vector<int> topologicalSort();
};
