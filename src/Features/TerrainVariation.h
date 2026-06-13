#pragma once

/** @brief Reduces terrain texture tiling artifacts by adding distance-based variation to texture sampling. */
struct TerrainVariation : Feature
{
private:
	static constexpr std::string_view MOD_ID = "148123";

public:
	/** @brief Returns the internal name of this feature. */
	virtual inline std::string GetName() override { return "Terrain Variation"; }
	/** @brief Returns the localized display name for the UI. */
	virtual std::string GetDisplayName() override { return T("feature.terrain_variation.name", "Terrain Variation"); }
	/** @brief Returns the short identifier name. */
	virtual inline std::string GetShortName() override { return "TerrainVariation"; }
	/** @brief Returns the Nexus Mods URL for this feature. */
	virtual inline std::string GetFeatureModLink() override { return MakeNexusModURL(MOD_ID); }
	/** @brief Returns the shader preprocessor define name. */
	virtual inline std::string_view GetShaderDefineName() override { return "TERRAIN_VARIATION"; }
	/** @brief Indicates whether this feature injects shader defines for the given shader type. */
	virtual inline bool HasShaderDefine(RE::BSShader::Type shaderType) override
	{
		return (shaderType == RE::BSShader::Type::Lighting);
	}
	/** @brief Indicates this is not a core feature. */
	virtual bool IsCore() const override { return false; };
	/** @brief Returns the feature category for menu organization. */
	virtual std::string_view GetCategory() const override { return FeatureCategories::kLandscapeAndTextures; }

	/** @brief Returns a description and list of key features for the UI summary. */
	virtual std::pair<std::string, std::vector<std::string>> GetFeatureSummary() override
	{
		return { T("feature.terrain_variation.description", "Terrain Variation reduces the repeating pattern effect on terrain textures.\nThis technique creates more natural-looking terrain by adding variation to texture sampling."),
			{ T("feature.terrain_variation.key_feature_1", "Reduces terrain texture tiling"),
				T("feature.terrain_variation.key_feature_2", "Adjustable distance-based blending"),
				T("feature.terrain_variation.key_feature_3", "Improved terrain visual quality"),
				T("feature.terrain_variation.key_feature_4", "Compatible with Extended Materials parallax") } };
	};

	struct Settings
	{
		uint enableTilingFix = true;
		uint enableLODTerrainTilingFix = true;
		float pad0[2];
	} settings;

	/** @brief Draws the ImGui settings panel for Terrain Variation configuration. */
	virtual void DrawSettings() override;
	/** @brief Suppresses the default failed-load message display. */
	virtual bool DrawFailLoadMessage() const override;
	/** @brief Loads terrain variation settings from a JSON object. */
	virtual void LoadSettings(json& o_json) override;
	/** @brief Saves current terrain variation settings to a JSON object. */
	virtual void SaveSettings(json& o_json) override;
	/** @brief Restores all terrain variation settings to their default values. */
	virtual void RestoreDefaultSettings() override;

	/** @brief Initializes the feature and applies shader settings after plugin load. */
	virtual void PostPostLoad() override;
	/** @brief Marks the vertex descriptor as dirty to trigger a shader settings update. */
	void UpdateShaderSettings();
};