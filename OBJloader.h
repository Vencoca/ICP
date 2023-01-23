#pragma once

#ifndef OBJloader_H
#define OBJloader_H

#include <vector>
#include <glm/fwd.hpp>
#include <string>
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include "mesh.h"

bool loadOBJ(
	const char* path,
	std::vector < glm::vec3 >& out_vertices,
	std::vector < glm::vec2 >& out_uvs,
	std::vector < glm::vec3 >& out_normals
);

void createMesh(const char* path, GLuint& shader_mesh, mesh& mesh_out, glm::vec3 color);

#endif