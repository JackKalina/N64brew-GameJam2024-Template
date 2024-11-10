#include "game_object.h"

GameObject::GameObject(const char* t3dmPath) {
    m_model = t3d_model_load(t3dmPath);
    t3d_mat4_identity(&m_modelMat);
    m_modelMatFP = (T3DMat4FP*)malloc_uncached(sizeof(T3DMat4FP));
}

GameObject::~GameObject() {
    t3d_model_free(m_model);
    free(m_modelMatFP);
}

T3DVec3 GameObject::position() { return m_position; }

T3DVec3 GameObject::scale() { return m_scale; }

T3DVec3 GameObject::rotation() { return m_rotation; }

void GameObject::setTransform(T3DVec3 position, T3DVec3 scale, T3DVec3 rotation) {
    t3d_mat4_from_srt_euler(&m_modelMat, scale.v, rotation.v, position.v);
    t3d_mat4_to_fixed(m_modelMatFP, &m_modelMat);
}

void GameObject::setTransform(T3DVec3 position, T3DVec3 scale, float rotAngle, T3DVec3 rotAxis) {
    setTransform(position, scale, axisAngleToEuler(rotAxis, rotAngle));
}

void GameObject::setTransform(T3DVec3 position, float scale, T3DVec3 rotation) {
    setTransform(position, (T3DVec3){{scale, scale, scale}}, rotation);
}

void GameObject::setTransform(T3DVec3 position, float scale, float rotAngle, T3DVec3 rotAxis) {
    setTransform(position, (T3DVec3){{scale, scale, scale}}, axisAngleToEuler(rotAxis, rotAngle));
}

void GameObject::draw() {
    if (!m_drawRoutine) {
        rspq_block_begin();
        t3d_matrix_push(m_modelMatFP);
        t3d_model_draw(m_model);
        t3d_matrix_pop(1);
        m_drawRoutine = rspq_block_end();
    }
    rspq_block_run(m_drawRoutine);
}