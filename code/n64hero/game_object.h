#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <t3d/t3d.h>
#include <t3d/t3dmath.h>
#include <t3d/t3dmodel.h>

#include "./math_utils.h"

class GameObject {
   private:
    T3DModel* m_model;
    T3DVec3 m_position;
    T3DVec3 m_scale;
    T3DVec3 m_rotation;
    T3DMat4 m_modelMat;
    T3DMat4FP* m_modelMatFP;
    rspq_block_t* m_drawRoutine = nullptr;

   public:
    GameObject(const char* t3dmPath);
    ~GameObject();

    T3DVec3 position();
    T3DVec3 scale();
    T3DVec3 rotation();

    void setTransform(T3DVec3 position, T3DVec3 scale, T3DVec3 rotation);
    void setTransform(T3DVec3 position, T3DVec3 scale, float rotAngle, T3DVec3 rotAxis);
    void setTransform(T3DVec3 position, float scale, float rotAngle, T3DVec3 rotAxis);
    void setTransform(T3DVec3 position, float scale, T3DVec3 rotation);

    void draw();
};

#endif  // GAMEOBJECT_H