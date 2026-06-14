#include "Graph.hpp"


template<typename V>
Graph<V>::Graph(bool directed, bool weighted): nextId(0), directed(directed), weighted(weighted), vertices(HashTableChain<int, Vertex<V>, SortedBucket>{MIN_SIZE,Hasher<int>::fnv1aHash}), edges(HashTableChain<int, std::vector<Edge>, SortedBucket>{MIN_SIZE,Hasher<int>::fnv1aHash}) {}
template<typename V>
Graph<V>::Graph(const Graph& other)
    : nextId(other.nextId), directed(other.directed), weighted(other.weighted),
      vertices(HashTableChain<int, Vertex<V>, SortedBucket>{other.vertices.stats().tableSize, Hasher<int>::fnv1aHash}),
      edges(HashTableChain<int, std::vector<Edge>, SortedBucket>{other.edges.stats().tableSize, Hasher<int>::fnv1aHash})
{
    for(auto& [k,v]: other.vertices)
        vertices.insert(k,v);
    for(auto& [k,v]: other.edges)
        edges.insert(k,v);
}
template<typename V>
Graph<V>& Graph<V>::operator=(const Graph<V>& other){
    if(this == &other) return *this;
    nextId   = other.nextId;
    directed = other.directed;
    weighted = other.weighted;
    vertices = HashTableChain<int, Vertex<V>, SortedBucket>{other.vertices.stats().tableSize, Hasher<int>::fnv1aHash};
    edges    = HashTableChain<int, std::vector<Edge>, SortedBucket>{other.edges.stats().tableSize, Hasher<int>::fnv1aHash};
    for(auto& [k,v]: other.vertices)
        vertices.insert(k,v);
    for(auto& [k,v]: other.edges)
        edges.insert(k,v);
    return *this;
}
template<typename V>
Edge* Graph<V>::_getEdge(const int& from, const int& to){
    std::vector<Edge>* fromEdges = edges.get(from);
    if(!fromEdges) return nullptr;
    for(auto& e: *fromEdges)
        if(e.to==to) return &e;
    return nullptr;
}
template<typename V>
const Edge* Graph<V>::_getEdge(const int& from, const int& to) const{
    const std::vector<Edge>* fromEdges = edges.get(from);
    if(!fromEdges) return nullptr;
    for(const auto& e: *fromEdges)
        if(e.to==to) return &e;
    return nullptr;
}
template<typename V>
bool Graph<V>::addVertex(V value){
    try{
        vertices.insert(nextId,Vertex<V>{nextId,value});
        nextId++;
        return true;
    } catch(...) {return false;}
}
template<typename V>
bool Graph<V>::addEdge(int from, int to, double weightTo, double weightBack){
    try{
        std::vector<Edge>* existing = edges.get(from);
        if(!existing) {
            edges.insert(from, std::vector<Edge>{{to,weightTo}});
        } else {
            existing->push_back(Edge{to,weightTo});
            edges.insert(from, *existing);
        }
        if(!directed){
            std::vector<Edge>* existing = edges.get(to);
            if(!existing) {
                edges.insert(from, std::vector<Edge>{{from,weightBack}});
            } else {
                existing->push_back(Edge{from,weightBack});
                edges.insert(to, *existing);
            }
        }
        return true;
    } catch(...) {return false;}
}
template<typename V>
bool Graph<V>::removeVertex(int id){
    try{
        if(!vertices.remove(id)) return false;
        edges.remove(id);
        for(auto&[k,edgeList]: edges){
            std::vector<Edge> remaining;
            for(auto& e : edgeList)
                if(e.to != id) remaining.push_back(e);
            edges.insert(k, remaining);
        }
        return true;
    } catch(...) {return false;}
}
template<typename V>
bool Graph<V>::removeEdge(int from, int to){
    try{
        std::vector<Edge>* fromEdges = edges.get(from);
        if(!fromEdges) return false;
        std::vector<Edge> remaining;
        for(auto& e: *fromEdges){
            if(e.to!=to) remaining.push_back(e);
        }
        edges.insert(from,remaining);

        if(!directed){
            std::vector<Edge>* toEdges = edges.get(to);
            if(!toEdges) return false;
            std::vector<Edge> remaining2;

            for(auto& e: *toEdges){
                if(e.to!=from) remaining2.push_back(e);
            }
            edges.insert(to, remaining2);
        }
        return true;
    } catch(...) {return false;}
}
template<typename V>
bool Graph<V>::updateVertex(int id, V value){
    if(!vertices.get(id)) return false;
    vertices.insert(id, Vertex<V>{id, value});
    return true;
}
template<typename V>
bool Graph<V>::updateEdgeWeight(int from, int to, double weight){
    Edge* e = _getEdge(from, to);
    if(!e) return false;
    e->weight = weight;
    if(!directed){
        Edge* back = _getEdge(to, from);
        if(back) back->weight = weight;
    }
    return true;
}
template<typename V>
int Graph<V>::getNextId()const{
    return nextId-1;
}
template<typename V>
std::vector<int> Graph<V>::getIds()const{
    std::vector<int> result;
    for(auto& [_,v]: vertices){result.push_back(v.id);}
    return result;
}
template<typename V>
std::vector<Edge> Graph<V>::getNeighbours(int id) const{
    std::vector<Edge>* fromEdges = edges.get(id);
    std::vector<Edge> neighbours;
    if(fromEdges)
        for(auto& e: *fromEdges)
                neighbours.push_back(e);
    return neighbours;
}
template<typename V>
std::vector<std::tuple<int, int, double>> Graph<V>::getAllEdges() const{
    std::vector<std::tuple<int, int, double>> result;
    for(auto& id: getIds()){
        for(auto& e: getNeighbours(id)){
            result.push_back({id,e.to,e.weight});
        }
    }
    return result;
}
template<typename V>
std::vector<std::vector<double>> Graph<V>::getAdjMatrix()const{
    std::vector<std::vector<double>> result;
    std::vector<int> ids = _getIds();
    for(auto& from: ids){
        std::vector<double> inner;
        for(auto& to: ids){
            Edge* e = getEdge(from, to);
            inner.push_back(e ? e->weight : 0.0);
        }
        result.push_back(inner);
    }
    return  result;
}
#include <iomanip>
template<typename V>
void Graph<V>::printAdjMatrix()const{
    std::vector<int> ids = _getIds();

    std::cout << "    ";
    for(auto& head: ids)
        std::cout << "|   " << std::setw(3) << head << "   ";
    std::cout << "|" << std::endl;

    for(auto& y: ids){
        std::cout << std::setw(3) << y << " ";
        for(auto& x: ids){
            auto e = findEdge(y, x);
            std::cout << "| " << std::setw(5) << std::fixed << std::setprecision(1) 
                      << (e ? e->weight : 0.0) << " ";
        }
        std::cout << "|" << std::endl;
    }
}
template<typename V>
void Graph<V>::printAdjList()const{
    std::vector<int> ids = _getIds();

    for(auto& node: ids){
        std::cout << "[" << std::setw(3) << node << "] : [";
        auto next = getNeighbours(node);
        for(auto& e: next){
            std::cout << " {" << std::setw(3) << e.to << ", " << std::setw(5) << std::fixed << std::setprecision(1) 
                      << e.weight << "} ";
        }
        std::cout << "]" << std::endl;
    }
}
template<typename V>
V Graph<V>::getVertex(int id) const{
    return vertices.get(id)->value;
}
template<typename V>
std::optional<std::pair<std::vector<int>, double>> Graph<V>::BFS(const int& startId, const int& targetId) const {
    return breadthFirstSearch(*this,startId,targetId);
}
template<typename V>
std::vector<Edge> Graph<V>::BFSTraverse(const int& startId) const {
    std::set<int> visited;
    std::vector<Edge> result;
    Queue<int> q{nextId};
    
    q.queue(startId);
    visited.insert(startId);
    
    while(!q.isEmpty()){
        int current = q.dequeue();
        
        for(auto& e : getNeighbours(current)){
            if(!visited.count(e.to)){
                visited.insert(e.to);
                result.push_back(e);
                q.queue(e.to);
            }
        }
    }
    return result;
}
template<typename V>
std::optional<std::pair<std::vector<int>, double>> Graph<V>::DFS(const int& startId, const int& targetId) const {
    return depthFistSearch(*this, startId, targetId);
}
template<typename V>
std::vector<Edge> Graph<V>::DFSTraverse(const int& startId) const {
    std::set<int> visited;
    std::vector<Edge> result;
    Stack<int> s{nextId};

    s.push(startId);

    while(!s.isEmpty()){
        int current = s.pop();
        if(visited.count(current)) continue;
        visited.insert(current);
        
        for(auto& e : getNeighbours(current)){
            if(!visited.count(e.to)){
                result.push_back(e);
                s.push(e.to);
            }
        }
    }
    return result;
}


template<typename V>
std::optional<std::pair<std::vector<int>, double>> Graph<V>::getPath(const int& start, const int& end, RoutingType type){
    std::unordered_map<int,int> previous;
    switch(type){
        case DIJKSTRA:     previous = Dijkstra(*this, start); break;
        case BELLMAN_FORD: previous = BellmanFord(*this, start); break;
        case BFS_PATH: {
            auto result = BFS(start, end);
            return result;}
    }
    int current = end;
    Stack<Edge> tmp{nextId};
    while (current!=start){
        if(previous[current] == -1) return std::nullopt;
        const Edge* e = _getEdge(previous[current], current);
        if(!e) return std::nullopt;
        tmp.push(*e);
        current=previous[current];
    }
    std::vector<int> path;
    double cost = 0.0;
    path.push_back(start);
    while (!tmp.empty()){
        auto& e = tmp.pop();
        path.push_back(e.to);
        cost += e.weight;
    }
    return std::pair<std::vector<int>, double>{path,cost};
}
template<typename V>
std::vector<int> Graph<V>::topologicalSort(){
    return kahnsAlgorithm(*this);
}

/* template<typename V>
std::optional<std::pair<std::vector<int>, double>> Graph<V>::BFS(const int& startId, const int& targetId) const {
    std::vector<int> resultV;
    double resultC=0.0;
    
    bool visited[nextId];
    visited.fill[false];
    Queue<std::pair<int,int>> traverseList{nextId};
    Stack<std::pair<int,Edge>> trackEdges{nextId};
    traverseList.queue({-1,startId});
    
    bool found=false;
    int previous, current;
    while(true){
        try{auto [p,c]=traverseList.dequeue();previous=p;current=c;} catch(std::runtime_error){return NULL;}// no more edges to traverse -> not found
        std::vector<Edge> nextV = getNeighbours(current);
        if(nextV.empty())continue; //dead end
        if(previous!=-1){// track where you came from
            std::vector<Edge>* fromPrevious = edges[previous];
            for(auto& e: *fromPrevious){
                if(e.to==current)trackEdges.push(previous,e); 
            }
        }
        for(auto& e: nextV){// for all current edges if next vertex already traversed ignore, if next vertex is target track it and go next step else queue edge
            if(visited[e.to])continue;
            if(e.to==targetId){
                trackEdges.push(std::pair<int,Edge>{current,e});
                found=true;
                break;
            }
            traverseList.queue(std::pair{current,e.to});
        }
        if(found) break; // if not found check as visited and that 
        visited[current]=true;
        previous=current;
    }
    Stack<int> tmp{nextId};
    auto& [pid, ped] = trackEdges.pop();
    tmp.push(pid);
    resultC+=ped.weight;
    int id;
    while(true){
        try{auto& [_id, ed] = trackEdges.pop();id=_id;}catch(...){break;}
        if(pid==ed.to){
            tmp.push(id);
            resultC+=ed.weight;
        }
        pid=id;
    }
    while(true){try{resultV.push_back(tmp.pop());}catch(...){break;}}
    return {resultV, resultC};
} */