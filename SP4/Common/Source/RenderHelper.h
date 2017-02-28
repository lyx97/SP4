#ifndef RENDER_HELPER_H
#define RENDER_HELPER_H

#include <list>
#include "Vertex.h"
#include "MatrixStack.h"

class Mesh;

class RenderHelper
{
public:
	static void RenderMesh(Mesh* _mesh);
	static void RenderMeshWithLight(Mesh* _mesh);
    static void RenderText(Mesh* _mesh, const std::string& _text, Color _color);

private:
};

#endif // RENDER_HELPER_H