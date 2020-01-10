#ifndef __FRAMEBUFFERMANAGER_H__
#define __FRAMEBUFFERMANAGER_H__

#include <imgui/imgui.h>
#include <vector>

#include "GreyscaleEffect.h"
#include "SepiaEffect.h"
#include "VignetteEffect.h"
#include "GrainEffect.h"
#include "PixelateEffect.h"
#include "SelectiveColorEffect.h"
#include "BloomEffect.h"

#include "BackEnd.h"
#include "Shader.h"

enum EffectType
{
	Sepia,
	Greyscale,
	Vignette,
	Grain,
	Pixelate,
	SelectiveColor,
	Bloom,

	NUM_EFFECTS
};

//Framebuffer Manager
class EffectManager abstract
{
public:
	//Deconstructor for Framebuffer manager
	//*Runs at program termination
	~EffectManager();

	//Imgui editor
	static void CreateTab();
	static void CreateEditor();

	//Initialize effect manager
	static void InitEffectManager(unsigned width, unsigned height);

	//Creates and adds x type of framebuffer to the list of buffers to draw
	//each type of framebuffer has a tag (look above)
	//returns the index of the buffer
	static int CreateEffect(EffectType type, unsigned width, unsigned height);
	//Gets Framebuffer using the index given when it was created
	static PostEffect* GetEffect(int index);
	//Adjusts the indices of the effects that are still in there
	static void AdjustIndices(int indexToRemove);
	//Removes the effect using the index given for that effect
	static void RemoveEffect(int index);

	//Clear all the buffers
	static void ClearBuffers();
	//Draw all the effects (and subsequentially apply each effect)
	static void DrawEffects();

	//Reshape the framebuffers so they still work
	static void ReshapeBuffers(unsigned width, unsigned height);

	//Bind color and depth textures for the basic effect
	static void BindColorTex(int textureSlot);
	static void BindDepthTex(int textureSlot);
	//Unbind any texture for the basic buffer at textureSlot
	static void UnbindTexture(int textureSlot);

	//Draw the compound buffer
	static void DrawCompoundEffect();

	//Bind the basic effect stuffs
	static void BindBasicEffect();
	static void UnbindBasicEffect();

	//Getters
	//Buffer init
	static bool GetEffectInit();
	static int GetNumEffects();
	
	//Effect handles
	static int GetSepiaHandle();
	static int GetGreyscaleHandle();
	static int GetVignetteHandle();
	static int GetGrainHandle();
	static int GetPixelateHandle();
	static int GetSelectiveColorHandle();
	static int GetBloomHandle();
private:
	static Selectable m_selectable;

	//has there been one buffer initialized?
	static bool m_effectsInit;
	//Number of effects
	static int m_numEffects;

	//Types of effects
	static int m_sepia;
	static int m_greyscale;
	static int m_vignette;
	static int m_grain;
	static int m_pixelate;
	static int m_selectiveColor;
	static int m_bloom;

	//Basic "effect"
	//*This holds the original draw calls
	static PostEffect* m_basicEffect;
	
	//This effect is the last effect to be drawn
	//*It holds the "compound" of all the other effects
	static PostEffect* m_compoundEffect;

	//List of effects that have been created
	static std::vector<PostEffect*> m_effects;
};

#endif // !__FRAMEBUFFERMANGER_H__
