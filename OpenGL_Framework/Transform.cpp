#include "Transform.h"

Transform::Transform()
	: m_pLocalScale(1.0f), m_pParent(nullptr), uniqueID(-1)
{
}

Transform::~Transform()
{

}

void Transform::addChild(Transform * newChild)
{
	if (newChild)
	{
		m_pChildren.push_back(newChild);
		newChild->m_pParent = this; // tell new child that this game object is its parent
	}
}

void Transform::removeChild(Transform * rip)
{
	for (unsigned int i = 0; i < m_pChildren.size(); ++i)
	{
		if (m_pChildren[i] == rip) // compare memory locations (pointers)
		{
			//std::cout << "Removing child: " + rip->name << " from object: " << this->name;
			m_pChildren.erase(m_pChildren.begin() + i);
		}
	}
}

void Transform::setLocalPos(vec3 newPosition)
{
	m_pLocalPosition = newPosition;
}

void Transform::setLocalRotX(float newAngle)
{
	m_pLocalRotationEuler.x = (newAngle);
}

void Transform::setLocalRotY(float newAngle)
{
	m_pLocalRotationEuler.y = (newAngle);
}

void Transform::setLocalRotZ(float newAngle)
{
	m_pLocalRotationEuler.z = (newAngle);
}

void Transform::setLocalRot(vec3 newAngle)
{
	m_pLocalRotationEuler = newAngle;
}

void Transform::setScale(vec3 newScale)
{
	m_pLocalScale = newScale;
}

#include "IO.h"
void Transform::setWorldPos(vec3 newPosiiton)
{
	m_pLocalPosition = vec3(m_pLocalToWorldInverse * vec4(newPosiiton, 1.0f));
	SAT_DEBUG_LOG("%f\t%f\t%f\t", m_pLocalPosition.x, m_pLocalPosition.y, m_pLocalPosition.z);
}

void Transform::setWorldRot(quat newRotation)
{

}

mat4 Transform::getLocalToWorld()
{
	return m_pLocalToWorld;
}

mat4 Transform::getLocalToWorldInverse()
{
	return m_pLocalToWorldInverse;
}

vec3 Transform::getWorldPos()
{
	if (m_pParent)
		return vec3(m_pParent->getLocalToWorld() * vec4(m_pLocalPosition, 1.0f));
	else
		return m_pLocalPosition;
}

mat4 Transform::getWorldRot()
{
	if (m_pParent)
		return m_pParent->getWorldRot() * m_pLocalRotation;
	else
		return m_pLocalRotation;
}

vec3 Transform::getLocalScale()
{
	return m_pLocalScale;
}

vec3 Transform::getLocalPos()
{
	return m_pLocalPosition;
}

mat4 Transform::getLocalRot()
{
	return m_pLocalRotation;
}

vec3 Transform::getLocalRotEuler()
{
	return m_pLocalRotationEuler;
}

std::vector<Transform*> Transform::getChildren()
{
	return m_pChildren;
}

Transform * Transform::getParent()
{
	return m_pParent;
}

void Transform::update(float dt)
{
	// Create 4x4 transformation matrix

	// Create rotation matrix
	mat3 rx = rx.rotatex(degrees(m_pLocalRotationEuler.x));
	mat3 ry = ry.rotatey(degrees(m_pLocalRotationEuler.y));
	mat3 rz = rz.rotatez(degrees(m_pLocalRotationEuler.z));
	
	// Note: pay attention to rotation order, ZYX is not the same as XYZ
	m_pLocalRotation = rz * ry * rx;

	// Create translation matrix
	mat4 tran = tran.translate(m_pLocalPosition);

	// Create scale matrix
	mat4 scale = scale.scale(m_pLocalScale);

	// Combine all above transforms into a single matrix
	m_pLocalTransform = tran * m_pLocalRotation * scale;

	if (m_pParent)
	{
		m_pLocalToWorld = m_pParent->getLocalToWorld() * m_pLocalTransform;
		m_pLocalToWorldInverse = m_pParent->getLocalToWorldInverse() * fastinverse(m_pLocalTransform);
	}
	else
	{
		m_pLocalToWorld = m_pLocalTransform;
		m_pLocalToWorldInverse = fastinverse(m_pLocalTransform);
	}

	// Update children
	for (unsigned int i = 0; i < m_pChildren.size(); i++)
		m_pChildren[i]->update(dt);
}

void Transform::draw()
{
}
