#include "Graph.hpp"

template<typename V>
class DAG: Graph<V>{
    DAG(bool weighted);
    bool addEdge(int from, int to, double weightTo = 1.0, double weightBack = 1.0) override;
};