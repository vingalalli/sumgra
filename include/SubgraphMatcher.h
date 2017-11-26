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

#ifndef SUBGRAPHMATCHER_H
#define SUBGRAPHMATCHER_H

#include "FileManager.h"
#include "IndexManager.h"

const int MAX_EMB = 1000000; // The maximum number of embeddings to be found by the algorithm
const int MAX_ALLOWED_TIME = 60000000; // 60 seconds (60*CLOCKS_PER_SEC)

class SubgraphMatcher
{
    public:
        SubgraphMatcher();
        virtual ~SubgraphMatcher();
		void FindEmbeddings(GraphParameter& queryGraph, IndexType& graphIndexes);
		void OrderVertices(const int& queryNodes, const EdgeLabel& queryNeighbourSign, const Vector2D& queryAdjacencyList, std::vector<int>& querySequence);		
		void FindMatches(const std::vector<int>& initialMatches, GraphParameter& queryGraph, const VecOfSet& nodeMatches, IndexType& graphIndexes);
		void SubgraphSearch(GraphParameter& queryGraph, IndexType& graphIndexes, const int& p_m, const VecOfSet& nodeMatches, const Vector2D& matchedQueryNeighbours, const std::vector<int>& matchedDataVertices, Vector2D& exact_stack);
    protected:
    private:
};

#endif // SUBGRAPHMATCHER_H
