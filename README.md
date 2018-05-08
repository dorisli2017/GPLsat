# GPLsat
Hypergraph partion based local search of SAT problem

Pseodocode:
1.Input: CNF Formula
2.Graph construction (Hypergraph/ Dual Hypergraph/ None for small instances)
3.Graph Partition (hMetis/ CFPM+FM/others)->see Literature (Hypergraph)
4.Initial solution (Hard way(give cut a permenant satisfing initialization) 
                    /soft way (control priority to lead up satisfy the cut first and recognize the status of cut variables in process)
                    )
5.Parallel solver (Genetic combination using graph property(distence/independences)
6.probalistische local search for one agent

todo: 1 2 3 4 5 
doing: 6 (05.08-05.13) References (probSat & lazyWalker)
