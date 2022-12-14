#pragma once
#include <vector>
#include <set>

#include "Mesh.h"
#include "Shader.h"

/*
* BoxVertices:
*	x min, x max
*	y min, y max
*	z min, z max
*
*/

//class BoundingBoxViewer {
//public:
//	BoundingBoxViewer();
//	void draw(Shader* shader, glm::mat4 model);
//
//	unsigned int VAO, VBO, EBO;
//	void loadVAO(std::vector<glm::vec3> vertex);
//
//};
//const glm::vec3 normalTable[] = {
//	glm::vec3(0.0, 0.0, 1.0),    //Front
//	glm::vec3(1.0, 0.0, 0.0),	 //Right
//	glm::vec3(0.0, 0.0, -1.0),	 //Back
//	glm::vec3(-1.0, 0.0, 0.0),	 //Left
//	glm::vec3(0.0, 1.0, 0.0),	 //Top
//	glm::vec3(0.0, -1.0, 0.0)	 //Bottom
//};
class BoundingBox
{
public:
	enum Vertices
	{
		XMin,
		XMax,
		YMin,
		YMax,
		ZMin,
		ZMax,
	};
	enum Direction
	{
		Front,
		Right,
		Back,
		Left,
		Top,
		Bottom
	};

	std::vector<glm::vec3> vertex;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<float> normal;

	BoundingBox();
	~BoundingBox();
	BoundingBox(std::vector<Mesh> mesh);
	BoundingBox(glm::vec3 min, glm::vec3 max);
	BoundingBox(const BoundingBox& bb);

	std::vector<glm::vec3> loadBoxWithMesh(std::vector<Mesh> mesh);

	float raycast(glm::vec3 origin, glm::vec3 direction, float distance, glm::mat4 model);
	bool collision(BoundingBox bb, glm::mat4 model1, glm::mat4 model2);
	bool collision(glm::vec3 point, glm::mat4 model);
	void draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model);
};