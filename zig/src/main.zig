/// Game Window using SDL2
const c = @cImport({
    @cInclude("SDL2/SDL.h");
});

const std = @import("std");

const level_width: i32 = 640;
const level_height: i32 = 400;
const player_width: u32 = 20;
const player_height: u32 = 20;
const player_speed: u32 = 2;

const Player = struct {
    srcrect: c.SDL_Rect,
    dstrect: c.SDL_Rect,
    texture: ?*c.SDL_Texture,
};

pub fn main() !void {
    // Initialize the SDL library
    const sdl_status: c_int = c.SDL_Init(c.SDL_INIT_VIDEO);
    defer c.SDL_Quit();

    if (sdl_status == -1) {
        std.debug.print("SDL_Init Error\n", .{});
    }

    // Create window
    const window: ?*c.SDL_Window = c.SDL_CreateWindow("SDL2 Window", c.SDL_WINDOWPOS_CENTERED, c.SDL_WINDOWPOS_CENTERED, level_width, level_height, 0);
    defer c.SDL_DestroyWindow(window);

    // Create renderer
    const rend: ?*c.SDL_Renderer = c.SDL_CreateRenderer(window, 0, c.SDL_RENDERER_ACCELERATED);
    defer c.SDL_DestroyRenderer(rend);

    // Create player surface
    const player_surface: [*]c.SDL_Surface = c.SDL_LoadBMP("player.bmp");
    defer c.SDL_FreeSurface(player_surface);

    // Create player texture
    const player_texture: ?*c.SDL_Texture = c.SDL_CreateTextureFromSurface(rend, player_surface);
    defer c.SDL_DestroyTexture(player_texture);

    // Source and destination rectangle of the player
    const player_srcrect: c.SDL_Rect = c.SDL_Rect{ .x = 0, .y = 0, .w = player_width, .h = player_height };
    var player_dstrect: c.SDL_Rect = c.SDL_Rect{ .x = 20, .y = 20, .w = player_width, .h = player_height };

    var player = Player{ .srcrect = player_srcrect, .dstrect = player_dstrect, .texture = player_texture };

    // Set the background color to white
    _ = c.SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);

    mainloop: while (true) {
        // Game loop
        var event: c.SDL_Event = undefined;
        while (c.SDL_PollEvent(&event) != 0) {
            switch (event.type) {
                c.SDL_QUIT => break :mainloop,
                c.SDL_KEYDOWN => {
                    if (event.key.keysym.scancode == c.SDL_SCANCODE_ESCAPE) {
                        break :mainloop;
                    }
                },
                else => {},
            }
        }

        // Hold Movement Keybindings
        var state: [*]const u8 = c.SDL_GetKeyboardState(null);
        if (state[c.SDL_SCANCODE_RIGHT] == 1) {
            // move the player right
            player.dstrect.x += player_speed;
        }
        if (state[c.SDL_SCANCODE_LEFT] == 1) {
            // move the player left
            player.dstrect.x -= player_speed;
        }
        if (state[c.SDL_SCANCODE_DOWN] == 1) {
            // move the player down
            player.dstrect.y += player_speed;
        }
        if (state[c.SDL_SCANCODE_UP] == 1) {
            // move the player up
            player.dstrect.y -= player_speed;
        }

        // Player boundaries
        if (player.dstrect.x < 0) {
            // left boundary
            player.dstrect.x = 0;
        }
        if (player.dstrect.x + player.dstrect.w > level_width) {
            // right boundary
            player.dstrect.x = level_width - player.dstrect.w;
        }
        if (player.dstrect.y + player.dstrect.h > level_height) {
            // bottom boundary
            player.dstrect.y = level_height - player.dstrect.h;
        }
        if (player.dstrect.y < 0) {
            // top boundary
            player.dstrect.y = 0;
        }

        // Clear renderer
        _ = c.SDL_RenderClear(rend);

        // Render the player
        _ = c.SDL_RenderCopy(rend, player.texture, &player.srcrect, &player.dstrect);

        // Updates the screen (renderer)
        c.SDL_RenderPresent(rend);

        // Calculates to 60 fps
        // 1000ms equals 1s
        const miliseconds: i32 = 1000;
        const gameplay_frames: i32 = 60;
        c.SDL_Delay(miliseconds / gameplay_frames);
    }
}
