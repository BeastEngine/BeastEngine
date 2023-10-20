#pragma once
#include "Beast/Graphics/IContext.h"
#include "Beast/Graphics/D3D11/D3D11.h"
#include "Beast/Graphics/D3D11/VertexBuffer.h"

#include "Beast/Windows/IWindow.h"
#include "Beast/Common/IdGenerators/IUuIdGenerator.h"

namespace be::graphics::d3d11
{
    namespace wrl = Microsoft::WRL;

    class Context : public IContext
    {
    public:
        Context(const IWindow& window, Shared<IUuIdGenerator> idGenerator);

        graphics::VertexBuffer CreateVertexBuffer() override;
        
        void Clear(const Color& color) const noexcept override;
        void Present() const noexcept override;

    private:
        Shared<IUuIdGenerator> m_idGenerator;

        wrl::ComPtr<ID3D11Device> m_device = nullptr;
        wrl::ComPtr<IDXGISwapChain> m_swapChain = nullptr;
        wrl::ComPtr<ID3D11DeviceContext> m_context = nullptr;
        wrl::ComPtr<ID3D11RenderTargetView> m_renderTargetView = nullptr;

        std::unordered_map<Id, d3d11::VertexBuffer> m_vertexBuffers;
    };
} // namespace be::graphics::d3d11
