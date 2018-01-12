const gl = require('../../');

exports.start = (main) => {
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
};

exports.compileShader = (type, source) => {
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

exports.linkProgram = (...shaders) => {
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

exports.compileProgram = (vsSource, fsSource) => {
  const vs = exports.compileShader(gl.VERTEX_SHADER, vsSource);
  try {
    const fs = exports.compileShader(gl.FRAGMENT_SHADER, fsSource);
    try {
      return exports.linkProgram(vs, fs);
    } catch (e) {
      gl.deleteShader(fs);
      throw e;
    }
  } catch (e) {
    gl.deleteShader(vs);
    throw e;
  }
}
