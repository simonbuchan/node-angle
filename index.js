const gl = require('bindings')('node_angle.node');
const { inherits } = require('util');
const { EventEmitter } = require('events');

inherits(gl.Window, EventEmitter);

module.exports = gl;
