About
-----
The current version of Sumgra works for undirected multigraphs without any vertex labels. This release works on linux machines.


I. Running sumgra
-----------------
1. If you have a working shell, and make, run the command 'make' from the project path '../sumgra/' to compile Sumgra for your system.
2. The following command will run Sumgra
	./sumgra <arg1> <arg2> <arg3> <arg4> <arg5>
where,
<arg1> = [ data folder path ] (e.g., '../sumgra/dataset/sample/data/')
<arg2> = [ query folder path ] (e.g., '../sumgra/dataset/sample/queries/')
<arg3> = [ result folder path ] (e.g., '../sumgra/dataset/sample/result/')
<arg4> = [ No. of queries ] (e.g., <arg4> = 10, indicates 10 queries)
<arg5> = [yes | no] tells Sumgra if user wants to print all the embeddings


II. Parameter settings
----------------------
The following global constant parameters are set in 'SubgraphMatcher.h' file. 
	1. MAX_EMB is the allowed maximum number of embeddings of a query that can be discovered by Sumgra.
	2. MAX_ALLOWED_TIME is the maximum time allowed for a query to discover embeddings. 


III. File formats
-----------------
Sumgra takes two files for dataset, and two files for each query as input
	1. Data files: Two data files (edges.txt and nodes.txt) are to be maintained to successfully run sumgra.
	2. Query files: Each query is represented by two files; 'i_nodes.txt' and 'i_edges.txt', where 'i' is the query id
	3. A file 'time.txt' is automatically created in the 'result folder path' to save the results.
Format for *edges.txt: [ node1 node2 multiedge], where a multiedge is represented by comma separated edge-ids.
Format for *nodes.txt: [ 1 ]. For unlabeled graphs, each vertex is represented by an integer '1'

A sample data, query and result files are available at '../sumgra/dataset/sample/'

***
