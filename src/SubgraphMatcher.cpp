/*
 *  Copyright (c) 2015 Vijay Ingalalli
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "SubgraphMatcher.h"
#include "MaxMatch.h"

SubgraphMatcher::SubgraphMatcher()
{
    //ctor
}

SubgraphMatcher::~SubgraphMatcher()
{
    //dtor
}


bool bitSetMatch(const std::vector<std::bitset<DIM>>& dataBitSet, const std::vector<std::bitset<DIM>>& queryBitSet)
{
	MaxMatch maxMatch;
    bool matched = false;
    maxMatch.init(queryBitSet.size(), dataBitSet.size()); // Initialize bipartite graph |
    bool itemFound = false;
    int r = 0;
    for(auto qIt = queryBitSet.begin(); qIt != queryBitSet.end(); ++qIt) {
        bool notFound = true;
        int c = 0;
        for(auto dIt = dataBitSet.begin(); dIt != dataBitSet.end(); ++dIt) {
                if (((*qIt) & ~(*dIt)).none()) {
                    notFound = false;
                    itemFound = true;
                    maxMatch.addEdge(r, c); // a connecting edge
                }
            ++c;
        }
        if(notFound) // checks if an item of query signature is not found in the entire data vertex; skip further checks.
            break;
        ++r;
    }
    if(itemFound)
        if(maxMatch.maxMatching() == queryBitSet.size())
            matched = true;
    return matched;
}


void chooseFrontier(const std::vector<int>& already_m, const Vector2D& queryAdjacencyList, std::vector<int>& frontier)
{
    for (size_t i = 0; i < already_m.size(); ++i) {
        if (i == 0)
            frontier = queryAdjacencyList[already_m[i]];
        else
            frontier = setUnion(frontier, queryAdjacencyList[already_m[i]]);
    }
    frontier = setMinus(frontier, already_m);
    return;
}

void SubgraphMatcher::orderVertices(const int& queryNodes, const EdgeLabel& queryNeighbourSign, const Vector2D& queryAdjacencyList, std::vector<int>& querySequence)
{
    std::vector<int> signRank;
    for (size_t i = 0; i < queryNodes; ++i) {
        int allDim = 0;
        for (int j = 0; j < queryNeighbourSign[i].size(); ++j)
            allDim += queryNeighbourSign[i][j].size();
        signRank.push_back(allDim);
    }


/// Using only neighbourhood signature size |
    querySequence.push_back(sortIndex(signRank)[0]); //; Decreasing order of signRank
    while (querySequence.size() != queryNodes) {
        std::vector<int> frontier;
        chooseFrontier(querySequence, queryAdjacencyList, frontier);
        if (frontier.size() == 1)
            querySequence.push_back(frontier[0]);
        else {
            std::vector<int> rank_1(frontier.size());

            for (size_t i = 0; i < frontier.size(); ++i)
                rank_1[i] = 0;
            std::vector<int> srt_rank_1 = sortIndex(rank_1);

/// Check for 2 constraints;
//
            if (rank_1[srt_rank_1[0]] != rank_1[srt_rank_1[1]])
                querySequence.push_back(frontier[srt_rank_1[0]]);
            else {
                std::vector<int> same_rank_1;
                same_rank_1.push_back(frontier[srt_rank_1[0]]);
                int i = 0;
                while (i < frontier.size()-1){
                    if (rank_1[srt_rank_1[i]] == rank_1[srt_rank_1[i+1]])
                        same_rank_1.push_back(frontier[srt_rank_1[i+1]]);
                    else
                        break;
                    ++i;
                }
                std::vector<int> rank_2(same_rank_1.size());
                for (size_t i = 0; i < same_rank_1.size(); ++i)
                    rank_2[i] = signRank[same_rank_1[i]];
                std::vector<int> srt_rank_2 = sortIndex(rank_2);
                querySequence.push_back(same_rank_1[srt_rank_2[0]]);
            }
//
        }
    }
    int initialVertex = querySequence[0];
}


void SubgraphMatcher::findMatches(const std::vector<int>& initialMatches, GraphParameter& queryGraph, const VecOfSet& nodeMatches, IndexType& graphIndexes)
{
    Vector2D matchedQueryNeighbours(queryGraph.nNodes-1);
    std::vector<int> q_v;
    q_v.push_back(queryGraph.orderedNodes[0]);
    for(size_t i = 0; i < queryGraph.nNodes-1; ++i) {
        int nextVertex = queryGraph.orderedNodes[i+1];
        matchedQueryNeighbours[i] = setIntersection(queryGraph.adjacencyList[nextVertex], q_v);
        q_v.push_back(nextVertex);
    }

    std::vector<int> matchedDataVertices(queryGraph.nNodes);
    clock_t stopTime = clock();
    /// Iterative Approach for Subgraph Isomorphic Matching
    for (auto it = initialMatches.begin(); it != initialMatches.end(); ++it) {
        int p_m = 0;
        matchedDataVertices[p_m] = (*it);
        ++p_m;
        Vector2D exactStack(queryGraph.nNodes-1);
        bool incr = true;
        while (p_m != 0 ) {
            if (p_m == queryGraph.orderedNodes.size()) {
                queryGraph.embeddings.push_back(matchedDataVertices);
                --p_m;
                incr = false;
                if (queryGraph.embeddings.size() >= MAX_EMB)
                    break;
            }
            else {
                if (incr){
                    subgraphSearch(queryGraph, graphIndexes, p_m, nodeMatches, matchedQueryNeighbours, matchedDataVertices, exactStack);
		            if (double(clock() - stopTime) > MAX_ALLOWED_TIME) {
		                queryGraph.timedOut = true;
		                break;
		            }
                }

            }

            if (!exactStack[p_m-1].empty()) {
                matchedDataVertices[p_m] = exactStack[p_m-1].back();
                exactStack[p_m-1].pop_back();
                ++p_m;
                incr = true;
            }
            else {
                --p_m;
                incr = false;
            }
        }
        if (queryGraph.embeddings.size() >= MAX_EMB || queryGraph.timedOut)
            break;
    }
}


void SubgraphMatcher::subgraphSearch(GraphParameter& queryGraph, IndexType& graphIndexes, const int& p_m, const VecOfSet& nodeMatches, const Vector2D& matchedQueryNeighbours, const std::vector<int>& matchedDataVertices, Vector2D& exact_stack)
{
    IndexManager dataIndex;
    int nextVertex = queryGraph.orderedNodes[p_m];
    std::vector<int> matchedDataNeighbours(matchedQueryNeighbours[p_m-1].size());

    for(size_t i = 0; i < matchedDataNeighbours.size(); ++i)
        matchedDataNeighbours[i] = matchedDataVertices[find(queryGraph.orderedNodes.begin(), queryGraph.orderedNodes.end(), matchedQueryNeighbours[p_m-1][i]) - queryGraph.orderedNodes.begin()];

    bool allNbrsMatched = true;
    std::vector<int> edgeMatches;
    for(size_t i = 0; i < matchedQueryNeighbours[p_m-1].size(); ++i) {
        auto query_it = queryGraph.eLabelMap.find(std::make_pair(matchedQueryNeighbours[p_m-1][i], nextVertex));
        std::vector<int> nbrLblMatches;
        dataIndex.queryNeighTrie(graphIndexes.neighborTrie[matchedDataNeighbours[i]], query_it->second, nbrLblMatches);
        if (!nbrLblMatches.empty()){
            if (i == 0) {
                edgeMatches = nbrLblMatches;
            }
            else {
                std::unordered_set<int> s(edgeMatches.begin(), edgeMatches.end()); // perform intersection
                edgeMatches.clear();
                for (auto it = nbrLblMatches.begin(); it != nbrLblMatches.end(); ++it) {
                    if (s.find((*it)) != s.end())
                        edgeMatches.emplace_back((*it));
                }
            }
        }
        else{
            allNbrsMatched = false;
            break;
        }
    }

    if (allNbrsMatched) {
        std::vector<int> exactMatches;
        if (!nodeMatches[nextVertex].empty()) { // Find the common elements between edge labels and node labels |
            for (size_t i = 0; i < edgeMatches.size(); ++i){
                if (nodeMatches[nextVertex].find(edgeMatches[i]) != nodeMatches[nextVertex].end())
                    exactMatches.push_back(edgeMatches[i]);
            }
        }
        else
            exactMatches=edgeMatches;

        for(auto it_e = exactMatches.begin(); it_e != exactMatches.end(); ++it_e) {
            auto dataEnd_it = matchedDataVertices.begin()+p_m;
            if (find(matchedDataVertices.begin(), dataEnd_it, (*it_e)) == dataEnd_it)
	             //if (bitSetMatch(graphIndexes.bitSetMap[(*it_e)], queryGraph.bitSetMap[nextVertex]))
                    exact_stack[p_m-1].emplace_back((*it_e));
        }
    }
}


void SubgraphMatcher::findEmbeddings(GraphParameter& queryGraph, IndexType& graphIndexes)
{
    SubgraphMatcher subGraph;
    subGraph.orderVertices(queryGraph.nNodes, queryGraph.neighbourSign, queryGraph.adjacencyList, queryGraph.orderedNodes);

    /// Fetch the vertex attribute solutions for all the query vertices to be used when necessary.
		IndexManager index;
    VecOfSet nodeMatches(queryGraph.nNodes);	// maintains matched lables in the natural order, not query order.
		index.queryAttHash(queryGraph.attributes, graphIndexes.attributeHash, nodeMatches);
		// queryGraph.bitSetMap.resize(queryGraph.nNodes);
    // index.buildBitSign(queryGraph.neighbourSign, queryGraph.nNodes, queryGraph.bitSetMap);

    // Find matches for the initial node
    std::vector<int> initialEdgeMatches;
    int initialVertex = queryGraph.orderedNodes[0];
    if (!queryGraph.neighbourSign[initialVertex].empty())
        index.querySynTrie(queryGraph.neighbourSign[initialVertex], graphIndexes.synopsesTrie, initialEdgeMatches);

		std::vector<int> initialMatches;
    if (!nodeMatches[initialVertex].empty()) { // Filter the 'initialEdgeMatches' with node label constraints |
        for (size_t i = 0; i < initialEdgeMatches.size(); ++i){
            if (nodeMatches[initialVertex].find(initialEdgeMatches[i]) != nodeMatches[initialVertex].end())
                initialMatches.push_back(initialEdgeMatches[i]);
        }
    }
    else
        initialMatches=initialEdgeMatches;

    /// Find all the  embeddings bounded by MAX_EMB, and then discovered the frequent patterns.
    if (!initialMatches.empty())
        subGraph.findMatches(initialMatches, queryGraph, nodeMatches, graphIndexes);

}
