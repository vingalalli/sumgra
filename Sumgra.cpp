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

#include "include/FileManager.h"
#include "include/IndexManager.h"
#include "include/SubgraphMatcher.h"

int main(int argc, char* argv[])
{
		std::string dataPath = argv[1];
		std::string queryPath = argv[2];
		std::string resultPath = argv[3];
		std::string nQueries = argv[4];
		std::string printEmb = argv[5];

	  /// Read data graph
	  cout << "Reading data files..." << endl;
	  std::string nodeFile = dataPath + "nodes.txt";
	  std::string edgeFile = dataPath + "edges.txt";
	  GraphParameter dataGraph;
	  FileManager dataGraphInfo;
	  dataGraphInfo.readContents(nodeFile, edgeFile, dataGraph);

	  /// Build data graph indexes
	  cout << "Building indexes..." << endl;
	  IndexManager graphDB;
	  IndexType graphIndexes;
	  TimeEval timeEval;
		clock_t start = clock();
	  graphDB.buildIndexes(dataGraph, graphIndexes);
		timeEval.buildIndex = double(clock()- start)/CLOCKS_PER_SEC;

		/// Query processing
	  int qN = std::stoi(nQueries), i = 0;
		std::string timeFile = resultPath + "time.txt";
		FileManager writeFile;
		writeFile.createNew(timeFile);
		cout << "Reading query files..." << endl;
	  while(i < qN){
		  	/// Read query graph
				std::string nodeFile = queryPath + std::to_string(i) + "_nodes.txt";
				std::string edgeFile = queryPath + std::to_string(i) + "_edges.txt";
				GraphParameter queryGraph;
				FileManager queryGraphInfo;
				queryGraphInfo.readContents(nodeFile, edgeFile, queryGraph);

				/// Perfrom subgraph matching
				SubgraphMatcher subGraph;
				start = clock();
				subGraph.findEmbeddings(queryGraph, graphIndexes);
				timeEval.queryMatch =  double(clock()- start)/CLOCKS_PER_SEC;
		    cout << i << ": " << queryGraph.embeddings.size() <<  " embeddings found" << endl;
		    //cout << "Time: " << timeEval.queryMatch << endl;

				/// Collect results
				writeFile.printTime(timeEval, timeFile, queryGraph, i); // #embeddings, query time consumption
				if(printEmb == "yes"){ // Print embeddings
					std::string embFile = resultPath + std::to_string(i) + "_emb.txt";
					writeFile.printEmbeddings(queryGraph, embFile);
				}
				++i;
		}
		return 0;
}
