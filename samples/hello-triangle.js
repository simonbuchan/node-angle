// Based on angle/samples/hello-triangle and code from https://webglfundamentals.org
// See angle/LICENSE

const gl = require('../');

function main() {
  const width = 800;
  const height = 800;
  const win = new gl.Window({ width, height });
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

  const program = compileProgram(vs, fs);

  const positionAttributeLocation = gl.getAttribLocation(program, "vPosition");
  const positionBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
  const vertices = [
     0.0,  0.5, 0.0,
    -0.5, -0.5, 0.0,
     0.5, -0.5, 0.0,
  ];
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertices), gl.STATIC_DRAW);
  gl.viewport(0, 0, width, height);

  render();

  function render() {
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

    win.swapBuffers();
  }
}

function compileShader(type, source) {
  const shader = gl.createShader(type);
  gl.shaderSource(shader, source);
  gl.compileShader(shader);

  if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
    const infoLog = gl.getShaderInfoLog(shader);
    gl.deleteShader(shader);

    throw new Error(`GL Shader compile failed: ${infoLog}`);
  }

  return shader;
}

function linkProgram(...shaders) {
  const program = gl.createProgram();

  for (const shader of shaders) {
    gl.attachShader(program, shader);
  }
  gl.linkProgram(program);
  if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
    const infoLog = gl.getProgramInfoLog(program);
    gl.deleteProgram(program);

    throw new Error(`GL program link failed: ${infoLog}`);
  }

  return program;
}

function compileProgram(vsSource, fsSource) {
  const vs = compileShader(gl.VERTEX_SHADER, vsSource);
  try {
    const fs = compileShader(gl.FRAGMENT_SHADER, fsSource);
    try {
      return linkProgram(vs, fs);
    } catch (e) {
      gl.deleteShader(fs);
      throw e;
    }
  } catch (e) {
    gl.deleteShader(vs);
    throw e;
  }
}

try {
  main();
} catch (e) {
  console.error(e);
  process.stdin.setRawMode(true);
  process.stdout.write('press any key to exit');
  process.stdin.on('data', () => {
    process.exit(1);
  });
}
