#include <libdragon.h>
#include <vector>
#include <string>
#include "../../core.h"
#include "../../minigame.h"

using namespace std;

extern const MinigameDef minigame_def = {
    .gamename = "n64hero",
    .developername = "Team Kinnock",
    .description = "This is an example game.",
    .instructions = "Press A to win."
};


typedef struct
{
    int time;
    int duration;
} Note;

typedef struct {
    string name;
    std::vector<Note> track_cu;
    std::vector<Note> track_cr;
    std::vector<Note> track_cd;
    std::vector<Note> track_cl;
} Song;

const Song freebird = {
    "Freebird",
    {
        {0, 0},
        {1000, 0},
        {2000, 0},
        {3000, 1000}
    },
    {
        {0, 0},
        {1000, 0},
        {2000, 0},
        {3000, 1000}
    }
};

/*==============================
    minigame_init
    The minigame initialization function
==============================*/
extern "C" void minigame_init()
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE);
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

/*==============================
    minigame_loop
    Code that is called every loop.
    @param  The delta time for this tick
==============================*/
extern "C" void minigame_loop(float deltatime)
{

}

/*==============================
    minigame_cleanup
    Clean up any memory used by your game just before it ends.
==============================*/
extern "C" void minigame_cleanup()
{

}