#ifndef COORDINATOR_H
#define	COORDINATOR_H

#include <mpi.h>
#include "SectionDataType.h"
#include "CTerminal.h"

class CCoordinator{
    public:
        int processNum;
    private:
        CTerminal terminal;
    
        MPI_Datatype sectionDataType;
    
        MPI_Comm parent;
        
        int globalWidth;
        int globalHeight;
        t_section_datatype ambiantTemperature;
        
        t_section_datatype globalTemperatures[100][100];
        
    public:
        CCoordinator();
        
        bool init(int argc, char** argv);
        void finalize();
        
        void recvAmbiantTemperatureFromMaster();
        void recvGlobalSizeFromMaster();
        void sendCharToMaster(char ch);
        
        void sendAmbiantTemperatureToSlaves();
        void recvTemperaturesFromSlaves();
        
        void printTemperatures();
        
    private:
        MPI_Datatype getSectionDatatype ();
};

#endif
