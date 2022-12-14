#include "ShadowMap.h"
#include "stb-master/stb_image_write.h"

ShadowMap::ShadowMap(std::shared_ptr<Shader> shader, std::shared_ptr<Shader> debugShader)
{
	width = 4096;
	height = 4096;
	this->shader = shader;
	this->debugShader = debugShader;

	glGenFramebuffers(1, &(ID));
	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	depthTexture = new DepthTexture(GL_TEXTURE_2D, width, height);
	depthTexture->bind(0);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->getID(), 0);
	glDrawBuffer(GL_NONE); // No color buffer is drawn to.
	glReadBuffer(GL_NONE); // No color buffer is drawn to.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	depthTexture->unbind();
	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;


	float quadVertices[] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};
	// setup plane VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	debugShader->use();
	debugShader->setInt("depthMap", 0);
}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::update(float width, float height)
{
}

glm::mat4 ShadowMap::lightSpaceMatrix(glm::vec2 range, glm::vec3 position, glm::vec3 direction) const
{
	if (range[0] > range[1]) {
		float tmp = range[0];
		range[0] = range[1];
		range[1] = range[0];
	}
	glm::mat4 lightProjection = glm::ortho(-16.0f, 16.0f, -16.0f, 16.0f, range[0], range[1]);
	//glm::mat4 lightProjection = glm::perspective<float>(90 , 1, range[0], range[1]);
	glm::mat4 lightView = glm::lookAt(position, position + glm::normalize(direction), glm::vec3(0.0f, 1.0f, 0.0f));

	return lightProjection * lightView;
}


void ShadowMap::use()
{
	glCullFace(GL_FRONT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
	glViewport(0, 0, width,height);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glClear(GL_DEPTH_BUFFER_BIT);

	// rending scene
}

void ShadowMap::unUse()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glCullFace(GL_BACK);
}

void ShadowMap::draw()
{
}

void ShadowMap::exportTexture(const char* fileName)
{
	std::ofstream of(fileName);
	of.close();

	GLuint* pixels = new GLuint[3 * width * height];
	memset(pixels, 0, 3 * width * height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	stbi_flip_vertically_on_write(true);
	stbi_write_png(fileName, width, height, 3, pixels, width * 3);
	stbi_flip_vertically_on_write(false);
	delete[] pixels;
}

void ShadowMap::renderQuad()
{
	depthTexture->bind(0);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
