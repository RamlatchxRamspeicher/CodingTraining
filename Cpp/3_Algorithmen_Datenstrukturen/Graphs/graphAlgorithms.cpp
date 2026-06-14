#include "../LinearDatastructures/Queue.hpp"
#include "../LinearDatastructures/Stack.hpp"

#include <unordered_map>
#include <vector>
#include <set>

//search
template<typename graph_t>
std::optional<std::pair<std::vector<int>, double>> depthFistSearch(const graph_t& g, const int& startId, const int& targetId) {
    try{if(g.getVertex(startId)&&g.getVertex(targetId));}
    catch(...){return std::nullopt;} 
    std::set<int> visited;
    std::unordered_map<int, std::pair<int,double>> parent;
    Stack<int> s{g.getNextId()};
    s.push(startId);
    parent[startId] = {-1, 0.0};

    while(!s.isEmpty()){
        int current = s.pop();
        if(visited.count(current)) continue;
        visited.insert(current);
        if(current == targetId){
            // Pfad rekonstruieren
            std::vector<int> path;
            double cost = 0.0;
            int node = targetId;
            while(node != -1){
                path.push_back(node);
                auto& [prev, w] = parent[node];
                cost += w;
                node = prev;
            }
            std::reverse(path.begin(), path.end());
            return {{path, cost}};
        }
        
        for(auto& e : g.getNeighbours(current)){
            if(!visited.count(e.to)){
                parent[e.to] = {current, e.weight};
                s.push(e.to);
            }
        }
    }
    return std::nullopt;
}
template<typename graph_t>
std::optional<std::pair<std::vector<int>, double>> breadthFirstSearch(const graph_t& g, const int& startId, const int& targetId) {
    try{if(g.getVertex(startId)&&g.getVertex(targetId));}
    catch(...){return std::nullopt;} 
    std::set<int> visited;
    std::unordered_map<int, std::pair<int,double>> parent;
    Queue<int> q{g.getNextId()};
    q.queue(startId);
    visited.insert(startId);
    parent[startId] = {-1, 0.0};
    
    while(!q.isEmpty()){
        int current = q.dequeue();
        
        if(current == targetId){
            // Pfad rekonstruieren
            std::vector<int> path;
            double cost = 0.0;
            int node = targetId;
            while(node != -1){
                path.push_back(node);
                auto& [prev, w] = parent[node];
                cost += w;
                node = prev;
            }
            std::reverse(path.begin(), path.end());
            return {{path, cost}};
        }
        
        for(auto& e : g.getNeighbours(current)){
            if(!visited.count(e.to)){
                visited.insert(e.to);
                parent[e.to] = {current, e.weight};
                q.queue(e.to);
            }
        }
    }
    return std::nullopt;
}
//paths
template<typename graph_t>
std::unordered_map<int,int> BellmanFord(const graph_t& g, const int& start){
    //init
    std::vector<std::tuple<int, int, double>> Q = g.getAllEdges();
    std::unordered_map<int, double> costs;
    std::unordered_map<int, int> previous;
    for(auto& v: g.getIds()){
        costs[v] = std::numeric_limits<double>::max();
        previous[v] = -1;
    }
    costs[start] = 0;
    
    for(int i = 0; i<Q.size()-1;i++){
        for(auto& [from, to, weight]: Q){
            if(costs[from] != std::numeric_limits<double>::max()) continue;
            double alt = costs[from] + weight;
            if(alt < costs[to]){
                costs[to] = alt;
                previous[to] = from;
            }
        }
    }
    for(auto& [from, to, weight]: Q){
        if(costs[from] == std::numeric_limits<double>::max()) continue;
        if(costs[from] + weight < costs[to]) throw std::runtime_error("negative cycle!");
    }
    return previous;
}

template<typename graph_t>
std::unordered_map<int,int> Dijkstra(const graph_t& g, const int& start){
    //init
    std::set<int> Q = g.getIds();
    std::unordered_map<int, double> costs;
    std::unordered_map<int, int> previous;
    for(auto& v: Q){
        costs[v] = std::numeric_limits<double>::max();
        previous[v] = -1;
    }
    costs[start] = 0;

    
    //dijkstra alg
    while(!Q.empty()){
        int current = *Q.begin();
        for(auto& id: Q){
            if(costs[id] < costs[current]) current = id;
        }
        Q.erase(current);

        for(auto& e: g.getNeighbours(current)){
            if(Q.count(e.to)){
                double alt = costs[current] + e.weight;
                if(alt < costs[e.to]){
                    costs[e.to] = alt;
                    previous[e.to] = current;
                }
            }
        }
    }
    return previous;
}

template<typename graph_t>
std::vector<int> kahnsAlgorithm(const graph_t& g){
    std::vector<int> result;
    std::unordered_map<int, int> inDegree;
    std::vector<int> vertices = g.getIds();
    std::vector<std::tuple<int, int, double>> edges = g.getAllEdges();
    for(auto& id : vertices) inDegree[id] = 0;
    for(auto& [from, to, weight]: edges){
        inDegree[to]++;
    }
    Queue<int> q{(int)vertices.size()};
    for(auto& [k,v]: inDegree){
        if(v==0) q.queue(k);
    }

    while(!q.isEmpty()){
        auto v = q.dequeue();
        result.push_back(v);
        for(auto& e: g.getNeighbours(v)){
            inDegree[e.to]--;
            if(inDegree[e.to]==0) q.queue(e.to);
        }
    }
    if (result.size()!=vertices.size()) throw std::runtime_error("graph is cyclic!");
    return result;
}