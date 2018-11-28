#include <gatb/gatb_core.hpp>
#include <iostream>
#include <fstream>
#include "../../Con10x/Fragment10x.hpp"
#include <string>
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/archive/binary_oarchive.hpp>

/********************************************************************************/
/*                                                                              */
/*             IndexReads.cpp                                                   */
/* this is a simple test to rapidly index 10x reads using their barcodes         */
/********************************************************************************/

int main (int argc, char* argv[])
{

    static const char* STR_BANK1 = "-one";
    static const char* STR_BANK2 = "-two";
    static const char* OUT_BANK = "-out";

    OptionsParser parser ("TwoBanks");

    parser.push_back (new OptionOneParam (STR_BANK1, "bank one",   true));
    parser.push_back (new OptionOneParam (STR_BANK2, "bank two",   true));
    parser.push_back (new OptionOneParam (OUT_BANK, "output bank (interleaved)",   true));
    try
    {
        IProperties* options = parser.parse (argc, argv);
        // We open the two banks
        IBank* bank1 = Bank::open (options->getStr(STR_BANK1));  LOCAL (bank1);
        IBank* bank2 = Bank::open (options->getStr(STR_BANK2));  LOCAL (bank2);
        ofstream output_bank (options->getStr(OUT_BANK));

        // We iterate the two banks. Note how we provide two iterators from the two banks.
        PairedIterator<Sequence> *  itPair  = new  PairedIterator<Sequence> (bank1->iterator(), bank2->iterator());
        LOCAL(itPair);
        ProgressIterator< std::pair <Sequence, Sequence>> progress_iter (itPair, "", bank1->estimateNbItems());

        /* Map creation */
        //std::map<std::string, Fragment_map>> barcode_map;
        //std::map<std::string,std::list<streampos>> barcode_map;
        std::map<std::string,std::list<long long>> barcode_map;

        for (progress_iter.first(); !progress_iter.isDone(); progress_iter.next())
        {
            Sequence& s1 = itPair->item().first;
            Sequence& s2 = itPair->item().second;
            //std::cout << " avant : " << s1.toString() << endl;
            Fragment10x f(s1,s2);

            barcode_map[f.barcode].push_back((long long) output_bank.tellp());
            output_bank << '@'+f.r1.getComment()  << endl;
            output_bank << f.r1.toString()  << endl;
            output_bank << '+'  << endl;
            output_bank << f.r1.getQuality() << endl;
            output_bank << '@'+f.r2.getComment()  << endl;
            output_bank << f.r2.toString()  << endl;
            output_bank << '+'  << endl;
            output_bank << f.r2.getQuality() << endl;
        }

        // register the map with boost
        std::string index = options->getStr(OUT_BANK);
        ofstream barcodemap_index (index+".idx");
        boost::archive::binary_oarchive oarch(barcodemap_index);
        oarch << barcode_map;
        output_bank.close();
        barcodemap_index.close();
      }
    catch (OptionFailure& e)
    {
        return e.displayErrors (std::cout);
    }
    catch (Exception& e)
    {
        std::cerr << "EXCEPTION: " << e.getMessage() << std::endl;
    }
}
