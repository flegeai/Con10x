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

    static const char* READS = "-reads";
    static const char* BARCODE = "-barcodes";

    OptionsParser parser ("getReads");

    parser.push_back (new OptionOneParam (READS, "file of indexed reads",   true));
    parser.push_back (new OptionOneParam (BARCODE, "barcodes",   true));
    try
    {
        IProperties* options = parser.parse (argc, argv);
        string reads_file = options->getStr(READS);
        string barcode_file = options->getStr(BARCODE);

        std::ifstream barcodemap_index(reads_file + ".idx", std::ios::binary);
        std::map<std::string,std::list<long long>>  barcode_map;
        boost::archive::binary_iarchive iarch(barcodemap_index);
        iarch & barcode_map;

        ifstream  barcodemap_file_r (reads_file);

         ifstream barcodes (barcode_file);
         if (barcodes.is_open()){
            string barcode;
            while ( getline (barcodes,barcode) )
            {
                std::list<long long>::iterator it;
                for (it = barcode_map[barcode].begin(); it != barcode_map[barcode].end(); ++it) {
                  barcodemap_file_r.seekg((streampos) *it);
                  for (int i =0; i<=7; i++) {
                    string pair;
                    std::getline(barcodemap_file_r,pair);
                    cout << pair<< endl;
                  }
                }
            }
            barcodes.close();
          }

          else cout << "Unable to open " + barcode_file << endl;
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
