#pragma once

struct LODBlending : Feature
{
	/** @brief Returns the internal name of this feature. */
	virtual inline std::string GetName() override { return "LOD Blending"; }
	/** @brief Returns the localized display name for the UI. */
	virtual std::string GetDisplayName() override { return T("feature.lod_blending.name", "LOD Blending"); }
	/** @brief Returns the short identifier used for file paths and logging. */
	virtual inline std::string GetShortName() override { return "LODBlending"; }
	/** @brief Returns the shader preprocessor define name for this feature. */
	virtual inline std::string_view GetShaderDefineName() override { return "LOD_BLENDING"; }
	/** @brief Returns the UI category this feature belongs to. */
	virtual std::string_view GetCategory() const override { return FeatureCategories::kLandscapeAndTextures; }
	/** @brief Returns a localized description and list of key features for the UI summary panel. */
	virtual std::pair<std::string, std::vector<std::string>> GetFeatureSummary() override
	{
		return { T("feature.lod_blending.description", "Provides seamless visual transitions between Level of Detail (LOD) objects and full-detail objects, eliminating harsh transitions and creating smooth visual continuity."),
			{ T("feature.lod_blending.key_feature_1", "Smooth LOD object brightness blending"),
				T("feature.lod_blending.key_feature_2", "Enhanced terrain LOD appearance matching"),
				T("feature.lod_blending.key_feature_3", "Snow-specific LOD brightness adjustment"),
				T("feature.lod_blending.key_feature_4", "Optional terrain vertex color modification"),
				T("feature.lod_blending.key_feature_5", "Seamless transition between detail levels") } };
	};

	/** @brief Indicates this feature injects a shader define for all shader types. */
	virtual inline bool HasShaderDefine(RE::BSShader::Type) override { return true; };

	struct Settings
	{
		float LODTerrainBrightness = 1;
		float LODObjectBrightness = 1;
		float LODObjectSnowBrightness = 1;
		uint DisableTerrainVertexColors = false;
		float LODTerrainGamma = 1;
		float LODObjectGamma = 1;
		float LODObjectSnowGamma = 1;
		float pad;
	};

	Settings settings;

	/** @brief Draws the ImGui settings UI for LOD brightness, gamma, and vertex color configuration. */
	virtual void DrawSettings() override;

	/** @brief Loads feature settings from the provided JSON object. */
	virtual void LoadSettings(json& o_json) override;
	/** @brief Saves feature settings to the provided JSON object. */
	virtual void SaveSettings(json& o_json) override;

	/** @brief Resets all settings to their default values. */
	virtual void RestoreDefaultSettings() override;

	/** @brief Indicates this is a core feature bundled with the main mod. */
	virtual bool IsCore() const override { return true; };
};
