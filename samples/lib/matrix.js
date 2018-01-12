// Written with reference to angle/util/Matrix.cpp

exports.identity = () => {
  return new Float32Array([
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
  ]);
};

const len = ([x, y, z]) => {
  return Math.sqrt(x * x + y * y + z * z);
};

const normalize = (p) => {
  const l = len(p);
  return [p[0] / l, p[1] / l, p[2] / l];
};

exports.rotate = (angle, p) => {
  const [x, y, z] = normalize(p);
  const theta = angle * (Math.PI / 180);
  const cos = Math.cos(theta);
  const sin = Math.sin(theta);
  const icos = 1 - cos;
  return new Float32Array([
    cos          + x * x * icos,  x * y * icos - z * sin,      x * z * icos + y * sin,      0,
    y * x * icos + z * sin,       cos          + y * y * icos, y * z * icos - x * sin,      0,
    z * x * icos - y * sin,       z * y * icos + x * sin,      cos          + z * z * icos, 0,
    0, 0, 0, 1,
  ]);
};

exports.translate = ([x, y, z]) => {
  return new Float32Array([
    1, 0, 0, x,
    0, 1, 0, y,
    0, 0, 1, z,
    0, 0, 0, 1,
  ]);
};

exports.scale = ([x, y, z]) => {
  return new Float32Array([
    x, 0, 0, 0,
    0, y, 0, 0,
    0, 0, z, 0,
    0, 0, 0, 1,
  ]);
};

exports.frustrum = (l, r, b, t, n, f) => {
  return new Float32Array([
    2 * n / (r - l), 0,                (r + l) / (r - l), 0,
    0,               2 * n / (t - b),  (t + b) / (t - b), 0,
    0,               0,               -(f + n) / (f - n), -2 * f * n / (f - n),
    0,               0,               -1,                 0,
  ]);
};

exports.perspective = (fovY, aspectRatio, nearZ, farZ) => {
  const h = Math.tan(fovY * Math.PI / 360) * nearZ;
  const w = h * aspectRatio;
  return exports.frustrum(-w, +w, -h, +h, nearZ, farZ);
};

exports.mul = (...inputs) => {
  const dst = new Float32Array(16);
  return inputs.reduceRight((a, b) => exports.mul2(a, b, dst));
};

exports.mul2 = (a, b, dst = new Float32Array(16)) => {
  const a00 = a[ 0], a01 = a[ 1], a02 = a[ 2], a03 = a[ 3];
  const a10 = a[ 4], a11 = a[ 5], a12 = a[ 6], a13 = a[ 7];
  const a20 = a[ 8], a21 = a[ 9], a22 = a[10], a23 = a[11];
  const a30 = a[12], a31 = a[13], a32 = a[14], a33 = a[15];
  const b00 = b[ 0], b01 = b[ 1], b02 = b[ 2], b03 = b[ 3];
  const b10 = b[ 4], b11 = b[ 5], b12 = b[ 6], b13 = b[ 7];
  const b20 = b[ 8], b21 = b[ 9], b22 = b[10], b23 = b[11];
  const b30 = b[12], b31 = b[13], b32 = b[14], b33 = b[15];

  dst[ 0] = a00 * b00 + a10 * b01 + a20 * b02 + a30 * b03;
  dst[ 1] = a00 * b10 + a10 * b11 + a20 * b12 + a30 * b13;
  dst[ 2] = a00 * b20 + a10 * b21 + a20 * b22 + a30 * b23;
  dst[ 3] = a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33;

  dst[ 4] = a01 * b00 + a11 * b01 + a21 * b02 + a31 * b03;
  dst[ 5] = a01 * b10 + a11 * b11 + a21 * b12 + a31 * b13;
  dst[ 6] = a01 * b20 + a11 * b21 + a21 * b22 + a31 * b23;
  dst[ 7] = a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33;

  dst[ 8] = a02 * b00 + a12 * b01 + a22 * b02 + a32 * b03;
  dst[ 9] = a02 * b10 + a12 * b11 + a22 * b12 + a32 * b13;
  dst[10] = a02 * b20 + a12 * b21 + a22 * b22 + a32 * b23;
  dst[11] = a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33;

  dst[12] = a03 * b00 + a13 * b01 + a23 * b02 + a33 * b03;
  dst[13] = a03 * b10 + a13 * b11 + a23 * b12 + a33 * b13;
  dst[14] = a03 * b20 + a13 * b21 + a23 * b22 + a33 * b23;
  dst[15] = a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33;

  return dst;
};
