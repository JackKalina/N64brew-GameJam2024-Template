#include "scene.h"

Scene::Scene() {
    m_viewport = t3d_viewport_create();
    t3d_vec3_norm(&m_lightDirection);
    t3d_viewport_set_projection(&m_viewport, T3D_DEG_TO_RAD(85.0f), 10.0f, 150.0f);
}

Scene::~Scene() {}

void Scene::addObject(GameObject* obj) { m_objects.push_back(obj); }
void Scene::addLight(Light* light) { m_lights.push_back(light); }

void Scene::registerLights() {
    int lightIndex = 0;
    for (const auto& light : m_lights) {
        if (auto* pointLight = dynamic_cast<PointLight*>(light)) {
            t3d_light_set_point(lightIndex, pointLight->m_lightColor, &pointLight->m_position, pointLight->m_size,
                                pointLight->m_ignoreNormals);
        } else if (auto* directionalLight = dynamic_cast<DirectionalLight*>(light)) {
            t3d_light_set_directional(lightIndex, directionalLight->m_lightColor, &directionalLight->m_lightDirection);
        }
        lightIndex++;
    }
}

void Scene::draw() {
    // Attach the display and z-buffer to the RDPQ and start the frame
    rdpq_attach(display_get(), display_get_zbuf());
    t3d_frame_start();
    t3d_viewport_attach(&m_viewport);

    // Clear the screen
    t3d_screen_clear_color(RGBA32(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]));
    t3d_screen_clear_depth();

    // Set up the lights
    t3d_light_set_ambient(m_ambientLightColor);
    registerLights();
    t3d_light_set_count(m_lights.size());

    // Set up the camera
    t3d_viewport_look_at(&m_viewport, &m_camPos, &m_camTarget, &m_upVector);

    // Draw the objects
    for (auto obj : m_objects) {
        obj->draw();
    }
}