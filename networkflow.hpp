#ifndef NETWORKFLOW_HPP
#define NETWORKFLOW_HPP

/////////////////////////////////////////////////////////////////////
/// As a reminder, you are NOT allowed to modify this file.
/// The autograder will use its own version of netflow.hpp.
/////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>


/**
 * Each vertex is represented by a unique unsigned ID.
 */
struct Edge
{
public:
    Edge() = default;
    Edge(unsigned f, unsigned t, unsigned w)
        : from{f}, to{t}, weight{w} { }

    unsigned from;  // ID of vertex this edge is coming out of
    unsigned to;    // ID of vertex this edge is going into
    unsigned weight;
};

/**
 * Takes as argument a list of edges representing a flow network.
 *
 * Returns a list of edges representing a max flow.
 * Does not include any edge whose weight is zero.
 *
 * Throws std::runtime_error if any of the following is true
 * (if multiple of the below error conditions are true, the first
 * error condition's message should be used):
 * - @numVertices < 2.
 *      - Runtime error message: "Too few vertices."
 * - Zero edges.
 *      - Runtime error message: "Too few edges."
 * - There is at least one edge with weight 0.
 *      - Runtime error message: "Detected edge weight of 0."
 * - There is at least one Edge with a @from or @to member
 *    that is >= @numVertices. (Each vertex is supposed to have
 *    a unique ID from 0 to (@numVertices - 1).)
 *      - Runtime error message: "Edge interacts with nonexistent vertex."
 * - There is at least one self-loop.
 *      - Runtime error message: "At least one self-loop."
 * - There is at least one occurrence of multi-edges.
 *      - Runtime error message: "Detected multi-edges."
 * - The flow network does not contain only one source.
 *      - Runtime error message: "Zero or more than one source."
 * - The flow network does not contain only one sink.
 *      - Runtime error message: "Zero or more than one sink."
 */
std::vector<Edge> solveNetworkFlow(
    const std::vector<Edge>& flowNetwork,
    unsigned numVertices);

#endif  // NETFLOW_HPP