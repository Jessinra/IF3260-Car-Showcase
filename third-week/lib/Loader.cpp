#include <cstdio>
#include <cstdlib>
#include <string>
#include "Loader.hpp"

// DEPRECATED
void Loader::readFile(const char* file, char** buffer) {
  *buffer = nullptr;
  size_t size = 0;

  FILE *fp = fopen(file, "r");
  if (fp == nullptr) {
    throw "Can't open file. Maybe the path is wrong?";
  }
  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  rewind(fp);
  *buffer = (char *) malloc((size + 1) * sizeof(**buffer));
  fread(*buffer, size, 1, fp);
  (*buffer)[size] = '\0';
  fclose(fp);
}

void Loader::fillFromObjFile(const char* path, Mesh& obj) {

	std::vector<glm::vec3> tempVs; 
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;

	FILE * file = fopen(path, "r");
	if (file == nullptr) {
    throw "Failed to open the file";
	}

	while (true) {
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; 
		
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			tempVs.push_back(vertex);
		} else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; 
			tempUVs.push_back(uv);
		} else if (strcmp( lineHeader, "vn" ) == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			tempNormals.push_back(normal);
		} else if (strcmp( lineHeader, "f" ) == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9) {
				throw "Error parsing";
			}
			obj.vertexIndices.push_back(vertexIndex[0]);
			obj.vertexIndices.push_back(vertexIndex[1]);
			obj.vertexIndices.push_back(vertexIndex[2]);
			obj.uvIndices.push_back(uvIndex[0]);
			obj.uvIndices.push_back(uvIndex[1]);
			obj.uvIndices.push_back(uvIndex[2]);
			obj.normalIndices.push_back(normalIndex[0]);
			obj.normalIndices.push_back(normalIndex[1]);
			obj.normalIndices.push_back(normalIndex[2]);
		} else {
			char buf[1000];
			fgets(buf, 1000, file);
		}
	}

	// For each vertex of each triangle
	for (uint32_t idx = 0; idx < obj.vertexIndices.size(); idx++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = obj.vertexIndices[idx];
		unsigned int uvIndex = obj.uvIndices[idx];
		unsigned int normalIndex = obj.normalIndices[idx];
		
		// Get the attributes thanks to the index
		glm::vec3 vertex = tempVs[vertexIndex - 1];
		glm::vec2 uv = tempUVs[uvIndex - 1];
		glm::vec3 normal = tempNormals[normalIndex - 1];
		
		// Put the attributes in buffers
		obj.vertices.push_back(vertex);
		obj.uvs.push_back(uv);
		obj.normals.push_back(normal);
	}
	fclose(file);
}