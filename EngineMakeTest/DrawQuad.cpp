#include "DrawQuad.h"

struct vec3
{
	float x, y, z;
};

struct vertex
{
	vec3 position;
	vec3 position1;
	vec3 color;
	vec3 color1;
};


__declspec(align(16))
struct constant
{
	float m_angle;
};

int numQuads = 0;

void DrawQuad::LoadQuads(VertexBuffer* m_vb, void* shader_byte_code, size_t size_shader)
{

	vertex quadList[] =
	{
		//x - y - z
		//{-0.75f,-0.5f,0.0f,    -0.32f,-0.11f,0.0f,   0,0,0,  0,1,0 }, // pos1
		//{-0.5f,0.5f,0.0f,     -0.11f,0.78f,0.0f,    1,1,0,  0,1,1 }, // pos2
		//{ 0.5f,-0.5f,0.0f,     0.75f,-0.73f,0.0f,   0,0,1,  1,0,0 },// pos2
		//{ 0.5f,0.5f,0.0f,      0.88f,0.77f,0.0f,    1,1,1,  0,0,1 }

		//{-0.75f,-0.75f,0.0f,    -0.5f,-0.5f,0.0f,   0,0,0,  0,1,0 }, // POS1
		//{-0.75f, -0.25f,0.0f,     -0.5f,0.5f,0.0f,    1,1,0,  0,1,1 }, // POS2
		//{ -0.25f,-0.75f,0.0f,     0.5f,-0.5f,0.0f,   0,0,1,  1,0,0 },// POS2

		//{0.75f,0.75f,0.0f,    0.5f,0.5f,0.0f,   0,0,0,  0,1,0 }, // POS1
		//{0.75f, 0.25f,0.0f,     0.5f,0.5f,0.0f,    1,1,0,  0,1,1 }, // POS2
		//{ 0.25f,0.75f,0.0f,     0.5f,0.5f,0.0f,   0,0,1,  1,0,0 },// POS2

		{-0.75f,-0.75f,0.0f,    -0.75f,-0.75f,0.0f,   0,0,0,  0,1,0 }, // pos1
		{-0.75f,-0.25f,0.0f,     -0.75f,-0.25f,0.0f,    1,1,0,  0,1,1 }, // pos2
		{ -0.25f,-0.75f,0.0f,    -0.25f,-0.75f,0.0f,   0,0,1,  1,0,0 },// pos2
		{ -0.25f, -0.25f,0.0f,      -0.25f, -0.25f,0.0f,    1,1,1,  0,0,1 },

		{-0.5f,-0.5f,0.0f,    -0.32f,-0.11f,0.0f,   0,0,0,  0,1,0 }, // pos1
		{-0.5f,0.5f,0.0f,     -0.11f,0.78f,0.0f,    1,1,0,  0,1,1 }, // pos2
		{ 0.5f,-0.5f,0.0f,     0.75f,-0.73f,0.0f,   0,0,1,  1,0,0 },// pos2
		{ 0.5f,0.5f,0.0f,      0.88f,0.77f,0.0f,    1,1,1,  0,0,1 },

		{0.75f,0.75f,0.0f,    -0.75f,-0.75f,0.0f,   0,0,0,  0,1,0 }, // pos1
		{0.75f,0.25f,0.0f,     -0.75f,-0.25f,0.0f,    1,1,0,  0,1,1 }, // pos2
		{ 0.25f,0.75f,0.0f,    -0.25f,-0.75f,0.0f,   0,0,1,  1,0,0 },// pos2
		{ 0.25f, 0.25f,0.0f,      -0.25f, -0.25f,0.0f,    1,1,1,  0,0,1 },
	};


	
	m_vb->load(quadList, sizeof(vertex), ARRAYSIZE(quadList), shader_byte_code, size_shader);
	numQuads = (ARRAYSIZE(quadList)) / 4;
}

void DrawQuad::draw(DeviceContext* m_dc, int quadIndex)
{
	m_dc->drawTriangleStrip(4, quadIndex * 4);
}

void DrawQuad::drawAllQuads(DeviceContext* m_dc)
{
	
	for(int i = 0; i < numQuads; i++)
	{
		draw(m_dc, i);
	}
}
