#include "graph.hpp"
#include <queue>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <fstream>
#include <stack>

const static std::string filename{"viz.dot"};
const static std::string outfile{"./.cache_money/truck_paths.json"};

constexpr int NUM_TRUCKS{3};

std::tuple<std::vector<SubGraph>, std::vector<vertex_t>, std::vector<vertex_t>>
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
    //checkVertexType
    std::vector<vertex_t>
        sourceVertices{};
    std::vector<vertex_t> sinkVertices{};

    auto checkVertexType = [&](const int type) {
        return (type == 1) ? true : false;
    };

    for (json::iterator it = j.begin(); it != j.end(); ++it)
    {
        // checking if sink
        // probably need a bool conversion
        if ((stoi(it.value()["type"].dump())))
        {
            vertex_t src_vert = add_vertex(source);
            put(vertex_name, source, src_vert, it.key());
            // bool conversion?
            put(vertex_root, source, src_vert, checkVertexType(stoi(it.value()["type"].dump())));
            put(vertex_distance, source, src_vert, stoi(it.value()["vertex_weight"].dump()));

            nameMapSource[it.key()] = src_vert;
            sourceVertices.push_back(src_vert);
        }
        else
        {
            vertex_t sink_vert = add_vertex(sink);
            put(vertex_name, sink, sink_vert, it.key());
            put(vertex_root, sink, sink_vert, checkVertexType(stoi(it.value()["type"].dump())));
            put(vertex_distance, sink, sink_vert, -1 * stoi(it.value()["vertex_weight"].dump()));

            nameMapSink[it.key()] = sink_vert;
            sinkVertices.push_back(sink_vert);
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
        /*
        // ADDING VERTEX WEIGHTS
        if (checkVertexType(stoi(nameMap.at(it.key())["type"].dump())))
        {
            put(vertex_distance, g, nameMap.at(it.key()), stoi(it.value()["vertex_weight"].dump()));
            //std::cout << "Souce Val: " << get(get(vertex_distance, g), it.kety)
        }
        else
        {
            put(vertex_distance, g, nameMap.at(it.key()), -1 * stoi(it.value()["vertex_weight"].dump()));
        }

                //put(vertex_distance, g, nameMap.at(it.key()), stoi(it.value()["vertex_weight"].dump()));
        */
        // also add for source and sink graphs?
    }

    return {{g, source, sink}, sourceVertices, sinkVertices};
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

    SubGraph MSTgraph;
    SubGraph MSTsrcGraph;
    SubGraph MSTsinkGraph;

    if (num_edges(g) <= 1)
    {
        MSTgraph = g;
    }
    else
    {
        MSTgraph = constructFromMST(g, MST);
    }

    if (num_edges(src) <= 1)
    {
        MSTsrcGraph = src;
    }
    else
    {
        MSTsrcGraph = constructFromMST(src, srcMST);
    }

    if (num_edges(sink) <= 1)
    {
        MSTsinkGraph = sink;
    }
    else
    {
        MSTsinkGraph = constructFromMST(sink, sinkMST);
    }

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

void sortVerticesByAbsValue(std::vector<vertex_t> &vertices, SubGraph &graph)
{
    using namespace boost;
    auto compare_vertices = [&](const vertex_t &t1, const vertex_t &t2) {
        return get(get(vertex_distance, graph), t1) < get(get(vertex_distance, graph), t2);
    };
    std::sort(vertices.begin(), vertices.end(), compare_vertices);
}

std::map<vertex_t, vertex_t> assignSources(std::vector<vertex_t> &sources, std::vector<vertex_t> &sinks, SubGraph &sourceGraph, SubGraph &sinkGraph)
{
    using namespace boost;
    std::map<vertex_t, vertex_t> sinkMap;

    sortVerticesByAbsValue(sources, sourceGraph);
    sortVerticesByAbsValue(sinks, sinkGraph);

    for (const auto &vert : sources)
    {
        vertex_t sink = sinks[0];

        std::cout << "adding " << get(get(vertex_distance, sourceGraph), vert) << " to " << get(get(vertex_distance, sinkGraph), sink) << "\n";

        put(vertex_distance, sinkGraph, sink, get(get(vertex_distance, sinkGraph), sink) + get(get(vertex_distance, sourceGraph), vert));
        sortVerticesByAbsValue(sinks, sinkGraph);
        sinkMap[vert] = sink;
        //sinkMap.push_back({vert, sink});
    }

    return sinkMap;
}

std::optional<vertex_t> extractFromName(const vertex_t &sub_vertex, SubGraph &subGraph, SubGraph &transGraph)
{
    using namespace boost;
    for (auto [begin, end] = vertices(transGraph); begin != end; begin++)
    {
        if (get(get(vertex_name, subGraph), sub_vertex) == get(get(vertex_name, transGraph), *begin))
        {
            return *begin;
        }
    }
    // optional
    //std::cout << "failed to extract from name" << std::endl;
    return {};
}

std::vector<SubGraph> createSubgraphs(SubGraph &subGraph, SubGraph &globalGraph)
{
    using namespace boost;
    SubGraph sourceGraph{};
    SubGraph sinkGraph{};

    std::vector<vertex_t>
        sourceVertices{};
    std::vector<vertex_t> sinkVertices{};

    for (auto [begin, end] = vertices(subGraph); begin != end; begin++)
    {
        // source
        if (get(get(vertex_root, subGraph), *begin) == 1)
        {
            vertex_t newv = add_vertex(sourceGraph);

            put(vertex_name, sourceGraph, newv, get(get(vertex_name, subGraph), *begin));
            put(vertex_root, sourceGraph, newv, get(get(vertex_root, subGraph), *begin));
            put(vertex_distance, sourceGraph, newv, get(get(vertex_distance, subGraph), *begin));

            sourceVertices.push_back(newv);
        }
        else
        {
            vertex_t newv = add_vertex(sinkGraph);

            put(vertex_name, sinkGraph, newv, get(get(vertex_name, subGraph), *begin));
            put(vertex_root, sinkGraph, newv, get(get(vertex_root, subGraph), *begin));
            put(vertex_distance, sinkGraph, newv, get(get(vertex_distance, subGraph), *begin));

            sinkVertices.push_back(newv);
        }
    }

    for (auto [begin, end] = edges(globalGraph); begin != end; begin++)
    {
        edge_t ed = *begin;
        vertex_t src = source(ed, globalGraph);
        vertex_t dst = target(ed, globalGraph);

        auto sourceVertOpt = extractFromName(src, globalGraph, sourceGraph);
        auto sinkVertOpt = extractFromName(dst, globalGraph, sourceGraph);

        // adding edge to source
        if (sourceVertOpt && sinkVertOpt)
        {
            std::cout << "Adding source edge\n";

            auto [newed, test] = add_edge(*sourceVertOpt, *sinkVertOpt, sourceGraph);
            put(edge_weight, sourceGraph, newed, get(get(edge_weight, globalGraph), ed));
        }

        sourceVertOpt = extractFromName(src, globalGraph, sinkGraph);
        sinkVertOpt = extractFromName(dst, globalGraph, sinkGraph);

        // adding edge to sink
        if (sourceVertOpt && sinkVertOpt)
        {
            std::cout << "Adding sink edge\n";
            auto [newed, test] = add_edge(*sourceVertOpt, *sinkVertOpt, sinkGraph);
            put(edge_weight, sinkGraph, newed, get(get(edge_weight, globalGraph), ed));
        }
    }

    return {subGraph, sourceGraph, sinkGraph};
}

int main()
{
    using namespace boost;
    //graphTest();

    std::fstream fs;
    std::fstream out;

    fs.open(filename, std::fstream::out);
    out.open(outfile, std::fstream::out);

    std::string jsonfile = "./.cache_money/graph.json";

    auto [graphVec, sourceVec, sinkVec] = constructGraph(jsonfile);

    std::map<vertex_t, vertex_t> sinkMap = assignSources(sourceVec, sinkVec, graphVec[1], graphVec[2]);

    std::cout << "Source: " << sourceVec.size() << "\nSink: \n"
              << sinkVec.size();

    json outJson;
    //json outJson = constructRoute(graphVec);
    //std::cout << outJson << std::endl;
    //write_graphviz(fs, MSTsinkGraph);

    SubGraph finalGraph{};
    std::vector<SubGraph> subgraphs{};
    for (int i = 0; i < NUM_TRUCKS; ++i)
    {
        subgraphs.push_back(finalGraph.create_subgraph());
    }

    int i = 0;
    std::vector<vertex_t> finalVertices{};
    for (auto it = sinkMap.begin(); it != sinkMap.end(); it++)
    {
        auto sourceVertOpt = extractFromName(it->first, graphVec[1], graphVec[0]);
        auto sinkVertOpt = extractFromName(it->second, graphVec[2], graphVec[0]);

        if (sourceVertOpt && sinkVertOpt)
        {
            vertex_t sourceVert = *sourceVertOpt;
            vertex_t sinkVert = *sinkVertOpt;
            i++;

            vertex_t newSource = add_vertex(subgraphs[i % NUM_TRUCKS]);
            vertex_t newSink = add_vertex(subgraphs[i % NUM_TRUCKS]);

            // copy data
            put(vertex_name, subgraphs[i % NUM_TRUCKS], newSource, get(get(vertex_name, graphVec[0]), sourceVert));
            put(vertex_root, subgraphs[i % NUM_TRUCKS], newSource, get(get(vertex_root, graphVec[0]), sourceVert));
            put(vertex_distance, subgraphs[i % NUM_TRUCKS], newSource, get(get(vertex_distance, graphVec[0]), sourceVert));

            put(vertex_name, subgraphs[i % NUM_TRUCKS], newSink, get(get(vertex_name, graphVec[0]), sinkVert));
            put(vertex_root, subgraphs[i % NUM_TRUCKS], newSink, get(get(vertex_root, graphVec[0]), sinkVert));
            put(vertex_distance, subgraphs[i % NUM_TRUCKS], newSink, get(get(vertex_distance, graphVec[0]), sinkVert));

            finalVertices.push_back(newSource);
            finalVertices.push_back(newSink);
        }
        else
        {
            std::cout << "error copying\n";
        }
    }
    i = 0;

    for (auto it = sinkMap.begin(); it != sinkMap.end(); it++)
    {

        auto sourceVertOpt = extractFromName(it->first, graphVec[1], graphVec[0]);
        auto sinkVertOpt = extractFromName(it->second, graphVec[2], graphVec[0]);

        if (sourceVertOpt && sinkVertOpt)
        {
            vertex_t sourceVert = *sourceVertOpt;
            vertex_t sinkVert = *sinkVertOpt;
            i++;

            auto [begin, end] = adjacent_vertices(sourceVert, graphVec[0]);
            for (; begin != end; begin++)
            {
                auto vert1 = extractFromName(sourceVert, graphVec[0], subgraphs[i % NUM_TRUCKS]);
                auto vert2 = extractFromName(*begin, graphVec[0], subgraphs[i % NUM_TRUCKS]);

                if (vert1 && vert2)
                {
                    auto [ed, found] = add_edge(*vert1, *vert2, subgraphs[i % NUM_TRUCKS]);

                    auto [olded, found2] = edge(sourceVert, *begin, graphVec[0]);
                    put(edge_weight, subgraphs[i % NUM_TRUCKS], ed, get(get(edge_weight, graphVec[0]), olded));
                }
            }
        }
    }

    for (int i = 0; i < NUM_TRUCKS; ++i)
    {
        std::cout << "TRUCK " << i << " NODES: " << num_vertices(subgraphs[i]) << "\nSTOPS: " << num_edges(subgraphs[i]) << "\n";

        // have overall graph (subgraphs[i]). need individual source and sinks
        auto big_boys = createSubgraphs(subgraphs[i], graphVec[0]);
        std::cout << "SINK SIZE: " << num_vertices(big_boys[2]) << "\nEDGES: " << num_edges(big_boys[2]) << "\n";
        json newOutJson = constructRoute(big_boys);
        std::cout << newOutJson << std::endl;

        outJson[std::to_string(i)] = newOutJson;
    }

    std::cout << "overall stops: " << num_edges(finalGraph) << "\n";

    out
        << outJson;

    write_graphviz(fs, finalGraph);

    fs.close();
    out.close();

    return 0;
}
