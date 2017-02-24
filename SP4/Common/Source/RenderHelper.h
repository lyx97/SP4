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
    static void RenderParticleMesh(Mesh* _mesh);

    static void StoreParticlesMVP(void);

private:
    static std::list<Mtx44> MVPList;
};

#endif // RENDER_HELPER_H