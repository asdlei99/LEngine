////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "ShaderSpecularClass.h"
#include "UIBase.h"
#include "TextEngine.h"
#include "UIBackground.h"
#include "UIButton.h"
#include "UITick.h"
#include "MouseClassContainer.h"
#include "UISlider.h"
#include "ShaderPBRClass.h"
#include "UITexturePreview.h"
#include "SkyboxShaderClass.h"
#include "RenderTextureClass.h"
#include "UITexture.h"
#include "BlurShaderClass.h"
#include <ScreenGrab.h>
#include <iostream>
#include <fstream>
#include <WICTextureLoader.h>
#include "ConvoluteShaderClass.h"
#include "ShadowMapClass.h"
#include "LightClass.h"
#include "SingleColorClass.h"
#include "GBufferShader.h"
#include <random>

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 100.0f;
const float SCREEN_NEAR = 0.1f;
const bool BLUR_BILINEAR = false;
const bool ENABLE_DEBUG = false;
const bool DRAW_SKYBOX = false;
const bool ENABLE_GUI = false;

const int CONVOLUTION_DIFFUSE_SIZE = 256;
const int ENVIRONMENT_SPECULAR_SIZE = 128;
const int SSAO_KERNEL_SIZE = 64;
const int SSAO_NOISE_SIZE = 16;

const int MAX_TEXTURE_INPUT = 4;

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	void MoveCameraForward(float val);
	void MoveCameraBackward(float val);
	void MoveCameraLeft(float val);
	void MoveCameraRight(float val);
	void MoveCameraUp(float val);
	void MoveCameraDown(float val);
	void RotateCamera(XMVECTOR rotation);

	void UpdateUI();
	void SetMouseRef(MouseClass* mouse);
	MouseClass* GetMouse();
		
	D3DClass* GetD3D();

	TextEngine::FontData* AddText(float&& posX, float&& posY, std::string&& text, float&& scale = 1.0f, TextEngine::Align &&align = TextEngine::Align::LEFT, XMVECTOR&& color = DirectX::Colors::White);

private:
	bool Render();
	bool RenderScene();
	bool RenderGUI();

	bool RenderDebugSettings();
	bool RenderSkybox();
	//BILINEAR SCREEN BLUR
	bool RenderSceneToTexture();
	bool DownsampleTexture();
	bool UpscaleTexture();
	bool BlurFilterScreenSpace(bool vertical); //Vertical = true; Horizontal = false
	//IBL DIFFUSE CONVOLUTION
	bool DownsampleSkybox();
	bool DownsampleSkyboxFace(const wchar_t* inputFilename, const wchar_t* outputFilename, bool isDDS);
	bool BlurFilter(bool vertical, UITexture* srcTex, RenderTextureClass* dstTex, int width); //Vertical = true; Horizontal = false
	bool BlurFilter(bool vertical, RenderTextureClass* srcTex, RenderTextureClass* dstTex, int width); //Vertical = true; Horizontal = false
	bool BlurFilter(bool vertical, ID3D11ShaderResourceView* srcTex, RenderTextureClass* dstTex, int width);
	bool ConvoluteShader(ID3D11ShaderResourceView* srcTex, RenderTextureClass* dstTex);
	//SHADOW MAPPING
	bool CreateShadowMap(RenderTextureClass*& targetTex);
	bool RenderDepthScene();
	//IBL SPECULAR
	bool PrepareEnvironmentPrefilteredMap(ID3D11ShaderResourceView* srcTex, RenderTextureClass* dstTex);
	bool PrepareLutBrdf(RenderTextureClass* dstTex);
	bool CreateSingleEnvironmentMap();
	//SSAO
	bool RenderGBufferPosition(RenderTextureClass *targetTex);
	bool RenderGBufferNormal(RenderTextureClass *targetTex);
	bool RenderGBufferAlbedo(RenderTextureClass *targetTex);
	bool RenderSSAONoiseTexture(RenderTextureClass *targetTex);
	bool RenderSSAOTexture(RenderTextureClass *targetTex);
	//ImGUI
	inline void RenderTextureViewImGui(ID3D11Resource*& resource, ID3D11ShaderResourceView*& resourceView, const char* label);

	///// HELPER FUNCTIONS /////
	///<summary>Return a when value == 0, return b when value is >= 1</summary> ///
	static float lerp(float a, float b, float val);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ModelClass* m_cubeModel;
	ModelClass* m_skyboxModel;
	ShaderSpecularClass* m_specularShader;
	ShaderPBRClass* m_pbrShader;
	SkyboxShaderClass* m_skyboxShader;
	TextEngine* m_textEngine;
	UITexturePreview* m_texturePreviewRoughness;
	UITexturePreview* m_texturePreviewMetalness;
	UITexturePreview* m_texturePreviewNormal;
	UITexturePreview* m_texturePreviewAlbedo;
	ID3D11ShaderResourceView* m_emptyTexView[MAX_TEXTURE_INPUT];

	UIBackground* m_debugBackground;
	MouseClassContainer* m_mouse;
	UISlider* m_roughnessSlider;
	UISlider* m_metalnessSlider;

	LightClass* m_directionalLight;

	//BILINEAR SCREEN BLUR
	RenderTextureClass* m_renderTexture;
	RenderTextureClass* m_renderTextureDownsampled;
	RenderTextureClass* m_renderTextureUpscaled;
	RenderTextureClass* m_renderTextureHorizontalBlur;
	RenderTextureClass* m_renderTextureVerticalBlur;
	UITexture* m_renderTexturePreview;
	BlurShaderClass* m_blurShaderVertical;
	BlurShaderClass* m_blurShaderHorizontal;

	//CONVOLUTION IBL DIFFUSE
	RenderTextureClass* m_skyboxDownsampled;
	RenderTextureClass* m_skyboxBlurHorizontal;
	RenderTextureClass* m_skyboxBlurVertical;
	SingleColorClass* m_colorShader;
	SingleColorClass* m_singleColorShader;
	SkyboxShaderClass* m_convoluteShader;
	ModelClass* m_convoluteQuadModel;

	//IBL SPECULAR SETTINGS
	SkyboxShaderClass* m_specularIBLShader;
	RenderTextureClass* m_environmentTextureMap;
	RenderTextureClass* m_brdfLUT;
	BaseShaderClass* m_brdfLutShader;

	//PREVIEW SKYBOX
	RenderTextureClass* m_skyboxTextureLeft;
	RenderTextureClass* m_skyboxTextureRight;
	RenderTextureClass* m_skyboxTextureUp;
	RenderTextureClass* m_skyboxTextureDown;
	RenderTextureClass* m_skyboxTextureForward;
	RenderTextureClass* m_skyboxTextureBack;

	UITexture* m_skyboxPreviewLeft;
	UITexture* m_skyboxPreviewRight;
	UITexture* m_skyboxPreviewUp;
	UITexture* m_skyboxPreviewDown;
	UITexture* m_skyboxPreviewForward;
	UITexture* m_skyboxPreviewBack;

	//SHADOW MAP
	ShadowMapClass* m_shadowMapShader;
	RenderTextureClass* m_shadowMapTexture;
	ModelClass* m_shadowQuadModel;
	ModelClass* m_groundQuadModel;

	//SSAO
	GBufferShader* m_GBufferShader;
	RenderTextureClass* m_positionBuffer;
	RenderTextureClass* m_normalBuffer;
	RenderTextureClass* m_albedoBuffer;
	RenderTextureClass* m_ssaoNoiseTexture;
	RenderTextureClass* m_ssaoTexture;
	XMFLOAT3 m_ssaoKernel[SSAO_KERNEL_SIZE];
	XMFLOAT2 m_ssaoNoise[16];
	
	//ImGUI
	int m_internalTextureViewIndex = -1;

	float m_rotationY = 0.0f;
	int m_screenWidth = 0;
	int m_screenHeight = 0;
	int m_exitCount = 0;
	int m_convolutionFace = 0;
};

#endif