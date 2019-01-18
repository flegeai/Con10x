#include <gatb/gatb_core.hpp>
#include <iostream>
#include <fstream>
//#include "../../Con10x/src/Fragment10x.hpp"
#include "IndexReads_Hashed.hpp"
#include <string>
#include <map>
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

using namespace std;
/********************************************************************************/
/*             IndexReads                                      */
/* this is a simple test to rapidly index 10x reads using their barcodes    */
/********************************************************************************/

void IndexReads_Hashed::add (Fragment10x& fragment) {
    std::string reads = fragment.r1.toString() + "@" + fragment.r2.toString();
    // cout << "adding " + reads << endl;
    // cout << "barcode " + fragment.barcode << endl;
    barcode_map[fragment.barcode].push_back(reads);
 }


void IndexReads_Hashed::save (std::string index_file) {
  ofstream barcodemap_index (index_file);
  boost::archive::binary_oarchive oarch(barcodemap_index);
  oarch << barcode_map;
  barcodemap_index.close();
}

void IndexReads_Hashed::load (std::string index_file) {
  std::ifstream barcodemap_index (index_file,std::ios::binary);
  boost::archive::binary_iarchive iarch(barcodemap_index);
  iarch & barcode_map;
}

void IndexReads_Hashed::get (std::string barcode) {
  std::list<string>::iterator it;
  for (it = barcode_map[barcode].begin(); it != barcode_map[barcode].end(); ++it) {
      std::string reads =  *it;
      //to be changed to return a list of reads (in a GATB manner)
      cout << reads << endl;
    }
}
