#include <gatb/gatb_core.hpp>
#include <IndexKmers.hpp>

// We define some constant strings for names of command line parameters
static const char* STR_BANK1 = "-one";
static const char* STR_BANK2 = "-two";
static const char* KMER_SIZE = "-kmer_size";


IndexKmers::IndexKmers () : Tool("IndexKmers") {
		getParser()->push_front (
      new OptionOneParam (
        STR_BANK1, //argument name
        "reads bank 1", //argument short help message
        true //mandatory?
      ));
    getParser()->push_front (
      new OptionOneParam (
        STR_BANK2,
        "reads bank 2",
        true
      ));
		getParser()->push_front (
	      new OptionOneParam (
	        KMER_SIZE,
	        "kmer size",
	        true
	      ));
};

void IndexKmers::execute () {
		bank1 = Bank::open (getInput()->getStr(STR_BANK1));  LOCAL (bank1);
		bank2 = Bank::open (getInput()->getStr(STR_BANK2));  LOCAL (bank2);
		kmer_size = getInput()->getInt(KMER_SIZE);
}


int main (int argc, char* argv[])
{
  // We use a try/catch block since GATB functions may throw exceptions
  try
  {
    // We run our tool with the provided command line arguments.
    // This will call the ToyTool::execute method we have defined.
    IndexKmers().run (argc, argv);

  }
  catch (Exception& e)
  {
    std::cout << "EXCEPTION: " << e.getMessage() << std::endl;
    return EXIT_FAILURE;
  }
}
