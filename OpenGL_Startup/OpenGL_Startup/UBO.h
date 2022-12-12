#pragma once
#include "glad/glad.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

class UBO
{
public:
    typedef struct UpdateContent {
        glm::mat4 projection_matrix;
        glm::mat4 view_matrix;
    };

    UBO();
    void update(UpdateContent content);
    void setup(std::shared_ptr<Shader> shader);

private:
    GLuint _ID;
};

