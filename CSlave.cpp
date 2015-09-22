#include "CSlave.h"

#include <omp.h>

#include <iostream>
using namespace std;

CSlave::CSlave(){
    this->processNum = 0;
}

bool CSlave::init(int argc, char** argv){
    MPI_Init (&argc, &argv);
    
    MPI_Comm_get_parent (&this->parent);
    MPI_Comm_rank (MPI_COMM_WORLD, &this->processNum);
    
    this->sectionDataType = this->getSectionDatatype();
    
    return (this->parent != MPI_COMM_NULL);
}

void CSlave::finalize(){
    MPI_Finalize();
}



void CSlave::recvTemperatureFromMaster(){
    MPI_Status etat;
    
    MPI_Recv(&this->temperature, 1, this->sectionDataType, 0, 0, this->parent, &etat);
    this->neighbourhoudTemperatures[1][1] = this->temperature;
}

void CSlave::recvGlobalSizeFromMaster(){
    MPI_Status etat;
    
    MPI_Recv (&this->globalWidth, 1, MPI_INT, 0, 0, this->parent, &etat);
    MPI_Recv (&this->globalHeight, 1, MPI_INT, 0, 0, this->parent, &etat);
        
    // Définition des positions X & Y sur la plaque en fonction du numéro du processus et de la taille de la plaque
    this->positionX = this->processNum % this->globalWidth;
    this->positionY = (this->processNum - this->positionX) / this->globalWidth;
}


void CSlave::recvAmbiantTemperatureFromCoordinator(){
    MPI_Status etat;

    MPI_Recv (&this->ambiantTemperature, 1, this->sectionDataType, (this->globalWidth * this->globalHeight), 0, MPI_COMM_WORLD, &etat);
}

void CSlave::sendTemperatureToCoordinator(){
    MPI_Send (&this->temperature, 1, this->sectionDataType, (this->globalWidth * this->globalHeight), 0, MPI_COMM_WORLD);
}


void CSlave::recvTemperaturesFromNeighbourhoud(){
    MPI_Status etat;
    int tempProcessNum = 0, tempPosX = 0, tempPosY = 0;
    
    for(int i = -1; i < 2; i ++){
        for(int j = -1; j < 2; j ++){
            
            tempPosX = this->positionX + i;
            tempPosY = this->positionY + j;
            tempProcessNum = (tempPosY * this->globalWidth) + tempPosX;
            
            // Si le process courrant est valide et est différent du process actuel, on envoie la température
            if(tempProcessNum != this->processNum && tempPosX >= 0 && tempPosX < this->globalWidth && tempPosY >= 0 && tempPosY < this->globalHeight){
                
               MPI_Recv (&this->neighbourhoudTemperatures[j+1][i+1], 1, this->sectionDataType, tempProcessNum, 0, MPI_COMM_WORLD, &etat);
                
              // cout << "Slave " << this->processNum << " : Recoie temperature de slave " << tempProcessNum << endl;
                
            }else if(tempProcessNum != this->processNum){
                this->neighbourhoudTemperatures[j+1][i+1] = this->ambiantTemperature;
            }
        }
    }
}

void CSlave::sendTemperatureToNeighbourhoud(){
    int tempProcessNum = 0, tempPosX = 0, tempPosY = 0;
    MPI_Request request;
    
    
    for(int i = -1; i < 2; i ++){
        for(int j = -1; j < 2; j ++){
            tempPosX = this->positionX + i;
            tempPosY = this->positionY + j;
            tempProcessNum = (tempPosY * this->globalWidth) + tempPosX;
            
            // Si le process courrant est valide et est différent du process actuel, on envoie la température
            if(tempProcessNum != this->processNum && tempPosX >= 0 && tempPosX < this->globalWidth && tempPosY >= 0 && tempPosY < this->globalHeight){
                MPI_Isend (&this->temperature, 1, this->sectionDataType, tempProcessNum, 0, MPI_COMM_WORLD, &request);
                
                // cout << "Slave " << this->processNum << " : Envoie temperature a slave " << tempProcessNum << endl;
            }
            
        }
    }
}

/**
Calcul les températures de l'ensemble des sous-carrés de la section
*/
void CSlave::calcTemperature(){
    t_section_datatype tempoTemperature;
    
    #pragma omp parallel shared (tempoTemperature)
    {
        for(int i=0; i<SECTION_SIZE*SECTION_SIZE; i++){
            int x = i % SECTION_SIZE;
            int y = (i - x) / SECTION_SIZE;

            tempoTemperature.temperature[y][x] = calcSubTemperature(x, y);
        }
    }
    
    this->temperature = tempoTemperature;
}

/**
Calcul la température d'un sous carré en fonction des sous carrés voisins
    x : position du carré en X dans la section
    y : position du carré en Y dans la section
*/
int CSlave::calcSubTemperature(int x, int y){
    int sumOfTemperatures = 0;
    int currentX = 0, currentY = 0;

    for(int offsetY = -1; offsetY < 2; offsetY ++){
        for(int offsetX = -1; offsetX < 2; offsetX ++){
            currentX = x + offsetX;
            currentY = y + offsetY;
            
            if(currentX < 0 && currentY < 0){
                sumOfTemperatures += this->neighbourhoudTemperatures[0][0].temperature[SECTION_SIZE-1][SECTION_SIZE-1];
                
            }else if(currentX < 0 && currentY >= SECTION_SIZE){
                sumOfTemperatures += this->neighbourhoudTemperatures[2][0].temperature[0][SECTION_SIZE-1];
                
            }else if(currentX < 0) {
                sumOfTemperatures += this->neighbourhoudTemperatures[1][0].temperature[currentY][SECTION_SIZE-1];                
                
            }else if(currentX >= SECTION_SIZE && currentY < 0){
                sumOfTemperatures += this->neighbourhoudTemperatures[0][2].temperature[SECTION_SIZE-1][0];
                
            }else if(currentX >= SECTION_SIZE && currentY >= SECTION_SIZE){
                sumOfTemperatures += this->neighbourhoudTemperatures[2][2].temperature[0][0];
                
            }else if(currentX >= SECTION_SIZE) {
                sumOfTemperatures += this->neighbourhoudTemperatures[1][2].temperature[currentY][0];                
                
            }else if(currentY < 0){
                sumOfTemperatures += this->neighbourhoudTemperatures[0][1].temperature[SECTION_SIZE-1][currentX];
                
            }else if(currentY >= SECTION_SIZE){
                sumOfTemperatures += this->neighbourhoudTemperatures[2][1].temperature[0][currentX];
                
            }else{
                sumOfTemperatures += this->temperature.temperature[currentY][currentX];
            
            }
        }
    }

    return sumOfTemperatures / 9;
}




MPI_Datatype CSlave::getSectionDatatype ()
{
    MPI_Datatype typeSection;
    MPI_Datatype type = MPI_INT;
    int blocklen = SECTION_SIZE*SECTION_SIZE;
    MPI_Aint disp = 0;
    MPI_Type_struct (1, &blocklen, &disp, &type, &typeSection);
    MPI_Type_commit (&typeSection);
    return typeSection;
}