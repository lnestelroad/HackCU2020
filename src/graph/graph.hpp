#include <utility>
#include <memory>

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

//using namespace boost;
/*
namespace boost
{
enum edge_weight_t
{
    edge_weight
};

BOOST_INSTALL_PROPERTY(edge, weight);
} // namespace boost
*/
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
/*
using Graph = boost::adjacency_list<boost::vecS, boost::vecS,
                                    boost::undirectedS,
                                    boost::property<boost::vertex_attribute_t, GraphvizAttributes, Vertex>,
                                    boost::property<boost::edge_index_t, int, boost::property<boost::edge_attribute_t, GraphvizAttributes, Edge>>,
                                    boost::property<boost::graph_name_t, std::string,
                                                    boost::property<boost::graph_graph_attribute_t, GraphvizAttributes,
                                                                    boost::property<boost::graph_vertex_attribute_t, GraphvizAttributes,
                                                                                    boost::property<boost::graph_edge_attribute_t, GraphvizAttributes>>>>>;

*/
typedef boost::subgraph<Graph>
    SubGraph;

//typedef boost::named_graph<SubGraph> NamedSubGraph;

using edge_t = boost::graph_traits<SubGraph>::edge_descriptor;
//using vertex_t = boost::graph_traits<SubGraph>::vertex_descriptor;
using vertex_t = SubGraph::vertex_descriptor;
using vertex_pt = SubGraph::vertex_property_type;

//boost::property_map<SubGraph, boost::edge_weight_t>::type weight = get
//using edgeid_map = boost::property_map<SubGraph, boost::edge_index_t>::type;
//edgeid_map edge_id = boost::get(edge_index, G);
//boost::property_map < SubGraph,

/*
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::undirectedS, Edge> Traits;
typedef boost::subgraph < boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, Vertex, Edge> Graph;
//typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
//typedef boost::graph_traits<Graph>::edge_descriptor edge_t;
*/
// Constructs a subgraph of the graph
Graph constructGraph();

Graph constructSubgraph(const Graph &parent, const std::unique_ptr<int[]> &vertices);

//void writeGraph(const Graph &)