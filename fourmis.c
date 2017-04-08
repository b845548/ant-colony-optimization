
#include <string>


#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <vector>

#include <random>
#include <cassert>
#include <algorithm>
#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

typedef struct VertexProperties
{
    mutable int passage;
    mutable int nb_fourmis;
    mutable int ville;

} VertexProperties;

typedef struct EdgeProperties
{
    mutable double pheromones;
    mutable double distance;

} EdgeProperties;

// Define the type of the graph
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, VertexProperties, EdgeProperties> Graph;
void random_graph(Graph& g, int nb_vertex, int perc) {
    // Ajout de nb_vertex sommets
    for(int i = 0; i < nb_vertex; ++i){
        Graph::vertex_descriptor vertexProperty = boost::add_vertex(g);
        g[vertexProperty].ville=i;
        g[vertexProperty].passage=g[vertexProperty].nb_fourmis=2;
    }
    // Générateur de nombres pseudo-aléatoires
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long> dis(0, nb_vertex - 1);
    std::uniform_int_distribution<unsigned long> dis2(1, nb_vertex * perc / 100);
    std::uniform_real_distribution<double>initPheromone(0.0, 1.0);
    std::uniform_real_distribution<double>initDistance(1.0, 20.0);
    // Ajout d'arêtes
    for (auto pair_it = vertices(g);
         pair_it.first != pair_it.second;
         ++pair_it.first) {
        int nb_edge = dis2(gen);
        for(int i = 0; i < nb_edge; ++i) {
            auto vertex1 = *pair_it.first;
            auto vertex2 = dis(gen);
            if(vertex2 != vertex1){
               std::pair<Graph::edge_descriptor, bool> edgeProperty = boost::add_edge(vertex1, vertex2, g);
               g[edgeProperty.first].pheromones = initPheromone(gen);
               g[edgeProperty.first].distance = initDistance(gen);
            
            }
        }
    }
}


void affichage(const Graph& g) 
{
    for(auto ve = vertices(g);ve.first!=ve.second;++ve.first) 
    {
        std::cout<< g[*ve.first].ville << " nbf: "<< g[*ve.first].nb_fourmis<<std::endl;
        for (auto pair_it = adjacent_vertices(*ve.first, g); pair_it.first != pair_it.second; ++pair_it.first) 
        {
            std::pair<Graph::edge_descriptor, bool> edgeProperty = boost::edge(*ve.first, *pair_it.first, g);
            std::cout<<*ve.first<<"->"<< *pair_it.first<<" d: "<<g[edgeProperty.first].distance <<" p: "<< g[edgeProperty.first].pheromones << std::endl;
        }
    }
}


void deplacement(const Graph& g) 
{
    double probabiliteMax,probabilite;
    int indexMax;

    // POUR TOUS LES SOMMETS
    for(auto ve = vertices(g);ve.first!=ve.second;++ve.first) 
    {

        // TANT QUE IL Y A UN FOURMIS A DEPLACER
        while(g[*ve.first].nb_fourmis!=0)
        {

            // TROUVER MAX PROBABILITE
            indexMax=-1;
            probabiliteMax=-1.0;            
            for (auto pair_it = adjacent_vertices(*ve.first, g); pair_it.first != pair_it.second; ++pair_it.first) 
            {         
                std::pair<Graph::edge_descriptor, bool> edgeProperty = boost::edge(*ve.first, *pair_it.first, g);
                probabilite=g[edgeProperty.first].pheromones/(double)g[edgeProperty.first].distance;
                if(probabiliteMax < probabilite)
                {
                    probabiliteMax = probabilite;
                    indexMax = *pair_it.first;
                }
            }
            // TROUVER MAX PROBABILITE

            // DEPLACEMENT D'UN FOURMIS 
            g[*ve.first].nb_fourmis--;
            g[indexMax].nb_fourmis++;
            g[indexMax].passage++;
            // DEPLACEMENT D'UN FOURMIS

            // UPDATE PHEROMONE
            for (auto pair_it = adjacent_vertices(*ve.first, g); pair_it.first != pair_it.second; ++pair_it.first) 
            {         
                std::pair<Graph::edge_descriptor, bool> edgeProperty = boost::edge(*ve.first, *pair_it.first, g);
                g[edgeProperty.first].pheromones=g[edgeProperty.first].pheromones/2. + 100. * g[*ve.first].passage;
            }
            // UPDATE PHEROMONE

        }
        // TANT QUE IL Y A UN FOURMIS A DEPLACER

    }
    // POUR TOUS LES SOMMETS
}


// g++ fourmis.c -I/home/hbxxx/boost_1_63_0 -std=c++11 -Wall -W
int main(int,char*[])
{
  Graph g;
  random_graph(g, 40, 3);
  affichage(g);
  for(int i=0;i<1000;++i)
      deplacement(g);
  std::cout << "num_vertices: " << boost::num_vertices(g) << std::endl;
  std::cout << "num_edges: " << boost::num_edges(g) << std::endl;
  return 0;
}
