// Copyright Colby Hall. All Rights Reserved.

#pragma once

#include <gpu/graphics_pipeline.h>
#include <gpu/d3d12/d3d12.h>

GPU_NAMESPACE_BEGIN

class D3D12GraphicsPipeline : public GraphicsPipelineInterface {
public:
	explicit D3D12GraphicsPipeline(GraphicsPipelineConfig&& config);

	const GraphicsPipelineConfig& config() const override { return m_config; }

	GraphicsPipelineConfig m_config;
	ComPtr<ID3D12PipelineState> m_pipeline;
};

GPU_NAMESPACE_END
