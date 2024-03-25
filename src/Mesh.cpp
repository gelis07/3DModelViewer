#include "Mesh.h"
void Mesh::Draw(Shader* Program)
{
    Vp.Bind();
    mat->SetUniforms("mat", Program);
    glDrawElements(GL_TRIANGLES, Vp.GetIndCount(), GL_UNSIGNED_INT, 0);
    Vp.Unbind();
}
