#pragma once
#include "Audio/Audio.h"
#include "Image/Image.h"
#include "Fence/Fence.h"
#include "Queue/Queue.h"
#include "Buffer/Buffer.h"
#include "Memory/Memory.h"
#include "Shader/Shader.h"
#include "Drawer/Drawer.h"
#include "Window/Window.h"
#include "Sampler/Sampler.h"
#include "Scissor/Scissor.h"
#include "Viewport/Viewport.h"
#include "SwapChain/SwapChain.h"
#include "ImageView/ImageView.h"
#include "Semaphore/Semaphore.h"
#include "RenderPass/RenderPass.h"
#include "GLFWManager/GLFWManager.h"
#include "CommandPool/CommandPool.h"
#include "FrameBuffer/FrameBuffer.h"
#include "VertexInput/VertexInput.h"
#include "WindowSurface/WindowSurface.h"
#include "CommandBuffer/CommandBuffer.h"
#include "DescriptorSet/DescriptorSet.h"
#include "LogicalDevice/LogicalDevice.h"
#include "PhysicalDevice/PhysicalDevice.h"
#include "VulkanInstance/VulkanInstance.h"
#include "DescriptorPool/DescriptorPool.h"
#include "PipelineLayout/PipelineLayout.h"
#include "ComputePipeline/ComputePipeline.h"
#include "PipelineBarrier/PipelineBarrier.h"
#include "GraphicsPipeline/GraphicsPipeline.h"
#include "DescriptorSetLayout/DescriptorSetLayout.h"




class Core{
    
public:
    
    using Audio = ::Audio;
    using Queue = ::Queue;
    using Image = ::Image;
    using Fence = ::Fence;
    using Drawer = ::Drawer;
    using Buffer = ::Buffer;
    using Memory = ::Memory;
    using Shader = ::Shader;
    using Window = ::Window;
    using Scissor = ::Scissor;
    using Sampler = ::Sampler;
    using Viewport = ::Viewport;
    using SwapChain = ::SwapChain;
    using ImageView = ::ImageView;
    using Semaphore = ::Semaphore;
    using RenderPass = ::RenderPass;
    using CommandPool = ::CommandPool;
    using FrameBuffer = ::FrameBuffer;
    using GLFWManager = ::GLFWManager;
    using VertexInput = ::VertexInput;
    using DescriptorSet = ::DescriptorSet;
    using CommandBuffer = ::CommandBuffer;
    using WindowSurface = ::WindowSurface;
    using LogicalDevice = ::LogicalDevice;
    using DescriptorPool = ::DescriptorPool;
    using PipelineLayout = ::PipelineLayout;
    using PhysicalDevice = ::PhysicalDevice;
    using VulkanInstance = ::VulkanInstance;
    using PipelineBarrier = ::PipelineBarrier;
    using ComputePipeline = ::ComputePipeline;
    using GraphicsPipeline = ::GraphicsPipeline;
    using DescriptorSetLayout = ::DescriptorSetLayout;
};
