# Generate consts.js from the API definition.
# Steals stuff from angle/scripts/generate_entry_points.py

from __future__ import print_function
import sys, os, pprint
import xml.etree.ElementTree as etree

script_path = os.path.abspath(sys.argv[0])
script_dir = os.path.dirname(script_path)
repo_dir = os.path.dirname(script_dir)

def repo_relative(path):
    return os.path.join(repo_dir, path)

output_path = repo_relative('consts.js')

tree = etree.parse(repo_relative('angle/scripts/gl.xml'))
root = tree.getroot()

lines = [
    "const gl = require('./native')"
]

# Just dumping all <enums> out for now.
# In the future (should be pre 1.0), restrict to a useful whitelist.
for group in root.findall('.//enums'):
    enums = group.findall('.//enum')
    if not enums:
        continue

    lines.append('')

    if group.attrib['namespace'] == 'GL':
        if 'group' in group.attrib:
            lines.append('// {}'.format(group.attrib['group']))
    else:
        if 'group' in group.attrib:
            lines.append('// {}::{}'.format(group.attrib['namespace'], group.attrib['group']))
        else:
            lines.append('// {}'.format(group.attrib['namespace']))

    if 'comment' in group.attrib:
        lines.append('// {}'.format(group.attrib['comment']))

    for enum in enums:
        name = enum.attrib['name']
        if not name.startswith('GL_'):
            raise Exception('Weird enum name: {}'.format(name))
        name = name[3:]
        if name[0].isdigit():
            name = '_' + name
        lines.append('gl.{name} = {value};'.format(name=name, value=enum.attrib['value']))

with open(output_path, "w") as out:
    out.write('\n'.join(lines))
