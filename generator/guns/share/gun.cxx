

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC3.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Print.h"
#include "HepMC3/WriterRootTree.h"
#include <iostream>
#include <sstream>

using namespace HepMC3;


/** Main */
int main(int argc, char **argv) {

    if( argc<3 ) {
        std::cout << "Usage: " << argv[0] << " <nov> <eventNumber> <seed> <input_data_file> <output_root_file>" <<std:: endl;
        exit(-1);
    }

    Pythia8::Pythia pythia;
    HepMC3::Pythia8ToHepMC3 pythiaToHepMC;


    // get all argvs
    int nEvent = atoi(argv[1]);
    int eventNumber = atoi(argv[2]);
    int seed = atoi(argv[3]);

    std::cout << "Nov           : " << argv[1] << std::endl;
    std::cout << "Event number  : " << argv[2] << std::endl;
    std::cout << "Seed          : " << argv[3] << std::endl;
    std::cout << "File          : " << argv[4] << std::endl;
    std::cout << "Output        : " << argv[5] << std::endl;

    pythia.readFile(argv[4]);
    HepMC3::WriterRootTree  root_output(argv[5]);


    // initialize pythia
    std::stringstream cmdseed; cmdseed << "Random:seed = " << seed;
    pythia.readString("Random:setSeed = on");
    pythia.readString(cmdseed.str());
    pythia.init();

    int saved = 0;
    for( int i = 0; i< nEvent; ++i ) 
    {

        if( !pythia.next() ){
            i--;
            continue;
        }

        float weight    = pythia.info.mergingWeight();
        float evtweight = pythia.info.weight();
        weight *= evtweight;

        if ( weight == 0. ) {
            i--;
            continue;
        }

        HepMC3::GenEvent evt( Units::GEV, Units::MM );
        pythiaToHepMC.fill_next_event(pythia.event, &evt, -1, &pythia.info);

        evt.set_event_number(eventNumber+i);

        if( i==0 ) {
            std::cout << "First event: " << std::endl;
            HepMC3::Print::listing(evt);
        }

        if( i%1000 == 0 ) {
            std::cout << "Event: " << i << std::endl;
        }

        root_output.write_event(evt);
        saved++;
    }

    root_output.close();

    std::cout << "Events parsed and written: " << saved << std::endl;
    
    return 0;
}

