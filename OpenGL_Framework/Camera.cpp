#include "Camera.h"
#include "Framebuffer.h"
#include "IO.h"

Camera::Camera() : Camera(ProjectionType::Perspective)
{

}

Camera::Camera(ProjectionType projType)
{
	init();
	if (projType == ProjectionType::Perspective)
		perspective(60.0f, 1.0f, 0.1f, 100.0f);
	else
		orthographic(-10, 10, -10, 10, -100, 100);
}

Camera::~Camera()
{

}

void Camera::perspective(float fovy, float aspect, float zNear, float zFar)
{
	projectionType = ProjectionType::Perspective;
	m_pProjection = mat4::perspective(
		degrees(fovy), aspect,
		zNear, zFar);

	m_pAspectRatio = aspect;
	m_pNear = zNear;
	m_pFar = zFar;
	m_pFov.y = fovy;
	m_pFov.x = radians(2 * atan(tan(degrees(m_pFov.y).radians() * 0.5f) * aspect)).degrees();
}

void Camera::orthographic(float left, float right, float bottom, float top, float zNear, float zFar)
{
	projectionType = ProjectionType::Orthographic;
	m_pProjection = mat4::ortho(
		left, right,
		bottom, top,
		zNear, zFar);

	m_pOrthoSize = vec4(left, right, top, bottom);
	m_pAspectRatio = (right - left) / (top - bottom);
	m_pNear = zNear;
	m_pFar = zFar;
}

mat4 Camera::getView() const
{
	return m_pView;
}

mat4 Camera::getViewProjection() const
{
	return m_pProjection * getView();
}

mat4 Camera::getProjection() const
{
	return m_pProjection;
}

void Camera::update(float dt)
{
	Transform::update(dt);

	// Calculate view matrix
	m_pView = inverse(m_pLocalToWorld);
	m_pUBO.sendMatrix(getProjection(), 0);
	m_pUBO.sendMatrix(getView(), sizeof(mat4));
}

void Camera::render()
{
	glClearColor(0.05f, 0.05f, 0.05f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glViewport(0, 0, Window::getWidth(), Window::getHeight());

	m_pUBO.bind(1);
}

void Camera::clear()
{
	GLbitfield clearFlag = 0;
	switch (m_pClearFlag)
	{
	case ClearFlag::Skybox:
		SAT_ERROR_LOC("Skybox not setup yet");
		break;
	case ClearFlag::SolidColor:
		clearFlag |= GL_COLOR_BUFFER_BIT;
		break;
	case ClearFlag::DepthOnly:
		clearFlag |= GL_DEPTH_BUFFER_BIT;
		break;
	case ClearFlag::NoClear:
		break;
	}
}

void Camera::setupViewport()
{
	m_pViewportRectNormalized.pos = clamp(m_pViewportRect.pos, 0.0f, 1.0f); //
	m_pViewportRectNormalized.size = clamp(m_pViewportRect.size, m_pViewportRect.pos, m_pViewportRect.getMax() - m_pViewportRect.pos);
}

void Camera::bind()
{
	if (m_pUBO.isInit())
	{
		m_pUBO.bind(1);
	}
}

void Camera::init()
{
	//m_pViewportRect = vec4
	if (!m_pUBO.isInit())
	{
		m_pUBO.allocateMemory(sizeof(mat4) * 2);
		m_pUBO.bind(1);
	}
}
