#include <cstdlib>
#include <string>
#include <mpi.h>

#include "CCoordinator.h"

using namespace std;

#define NB_ITERATIONS 10

/*
 * 
 */
int main(int argc, char** argv) {
    
    CCoordinator* coordinator = new CCoordinator();
    
    if(coordinator->init(argc, argv)){
        coordinator->recvAmbiantTemperatureFromMaster();
        coordinator->recvGlobalSizeFromMaster();
        
        for(int i=0; i < NB_ITERATIONS; i++){
            coordinator->sendAmbiantTemperatureToSlaves();
            coordinator->recvTemperaturesFromSlaves();
            coordinator->printTemperatures();
        }
        
        coordinator->sendCharToMaster('1');
        
    }else{
        cout << "Coordinator : Pas de maitre !" << endl;
    }
    
    coordinator->finalize();
    
    return 0;
}
