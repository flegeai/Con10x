#include <gatb/gatb_core.hpp>

using namespace std;


/********************************************************************************/



class IndexKmers : public Tool
{
	private :
	IBank* bank1;
	IBank* bank2;
	int kmer_size;

	public :
    // Constructor
	IndexKmers();
	void execute ();
};
