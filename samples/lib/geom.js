exports.cube = (radius) => {
  return {
    position: [
      // -Y
      -radius, -radius, -radius,  // 0
      -radius, -radius, +radius,  // 1
      +radius, -radius, +radius,  // 2
      +radius, -radius, -radius,  // 3
      // +Y
      -radius, +radius, -radius,  // 4
      -radius, +radius, +radius,  // 5
      +radius, +radius, +radius,  // 6
      +radius, +radius, -radius,  // 7
      // -Z
      -radius, -radius, -radius,  // 8
      -radius, +radius, -radius,  // 9
      +radius, +radius, -radius,  // 10
      +radius, -radius, -radius,  // 11
      // +Z
      -radius, -radius, +radius,  // 12
      -radius, +radius, +radius,  // 13
      +radius, +radius, +radius,  // 14
      +radius, -radius, +radius,  // 15
      // -X
      -radius, -radius, -radius,  // 16
      -radius, -radius, +radius,  // 17
      -radius, +radius, +radius,  // 18
      -radius, +radius, -radius,  // 19
      // +X
      +radius, -radius, -radius,  // 20
      +radius, -radius, +radius,  // 21
      +radius, +radius, +radius,  // 22
      +radius, +radius, -radius,  // 23
    ],
    normals: [
      // -Y
      +0, -1, +0,
      +0, -1, +0,
      +0, -1, +0,
      +0, -1, +0,
      // +Y
      +0, +1, +0,
      +0, +1, +0,
      +0, +1, +0,
      +0, +1, +0,
      // -Z
      +0, +0, -1,
      +0, +0, -1,
      +0, +0, -1,
      +0, +0, -1,
      // +Z
      +0, +0, +1,
      +0, +0, +1,
      +0, +0, +1,
      +0, +0, +1,
      // -X
      -1, +0, +0,
      -1, +0, +0,
      -1, +0, +0,
      -1, +0, +0,
      // +X
      +1, +0, +0,
      +1, +0, +0,
      +1, +0, +0,
      +1, +0, +0,
    ],
    texcoords: [
      // -Y
      0, 0,
      0, 1,
      1, 1,
      1, 0,
      // +Y
      1, 0,
      1, 1,
      0, 1,
      0, 0,
      // -Z
      0, 0,
      0, 1,
      1, 1,
      1, 0,
      // +Z
      0, 0,
      0, 1,
      1, 1,
      1, 0,
      // -X
      0, 0,
      0, 1,
      1, 1,
      1, 0,
      // +X
      0, 0,
      0, 1,
      1, 1,
      1, 0,
    ],
    indices: [
      // -Y
      0, 2, 1,
      0, 3, 2,
      // +Y
      4, 5, 6,
      4, 6, 7,
      // -Z
      8, 9, 10,
      8, 10, 11,
      // +Z
      12, 15, 14,
      12, 14, 13,
      // -X
      16, 17, 18,
      16, 18, 19,
      // +X
      20, 23, 22,
      20, 22, 21,
    ],
  };
}
