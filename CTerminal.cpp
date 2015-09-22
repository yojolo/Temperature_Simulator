#include "CTerminal.h"

CTerminal::CTerminal(){
    
}


void CTerminal::initWindow(){
    /* Création de la fenêtre et du renderer */
    SDL_CreateWindowAndRenderer(800, 800, 0, &this->window, &this->renderer);
    clearWindow();
}

void CTerminal::renderWindow(){
    SDL_RenderPresent (this->renderer);
    SDL_Delay(500);
}

void CTerminal::clearWindow(){
    // Select the color for drawing. It is set to red here.
    SDL_SetRenderDrawColor(this->renderer, 50, 50, 50, 255);
    // Clear the entire screen to our selected color.
    SDL_RenderClear(this->renderer);
}

void CTerminal::closeWindow(){
    SDL_Quit();
}

void CTerminal::createTemperatureSection(t_section_datatype temperatureSection, int size, int x, int y){
    
    float offsetGlobalX = ((SQUARE_SIZE+1) * size * x) + 100;
    float offsetGlobalY = ((SQUARE_SIZE+1) * size * y) + 100;
    
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            t_color tempColor = getColorByTemperature(temperatureSection.temperature[i][j]);
            SDL_SetRenderDrawColor(this->renderer, tempColor.r, tempColor.g ,tempColor.b, 255);
            
            float offsetX = offsetGlobalX + (j * SQUARE_SIZE);
            float offsetY = offsetGlobalY + (i * SQUARE_SIZE);
            
            SDL_Rect tempRect;
            tempRect.x = offsetX;
            tempRect.y = offsetY;
            tempRect.w = SQUARE_SIZE;
            tempRect.h = SQUARE_SIZE;
            
        
            SDL_RenderFillRect(this->renderer, &tempRect);
        }
    }
}

t_color CTerminal::getColorByTemperature(int temperature){
    t_color color;
    
    if(temperature < this->minTemperature){
        temperature = this->minTemperature;
    }
    if(temperature > this->maxTemperature){
        temperature = this->maxTemperature;
    }

    float hue = (float)(temperature - this->minTemperature) / (this->maxTemperature - this->minTemperature);
    
    color.r = hue * 200 + 55;
    color.g = (1-hue) * 200 + 55;
    color.b = (1-hue) * 200 + 55;
    
    return color;
}