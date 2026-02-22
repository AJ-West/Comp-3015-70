#pragma once

#include "../../helper/drawable.h"
#include "../../helper/trianglemesh.h"

class Cube : public TriangleMesh
{
public:
    Cube(GLfloat size = 1.0f);
};
