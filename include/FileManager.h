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

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <functional>
#include <sstream>
#include <set>
#include <unordered_set>
#include <string>
#include <cstdio>
#include <array>
#include <stdio.h>
#include <stdlib.h>
#include <bitset>

#define SYN_SIZE 6 // Number of fields in the Synopses vector
#define DIM 700 // Expected maximum number of dimensions in the data set

using namespace std;
using namespace std::placeholders;

typedef std::map< std::pair<int, int>, std::set<int>> EdgeLabelMap;
typedef std::map< std::pair<int, int>, std::bitset<DIM>> BitLabelMap;
typedef std::vector<int> IndexVector;
typedef std::vector<IndexVector> Vector2D;
typedef std::vector<std::string> VecOfStr;
typedef std::vector<std::set<int>> VecOfSet;
typedef std::vector<std::vector<std::set<int>>> EdgeLabel;
typedef std::vector<std::vector<std::bitset<DIM>>> EdgeLabelBit;
typedef std::unordered_map<int, std::set<int>> AttMap;

struct GraphParameter {
    int nNodes;
    VecOfSet attributes;
    bool nodeLabelsExist;
    GraphParameter() : edges(2) {}
    Vector2D edges;
    EdgeLabel neighbourSign;
    Vector2D adjacencyList;
    EdgeLabelMap eLabelMap;
    EdgeLabelBit bitSetMap;
    std::vector<int> orderedNodes;
    std::vector<int> inNodes;  // node_id in the order they appear in 'edges.txt', necessary for outuputting the solutions by reverse mapping.
    std::vector<int> sumgraNodes; // sumgra_id that correspond to he natural order of inNodes, necessary for building attribute index.
    Vector2D embeddings;
    bool timedOut = false;
};

struct TimeEval{
    double buildIndex = 0;
    double queryMatch = 0;
    double fileReading = 0;
};

template <typename T>
vector<T> sortIndex(const vector<T> &v) {
    vector<T> idx(v.size());
    for (int i = 0; i != idx.size(); ++i)
        idx[i] = i;
    sort(idx.begin(), idx.end(), [&v](int i1, int i2) {return v[i1] > v[i2];});
    return idx;
}

template <typename T>
std::vector<T> setMinus(std::vector<T> a, std::vector<T> b)
{
    std::vector<T> c(a.size()+b.size());
    typename std::vector<T>::iterator it;

    std::sort (a.begin(), a.end());
    std::sort (b.begin(), b.end());

    it=std::set_difference (a.begin(), a.end(), b.begin(), b.end(), c.begin());
    c.resize(it-c.begin());
    return c;
}


template <typename T>
std::vector<T> setUnion(std::vector<T> a, std::vector<T> b)
{
    std::vector<T> c(a.size()+b.size());
    typename std::vector<T>::iterator it;

    std::sort (a.begin(), a.end());
    std::sort (b.begin(), b.end());

    it=std::set_union (a.begin(), a.end(), b.begin(), b.end(), c.begin());
    c.resize(it-c.begin());
    return c;
}


template <typename T>
std::vector<T> setIntersection(std::vector<T> a, std::vector<T> b)
{
    std::vector<T> c(a.size()+b.size());
    typename std::vector<T>::iterator it;

    std::sort (a.begin(), a.end());
    std::sort (b.begin(), b.end());

    it=std::set_intersection (a.begin(), a.end(), b.begin(), b.end(), c.begin());
    c.resize(it-c.begin());
    return c;
}

class FileManager
{
    public:
        FileManager();
        virtual ~FileManager();
        void readContents(const std::string& nodeFile, const std::string& edgeFile, GraphParameter& graphInfo);
        void printTime(TimeEval& timeEval, std::string& timeFile, const GraphParameter& queryGraph, int i);
        void createNew(const std::string& timeFile);
        void printEmbeddings(const GraphParameter& queryGraph, const std::vector<int>& dataInNodes, std::string& embFile);
    protected:
        void splitString(const std::string& str, char chr, std::vector<std::string>& strs);
};


#endif // FILEMANAGER_H
