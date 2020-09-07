#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"
#include "Content\SampleFpsTextRenderer.h"
#include <SimpleMath.h>
#include "AnimatedTexture.h"
#include "ScrollingBackground.h"

// Renders Direct2D and 3D content on the screen.
namespace AnimatedSprites
{
	class AnimatedSpritesMain : public DX::IDeviceNotify
	{
	public:
		AnimatedSpritesMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~AnimatedSpritesMain();
		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;
		std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
		std::unique_ptr<AnimatedTexture> m_ship;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
		DirectX::SimpleMath::Vector2 m_shipPos;
		std::unique_ptr<ScrollingBackground> m_stars;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backgroundTex;
	};
}