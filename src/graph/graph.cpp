#include "graph.hpp"
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <fstream>

const static std::string filename{"graphviz.dot"};

int main()
{
    using namespace boost;

    std::fstream fs;

    fs.open(filename, std::fstream::out);

    const int N = 6;

    SubGraph G0(0);

    enum
    {
        A,
        B,
        C,
        D,
        E,
        F
    };

    SubGraph &G1 = G0.create_subgraph(), G2 = G0.create_subgraph();

    enum
    {
        A1,
        B1,
        C2
    };
    enum
    {
        A2,
        B2
    };

    // Add vertices to g1
    //vertex_pt vpt = (std::map<std::string, std::string> m{}, "hi");
    //vertex_t vpt{std::map<std::string, std::string>{}, "HI"};
    /*
    vertex_t vert = add_vertex(vpt, G1);
    
    add_vertex(E, G1);
    add_vertex(F, G1);

    // Add vertices to g2
    add_vertex(A, G2);
    add_vertex(B, G2);

    // Add edges gto g0
    add_edge(A, B, 2, G0);
    add_edge(B, C, 2, G0);
    add_edge(B, D, 2, G0);
    add_edge(E, B, 2, G0);
    add_edge(E, F, 2, G0);
    add_edge(F, D, 7, G0);
    */

    vertex_t u = add_vertex(G0);
    vertex_t v = add_vertex(G0);
    add_edge(u, v, 2, G0);

    //G0[u].vertex_name = "hi";
    put(vertex_name, G0, u, "hi");
    put(vertex_name, G0, v, "this");
    //v.name = "hi";
    //get(get(vertex_name, G0), v);
    //put(vertex_name, v, "Hello!");
    //G0[vert].vertex_name = "HI";

    std::vector<edge_t>
        MST;

    std::cout << u << v << std::endl;

    kruskal_minimum_spanning_tree(G0, std::back_inserter(MST));

    for (const auto &ed : MST)
    {
        vertex_t src = source(ed, G0);
        vertex_t dest = target(ed, G0);
        std::cout << get(get(vertex_name, G0), src) << "\n";
        std::cout << get(get(vertex_name, G0), dest) << "\n";
        std::cout << get(get(edge_weight, G0), ed) << "\n";
    }

    write_graphviz(fs, G0);

    fs.close();

    return 0;
}

SubGraph constructGraph(const std::string &filename)
{
    // IMPLIMENT
    SubGraph g{0};
    return g;
}

SubGraph constructSubgraphFromEdges(const SubGraph &parent, const std::vector<edge_t> &edges)
{
    // IMPLIMENT
    SubGraph g{0};
    return g;
}

void appendSubgraph(SubGraph &graph, const std::vector<vertex_t> &vertices)
{
    return 0;
}
