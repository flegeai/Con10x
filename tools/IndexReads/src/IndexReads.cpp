#include <gatb/gatb_core.hpp>
#include "Fragment10x.hpp"
#include "IndexReads_Hashed.hpp"
#include "IndexReads_Interleaved.hpp"
#include <string>

/********************************************************************************/
/*                                                                              */
/*             IndexReads.cpp                                                   */
/* this is a simple test to rapidly index 10x reads using their barcodes         */
/********************************************************************************/
using namespace std;

int main (int argc, char* argv[])
{

    static const char* STR_BANK1 = "-one";
    static const char* STR_BANK2 = "-two";
    static const char* ITL_BANK = "-itl";

    OptionsParser parser ("Index");

    parser.push_back (new OptionOneParam (STR_BANK1, "bank one", true));
    parser.push_back (new OptionOneParam (STR_BANK2, "bank two", true));
    parser.push_back (new OptionOneParam (ITL_BANK, "interleaved bank", false));
    try
    {
        IProperties* options = parser.parse (argc, argv);
        // We open the two banks
        IBank* bank1 = Bank::open (options->getStr(STR_BANK1));
        LOCAL (bank1);
        IBank* bank2 = Bank::open (options->getStr(STR_BANK2));
        LOCAL (bank2);

        // We iterate the two banks. Note how we provide two iterators from the two banks.
        PairedIterator<Sequence> *  itPair  = new  PairedIterator<Sequence> (bank1->iterator(), bank2->iterator());
        LOCAL(itPair);
        ProgressIterator< std::pair <Sequence, Sequence>> progress_iter (itPair, "", bank1->estimateNbItems());

        //IndexReads_Interleaved idRI(options->getStr(ITL_BANK));
      //  if (!options->getStr(ITL_BANK).empty()) {
      if ( parser.saw(ITL_BANK)) {
          std::string itl_file = options->getStr(ITL_BANK);
          IndexReads_Interleaved idRI(itl_file, "app");

          for (progress_iter.first(); !progress_iter.isDone(); progress_iter.next()){
              Sequence& s1 = itPair->item().first;
              Sequence& s2 = itPair->item().second;
              Fragment10x frag(s1,s2);
              idRI.add(frag);
            }

            idRI.save(itl_file+".idx");
        }
        else {
            IndexReads_Hashed idRH;

            for (progress_iter.first(); !progress_iter.isDone(); progress_iter.next()){
                Sequence& s1 = itPair->item().first;
                Sequence& s2 = itPair->item().second;
                Fragment10x frag(s1,s2);
                idRH.add(frag);
            }
            
            idRH.save("barcode.idx");
        }
      }
    catch (Exception& e)
    {
        std::cerr << "EXCEPTION: " << e.getMessage() << std::endl;
    }
}
