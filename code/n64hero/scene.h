#ifndef SCENE_H
#define SCENE_H

#include <libdragon.h>
#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>

#include <vector>

#include "game_object.h"
#include "light.h"

class Scene {
   private:
    T3DVec3 m_camPos = {{0, 10.0f, 40.0f}};
    T3DVec3 m_camTarget = {{0, 0, 0}};
    unsigned char m_clearColor[4] = {255, 80, 80, 0xFF};
    unsigned char m_ambientLightColor[4] = {80, 80, 100, 0xFF};
    unsigned char m_lightColor[4] = {0xEE, 0xAA, 0xAA, 0xFF};
    T3DVec3 m_lightDirection = {{-1.0f, 1.0f, 1.0f}};
    T3DVec3 m_upVector = {{0, 1, 0}};

    std::vector<GameObject*> m_objects;
    std::vector<Light*> m_lights;

   public:
    T3DViewport m_viewport;
    Scene();
    ~Scene();

    void addObject(GameObject* obj);
    void addLight(Light* light);
    void registerLights();
    void draw();
};

#endif  // SCENE_H