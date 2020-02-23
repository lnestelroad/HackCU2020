#include <utility>
#include <memory>
#include <string>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/subgraph.hpp>
#include <boost/graph/graphviz.hpp>

struct Vertex
{
    std::string name;
    int weight;
};

struct Edge
{
    int weight;
};

using GraphvizAttributes =
    std::map<std::string, std::string>;

typedef boost::property<boost::edge_weight_t, int, boost::property<boost::edge_index_t, int, boost::property<boost::edge_attribute_t, GraphvizAttributes, Edge>>> EdgeProperty;

using Graph = boost::adjacency_list<boost::vecS, boost::vecS,
                                    boost::undirectedS,
                                    boost::property<boost::vertex_attribute_t, GraphvizAttributes, boost::property<boost::vertex_name_t, std::string>>,
                                    EdgeProperty,
                                    boost::property<boost::graph_name_t, std::string,
                                                    boost::property<boost::graph_graph_attribute_t, GraphvizAttributes,
                                                                    boost::property<boost::graph_vertex_attribute_t, GraphvizAttributes,
                                                                                    boost::property<boost::graph_edge_attribute_t, GraphvizAttributes>>>>>;

typedef boost::subgraph<Graph>
    SubGraph;

using edge_t = boost::graph_traits<SubGraph>::edge_descriptor;
//using vertex_t = boost::graph_traits<SubGraph>::vertex_descriptor;
using vertex_t = SubGraph::vertex_descriptor;
using vertex_pt = SubGraph::vertex_property_type;

// Constructs a graph from filename
SubGraph constructGraph(const std::string &filename);

// Constructs a subgraph from a vector of edges
SubGraph constructSubgraphFromEdges(const SubGraph &parent, const std::vector<edge_t> &edges);

// Adds vertices to a subgraph
void appendSubgraph(SubGraph &graph, const std::vector<vertex_t> &vertices);
//void writeGraph(const Graph &)