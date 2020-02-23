#include "graph.hpp"
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <fstream>
#include <stack>

const static std::string filename{"viz.dot"};
const static std::string outfile{"../../.cache_money/truck_paths.json"};

std::vector<SubGraph>
constructGraph(const std::string &jsonfile)
{
    using namespace boost;
    SubGraph g{0};
    SubGraph source{0};
    SubGraph sink{0};

    json j;

    std::ifstream i(jsonfile);
    i >> j;

    // Construct 3 graphs

    std::map<std::string, vertex_t> nameMap{};
    std::map<std::string, vertex_t> nameMapSource{};
    std::map<std::string, vertex_t> nameMapSink{};

    auto checkVertexType = [&](const int type) {
        return (type == 1) ? true : false;
    };

    for (json::iterator it = j.begin(); it != j.end(); ++it)
    {
        // checking if sink
        // probably need a bool conversion
        if (checkVertexType(stoi(it.value()["type"].dump())))
        {
            vertex_t src_vert = add_vertex(source);
            put(vertex_name, source, src_vert, it.key());
            // bool conversion?
            put(vertex_root, source, src_vert, checkVertexType(stoi(it.value()["type"].dump())));
            nameMapSource[it.key()] = src_vert;
        }
        else
        {
            vertex_t sink_vert = add_vertex(sink);
            put(vertex_name, sink, sink_vert, it.key());
            put(vertex_root, sink, sink_vert, checkVertexType(stoi(it.value()["type"].dump())));
            nameMapSink[it.key()] = sink_vert;
        }

        // Initializing full graph
        vertex_t new_vert = add_vertex(g);
        put(vertex_name, g, new_vert, it.key());
        put(vertex_root, g, new_vert, checkVertexType(stoi(it.value()["type"].dump())));
        //std::cout << "adding vertex: " << get(get(vertex_name, g), new_vert) << "\n"
        //          << it.value()["type"].dump() << "\n\n";
        nameMap[it.key()] = new_vert;
    }
    for (json::iterator it = j.begin(); it != j.end(); ++it)
    {
        for (json::iterator it2 = it.value()["edges"].begin(); it2 != it.value()["edges"].end(); ++it2)
        {
            if (nameMap.find(it2.key()) == nameMap.end())
            {
                std::cout << "shouldn't be here\n";
                vertex_t new_vert = add_vertex(g);
                put(vertex_name, g, new_vert, it2.key());
                nameMap[it2.key()] = new_vert;
            }
            auto [new_edge, added] = add_edge(nameMap.at(it.key()), nameMap.at(it2.key()), g);
            put(edge_weight, g, new_edge, stoi(it2.value()["edge_weight"].dump()));

            // Add edge to source graph
            if (nameMapSource.find(it2.key()) != nameMapSource.end() && nameMapSource.find(it.key()) != nameMapSource.end())
            {
                auto [new_edge, added] = add_edge(nameMapSource.at(it.key()), nameMapSource.at(it2.key()), source);
                put(edge_weight, source, new_edge, stoi(it2.value()["edge_weight"].dump()));
            }
            else if (nameMapSink.find(it2.key()) != nameMapSink.end() && nameMapSink.find(it.key()) != nameMapSink.end())
            {
                auto [new_edge, added] = add_edge(nameMapSink.at(it.key()), nameMapSink.at(it2.key()), sink);
                put(edge_weight, sink, new_edge, stoi(it2.value()["edge_weight"].dump()));
            }
        }

        // ADDING VERTEX WEIGHTS
        put(vertex_distance, g, nameMap.at(it.key()), stoi(it.value()["vertex_weight"].dump()));

        // also add for source and sink graphs?
    }

    return {g, source, sink};
}

SubGraph constructSubgraphFromEdges(SubGraph &parent, const std::vector<edge_t> &edges)
{
    using namespace boost;
    SubGraph g = parent.create_subgraph();
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

        put(vertex_name, g, src, get(get(vertex_name, parent), src));
        put(vertex_name, g, dest, get(get(vertex_name, parent), dest));

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

void graphTest()
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

    SubGraph MSTgraph = constructFromMST(G0, MST);
    write_graphviz(fs, MSTgraph);

    //write_graphviz(fs, G0);

    fs.close();
}

json preorderTraversal(const SubGraph &graph)
{
    using namespace boost;
    std::vector<bool> visited(num_vertices(graph), false);

    std::stack<vertex_t> stack;
    auto [begin, end] = vertices(graph);
    stack.push(*begin);

    json out;

    while (!stack.empty())
    {
        vertex_t vert = stack.top();
        stack.pop();

        if (!visited[vert])
        {
            out.push_back(get(get(vertex_name, graph), vert));

            auto [begin, end] = adjacent_vertices(vert, graph);
            for (; begin != end; begin++)
            {
                if (!visited[*begin])
                {
                    stack.push(*begin);
                }
            }
            visited[vert] = true;
        }
    }
    return out;
}

edge_t findConnectingEdge(const SubGraph &graph)
{
    using namespace boost;

    std::vector<bool> visited(num_vertices(graph), false);

    std::stack<vertex_t> stack;
    auto [begin, end] = vertices(graph);

    stack.push(*begin);

    int lowest = INT_MAX;
    edge_t lowest_edge;

    while (!stack.empty())
    {
        vertex_t vert = stack.top();
        stack.pop();

        if (!visited[vert])
        {
            visited[vert] = true;

            for (auto [begin, end] = adjacent_vertices(vert, graph); begin != end; begin++)
            {
                if (get(get(vertex_root, graph), *begin) != get(get(vertex_root, graph), vert))
                {
                    auto [ed, found] = edge(vert, *begin, graph);
                    if (get(get(edge_weight, graph), ed) < lowest)
                    {
                        lowest = get(get(edge_weight, graph), ed);
                        lowest_edge = ed;
                    }
                }
                if (!visited[*begin])
                {
                    stack.push(*begin);
                }
            }
        }
    }

    /*if (lowest_edge == -1)
    {
        std::cout << "ERROR FINDING CONNECTING\n";
    }*/
    return lowest_edge;
}

json constructRoute(std::vector<SubGraph> &graphs)
{
    using namespace boost;

    SubGraph g, src, sink;
    g = graphs[0];
    src = graphs[1];
    sink = graphs[2];

    std::vector<edge_t> MST;
    kruskal_minimum_spanning_tree(g, std::back_inserter(MST));

    std::vector<edge_t> srcMST;
    kruskal_minimum_spanning_tree(src, std::back_inserter(srcMST));

    std::vector<edge_t> sinkMST;
    kruskal_minimum_spanning_tree(sink, std::back_inserter(sinkMST));

    SubGraph MSTgraph = constructFromMST(g, MST);
    SubGraph MSTsrcGraph = constructFromMST(src, srcMST);
    SubGraph MSTsinkGraph = constructFromMST(sink, sinkMST);

    edge_t connecting = findConnectingEdge(g);
    vertex_t c1 = source(connecting, g);
    vertex_t c2 = target(connecting, g);

    json output;
    json srcTraversal = preorderTraversal(MSTsrcGraph);
    json sinkTraversal = preorderTraversal(MSTsinkGraph);
    json connect;
    connect.push_back(get(get(vertex_name, g), c2));
    output["source"] = srcTraversal;
    output["connect"] = connect;
    output["sink"] = sinkTraversal;

    return output;
}

int main()
{
    using namespace boost;
    //graphTest();

    std::fstream fs;
    std::fstream out;

    fs.open(filename, std::fstream::out);
    out.open(outfile, std::fstream::out);

    std::string jsonfile = "../../.cache_money/graph.json";
    SubGraph g, src, sink;

    std::vector<SubGraph> graphVec = constructGraph(jsonfile);

    json outJson = constructRoute(graphVec);
    std::cout << outJson << std::endl;
    //write_graphviz(fs, MSTsinkGraph);

    out << outJson;

    fs.close();
    out.close();

    return 0;
}
