#include <iostream>

#include "CSlave.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    CSlave* slave = new CSlave();
    
    
    if(slave->init(argc, argv)){
        slave->recvTemperatureFromMaster();
        slave->recvGlobalSizeFromMaster();
        
        for(int i=0; i<10; i++){
            slave->recvAmbiantTemperatureFromCoordinator();
            slave->sendTemperatureToNeighbourhoud();
            slave->recvTemperaturesFromNeighbourhoud();
            slave->calcTemperature();
            slave->sendTemperatureToCoordinator();
        }
        
    }else{
        cout << "Slave " << slave->processNum  << " : Pas de maitre !" << endl;
    }
    
    slave->finalize();
    
    delete slave;

    return 0;
}
