#include "graphiso.h"

#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

// ================= Complete - Begin Graph class implementation ===================
Graph::Graph(std::istream& istr) {
    string aline;
    while (getline(istr, aline)) {
        istringstream iss(aline);
        string u, v;
        if (iss >> u) {
            VERTEX_SET_T neighbors;
            while (iss >> v) {
                neighbors.insert(v);
            }
            adj_.insert(std::make_pair(u, neighbors));
        }
    }
}

bool Graph::edgeExists(const VERTEX_T& u, const VERTEX_T& v) const {
    if ((adj_.find(u) != adj_.end()) && (adj_.at(u).find(v) != adj_.at(u).end())) {
        return true;
    }
    return false;
}
const VERTEX_SET_T& Graph::neighbors(const VERTEX_T& v) const {
    if (adj_.find(v) == adj_.end()) {
        throw std::invalid_argument("Neighbors: invalid vertex - not in  map");
    }
    return adj_.at(v);
}
VERTEX_LIST_T Graph::vertices() const {
    VERTEX_LIST_T verts;
    for (const auto& p : adj_) {
        verts.push_back(p.first);
    }
    return verts;
}
// ================= Complete - End Graph class implementation ===================

// Prototype and helper functions here

// To be completed
bool isConsistent(const Graph& g1, const Graph& g2, VERTEX_ID_MAP_T& mapping) {
    // Feel free to change or remove this starter code

    VERTEX_LIST_T g1verts = g1.vertices();
    for (const auto& g1u : g1verts) {
        if (mapping.find(g1u) != nullptr) {
            VERTEX_SET_T g1n = g1.neighbors(mapping.find(g1u)->first);
            VERTEX_T g2u = mapping.find(g1u)->second;
            if ((g1.neighbors(mapping.find(g1u)->first).size())
                != (g2.neighbors(mapping.find(g1u)->second).size())) {  // graphs are differetn degrees
                return false;
            }
            // cout << mapping.find(g1u)->first << ":" << mapping.find(g1u)->second << endl;
            // iter through g1 neighbors
            VERTEX_SET_T::const_iterator itr;
            for (itr = g1n.begin(); itr != g1n.end(); itr++) {
                if (mapping.find(*itr) != nullptr && mapping.find(g1u) != nullptr) {
                    // element exists
                    VERTEX_T g2Node = mapping.find(*itr)->second;
                    if (!g2.edgeExists(g2u, g2Node)) {
                        // Different degrees
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

// Add any helper functions you deem useful
bool graphIsoHelper(VERTEX_SET_T& G1, VERTEX_SET_T& G2, const Graph& g1, const Graph& g2, VERTEX_ID_MAP_T& mapping);

// To be completed
bool graphIso(const Graph& g1, const Graph& g2, VERTEX_ID_MAP_T& mapping) {
    VERTEX_LIST_T g1verts = g1.vertices();
    if (g1verts.size() != g2.vertices().size()) {
        return false;  // means graphs are of different size
    }
    // Add code here
    VERTEX_SET_T G1;
    VERTEX_SET_T G2;

    return graphIsoHelper(G1, G2, g1, g2, mapping);
}

bool graphIsoHelper(VERTEX_SET_T& G1, VERTEX_SET_T& G2, const Graph& g1, const Graph& g2, VERTEX_ID_MAP_T& mapping) {
    VERTEX_LIST_T g1verts = g1.vertices();
    VERTEX_LIST_T g2verts = g2.vertices();

    for (const auto& g1u : g1verts) {  // iter. through all the g1 vertices
        VERTEX_SET_T ::iterator it1 = std::find(G1.begin(), G1.end(), g1u);
        if (it1 == G1.end()) {
            for (const auto& g2u : g2verts) {  // iter. through all the g2 vertices
                VERTEX_SET_T ::iterator it2 = std::find(G2.begin(), G2.end(), g2u);
                if (it2 == G2.end()) {
                    // insert g1u, g2u
                    mapping.insert(make_pair(g1u, g2u));
                    G1.insert(g1u);
                    G2.insert(g2u);
                    // check for consistency
                    if (isConsistent(g1, g2, mapping)) {
                        if (graphIsoHelper(G1, G2, g1, g2, mapping)) {  // recurse to the next
                            return true;
                        } else {
                            // backtrack
                            mapping.remove(g1u);
                            G1.erase(g1u);
                            G2.erase(g2u);
                        }

                    } else {
                        // if not consistent backtrack
                        mapping.remove(g1u);
                        G1.erase(g1u);
                        G2.erase(g2u);
                    }
                }
            }
            if (mapping.find(g1u) == nullptr) {
                return false;
            }
        }
    }
    if (!isConsistent(g1, g2, mapping)) {  // vertices are not consistent || mapped
        return false;
    } else {
        return true;
    }
}
