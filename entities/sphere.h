#ifndef SOFTWAREENGINEERINGPROJECT_SPHERE_H
#define SOFTWAREENGINEERINGPROJECT_SPHERE_H

#include "../shape.h"
#include "glad/glad.h"
#include <vector>
#include <glm/glm.hpp>
#include <cmath>

class Sphere : public Shape {
public:
    Sphere(glm::vec3 pos, float radius, int segments, float opacity) {
        position = pos;

        // Number of latitude and longitude segments
        int latitudeSegments = segments;
        int longitudeSegments = segments;

        // Vertex generation for sphere
        for (int lat = 0; lat <= latitudeSegments; ++lat) {
            float theta = lat * M_PI / latitudeSegments;  // Latitude angle
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            for (int lon = 0; lon <= longitudeSegments; ++lon) {
                float phi = lon * 2.0f * M_PI / longitudeSegments;  // Longitude angle
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);

                // Calculate the vertex position using spherical coordinates
                float x = radius * sinTheta * cosPhi;
                float y = radius * cosTheta;
                float z = radius * sinTheta * sinPhi;

                // Create a gradient effect based on the position of the vertex
                float red = (x + radius) / (2.0f * radius);   // Normalize x to [0, 1] range
                float green = (y + radius) / (2.0f * radius); // Normalize y to [0, 1] range
                float blue = 1.0f - (red + green) / 2.0f;     // Make sure the colors balance for a smooth gradient

                // Push the vertex position
                vertices.push_back(x + position.x);
                vertices.push_back(y + position.y);
                vertices.push_back(z + position.z);

                // Color
                vertices.push_back(red);
                vertices.push_back(green);
                vertices.push_back(blue);
                vertices.push_back(opacity);
            }
        }

        // Indices for sphere rendering (using triangle strips)
        for (int lat = 0; lat < latitudeSegments; ++lat) {
            for (int lon = 0; lon < longitudeSegments; ++lon) {
                int first = (lat * (longitudeSegments + 1)) + lon;
                int second = first + longitudeSegments + 1;

                // Two triangles for each segment
                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }

        initializeBuffers();
    }

    void draw() override {
        glBindVertexArray(getVAO());
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

#endif