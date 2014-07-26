#pragma once

#include <trutle/Trutle.h>

/* LightTexture
 * LightSource instances use a single, shared texture if 
 * they are not using a custom texture.
 * 
 * The texture is created as a white large texture (256px
 * radius) and is scaled & colored by the LightSource 
 * instances. 
 * 
 * Retain the texture after retrieval, release upon deletion
 * of object or disassociation.
 */
class LightTexture {
public:
	static RenderTexture* GetSharedTexture();
	static int GetRetainCount();
	
	static void RetainSharedTexture();
	static void ReleaseSharedTexture();
	
private:
	static const float c_lightRadius;
	
	static int _refs;
	static RenderTexture *_renderTexture;
	
	static void CreateSharedTexture();
	static void DeleteSharedTexture();
};


/* LightSource
 * Creates and manages a light texture which is
 * used to draw a light attached to a game object.
 * 
 * The light is defined by two properties; the color
 * and the radius of the light. The radius is in 
 * pixels. A custom texture can be used as well for
 * more advanced lightings (flashlight, etc).
 */
class LightSource : public Component {
public:
	struct Properties;

	// The LightSource is not initialized. 
	// Assign a custom texture or creating the light 
	// set the light properties to create the light.
	LightSource();
	
	~LightSource();
	
	// Sets the light distance and the color
	void SetLightProperties(float lightDistance, Color color);
	
	// Set a custom texture. 
	void SetTexture(Texture *texture);
	
	Properties GetLightProperties() const;
	float GetLightRadius() const;
	Color GetLightColor() const;
	Rect GetAABB() const;
	
	void RenderLight(Renderer *renderer);

	struct Properties {
		Properties();
		Properties(float, Color);
		bool IsInitialized();
		
		float radius;
		Color color;
		
	private:
		friend class LightSource;
		bool initialized;
	};
	
private:
	Texture *_texture;
	RenderTexture *_sharedTexture;
	Properties _properties;

	void UseSharedTexture();
	void ReleaseSharedTexture();
	bool IsUsingSharedTexture();
};
