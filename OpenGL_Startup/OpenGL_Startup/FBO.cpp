#include "FBO.h"
#include "stb-master/stb_image_write.h"

FBO::FBO(Shader* shader)
{
	this->shader = shader;

	width = 1280;
	height = 720;

	glGenFramebuffers(1, &(ID));
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	// create a color attachment texture
	colorTextures[0] = new ColorTexture(GL_TEXTURE_2D, 1280, 720);
	colorTextures[0]->bind(0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextures[0]->getID(), 0);

	depthTexture = new DepthTexture(GL_TEXTURE_2D, width, height);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture->getID(), 0);

	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &rboID);
	glBindRenderbuffer(GL_RENDERBUFFER, rboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboID); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

FBO::~FBO()
{
	delete depthTexture;
	while (colorTextures.size()) {
		ColorTexture* tmpTexture = colorTextures.back();
		colorTextures.pop_back();
		delete tmpTexture;
	}
}

void FBO::update(float width, float height)
{
	if (abs(width - this->width) > 0.001 || abs(height - this->height) > 0.001) {
		this->width = width;
		this->height = height;
		std::cout << "[Debug] Size is changed: " << this->width << " -> " << width << "\n";
		colorTextures[0]->bind(0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	}
}

void FBO::exportTexture(const char* fileName)
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

void FBO::use()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	glEnable(GL_DEPTH_TEST);
}

void FBO::unUse()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void FBO::draw()
{
	shader->use();
	shader->setVec2("frameBufSize", glm::vec2(width, height));
	shader->setFloat("aspect", width / height);
	glBindVertexArray(quadVAO);
	glDisable(GL_DEPTH_TEST);
	colorTextures[0]->bind(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);
}
