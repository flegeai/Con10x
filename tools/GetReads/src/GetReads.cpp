#include <gatb/gatb_core.hpp>
#include <iostream>
#include <fstream>
#include <IndexReads_Hashed.hpp>
#include <IndexReads_Interleaved.hpp>
#include <string>


/********************************************************************************/
/*                                                                              */
/*             IndexReads.cpp                                                   */
/* this is a simple test to rapidly index 10x reads using their barcodes         */
/********************************************************************************/

int main (int argc, char* argv[])
{

    static const char* READS = "-reads";
    static const char* BARCODE = "-barcodes";
    static const char* INDEX = "-index";

    OptionsParser parser ("getReads");

    parser.push_back (new OptionOneParam (READS, "file of indexed reads",   false));
    parser.push_back (new OptionOneParam (BARCODE, "barcodes",   true));
    parser.push_back (new OptionOneParam (INDEX, "barcodes index file",   true));

    try
    {
        IProperties* options = parser.parse (argc, argv);

        string barcode_file = options->getStr(BARCODE);
        string index_file = options->getStr(INDEX);

        if ( parser.saw(READS)) {
          std::string itl_file = options->getStr(READS);
          IndexReads_Interleaved idRI(itl_file, "rd");
          idRI.load(index_file);
          ifstream barcodes (barcode_file);
          if (barcodes.is_open()){
              string barcode;
              while ( getline (barcodes,barcode) ){idRI.get(barcode);}
          }
          barcodes.close();
        }
        else {
          IndexReads_Hashed idRH;
          idRH.load(index_file);
          ifstream barcodes (barcode_file);
           if (barcodes.is_open()){
               string barcode;
               while ( getline (barcodes,barcode) ){
                 cout << barcode << endl;
                 idRH.get(barcode);
               }
           }
          barcodes.close();
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
