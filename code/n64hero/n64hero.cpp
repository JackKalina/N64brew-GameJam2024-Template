#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>

#include "../../core.h"
#include "../../minigame.h"
#include "./game_object.h"
#include "./light.h"
#include "./scene.h"
#include "./tracks.h"

using namespace std;

extern const MinigameDef minigame_def = {.gamename = "n64hero",
                                         .developername = "Team Kinnock",
                                         .description = "This is an example game.",
                                         .instructions = "Press A to win."};

Scene* scene;
GameObject* myModel;
DirectionalLight* light;
float rotAngle = 0.0f;

/*==============================
    minigame_init
    The minigame initialization function
==============================*/
extern "C" void minigame_init() {
#ifdef DEBUG
    debug_init_isviewer();
    debug_init_usblog();
    asset_init_compression(2);
    dfs_init(DFS_DEFAULT_LOCATION);
#endif
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE_ANTIALIAS);
    rdpq_init();
    t3d_init((T3DInitParams){});

    scene = new Scene();

    myModel = new GameObject("rom:/n64hero/model.t3dm");
    scene->addObject(myModel);

    light = new DirectionalLight({0xEE, 0xAA, 0xAA, 0xFF}, {{-1.0f, 1.0f, 1.0f}});
    scene->addLight(light);
}

/*==============================
    minigame_fixedloop
    Code that is called every loop, at a fixed delta time.
    Use this function for stuff where a fixed delta time is
    important, like physics.
    @param  The fixed delta time for this tick
==============================*/
extern "C" void minigame_fixedloop(float deltatime) {}

/*==============================
    minigame_loop
    Code that is called every loop.
    @param  The delta time for this tick
==============================*/
extern "C" void minigame_loop(float deltatime) {
    // ======== Update ======== //
    rotAngle -= 0.02f;
    myModel->setTransform((T3DVec3){{0, 0, 0}}, 0.1f, (T3DVec3){{0, rotAngle, 0}});

    // ======== Draw ======== //

    scene->draw();

    // ======== End ======== //

    rdpq_detach_show();
}

/*==============================
    minigame_cleanup
    Clean up any memory used by your game just before it ends.
==============================*/
extern "C" void minigame_cleanup() { t3d_destroy(); }