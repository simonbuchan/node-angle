{
  'variables': {
    'conditions': [
      ['OS=="win" and target_arch=="ia32"', {
        'angle_out': [
          '../angle/gyp/Release_Win32',
        ],
      }],
      ['OS=="win" and target_arch=="x64"', {
        'angle_out': [
          '../angle/gyp/Release_x64',
        ],
      }],
    ],
  },

  'targets': [
    {
      'target_name': 'node_angle',
      'sources': [
        'src/node_angle.cc',
        'src/EGLWindowObject.cc',
      ],

      'include_dirs': [
        '<!(node -e "require(\'nan\')")',
        'angle/include',
        'angle/src',
        'angle/util',
      ],

      'libraries': [
        '<(angle_out)/lib/libEGL',
        '<(angle_out)/lib/libGLESv2',
        '<(angle_out)/lib/angle_util',
      ],

      'defines': [
        'GL_GLEXT_PROTOTYPES',
        'EGL_EGLEXT_PROTOTYPES',
      ],

      # Fails due to variables defined in angle/gyp/common_defines.gypi not being set,
      # despite everything I've tried, so hand-write the options above.
      # 'dependencies': [
      #   'angle/util/util.gyp:angle_util',
      # ],
    },
  ],
}