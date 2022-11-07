#pragma once
#include "DeviceContext.h"
#include "VertexBuffer.h"

class DrawQuad
{
public:
	static void LoadQuads(VertexBuffer* m_vb, void* shader_byte_code, size_t size_byte_shader);
	static void draw(DeviceContext* m_dc, int quadIndex);
	static void drawAllQuads(DeviceContext* m_dc);
};

