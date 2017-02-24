#include "RenderHelper.h"
#include "Mesh.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "GL\glew.h"

std::list<Mtx44> RenderHelper::MVPList;

void RenderHelper::RenderMesh(Mesh* _mesh)
{
	// Get all our transform matrices & update shader
	Mtx44 MVP;
	MVP = GraphicsManager::GetInstance()->GetProjectionMatrix() * GraphicsManager::GetInstance()->GetViewMatrix() * GraphicsManager::GetInstance()->GetModelStack().Top();
	ShaderProgram* currProg = GraphicsManager::GetInstance()->GetActiveShader();
	currProg->UpdateMatrix44("MVP", &MVP.a[0]);

	// Disable lighting stuff
	currProg->UpdateInt("lightEnabled", 0);

	// Update textures first if available
	if (_mesh->textureID > 0)
	{
		currProg->UpdateInt("colorTextureEnabled", 1);
		GraphicsManager::GetInstance()->UpdateTexture(0, _mesh->textureID);
		currProg->UpdateInt("colorTexture", 0);
	}
	else
	{
		currProg->UpdateInt("colorTextureEnabled", 0);
	}

	// Do actual rendering
	_mesh->Render();

	// Unbind texture for safety (in case next render call uses it by accident)
	if (_mesh->textureID > 0)
	{
		GraphicsManager::GetInstance()->UnbindTexture(0);
	}
}

void RenderHelper::RenderMeshWithLight(Mesh* _mesh)
{
	// Get all our transform matrices & update shader
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	MVP = GraphicsManager::GetInstance()->GetProjectionMatrix() * GraphicsManager::GetInstance()->GetViewMatrix() * GraphicsManager::GetInstance()->GetModelStack().Top();
	ShaderProgram* currProg = GraphicsManager::GetInstance()->GetActiveShader();
	currProg->UpdateMatrix44("MVP", &MVP.a[0]);

	// Update light stuff
	currProg->UpdateInt("lightEnabled", 1);
	modelView = GraphicsManager::GetInstance()->GetViewMatrix() * GraphicsManager::GetInstance()->GetModelStack().Top();
	currProg->UpdateMatrix44("MV", &modelView.a[0]);
	modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
	currProg->UpdateMatrix44("MV_inverse_transpose", &modelView.a[0]);

	//load material
	currProg->UpdateVector3("material.kAmbient", &_mesh->material.kAmbient.r);
	currProg->UpdateVector3("material.kDiffuse", &_mesh->material.kDiffuse.r);
	currProg->UpdateVector3("material.kSpecular", &_mesh->material.kSpecular.r);
	currProg->UpdateFloat("material.kShininess", _mesh->material.kShininess);
	
	// Update textures first if available
	if (_mesh->textureID > 0)
	{
		currProg->UpdateInt("colorTextureEnabled", 1);
		GraphicsManager::GetInstance()->UpdateTexture(0, _mesh->textureID);
		currProg->UpdateInt("colorTexture", 0);
	}
	else
	{
		currProg->UpdateInt("colorTextureEnabled", 0);
	}

	// Do actual rendering
	_mesh->Render();

	// Unbind texture for safety (in case next render call uses it by accident)
	if (_mesh->textureID > 0)
	{
		GraphicsManager::GetInstance()->UnbindTexture(0);
	}
}

void RenderHelper::RenderText(Mesh* _mesh, const std::string& _text, Color _color)
{
	// Trivial Rejection : Unable to render without mesh or texture
	if (!_mesh || _mesh->textureID <= 0)
		return;

    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	ShaderProgram* currProg = GraphicsManager::GetInstance()->GetActiveShader();

	currProg->UpdateInt("textEnabled", 1);
	currProg->UpdateVector3("textColor", &_color.r);
	currProg->UpdateInt("lightEnabled", 0);
	currProg->UpdateInt("colorTextureEnabled", 1);
	
	currProg->UpdateInt("colorTextureEnabled", 1);
	GraphicsManager::GetInstance()->UpdateTexture(0, _mesh->textureID);
	currProg->UpdateInt("colorTexture", 0);

    float currentOffset = 0;

	for (unsigned i = 0; i < _text.length(); ++i)
	{
		Mtx44 characterSpacing, MVP;
		//characterSpacing.SetToTranslation((i+0.5f) * 1.0f, 0, 0); // 1.0f is the spacing of each character, you may change this value
		characterSpacing.SetToTranslation((float)(1.0f + ((int)i * .5f)), 0.0f, 0.0f); // 1.0f is the spacing of each character, you may change this value
		MVP = GraphicsManager::GetInstance()->GetProjectionMatrix() * GraphicsManager::GetInstance()->GetViewMatrix() * GraphicsManager::GetInstance()->GetModelStack().Top() * characterSpacing;
		currProg->UpdateMatrix44("MVP", &MVP.a[0]);

		_mesh->Render((unsigned)_text[i] * 6, 6);
	}

	GraphicsManager::GetInstance()->UnbindTexture(0);
	currProg->UpdateInt("textEnabled", 0);
    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
}

void RenderHelper::StoreParticlesMVP(void)
{
    Mtx44 MVP;
    MVP = GraphicsManager::GetInstance()->GetProjectionMatrix() * GraphicsManager::GetInstance()->GetViewMatrix() * GraphicsManager::GetInstance()->GetModelStack().Top();
    MVPList.push_back(MVP);
}

void RenderHelper::RenderParticleMesh(Mesh* _mesh)
{
    ShaderProgram* currProg = GraphicsManager::GetInstance()->GetActiveShader();

    // Disable lighting stuff
    currProg->UpdateInt("lightEnabled", 0);

    // Update textures first if available
    if (_mesh->textureID > 0)
    {
        currProg->UpdateInt("colorTextureEnabled", 1);
        GraphicsManager::GetInstance()->UpdateTexture(0, _mesh->textureID);
        currProg->UpdateInt("colorTexture", 0);
    }
    else
    {
        currProg->UpdateInt("colorTextureEnabled", 0);
    }

    // VertexAttribArray
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, _mesh->vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));
    
    if (_mesh->textureID > 0)
    {
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));
    }

    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->indexBuffer);

    // Get all our transform matrices & update shader
    std::list<Mtx44>::iterator it, end = MVPList.end();

    for (it = MVPList.begin(); it != end; ++it)
    {
        currProg->UpdateMatrix44("MVP", &(*it).a[0]);

        // Do actual rendering
        _mesh->RenderParticle();
    }

    if (_mesh->textureID > 0)
    {
        glDisableVertexAttribArray(3);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    // Unbind texture for safety (in case next render call uses it by accident)
    if (_mesh->textureID > 0)
    {
        GraphicsManager::GetInstance()->UnbindTexture(0);
    }

    MVPList.clear();
}