#include <Fragment10x.hpp>

#ifndef IDRI_H_
#define IDRI_H_

/********************************************************************************/
/*             IndexReads_Interleaved                                       */
/* this is a simple test to rapidly index 10x reads using their barcodes    */
/* with the generation of a interleaved file                             */
/********************************************************************************/
//class Fragment10x;
class IndexReads_Interleaved {
public:
      IndexReads_Interleaved(std::string itl_file, std::string);
      void add (Fragment10x& fragment);
      void get (std::string barcode);
      void save(std::string index_file);
      void load(std::string index_file);
    //  std::list<Fragment10x> get(std::string barcode);
private:
    std::map<std::string,std::list<long long>> barcode_map;
    fstream interleaved_bank;
};

#endif
