# Generate src/simple_commands{_def,_init}.inl from the API definition and the whitelist.
# Steals stuff from angle/scripts/generate_entry_points.py

from __future__ import print_function
import sys, os, pprint
import xml.etree.ElementTree as etree

script_path = os.path.abspath(sys.argv[0])
script_dir = os.path.dirname(script_path)
repo_dir = os.path.dirname(script_dir)

def repo_relative(path):
    return os.path.join(repo_dir, path)

whitelist_path = repo_relative('scripts/simple_commands.list')
def_output_path = repo_relative('src/simple_commands_def.inl')
init_output_path = repo_relative('src/simple_commands_init.inl')

tree = etree.parse(repo_relative('angle/scripts/gl.xml'))
root = tree.getroot()

def_lines = []
init_lines = []

def_template = '''
NAN_METHOD({name}) {{
    auto _context = info.GetIsolate()->GetCurrentContext();
    {gl_name}({args});
}}
'''

def_arg_template = '''
        info[{index}]->To{type}(_context).ToLocalChecked()->Value() /* {name} */'''

def generate_command(name):
    gl_name = 'gl' + name[0].upper() + name[1:]
    command = root.find(".//command/proto[name='{}']/..".format(gl_name))

    args = [
        def_arg_template.format(index=index, type='Int32', name=param_name.text)
        for index, param_name in enumerate(command.findall('./param/name'))]

    init_lines.append('NAN_EXPORT(target, {});'.format(name))
    def_lines.append(def_template.format(
        name=name,
        gl_name=gl_name,
        args=','.join(args)))

with open(whitelist_path) as inp:
    for name in inp:
        name = name.strip()
        if name:
            try:
                generate_command(name)
            except:
                exc_class, exc, tb = sys.exc_info()
                msg = 'Failed generating {!r}: {}: {}'.format(name, exc_class.__name__, exc)
                raise Exception, msg, tb

with open(def_output_path, 'w') as out:
    out.write('\n'.join(def_lines))
with open(init_output_path, 'w') as out:
    out.write('\n'.join(init_lines))
