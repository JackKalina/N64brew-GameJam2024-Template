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
        }
    }
};


sprite_t* c_up;
sprite_t* c_down;
sprite_t* c_left;
sprite_t* c_right;

std::deque<Note> current_cu;
std::deque<Note> current_cd;
std::deque<Note> current_cl;
std::deque<Note> current_cr;

int cu_idx;
int cd_idx;
int cl_idx;
int cr_idx;


/*==============================
    minigame_init
    The minigame initialization function
==============================*/
extern "C" void minigame_init()
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);
    font = rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR);
    rdpq_text_register_font(FONT_TEXT, font);

    // Create deque for tracks encapsulate somewhere

    c_up = sprite_load("rom:/core/CUp.sprite");
    c_down = sprite_load("rom:/core/CDown.sprite");
    c_left = sprite_load("rom:/core/CLeft.sprite");
    c_right = sprite_load("rom:/core/CRight.sprite");
    
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

float elapsed;
/*==============================
    minigame_loop
    Code that is called every loop.
    @param  The delta time for this tick
==============================*/
extern "C" void minigame_loop(float deltatime)
{
    rdpq_attach(display_get(), NULL);
    rdpq_clear(color_from_packed32(GAME_BACKGROUND));
    rdpq_set_mode_copy(true);

    if ((int)freebird.tracks[0].size() > cu_idx && elapsed + 3 >= freebird.tracks[0][cu_idx].time + 3) {
        current_cu.push_back(freebird.tracks[0][cu_idx]);
        cu_idx++;
    }
    if ((int)freebird.tracks[1].size() > cd_idx && elapsed + 3 >= freebird.tracks[1][cd_idx].time + 3) {
        current_cd.push_back(freebird.tracks[1][cd_idx]);
        cd_idx++;
    }
    
    for (Note c : current_cu) {
        rdpq_sprite_blit(c_up, 100, (elapsed - c.time) * 20, NULL);
    }
     for (Note c : current_cd) {
        rdpq_sprite_blit(c_down, 120, (elapsed - c.time) * 20, NULL);
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