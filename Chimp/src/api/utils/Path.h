#pragma once

#include "stdafx.h"
#include "api/utils/Maths.h"

namespace Chimp {
	template <typename T, typename Enable = void>
	class Path;

	// Represents a path of n (n >= 1) points in dD (1 <= d <= 4) space
	// The path is interpolated linearly between points
	template <typename T>
	class Path<T, typename std::enable_if<
		std::is_same<T, float>::value ||
		std::is_same<T, Chimp::Vector2f>::value ||
		std::is_same<T, Chimp::Vector3f>::value ||
		std::is_same<T, Chimp::Vector4f>::value
	>::type> {
	public:
		Path(std::vector<T> points, T offsetPoints = {})  {
			assert(!points.empty());
			float distance = 0.0f;
			for (size_t i = 0; i < points.size(); ++i) {
				if (i > 0) {
					distance += GetDistanceBetween(points[i - 1], points[i]);
				}
				m_Points.push_back(std::make_tuple(distance, points[i] + offsetPoints));
			}
		}

		static Path<Chimp::Vector2f> Deserialise(const YAMLBlock& block, Chimp::Vector2f offsetPoints = {}) {
			size_t numPoints = block.Blocks.size();
			std::vector<Chimp::Vector2f> points;
			for (size_t i = 0; i < numPoints; ++i) {
				auto iter = block.Blocks.find(std::to_string(i));
				if (iter == block.Blocks.end()) {
					assert(false);
					return Path<Chimp::Vector2f>({});
				}

				auto& block = iter->second;
				points.push_back(Chimp::Vector2f{
					block.Floats.at("x"),
					block.Floats.at("y")
					});
			}
			return Path<Chimp::Vector2f>(points, offsetPoints);
		}

		// Get point by index
		// Make sure index is valid
		T GetPointByIndex(size_t index) const {
			assert(index < m_Points.size());
			assert(index >= 0);
			return std::get<1>(m_Points[index]);
		}

		// Get start point
		T GetStart() const {
			return GetPointByIndex(0);
		}

		// Get end point
		T GetEnd() const {
			return GetPointByIndex(m_Points.size() - 1);
		}

		// Get total distance of path
		float GetTotalDistance() const {
			return std::get<0>(m_Points.back());
		}

		// Get point t along the path, where t is a value from 0 to 1
		// clamps if t is invalid
		// Returns point, if point is before or at first point, if point is after last point
		std::tuple<T, bool, bool> GetPointByPercent(float t) const {
			return GetPointAtDistance(t * std::get<0>(m_Points.back()));
		}

		// Get point at a specific distance along the path
		// clamps if distance is invalid
		// Returns point, if point is before or at first point, if point is after last point
		std::tuple<T, bool, bool> GetPointByDistance(float distance) const {
			for (size_t i = 0; i < m_Points.size(); ++i) {
				if (std::get<0>(m_Points[i]) >= distance) {
					if (i == 0) {
						return { std::get<1>(m_Points[0]), true, false };
					}

					float lastPointDistance = std::get<0>(m_Points[i - 1]);
					T lastPoint = std::get<1>(m_Points[i - 1]);
					float nextPointDistance = std::get<0>(m_Points[i]);
					T nextPoint = std::get<1>(m_Points[i]);

					// Lerp between the two points
					// Inverse lerp to determine how far between the two points we are
					float t = (distance - lastPointDistance) / (nextPointDistance - lastPointDistance);
					return { Lerp(lastPoint, nextPoint, t), false, false };
				}
			}
			return { std::get<1>(m_Points.back()), false, true };
		}
	private:
		std::vector<std::tuple<float, T>> m_Points; // (summed length of all previous points), (point)
	};
}