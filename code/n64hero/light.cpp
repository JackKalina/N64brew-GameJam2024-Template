#include "light.h"

#include <algorithm>

// Constructor for Light class
Light::Light(const unsigned char (&lightColor)[4]) { std::copy(std::begin(lightColor), std::end(lightColor), m_lightColor); }

// Destructor for Light class
Light::~Light() {}

// Constructor for DirectionalLight class
DirectionalLight::DirectionalLight(const unsigned char (&lightColor)[4], const T3DVec3& lightDirection)
    : Light(lightColor), m_lightDirection(lightDirection) {}

// Destructor for DirectionalLight class
DirectionalLight::~DirectionalLight() {}

// Display type for DirectionalLight
void DirectionalLight::displayType() const { std::cout << "This is a DirectionalLight.\n"; }

// Constructor for PointLight class
PointLight::PointLight(const unsigned char (&lightColor)[4], const T3DVec3& position, float size, bool ignoreNormals)
    : Light(lightColor), m_position(position), m_size(size), m_ignoreNormals(ignoreNormals) {}

// Destructor for PointLight class
PointLight::~PointLight() {}

// Display type for PointLight
void PointLight::displayType() const { std::cout << "This is a PointLight.\n"; }