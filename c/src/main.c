#include <SDL2/SDL.h>
#include <stdbool.h>

#define LEVEL_WIDTH 750
#define LEVEL_HEIGHT 500

struct Player {
	SDL_Texture *PlayerTex; // player texture
    SDL_Rect dstrect; // player destination
    SDL_Rect srcrect;       // player source from the player spritesheet
	int speed;        // horizontal and vertical velocity
};

int main() {
	/* Frames per second */
	const int miliseconds = 1000;   // 1000 ms equals 1s
	const int gameplay_frames = 60; // amount of frames per second

    /* Player Attributes */
    const int player_width = 20;
    const int player_height = 20;
    const int player_speed = 2;   // speed of player
    const int player_offset = 50; // gap between left corner of the window

    /* Paths to the assets of the game */
    const char *player_path = "player.bmp";

    /* Initialize SDL, window, audio, and renderer */
    int sdl_status = SDL_Init(SDL_INIT_VIDEO); // Initialize SDL library

    if (sdl_status == -1) {
        printf("SDL_Init: %s\n", SDL_GetError());
    }

    // Create window
    SDL_Window *win = SDL_CreateWindow("Myzdin", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LEVEL_WIDTH, LEVEL_HEIGHT, 0);

    // Creates a renderer to render the images
    // * SDL_RENDERER_SOFTWARE starts the program using the CPU hardware
    // * SDL_RENDERER_ACCELERATED starts the program using the GPU hardware
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(rend, 134, 191, 255, 255);

    /* Loads images, music, and soundeffects */
    // Creates the asset that loads the image into main memory
    SDL_Surface *PlayerSurf = SDL_LoadBMP(player_path);

    // Loads image to our graphics hardware memory
    SDL_Texture *PlayerTex = SDL_CreateTextureFromSurface(rend, PlayerSurf);

	SDL_Rect dstrect = {0 + player_offset, LEVEL_HEIGHT - player_height - player_offset, player_width, player_height};
	SDL_Rect srcrect = {0, 0, player_width, player_height};

    struct Player player = {PlayerTex, dstrect, srcrect, player_speed};

    /* Gameplay Loop */
    // GamePlay(rend, player, gamecontroller); // Start movement and physics
    bool quit = false; // gameplay loop switch

    while (!quit) { // gameplay loop
        /* Keybindings */
        /* Click Key Bindings */
        SDL_Event event; // Event handling

        while (SDL_PollEvent(&event) == 1) { // Events management
            switch (event.type) {
            case SDL_QUIT: // close button
                quit = true;
                break;
            case SDL_KEYDOWN: // key press
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    quit = true;
                }
                break;

            case SDL_CONTROLLERBUTTONDOWN: // controller button press
                if (event.cbutton.button == SDL_CONTROLLER_BUTTON_START) {
                    quit = true;
                }
            default:
                break;
            }
        }

        /* Hold Keybindings */
        // Get the snapshot of the current state of the keyboard
        const Uint8 *state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_LEFT] == 1) { // move player left
            player.dstrect.x -= player.speed;
        }

		else if (state[SDL_SCANCODE_RIGHT] == 1) { // move player right
            player.dstrect.x += player.speed;
        }

        if (state[SDL_SCANCODE_UP] == 1) { // move player up
            player.dstrect.y -= player.speed;
        } 

		else if (state[SDL_SCANCODE_DOWN] == 1) { // move player down
            player.dstrect.y += player.speed;
        }

        /* Player boundaries */
        if (player.dstrect.x < 0) {
        	// left boundary
            player.dstrect.x = 0;
        }
        if (player.dstrect.x + player.dstrect.w > LEVEL_WIDTH) {
        	// right boundary
            player.dstrect.x = LEVEL_WIDTH - player.dstrect.w;
        }
        if (player.dstrect.y + player.dstrect.h > LEVEL_HEIGHT) {
        	// bottom boundary
            player.dstrect.y = LEVEL_HEIGHT - player.dstrect.h;
        }
        if (player.dstrect.y < 0) {
        	// top boundary
            player.dstrect.y = 0;
        }

        SDL_RenderClear(rend);

        SDL_RenderCopy(rend, player.PlayerTex, &player.srcrect, &player.dstrect);

        SDL_RenderPresent(rend); // Triggers double buffers for multiple rendering
        SDL_Delay(miliseconds / gameplay_frames); // Calculates to 60 fps
    }

    /* Free resources and close SDL and SDL mixer */
    // Deallocate player and scene surfaces
    SDL_FreeSurface(PlayerSurf);

    // Destroy scene and player textures
    SDL_DestroyTexture(PlayerTex);

    // Destroy renderer
    SDL_DestroyRenderer(rend);

    // Destroy window
    SDL_DestroyWindow(win);
	
	// Quit SDL subsystems
    SDL_Quit();

    return 0;
}
