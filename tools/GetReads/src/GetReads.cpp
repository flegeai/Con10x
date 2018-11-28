#include <gatb/gatb_core.hpp>
#include <iostream>
#include <fstream>
#include "../../Con10x/Fragment10x.hpp"
#include <string>
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/archive/binary_iarchive.hpp>

/********************************************************************************/
/*                                                                              */
/*             IndexReads.cpp                                                   */
/* this is a simple test to rapidly index 10x reads using their barcodes         */
/********************************************************************************/

int main (int argc, char* argv[])
{

    static const char* IDX = "-idx";
    static const char* MAP = "-map";
    static const char* BARCODE = "-barcode";

    OptionsParser parser ("getReads");

    parser.push_back (new OptionOneParam (IDX, "file of reads indexed by barcode",   true));
    parser.push_back (new OptionOneParam (MAP, "reads map",   true));
    parser.push_back (new OptionOneParam (BARCODE, "barcode",   true));
    try
    {
        IProperties* options = parser.parse (argc, argv);
        string idx_file = options->getStr(IDX);
        string map_file = options->getStr(MAP);
        string barcode = options->getStr(BARCODE);

        std::ifstream barcodemap_file ("barcode_reads.map");
        std::ifstream barcodemap_index(idx_file, std::ios::binary);
        //std::ifstream barcodemap_index idx_file);

        std::map<std::string,std::list<long>>  barcode_map;
        boost::archive::binary_iarchive iarch(barcodemap_index);
        iarch & barcode_map;

        // register the map with boost


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
