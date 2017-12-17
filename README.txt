About
-----
Sumgra is an algorithm for discovering isomorphic embeddings in a multigraph. The current version of Sumgra can handle undirected multigraphs with/without vertex labels. This release works on linux machines.

For more details, check our paper:
Vijay Ingalalli, Dino Ienco, and Pascal Poncelet. "SuMGra: Querying Multigraphs via Efficient Indexing." International Conference on Database and Expert Systems Applications. Springer International Publishing, 2016.

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
1. Sumgra takes two files for dataset, and two files for each query as input
	* Data files: Two data files (edges.txt and nodes.txt) are to be maintained to successfully run sumgra.
	* Query files: Each query is represented by two files; 'i_nodes.txt' and 'i_edges.txt', where 'i' is the query id
	* A file 'time.txt' is automatically created in the 'result folder path' to save the results.

2. Format for *edges.txt: [ node1 node2 multiedge], where a multiedge is represented by comma separated edge-ids.

3. Format for *nodes.txt: [ -1 ], for multigraphs without vertex labels, where each vertex is to be labeled as '-1';
		       [ v_labels ], for multigraphs with vertex labels, where 'v_labels' is represented by comma separated vertex labels

4. A sample data, query and result files are available at '../sumgra/dataset/sample/' for multigraphs without vertex labels
5. A sample data, query and result files are available at '../sumgra/dataset/sample_vtx_labels/' for multigraphs with vertex labels

NB: For a vertex labeled multigraph, any unlabeled vertex (in both data and query files) has to be labeled as '-1'

***
