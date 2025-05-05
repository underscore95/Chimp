#pragma once

#include "stdafx.h"
#include "api/utils/Maths.h"

namespace Chimp {

	class IListener {
	protected:
		IListener() = default;

	public:
		virtual ~IListener() = default;

	public:
		// Setters
		virtual void SetPosition(Vector3f position) = 0;
		virtual void SetVelocity(Vector3f velocity) = 0;
		virtual void SetOrientation(const Vector3f forward, const Vector3f up) = 0;

		// Getters
		Vector3f GetPosition() const {
			return m_Position;
		}
		Vector3f GetVelocity() const {
			return m_Velocity;
		}
		Vector3f GetForward() const {
			return m_Forward;
		}
		Vector3f GetUp() const {
			return m_Up;
		}

	protected:
		Vector3f m_Position;
		Vector3f m_Velocity;
		Vector3f m_Forward;
		Vector3f m_Up;
	};
}