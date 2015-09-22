#ifndef SLAVE_H
#define	SLAVE_H

#include <mpi.h>
#include "SectionDataType.h"

class CSlave{
    public:
        int processNum;
    
    private:
        MPI_Datatype sectionDataType;
    
        MPI_Comm parent;
        
        int positionX;
        int positionY;
        
        t_section_datatype temperature;
        
        int globalWidth;
        int globalHeight;
        
        t_section_datatype ambiantTemperature;
        
        t_section_datatype neighbourhoudTemperatures[3][3];
        
    
    
    public:
        CSlave();
        
        bool init(int argc, char** argv);
        void finalize();
        
        void recvTemperatureFromMaster();
        void recvGlobalSizeFromMaster();
        
        void recvAmbiantTemperatureFromCoordinator();
        void sendTemperatureToCoordinator();
        
        void recvTemperaturesFromNeighbourhoud();
        void sendTemperatureToNeighbourhoud();
        
        void calcTemperature();
    
    private:
        t_section_datatype getTemperaturesSection(int x, int y);
        int calcSubTemperature(int x, int y);
        
        MPI_Datatype getSectionDatatype ();
};

#endif