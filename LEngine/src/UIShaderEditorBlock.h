#pragma once
#ifndef _UISHADEREDITOR_H_
#define _UISHADEREDITOR_H_

#include "UIBase.h"
#include "UIShaderEditorInput.h"
#include "UIShaderEditorOutput.h"
#include "TextEngine.h"
#include "UITextureMoveable.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iomanip>

///<summary>Class used for rendering Material Editor blocks</summary>
class UIShaderEditorBlock : public UIBase
{
public:
	enum class EDragPinBehaviour
	{
		Dragging,
		Start,
		Stop,
		Break,
		Nothing
	};

private:
	struct Size
	{
		float x{ 0.0f };
		float y{ 0.0f };

		Size(float x, float y) : x(x), y(y) {}
	};

	struct Margin
	{
		float x{ 0.0f };
		float y{ 0.0f };

		Margin(float x, float y) : x(x), y(y) {}
	};

public:
	UIShaderEditorBlock();
	UIShaderEditorBlock(XMFLOAT2 startPosition);
	UIShaderEditorBlock(XMFLOAT2 startPosition, std::string functionName, std::string returnType, vector<std::string> argumentTypes, int id);

	bool MouseOnArea(MouseClass* mouse) override;

	bool Initialize(D3DClass* d3d, int inCount, int outCount);
	
	void Move(float x, float y);
	void StartDragging();
	void StopDragging();
	bool IsDragging() const;
	bool IsPinDragging() const;

	UIShaderEditorBlock::EDragPinBehaviour DragPins(MouseClass* mouse, UIShaderEditorOutput*& out);

	UIShaderEditorInput* CheckIfMouseOnInputPin(MouseClass* mouse) const;

	virtual bool Render(ID3D11DeviceContext *deviceContext) final;

	std::string GenerateShaderCode(bool skipTabulator = false) const;
	std::string ConvertType(std::string outName, std::string typeIn, std::string typeOut) const;
	int GetInputCount() const;
	void SetOutputPinName(std::string name);
	UIShaderEditorOutput* GetFirstOutputNode();
	std::string GetFunctionName() const;
	std::string GetReturnType() const;

	void SetScale(float scale);
	bool TryToMarkBlock(RectangleVertices markingBounds);
	Size GetPosition() const;
	void ChangeBlockName();
	int GetBlockID() const;
	void UpdateVariable();

private:
	void CalculateBlockSize(int inCount, int outCount);
	RectangleVertices CalculateOutlineSize(RectangleVertices blockSize) const;
	bool InitializeInputNodes(int inCount);
	bool InitializeOutputNodes(int outCount);
	std::string ReturnEmptyForGivenType(std::string type) const;

public:
	std::string m_fileName{};
	std::string m_variableName{"test"};
	vector<UIShaderEditorInput*> m_inputNodes = {};
	vector<UIShaderEditorOutput*> m_outputNodes = {};
	bool m_focused = { false };

private:
	UIBase* m_colorPreview{ nullptr };
	UITextureMoveable* m_texturePreview{ nullptr };

	int m_blockID{ -1 };
	D3DClass* m_D3D{ nullptr };
	bool m_blockInitialized{ false };

	bool m_moveAfterInitializing{ false };
	XMFLOAT2 m_movemementAfterInitialization{ 0,0 };

	float m_translationX{ 0 };
	float m_translationY{ 0 };
	bool m_dragged = { false };
	bool m_pinDragged = { false };
	RectangleVertices m_blockVertices;
	TextEngine* m_textEngine{};

	std::string m_returnType{ "float" };
	std::vector<std::string> m_argumentTypes{ "float", "float" };
	std::string m_functionName{ "add" };
	std::string m_blockName{ "add" };

	UIBase* m_outlineObject{};

	float m_scale{ 1.0f };

private:
	const vector<Size> blockSizeVector = { Size{ 0.4f, 0.2f }, Size{ 0.4f, 0.28f }, Size{ 0.4f, 0.35f }, Size{ 0.4f, 0.43f }, Size{ 0.4f, 0.51f } };
	const Margin inOutMargin = Margin{ 0.02f, 0.11f };
	const Size inOutSize = Size{ 0.03f, 0.04f };
	const float paddingBetweenBlocks = 0.08f;
	const XMFLOAT4 blockColor{0.2f, 0.2f, 0.2f, 0.8f};
	//Outline
	const XMFLOAT4 outlineColor{0.7f, 0.7f, 0.7f, 0.95f};
	const float outlineMargin = 0.001f;
};
#endif // !_UIBASE_H_