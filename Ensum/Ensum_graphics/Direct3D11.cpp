#include "Ensum_graphics\Direct3D11.h"
#include "Exception.h"
#include "Ensum_utils\Options.h"
#include "Ensum_core\WindowsWindow.h"
#include "Safe_Delete.h"

#ifdef _DEBUG
#pragma comment(lib, "Ensum_utilsD.lib")
#pragma comment(lib, "Ensum_coreD.lib")
#else
#pragma comment(lib, "Ensum_utils.lib")
#pragma comment(lib, "Ensum_core.lib")
#endif

namespace Ensum
{
	namespace Graphics
	{
// Initializes Direct3D11 by creating the device, context, and swap chain
// with back buffer.
const void Direct3D11::Start()
{
	this->_hWnd = Core::WinWindow::GetWindow();

	IDXGIFactory *dxgiFactory = nullptr;
	_com_error hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
	if (FAILED(hr.Error()))
	{
		Exception("Couldn't create DXGIFactory. Error: %S", hr.ErrorMessage());
	}

	IDXGIAdapter *dxgiAdapter = nullptr;
	hr = dxgiFactory->EnumAdapters(0, &dxgiAdapter);
	if (FAILED(hr.Error()))
	{
		SAFE_RELEASE(dxgiFactory);
		Exception("Failed to get adapter. Error: %S", hr.ErrorMessage());
	}
	hr = dxgiAdapter->EnumOutputs(0, &_DXGIOutput);
	if (FAILED(hr.Error()))
	{
		SAFE_RELEASE(dxgiFactory);
		SAFE_RELEASE(dxgiAdapter);
		Exception("Failed to get output. Error: %S", hr.ErrorMessage());
	}

	unsigned deviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
	unsigned numFeatureLevels = sizeof(featureLevels) / sizeof(featureLevels[0]);

	hr = D3D11CreateDevice(
		dxgiAdapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		deviceFlags,
		featureLevels,
		numFeatureLevels,
		D3D11_SDK_VERSION,
		&_d3dDevice,
		&_FeatureLevel,
		&_d3dDeviceContext);

	if (FAILED(hr.Error()))
	{
		SAFE_RELEASE(dxgiFactory);
		SAFE_RELEASE(dxgiAdapter);
		SAFE_RELEASE(_DXGIOutput);
		Exception("Failed to create device. Error: %S", hr.ErrorMessage());
	}

	DXGI_SWAP_CHAIN_DESC sd;
	memset(&sd, 0, sizeof(DXGI_SWAP_CHAIN_DESC));
	sd.BufferCount = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	unsigned backbufferWidth = (unsigned)Utils::Options::GetIntegerOption("Screen", "Width", 800);
	unsigned backbufferHeight = (unsigned)Utils::Options::GetIntegerOption("Screen", "Height", 800);

	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.RefreshRate.Numerator = (unsigned)Utils::Options::GetIntegerOption("Screen", "RefreshRateNumerator", 60);
	sd.BufferDesc.RefreshRate.Denominator = (unsigned)Utils::Options::GetIntegerOption("Screen", "RefreshRateDenominator", 1);
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.OutputWindow = _hWnd;
	sd.Windowed = !Utils::Options::GetBooleanOption("Screen", "Fullscreen", false);
	hr = dxgiFactory->CreateSwapChain(_d3dDevice, &sd, &_SwapChain);
	if (FAILED(hr.Error()))
	{
		SAFE_RELEASE(dxgiFactory);
		SAFE_RELEASE(dxgiAdapter);
		SAFE_RELEASE(_DXGIOutput);		
		SAFE_RELEASE(_d3dDevice);
		SAFE_RELEASE(_d3dDeviceContext);
		Exception("Failed to create swapchain. Error: %S", hr.ErrorMessage());
	}

	// Make sure DXGI does not interfere with window messages.
	// I'll just handle Alt+Enter myself, thank you.
	dxgiFactory->MakeWindowAssociation(_hWnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);

	SAFE_RELEASE(dxgiAdapter);
	SAFE_RELEASE(dxgiFactory);

	Resize();
}

const void Direct3D11::Shutdown( void )
{
	// TODO: Release all created resources

	SAFE_RELEASE( _BackBufferRTV );
	SAFE_RELEASE( _SwapChain );
	SAFE_RELEASE( _d3dDeviceContext );

	// Release device and notify if a com object has not been properly released.
	if ( _d3dDevice )
	{
		UINT references = _d3dDevice->Release();
		if ( references > 0 )
		{
			Utils::ConsoleLog::DumpToConsole( "A com object has not been released." );
		}
		_d3dDevice = nullptr;
	}

	SAFE_RELEASE( _DXGIOutput );
}

const void Direct3D11::Resize(  )
{
	unsigned backbufferWidth = (unsigned)Utils::Options::GetIntegerOption("Screen", "Width", 800);
	unsigned backbufferHeight = (unsigned)Utils::Options::GetIntegerOption("Screen", "Height", 800);
	if ( _SwapChain )
	{
		// Unbind any buffers
		_d3dDeviceContext->OMSetRenderTargets( 0, nullptr, nullptr );

		// Release old render target view as it references the buffer that
		// will be destroyed.
		if ( _BackBufferRTV ) _BackBufferRTV->Release();

		// Preserve number of existing buffers and existing back buffer format.
		_com_error hr = _SwapChain->ResizeBuffers( 0, backbufferWidth, backbufferHeight, DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH );

		if ( FAILED( hr.Error() ) )
		{
			Exception( "Failed to resize buffers! Error: %S", hr.ErrorMessage() );
		}

		// Recreate render target view to back buffer.

		ID3D11Texture2D *backBufferTexture = nullptr;
		hr = _SwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBufferTexture) );

		if ( FAILED(hr.Error()) )
		{

			Exception( "Failed to get the swap chain back buffer! Error: %S", hr.ErrorMessage());
		}

		hr = _d3dDevice->CreateRenderTargetView( backBufferTexture, nullptr, &_BackBufferRTV );

		SAFE_RELEASE( backBufferTexture );

		if ( FAILED(hr.Error()) )
		{
			Exception( "Failed to create the render target view! Error: %S", hr.ErrorMessage());
		}

		_d3dDeviceContext->OMSetRenderTargets( 1, &_BackBufferRTV, nullptr );

		D3D11_VIEWPORT viewport;
		viewport.Width = static_cast<float>(backbufferWidth);
		viewport.Height = static_cast <float>(backbufferHeight);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		_d3dDeviceContext->RSSetViewports( 1, &viewport );
	}

}

RenderTarget Direct3D11::CreateRenderTarget(
	DXGI_FORMAT format,
	unsigned width,
	unsigned height,
	unsigned depth,
	unsigned flags,
	unsigned arraySize,
	unsigned mipLevels,
	unsigned sampleCount )const
{
	RenderTarget renderTarget;

	// Create the underlying resource.

	// 1D
	if ( height == 0 && depth == 0 )
	{
		D3D11_TEXTURE1D_DESC texDesc;
		texDesc.Width = width;
		texDesc.MipLevels = mipLevels;
		texDesc.ArraySize = arraySize;
		texDesc.Format = format;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		if ( flags & TEXTURE_COMPUTE_WRITE )
			texDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;

		if ( flags & TEXTURE_GEN_MIPS )
			texDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;

		_com_error hr = _d3dDevice->CreateTexture1D( &texDesc, nullptr, (ID3D11Texture1D**)&renderTarget.Texture );
		if ( FAILED(hr.Error()) )
		{
			Exception( "Failed to create 1D render target texture. Error: %S", hr.ErrorMessage());
		}

		renderTarget.SliceCount = texDesc.ArraySize;
	}
	// 2D
	else if ( depth == 0 )
	{
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.MipLevels = mipLevels;
		texDesc.ArraySize = arraySize * ((flags & TEXTURE_CUBE) ? 6 : 1);
		texDesc.Format = format;
		texDesc.SampleDesc.Count = sampleCount;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		if ( flags & TEXTURE_COMPUTE_WRITE )
			texDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;

		if ( flags & TEXTURE_GEN_MIPS )
			texDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;

		if ( flags & TEXTURE_CUBE )
			texDesc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;

		_com_error hr = _d3dDevice->CreateTexture2D( &texDesc, nullptr, (ID3D11Texture2D**)&renderTarget.Texture );
		if ( FAILED(hr.Error()) )
		{
			Exception( "Failed to create 2D render target texture. Error: %S", hr.ErrorMessage());
		}

		renderTarget.SliceCount = texDesc.ArraySize;
	}
	// 3D
	else
	{
		D3D11_TEXTURE3D_DESC texDesc;
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.Depth = depth;
		texDesc.MipLevels = mipLevels;
		texDesc.Format = format;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		if ( flags & TEXTURE_COMPUTE_WRITE )
			texDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;

		if ( flags & TEXTURE_GEN_MIPS )
			texDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;

		_com_error hr = _d3dDevice->CreateTexture3D( &texDesc, nullptr, (ID3D11Texture3D**)&renderTarget.Texture );
		if ( FAILED(hr.Error()) )
		{
			Exception( "Failed to create 3D render target texture. Error: %S", hr.ErrorMessage());
		}

		renderTarget.SliceCount = depth;
	}

	// Create views to texture.

	try {
		renderTarget.RTV = _CreateRTV(renderTarget.Texture);
		renderTarget.SRV = _CreateSRV(renderTarget.Texture);
		renderTarget.UAV = _CreateUAV(renderTarget.Texture);
	}
	catch (const Exce& e)
	{
		SAFE_RELEASE(renderTarget.Texture);
		SAFE_RELEASE(renderTarget.RTV);
		SAFE_RELEASE(renderTarget.SRV);
		SAFE_RELEASE(renderTarget.UAV);
		throw e;
	}
	renderTarget.RTVSlices = new ID3D11RenderTargetView *[renderTarget.SliceCount];
	renderTarget.SRVSlices = new ID3D11ShaderResourceView *[renderTarget.SliceCount];
	renderTarget.UAVSlices = new ID3D11UnorderedAccessView *[renderTarget.SliceCount];

	try {
		for (unsigned i = 0; i < renderTarget.SliceCount; ++i)
		{
			renderTarget.RTVSlices[i] = _CreateRTV(renderTarget.Texture, format, i);
			renderTarget.SRVSlices[i] = _CreateSRV(renderTarget.Texture, format, i);
			renderTarget.UAVSlices[i] = _CreateUAV(renderTarget.Texture, format, i);
		}
	}
	catch (const Exce& e)
	{
		SAFE_RELEASE(renderTarget.Texture);
		SAFE_RELEASE(renderTarget.RTV);
		SAFE_RELEASE(renderTarget.SRV);
		SAFE_RELEASE(renderTarget.UAV);
		for (unsigned i = 0; i < renderTarget.SliceCount; ++i)
		{
			SAFE_RELEASE(renderTarget.RTVSlices[i]);
			SAFE_RELEASE(renderTarget.SRVSlices[i]);
			SAFE_RELEASE(renderTarget.UAVSlices[i]);
		}
		throw e;
	}

	// Cache stuff
	renderTarget.Format = format;
	renderTarget.Width = width;
	renderTarget.Height = height;
	renderTarget.Depth = depth;
	renderTarget.ArraySize = arraySize;
	renderTarget.MipLevels = mipLevels;
	renderTarget.SampleCount = sampleCount;

	return renderTarget;
}

const void Direct3D11::DeleteRenderTarget( RenderTarget &rt )const
{
	SAFE_RELEASE( rt.Texture );
	SAFE_RELEASE( rt.RTV );
	SAFE_RELEASE( rt.SRV );
	SAFE_RELEASE( rt.UAV );

	if ( rt.RTVSlices )
	{
		for ( unsigned i = 0; i < rt.SliceCount; ++i )
		{
			SAFE_RELEASE( rt.RTVSlices[i] );
		}

		SAFE_DELETE_ARRAY( rt.RTVSlices );
	}

	if ( rt.SRVSlices )
	{
		for ( unsigned i = 0; i < rt.SliceCount; ++i )
		{
			SAFE_RELEASE( rt.SRVSlices[i] );
		}

		SAFE_DELETE_ARRAY( rt.SRVSlices );
	}

	if ( rt.UAVSlices )
	{
		for ( unsigned i = 0; i < rt.SliceCount; ++i )
		{
			SAFE_RELEASE( rt.UAVSlices[i] );
		}

		SAFE_DELETE_ARRAY( rt.UAVSlices );
	}

	rt = RenderTarget();
}

DepthBuffer Direct3D11::CreateDepthBuffer(
	DXGI_FORMAT format,
	unsigned width,
	unsigned height,
	bool sampleInShader,
	unsigned flags,
	unsigned arraySize,
	unsigned mipLevels,
	unsigned sampleCount )const
{
	DepthBuffer depthBuffer;
	depthBuffer.FormatDSV = depthBuffer.FormatTex = format;

	if ( sampleInShader )
	{
		switch ( depthBuffer.FormatDSV )
		{
		case DXGI_FORMAT_D16_UNORM:
			depthBuffer.FormatTex = DXGI_FORMAT_R16_TYPELESS;
			depthBuffer.FormatSRV = DXGI_FORMAT_R16_UNORM;
			break;

		case DXGI_FORMAT_D24_UNORM_S8_UINT:
			depthBuffer.FormatTex = DXGI_FORMAT_R24G8_TYPELESS;
			depthBuffer.FormatSRV = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			break;

		case DXGI_FORMAT_D32_FLOAT:
			depthBuffer.FormatTex = DXGI_FORMAT_R32_TYPELESS;
			depthBuffer.FormatSRV = DXGI_FORMAT_R32_FLOAT;
			break;

		default:
			Exception( "Invalid depth format." );
		}
	}

	// Create the underlying resource.

	// 1D
	if ( height == 0 )
	{
		D3D11_TEXTURE1D_DESC texDesc;
		texDesc.Width = width;
		texDesc.MipLevels = mipLevels;
		texDesc.ArraySize = arraySize;
		texDesc.Format = depthBuffer.FormatTex;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		if ( sampleInShader )
			texDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;

		_com_error hr = _d3dDevice->CreateTexture1D( &texDesc, nullptr, (ID3D11Texture1D**)&depthBuffer.Texture );
		if ( FAILED( hr.Error() ) )
		{
			Exception( "Failed to create 1D depth buffer texture. Error: %S", hr.ErrorMessage());
		}

		depthBuffer.SliceCount = texDesc.ArraySize;
	}
	// 2D
	else
	{
		D3D11_TEXTURE2D_DESC texDesc;
		texDesc.Width = width;
		texDesc.Height = height;
		texDesc.MipLevels = mipLevels;
		texDesc.ArraySize = arraySize * ((flags & TEXTURE_CUBE) ? 6 : 1);
		texDesc.Format = depthBuffer.FormatTex;
		texDesc.SampleDesc.Count = sampleCount;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		if ( sampleInShader )
			texDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;

		if ( flags & TEXTURE_CUBE )
			texDesc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;

		_com_error hr = _d3dDevice->CreateTexture2D( &texDesc, nullptr, (ID3D11Texture2D**)&depthBuffer.Texture );
		if ( FAILED(hr.Error()) )
		{
			Exception( "Failed to create 2D depth buffer texture. Error: %S", hr.ErrorMessage());
		}

		depthBuffer.SliceCount = texDesc.ArraySize;
	}

	// Create views to texture.
	try {
		depthBuffer.DSV = _CreateDSV(depthBuffer.Texture, depthBuffer.FormatDSV);
		depthBuffer.DSVReadOnly = _CreateDSV(depthBuffer.Texture, depthBuffer.FormatDSV, true);
		depthBuffer.SRV = _CreateSRV(depthBuffer.Texture, depthBuffer.FormatSRV);
	}
	catch (const Exce& e)
	{
		SAFE_RELEASE(depthBuffer.Texture);
		SAFE_RELEASE(depthBuffer.DSV);
		SAFE_RELEASE(depthBuffer.DSVReadOnly);
		SAFE_RELEASE(depthBuffer.SRV);
		throw e;
	}
	depthBuffer.DSVSlices = new ID3D11DepthStencilView *[depthBuffer.SliceCount];
	depthBuffer.DSVReadOnlySlices = new ID3D11DepthStencilView *[depthBuffer.SliceCount];
	depthBuffer.SRVSlices = new ID3D11ShaderResourceView *[depthBuffer.SliceCount];
	try {
		for (unsigned i = 0; i < depthBuffer.SliceCount; ++i)
		{
			depthBuffer.DSVSlices[i] = _CreateDSV(depthBuffer.Texture, depthBuffer.FormatDSV, false, i);
			depthBuffer.DSVReadOnlySlices[i] = _CreateDSV(depthBuffer.Texture, depthBuffer.FormatDSV, true, i);
			depthBuffer.SRVSlices[i] = _CreateSRV(depthBuffer.Texture, depthBuffer.FormatSRV, i);
		}
	}
	catch (const Exce& e)
	{
		SAFE_RELEASE(depthBuffer.Texture);
		SAFE_RELEASE(depthBuffer.DSV);
		SAFE_RELEASE(depthBuffer.DSVReadOnly);
		SAFE_RELEASE(depthBuffer.SRV);

		for (unsigned i = 0; i < depthBuffer.SliceCount; ++i)
		{
			SAFE_RELEASE(depthBuffer.DSVSlices[i]);
			SAFE_RELEASE(depthBuffer.DSVReadOnlySlices[i]);
			SAFE_RELEASE(depthBuffer.SRVSlices[i]);

		}	
		throw e;
	}
	// Cache stuff
	depthBuffer.Width = width;
	depthBuffer.Height = height;
	depthBuffer.ArraySize = arraySize;
	depthBuffer.MipLevels = mipLevels;
	depthBuffer.SampleCount = sampleCount;

	return depthBuffer;
}

const void Direct3D11::DeleteDepthBuffer( DepthBuffer &db )const
{
	SAFE_RELEASE( db.Texture );
	SAFE_RELEASE( db.DSV );
	SAFE_RELEASE( db.DSVReadOnly );
	SAFE_RELEASE( db.SRV );

	if ( db.DSVSlices )
	{
		for ( unsigned i = 0; i < db.SliceCount; ++i )
		{
			SAFE_RELEASE( db.DSVSlices[i] );
		}

		SAFE_DELETE_ARRAY( db.DSVSlices );
	}

	if ( db.DSVReadOnlySlices )
	{
		for ( unsigned i = 0; i < db.SliceCount; ++i )
		{
			SAFE_RELEASE( db.DSVReadOnlySlices[i] );
		}

		SAFE_DELETE_ARRAY( db.DSVReadOnlySlices );
	}

	if ( db.SRVSlices )
	{
		for ( unsigned i = 0; i < db.SliceCount; ++i )
		{
			SAFE_RELEASE( db.SRVSlices[i] );
		}

		SAFE_DELETE_ARRAY( db.SRVSlices );
	}

	db = DepthBuffer();
}

DepthStencilState Direct3D11::CreateDepthStencilState(
	bool depthEnable, 
	D3D11_DEPTH_WRITE_MASK depthWriteMask, 
	D3D11_COMPARISON_FUNC 	depthComparison, 
	bool stencilEnable, 
	UINT8 stencilReadMask, 
	UINT8 stencilWriteMask) const
{
	DepthStencilState state;
	state.depthEnable = depthEnable;
	state.depthWriteMask = depthWriteMask;
	state.depthComparison = depthComparison;
	state.stencilEnable = stencilEnable;
	state.stencilReadMask = stencilReadMask;
	state.stencilWriteMask = stencilWriteMask;

	try {
		state.DSS = _CreateDSS(depthEnable,
			depthWriteMask,
			depthComparison,
			stencilEnable,
			stencilReadMask,
			stencilWriteMask);
	}
	catch (const Exce& e)
	{
		SAFE_RELEASE(state.DSS);
		throw e;
	}
	return state;
}

const void Direct3D11::DeleteDepthStencilState(DepthStencilState & dss) const
{
	SAFE_RELEASE(dss.DSS);
	return void();
}

RasterizerState Direct3D11::CreateRasterizerState(
	D3D11_FILL_MODE fillMode, 
	D3D11_CULL_MODE cullMode, 
	bool frontCounterClockwise, 
	bool depthBias, 
	float depthBiasClamp, 
	float slopeScaledDepthBias, 
	bool depthClipEnable, 
	bool scissorEnable, 
	bool multiSampleEnable, 
	bool antialiasedLineEnable) const
{
	RasterizerState state;
	state.fillMode = fillMode;
	state.cullMode = cullMode;
	state.frontCounterClockwise = frontCounterClockwise;
	state.depthBias = depthBias;
	state.depthBiasClamp = depthBiasClamp;
	state.slopeScaledDepthBias = slopeScaledDepthBias;
	state.depthClipEnable = depthClipEnable;
	state.scissorEnable = scissorEnable;
	state.multiSampleEnable = multiSampleEnable;
	state.antialiasedLineEnable = antialiasedLineEnable;

	try {
		state.RS = _CreateRS(
			fillMode,
			cullMode,
			frontCounterClockwise,
			depthBias,
			depthBiasClamp,
			slopeScaledDepthBias,
			depthClipEnable,
			scissorEnable,
			multiSampleEnable,
			antialiasedLineEnable);
	}
	catch (const Exce& e)
	{
		SAFE_RELEASE(state.RS);
		throw e;
	}
	return state;
}

const void Direct3D11::DeleteRasterizerState(RasterizerState & rs) const
{
	SAFE_RELEASE(rs.RS);
	return void();
}

BlendState Direct3D11::CreateBlendState(bool blendEnabled, UINT8 renderTargetWriteMask) const
{
	BlendState bs;
	bs.blendEnable = blendEnabled;
	bs.renderTargetWriteMask = renderTargetWriteMask;
	try {
		bs.BS = _CreateBS(blendEnabled, renderTargetWriteMask);
	}
	catch (const Exce& e)
	{
		SAFE_RELEASE(bs.BS);
		throw e;
	}
	return bs;
}

BlendState Direct3D11::CreateBlendState(D3D11_BLEND_DESC * blendDesc) const
{
	BlendState bs;

	try {
		bs.BS = _CreateBS(blendDesc);
	}
	catch (const Exce& e)
	{
		SAFE_RELEASE(bs.BS);
		throw e;
	}
	return bs;
}

const void Direct3D11::DeleteBlendState(BlendState & bs) const
{
	SAFE_RELEASE(bs.BS);
	return void();
}

ID3D11RenderTargetView* Direct3D11::_CreateRTV(
	ID3D11Resource *resource,
	DXGI_FORMAT format,
	int firstSlice,
	unsigned numSlices
	)const
{
	// Get dimension of resource so that we know what we're working with.
	D3D11_RESOURCE_DIMENSION type;
	resource->GetType( &type );

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;

	switch ( type )
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
	{
		// Get description of underlying texture.
		D3D11_TEXTURE1D_DESC texDesc;
		((ID3D11Texture1D*)resource)->GetDesc( &texDesc );

		rtvDesc.Format = (format == DXGI_FORMAT_UNKNOWN) ? texDesc.Format : format;

		// Texture array
		if ( texDesc.ArraySize > 1 )
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1DARRAY;
			rtvDesc.Texture1DArray.MipSlice = 0;

			// firstSlice == -1 represents a view to the entire array.
			unsigned theFirstSlice = (firstSlice == -1) ? 0 : firstSlice;
			unsigned theSliceCount = (firstSlice == -1) ? texDesc.ArraySize : numSlices;

			rtvDesc.Texture1DArray.FirstArraySlice = theFirstSlice;
			rtvDesc.Texture1DArray.ArraySize = theSliceCount;
		}
		// Regular texture
		else
		{
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1D;
			rtvDesc.Texture1D.MipSlice = 0;
		}

		break;
	}

	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
	{
		// Get description of underlying texture.
		D3D11_TEXTURE2D_DESC texDesc;
		((ID3D11Texture2D*)resource)->GetDesc( &texDesc );

		rtvDesc.Format = (format == DXGI_FORMAT_UNKNOWN) ? texDesc.Format : format;

		// Texture array
		if ( texDesc.ArraySize > 1 )
		{
			// firstSlice == -1 represents a view to the entire array.
			unsigned theFirstSlice = (firstSlice == -1) ? 0 : firstSlice;
			unsigned theSliceCount = (firstSlice == -1) ? texDesc.ArraySize : numSlices;

			// Multi sampled texture array
			if ( texDesc.SampleDesc.Count > 1 )
			{
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
				rtvDesc.Texture2DMSArray.FirstArraySlice = theFirstSlice;
				rtvDesc.Texture2DMSArray.ArraySize = theSliceCount;
			}
			// Regular texture array
			else
			{
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				rtvDesc.Texture2DArray.MipSlice = 0;
				rtvDesc.Texture2DArray.FirstArraySlice = theFirstSlice;
				rtvDesc.Texture2DArray.ArraySize = theSliceCount;
			}
		}
		// Regular texture
		else
		{
			// Multi sampled texture
			if ( texDesc.SampleDesc.Count > 1 )
			{
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
			}
			// Regular texture array
			else
			{
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				rtvDesc.Texture2D.MipSlice = 0;
			}
		}

		break;
	}

	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
	{
		// Get description of underlying texture.
		D3D11_TEXTURE3D_DESC texDesc;
		((ID3D11Texture3D*)resource)->GetDesc( &texDesc );

		rtvDesc.Format = (format == DXGI_FORMAT_UNKNOWN) ? texDesc.Format : format;
		rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
		rtvDesc.Texture3D.MipSlice = 0;

		// firstSlice == -1 represents a view to the entire volume.
		unsigned theFirstSlice = (firstSlice == -1) ? 0 : firstSlice;
		unsigned theSliceCount = (firstSlice == -1) ? texDesc.Depth : numSlices;

		rtvDesc.Texture3D.FirstWSlice = theFirstSlice;
		rtvDesc.Texture3D.WSize = theSliceCount;

		break;
	}

	default:
		Exception( "There's no implementation for resource dimension %i", type );
	}

	// RTV description complete; time to create it.
	ID3D11RenderTargetView *rtv = nullptr;
	_com_error hr = _d3dDevice->CreateRenderTargetView( resource, &rtvDesc, &rtv );
	if ( FAILED( hr.Error() ) )
	{
		Exception( "Failed to create render target view. Error: %S", hr.ErrorMessage() );
	}

	return rtv;
}

// Note that the function is somewhat misleading; I can't create a SRV to a part
// of a 3D texture for instance...
ID3D11ShaderResourceView* Direct3D11::_CreateSRV(
	ID3D11Resource *resource,
	DXGI_FORMAT format,
	int firstSlice,
	unsigned numSlices )const
{
	// Get dimension of resource so that we know what we're working with.
	D3D11_RESOURCE_DIMENSION type;
	resource->GetType( &type );

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	switch ( type )
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
	{
		// Get description of underlying texture.
		D3D11_TEXTURE1D_DESC texDesc;
		((ID3D11Texture1D*)resource)->GetDesc( &texDesc );

		srvDesc.Format = (format == DXGI_FORMAT_UNKNOWN) ? texDesc.Format : format;

		// Texture array
		if ( texDesc.ArraySize > 1 )
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
			srvDesc.Texture1DArray.MostDetailedMip = 0;
			srvDesc.Texture1DArray.MipLevels = texDesc.MipLevels;

			// firstSlice == -1 represents a view to the entire array.
			unsigned theFirstSlice = (firstSlice == -1) ? 0 : firstSlice;
			unsigned theSliceCount = (firstSlice == -1) ? texDesc.ArraySize : numSlices;

			srvDesc.Texture1DArray.FirstArraySlice = theFirstSlice;
			srvDesc.Texture1DArray.ArraySize = theSliceCount;
		}
		// Regular texture
		else
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
			srvDesc.Texture1D.MostDetailedMip = 0;
			srvDesc.Texture1D.MipLevels = texDesc.MipLevels;
		}

		break;
	}

	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
	{
		// Get description of underlying texture.
		D3D11_TEXTURE2D_DESC texDesc;
		((ID3D11Texture2D*)resource)->GetDesc( &texDesc );

		srvDesc.Format = (format == DXGI_FORMAT_UNKNOWN) ? texDesc.Format : format;

		// firstSlice == -1 represents a view to the entire array.
		unsigned theFirstSlice = (firstSlice == -1) ? 0 : firstSlice;
		unsigned theSliceCount = (firstSlice == -1) ? texDesc.ArraySize : numSlices;

		// Texture array
		if ( texDesc.ArraySize > 1 )
		{
			// Texture cube
			if ( texDesc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE )
			{
				// Single cube
				if ( texDesc.ArraySize == 6 )
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
					srvDesc.TextureCube.MostDetailedMip = 0;
					srvDesc.TextureCube.MipLevels = texDesc.MipLevels;
				}
				// Cube array
				else
				{
					srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
					srvDesc.TextureCubeArray.MostDetailedMip = 0;
					srvDesc.TextureCubeArray.MipLevels = texDesc.MipLevels;
					srvDesc.TextureCubeArray.First2DArrayFace = theFirstSlice;
					srvDesc.TextureCubeArray.NumCubes = (firstSlice == -1) ? texDesc.ArraySize / 6 : numSlices;
				}
			}
			// Multi sampled texture array
			else if ( texDesc.SampleDesc.Count > 1 )
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
				srvDesc.Texture2DMSArray.FirstArraySlice = theFirstSlice;
				srvDesc.Texture2DMSArray.ArraySize = theSliceCount;
			}
			// Regular texture array
			else
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				srvDesc.Texture2DArray.MostDetailedMip = 0;
				srvDesc.Texture2DArray.MipLevels = texDesc.MipLevels;
				srvDesc.Texture2DArray.FirstArraySlice = theFirstSlice;
				srvDesc.Texture2DArray.ArraySize = theSliceCount;
			}
		}
		// Regular texture
		else
		{
			// Multi sampled texture
			if ( texDesc.SampleDesc.Count > 1 )
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
			}
			// Regular texture array
			else
			{
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
			}
		}

		break;
	}

	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
	{
		// Get description of underlying texture.
		D3D11_TEXTURE3D_DESC texDesc;
		((ID3D11Texture3D*)resource)->GetDesc( &texDesc );

		srvDesc.Format = (format == DXGI_FORMAT_UNKNOWN) ? texDesc.Format : format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		srvDesc.Texture3D.MostDetailedMip = 0;
		srvDesc.Texture3D.MipLevels = texDesc.MipLevels;

		break;
	}

	default:
		Exception( "There's no implementation for resource dimension %i", type );
		return nullptr;
	}

	// SRV description complete; time to create it.
	ID3D11ShaderResourceView *srv = nullptr;
	_com_error hr = _d3dDevice->CreateShaderResourceView( resource, &srvDesc, &srv );
	if ( FAILED( hr.Error() ) )
	{
		Exception( "Failed to create shader resource view. Error: %S", hr.ErrorMessage());
	}

	return srv;
}

ID3D11UnorderedAccessView* Direct3D11::_CreateUAV(
	ID3D11Resource *resource,
	DXGI_FORMAT format,
	int firstSlice,
	unsigned numSlices
	)const
{
	// Get dimension of resource so that we know what we're working with.
	D3D11_RESOURCE_DIMENSION type;
	resource->GetType( &type );

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;

	switch ( type )
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
	{
		// Get description of underlying texture.
		D3D11_TEXTURE1D_DESC texDesc;
		((ID3D11Texture1D*)resource)->GetDesc( &texDesc );

		uavDesc.Format = (format == DXGI_FORMAT_UNKNOWN) ? texDesc.Format : format;

		// Texture array
		if ( texDesc.ArraySize > 1 )
		{
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1DARRAY;
			uavDesc.Texture1DArray.MipSlice = 0;

			// firstSlice == -1 represents a view to the entire array.
			unsigned theFirstSlice = (firstSlice == -1) ? 0 : firstSlice;
			unsigned theSliceCount = (firstSlice == -1) ? texDesc.ArraySize : numSlices;

			uavDesc.Texture1DArray.FirstArraySlice = theFirstSlice;
			uavDesc.Texture1DArray.ArraySize = theSliceCount;
		}
		// Regular texture
		else
		{
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
			uavDesc.Texture1D.MipSlice = 0;
		}

		break;
	}

	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
	{
		// Get description of underlying texture.
		D3D11_TEXTURE2D_DESC texDesc;
		((ID3D11Texture2D*)resource)->GetDesc( &texDesc );

		uavDesc.Format = (format == DXGI_FORMAT_UNKNOWN) ? texDesc.Format : format;

		// firstSlice == -1 represents a view to the entire array.
		unsigned theFirstSlice = (firstSlice == -1) ? 0 : firstSlice;
		unsigned theSliceCount = (firstSlice == -1) ? texDesc.ArraySize : numSlices;

		// Texture array
		if ( texDesc.ArraySize > 1 )
		{
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
			uavDesc.Texture2DArray.MipSlice = 0;
			uavDesc.Texture2DArray.FirstArraySlice = theFirstSlice;
			uavDesc.Texture2DArray.ArraySize = theSliceCount;
		}
		// Regular texture
		else
		{
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			uavDesc.Texture2D.MipSlice = 0;
		}

		break;
	}

	case D3D11_RESOURCE_DIMENSION_TEXTURE3D:
	{
		// Get description of underlying texture.
		D3D11_TEXTURE3D_DESC texDesc;
		((ID3D11Texture3D*)resource)->GetDesc( &texDesc );

		uavDesc.Format = (format == DXGI_FORMAT_UNKNOWN) ? texDesc.Format : format;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
		uavDesc.Texture3D.MipSlice = 0;

		// firstSlice == -1 represents a view to the entire volume.
		unsigned theFirstSlice = (firstSlice == -1) ? 0 : firstSlice;
		unsigned theSliceCount = (firstSlice == -1) ? texDesc.Depth : numSlices;

		uavDesc.Texture3D.FirstWSlice = theFirstSlice;
		uavDesc.Texture3D.WSize = theSliceCount;

		break;
	}

	default:
		Exception( "There's no implementation for resource dimension %i", type );
		return nullptr;
	}

	// UAV description complete; time to create it.
	ID3D11UnorderedAccessView *uav = nullptr;
	_com_error hr = _d3dDevice->CreateUnorderedAccessView( resource, &uavDesc, &uav );
	if ( FAILED( hr.Error() ) )
	{
		Exception( "Failed to create unordered access view. Error: %S", hr.ErrorMessage());
	}

	return uav;
}

ID3D11DepthStencilView* Direct3D11::_CreateDSV(
	ID3D11Resource *resource,
	DXGI_FORMAT format,
	bool readOnly,
	int firstSlice,
	unsigned numSlices
	)const
{
	// Get dimension of resource so that we know what we're working with.
	D3D11_RESOURCE_DIMENSION type;
	resource->GetType( &type );

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;

	// Read only stencil depends on format
	if ( readOnly )
		dsvDesc.Flags |= D3D11_DSV_READ_ONLY_DEPTH;

	switch ( type )
	{
	case D3D11_RESOURCE_DIMENSION_TEXTURE1D:
	{
		// Get description of underlying texture.
		D3D11_TEXTURE1D_DESC texDesc;
		((ID3D11Texture1D*)resource)->GetDesc( &texDesc );

		dsvDesc.Format = (format == DXGI_FORMAT_UNKNOWN) ? texDesc.Format : format;

		if ( readOnly )
		{
			if ( dsvDesc.Format == DXGI_FORMAT_D24_UNORM_S8_UINT || dsvDesc.Format == DXGI_FORMAT_D32_FLOAT_S8X24_UINT )
				dsvDesc.Flags |= D3D11_DSV_READ_ONLY_STENCIL;
		}

		// Texture array
		if ( texDesc.ArraySize > 1 )
		{
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1DARRAY;
			dsvDesc.Texture1DArray.MipSlice = 0;

			// firstSlice == -1 represents a view to the entire array.
			unsigned theFirstSlice = (firstSlice == -1) ? 0 : firstSlice;
			unsigned theSliceCount = (firstSlice == -1) ? texDesc.ArraySize : numSlices;

			dsvDesc.Texture1DArray.FirstArraySlice = theFirstSlice;
			dsvDesc.Texture1DArray.ArraySize = theSliceCount;
		}
		// Regular texture
		else
		{
			dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1D;
			dsvDesc.Texture1D.MipSlice = 0;
		}

		break;
	}

	case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
	{
		// Get description of underlying texture.
		D3D11_TEXTURE2D_DESC texDesc;
		((ID3D11Texture2D*)resource)->GetDesc( &texDesc );

		dsvDesc.Format = (format == DXGI_FORMAT_UNKNOWN) ? texDesc.Format : format;

		if ( readOnly )
		{
			if ( dsvDesc.Format == DXGI_FORMAT_D24_UNORM_S8_UINT || dsvDesc.Format == DXGI_FORMAT_D32_FLOAT_S8X24_UINT )
				dsvDesc.Flags |= D3D11_DSV_READ_ONLY_STENCIL;
		}

		// Texture array
		if ( texDesc.ArraySize > 1 )
		{
			// firstSlice == -1 represents a view to the entire array.
			unsigned theFirstSlice = (firstSlice == -1) ? 0 : firstSlice;
			unsigned theSliceCount = (firstSlice == -1) ? texDesc.ArraySize : numSlices;

			// Multi sampled texture array
			if ( texDesc.SampleDesc.Count > 1 )
			{
				dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
				dsvDesc.Texture2DMSArray.FirstArraySlice = theFirstSlice;
				dsvDesc.Texture2DMSArray.ArraySize = theSliceCount;
			}
			// Regular texture array
			else
			{
				dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				dsvDesc.Texture2DArray.MipSlice = 0;
				dsvDesc.Texture2DArray.FirstArraySlice = theFirstSlice;
				dsvDesc.Texture2DArray.ArraySize = theSliceCount;
			}
		}
		// Regular texture
		else
		{
			// Multi sampled texture
			if ( texDesc.SampleDesc.Count > 1 )
			{
				dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			}
			// Regular texture array
			else
			{
				dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				dsvDesc.Texture2D.MipSlice = 0;
			}
		}

		break;
	}

	default:
		Exception( "There's no implementation for resource dimension %i", type );
		return nullptr;
	}

	// DSV description complete; time to create it.
	ID3D11DepthStencilView *dsv = nullptr;
	_com_error hr = _d3dDevice->CreateDepthStencilView( resource, &dsvDesc, &dsv );
	if ( FAILED( hr.Error() ) )
	{
		Exception( "Failed to create depth stencil view. Error: %S", hr.ErrorMessage());
	}

	return dsv;
}

ID3D11DepthStencilState* Direct3D11::_CreateDSS(
	bool						depthEnable,
	D3D11_DEPTH_WRITE_MASK		depthWriteMask,
	D3D11_COMPARISON_FUNC		depthComparison,
	bool						stencilEnable,
	UINT8						stencilReadMask,
	UINT8						stencilWriteMask
	)const
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = depthEnable;
	dsDesc.DepthWriteMask = depthWriteMask;
	dsDesc.DepthFunc = depthComparison;

	// Stencil test parameters
	dsDesc.StencilEnable = stencilEnable;
	dsDesc.StencilReadMask = stencilReadMask;
	dsDesc.StencilWriteMask = stencilWriteMask;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	ID3D11DepthStencilState * pDSState;
	_com_error hr = _d3dDevice->CreateDepthStencilState(&dsDesc, &pDSState);
	if (FAILED(hr.Error()))
	{
		Exception("Failed to create depth stencil state. Error: %S", hr.ErrorMessage());
	}


	return pDSState;
}

ID3D11RasterizerState* Direct3D11::_CreateRS(
	D3D11_FILL_MODE				fillMode,
	D3D11_CULL_MODE				cullMode,
	bool						frontCounterClockwise,
	bool						depthBias,
	float						depthBiasClamp,
	float						slopeScaledDepthBias,
	bool						depthClipEnable,
	bool						scissorEnable,
	bool						multiSampleEnable,
	bool						antialiasedLineEnable 
	)const
{
	D3D11_RASTERIZER_DESC rasterizerState;
	rasterizerState.FillMode = fillMode;
	rasterizerState.CullMode = cullMode;
	rasterizerState.FrontCounterClockwise = frontCounterClockwise;
	rasterizerState.DepthBias = depthBias;
	rasterizerState.DepthBiasClamp = depthBiasClamp;
	rasterizerState.SlopeScaledDepthBias = slopeScaledDepthBias;
	rasterizerState.DepthClipEnable = depthClipEnable;
	rasterizerState.ScissorEnable = scissorEnable;
	rasterizerState.MultisampleEnable = multiSampleEnable;
	rasterizerState.AntialiasedLineEnable = antialiasedLineEnable;

	ID3D11RasterizerState * RasterState;
	_com_error hr = _d3dDevice->CreateRasterizerState(&rasterizerState, &RasterState);
	if (FAILED(hr.Error()))
	{
		Exception("Failed to create Raster state. Error: %S", hr.ErrorMessage());
	}
	return RasterState;
}

ID3D11BlendState * Direct3D11::_CreateBS(
	bool					blendEnabled,
	UINT8					renderTargetWriteMask) const
{
	D3D11_BLEND_DESC desc;
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	desc.RenderTarget[0].BlendEnable = blendEnabled;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = renderTargetWriteMask;
	ID3D11BlendState* bs;
	
	_com_error hr = _d3dDevice->CreateBlendState(&desc, &bs);
	if (FAILED(hr.Error()))
	{
		Exception("Failed to create Blend State. Error: %S", hr.ErrorMessage());
	}
	return bs;
}

ID3D11BlendState * Direct3D11::_CreateBS(D3D11_BLEND_DESC * blendDesc) const
{
	ID3D11BlendState* bs;
	_com_error hr = _d3dDevice->CreateBlendState(blendDesc, &bs);
	if (FAILED(hr.Error()))
	{
		Exception("Failed to create Blend State. Error: %S", hr.ErrorMessage());
	}
	return bs;
}

StructuredBuffer Direct3D11::CreateStructuredBuffer(
	unsigned stride,
	unsigned elementCount,
	bool CPUWrite,
	bool GPUWrite,
	const void *initialData,
	bool addStructureCounter
	)const
{
	StructuredBuffer structuredBuffer;

	// Create the underlying resource.

	D3D11_BUFFER_DESC bufDesc;
	bufDesc.ByteWidth = stride * elementCount;
	bufDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufDesc.StructureByteStride = stride;

	if ( !CPUWrite && !GPUWrite )
	{
		bufDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bufDesc.CPUAccessFlags = 0;
	}
	else if ( !CPUWrite && GPUWrite )
	{
		bufDesc.Usage = D3D11_USAGE_DEFAULT;
		bufDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		bufDesc.CPUAccessFlags = 0;
	}
	else if ( CPUWrite && !GPUWrite )
	{
		bufDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		bufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		Exception( "Cannot create a structured buffer with both CPU write and GPU write access" );
		return StructuredBuffer();
	}

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = initialData;
	initData.SysMemPitch = 0; // Not used for buffers
	initData.SysMemSlicePitch = 0; // Not used for buffers

	_com_error hr = _d3dDevice->CreateBuffer( &bufDesc, initialData ? &initData : nullptr, &structuredBuffer.Buffer );
	if ( FAILED( hr.Error() ) )
	{
		Exception( "Failed to create structured buffer. Error: %S", hr.ErrorMessage());
	}

	// Create views to the buffer.

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = elementCount;

	hr = _d3dDevice->CreateShaderResourceView( structuredBuffer.Buffer, &srvDesc, &structuredBuffer.SRV );
	if (FAILED(hr.Error()))
	{
		SAFE_RELEASE(structuredBuffer.Buffer);
		Exception( "Failed to create shader resource view. Error: %S", hr.ErrorMessage());
	}

	if ( GPUWrite )
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = elementCount;
		uavDesc.Buffer.Flags = addStructureCounter ? D3D11_BUFFER_UAV_FLAG_COUNTER : 0;

		hr = _d3dDevice->CreateUnorderedAccessView( structuredBuffer.Buffer, &uavDesc, &structuredBuffer.UAV );
		if (FAILED(hr.Error()))
		{
			SAFE_RELEASE(structuredBuffer.Buffer);
			SAFE_RELEASE(structuredBuffer.SRV);
			Exception( "Failed to create unordered access view. Error: %S", hr.ErrorMessage());
		}
	}

	// Cache stuff.
	structuredBuffer.Stride = stride;
	structuredBuffer.ElementCount = elementCount;

	return structuredBuffer;
}

StructuredBuffer Direct3D11::CreateAppendConsumeBuffer(
	unsigned stride,
	unsigned elementCount,
	const void *initialData
	)const
{
	StructuredBuffer structuredBuffer;

	// Create the underlying resource.

	D3D11_BUFFER_DESC bufDesc;
	bufDesc.ByteWidth = stride * elementCount;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufDesc.StructureByteStride = stride;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = initialData;
	initData.SysMemPitch = 0; // Not used for buffers
	initData.SysMemSlicePitch = 0; // Not used for buffers

	_com_error hr = _d3dDevice->CreateBuffer( &bufDesc, initialData ? &initData : nullptr, &structuredBuffer.Buffer );
	if (FAILED(hr.Error()))
	{
		Exception( "Failed to create structured buffer. Error: %S", hr.ErrorMessage());
	}

	// Create views to the buffer.

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = elementCount;

	hr = _d3dDevice->CreateShaderResourceView( structuredBuffer.Buffer, &srvDesc, &structuredBuffer.SRV );
	if (FAILED(hr.Error()))
	{
		Exception( "Failed to create shader resource view. Error: %S", hr.ErrorMessage());
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = elementCount;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_APPEND;

	hr = _d3dDevice->CreateUnorderedAccessView( structuredBuffer.Buffer, &uavDesc, &structuredBuffer.UAV );
	if (FAILED(hr.Error()))
	{
		Exception( "Failed to create unordered access view. Error: %S", hr.ErrorMessage());
	}

	// Cache stuff.
	structuredBuffer.Stride = stride;
	structuredBuffer.ElementCount = elementCount;

	return structuredBuffer;
}

const void Direct3D11::DeleteStructuredBuffer( StructuredBuffer &sb )const
{
	SAFE_RELEASE( sb.Buffer );
	SAFE_RELEASE( sb.SRV );
	SAFE_RELEASE( sb.UAV );
	sb = StructuredBuffer();
}

IndirectArgsBuffer Direct3D11::CreateIndirectArgsBuffer(
	unsigned size,
	void *initialData
	)const
{
	IndirectArgsBuffer indirectArgs;

	// Create underlying resource.
	D3D11_BUFFER_DESC bufDesc;
	bufDesc.ByteWidth = size;
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.BindFlags = 0;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = D3D11_RESOURCE_MISC_DRAWINDIRECT_ARGS;
	bufDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = initialData;
	initData.SysMemPitch = 0; // Not used for buffers
	initData.SysMemSlicePitch = 0; // Not used for buffers

	_com_error hr = _d3dDevice->CreateBuffer( &bufDesc, initialData ? &initData : nullptr, &indirectArgs.Buffer );
	if (FAILED(hr.Error()))
	{
		Exception( "Failed to create indirect args buffer. Error: %S", hr.ErrorMessage());
	}

	// Cache stuff
	indirectArgs.Size = size;

	return indirectArgs;
}

const void Direct3D11::DeleteIndirectArgsBuffer( IndirectArgsBuffer &iab )const
{
	SAFE_RELEASE( iab.Buffer );
	iab = IndirectArgsBuffer();
}
}}