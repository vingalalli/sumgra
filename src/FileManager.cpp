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

#include "FileManager.h"

FileManager::FileManager()
{
    //ctor
}

FileManager::~FileManager()
{
    //dtor
}

void FileManager::SplitString(const std::string& str, char chr, std::vector<std::string>& strs)
{
    std::string::const_iterator first = str.cbegin();
    std::string::const_iterator second = std::find(first+1, str.cend(), chr);

    while(second != str.cend()) {
        strs.emplace_back(first, second);
        first = second+1;
        second = std::find(second+1, str.cend(), chr);
    }
    strs.emplace_back(first, str.cend());
}

void FileManager::ReadContents(const std::string& nodeFile, const std::string& edgeFile, GraphParameter& graphInfo)
{
    /// Reading node file
    graphInfo.nodeLabelsExist = false; // As of now, node labels are not considered
    const char * nF = nodeFile.c_str();
    std::ifstream nFile (nF);
    if (nFile.is_open()) {
        std::string nodeAtt;
        while (getline(nFile, nodeAtt)) {
            std::vector<std::string> nodeAttAll;
            SplitString(nodeAtt, ',', nodeAttAll);
            std::set<int> setAtt;
            for (size_t i = 0; i < nodeAttAll.size(); ++i)
                setAtt.insert(stoi(nodeAttAll[i]));
            graphInfo.attributes.push_back(setAtt);
        }
        nFile.close();
    }
    else {
    	cout << "Unable to open *node file" << endl;
    	exit(0);
    }
    graphInfo.nNodes = graphInfo.attributes.size();

    /// Reading node file
    const char * eF = edgeFile.c_str();
    std::ifstream eFile (eF);
    std::map<std::string, int> node_m; // < original_id, mapped_id >

    if (eFile.is_open()) {
        std::string multiEdge;
        int n = 0;
        std::vector<std::vector<int>> adjListTemp(graphInfo.nNodes);
        EdgeLabel edgeLabels(graphInfo.nNodes);
        while (getline(eFile, multiEdge)) {
            std::vector<std::string> edgeContent;
            SplitString(multiEdge, ' ', edgeContent);
            int node1; int node2;
            auto it = node_m.find(edgeContent.at(0));
            if (it == node_m.end()) {
              node_m.insert(make_pair(edgeContent.at(0), n));
              node1 = n;
              ++n;
            }
            else
              node1 = it->second;
            it = node_m.find(edgeContent.at(1));
            if (it == node_m.end()) {
              node_m.insert(make_pair(edgeContent.at(1), n));
              node2 = n;
              ++n;
            }
            else
              node2 = it->second;

            graphInfo.edges[0].push_back(node1);
            graphInfo.edges[1].push_back(node2);

            std::vector<std::string> labels;
            SplitString(edgeContent.at(2), ',', labels);
            std::set<int> temp;
            for(auto it = labels.begin(); it != labels.end(); ++it)
                temp.insert(stoi((*it)));

            edgeLabels[node1].push_back(temp);
            edgeLabels[node2].push_back(temp);

            std::pair<int, int> p_1 = std::make_pair(node1,node2);
            auto it_1 = graphInfo.eLabelMap.find(p_1);
            if (it_1 == graphInfo.eLabelMap.end())
                graphInfo.eLabelMap.insert(std::make_pair(p_1, temp));
            else
                for (auto it = temp.begin(); it != temp.end(); ++it)
                    it_1->second.insert(*it);
            std::pair<int, int> p_2 = std::make_pair(node2,node1);
            auto it_2 = graphInfo.eLabelMap.find(p_2);
            if (it_2 == graphInfo.eLabelMap.end())
                graphInfo.eLabelMap.insert(std::make_pair(p_2, temp));
            else
                for (auto it = temp.begin(); it != temp.end(); ++it)
                    it_2->second.insert(*it);

            if (find(adjListTemp[node1].begin(), adjListTemp[node1].end(), node2) == adjListTemp[node1].end())
              adjListTemp[node1].push_back(node2);
            if (find(adjListTemp[node2].begin(), adjListTemp[node2].end(), node1) == adjListTemp[node2].end())
              adjListTemp[node2].push_back(node1);

        }
        graphInfo.adjacencyList = adjListTemp;
        graphInfo.neighbourSign = edgeLabels;
    }
    else {
        cout << "Unable to open *edge file" << endl;
        exit(0);
    }
}

void FileManager::createNew(const std::string& timeFile)
{
    const char * tF = timeFile.c_str();
  	if( remove(tF) != 0 )
		  perror( "Creating new file" );
    ofstream outFile;
    outFile.open(timeFile, std::ios_base::app);
    outFile << "q_id\t#emb\tq_time(secs)" <<  endl;
}

void FileManager::printTime(TimeEval& timeEval, std::string& timeFile, const GraphParameter& queryGraph, int i)
{
    ofstream outFile;
    outFile.open(timeFile, std::ios_base::app);
    if(queryGraph.timedOut)
		  outFile << i << "\t" << "-" << "\t" << ">60" << endl;
    else
    	outFile << i << "\t" << queryGraph.embeddings.size() << "\t" << timeEval.queryMatch << endl;	// Time in seconds
}

void FileManager::printEmbeddings(const GraphParameter& queryGraph, std::string& embFile)
{
  if(queryGraph.timedOut){
		cout << "Query timed out. No embeddings to print!" << endl;
		return;
	}

	ofstream outFile;
	outFile.open(embFile);
    for (size_t j = 0; j < queryGraph.orderedNodes.size(); ++j)
    	outFile << queryGraph.orderedNodes[j] << "\t" ;
    outFile << endl;
	for (size_t i = 0; i < queryGraph.embeddings.size(); ++i) {
        for (size_t j = 0; j < queryGraph.orderedNodes.size(); ++j)
            outFile << queryGraph.embeddings[i][j] << "\t";
        outFile << endl;
	}
	outFile.close();
}
