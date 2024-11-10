#ifndef LIGHT_H
#define LIGHT_H

#include <t3d/t3d.h>

#include <iostream>

// Base Light class
class Light {
   public:
    unsigned char m_lightColor[4];

    Light(const unsigned char (&lightColor)[4]);
    virtual ~Light();
    virtual void displayType() const = 0;
};

// Derived DirectionalLight class
class DirectionalLight : public Light {
   public:
    T3DVec3 m_lightDirection;

    DirectionalLight(const unsigned char (&lightColor)[4], const T3DVec3& lightDirection);
    ~DirectionalLight() override;
    void displayType() const override;
};

// Derived PointLight class
class PointLight : public Light {
   public:
    T3DVec3 m_position;
    float m_size;
    bool m_ignoreNormals;

    PointLight(const unsigned char (&lightColor)[4], const T3DVec3& position, float size, bool ignoreNormals);
    ~PointLight() override;
    void displayType() const override;
};

#endif  // LIGHT_H