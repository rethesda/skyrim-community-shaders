#pragma once

struct CloudShadows : Feature
{
private:
	static constexpr std::string_view MOD_ID = "139185";

public:
	struct alignas(16) Settings
	{
		float Opacity = 0.8f;
		float pad[3];
	};

	Settings settings;

	/** @brief Returns the internal name of this feature. */
	virtual inline std::string GetName() override { return "Cloud Shadows"; }
	/** @brief Returns the localized display name for the UI. */
	virtual std::string GetDisplayName() override { return T("feature.cloud_shadows.name", "Cloud Shadows"); }
	/** @brief Returns the short identifier used for file paths and settings keys. */
	virtual inline std::string GetShortName() override { return "CloudShadows"; }
	/** @brief Returns the Nexus Mods URL for this feature. */
	virtual inline std::string GetFeatureModLink() override { return MakeNexusModURL(MOD_ID); }
	/** @brief Returns the category this feature belongs to. */
	virtual std::string_view GetCategory() const override { return FeatureCategories::kSky; }
	/** @brief Returns the HLSL preprocessor define name for this feature. */
	virtual inline std::string_view GetShaderDefineName() override { return "CLOUD_SHADOWS"; }
	/** @brief Returns a localized description and key feature bullet points for the UI. */
	virtual std::pair<std::string, std::vector<std::string>> GetFeatureSummary() override
	{
		return { T("feature.cloud_shadows.description", "Adds realistic cloud shadows that move across the landscape, creating dynamic lighting changes as clouds pass overhead, enhancing atmospheric immersion."),
			{ T("feature.cloud_shadows.key_feature_1", "Dynamic cloud shadow projection on terrain and objects"),
				T("feature.cloud_shadows.key_feature_2", "Configurable shadow opacity for artistic control"),
				T("feature.cloud_shadows.key_feature_3", "Real-time shadow movement synchronized with cloud motion"),
				T("feature.cloud_shadows.key_feature_4", "Cubemap-based shadow calculation for accurate projection"),
				T("feature.cloud_shadows.key_feature_5", "Enhanced sky rendering integration") } };
	};

	/** @brief Returns true for all shader types, enabling cloud shadow defines globally. */
	virtual inline bool HasShaderDefine(RE::BSShader::Type) override { return true; }

	bool overrideSky = false;
	/**
	 * @brief Applies sky shader render state overrides for cloud shadow capture.
	 *
	 * When overrideSky is set, redirects rendering to the cloud occlusion cubemap
	 * and configures the appropriate blend state and depth resources.
	 */
	void SkyShaderHacks();

	Texture2D* texCubemapCloudOcc = nullptr;
	Texture2D* texCubemapCloudOccCopy = nullptr;

	ID3D11RenderTargetView* cubemapCloudOccRTVs[6] = { nullptr };
	ID3D11RenderTargetView* cubemapCloudOccCopyRTVs[6] = { nullptr };

	ID3D11BlendState* cloudShadowBlendState = nullptr;

	/** @brief Creates cubemap textures, SRVs, RTVs, and blend state for cloud shadow rendering. */
	virtual void SetupResources() override;

	/** @brief Draws the ImGui settings UI for cloud shadow opacity. */
	virtual void DrawSettings() override;

	/** @brief Loads cloud shadow settings from JSON. */
	virtual void LoadSettings(json& o_json) override;
	/** @brief Saves cloud shadow settings to JSON. */
	virtual void SaveSettings(json& o_json) override;

	/** @brief Resets all settings to their default values. */
	virtual void RestoreDefaultSettings() override;

	/**
	 * @brief Clears the cloud occlusion render target for a given cubemap face if not yet cleared this frame.
	 * @param side Cubemap face index (0-5).
	 */
	void CheckResourcesSide(int side);
	/**
	 * @brief Checks if the current sky render pass is rendering clouds to the reflections cubemap and flags it for override.
	 * @param Pass The BSRenderPass being set up for rendering.
	 */
	void ModifySky(RE::BSRenderPass* Pass);

	/** @brief Copies the cloud occlusion cubemap and binds it as a shader resource for the reflections prepass. */
	virtual void ReflectionsPrepass() override;
	/** @brief Binds the cloud occlusion cubemap as a shader resource for the early prepass. */
	virtual void EarlyPrepass() override;

	/** @brief Installs the BSSkyShader hooks after all plugins have loaded. */
	virtual inline void PostPostLoad() override { Hooks::Install(); }

	struct Hooks
	{
		struct BSSkyShader_SetupMaterial
		{
			static void thunk(RE::BSShader* This, RE::BSRenderPass* Pass, uint32_t RenderFlags);
			static inline REL::Relocation<decltype(thunk)> func;
		};

		static void Install()
		{
			stl::write_vfunc<0x6, BSSkyShader_SetupMaterial>(RE::VTABLE_BSSkyShader[0]);
			logger::info("[Cloud Shadows] Installed hooks");
		}
	};
};
