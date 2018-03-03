/**
 * @author Nadir Rom�n Guerrero
 * @email nadir.ro.gue@gmail.com
 */
#include "DeferredRenderObject.h"

#include <iostream>

using namespace Engine;

GLenum DeferredRenderObject::COLOR_ATTACHMENTS[8] =
{
	GL_COLOR_ATTACHMENT0,
	GL_COLOR_ATTACHMENT1,
	GL_COLOR_ATTACHMENT2,
	GL_COLOR_ATTACHMENT3,
	GL_COLOR_ATTACHMENT4,
	GL_COLOR_ATTACHMENT5,
	GL_COLOR_ATTACHMENT6,
	GL_COLOR_ATTACHMENT7
};

DeferredRenderObject::DeferredRenderObject(unsigned int numBuffers, bool renderDepth)
	:colorBuffersSize(numBuffers),renderDepth(renderDepth)
{
	colorBuffers = 0;
	if (numBuffers > 0)
	{
		colorBuffers = new BufferInfo[numBuffers];
	}
	usedColorBuffers = 0;

	depthBuffer.texture = 0;
}

DeferredRenderObject::~DeferredRenderObject()
{
	if (colorBuffers != 0)
	{
		delete[] colorBuffers;
	}
}

unsigned int DeferredRenderObject::getFrameBufferId()
{
	return fbo;
}

void DeferredRenderObject::addColorBuffer(unsigned int index, GLenum gpuTextureFormat, GLenum inputTextureFormat, GLenum pixelFormat, unsigned int w, unsigned int h, float filterMethod)
{
	if (index < 0 || index > colorBuffersSize || usedColorBuffers >= 8)
		exit(-1);

	GLenum colorAttachment = COLOR_ATTACHMENTS[usedColorBuffers];
	usedColorBuffers++;

	Texture * texture = new Texture("", 0, w, h);
	texture->setGenerateMipMaps(false);
	texture->setMemoryLayoutFormat(gpuTextureFormat);
	texture->setImageFormatType(inputTextureFormat);
	texture->setPixelFormatType(pixelFormat);

	TextureInstance * ti = new TextureInstance(texture);
	ti->setMagnificationFilterType(filterMethod);
	ti->setMinificationFilterType(filterMethod);
	ti->setAnisotropicFilterEnabled(false);
	ti->setSComponentWrapType(GL_REPEAT);
	ti->setTComponentWrapType(GL_REPEAT);

	colorBuffers[index].bufferType = colorAttachment;
	colorBuffers[index].texture = ti;
}

void DeferredRenderObject::addDepthBuffer24(unsigned int w, unsigned int h)
{
	if (depthBuffer.texture != 0)
	{
		depthBuffer.texture->~TextureInstance();
		delete depthBuffer.texture;
	}

	Texture * texture = new Texture("", 0, w, h);
	texture->setGenerateMipMaps(false);
	texture->setMemoryLayoutFormat(GL_DEPTH_COMPONENT24);
	texture->setImageFormatType(GL_DEPTH_COMPONENT);
	texture->setPixelFormatType(GL_UNSIGNED_BYTE);
	
	TextureInstance * textureInstance = new TextureInstance(texture);
	textureInstance->setAnisotropicFilterEnabled(false);
	textureInstance->setMagnificationFilterType(GL_NEAREST);
	textureInstance->setMinificationFilterType(GL_NEAREST);
	
	depthBuffer.bufferType = GL_DEPTH_ATTACHMENT;
	depthBuffer.texture = textureInstance;
}

void DeferredRenderObject::addDepthBuffer32(unsigned int w, unsigned int h)
{
	if (depthBuffer.texture != 0)
	{
		depthBuffer.texture->~TextureInstance();
		delete depthBuffer.texture;
	}

	Texture * texture = new Texture("", 0, w, h);
	texture->setGenerateMipMaps(false);
	texture->setMemoryLayoutFormat(GL_DEPTH_COMPONENT32);
	texture->setImageFormatType(GL_DEPTH_COMPONENT);
	texture->setPixelFormatType(GL_UNSIGNED_BYTE);

	TextureInstance * textureInstance = new TextureInstance(texture);
	textureInstance->setAnisotropicFilterEnabled(false);
	textureInstance->setMagnificationFilterType(GL_NEAREST);
	textureInstance->setMinificationFilterType(GL_NEAREST);

	depthBuffer.bufferType = GL_DEPTH_ATTACHMENT;
	depthBuffer.texture = textureInstance;
}

void DeferredRenderObject::initialize()
{
	glGenFramebuffers(1, &fbo);
	for (unsigned int i = 0; i < colorBuffersSize; i++)
	{
		colorBuffers[i].texture->generateTexture();
	}

	depthBuffer.texture->generateTexture();
}

void DeferredRenderObject::resizeFBO(unsigned int w, unsigned int h)
{
	for (unsigned int i = 0; i < colorBuffersSize; i++)
	{
		colorBuffers[i].texture->reseize(w, h);
	}

	depthBuffer.texture->reseize(w, h);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLenum * buffers = new GLenum[colorBuffersSize];
	for (unsigned int i = 0; i < colorBuffersSize; i++)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, colorBuffers[i].bufferType, GL_TEXTURE_2D, colorBuffers[i].texture->getTexture()->getTextureId(), 0);
		buffers[i] = colorBuffers[i].bufferType;
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer.texture->getTexture()->getTextureId(), 0); 
		
	glDrawBuffers(colorBuffersSize, buffers);

	if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
		exit(-1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRenderObject::populateDeferredObject(Object * object)
{
	for (unsigned int i = 0; i < colorBuffersSize; i++)
	{
		object->addTexture("color_" + std::to_string(i), colorBuffers[i].texture);
	}

	if (renderDepth)
	{
		object->addTexture("depth", depthBuffer.texture);
	}
}