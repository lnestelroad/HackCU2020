#include "graph.hpp"
#include <boost/graph/kruskal_min_spanning_tree.hpp>

int main()
{
    using namespace boost;

    const int N = 6;

    SubGraph G0(N);

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

    add_vertex(C, G1);
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

    std::vector<edge_t> MST;

    kruskal_minimum_spanning_tree(G0, std::back_inserter(MST));

    for (const auto &ed : MST)
    {
        std::cout << get(get(edge_weight, G0), ed) << "\n";
    }

    write_graphviz(std::cout, G0);

    return 0;
}