#include "SpriteAnimation.h"
#include "GL\glew.h"
#include "Vertex.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../EntityManager.h"

SpriteAnimation::SpriteAnimation()
    : Mesh("")
    , m_timeCurrent(0)
    , m_currentFrame(0)
    , m_playCount(0)
    , m_attackFrame(-1)
    , m_attack(false)
{
    m_anim = NULL;
}

SpriteAnimation::SpriteAnimation(const std::string& meshName)
    : Mesh(meshName)
	, m_timeCurrent(0)
	, m_currentFrame(0)
	, m_playCount(0)
    , m_attackFrame(-1)
    , m_attack(false)
{
	m_anim = NULL;
}

SpriteAnimation::~SpriteAnimation()
{
	if (m_anim)
		delete m_anim;
}

void SpriteAnimation::Update(double dt)
{
	if (m_anim->animActive) // If animation happens
	{
		m_timeCurrent += static_cast<float>(dt);
		
		int numFrame = Math::Max(1, m_anim->endFrame - m_anim->startFrame + 1);
		float frameTime = m_anim->animTime / numFrame;

		m_currentFrame = Math::Min(m_anim->endFrame, m_anim->startFrame + static_cast<int>(m_timeCurrent / frameTime));
		
        if (m_currentFrame == m_attackFrame && m_attackFrame > -1)
            m_attack = true;
        else
            m_attack = false;

		if (m_timeCurrent >= m_anim->animTime)
		{
			m_anim->ended = true;
			if (m_anim->repeatCount == 0)
			{
				m_anim->animActive = false;
				m_timeCurrent = 0.0f;
				m_currentFrame = m_anim->startFrame;
			}
			if (m_anim->repeatCount == 1)
			{
				m_timeCurrent = 0.0f;
				m_currentFrame = m_anim->startFrame;
			}
		}
	}
}

void SpriteAnimation::Render()
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));
    //if(textureID > 0)
    {
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));
    }

    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

    if (mode == DRAW_LINES)
        glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));
    else if (mode == DRAW_TRIANGLE_STRIP)
        glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));
    else
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    //if(textureID > 0)
    {
        glDisableVertexAttribArray(3);
    }
}