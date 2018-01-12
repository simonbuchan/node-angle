// Based on angle/samples/hello-triangle and code from https://webglfundamentals.org
// See angle/LICENSE

const gl = require('../');
const common = require('./lib/common');

function main() {
  const win = new gl.Window({ width: 800, height: 800, title: 'Hello Triangle - Sample App' });

  const vs = `
  attribute vec4 vPosition;
  void main()
  {
      gl_Position = vPosition;
  }
  `;

  const fs = `
  precision mediump float;
  void main()
  {
      gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  }
  `;

  const program = common.compileProgram(vs, fs);

  const positionAttributeLocation = gl.getAttribLocation(program, "vPosition");
  const positionBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
  const vertices = [
     0.0,  0.5, 0.0,
    -0.5, -0.5, 0.0,
     0.5, -0.5, 0.0,
  ];
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);

  win.on('resized', resized);
  win.on('preswap', render);

  function resized() {
    console.log('resized', win.width, win.height);
    gl.viewport(0, 0, win.width, win.height);
  }

  function render() {
    // console.log('render', win.width, win.height);
    gl.clearColor(0, 0, 0, 0);
    gl.clear(gl.COLOR_BUFFER_BIT);

    gl.useProgram(program);
    gl.enableVertexAttribArray(positionAttributeLocation);

    // Bind the position buffer.
    gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);

    // Tell the attribute how to get data out of positionBuffer (ARRAY_BUFFER)
    {
      const size = 3;
      const type = gl.FLOAT;
      const normalize = false;
      const stride = 0;
      const offset = 0;
      gl.vertexAttribPointer(
        positionAttributeLocation,
        size,
        type,
        normalize,
        stride,
        offset);
    }

    {
      const primitiveType = gl.TRIANGLES;
      const offset = 0;
      const count = 3;
      gl.drawArrays(primitiveType, offset, count);
    }
  }
}

common.start(main);