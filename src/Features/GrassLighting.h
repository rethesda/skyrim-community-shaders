#pragma once

#include "Buffer.h"

struct GrassLighting : Feature
{
public:
	/** @brief Returns the internal name of this feature. */
	virtual inline std::string GetName() override { return "Grass Lighting"; }
	/** @brief Returns the localized display name for the UI. */
	virtual std::string GetDisplayName() override { return T("feature.grass_lighting.name", "Grass Lighting"); }
	/** @brief Returns the short identifier used for file paths and settings keys. */
	virtual inline std::string GetShortName() override { return "GrassLighting"; }
	/** @brief Returns the HLSL preprocessor define name for this feature. */
	virtual inline std::string_view GetShaderDefineName() override { return "GRASS_LIGHTING"; }
	/** @brief Returns true only for Grass shader type. */
	virtual bool HasShaderDefine(RE::BSShader::Type shaderType) override { return shaderType == RE::BSShader::Type::Grass; };
	/** @brief Returns the category this feature belongs to. */
	virtual std::string_view GetCategory() const override { return FeatureCategories::kGrass; }

	/** @brief Returns a localized description and key feature bullet points for the UI. */
	virtual std::pair<std::string, std::vector<std::string>> GetFeatureSummary() override
	{
		return { T("feature.grass_lighting.description", "Grass Lighting enhances grass rendering with improved lighting, specularity, and subsurface scattering.\nThis makes grass appear more natural and responsive to lighting conditions."),
			{ T("feature.grass_lighting.key_feature_1", "Enhanced grass lighting model"),
				T("feature.grass_lighting.key_feature_2", "Specular highlights on grass"),
				T("feature.grass_lighting.key_feature_3", "Subsurface scattering effects"),
				T("feature.grass_lighting.key_feature_4", "Improved grass visual quality"),
				T("feature.grass_lighting.key_feature_5", "Configurable material properties") } };
	};

	struct alignas(16) Settings
	{
		float Glossiness = 20.0f;
		float SpecularStrength = 0.5f;
		float SubsurfaceScatteringAmount = 1.0f;
		uint OverrideComplexGrassSettings = false;
		float BasicGrassBrightness = 1.0f;
		float ComplexGrassThreshold = 0.03f;
		float2 pad0;
	};
	STATIC_ASSERT_ALIGNAS_16(Settings);

	Settings settings;

	/** @brief Draws the ImGui settings UI for grass specular, SSS, and lighting options. */
	virtual void DrawSettings() override;

	/** @brief Loads grass lighting settings from JSON. */
	virtual void LoadSettings(json& o_json) override;
	/** @brief Saves grass lighting settings to JSON. */
	virtual void SaveSettings(json& o_json) override;

	/** @brief Resets all settings to their default values. */
	virtual void RestoreDefaultSettings() override;

};
