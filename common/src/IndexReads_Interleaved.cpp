#include <gatb/gatb_core.hpp>
#include <iostream>
#include <fstream>
#include "IndexReads_Interleaved.hpp"
#include <Fragment10x.hpp>
#include <string>
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>


/********************************************************************************/
/*             IndexReads_Interleaved                                       */
/* this is a simple test to rapidly index 10x reads using their barcodes    */
/* with the generation of a interleaved file                             */
/********************************************************************************/

IndexReads_Interleaved::IndexReads_Interleaved(std::string itl_file, std::string mode) {
  //cout << "mode : " + mode + " "  << endl;
  if (mode.compare("app") == 0) {
    //cout << "open mode append" << endl;
    interleaved_bank.open(itl_file, ios::out);
  }
  if (mode.compare("rd") == 0) {
      interleaved_bank.open(itl_file, ios::in);
  }
}

void IndexReads_Interleaved::add (Fragment10x& fragment) {
    barcode_map[fragment.barcode].push_back((long long) interleaved_bank.tellp());
    interleaved_bank << '@'+fragment.r1.getComment()  << endl;
    interleaved_bank << fragment.r1.toString()  << endl;
    interleaved_bank << '+'  << endl;
    interleaved_bank << fragment.r1.getQuality() << endl;
    interleaved_bank << '@'+fragment.r2.getComment()  << endl;
    interleaved_bank << fragment.r2.toString()  << endl;
    interleaved_bank << '+'  << endl;
    interleaved_bank << fragment.r2.getQuality() << endl;
}


void IndexReads_Interleaved::get (std::string barcode) {
    std::list<long long>::iterator it;

    for (it = barcode_map[barcode].begin(); it != barcode_map[barcode].end(); ++it) {
      interleaved_bank.seekg((streampos) *it);
      for (int i =0; i<=7; i++) {
        string pair;
        std::getline(interleaved_bank,pair);
        //to be changed to return a list of reads (in a GATB manner)
        cout << pair<< endl;
      }
    }
}

void IndexReads_Interleaved::save (std::string index_file) {
  ofstream barcodemap_index (index_file);
  boost::archive::binary_oarchive oarch(barcodemap_index);
  oarch << barcode_map;
  barcodemap_index.close();
}

void IndexReads_Interleaved::load (std::string index_file) {
  std::ifstream barcodemap_index (index_file,std::ios::binary);
  boost::archive::binary_iarchive iarch(barcodemap_index);
  iarch & barcode_map;
}
