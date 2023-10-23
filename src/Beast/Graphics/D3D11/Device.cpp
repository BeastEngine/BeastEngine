#include "Beast/Graphics/D3D11/Device.h"
#include "Beast/Common/UUIDGenerator.h"

namespace be::graphics::d3d11
{
    Device::Device(const IWindow& window)
    {
        const auto hwnd = window.GetHandle();

        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 0;
        sd.BufferDesc.RefreshRate.Denominator = 0;
        sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;
        sd.OutputWindow = hwnd;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags = 0;

        const auto result = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            D3D11_CREATE_DEVICE_DEBUG,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &sd,
            &m_swapChain,
            &m_device,
            nullptr,
            &m_context
        );
        CheckResult(result);
    }

    wrl::ComPtr<ID3D11DeviceContext> Device::GetContext() const noexcept
    {
        return m_context;
    }

    wrl::ComPtr<IDXGISwapChain> Device::GetSwapChain() const noexcept
    {
        return m_swapChain;
    }

    wrl::ComPtr<ID3D11RenderTargetView> Device::CreateRenderTargetView(IDXGISwapChain& swapChain) const
    {
        wrl::ComPtr<ID3D11RenderTargetView> renderTargetView;

        wrl::ComPtr<ID3D11Resource> backBuffer = nullptr;
        CheckResult(swapChain.GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer));
        CheckResult(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView));

        return renderTargetView;
    }

    graphics::VertexBuffer Device::CreateVertexBuffer()
    {
        graphics::VertexBuffer buffer{.id = GenerateUUID4()};
        m_vertexBuffers.insert({buffer.id, {*m_device.Get()}});

        return buffer;
    }
} // namespace be::graphics::d3d11
