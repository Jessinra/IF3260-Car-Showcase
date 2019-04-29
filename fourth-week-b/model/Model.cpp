#include "Model.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>

Model::Model() = default;

Model::Model(const string &filename) {
    readFromFile(filename);
}

void Model::clearModel() {
    points.clear();
    colors.clear();
}

int Model::size() {
    return (int) points.size() / 4;
}

vector<vec3> Model::getPoints() {
    return points;
}

vector<vec3> & Model::getRefPoints() {
    return points;
}

const vector<vec3> & Model::getConstRefPoints() const {
    return points;
}

vector<vec4> Model::getColors() {
    return colors;
}

vector<vec4> & Model::getRefColors() {
    return colors;
}

const vector<vec4> & Model::getConstRefColors() const {
    return colors;
}

void Model::readFromFile(const string &filename) {
    
    clearModel();
    printf("Loading OBJ file %s...\n", filename.data());

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    FILE *file = fopen(filename.data(), "r");
    if (file == NULL) {
        cerr<<"Can't open the file : "<<filename<<endl;
        exit(1);
    }

    while (1) {

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader

        if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);

        } else if (strcmp(lineHeader, "vt") == 0) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
            temp_uvs.push_back(uv);

        } else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);

        } else if (strcmp(lineHeader, "f") == 0) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2],
                                 &normalIndex[2]);
            if (matches != 9) {
                printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                fclose(file);
                exit(1);
            }

            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);

        } else {
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }

    for (unsigned int i=0; i<vertexIndices.size(); i++) {

        // Get the indices of its attributes
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];

        // Get the attributes thanks to the index
        glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
        glm::vec2 uv = temp_uvs[ uvIndex-1 ];
        glm::vec3 normal = temp_normals[ normalIndex-1 ];

        // Put the attributes in buffers
        points.push_back(vertex);
        textures.push_back(uv);
        normals.push_back(normal);
    }

    fclose(file);
}

vector<vec2> Model::getTextures() {
    return textures;
}

vector<vec2> &Model::getRefTextures() {
    return textures;
}

const vector<vec2> &Model::getConstRefTextures() const {
    return textures;
}

const vector<vec3> &Model::getConstRefNormals() const {
    return normals;
}
