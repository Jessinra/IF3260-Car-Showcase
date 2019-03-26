#!/usr/bin/env python3
import struct
from collections import namedtuple
from functools import reduce


RawMesh = namedtuple('RawMesh', 'width height mode vertices indices')
Mesh = namedtuple('Mesh', 'coord_elems col_elems mode vertices indices')
ColouredVertex = namedtuple('ColouredVertex', 'coords colour')


def tokenize(f):
    for line in f:
        comment_pos = line.find('//')
        if comment_pos >= 0:
            line = line[:comment_pos]
        yield from line.split()
        yield '\n'


def next_or(it, default=None):
    try:
        return next(it)
    except StopIteration:
        return default


def process_tokens(tokens):
    def token_match(t, match):
        return t.lower() == match
    def skip_to(t):
        while not token_match(next(tokens), t): pass
    colour = (0, 0, 0)
    width = int(next(tokens))
    height = int(next(tokens))
    vertices = []
    indices = []
    skip_to('vertices')
    # Process vertices
    vertex = []
    token = next(tokens)
    while not token_match(token, 'triangles'):
        if token.lower() in ('color', 'colour'):
            # set colour
            r = int(next(tokens))
            g = int(next(tokens))
            b = int(next(tokens))
            colour = (r, g, b)
            skip_to('\n')
        elif token_match(token, '\n'):
            # end of vertex?
            if vertex:
                #print('Vertex', len(vertices), vertex, colour)
                # make sure to copy before clearing the original
                vertices.append(ColouredVertex(tuple(vertex), colour))
                vertex.clear()
        else:
            # coordinate
            v = float(token)
            vertex.append(v)
        token = next(tokens)
    # Process triangles
    token = next_or(tokens)
    while token is not None:
        if not token_match(token, '\n'):
            v = int(token) - 1 # 0-based indices please
            assert v >= 0 and v < len(vertices)
            indices.append(v)
        token = next_or(tokens)
    return RawMesh(width, height, 'triangles', vertices, indices)


def merge_rawmesh(a, b):
    assert a.mode == b.mode
    mode = a.mode
    width = max(a.width, b.width)
    height = max(a.height, b.height)
    vertices = a.vertices + b.vertices
    offset_b = len(a.vertices)
    indices = a.indices + [v + offset_b for v in b.indices]
    return RawMesh(width, height, mode, vertices, indices)


def pad_list(l, n, x=0.0):
    if not isinstance(l, list):
        l = list(l)
    return l + [x] * (n - len(l)) if len(l) < n else l


def cook_mesh(raw_mesh):
    max_dim = max(raw_mesh.width, raw_mesh.height)
    def map_coord(v):
        v = (v / (max_dim / 2.0)) - 1.0
        return v
    coord_elems = max(len(vertex.coords) for vertex in raw_mesh.vertices)
    col_elems = max(len(vertex.colour) for vertex in raw_mesh.vertices)
    vertices = []
    for vertex in raw_mesh.vertices:
        vertices.append(
            pad_list(map(map_coord, vertex.coords), coord_elems) +
            pad_list(map(lambda v: float(v)/255, vertex.colour), col_elems))
    mode = raw_mesh.mode
    indices = raw_mesh.indices
    return Mesh(coord_elems, col_elems, mode, vertices, indices)


def write_mesh(f, mesh):
    header = struct.pack('@IIII', mesh.coord_elems, mesh.col_elems,
                         len(mesh.vertices), len(mesh.indices))
    f.write(header)
    vertex_format = '@{}f'.format(mesh.coord_elems + mesh.col_elems)
    for vertex in mesh.vertices:
        f.write(struct.pack(vertex_format, *vertex))
    for index in mesh.indices:
        f.write(struct.pack('@I', index))


def read_rawmesh(f):
    return process_tokens(tokenize(f))


def main(outfile, infiles):
    raw_meshes = []
    for inf in infiles:
        with open(inf, 'rt') as f:
            raw_meshes.append(read_rawmesh(f))
    raw_mesh = reduce(merge_rawmesh, raw_meshes)
    mesh = cook_mesh(raw_mesh)
    with open(outfile, 'wb') as f:
        write_mesh(f, mesh)


if __name__ == '__main__':
    import sys
    if (len(sys.argv) < 3):
        print("Usage: {} outfile infiles...".format(sys.argv[0]))
        sys.exit(1)
    main(sys.argv[1], sys.argv[2:])
