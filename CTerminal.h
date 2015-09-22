#ifndef TERMINAL_H
#define	TERMINAL_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"

#include "SectionDataType.h"

typedef struct {
    int r;
    int g;
    int b;  
}t_color;

#define SQUARE_SIZE 50

class CTerminal {
    public:
        int maxTemperature;
        int minTemperature;
    
    private:
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
    
    public:
        CTerminal();
    
        void initWindow();
        void renderWindow();
        void clearWindow();
        void closeWindow();
        void createTemperatureSection(t_section_datatype temperatureSection, int size, int x, int y);
        t_color getColorByTemperature(int temperature);
    
};

#endif