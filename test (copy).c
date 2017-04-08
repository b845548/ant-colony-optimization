
#include <string>


#include <boost/graph/adjacency_list.hpp>
#include <iostream>
#include <vector>

#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

// Create a struct to hold properties for each vertex
typedef struct VertexProperties
{
  int p1;
} VertexProperties;

// Create a struct to hold properties for each edge
typedef struct EdgeProperties
{
  int   p1;
} EdgeProperties;

// Define the type of the graph
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, VertexProperties, EdgeProperties> Graph;


int main(int,char*[])
{
  // Create a graph object
  Graph g;

  // Add vertices
  Graph::vertex_descriptor v0 = boost::add_vertex(g);
  Graph::vertex_descriptor v1 = boost::add_vertex(g);
  Graph::vertex_descriptor v2 = boost::add_vertex(g);

  // Set vertex properties
  g[v0].p1 = 1;
  g[v1].p1 = 2;
  g[v2].p1 = 3;

  // Add edges
  std::pair<Graph::edge_descriptor, bool> e01 = boost::add_edge(v0, v1, g);
  std::pair<Graph::edge_descriptor, bool> e02 = boost::add_edge(v1, v2, g);

  // Set edge properties
  g[e01.first].p1 = 1;
  g[e02.first].p1 = 2;

  std::cout << "num_verts: " << g[e02.first].p1 << std::endl;
  std::cout << "num_edges: " << boost::num_edges(g) << std::endl;

  // compute ST shortest paths here...

  return 0;
}
