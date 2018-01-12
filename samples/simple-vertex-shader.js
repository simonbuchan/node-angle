const gl = require('../');
const common = require('./lib/common');
const matrix = require('./lib/matrix');
const geom = require('./lib/geom');

const vs = `
  uniform mat4 u_mvpMatrix;
  attribute vec4 a_position;
  attribute vec2 a_texcoord;
  varying vec2 v_texcoord;
  void main()
  {
      gl_Position = u_mvpMatrix * a_position;
      v_texcoord = a_texcoord;
  }
`;

const fs = `
  precision mediump float;
  varying vec2 v_texcoord;
  void main()
  {
      gl_FragColor = vec4(v_texcoord.x, v_texcoord.y, 1.0, 1.0);
  }
`;

function main() {
  const win = new gl.Window({ width: 1280, height: 720, title: 'Simple Vertex Shader - Sample App' });

  const program = common.compileProgram(vs, fs);

  const positionLocation = gl.getAttribLocation(program, 'a_position');
  const texcoordLocation = gl.getAttribLocation(program, 'a_texcoord');

  const mvpMatrixLocation = gl.getUniformLocation(program, 'u_mvpMatrix');

  const cube = geom.cube(0.5);

  const positionBuffer = gl.createBuffer();
  const texcoordBuffer = gl.createBuffer();
  const indicesBuffer = gl.createBuffer();

  gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(cube.position), gl.STATIC_DRAW);
  gl.bindBuffer(gl.ARRAY_BUFFER, texcoordBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(cube.texcoords), gl.STATIC_DRAW);
  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indicesBuffer);
  gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(cube.indices), gl.STATIC_DRAW);

  let rotation = 45;

  gl.clearColor(0, 0, 0, 0);
  gl.cullFace(gl.BACK);
  gl.enable(gl.CULL_FACE);

  win.on('preswap', () => {
    step();
    draw();
  });

  function step() {
    rotation = (Date.now() * 0.04) % 360;

    const perspective = matrix.perspective(60, win.width / win.height, 1, 20);
    const model = matrix.mul(
      matrix.translate([0, 0, -2]),
      matrix.rotate(rotation, [1, 0, 1]));
    const view = matrix.identity();
    const mvp = matrix.mul(perspective, view, model);
    gl.uniformMatrix(mvpMatrixLocation, 1, false, mvp);
  }

  function draw() {
    gl.viewport(0, 0, win.width, win.height);
    gl.clear(gl.COLOR_BUFFER_BIT);
    gl.useProgram(program);
    gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
    gl.vertexAttribPointer(positionLocation, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(positionLocation);
    gl.bindBuffer(gl.ARRAY_BUFFER, texcoordBuffer);
    gl.vertexAttribPointer(texcoordLocation, 2, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(texcoordLocation);
    gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indicesBuffer);
    gl.drawElements(gl.TRIANGLES, cube.indices.length, gl.UNSIGNED_SHORT, 0);
  }
}

common.start(main);
