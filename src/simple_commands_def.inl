
NAN_METHOD(compileShader) {
    auto _context = info.GetIsolate()->GetCurrentContext();
    glCompileShader(
        info[0]->ToInt32(_context).ToLocalChecked()->Value() /* shader */);
}


NAN_METHOD(deleteShader) {
    auto _context = info.GetIsolate()->GetCurrentContext();
    glDeleteShader(
        info[0]->ToInt32(_context).ToLocalChecked()->Value() /* shader */);
}


NAN_METHOD(attachShader) {
    auto _context = info.GetIsolate()->GetCurrentContext();
    glAttachShader(
        info[0]->ToInt32(_context).ToLocalChecked()->Value() /* program */,
        info[1]->ToInt32(_context).ToLocalChecked()->Value() /* shader */);
}


NAN_METHOD(linkProgram) {
    auto _context = info.GetIsolate()->GetCurrentContext();
    glLinkProgram(
        info[0]->ToInt32(_context).ToLocalChecked()->Value() /* program */);
}


NAN_METHOD(deleteProgram) {
    auto _context = info.GetIsolate()->GetCurrentContext();
    glDeleteProgram(
        info[0]->ToInt32(_context).ToLocalChecked()->Value() /* program */);
}


NAN_METHOD(useProgram) {
    auto _context = info.GetIsolate()->GetCurrentContext();
    glUseProgram(
        info[0]->ToInt32(_context).ToLocalChecked()->Value() /* program */);
}


NAN_METHOD(viewport) {
    auto _context = info.GetIsolate()->GetCurrentContext();
    glViewport(
        info[0]->ToInt32(_context).ToLocalChecked()->Value() /* x */,
        info[1]->ToInt32(_context).ToLocalChecked()->Value() /* y */,
        info[2]->ToInt32(_context).ToLocalChecked()->Value() /* width */,
        info[3]->ToInt32(_context).ToLocalChecked()->Value() /* height */);
}


NAN_METHOD(cullFace) {
    auto _context = info.GetIsolate()->GetCurrentContext();
    glCullFace(
        info[0]->ToInt32(_context).ToLocalChecked()->Value() /* mode */);
}


NAN_METHOD(enable) {
    auto _context = info.GetIsolate()->GetCurrentContext();
    glEnable(
        info[0]->ToInt32(_context).ToLocalChecked()->Value() /* cap */);
}


NAN_METHOD(disable) {
    auto _context = info.GetIsolate()->GetCurrentContext();
    glDisable(
        info[0]->ToInt32(_context).ToLocalChecked()->Value() /* cap */);
}


NAN_METHOD(bindBuffer) {
    auto _context = info.GetIsolate()->GetCurrentContext();
    glBindBuffer(
        info[0]->ToInt32(_context).ToLocalChecked()->Value() /* target */,
        info[1]->ToInt32(_context).ToLocalChecked()->Value() /* buffer */);
}


NAN_METHOD(enableVertexAttribArray) {
    auto _context = info.GetIsolate()->GetCurrentContext();
    glEnableVertexAttribArray(
        info[0]->ToInt32(_context).ToLocalChecked()->Value() /* index */);
}


NAN_METHOD(drawArrays) {
    auto _context = info.GetIsolate()->GetCurrentContext();
    glDrawArrays(
        info[0]->ToInt32(_context).ToLocalChecked()->Value() /* mode */,
        info[1]->ToInt32(_context).ToLocalChecked()->Value() /* first */,
        info[2]->ToInt32(_context).ToLocalChecked()->Value() /* count */);
}
