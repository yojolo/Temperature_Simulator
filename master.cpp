/* 
 * File:   main.cpp
 * Author: zack
 *
 * Created on 16 septembre 2015, 09:46
 */

#include <cstdlib>
#include <mpi.h>
#include "SectionDataType.h"

using namespace std;

#define GLOBAL_SIZE_X 3
#define GLOBAL_SIZE_Y 3

#define NB_PROCESS 2

MPI_Datatype getSectionDatatype ();

/*
 * 
 */
int main(int argc, char** argv) {
    MPI_Init( &argc, &argv );
    
    MPI_Datatype sectionDataType = getSectionDatatype();
    
    /* Lancement de tous les processus (Esclaves et Terminaux) */
    MPI_Status etat;
    char *cmds[NB_PROCESS] = {
        "slave",
        "coordinator"
    };
    int np[NB_PROCESS] = {
        (GLOBAL_SIZE_X * GLOBAL_SIZE_Y), // On lance X instances des esclaves
        1 // On lance 1 instance du coordinateur
    };
    // Pas d'info supplémentaire pour contrôler le lancement
    // des programmes 1 et 2
    
    MPI_Info infos[NB_PROCESS] = { MPI_INFO_NULL, MPI_INFO_NULL };
    int errcodes[((GLOBAL_SIZE_X * GLOBAL_SIZE_Y) +2)]; // Les codes de retours des processus
    
    MPI_Comm intercomm; // L'espace de communication père - fils
    // On lance simultanément toutes les instances
    MPI_Comm_spawn_multiple (
        NB_PROCESS,
        cmds,
        MPI_ARGVS_NULL,
        np,
        infos,
        0,
        MPI_COMM_WORLD,
        &intercomm,
        errcodes
    );
    printf ("Master : J'ai lance toutes les instances.\n");
    /* Fin du lancement */
    
    
    // Distribution des températures de départ aux slaves
    t_section_datatype startTemperature;
    for(int i=0; i < SECTION_SIZE; i++){
        for(int j=0; j < SECTION_SIZE; j++){
            startTemperature.temperature[i][j] = 30;
        }
    }
    
    int processNum = 0, globalSizeX = GLOBAL_SIZE_X, globalSizeY = GLOBAL_SIZE_Y;
    for(int x=0; x < GLOBAL_SIZE_X; x++){
        for(int y=0; y < GLOBAL_SIZE_Y; y++){
            processNum = (y * GLOBAL_SIZE_Y) + x;
            MPI_Send (&startTemperature, 1, sectionDataType, processNum, 0, intercomm);
            MPI_Send (&globalSizeX, 1, MPI_INT, processNum, 0, intercomm);
            MPI_Send (&globalSizeY, 1, MPI_INT, processNum, 0, intercomm);
        }
    }
    // Distribution de la température ambiante au coordinateur
    int ambianteTemperature = 20;
    MPI_Send (&ambianteTemperature, 1, MPI_INT, (GLOBAL_SIZE_X*GLOBAL_SIZE_Y), 0, intercomm);
    MPI_Send (&globalSizeX, 1, MPI_INT, (GLOBAL_SIZE_X*GLOBAL_SIZE_Y), 0, intercomm);
    MPI_Send (&globalSizeY, 1, MPI_INT, (GLOBAL_SIZE_X*GLOBAL_SIZE_Y), 0, intercomm);
    
    
    // Attente de la réception des char du coordinateur
    char coordinatorReturn = '\0';
    MPI_Recv(&coordinatorReturn, 1, MPI_CHAR, (GLOBAL_SIZE_X*GLOBAL_SIZE_Y), 0, intercomm, &etat);
    cout << "Master : Retour du Coordinator : " << coordinatorReturn << endl;
    
    cout << "Master : Fin." << endl;
    MPI_Finalize();

    return 0;
}



MPI_Datatype getSectionDatatype ()
{
    MPI_Datatype typeSection;
    MPI_Datatype type = MPI_INT;
    int blocklen = SECTION_SIZE*SECTION_SIZE;
    MPI_Aint disp = 0;
    MPI_Type_struct (1, &blocklen, &disp, &type, &typeSection);
    MPI_Type_commit (&typeSection);
    return typeSection;
}


