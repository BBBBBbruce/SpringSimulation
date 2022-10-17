#ifndef VR_CG_OPENGL_H
#define VR_CG_OPENGL_H

#include <string>
#include <glew.h>
#include "openvr.h"



class VR_CG_OpenGL
{
public:
	VR_CG_OpenGL(const std::string& sRenderModelName);
	~VR_CG_OpenGL();

	bool BInit(const vr::RenderModel_t& vrModel, const vr::RenderModel_TextureMap_t& vrDiffuseTexture);
	void Cleanup();
	void Draw();
	const std::string& GetName() const { return m_sModelName; }

private:
	GLuint m_glVertBuffer;
	GLuint m_glIndexBuffer;
	GLuint m_glVertArray;
	GLuint m_glTexture;
	GLsizei m_unVertexCount;
	std::string m_sModelName;
};

#endif