#include "UBO.h"

UBO::UBO()
{
    glGenBuffers(1, &_ID);
    glBindBuffer(GL_UNIFORM_BUFFER, _ID);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW); // projection mat, view mat
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, _ID, 0, 2 * sizeof(glm::mat4));
}

void UBO::update(UpdateContent content)
{
    glBindBuffer(GL_UNIFORM_BUFFER, _ID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &content.projection_matrix[0][0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &content.view_matrix[0][0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UBO::setup(std::shared_ptr<Shader> shader)
{
    shader->use();
    shader->setBlockBind("Matrices", 0);
    shader->unUse();
}
