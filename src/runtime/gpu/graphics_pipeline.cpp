// Copyright Colby Hall. All Rights Reserved.

#include <gpu/context.h>
#include <gpu/d3d12/d3d12_graphics_pipeline.h>

GPU_NAMESPACE_BEGIN

GraphicsPipeline GraphicsPipeline::make(GraphicsPipelineConfig&& config) {
	auto& context = Context::the();

	Option<Shared<GraphicsPipelineInterface, SMode::Atomic>> interface;
	switch (context.backend()) {
		case Backend::D3D12:
			interface = make_shared<D3D12GraphicsPipeline, SMode::Atomic>(core::forward<GraphicsPipelineConfig>(config));
			break;
	}

	return GraphicsPipeline { interface.unwrap() };
}

GPU_NAMESPACE_END
