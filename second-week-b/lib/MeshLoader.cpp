#include <memory>
#include <fstream>
#include "MeshLoader.hpp"

struct MeshHeader {
  unsigned dims, colElems;
  unsigned nVertices, nIndices;
} __attribute__((packed));

Mesh *MeshLoader::parse(std::istream &input) {
  MeshHeader header;
  if (!input.read(reinterpret_cast<char *>(&header), sizeof(header)))
    throw "Cannot read mesh header";
  std::unique_ptr<Mesh> m(new Mesh(header.dims, header.colElems,
                                   header.nVertices, header.nIndices));
  size_t vertexSize = (header.dims + header.colElems) * sizeof(float);
  if (!input.read(reinterpret_cast<char *>(m->vbuf),
                  m->vsize * vertexSize))
    throw "Cannot read vertices";
  if (!input.read(reinterpret_cast<char *>(m->ebuf),
                  m->esize * sizeof(unsigned)))
    throw "Cannot read indices";
  return m.release();
}

Mesh *MeshLoader::loadFile(const char *path) {
  std::fstream file(path, std::fstream::in | std::fstream::binary);
  if (!file)
    throw "Cannot open mesh file";
  return parse(file);
}
