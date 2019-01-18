#include <iostream>
#include <gatb/gatb_core.hpp>
#include <fstream>
class FragmentCollection {
    string barcode;
    std::list<long> poslis;

public:
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & barcode;
        ar & poslis;
        //std::list<streampos>::iterator it;
        //for (it = poslis.begin(); it != poslis.end(); ++it){ar & (long) it*}
    }
};
