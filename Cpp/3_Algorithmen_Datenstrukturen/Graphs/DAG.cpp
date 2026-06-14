#include "DAG.hpp"



template<typename V>
DAG<V>::DAG(bool weighted) : Graph<V>(true, weighted) {}
template<typename V>
bool DAG<V>::addEdge(int from, int to, double weightTo, double weightBack){
    if(!Graph<V>::addEdge(from, to, weightTo)) return false;
    try{
        this->topologicalSort();
        return true;
    } catch(...){
        Graph<V>::removeEdge(from, to);
        return false;
    }
}