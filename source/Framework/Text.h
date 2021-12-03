#pragma once

#include "GfxDevice.h"
#include "Model.h"
#include "Font.h"
#include "Camera.h"

#include <string>

class Text {
public:
	Text(GfxDevice* gfxDevice, Font* pFont, Shader* pShader, XMFLOAT2 pos, float new_scale);
	~Text();

	const char* GetText() { return text; }
	void SetText(const char* new_text);

	void Render(Camera* cam);

private:
	GfxDevice* m_GfxDevice;
	Font* m_Font;
	Shader* m_Shader;
	Constants constants;

	XMFLOAT2 position;
	float scale;

	Vertex VertexData[4];
	ID3D11Buffer* vertexBuffer;
	ID3D11RasterizerState* rasterizerState;

	char *text;
};
