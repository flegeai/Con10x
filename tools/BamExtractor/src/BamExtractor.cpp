#include <iostream>
#include <stdexcept>

#include "hts.h"
#include "sam.h"
#include <map>
#define BARCODE_SIZE 16
#define BXTAG "ZOB"

using namespace std;
bool *fromflagtobits (int);

int main(int argc, char* argv[]) {
	  try {
    	if(argc < 2) {
				cerr << "Arguments error\n";
      	return (123);
    	}

		 std::map<std::string,bool> barcode_map;


		string bam = string(argv[1]);
    string region_ = ".";
    if(argc > 2) {
        region_ = string(argv[2]);
    }

    if(!bam.empty()) {
        //open BAM for reading
        samFile *in = sam_open(bam.c_str(), "r");
        if(in == NULL) {
            throw runtime_error("Unable to open BAM/SAM file.");
        }
        //Load the index
        hts_idx_t *idx = sam_index_load(in, bam.c_str());
        if(idx == NULL) {
            throw runtime_error("Unable to open BAM/SAM index."
                                " Make sure alignments are indexed");
        }
        //Get the header
        bam_hdr_t *header = sam_hdr_read(in);
        //Initialize iterator
        hts_itr_t *iter = NULL;
        //Move the iterator to the region we are interested in
        iter  = sam_itr_querys(idx, header, region_.c_str());
        if(header == NULL || iter == NULL) {
            sam_close(in);
            throw runtime_error("Unable to iterate to region within BAM.");
        }
        //Initiate the alignment record
        bam1_t *aln = bam_init1();
        while(sam_itr_next(in, iter, aln) >= 0) {
						//printf("qname: %s\n",bam_get_qname(aln));
            //cout << "\tPos: " << aln->core.pos;
						//cout << "\tFlag: " << aln->core.flag;
						uint8_t* bxtag = bam_aux_get(aln, BXTAG);

						if (bxtag != 0) { // The  barcode is in the bam auxiliary tags (LongRanger)
								string barcode;
								int i = 1;

								while (*(bxtag+i) != '\0') {
										barcode+=*(bxtag+i);
										i++;
								}
								cout<< barcode <<endl;
								barcode_map[barcode]=true;
						}
						else { // the barcode is in the sequence

							bool * bits = fromflagtobits(aln->core.flag);


							/* Barcode Extraction */
							/* We need to find the first seq of the fragments (bit 7 == true) */
							if (*(bits + 6) == true) {
								string barcode,seq;
								uint8_t *seqi = bam_get_seq(aln);
								if (*(bits +4) == false) { // Not reverse complemented
								  	for (int i = 0; i < BARCODE_SIZE; i++) {
                   	barcode += seq_nt16_str[bam_seqi(seqi, i)];
							    	}
								}
								else {
									  for (int i = aln->core.l_qseq-1; i >= aln->core.l_qseq-BARCODE_SIZE; i--) {
											switch (seq_nt16_str[bam_seqi(seqi, i)]) {
												case 'T' :
													barcode+='A';
													break;
												case 'A' :
													barcode+='T';
													break;
												case 'G' :
													barcode+='C';
													break;
												case 'C' :
													barcode+='G';
													break;
													default :
													barcode+='N';
											}
										}
									}

								  for (int i = 0; i < aln->core.l_qseq; i++) {
								         seq += seq_nt16_str[bam_seqi(seqi, i)];
								  }
									//cout << "\tSeq: " << seq << endl;
									//cout << "\tBarcode: " << barcode << endl;
									barcode_map[barcode]=true;
								}

						}
					}
				map<string, bool>::iterator itr;
				for (itr = barcode_map.begin(); itr != barcode_map.end(); ++itr) {
        		cout << itr->first << endl;
        }
        hts_itr_destroy(iter);
        hts_idx_destroy(idx);
        bam_destroy1(aln);
        bam_hdr_destroy(header);
        sam_close(in);
    }
    return 0;
  } catch (const runtime_error& e) {
        cerr << e.what();
    }
}

bool* fromflagtobits(int n)
{
    static bool bits[12] ;
    int remainder;
		int step=0;
   while (n!=0)
    {
      	remainder = n%2;
      // 	cout << "Step " << step << ": " << n << "/2, Remainder = " << remainder << ", Quotient = " << n/2 << endl;
        n /= 2;
				bits[step]=remainder;
				step++;
    }
    return bits;
}
