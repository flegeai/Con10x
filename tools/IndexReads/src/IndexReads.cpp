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
    static const char* BARCODE = "-barcode";

    OptionsParser parser ("TwoBanks");

    parser.push_back (new OptionOneParam (STR_BANK1, "bank one",   true));
    parser.push_back (new OptionOneParam (STR_BANK2, "bank two",   true));
    parser.push_back (new OptionOneParam (BARCODE, "barcode",   true));
    try
    {
        IProperties* options = parser.parse (argc, argv);
        // We open the two banks
        IBank* bank1 = Bank::open (options->getStr(STR_BANK1));  LOCAL (bank1);
        IBank* bank2 = Bank::open (options->getStr(STR_BANK2));  LOCAL (bank2);
        string barcode = options->getStr(BARCODE);

        // We iterate the two banks. Note how we provide two iterators from the two banks.
        PairedIterator<Sequence> *  itPair  = new  PairedIterator<Sequence> (bank1->iterator(), bank2->iterator());
                LOCAL(itPair);
                ProgressIterator< std::pair <Sequence, Sequence>> progress_iter (itPair, "", bank1->estimateNbItems());

        ofstream barcodemap_file ("barcode_reads.map");

        /* Map creation */
        //std::map<std::string, Fragment_map>> barcode_map;
        //std::map<std::string,std::list<streampos>> barcode_map;
        std::map<std::string,std::list<long>> barcode_map;

        for (progress_iter.first(); !progress_iter.isDone(); progress_iter.next())
        {
            Sequence& s1 = itPair->item().first;
            Sequence& s2 = itPair->item().second;
            //std::cout << " avant : " << s1.toString() << endl;
            Fragment10x f(s1,s2);
            /* std::cout << " barcode: " << f.barcode << endl;
            std::cout << "r1: " << f.r1.toString() << endl;
            std::cout << "r2: " << f.r2.toString() << endl;
            */
            barcode_map[f.barcode].push_back((long) barcodemap_file.tellp());
            //barcode_map[f.barcode].add((long)barcodemap_file.tellp());
            barcodemap_file << f.barcode + " " + f.r1.toString() + " " + f.r2.toString() << endl;
        }

        // register the map with boost
        ofstream barcodemap_index ("barcode_reads.idx");
        boost::archive::binary_oarchive oarch(barcodemap_index);
        oarch << barcode_map;

        barcodemap_file.close();
        barcodemap_index.close();

        ifstream  barcodemap_file_r ("barcode_reads.map");
        std::list<long>::iterator it;

        for (it = barcode_map[barcode].begin(); it != barcode_map[barcode].end(); ++it) {
          barcodemap_file_r.seekg((streampos) *it);
          //long pos = (long long) *it;
          //cout << pos << endl;
          string pair;
          std::getline(barcodemap_file_r,pair);
          cout << pair<< endl;
        }
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
