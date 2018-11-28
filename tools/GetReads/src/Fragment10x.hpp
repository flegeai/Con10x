#include <iostream>
#include <string>
#include <gatb/gatb_core.hpp>
#define BARCODE_SIZE 16
#define SEQ_START 23

class Fragment10x {

public:
    Sequence r1,r2;
    string barcode;
    Fragment10x (const Sequence&, const Sequence&, const string);
    Fragment10x (const Sequence&, const Sequence&);
};

Fragment10x::Fragment10x(const Sequence& seq1 , const Sequence& seq2,  const string) {
  r1=seq1;
  r2=seq2;
  barcode=barcode;
}

Fragment10x::Fragment10x(const Sequence& seq1 , const Sequence& seq2) {
  barcode=seq1.toString().substr(0,BARCODE_SIZE);
  char* data = seq1.getDataBuffer();
  data+=SEQ_START;
  r1=Sequence(data);
  r2=seq2;
}
