//
/* All used GL-functions */



GL_FUNC(void,glBegin,(GLenum));
GL_FUNC(void,glBindTexture,(GLenum,GLuint));
GL_FUNC(void,glBlendFunc,(GLenum,GLenum));
GL_FUNC(void,glColor4f,(GLfloat,GLfloat,GLfloat,GLfloat));
GL_FUNC(void,glDisable,(GLenum cap));
GL_FUNC(void,glEnable,(GLenum cap));
GL_FUNC(void,glEnd,(void));
GL_FUNC(void,glGenTextures,(GLsizei n, GLuint *textures));
GL_FUNC(void,glDeleteTextures,(GLsizei n, GLuint *textures));
GL_FUNC(void,glGetIntegerv,(GLenum pname, GLint *params));
GL_FUNC(const GLubyte *,glGetString,(GLenum name));
GL_FUNC(void,glLoadIdentity,(void));
GL_FUNC(void,glMatrixMode,(GLenum mode));
GL_FUNC(void,glOrtho,(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar));
GL_FUNC(void,glTexCoord2f,(GLfloat s, GLfloat t));
GL_FUNC(void,glTexImage2D,(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels));
GL_FUNC(void,glTexParameteri,(GLenum target, GLenum pname, GLint param));
GL_FUNC(void,glTexSubImage2D,(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels));
GL_FUNC(void,glVertex2i,(GLint x, GLint y));
GL_FUNC(void,glVertex2f,(GLfloat x, GLfloat y));
GL_FUNC(void,glViewport,(GLint x, GLint y, GLsizei width, GLsizei height));
GL_FUNC(void,glColor4ub,(GLubyte r, GLubyte g, GLubyte b, GLubyte a));
GL_FUNC(GLboolean,glIsTexture,( GLuint texture ));
GL_FUNC(void,glTexEnvf,(GLenum, GLenum, GLfloat));
GL_FUNC(void,glRasterPos2i,(GLint x, GLint y));
GL_FUNC(void,glPushAttrib,(GLbitfield mask));
GL_FUNC(void,glPopAttrib,(void));
GL_FUNC(void,glPushMatrix,(void));
GL_FUNC(void,glPopMatrix,(void));
GL_FUNC(void,glPointSize,(GLfloat size));
GL_FUNC(void,glLineWidth,(GLfloat size));
GL_FUNC(void,glScissor,(GLint x, GLint y, GLsizei width, GLsizei height));
GL_FUNC(void,glDrawPixels,(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels));
GL_FUNC(GLenum,glGetError,(void));
GL_FUNC_OPTIONAL(void,glColorTableEXT, (GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid *));
GL_FUNC_OPTIONAL(void,glActiveTextureARB, (GLenum));
GL_FUNC_OPTIONAL(void,glMultiTexCoord2fARB, (GLenum,GLfloat,GLfloat));

