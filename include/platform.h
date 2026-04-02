#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <GLUT/glut.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#endif

#endif /* PLATFORM_H */
