#include <utility>
#include <memory>
#include <string>
#include <vector>
#include <tuple>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/subgraph.hpp>
#include <boost/graph/graphviz.hpp>
#include "parsing.hpp"

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
                                    boost::property<boost::vertex_attribute_t, GraphvizAttributes, boost::property<boost::vertex_name_t, std::string, boost::property<boost::vertex_distance_t, int, boost::property<boost::vertex_root_t, bool>>>>,
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
std::tuple<std::vector<SubGraph>, std::vector<vertex_t>, std::vector<vertex_t>>
constructGraph(const std::string &jsonfile);

// Constructs a subgraph from a vector of edges
SubGraph constructSubgraphFromEdges(SubGraph &parent, const std::vector<edge_t> &edges);

// PRECONDITION: edges need to include all vertices. i.e: be a MST
SubGraph constructFromMST(SubGraph &parent, const std::vector<edge_t> &edges);

// Adds vertices to a subgraph
void appendSubgraph(SubGraph &graph, const std::vector<vertex_t> &vertices);
//void writeGraph(const Graph &)

json preorderTraversal(const SubGraph &graph);

edge_t findConnectingEdge(const SubGraph &graph);
//edge_t findMinEdge(std::pair<boost::vertex_iterator, boost::vertex_iterator> group1, std::pair<boost::vertex_iterator, boost::vertex_iterator> group2);

// Vector of three elements, main, source, and sink. Returns truck route to traverse source then sink
json constructRoute(std::vector<SubGraph> &graphs);

void sortVerticesByAbsValue(std::vector<vertex_t> &vertices, SubGraph &graph);

std::map<vertex_t, vertex_t> assignSources(std::vector<vertex_t> &sources, std::vector<vertex_t> &sinks, SubGraph &sourceGraph, SubGraph &sinkGraph);

std::optional<vertex_t> extractFromName(const vertex_t &sub_vertex, SubGraph &subGraph, SubGraph &transGraph);

std::vector<SubGraph> createSubgraphs(SubGraph &subGraph);