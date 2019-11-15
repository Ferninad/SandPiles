#include "common.h"
#include "cmath"
#include "vector"

bool Init();
void CleanUp();
void Run();
void Draw();
double ScaleNum(double n, double minN, double maxN, double min, double max);

SDL_Window *window;
SDL_GLContext glContext;
SDL_Surface *gScreenSurface = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Rect pos;

int screenWidth = 500;
int screenHeight = 500;
int randx;
int randy;
int maxSand = 10000000;
int gridSize = 2;

vector<vector<int>> current;
vector<vector<int>> previous;

bool Init()
{
    if (SDL_Init(SDL_INIT_NOPARACHUTE & SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
        return false;
    }
    else
    {
        //Specify OpenGL Version (4.2)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_Log("SDL Initialised");
    }

    //Create Window Instance
    window = SDL_CreateWindow(
        "Game Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,   
        SDL_WINDOW_OPENGL);

    //Check that the window was succesfully created
    if (window == NULL)
    {
        //Print error, if null
        printf("Could not create window: %s\n", SDL_GetError());
        return false;
    }
    else{
        gScreenSurface = SDL_GetWindowSurface(window);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_Log("Window Successful Generated");
    }
    //Map OpenGL Context to Window
    glContext = SDL_GL_CreateContext(window);

    return true;
}

int main()
{
    //Error Checking/Initialisation
    if (!Init())
    {
        printf("Failed to Initialize");
        return -1;
    }

    // Clear buffer with black background
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //Swap Render Buffers
    SDL_GL_SwapWindow(window);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    Run();

    CleanUp();
    return 0;
}

void CleanUp()
{
    //Free up resources
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Run()
{
    bool gameLoop = true;
    srand(time(NULL));
    vector<int> temp;
    for(int x = 0; x < screenWidth / gridSize; x++){
        for(int y = 0; y < screenHeight / gridSize; y++){
            temp.push_back(0);
        }
        current.push_back(temp);
        previous.push_back(temp);
    }
    Draw();
    previous[screenWidth / gridSize / 2][screenHeight / gridSize / 2] = maxSand;
    while (gameLoop)
    {   
        Draw();
        SDL_RenderPresent(renderer);
        pos.x = 0;
        pos.y = 0;
        pos.w = screenWidth;
        pos.h = screenHeight;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &pos);
        
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                gameLoop = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        gameLoop = false;
                        break;
                    default:
                        break;
                }
            }

            if (event.type == SDL_KEYUP)
            {
                switch (event.key.keysym.sym){
                    case SDLK_SPACE:
                        // randx = rand() % (screenWidth / gridSize - 20) + 10;
                        // randy = rand() % (screenHeight / gridSize - 20) + 10;
                        //previous[randx][randy] = 500;
                        previous[screenWidth / gridSize / 2][screenHeight / gridSize / 2] = maxSand;
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

void Draw(){
    current = previous;
    for(int x = 0; x < screenWidth / gridSize; x++){
        for(int y = 0; y < screenHeight / gridSize; y++){
            int temp = 0;
            if(previous[x][y] >= 4){
                current[x][y]-=4;
                if(x-1 < 0){}
                else
                    current[x-1][y]++;
                if(x+1 > screenWidth / gridSize - 1){}
                else
                    current[x+1][y]++;
                if(y-1 < 0){}
                else
                    current[x][y-1]++;
                if(y+1 > screenHeight / gridSize - 1){}
                else
                    current[x][y+1]++;
            }
            double color = current[x][y];
            if(color > 3)
                color = 3;
            pos.x = x * gridSize;
            pos.y = y * gridSize;
            pos.w = gridSize;
            pos.h = gridSize;
            if(color == 0)
                SDL_SetRenderDrawColor(renderer, 0, 85, 255, 255);
            else if(color == 1)
                SDL_SetRenderDrawColor(renderer, 0, 188, 255, 255);
            else if(color == 2)
                SDL_SetRenderDrawColor(renderer, 255, 205, 0, 255);
            else if(color == 3)
                SDL_SetRenderDrawColor(renderer, 192, 125, 10, 255);
            SDL_RenderFillRect(renderer, &pos);
        }
    }
    vector<vector<int>> temp = previous;
    previous = current;
    current = temp;
}

double ScaleNum(double n, double minN, double maxN, double min, double max){
    return (((n - minN) / (maxN - minN)) * (max - min)) + min;
}