//! [snippet1]

// We include what we need for the test
#include <gatb/gatb_core.hpp>

#undef NDEBUG
#include <cassert>

/********************************************************************************/
/*                          Node neighbors management                           */
/*                                                                              */
/* Cmd-line: debruijn10 (takes no argument)                                     */
/*                                                                              */
/********************************************************************************/
int main (int argc, char* argv[])
{
    try
    {
        // We create the graph with a bank holding one sequence, and use a specific kmer size and solid kmer abundance to 1
        Graph graph = Graph::create (new BankStrings ("AATGC", NULL), "-kmer-size 4  -abundance-min 1  -verbose 0");

        // We get an iterator for all nodes of the graph.
        GraphIterator<Node> it = graph.iterator ();

        // We check that we have only two possible nodes
        assert (it.size() == 2);

        // We loop each node.
        for (it.first(); !it.isDone(); it.next())
        {
            // A shortcut.
            Node& current = it.item();

            // We get the ascii representation of the current iterated node
            std::string s = graph.toString (current);

            // We check that it is correct.
            assert (s=="AATG" || s=="ATGC");

            if (s=="AATG")
            {
                // We get the neighbors of this specific current
                // Note that we get here the full edge information
                GraphVector<Edge> neighbors = graph.successorsEdge (current);

                // We check that we got only one successor
                assert (neighbors.size() == 1);

                // We check that the target node is ok
                assert (graph.toString(neighbors[0].to) == "ATGC");

                // We check that the transition nucleotide is ok
                assert (neighbors[0].nt == NUCL_C);
            }
        }

        std::cout << "Test OK" << std::endl;
    }
    catch (Exception& e)
    {
        std::cerr << "EXCEPTION: " << e.getMessage() << std::endl;
    }

    return EXIT_SUCCESS;
}
//! [snippet1]
