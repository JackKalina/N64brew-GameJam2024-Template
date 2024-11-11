#include <libdragon.h>
#include <vector>
#include <queue>
#include <string>
#include "../../core.h"
#include "../../minigame.h"
#include "song.h"

using namespace std;

#define FONT_TEXT           1
#define GAME_BACKGROUND     0x000000FF

extern const MinigameDef minigame_def = {
    .gamename = "n64hero",
    .developername = "Team Kinnock",
    .description = "This is an example game.",
    .instructions = "Press A to win."
};

rdpq_font_t *font;

const Song freebird = {
    "Freebird",
    {
        {
            {0, 0},
            {2, 0},
            {4, 0},
            {6, 1000},
            {9, 1000}
        },
        {
            {1, 0},
            {3, 0},
            {5, 0},
            {9, 1000}
        },
        {
            {3, 0},
            {8, 0},
        },
        {
            {4, 0},
        }
    }
};

typedef struct
{
    PlyNum plynum;
} player_data;
player_data players[MAXPLAYERS];

typedef struct {
    sprite_t* sprite;
    uint16_t button;
} track_data;

sprite_t* c_up;
sprite_t* c_down;
sprite_t* c_left;
sprite_t* c_right;

std::vector <track_data> tracks;

SongTracker* tracker;

void player_init(player_data *player)
{

}

/*==============================
    minigame_init
    The minigame initialization function
==============================*/
extern "C" void minigame_init()
{
    // Configure each track 8,4,2,1 correspond to bit values in joypad_buttons_t type
    tracks = { 
        { sprite_load("rom:/core/CUp.sprite"), 8 }, 
        { sprite_load("rom:/core/CDown.sprite"), 4}, 
        { sprite_load("rom:/core/CLeft.sprite"), 2}, 
        { sprite_load("rom:/core/CRight.sprite"), 1} 
    };

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);
    font = rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR);
    rdpq_text_register_font(FONT_TEXT, font);

    tracker = new SongTracker(freebird);

    for (size_t i = 0; i < MAXPLAYERS; i++)
    {
        player_init(&players[i]);
        players[i].plynum = PlyNum(i);
    }
}

/*==============================
    minigame_fixedloop
    Code that is called every loop, at a fixed delta time.
    Use this function for stuff where a fixed delta time is 
    important, like physics.
    @param  The fixed delta time for this tick
==============================*/
extern "C" void minigame_fixedloop(float deltatime)
{

}

float elapsed = -3;

void player_loop(player_data *player, float deltaTime, joypad_port_t port, bool is_human)
{
    if (is_human) {
        joypad_buttons_t btn = joypad_get_buttons_pressed(port);
        
        if (btn.start) minigame_end();
    }
}

void player_draw(player_data *player, joypad_port_t port)
{
    int y = 140;
    if (player->plynum > 1) {
        y = 180;
    }
    
    joypad_buttons_t held = joypad_get_buttons_held(port);
    rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, 10 + (100 * (player->plynum % 2)), y, "Player %d", port + 1);

    rdpq_set_mode_copy(true);
    
    for (size_t i = 0; i < tracks.size(); i++) {
        if (tracks[i].button & held.raw) {
            rdpq_sprite_blit(tracks[i].sprite, 10 + (100 * player->plynum % 2) + (i * 20), y + 10, NULL);
        }
    }
}



/*==============================
    minigame_loop
    Code that is called every loop.
    @param  The delta time for this tick
==============================*/
extern "C" void minigame_loop(float deltatime)
{
    rdpq_attach(display_get(), NULL);
    rdpq_clear(color_from_packed32(GAME_BACKGROUND));
    
    uint32_t playercount = core_get_playercount();
    for (size_t i = 0; i < MAXPLAYERS; i++)
    {
        player_loop(&players[i], deltatime, core_get_playercontroller(PlyNum(i)), i < playercount);
    }

    tracker->tick(deltatime);

    rdpq_set_mode_fill(RGBA32(0xFF, 0x00, 0x00, 0));
	rdpq_fill_rectangle(100, 100, 200, 110);

    rdpq_set_mode_copy(true);
    int t = 0;
    for(const deque<Note> &track: tracker->current_notes) {
        for (Note c : track) {
            rdpq_sprite_blit(tracks[t].sprite, 100 + (t * 20), 100 + (elapsed - c.time) * 20, NULL);
        }
        t++;
    }

    for (size_t i = 0; i < MAXPLAYERS; i++)
    {
        player_draw(&players[i], core_get_playercontroller(PlyNum(i)));
    }

    elapsed += deltatime;
    rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, 10, 10, "Elapsed %f", elapsed);
    rdpq_detach_show();
}

/*==============================
    minigame_cleanup
    Clean up any memory used by your game just before it ends.
==============================*/
extern "C" void minigame_cleanup()
{

}