#include "Mesh.hpp"

Mesh::Mesh(unsigned dims, unsigned colElems, unsigned nVertices,
           unsigned nIndices)
  : dims(dims), colElems(colElems) {
  vsize = nVertices;
  esize = nIndices;
  vbuf = new float[(dims + colElems) * vsize];
  ebuf = new unsigned[esize];
}

void Mesh::bind() {
  unsigned stride = (dims + colElems) * sizeof(float);
  glBindVertexArray(vao);
  // Bind vertices
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 6 * vsize * sizeof(float), vbuf,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, dims, GL_FLOAT, GL_FALSE, stride, (void*)0);
  // Bind colors
  glVertexAttribPointer(1, colElems, GL_FLOAT, GL_FALSE, stride,
                        (void*)(dims * sizeof(float)));
  // Enable vertex attrib
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  // Bind indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, esize * sizeof(unsigned), ebuf,
               GL_STATIC_DRAW);
}
