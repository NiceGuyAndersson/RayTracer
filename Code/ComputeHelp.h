//--------------------------------------------------------------------------------------
// Real-Time JPEG Compression using DirectCompute - Demo
//
// Copyright (c) Stefan Petersson 2012. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once

#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dcompiler.h>
//#include <d3dx11.h>

#include <tchar.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=nullptr; } }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)      { if (p) { delete (p); (p)=nullptr; } }
#endif

enum COMPUTE_BUFFER_TYPE
{
	STRUCTURED_BUFFER,
	RAW_BUFFER
};

class ComputeBuffer
{
public:
	ID3D11Buffer*				GetResource()
	{ return _Resource; }
	ID3D11ShaderResourceView*	GetResourceView()
	{ return _ResourceView; }
	ID3D11UnorderedAccessView*	GetUnorderedAccessView()
	{ return _UnorderedAccessView; }
	ID3D11Buffer*				GetStaging()
	{ return _Staging; }
	void CopyToStaging()
	{ _D3DContext->CopyResource(_Staging, _Resource); }

	template<class T>
	T* Map()
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource; 
		T* p = nullptr;
		if(SUCCEEDED(_D3DContext->Map( _Staging, 0, D3D11_MAP_READ, 0, &MappedResource )))
			p = (T*)MappedResource.pData;

		return p;
	}

	void Unmap()
	{ 	_D3DContext->Unmap( _Staging, 0 ); }

	explicit ComputeBuffer()
	{
		_Resource = nullptr;
		_ResourceView = nullptr;
		_UnorderedAccessView = nullptr;
		_Staging = nullptr;	
	}

	~ComputeBuffer()
	{ Release(); }
	void Release()
	{
		SAFE_RELEASE(_Resource);
		SAFE_RELEASE(_ResourceView);
		SAFE_RELEASE(_UnorderedAccessView);
		SAFE_RELEASE(_Staging);
	}
private:
	ComputeBuffer(const ComputeBuffer& cb) {}

	ID3D11Buffer*				_Resource;
	ID3D11ShaderResourceView*	_ResourceView;
	ID3D11UnorderedAccessView*	_UnorderedAccessView;
	ID3D11Buffer*				_Staging;

	ID3D11DeviceContext*        _D3DContext;

	friend class ComputeWrap;
};

class ComputeTexture
{
public:
	ID3D11Texture2D*			GetResource()
	{ return _Resource; }
	ID3D11ShaderResourceView*	GetResourceView()
	{ return _ResourceView; }
	ID3D11UnorderedAccessView*	GetUnorderedAccessView()
	{ return _UnorderedAccessView; }
	ID3D11Texture2D*			GetStaging()
	{ return _Staging; }
	void CopyToStaging()
	{ _D3DContext->CopyResource(_Staging, _Resource); }

	template<class T>
	T* Map()
	{
		D3D11_MAPPED_SUBRESOURCE MappedResource; 
		T* p = nullptr;
		if(SUCCEEDED(_D3DContext->Map( _Staging, 0, D3D11_MAP_READ, 0, &MappedResource )))
			p = (T*)MappedResource.pData;

		return p;
	}

	void Unmap()
	{ 	_D3DContext->Unmap( _Staging, 0 ); }

	explicit ComputeTexture()
	{
		_Resource = nullptr;
		_ResourceView = nullptr;
		_UnorderedAccessView = nullptr;
		_Staging = nullptr;	
	}

	~ComputeTexture()
	{ Release(); }
	void Release()
	{
		SAFE_RELEASE(_Resource);
		SAFE_RELEASE(_ResourceView);
		SAFE_RELEASE(_UnorderedAccessView);
		SAFE_RELEASE(_Staging);
	}
private:
	ComputeTexture(const ComputeBuffer& cb) {}

	ID3D11Texture2D*			_Resource;
	ID3D11ShaderResourceView*	_ResourceView;
	ID3D11UnorderedAccessView*	_UnorderedAccessView;
	ID3D11Texture2D*			_Staging;

	ID3D11DeviceContext*        _D3DContext;

	friend class ComputeWrap;
};

class ComputeShader
{
	friend class ComputeWrap;

	ID3D11Device*               mD3DDevice;
	ID3D11DeviceContext*        mD3DDeviceContext;

	ID3D11ComputeShader*		mShader;
private:
	explicit ComputeShader();

	bool Init(TCHAR* shaderFile, TCHAR* blobFileAppendix, char* pFunctionName, D3D10_SHADER_MACRO* pDefines,
		ID3D11Device* d3dDevice, ID3D11DeviceContext*d3dContext);

public:
	~ComputeShader();

	void Set();
	void Unset();
};

class ComputeWrap
{
	ID3D11Device*               mD3DDevice;
	ID3D11DeviceContext*        mD3DDeviceContext;

public:
	ComputeWrap(ID3D11Device* d3dDevice, ID3D11DeviceContext* d3dContext)
	{
		mD3DDevice = d3dDevice;
		mD3DDeviceContext = d3dContext;
	}

	ComputeShader* CreateComputeShader(TCHAR* shaderFile, TCHAR* blobFileAppendix, char* pFunctionName, D3D10_SHADER_MACRO* pDefines);

	ID3D11Buffer* CreateConstantBuffer(UINT uSize, VOID* pInitData, char* debugName = nullptr);

	//ComputeBuffer* CreateBuffer(COMPUTE_BUFFER_TYPE uType, UINT uElementSize,
	//	UINT uCount, bool bSRV, bool bUAV, VOID* pInitData, bool bCreateStaging = false, char* debugName = nullptr);

	ComputeTexture* CreateTexture(DXGI_FORMAT dxFormat,	UINT uWidth,
		UINT uHeight, UINT uRowPitch, VOID* pInitData, bool bCreateStaging = false, char* debugName = nullptr);


private:
	//ID3D11Buffer* CreateStructuredBuffer(UINT uElementSize, UINT uCount, bool bSRV, bool bUAV, VOID* pInitData);
	//ID3D11Buffer* CreateRawBuffer(UINT uSize, VOID* pInitData);
	//ID3D11ShaderResourceView* CreateBufferSRV(ID3D11Buffer* pBuffer);
	//ID3D11UnorderedAccessView* CreateBufferUAV(ID3D11Buffer* pBuffer);
	//ID3D11Buffer* CreateStagingBuffer(UINT uSize);

	//texture functions
	ID3D11Texture2D* CreateTextureResource(DXGI_FORMAT dxFormat,
		UINT uWidth, UINT uHeight, UINT uRowPitch, VOID* pInitData);
	//ID3D11Buffer* CreateRawBuffer(UINT uSize, VOID* pInitData);
	ID3D11ShaderResourceView* CreateTextureSRV(ID3D11Texture2D* pTexture);
	ID3D11UnorderedAccessView* CreateTextureUAV(ID3D11Texture2D* pTexture);
	ID3D11Texture2D* CreateStagingTexture(ID3D11Texture2D* pTexture);
	
	void SetDebugName(ID3D11DeviceChild* object, char* debugName);
};