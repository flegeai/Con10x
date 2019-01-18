#include <iostream>
#include <string>
#include <gatb/gatb_core.hpp>
#ifndef BARCODE_SIZE
#define BARCODE_SIZE 16
#endif
#ifndef SEQ_START
#define SEQ_START 23
#endif

#ifndef FRAGMENT_H
#define FRAGMENT_H

class Fragment10x {
public:
    Sequence r1,r2;
    string barcode;
    Fragment10x (const Sequence&, const Sequence&, const string);
    Fragment10x (const Sequence& s1, const Sequence& s2);
};

inline Fragment10x::Fragment10x(const Sequence& seq1, const Sequence& seq2) {
  barcode=seq1.toString().substr(0,BARCODE_SIZE);
  char* data = seq1.getDataBuffer();
  data+=SEQ_START;
  const char* quality = seq1.getQuality().c_str();
  quality+=SEQ_START;
  r1=Sequence(data);
  r1.setComment(seq1.getComment());
  r1.setQuality(quality);
  r2=seq2;
}


#endif
