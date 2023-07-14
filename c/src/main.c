#include <SDL2/SDL.h>
/*#include <stdbool.h>*/

#define LEVEL_WIDTH 750
#define LEVEL_HEIGHT 500

struct Player {
    SDL_Texture *texture; /* player texture */
    SDL_Rect dstrect; /* player destination */
    SDL_Rect srcrect;       /* player source from the player spritesheet */
    int speed;        /* horizontal and vertical velocity */
};

int main() {
    /* Frames per second */
    /* 1000 ms equals 1s */
    const int miliseconds = 1000;
    /* amount of frames per second */
    const int gameplay_frames = 60;

    /* Player Attributes */
    const int player_width = 20;
    const int player_height = 20;
    const int player_speed = 2;   /* speed of player */
    const int player_offset = 50; /* gap between left corner of the window */
    /* Paths to the assets of the game */
    const char *player_path = "./player.bmp";

    int sdl_status;
    SDL_Window *win;
    SDL_Renderer *rend;
    SDL_Surface *player_surface;
    SDL_Texture *player_texture;
    SDL_Rect player_dstrect;
    SDL_Rect player_srcrect;
    struct Player player;
    int quit = 0; /* gameplay loop switch */
    const Uint8 *state;

    /* Initialize SDL Library */
    sdl_status = SDL_Init(SDL_INIT_VIDEO);

    if (sdl_status == -1) {
        printf("SDL_Init: %s\n", SDL_GetError());
    }

    /* Create window */
    win = SDL_CreateWindow("Myzdin", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LEVEL_WIDTH, LEVEL_HEIGHT, 0);

    /* Creates a renderer to render the images
    * SDL_RENDERER_SOFTWARE starts the program using the CPU hardware
    * SDL_RENDERER_ACCELERATED starts the program using the GPU hardware
    */
    rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(rend, 134, 191, 255, 255);

    /* Creates the asset that loads the image into main memory */
    player_surface = SDL_LoadBMP(player_path);

    /* Loads image to our graphics hardware memory */
    player_texture = SDL_CreateTextureFromSurface(rend, player_surface);

    player_dstrect.x = 0 + player_offset;
    player_dstrect.y = LEVEL_HEIGHT - player_height - player_offset;
    player_dstrect.w = player_width;
    player_dstrect.h = player_height;

    player_srcrect.x = 0;
    player_srcrect.y = 0;
    player_srcrect.w = player_width;
    player_srcrect.h = player_height;

    player.texture = player_texture;
    player.dstrect = player_dstrect;
    player.srcrect = player_srcrect;
    player.speed = player_speed;

    while (!quit) { 
        /* Gameplay Loop */
        /* Keybindings */
        /* Click Key Bindings */
        SDL_Event event; /* Event handling */

        while (SDL_PollEvent(&event) == 1) { /* Events management */
            switch (event.type) {
            case SDL_QUIT: /* close button */
                quit = 1;
                break;
            case SDL_KEYDOWN: /* key press */
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    quit = 1;
                }
                break;

            case SDL_CONTROLLERBUTTONDOWN: /* controller button press */
                if (event.cbutton.button == SDL_CONTROLLER_BUTTON_START) {
                    quit = 1;
                }
            default:
                break;
            }
        }

        /* Hold Keybindings */
        /* Get the snapshot of the current state of the keyboard */
        state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_LEFT] == 1) { 
            /* move player left */
            player.dstrect.x -= player.speed;
        }

        else if (state[SDL_SCANCODE_RIGHT] == 1) { 
            /* move player right */
            player.dstrect.x += player.speed;
        }

        if (state[SDL_SCANCODE_UP] == 1) { 
            /* move player up */
            player.dstrect.y -= player.speed;
        } 

        else if (state[SDL_SCANCODE_DOWN] == 1) { 
            /* move player down */
            player.dstrect.y += player.speed;
        }

        /* Player boundaries */
        if (player.dstrect.x < 0) {
            /* left boundary */
            player.dstrect.x = 0;
        }
        if (player.dstrect.x + player.dstrect.w > LEVEL_WIDTH) {
            /* right boundary */
            player.dstrect.x = LEVEL_WIDTH - player.dstrect.w;
        }
        if (player.dstrect.y + player.dstrect.h > LEVEL_HEIGHT) {
            /* bottom boundary */
            player.dstrect.y = LEVEL_HEIGHT - player.dstrect.h;
        }
        if (player.dstrect.y < 0) {
            /* top boundary */
            player.dstrect.y = 0;
        }

        SDL_RenderClear(rend);

        SDL_RenderCopy(rend, player.texture, &player.srcrect, &player.dstrect);

		/* Triggers double buffers for multiple rendering */
        SDL_RenderPresent(rend);

		/* Calculates to 60 fps */
        SDL_Delay(miliseconds / gameplay_frames);
    }

    /* Deallocate player and scene surfaces */
    SDL_FreeSurface(player_surface);

    /* Destroy scene and player textures */
    SDL_DestroyTexture(player_texture);

    /* Destroy renderer */
    SDL_DestroyRenderer(rend);

    /* Destroy window */
    SDL_DestroyWindow(win);

    /* Quit SDL subsystems */
    SDL_Quit();

    return 0;
}
