#include <libdragon.h>
#include <vector>
#include <queue>
#include <string>
#include <cmath>

#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>
#include <t3d/t3dskeleton.h>
#include <t3d/t3danim.h>
#include <t3d/t3ddebug.h>

#include "../../core.h"
#include "../../minigame.h"

#include "song.h"

using namespace std;

#define FONT_TEXT           1
#define GAME_BACKGROUND     0x000000FF

// Songs
const Song freebird = {
    "Freebird",
    {
        {
            {0, 0},
            {2, 0},
            {4, 0},
            {6, 1000},
            {9, 1000},
            {10, 0},
            {11, 0},
            {12, 0},
            {13, 1000},
            {14, 1000},
            {15, 1000},
            {16, 1000},
            {17, 1000},
            {18, 1000},
            {19, 1000},
            {20, 1000},
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

extern const MinigameDef minigame_def = {
    .gamename = "n64hero",
    .developername = "Team Kinnock",
    .description = "This is an example game.",
    .instructions = "Press A to win."
};

rdpq_font_t *font;

T3DModel *model;
surface_t *depthBuffer;
T3DViewport viewport;
T3DVec3 camPos;
T3DVec3 camTarget;
T3DVec3 lightDirVec;

typedef struct
{
    PlyNum plynum;

    // Gameplay state tracking
    vector<Note> last_note;
    vector<float> last_note_time;
    int score;
    int streak;

    T3DVec3 moveDir;
    T3DVec3 playerPos;

    // Model data
    T3DMat4FP* modelMatFP;
    rspq_block_t *dplPlayer;

    // Animations
    T3DAnim animIdle;
    T3DAnim animRun;
    T3DAnim animDance;
    vector<T3DAnim> animDances;
    T3DSkeleton skelBlend;
    T3DSkeleton skel;

    float animBlend;
    bool isDancing;

} player_data;
player_data players[MAXPLAYERS];

typedef struct {
    int n;
    sprite_t* sprite;
    uint16_t button;
} TrackConfig;

std::vector <TrackConfig> tracks;

SongTracker* tracker;
float elapsed = -3;

void player_init(player_data *player, color_t color, T3DVec3 position)
{
    player->last_note = vector<Note>(tracks.size());
    player->last_note_time = vector<float>(tracks.size());

    player->modelMatFP = new T3DMat4FP;

    player->moveDir = (T3DVec3){{0,0,0}};
    player->playerPos = position;

     // First instantiate skeletons, they will be used to draw models in a specific pose
    // And serve as the target for animations to modify
    player->skel = t3d_skeleton_create(model);
    player->skelBlend = t3d_skeleton_clone(&player->skel, false); // optimized for blending, has no matrices


    player->animIdle = t3d_anim_create(model, "Idle");
    t3d_anim_attach(&player->animIdle, &player->skel); // tells the animation which skeleton to modify
    
    player->animRun = t3d_anim_create(model, "Run");
    t3d_anim_attach(&player->animRun, &player->skelBlend); // tells the animation which skeleton to modify
    

    // Load animations for each track input
    for (size_t i = 0; i < tracks.size(); i++) {
        const string anim = "dance_" + std::to_string(i);
        player->animDances.push_back(t3d_anim_create(model, anim.c_str()));
        t3d_anim_set_looping(&player->animDances[i], false); // don't loop this animation
        t3d_anim_set_playing(&player->animDances[i], false); // start in a paused state
        t3d_anim_attach(&player->animDances[i], &player->skelBlend);
    }

    rspq_block_begin();
        t3d_matrix_push(player->modelMatFP);
        rdpq_set_prim_color(color);
        t3d_model_draw_skinned(model, &player->skel); // as in the last example, draw skinned with the main skeleton
        
        // rdpq_set_prim_color(RGBA32(0, 0, 0, 120));
        // t3d_model_draw(modelShadow);
        t3d_matrix_pop(1);
    player->dplPlayer = rspq_block_end();
    
    player->animBlend = 0.0f;
    player->animBlend = 1.0f;
}

/*==============================
    minigame_init
    The minigame initialization function
==============================*/
extern "C" void minigame_init()
{
    // Configure each track 8,4,2,1 correspond to bit values in joypad_buttons_t type
    tracks = { 
        { 0, sprite_load("rom:/core/CUp.sprite"), 8 }, 
        { 1, sprite_load("rom:/core/CDown.sprite"), 4}, 
        { 2, sprite_load("rom:/core/CLeft.sprite"), 2}, 
        { 3, sprite_load("rom:/core/CRight.sprite"), 1} 
    };

    // Player colors
    const color_t colors[] = {
        PLAYERCOLOR_1,
        PLAYERCOLOR_2,
        PLAYERCOLOR_3,
        PLAYERCOLOR_4,
    };

    T3DVec3 start_positions[] = {
        (T3DVec3){{-100,0.15f,0}},
        (T3DVec3){{-100,0.15f,-100}},
        (T3DVec3){{100,0.15f,0}},
        (T3DVec3){{100,0.15f,-100}},
    };

    model = t3d_model_load("rom:/n64hero/player.t3dm");

    tracker = new SongTracker(freebird);

   

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE_ANTIALIAS);
    depthBuffer = display_get_zbuf();

    t3d_init((T3DInitParams){});

    font = rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_VAR);
    rdpq_text_register_font(FONT_TEXT, font);

    viewport = t3d_viewport_create();

    camPos = (T3DVec3){{0, 100.0f, 100.0f}};
    camTarget = (T3DVec3){{0, 0, 40}};

    lightDirVec = (T3DVec3){{1.0f, 1.0f, 1.0f}};
    t3d_vec3_norm(&lightDirVec);

    for (size_t i = 0; i < MAXPLAYERS; i++)
    {
        player_init(&players[i], colors[i], start_positions[i]);
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



void player_loop(player_data *player, float deltaTime, joypad_port_t port, bool is_human)
{
    if (is_human) {
        joypad_buttons_t btn = joypad_get_buttons_pressed(port);
        
        if (btn.start) minigame_end();

        // Check if any track buttons were pressed
        for(const TrackConfig cfg: tracks) {
            if (cfg.button & btn.raw) {
                deque<Note> notes = tracker->current_notes[cfg.n];
                
                // If no notes in current timeline view there is a mishit
                if (notes.size() == 0) {
                    // Miss
                    debugf("Miss track %d\n", cfg.n);
                    player->streak = 0;
                } else {
                    Note candidate = notes.front();
                    if (fabs(candidate.time - tracker->elapsed) < .25) {
                        // Hit
                        player->streak++;
                        player->last_note[cfg.n] = candidate;
                        player->score += min(4, (player->streak / 10) + 1);
                        debugf("Hit track %d\n", cfg.n);
                    } else {
                        // Miss
                        player->streak = 0;
                        debugf("Miss track %d\n", cfg.n);
                    }
                }

                if (!player->animDance.isPlaying) {
                    player->animDance = player->animDances[cfg.n];
                    t3d_anim_set_playing(&player->animDance, true);
                    t3d_anim_set_time(&player->animDance, 0.0f);
                    player->isDancing = true;
                }
            }
        }
    }

    t3d_anim_update(&player->animIdle, deltaTime);
    t3d_anim_set_speed(&player->animRun, player->animBlend + 0.15f);
    t3d_anim_update(&player->animRun, deltaTime);
    if (player->isDancing) {
        t3d_anim_update(&player->animDance, deltaTime);
        if(!player->animDance.isPlaying)player->isDancing = false;
    }

    t3d_skeleton_blend(&player->skel, &player->skel, &player->skelBlend, player->animBlend);

    t3d_skeleton_update(&player->skel);

    // Update player matrix
    t3d_mat4fp_from_srt_euler(player->modelMatFP,
        (float[3]){0.2f, 0.2f, 0.2f},
        (float[3]){0.0f, 0, 0},
        player->playerPos.v
    );
}

void player_draw(player_data *player, joypad_port_t port)
{
    int y = 140;
    if (player->plynum > 1) {
        y = 180;
    }   
    joypad_buttons_t held = joypad_get_buttons_held(port);
    rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, 10 + (100 * (player->plynum % 2)), y, "Player %d : %d", port + 1, player->score);
    
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
    tracker->tick(deltatime);
    
    uint32_t playercount = core_get_playercount();
    for (size_t i = 0; i < MAXPLAYERS; i++)
    {
        player_loop(&players[i], deltatime, core_get_playercontroller(PlyNum(i)), i < playercount);
    }

    uint8_t colorAmbient[4] = {0xAA, 0xAA, 0xAA, 0xFF};
    uint8_t colorDir[4]     = {0xFF, 0xAA, 0xAA, 0xFF};

    t3d_viewport_set_projection(&viewport, T3D_DEG_TO_RAD(90.0f), 20.0f, 160.0f);
    T3DVec3 up = {0, 1, 0};
    t3d_viewport_look_at(&viewport, &camPos, &camTarget, &up);

    // ======== Draw (3D) ======== //
    rdpq_attach(display_get(), depthBuffer);
    t3d_frame_start();
    t3d_viewport_attach(&viewport);

    t3d_screen_clear_color(RGBA32(224, 180, 96, 0xFF));
    t3d_screen_clear_depth();

    t3d_light_set_ambient(colorAmbient);
    t3d_light_set_directional(0, colorDir, &lightDirVec);
    t3d_light_set_count(1);

    for (size_t i = 0; i < MAXPLAYERS; i++)
    {
        player_data *player = &players[i];
        rspq_block_run(player->dplPlayer);
    }

    for (size_t i = 0; i < MAXPLAYERS; i++)
    {
        player_draw(&players[i], core_get_playercontroller(PlyNum(i)));
    }

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