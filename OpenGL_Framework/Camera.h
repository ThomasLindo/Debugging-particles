#pragma once
#include "Transform.h"
#include "UniformBuffer.h"
#include <gmtk/gmtk.h>
#include <SatMath/Rect.h>

/*
  ////////////
 // Camera // 2019 Stephen Thompson
////////////

The Camera class uses the transform class to position the View Matrix, 
in the future it will also track which objects should be drawn, what order they're drawn in, and 
whether they should be culled or not

*/

enum ProjectionType
{
	Perspective = 0,
	Orthographic
};

enum class ClearFlag
{
	Skybox,
	SolidColor,
	DepthOnly,
	NoClear
};

class Camera : public Transform
{
public:	
	Camera();
	Camera(ProjectionType projType);
	~Camera();

	void perspective(
		float fovy, float aspect, 
		float zNear, float zFar);

	void orthographic(
		float left, float right,
		float bottom, float top,
		float zNear, float zFar);

	mat4 getView() const;
	mat4 getViewProjection() const;
	mat4 getProjection() const;

	void update(float dt);

	void render();
	void clear();
	void setupViewport();
	void bind();

private:
	void init();
	mat4 m_pProjection;
	mat4 m_pView;
	
	rect m_pViewportRect;
	rect m_pViewportRectNormalized; 

	vec4 m_pOrthoSize;
	vec2 m_pFov;
	float m_pAspectRatio;
	float m_pNear;
	float m_pFar;
	ProjectionType projectionType = ProjectionType::Perspective;
	ClearFlag m_pClearFlag = ClearFlag::SolidColor;

	UniformBuffer m_pUBO;
};