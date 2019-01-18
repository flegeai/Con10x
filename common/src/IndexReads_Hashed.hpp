#include <Fragment10x.hpp>

/********************************************************************************/
/*             IndexReads                                      */
/* this is a simple test to rapidly index 10x reads using their barcodes    */
/********************************************************************************/
#ifndef IDRH_H
#define IDRH_H

class IndexReads_Hashed {
public:
//    IndexReads_Hashed();
    void add (Fragment10x& fragment);
    void save(std::string index_file);
    void load(std::string index_file);
    void get (std::string barcode);
private:
    std::map<std::string,std::list<string>> barcode_map;
//      std::map<std::string,int> barcode_map;
};

#endif
