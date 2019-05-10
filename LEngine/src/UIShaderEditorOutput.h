#pragma once
#ifndef _UISHADEREDITOROUTPUT_H_
#define _UISHADEREDITOROUTPUT_H_

#include "UIBase.h"

///<summary>Class used for rendering Material Editor input point</summary>
class UIShaderEditorOutput : public UIBase
{
public:
	UIShaderEditorOutput();

	bool MouseOnArea(MouseClass* mouse) override;

	///<summary> Initialize shape: Rectangle/Triangle </summary>
	bool Initialize(D3DClass* d3d, ModelClass::ShapeSize shape, float left, float right, float top, float bottom);
	///<summary> Initialize shape: Square </summary>
	bool Initialize(D3DClass* d3d, float centerX, float centerY, float size);
	
	void Move(float x, float y);
	void GetTranslation(float & x, float & y);
	void GetPosition(float & x, float & y);

	virtual bool Render(ID3D11DeviceContext *deviceContext) final;

	void StartDragging();
	void StopDragging();
	bool IsDragging();

public:
	float m_value{ 0.0f };
	std::string m_variableName{};

private:
	bool m_dragged{ false };

	float m_translationX{ 0 };
	float m_translationY{ 0 };

	D3DClass* m_D3D{ nullptr };

	float min_X;
	float max_X;
	float min_Y;
	float max_Y;
};
#endif // !_UIBASE_H_