#include "networkflow.hpp"

#define TOO_FEW_VERTICES "Too few vertices."
#define TOO_FEW_EDGES "Too few edges."
#define EDGE_WEIGHT_ZERO "Detected edge weight of 0."
#define EDGE_BAD_ENDPOINT "Edge interacts with nonexistent vertex."
#define SELF_LOOP "At least one self-loop."
#define MULTI_EDGES "Detected multi-edges."
#define NOT_ONE_SRC "Zero or more than one source."
#define NOT_ONE_SINK "Zero or more than one sink."
#include <map>
#include <algorithm>
#include <queue>
#include <iostream>
bool has_BFS_path(  const std::vector<std::vector<unsigned>> adj_matrix, 
                    unsigned s, 
                    unsigned t, 
                    std::vector<int>& parent_path, 
                    unsigned num_vertices){

    bool discovered[num_vertices];
    for(unsigned i = 0; i < num_vertices; i++){
        discovered[i] = false;
    }
    std::queue<unsigned> processing;
    processing.push(s);
    discovered[s] = true;
    parent_path[s] = -1; //to a value that cant be in the vertices
    while (!processing.empty()) {
        unsigned current_node = processing.front(); 
        processing.pop();
 
        for (unsigned j = 0; j < num_vertices; j++) {
            //when reaching the sink
            if (discovered[j] == false && j == t && adj_matrix[current_node][j] > 0) {
                parent_path[j] = current_node;
                return true;
            }
            if (discovered[j] == false && adj_matrix[current_node][j] > 0) {
                parent_path[j] = current_node;
                processing.push(j);
                discovered[j] = true;
                
            }
        }
    }
    return false;
}

std::vector<Edge> solveNetworkFlow(
    const std::vector<Edge>& flowNetwork,
    unsigned numVertices)
{

    unsigned num_source = 0;
    unsigned num_sink = 0;
    std::vector<unsigned> vertices;
    std::vector<unsigned> FROM;
    std::vector<unsigned> TO;
    unsigned s; //source
    unsigned t; //sink

    for(unsigned k = 0; k < numVertices; k++){
        vertices.push_back(k);
    }
    if(numVertices < 2){
        std::__throw_runtime_error(TOO_FEW_VERTICES);
    }
    else if(flowNetwork.size() == 0){
        std::__throw_runtime_error(TOO_FEW_EDGES);
    }
    for(unsigned i = 0; i < flowNetwork.size(); i++){
        if(flowNetwork[i].weight == 0){
            std::__throw_runtime_error(EDGE_WEIGHT_ZERO);
        }
        else if(flowNetwork[i].from >= numVertices || flowNetwork[i].to >= numVertices){
            std::__throw_runtime_error(EDGE_BAD_ENDPOINT);
        }
        else if(flowNetwork[i].from == flowNetwork[i].to){
            std::__throw_runtime_error(SELF_LOOP);
        }
        for(unsigned j = i+1; j < flowNetwork.size(); j++){
            if(flowNetwork[i].from == flowNetwork[j].from){ //from A to B and B back to A
                if(flowNetwork[j].to == flowNetwork[i].to && (flowNetwork[i].weight != flowNetwork[j].weight)){
                    std::__throw_runtime_error(MULTI_EDGES);
                }
            }
        }
        FROM.push_back(flowNetwork[i].from);
        TO.push_back(flowNetwork[i].to);  
    }
    //find s AND t  
    for(unsigned y = 0; y < numVertices; y++){
        if(std::find(TO.begin(), TO.end(), vertices[y]) == TO.end()){//other vertices dont have s as child(not in TO)
            num_source += 1;
            s = vertices[y];
        }
        if(std::find(FROM.begin(), FROM.end(), vertices[y]) == FROM.end()){//other vertices dont have t as parent(not in FROM)
            num_sink += 1;
            t = vertices[y];
        }
    }
    if(num_source != 1){
        std::__throw_runtime_error(NOT_ONE_SRC);
    } 
    else if(num_sink != 1){
        std::__throw_runtime_error(NOT_ONE_SINK);
    }
    // std::cout << "Source is: " << s << std::endl;
    // std::cout << "Sink is: " << t << std::endl;

    //Initialize the Rgraph and adjacency list
    std::vector<Edge> Rflow;
    for(unsigned z = 0; z < flowNetwork.size(); z++){
        Rflow.push_back(flowNetwork[z]);
    }
    std::vector<std::vector<unsigned>> adjacency_matrix(numVertices, std::vector<unsigned> (numVertices, 0)); //2d vector
    std::vector<std::vector<unsigned>> R_matrix(numVertices, std::vector<unsigned> (numVertices, 0));
    std::vector<std::vector<unsigned>> Back_flow_matrix(numVertices, std::vector<unsigned> (numVertices, 0));
    //from--> row to-->col
    for(unsigned a = 0; a < Rflow.size(); a++){
        adjacency_matrix[Rflow[a].from][Rflow[a].to] = Rflow[a].weight;
        R_matrix[Rflow[a].from][Rflow[a].to] = Rflow[a].weight;
    }
    //    A -> B -> C -> D
    std::vector<int> parent_path(numVertices); // array indicating selected path
    has_BFS_path(adjacency_matrix, s, t, parent_path, numVertices);
    unsigned current_node;
    unsigned bottleneck = adjacency_matrix[parent_path[t]][t];
    //Implement Ford Fulkerson
            //from now, keep changing adjacency matrix weights
    while(has_BFS_path(adjacency_matrix, s, t, parent_path, numVertices)){

        for(unsigned i = t; i != s; i = parent_path[i]){ //going backward from t to s to retrieve the path
            current_node = parent_path[i];
            bottleneck = std::min(adjacency_matrix[current_node][i], bottleneck); //i's parent to i find the bottleneck
        }

        for(unsigned i = t; i != s; i = parent_path[i]){
            current_node = parent_path[i];
            adjacency_matrix[current_node][i] -= bottleneck; //backward flow
            Back_flow_matrix[i][current_node] = 1;
            adjacency_matrix[i][current_node] += bottleneck;
        }
    }
    for ( size_t i = 0; i < R_matrix.size(); i++ )
    {
        for ( size_t j = 0; j < R_matrix[i].size(); j++ ){
            if(Back_flow_matrix[i][j] != 1){
                R_matrix[i][j] -= adjacency_matrix[i][j]; //not a backward flow in matrix
            }
        }
    }
    // std::cout << "Adj flow:"<<std::endl;
    // for ( size_t i = 0; i < adjacency_matrix.size(); i++ )
    // {
    //     for ( size_t j = 0; j < adjacency_matrix[i].size(); j++ ){
    //         std::cout << adjacency_matrix[i][j] << ' ';
            
    //     }
    //     std::cout << std::endl;
    // }
    std::vector<Edge> return_flowNetwork;
    for(unsigned i = 0; i < R_matrix.size(); i++) {
        for(unsigned j = 0; j < R_matrix[i].size(); j++) {
            if(R_matrix[i][j] != 0) {
                return_flowNetwork.push_back({i, j, R_matrix[i][j]});
            }         
        }
    }
    return return_flowNetwork;
}