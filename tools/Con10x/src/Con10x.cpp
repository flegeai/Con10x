#include <Con10x.hpp>

using namespace std;

/********************************************************************************/

// We define some constant strings for names of command line parameters
static const char* STR_FOO = "-foo";

/*********************************************************************
** METHOD  :
** PURPOSE :
** INPUT   :
** OUTPUT  :
** RETURN  :
** REMARKS :
*********************************************************************/
Con10x::Con10x ()  : Tool ("Con10x")
{
    // We add some custom arguments for command line interface
    getParser()->push_front (new OptionOneParam (STR_FOO, "my option",  false, "1"));
}

/*********************************************************************
** METHOD  :
** PURPOSE :
** INPUT   :
** OUTPUT  :
** RETURN  :
** REMARKS :
*********************************************************************/
void Con10x::execute ()
{
    // We can do here anything we want.
    // For further information about the Tool class, please have a look
    // on the ToyTool snippet  (http://gatb-core.gforge.inria.fr/snippets_tools.html)

    // We gather some statistics.
    getInfo()->add (1, "input");
    getInfo()->add (2, STR_FOO,  "%d",  getInput()->getInt(STR_FOO));
    getInfo()->add (1, &LibraryInfo::getInfo());
}
