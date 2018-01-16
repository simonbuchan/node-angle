const gl = require('./native');

const { inherits } = require('util');
const { EventEmitter } = require('events');

inherits(gl.Window, EventEmitter);

require('./consts');

module.exports = gl;
