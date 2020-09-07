#include "pch.h"
#include "AnimatedSpritesMain.h"
#include "Common\DirectXHelper.h"


using namespace AnimatedSprites;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;
using namespace DirectX;

// Loads and initializes application assets when the application is loaded.
AnimatedSpritesMain::AnimatedSpritesMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.
	m_sceneRenderer = std::unique_ptr<Sample3DSceneRenderer>(new Sample3DSceneRenderer(m_deviceResources));

	m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	m_spriteBatch = std::make_unique<SpriteBatch>(m_deviceResources->GetD3DDeviceContext());

	DX::ThrowIfFailed(CreateWICTextureFromFile(m_deviceResources->GetD3DDevice(), L"shipanimated.png",
		nullptr, m_texture.ReleaseAndGetAddressOf()));

	m_ship = std::make_unique<AnimatedTexture>();
	m_ship->Load(m_texture.Get(), 4, 20);

	float backBufferWidth = m_deviceResources->GetLogicalSize().Width;
	float backBufferHeight = m_deviceResources->GetLogicalSize().Height;

	m_shipPos.x = float(backBufferWidth / 2);
	m_shipPos.y = float((backBufferHeight / 2) + (backBufferHeight / 4));

	DX::ThrowIfFailed(CreateWICTextureFromFile(m_deviceResources->GetD3DDevice(), L"starfield.png",
		nullptr, m_backgroundTex.ReleaseAndGetAddressOf()));

	m_stars = std::make_unique<ScrollingBackground>();
	m_stars->Load(m_backgroundTex.Get());

	m_stars->SetWindow(backBufferWidth, backBufferHeight);


	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

AnimatedSpritesMain::~AnimatedSpritesMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void AnimatedSpritesMain::CreateWindowSizeDependentResources() 
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	m_sceneRenderer->CreateWindowSizeDependentResources();
}

// Updates the application state once per frame.
void AnimatedSpritesMain::Update() 
{
	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		m_sceneRenderer->Update(m_timer);
		m_fpsTextRenderer->Update(m_timer);
		m_ship->Update(m_timer.GetElapsedSeconds());
		m_stars->Update(m_timer.GetElapsedSeconds() * 500);

	});
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool AnimatedSpritesMain::Render() 
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
	//m_sceneRenderer->Render();
	//m_fpsTextRenderer->Render();

	m_spriteBatch->Begin();

	m_stars->Draw(m_spriteBatch.get());
	m_ship->Draw(m_spriteBatch.get(), m_shipPos);

	m_spriteBatch->End();
	return true;
}

// Notifies renderers that device resources need to be released.
void AnimatedSpritesMain::OnDeviceLost()
{
	m_sceneRenderer->ReleaseDeviceDependentResources();
	m_fpsTextRenderer->ReleaseDeviceDependentResources();
	m_ship.reset();
	m_spriteBatch.reset();
	m_texture.Reset();
	m_stars.reset();
	m_backgroundTex.Reset();
}

// Notifies renderers that device resources may now be recreated.
void AnimatedSpritesMain::OnDeviceRestored()
{
	m_sceneRenderer->CreateDeviceDependentResources();
	m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
