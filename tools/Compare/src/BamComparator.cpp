#include <iostream>
#include <fstream>
#include <stdexcept>
#include "hts.h"
#include "sam.h"
#include <map>
#include <list>
#include <vector>
#include <getopt.h>
//#include "boost/program_options.hpp"
#define BARCODE_SIZE 16
#define BXTAG "BX"

using namespace std;
//namespace po = boost::program_options;

// functions declarations
bool *fromflagtobits (int);
std::list<string> getBarcodesfromRegion(samFile *samFile,  hts_idx_t  *index, bam_hdr_t *header, string region);
int common_barcodes(std::list<string> list1, std::list<string> list2 );

int main (int argc, char* argv[])
{

	try{
		string bam;
		string reg_files;
	//	po::options_description desc("Options");
	//	desc.add_options()
    //	("list,l",po::value<string>(&reg_files)->required(), "file containing regions")
    //	("bam,b", po::value<string>(&bam)->required(), "bam file")
	//		;
		const char* const short_opts = "b:l:";

    static struct option long_options[] = {
        {"bam",      required_argument,       0,  'b' },
        {"list", required_argument,       0,  'l' },
        {0,           0,                 0,  0   }
    };

		while (true) {
			const auto opt = getopt_long(argc, argv,short_opts, long_options, 0);
			if (opt == -1) {break;}
		   switch (opt) {
             case 'b' :
						  	bam= std::string(optarg);
                 break;
						case 'l' :
							 reg_files= std::string(optarg);
								break;

            default:
							std::cout <<
            	"--bam <bam>: bam file\n"
            	"--list <list>: list of regions\n";
                 exit(EXIT_FAILURE);
        }
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

			std::map<std::string,std::list<string>> barcodes_map;

			// We extract barcodes from all the regions
			vector<string> regions;

			ifstream list_regions (reg_files);
			if (list_regions.is_open()){
				string region;
				while ( getline (list_regions, region) ){
					std::list<string>  l=getBarcodesfromRegion(in,idx,header,region);
					barcodes_map[region]=l;
					regions.push_back(region);
				}
			}

			// We generate the matrix

			for(vector<string>::iterator r1= regions.begin(); r1 != regions.end();++r1){
				for(vector<string>::iterator r2= regions.begin(); r2 != regions.end();++r2){
					cout << *r1 << " " << *r2 << " "<< common_barcodes(barcodes_map[*r1],barcodes_map[*r2]) << "\n";
				}
			}

			hts_idx_destroy(idx);
			bam_hdr_destroy(header);
			sam_close(in);

			// Generation de la matrice

			return 0;
		}

	} catch (const runtime_error& e) {
		cerr << e.what();
	}
}


std::list<string> getBarcodesfromRegion(samFile *samFile,  hts_idx_t  *index, bam_hdr_t *header, string region) {
	//Initialize iterator
	hts_itr_t *iter = NULL;
	string region_ = ".";
	//Move the iterator to the region we are interested in
	iter  = sam_itr_querys(index, header, region_.c_str());
	if(header == NULL || iter == NULL) {
		sam_close(samFile);
		throw runtime_error("Unable to iterate to region within BAM.");
	}

	std::map<std::string,bool> barcode_map;
	//Initiate the alignment record
	bam1_t *aln = bam_init1();
	while(sam_itr_next(samFile, iter, aln) >= 0) {
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
		std::list<string> barcodes;

		map<string, bool>::iterator itr;
		for (itr = barcode_map.begin(); itr != barcode_map.end(); ++itr) {
					barcodes.push_back(itr->first);
		}

		hts_itr_destroy(iter);
		bam_destroy1(aln);
		return barcodes;
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

int common_barcodes(std::list<string> list1, std::list<string> list2 ) {
	int common =0;
	for (std::list<string>::iterator it1 = list1.begin(); it1 != list1.end(); ++it1) {
		for (std::list<string>::iterator it2 = list2.begin(); it2 != list2.end(); ++it2) {
			common++;
		}
	}
	return common;
}
