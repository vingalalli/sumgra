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

#ifndef INDEXMANAGER_H
#define INDEXMANAGER_H

#include "FileManager.h"
#include "Trie.h"
#include "rtree/RStarTree.h"

typedef RStarTree<int, SYN_SIZE, 32, 64> RTree;
typedef RTree::BoundingBox BoundingBox;

struct IndexType{
    AttMap attributeHash;
    RTree synopsesTrie;
    std::vector<Trie*> neighborTrie;
//    IndexType() : bitSetMap(dataGraphInfo.nNodes) {}
    EdgeLabelBit bitSetMap;
};

struct Visitor {
	int cnt;
	bool ContinueVisiting;
	std::vector<int> edgeIndices;
	Visitor() : cnt(0), ContinueVisiting(true) {};

	void operator()(const RTree::Leaf * const leaf)
	{
		edgeIndices.push_back(leaf->leaf);
	}
};


class IndexManager
{
    public:
        IndexManager();
        virtual ~IndexManager();
		void BuildIndexes(GraphParameter& dataGraphInfo, IndexType& indexType);        
        void BuildAttHash(const VecOfSet& attSign, AttMap& attributeHash);
        void QueryAttHash(const VecOfSet& queryAtt, const AttMap& attributeHash, VecOfSet&  attMatches);
        void CreateSynopses(const std::vector<std::set<int>>& signature, std::vector<short>& synopses);
        void SortSignature(EdgeLabel& neighbourSign, std::vector<int>& sortedNodes, const int& elements);
        void BuildSynTrie(const EdgeLabel& dataSignature, const int& dataNodes, RTree& synopsesTrie);
        void QuerySynTrie(const std::vector<std::set<int>>& initSignature, RTree& synopsesTrie, std::vector<int>& initialMatches);
        void BuildNeighTrie(const Vector2D& adjacencyList, const EdgeLabelMap& eLabelMap, const int& dataNodes, std::vector<Trie*>& nbrIndex);
        void QueryNeighTrie(Trie* t, const std::set<int>& multi_e, std::vector<int>& MatchedIds);
        void BuildBitSign(const EdgeLabel& dataSignature, const int& dataNodes, EdgeLabelBit& dataBitSet);
        
    protected:
    private:
        BoundingBox bounds(std::vector<short> synopses);
};

#endif // INDEXMANAGER_H
