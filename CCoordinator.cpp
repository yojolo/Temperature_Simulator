#include "CCoordinator.h"

#include <iostream>
using namespace std;

CCoordinator::CCoordinator(){
    this->processNum = 0;
    
    this->terminal.initWindow();
    this->terminal.maxTemperature = 30;
    this->terminal.minTemperature = 20;
}

bool CCoordinator::init(int argc, char** argv){
    MPI_Init (&argc, &argv);
    
    MPI_Comm_get_parent (&this->parent);
    MPI_Comm_rank (MPI_COMM_WORLD, &this->processNum);
    
    this->sectionDataType = getSectionDatatype();
    
    return (this->parent != MPI_COMM_NULL);
}

void CCoordinator::finalize(){
    MPI_Finalize();
}


void CCoordinator::recvAmbiantTemperatureFromMaster(){
    MPI_Status etat;
    int recvAmbiantTemperature = 0;
    
    MPI_Recv(&recvAmbiantTemperature, 1, MPI_INT, 0, 0, this->parent, &etat);
    
    for(int y = 0; y < SECTION_SIZE; y ++){
        for(int x = 0; x < SECTION_SIZE; x ++){
            this->ambiantTemperature.temperature[y][x] = recvAmbiantTemperature;
        }
    }
}

void CCoordinator::recvGlobalSizeFromMaster(){
    MPI_Status etat;
    
    MPI_Recv (&this->globalWidth, 1, MPI_INT, 0, 0, this->parent, &etat);
    MPI_Recv (&this->globalHeight, 1, MPI_INT, 0, 0, this->parent, &etat);
}

void CCoordinator::sendCharToMaster(char ch){
    MPI_Send (&ch, 1, MPI_CHAR, 0, 0, this->parent);
}


void CCoordinator::sendAmbiantTemperatureToSlaves(){
    // Envoie de la température ambiante à tous les esclaves
    for(int i = 0; i < (this->globalWidth * this->globalHeight); i ++){
        MPI_Send (&this->ambiantTemperature, 1, this->sectionDataType, i, 0, MPI_COMM_WORLD);
    }
}

void CCoordinator::recvTemperaturesFromSlaves(){
    MPI_Status etat;
    int processNum = 0;
    
    // Récupération des températures des esclaves
    for(int x = 0; x < this->globalWidth; x++){
        for(int y = 0; y < this->globalHeight; y++){
            processNum = (y * this->globalHeight) + x;
            
            MPI_Recv (&this->globalTemperatures[y][x], 1, this->sectionDataType, processNum, 0, MPI_COMM_WORLD, &etat);
        }                
    }
}


void CCoordinator::printTemperatures(){
    this->terminal.clearWindow();
    
    
    for(int i=0; i < (this->globalWidth * this->globalHeight); i++){
        int x = i % this->globalWidth;
        int y = (i - x) / this->globalHeight;

        this->terminal.createTemperatureSection(this->globalTemperatures[y][x], SECTION_SIZE, x, y);
    }   
    
    this->terminal.renderWindow();
    
    /**
    cout << endl << "-----------------------------------" << endl << endl;
    
    for(int y = 0; y < this->globalHeight; y++){
        for(int x=0; x < this->globalWidth; x++){
            
            for(int subY = 0; subY < SECTION_SIZE; subY ++){
                for(int subX = 0; subX < SECTION_SIZE; subX ++){
                    cout << globalTemperatures[y][x].temperature[subY][subX] << " ";
                }
                cout << " - " << endl; 
            }
            cout << endl;
        }
        cout << endl;
    }
    cout << "-----------------------------------" << endl << endl;
    /**/
}





MPI_Datatype CCoordinator::getSectionDatatype ()
{
    MPI_Datatype typeSection;
    MPI_Datatype type = MPI_INT;
    int blocklen = SECTION_SIZE*SECTION_SIZE;
    MPI_Aint disp = 0;
    MPI_Type_struct (1, &blocklen, &disp, &type, &typeSection);
    MPI_Type_commit (&typeSection);
    return typeSection;
}

