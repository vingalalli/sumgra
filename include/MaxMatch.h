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

#ifndef MAXMATCH_H
#define MAXMATCH_H

const int MAXN1 = 500000;
const int MAXN2 = 500000;
const int MAXM = 1000000;	
		
#include <array>
using namespace std;

class MaxMatch
{
    public:
        MaxMatch();
        virtual ~MaxMatch();
		void init(int _n1, int _n2);
		void addEdge(int u, int v);
		int maxMatching();         
    protected:           				
    private:
		void bfs();
		bool dfs(int u1);
};

#endif // MAXMATCH_H
