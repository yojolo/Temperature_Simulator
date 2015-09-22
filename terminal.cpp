#include <cstdlib>
#include <string>
#include <mpi.h>
#include "SectionDataType.h"

using namespace std;

#define GLOBAL_SIZE_X 3
#define GLOBAL_SIZE_Y 3
int globalSizeX = GLOBAL_SIZE_X, globalSizeY = GLOBAL_SIZE_Y, ambiantTemperature = 0;

/*
 * 
 */
int main(int argc, char** argv) {
    
    MPI_Init (&argc, &argv);
    
    int processNum;
    MPI_Datatype sectionDataType = getSectionDatatype();
    
    /* Initialisation du processus */
    MPI_Comm parent;
    MPI_Status etat;
    MPI_Comm_get_parent (&parent);
    MPI_Comm_rank (MPI_COMM_WORLD,&processNum);
    /* Fin de l'initialisation */
    
    
    MPI_Finalize();

    return 0;
}

