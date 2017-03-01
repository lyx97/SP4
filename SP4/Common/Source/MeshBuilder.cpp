#include "MeshBuilder.h"
#include "Mesh.h"
#include <GL\glew.h>
#include <vector>
#include "MyMath.h"
#include "LoadOBJ.h"
#include "LoadTGA.h"
#include <iostream>
#include "../../Base/Source/EntityManager.h"

using namespace std;
/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	v.pos.Set(-1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, -1000, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 1000, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, -1000);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, 1000);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);

	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	AddMesh(meshName, mesh);

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a crosshair;
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCrossHair(const std::string &meshName, float colour_r, float colour_g, float colour_b, float length)
{
	// Declare the variable to store a vertex and the buffer for storing vertices
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	// Vertex #1
	v.pos.Set(-length, 0, 0);
	v.color.Set(colour_r, colour_g, colour_b);
	vertex_buffer_data.push_back(v);
	// Vertex #2
	v.pos.Set(length, 0, 0);
	v.color.Set(colour_r, colour_g, colour_b);
	vertex_buffer_data.push_back(v);
	// Vertex #3
	v.pos.Set(0, -length, 0);
	v.color.Set(colour_r, colour_g, colour_b);
	vertex_buffer_data.push_back(v);
	// Vertex #4
	v.pos.Set(0, length, 0);
	v.color.Set(colour_r, colour_g, colour_b);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);

	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	AddMesh(meshName, mesh);

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float length)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	v.pos.Set(-0.5f * length,-0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length,-0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(1.0f, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(1.0f, 1.0f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 1.0f);
	vertex_buffer_data.push_back(v);
	
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);
	
	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	AddMesh(meshName, mesh);

	return mesh;
}

Mesh* MeshBuilder::GenerateTriangle(const std::string &meshName, Color color, float length)
{
    Vertex v;
    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;

    v.pos.Set(-0.5f * length, -0.5f * length, 0);
    v.color = color;
    v.normal.Set(0, 0, 1);
    v.texCoord.Set(0, 0);
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, -0.5f * length, 0);
    v.color = color;
    v.normal.Set(0, 0, 1);
    v.texCoord.Set(1.0f, 0);
    vertex_buffer_data.push_back(v);
    v.pos.Set(0.5f * length, 0.5f * length, 0);
    v.color = color;
    v.normal.Set(0, 0, 1);
    v.texCoord.Set(1.0f, 1.0f);
    vertex_buffer_data.push_back(v);

    index_buffer_data.push_back(1);
    index_buffer_data.push_back(2);
    index_buffer_data.push_back(0);

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLES;

    AddMesh(meshName, mesh);

    return mesh;
}

Mesh* MeshBuilder::GenerateParticle(const std::string &meshName, Color color, float length)
{
    Vertex v;
    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;

    std::list<Vector3>::iterator it, end = EntityManager::particlePosList.end();
    int i = 0;
    for (it = EntityManager::particlePosList.begin(); it != end; ++it)
    {
        v.pos.Set((-0.5f * length) + (*it).x, (-0.5f * length) + (*it).z, 0);
        v.color = color;
        v.normal.Set(0, 0, 1);
        v.texCoord.Set(0, 0);
        vertex_buffer_data.push_back(v);
        v.pos.Set((0.5f * length) + (*it).x, (-0.5f * length) + (*it).z, 0);
        v.color = color;
        v.normal.Set(0, 0, 1);
        v.texCoord.Set(1.0f, 0);
        vertex_buffer_data.push_back(v);
        v.pos.Set((0.5f * length) + (*it).x, (0.5f * length) + (*it).z, 0);
        v.color = color;
        v.normal.Set(0, 0, 1);
        v.texCoord.Set(1.0f, 1.0f);
        vertex_buffer_data.push_back(v);

        index_buffer_data.push_back(1 + i);
        index_buffer_data.push_back(2 + i);
        index_buffer_data.push_back(0 + i);
        i += 3;
    }

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_TRIANGLES;

    AddMesh(meshName, mesh);

    return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string &meshName, Color color, float length)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;

	v.pos.Set(-0.5f * length,-0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length,-0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length,-0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length,-0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	
	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(5);
	
	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = 36;
	mesh->mode = Mesh::DRAW_TRIANGLES;

	AddMesh(meshName, mesh);

	return mesh;
}

Mesh* MeshBuilder::GenerateRing(const std::string &meshName, Color color, unsigned numSlice, float outerR, float innerR)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	Vertex v;

	float degreePerSlice = 360.f / numSlice;
	//float radianPerSlice = Math::DegreeToRadian(360.f) / numSlice;
	for(unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		float theta = slice * degreePerSlice;
		v.color = color;
		v.normal.Set(0, 1, 0);
		v.pos.Set(outerR * cos(Math::DegreeToRadian(theta)), 0, outerR * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
		
		v.color = color;
		v.normal.Set(0, 1, 0);
		v.pos.Set(innerR * cos(Math::DegreeToRadian(theta)), 0, innerR * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
	}
	for(unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		index_buffer_data.push_back(2 * slice + 0);
		index_buffer_data.push_back(2 * slice + 1);
		//index_buffer_data.push_back(2 * slice + 3);
		//index_buffer_data.push_back(2 * slice + 4);
		//index_buffer_data.push_back(2 * slice + 3);
		//index_buffer_data.push_back(2 * slice + 2);
	}

	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	mesh->indexSize = index_buffer_data.size();

	AddMesh(meshName, mesh);

	return mesh;
}

float sphereX(float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta));
}
float sphereY(float phi, float theta)
{
	return sin(Math::DegreeToRadian(phi));
}
float sphereZ(float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta));
}

Mesh* MeshBuilder::GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	float degreePerStack = 180.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	//float radianPerSlice = Math::DegreeToRadian(360.f) / numSlice;

	for(unsigned stack = 0; stack < numStack + 1; ++stack) //stack //replace with 180 for sphere
	{
		float phi = -90.f + stack * degreePerStack;
		for(unsigned slice = 0; slice < numSlice + 1; ++slice) //slice
		{
			float theta = slice * degreePerSlice;
			Vertex v;
			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));
			v.color = color;
			v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));
			vertex_buffer_data.push_back(v);
		}
	}
	for(unsigned stack = 0; stack < numStack; ++stack)
	{
		for(unsigned slice = 0; slice < numSlice + 1; ++slice)
		{
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
			//index_buffer_data.push_back((numSlice + 1) * stack + slice + 1);
			//index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 1);
			//index_buffer_data.push_back((numSlice + 1) * stack + slice + 1);
			//index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}

	Mesh *mesh = new Mesh(meshName);

	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	AddMesh(meshName, mesh);

	return mesh;
}

Mesh* MeshBuilder::GenerateCone(const std::string &meshName, Color color, unsigned numSlice, float radius, float height)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	Vertex v;
	float degreePerSlice = 360.f / numSlice;

	for(unsigned slice = 0; slice < numSlice + 1; ++slice) //slice
	{
		float theta = slice * degreePerSlice;
		Vector3 normal(height * cos(Math::DegreeToRadian(theta)), radius, height * sin(Math::DegreeToRadian(theta)));
		normal.Normalize();

		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal = normal;
		vertex_buffer_data.push_back(v);
		
		v.pos.Set(0, height, 0);
		v.color = color;
		v.normal = normal;
		vertex_buffer_data.push_back(v);
	}
	for(unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		index_buffer_data.push_back(slice * 2 + 0);
		index_buffer_data.push_back(slice * 2 + 1);
	}

	Mesh *mesh = new Mesh(meshName);

	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	AddMesh(meshName, mesh);

	return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string &meshName, const std::string &file_path)
{
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if(!success)
		return NULL;

	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	Mesh *mesh = new Mesh(meshName);
	
	mesh->mode = Mesh::DRAW_TRIANGLES;
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	AddMesh(meshName, mesh);

	return mesh;
}

Mesh* MeshBuilder::GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	float width = 1.f / numCol;
	float height = 1.f / numRow;
	int offset = 0;
	for(unsigned i = 0; i < numRow; ++i)
	{
		for(unsigned j = 0; j < numCol; ++j)
		{
			float u1 = j * width;
			float v1 = 1.f - height - i * height;
			v.pos.Set(-0.5f, -0.5f, 0);
			v.texCoord.Set(u1, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, -0.5f, 0);
			v.texCoord.Set(u1 + width, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, 0.5f, 0);
			v.texCoord.Set(u1 + width, v1 + height);
			vertex_buffer_data.push_back(v);

			v.pos.Set(-0.5f, 0.5f, 0);
			v.texCoord.Set(u1, v1 + height);
			vertex_buffer_data.push_back(v);
	
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);
			offset += 4;
		}
	}
	
	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	AddMesh(meshName, mesh);

	return mesh;
}

Mesh* MeshBuilder::GenerateRay(const std::string &meshName, const float length)
{
    Vertex v;
    std::vector<Vertex> vertex_buffer_data;
    v.pos.Set(0, length, 0);
    v.color.Set(1, 0, 0);
    vertex_buffer_data.push_back(v);
    v.pos.Set(0, 0, 0);
    v.color.Set(1, 0, 0);
    vertex_buffer_data.push_back(v);

    std::vector<GLuint> index_buffer_data;
    index_buffer_data.push_back(0);
    index_buffer_data.push_back(1);

    Mesh *mesh = new Mesh(meshName);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex),
        &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint),
        &index_buffer_data[0], GL_STATIC_DRAW);

    mesh->indexSize = index_buffer_data.size();
    mesh->mode = Mesh::DRAW_LINES;

    AddMesh(meshName, mesh);

    return mesh;
}

SpriteAnimation* MeshBuilder::GenerateSpriteAnimation(const std::string &meshName, unsigned numRow, unsigned numCol)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float width = 1.f / numCol;
	float height = 1.f / numRow;
	int offset = 0;
	for (unsigned i = 0; i < numRow; ++i)
	{
		for (unsigned j = 0; j < numCol; ++j)
		{
			float u1 = j * width;
			float v1 = 1.f - height - i * height;
			v.pos.Set(-0.5f, -0.5f, 0);
			v.texCoord.Set(u1, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, -0.5f, 0);
			v.texCoord.Set(u1 + width, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, 0.5f, 0);
			v.texCoord.Set(u1 + width, v1 + height);
			vertex_buffer_data.push_back(v);

			v.pos.Set(-0.5f, 0.5f, 0);
			v.texCoord.Set(u1, v1 + height);
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);
			offset += 4;
		}
	}

	SpriteAnimation *animation = new SpriteAnimation(meshName);

	animation->mode = Mesh::DRAW_TRIANGLES;

	glBindBuffer(GL_ARRAY_BUFFER, animation->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, animation->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	animation->indexSize = index_buffer_data.size();

	AddMesh(meshName, animation);

	return animation;
}

SpriteAnimation* MeshBuilder::GenerateSpriteAnimation(unsigned numRow, unsigned numCol)
{
    Vertex v;
    std::vector<Vertex> vertex_buffer_data;
    std::vector<GLuint> index_buffer_data;

    float width = 1.f / numCol;
    float height = 1.f / numRow;
    int offset = 0;
    for (unsigned i = 0; i < numRow; ++i)
    {
        for (unsigned j = 0; j < numCol; ++j)
        {
            float u1 = j * width;
            float v1 = 1.f - height - i * height;
            v.pos.Set(-0.5f, -0.5f, 0);
            v.texCoord.Set(u1, v1);
            vertex_buffer_data.push_back(v);

            v.pos.Set(0.5f, -0.5f, 0);
            v.texCoord.Set(u1 + width, v1);
            vertex_buffer_data.push_back(v);

            v.pos.Set(0.5f, 0.5f, 0);
            v.texCoord.Set(u1 + width, v1 + height);
            vertex_buffer_data.push_back(v);

            v.pos.Set(-0.5f, 0.5f, 0);
            v.texCoord.Set(u1, v1 + height);
            vertex_buffer_data.push_back(v);

            index_buffer_data.push_back(offset + 0);
            index_buffer_data.push_back(offset + 1);
            index_buffer_data.push_back(offset + 2);
            index_buffer_data.push_back(offset + 0);
            index_buffer_data.push_back(offset + 2);
            index_buffer_data.push_back(offset + 3);
            offset += 4;
        }
    }

    SpriteAnimation *animation = new SpriteAnimation();

    animation->mode = Mesh::DRAW_TRIANGLES;

    glBindBuffer(GL_ARRAY_BUFFER, animation->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, animation->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

    animation->indexSize = index_buffer_data.size();

    return animation;
}

Mesh* MeshBuilder::GetMesh(const std::string& _meshName)
{
	if (meshMap.count(_meshName) != 0)
		return meshMap[_meshName];

	return nullptr;
}

void MeshBuilder::AddMesh(const std::string& _meshName, Mesh* _newMesh)
{
	// Trivial Rejection : Invalid pointer provided
	if (_newMesh == nullptr)
		return;

	// Clean up first if there is an existing mesh with the same name
	RemoveMesh(_meshName);

	// Add the mesh now
	meshMap[_meshName] = _newMesh;
}

void MeshBuilder::RemoveMesh(const std::string& _meshName)
{
	Mesh* currMesh = GetMesh(_meshName);
	if (currMesh != nullptr)
	{
		delete currMesh;
		meshMap.erase(_meshName);
	}
}

void MeshBuilder::Init()
{
	// MISC && DEBUG TESTS
	GenerateAxes("reference");
	GenerateCrossHair("crosshair");
	GenerateQuad("quad", Color(1, 1, 1), 1.f);
	GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	GenerateText("text", 16, 16);
	GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 1.f);
	GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
	GenerateQuad("GRIDMESH", Color(1.0f, 1.0f, 1.0f), 1.f);
	GenerateCube("cubeSG", Color(1.0f, 0.64f, 0.0f), 1.0f);
    //GenerateTriangle("particle", Color(1.0f, 1.0f, 1.0f), 1.f);

	// GAME GRAPHICS
	GenerateRay("laser", 10.0f);
	GenerateQuad("direction", Color(1.0f, 1.0f, 1.0f), 1.f);
	GetMesh("direction")->textureID = LoadTGA("Image//direction.tga");
    GenerateQuad("wall", Color(1.0f, 1.0f, 1.0f), 1.f);
	GetMesh("wall")->textureID = LoadTGA("Image//Tile//wall.tga");
    GenerateQuad("floor", Color(1.0f, 1.0f, 1.0f), 1.f);
    GetMesh("floor")->textureID = LoadTGA("Image//Tile//floor.tga");
	GenerateQuad("room", Color(0.5f, 0.5f, 0.5f), 1.f);

	GenerateQuad("powerup_maxhealth", Color(1.0f, 1.0f, 1.0f), 1.f);
	GetMesh("powerup_maxhealth")->textureID = LoadTGA("Image//Powerups//powerup_maxhealth.tga");
	GenerateQuad("powerup_healthrecover", Color(1.0f, 1.0f, 1.0f), 1.f);
	GetMesh("powerup_healthrecover")->textureID = LoadTGA("Image//Powerups//powerup_healthrecover.tga");
	GenerateQuad("powerup_healthregen", Color(1.0f, 1.0f, 1.0f), 1.f);
	GetMesh("powerup_healthregen")->textureID = LoadTGA("Image//Powerups//powerup_healthregen.tga");
	GenerateQuad("powerup_speed", Color(1.0f, 1.0f, 1.0f), 1.f);
	GetMesh("powerup_speed")->textureID = LoadTGA("Image//Powerups//powerup_speed.tga");
	GenerateQuad("treasure_texture", Color(1.0f, 1.0f, 1.0f), 1.f);
	GetMesh("treasure_texture")->textureID = LoadTGA("Image//Powerups//treasure_texture.tga");

    // PROJECTILE
    GenerateQuad("ghast", Color(1.0f, 1.0f, 1.0f), 1.f);
    GetMesh("ghast")->textureID = LoadTGA("Image//Projectile//ghast.tga");

    // PLAYER
	GenerateQuad("health_full", Color(0.0f, 0.5f, 0.0f), 1.f);
	GenerateQuad("health_half", Color(0.5f, 0.5f, 0.0f), 1.f);
	GenerateQuad("health_quad", Color(0.5f, 0.0f, 0.0f), 1.f);
	GenerateQuad("health_invin", Color(0.7f, 0.7f, 0.7f), 1.f);
	GenerateQuad("dash", Color(0.0f, 0.0f, 1.0f), 1.f);
	GenerateQuad("dreambar", Color(0.0f, 0.5f, 0.5f), 1.f);
	GenerateQuad("treasure", Color(1.0f, 0.8f, 0.0f), 1.f);
	GenerateQuad("border", Color(0.5f, 0.5f, 0.5f), 1.f);
	GenerateQuad("healthicon", Color(1.0f, 1.0f, 1.0f), 1.f);
	GetMesh("healthicon")->textureID = LoadTGA("Image//UI//health.tga");
	GenerateQuad("speedicon", Color(1.0f, 1.0f, 1.0f), 1.f);
	GetMesh("speedicon")->textureID = LoadTGA("Image//UI//speed.tga");
	GenerateQuad("dashicon", Color(1.0f, 1.0f, 1.0f), 1.f);
	GetMesh("dashicon")->textureID = LoadTGA("Image//UI//dash.tga");
	GenerateQuad("dreambaricon", Color(1.0f, 1.0f, 1.0f), 1.f);
	GetMesh("dreambaricon")->textureID = LoadTGA("Image//UI//dreambar.tga");
	GenerateQuad("treasureicon", Color(1.0f, 1.0f, 1.0f), 1.f);
	GetMesh("treasureicon")->textureID = LoadTGA("Image//UI//treasure.tga");

	GenerateSpriteAnimation("player_walkleft", 1, 7);
	GetMesh("player_walkleft")->textureID = LoadTGA("Image//Player//player_walkleft.tga");
	GenerateSpriteAnimation("player_walkright", 1, 7);
	GetMesh("player_walkright")->textureID = LoadTGA("Image//Player//player_walkright.tga");
    GenerateQuad("laserblaster", Color(1.0f, 1.0f, 1.0f), 1.f);
    GetMesh("laserblaster")->textureID = LoadTGA("Image//Player//laserblaster.tga");
}