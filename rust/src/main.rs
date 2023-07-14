extern crate sdl2;

use sdl2::{event, keyboard, pixels, rect, render, video};
use std::{path, time};

pub struct Player<'a> {
    /* Player Object */
    pub texture: &'a render::Texture<'a>,
    pub speed: i32,
    pub srcrect: rect::Rect,
    pub dstrect: rect::Rect,
}

// Dimensions of the level
const LEVEL_WIDTH: u32 = 640;
const LEVEL_HEIGHT: u32 = 400;

// Dimensions of the player
const PLAYER_WIDTH: u32 = 20;
const PLAYER_HEIGHT: u32 = 20;

fn main() -> Result<(), String> {
    /* Run the Game */
    // Dimensions of the level, player, and tree as 32 bit integer data type
    let level_width: i32 = LEVEL_WIDTH as i32;
    let level_height: i32 = LEVEL_HEIGHT as i32;
    let player_width: i32 = PLAYER_WIDTH as i32;
    let player_height: i32 = PLAYER_HEIGHT as i32;

    // Frames per second of the game
    let fps: u32 = 60;

    // The speed of the player
    let player_speed: i32 = 2;

    // Image file paths
    let player_image_path: &path::Path = path::Path::new("player.bmp");

    // Initializes the SDL library
    let sdl: sdl2::Sdl = sdl2::init()?;

    // Initializes the audio subsystem
    let _audio: sdl2::AudioSubsystem = sdl.audio()?;

    // Initializes the video subsystem
    let video_subsystem: sdl2::VideoSubsystem = sdl.video()?;

    // Initializes a Window
    let window: video::Window = video_subsystem
        .window("venture", LEVEL_WIDTH, LEVEL_HEIGHT)
        .position_centered()
        .build()
        .map_err(|e| e.to_string())?;

    // Set the renderer to use hardware acceleration
    // This improves the game's performance (FPS)
    let mut canvas: render::Canvas<video::Window> = window
        .into_canvas()
        .accelerated()
        .build()
        .map_err(|e| e.to_string())?;

    // texture loader
    let texture_creator: render::TextureCreator<sdl2::video::WindowContext> =
        canvas.texture_creator();

    let player_surface = sdl2::surface::Surface::load_bmp(player_image_path)?;

    /* Player */
    let player_texture: render::Texture<'_> = texture_creator
        .create_texture_from_surface(player_surface)
        .map_err(|e| e.to_string())?;

    let player_srcrect: rect::Rect = rect::Rect::new(0, 0, PLAYER_WIDTH, PLAYER_HEIGHT);
    let player_dstrect: rect::Rect = rect::Rect::new(
        level_width / 4 - player_width / 2,
        level_height / 2 - player_height / 2,
        PLAYER_WIDTH,
        PLAYER_HEIGHT,
    );

    let mut player: Player<'_> = Player {
        texture: &player_texture,
        speed: player_speed,
        srcrect: player_srcrect,
        dstrect: player_dstrect,
    };

    /* Game loop */
    'running: loop {
        /* Click Keybindings */
        for event in sdl.event_pump()?.poll_iter() {
            match event {
                event::Event::Quit { .. }
                | event::Event::KeyDown {
                    keycode: Option::Some(keyboard::Keycode::Escape),
                    ..
                } => break 'running,
                _ => {}
            }
        }

        /* Hold Keybindings */
        let event: sdl2::EventPump = sdl.event_pump()?;

        /* Player Movement Keybindings */
        if event
            .keyboard_state()
            .is_scancode_pressed(keyboard::Scancode::Right)
        {
            // player moves right
            player.dstrect.x += player.speed;
        }
        if event
            .keyboard_state()
            .is_scancode_pressed(keyboard::Scancode::Left)
        {
            // player moves left
            player.dstrect.x -= player.speed;
        }
        if event
            .keyboard_state()
            .is_scancode_pressed(keyboard::Scancode::Up)
        {
            // player moves up
            player.dstrect.y -= player.speed;
        }
        if event
            .keyboard_state()
            .is_scancode_pressed(keyboard::Scancode::Down)
        {
            // player moves down
            player.dstrect.y += player.speed;
        }

        /* Player boundaries */
        if player.dstrect.x < 0 {
            // left boundary
            player.dstrect.x = 0;
        }
        if player.dstrect.x + player.dstrect.w > level_width {
            // right boundary
            player.dstrect.x = level_width - player.dstrect.w;
        }
        if player.dstrect.y + player.dstrect.h > level_height {
            // bottom boundary
            player.dstrect.y = level_height - player.dstrect.h;
        }
        if player.dstrect.y < 0 {
            // top boundary
            player.dstrect.y = 0;
        }

        /* Canvas renders the textures and background */
        canvas.set_draw_color(pixels::Color::RGB(134, 191, 255));
        canvas.clear();

        // Render the player texture
        canvas.copy(player.texture, player.srcrect, player.dstrect)?;

        canvas.present();

        // Set the frames per second to 60
        std::thread::sleep(time::Duration::new(0, 1_000_000_000u32 / fps));
    }

    Ok(())
}
