#include "graph.hpp"
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <fstream>

const static std::string filename{"viz.dot"};

SubGraph constructGraph(const std::string &filename)
{
    using namespace boost;
    // IMPLIMENT
    SubGraph g{0};
    return g;
}

SubGraph constructSubgraphFromEdges(SubGraph &parent, const std::vector<edge_t> &edges)
{
    using namespace boost;
    // IMPLIMENT
    SubGraph g = parent.create_subgraph();
    std::cout << edges.size() << std::endl;
    for (const auto &ed : edges)
    {
        vertex_t src = source(ed, parent);
        vertex_t dest = target(ed, parent);

        add_vertex(src, g);
        add_vertex(dest, g);
    }

    return g;
}

int getVectorIndex(const std::vector<std::string> &vec, const std::string &target)
{
    for (int i = 0; i < vec.size(); ++i)
    {
        if (vec[i] == target)
        {
            return i;
        }
    }
    return -1;
}

int getVectorIndex(const std::vector<vertex_t> &vec, const vertex_t &target)
{
    for (int i = 0; i < vec.size(); ++i)
    {
        if (vec[i] == target)
        {
            return i;
        }
    }
    return -1;
}

SubGraph constructFromMST(SubGraph &parent, const std::vector<edge_t> &edges)
{
    using namespace boost;
    SubGraph g{num_vertices(parent)};

    for (const auto &ed : edges)
    {
        vertex_t src = source(ed, parent);
        vertex_t dest = target(ed, parent);

        add_edge(src, dest, get(get(edge_weight, parent), ed), g);
    }

    return g;
}

void appendSubgraph(SubGraph &graph, const std::vector<vertex_t> &vertices)
{
    for (const auto &ver : vertices)
    {
        add_vertex(ver, graph);
    }
}

int main()
{
    using namespace boost;

    std::fstream fs;

    fs.open(filename, std::fstream::out);

    SubGraph G0(0);

    // Add vertices to g1
    vertex_t a = add_vertex(G0);
    vertex_t b = add_vertex(G0);
    vertex_t c = add_vertex(G0);
    vertex_t d = add_vertex(G0);
    vertex_t e = add_vertex(G0);
    vertex_t f = add_vertex(G0);
    vertex_t g = add_vertex(G0);

    add_edge(a, b, 1, G0);
    add_edge(b, c, 3, G0);
    add_edge(d, e, 5, G0);
    add_edge(c, d, 2, G0);
    add_edge(c, e, 7, G0);
    add_edge(f, g, 9, G0);
    add_edge(e, f, 11, G0);
    add_edge(a, f, 13, G0);
    add_edge(g, a, 17, G0);
    add_edge(d, c, 90, G0);

    put(vertex_name, G0, a, "a");
    put(vertex_name, G0, b, "b");
    put(vertex_name, G0, c, "c");
    put(vertex_name, G0, d, "d");
    put(vertex_name, G0, e, "e");
    put(vertex_name, G0, f, "f");
    put(vertex_name, G0, g, "g");

    std::vector<edge_t>
        MST;

    kruskal_minimum_spanning_tree(G0, std::back_inserter(MST));

    for (const auto &ed : MST)
    {
        vertex_t src = source(ed, G0);
        vertex_t dest = target(ed, G0);
        std::cout << get(get(vertex_name, G0), src) << "\n";
        std::cout << get(get(vertex_name, G0), dest) << "\n";
        //std::cout << get(get(edge_weight, G0), ed) << "\n";
    }

    SubGraph MSTgraph = copyGraphFromEdges(G0, MST);
    write_graphviz(fs, MSTgraph);

    //write_graphviz(fs, G0);

    fs.close();

    return 0;
}
