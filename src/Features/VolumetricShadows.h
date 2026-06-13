#pragma once

#include "Buffer.h"

/** @brief Provides downsampled VSM shadow maps for use by effects like particles and decals. */
struct VolumetricShadows : Feature
{
public:
	/** @brief Returns the internal feature name. */
	virtual inline std::string GetName() override { return "Volumetric Shadows"; }
	/** @brief Returns the user-facing display name. */
	virtual std::string GetDisplayName() override { return T("feature.volumetric_shadows.name", "Volumetric Shadows"); }
	/** @brief Returns the short identifier used for file paths and logging. */
	virtual inline std::string GetShortName() override { return "VolumetricShadows"; }
	/** @brief Returns the HLSL preprocessor define name for this feature. */
	virtual inline std::string_view GetShaderDefineName() override { return "VOLUMETRIC_SHADOWS"; }
	/** @brief Returns the UI category this feature belongs to. */
	virtual std::string_view GetCategory() const override { return FeatureCategories::kLighting; }
	/** @brief Indicates this is a core feature bundled with the main mod. */
	virtual bool IsCore() const override { return true; }
	/** @brief Indicates this feature appears in the settings menu. */
	virtual bool IsInMenu() const override { return true; }

	static constexpr uint32_t kSharedShadowMapShaderSlot = 18;

	/** @brief Returns a summary description and list of key features for the UI. */
	virtual std::pair<std::string, std::vector<std::string>> GetFeatureSummary() override
	{
		return { T("feature.volumetric_shadows.description", "Volumetric Shadows provides downsampled VSM shadow maps for use by effects like particles and decals.\nThis improves shadow quality on transparent objects with minimal performance impact."),
			{ T("feature.volumetric_shadows.key_feature_1", "Downsampled VSM shadows"),
				T("feature.volumetric_shadows.key_feature_2", "Gaussian blur filtering"),
				T("feature.volumetric_shadows.key_feature_3", "Multi-cascade support"),
				T("feature.volumetric_shadows.key_feature_4", "Optimized for effects rendering") } };
	};

	/** @brief Returns whether this feature injects shader defines for the given shader type. */
	bool HasShaderDefine(RE::BSShader::Type shaderType) override;

	// Compute shaders
	ID3D11ComputeShader* downsampleShadowMip0CS = nullptr;
	ID3D11ComputeShader* downsampleShadowMip1CS = nullptr;
	ID3D11ComputeShader* blurShadowHorizontalCS = nullptr;
	ID3D11ComputeShader* blurShadowVerticalCS = nullptr;

	ID3D11ShaderResourceView* shadowView = nullptr;

	// Downsampled shadow texture with 2 mip levels
	ID3D11Texture2D* shadowCopyTexture = nullptr;
	ID3D11ShaderResourceView* shadowCopySRV = nullptr;
	ID3D11ShaderResourceView* shadowCopyMip0SRV = nullptr;
	ID3D11ShaderResourceView* shadowCopyMip1SRV = nullptr;
	ID3D11UnorderedAccessView* shadowCopyMip0UAV = nullptr;
	ID3D11UnorderedAccessView* shadowCopyMip1UAV = nullptr;
	uint32_t shadowCopyWidth = 0;
	uint32_t shadowCopyHeight = 0;

	// Temporary texture for blur intermediate result
	ID3D11Texture2D* shadowBlurTempTexture = nullptr;
	ID3D11ShaderResourceView* shadowBlurTempMip0SRV = nullptr;
	ID3D11ShaderResourceView* shadowBlurTempMip1SRV = nullptr;
	ID3D11UnorderedAccessView* shadowBlurTempMip0UAV = nullptr;
	ID3D11UnorderedAccessView* shadowBlurTempMip1UAV = nullptr;

	// Samplers
	ID3D11SamplerState* linearSampler = nullptr;

	/** @brief Draws the ImGui settings panel for volumetric shadows. */
	virtual void DrawSettings() override;
	/** @brief Creates GPU resources including samplers, compute shaders, and shadow textures. */
	virtual void SetupResources() override;
	/** @brief Releases and recompiles all compute shaders. */
	virtual void ClearShaderCache() override;

	/** @brief Copies and downsamples the directional shadow map, then applies Gaussian blur filtering. */
	void CopyShadowLightData();

	/** @brief Loads feature settings from the JSON configuration. */
	virtual void LoadSettings(json& o_json) override;
	/** @brief Saves feature settings to the JSON configuration. */
	virtual void SaveSettings(json& o_json) override;
	/** @brief Restores all settings to their default values. */
	virtual void RestoreDefaultSettings() override;

	/** @brief Installs the engine hook for binding the shared shadow map SRV. */
	virtual void PostPostLoad() override;

private:
	static void SetSharedShadowMapSRV(ID3D11DeviceContext* a_context, ID3D11ShaderResourceView* a_srv);
};
