#pragma once
#ifndef _SHADERPBRCLASS_H_
#define _SHADERPBRCLASS_H_

#include "BaseShaderClass.h"

class ShaderPBRClass : public BaseShaderClass
{
private:
	static const int NUM_LIGHTS = 2;
	struct LightingBufferType
	{
		XMFLOAT3 direction[NUM_LIGHTS];
		float strength[NUM_LIGHTS];
	};

	struct CameraBufferType
	{
		XMFLOAT3 cameraDirection;
		float padding;
	};

	struct PBRBufferType 
	{
		float roughness;
		float metalness;
		XMFLOAT2 padding;
	};

	struct ShaderTextureBufferType
	{
		int hasNormalMap;
		int hasRoughnessMap;
		int hasMetalnessMap;
		int paddingShaderTextureBuffer;
	};

public:
	bool LoadIrradianceMap(ID3D11ShaderResourceView *& shaderResourceView);
	bool LoadIrradianceMap(ID3D11Device *device, const wchar_t* filename);

	bool LoadEnvironmentMap(ID3D11Device *device, const wchar_t* filename);
	bool LoadEnvironmentMap(ID3D11ShaderResourceView *& shaderResourceView);
	bool AddEnvironmentMapLevel(ID3D11ShaderResourceView *& shaderResourceView);
	bool AddEnvironmentMapLevel(ID3D11Device *device, const wchar_t* filename);
	int GetEnvironmentMipLevels();

	bool LoadBrdfLut(ID3D11Device *device, const wchar_t* filename);

	void AddLights(XMFLOAT4 directionStrength);
	void AddLights(XMFLOAT3 direction, float strength);

	void SetRoughness(float roughness);
	void SetMetalness(float metalness);

public:
	//Texture resources
	ID3D11Resource* m_diffuseTexture;
	ID3D11ShaderResourceView* m_diffuseTextureView;
	ID3D11Resource* m_normalTexture;
	ID3D11ShaderResourceView* m_normalTextureView;
	ID3D11Resource* m_roughnessTexture;
	ID3D11ShaderResourceView* m_roughnessTextureView;
	ID3D11Resource* m_metalnessTexture;
	ID3D11ShaderResourceView* m_metalnessTextureView;
	ID3D11Resource* m_brdfLut;
	ID3D11ShaderResourceView* m_brdfLutView;

	XMFLOAT3 m_cameraPosition;
	float m_roughness = 0;
	float m_metalness = 0;

protected:
	virtual bool CreateBufferAdditionals(ID3D11Device *&device) override;
	virtual bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX&, XMMATRIX&, XMMATRIX&) override;

private:
	std::vector<XMFLOAT4> m_lightDirection;

	ID3D11Buffer* m_lightingBuffer;
	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_PBRBuffer;
	ID3D11Buffer* m_ShaderTextureBuffer;

	ID3D11Resource* m_irradianceMap;
	ID3D11ShaderResourceView* m_irradianceMapView;
	ID3D11Resource* m_environmentMapTexture;
	ID3D11ShaderResourceView* m_environmentMapTextureView;

	std::vector<ID3D11ShaderResourceView*> m_environmentMapViews;
};

#endif // !_SHADERPBRCLASS_H_