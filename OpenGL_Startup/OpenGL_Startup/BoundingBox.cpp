#include "BoundingBox.h"

BoundingBox::BoundingBox(std::vector<Mesh> mesh)
{
	vertex = loadBoxWithMesh(mesh);
}

BoundingBox::BoundingBox(glm::vec3 min, glm::vec3 max)
{
}

BoundingBox::BoundingBox(const BoundingBox& bb)
{
	vertex = bb.vertex;
	vertices = bb.vertices;
	indices = bb.indices;
}

BoundingBox::BoundingBox()
{
	vertex = std::vector<glm::vec3>(2, glm::vec3(0));
}

BoundingBox::~BoundingBox()
{
	//delete shader;
}

vector<glm::vec3> BoundingBox::loadBoxWithMesh(std::vector<Mesh> mesh)
{
	std::set<float> setX;
	std::set<float> setY;
	std::set<float> setZ;

	for (const auto& singleMesh : mesh) {
		for (const auto& val : singleMesh.vertices) {
			setX.insert(val.Position.x);
			setY.insert(val.Position.y);
			setZ.insert(val.Position.z);
		}
	}

	vertex.push_back(glm::vec3(*setX.begin(), *setY.begin(), *setZ.begin())); // min
	vertex.push_back(glm::vec3(*setX.rbegin(), *setY.rbegin(), *setZ.rbegin())); // max

	float cube_vertices[] = {
		// front
		vertex[0].x, vertex[1].y, vertex[1].z, 1.0, 0.0, 0.0,
		vertex[1].x, vertex[1].y, vertex[1].z, 0.0, 1.0, 0.0,
		vertex[1].x, vertex[0].y, vertex[1].z, 0.0, 0.0, 1.0,
		vertex[0].x, vertex[0].y, vertex[1].z, 0.0, 1.0, 1.0,
		// back													  
		vertex[0].x, vertex[1].y, vertex[0].z, 1.0, 0.0, 1.0,
		vertex[1].x, vertex[1].y, vertex[0].z, 1.0, 1.0, 0.0,
		vertex[1].x, vertex[0].y, vertex[0].z, 0.1, 0.1, 0.1,
		vertex[0].x, vertex[0].y, vertex[0].z, 1.0, 1.0, 1.0,
	};
	unsigned int cube_elements[] = {
		0,3,2,  //Front
		2,1,0,
		1,5,6,	//Right
		6,2,1,
		5,4,7,	//Back
		7,6,5,
		4,7,3,	//Left
		3,0,4,
		4,5,1,	//Top
		1,0,4,
		3,2,6,	//Bottom
		6,7,3,
	};
	unsigned int cube_normal[] = {
		0.0, 0.0, 1.0,   //Front
		0.0, 0.0, 1.0,
		1.0, 0.0, 0.0,	 //Right
		1.0, 0.0, 0.0,
		0.0, 0.0, -1.0,	 //Back
		0.0, 0.0, -1.0,
		-1.0, 0.0, 0.0,	 //Left
		-1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,	 //Top
		0.0, 1.0, 0.0,
		0.0, -1.0, 0.0,	 //Bottom
		0.0, -1.0, 0.0
	};
	vertices.insert(vertices.begin(), std::begin(cube_vertices), std::end(cube_vertices));
	indices.insert(indices.begin(), std::begin(cube_elements), std::end(cube_elements));
	normal.insert(normal.begin(), std::begin(cube_normal), std::end(cube_normal));

	return vertex;
}

/*
	 1. reference:
		Solving equation -> can't work
		https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection

	 2. reference:
		Smits¡¦ algorithm -> working!
		http://people.csail.mit.edu/amy/papers/box-jgt.pdf
*/
float BoundingBox::raycast(glm::vec3 origin, glm::vec3 direction, float distance, glm::mat4 model)
{

	// distance < 0 -> behine
	//glm::vec3 end = origin + glm::normalize(direction) * distance;

	//float minDis = 10000000.0;
	//for (int i = 4; i < 5; i++) {
	//	glm::vec3 planeNormal = glm::mat3(glm::transpose(glm::inverse(model))) * normalTable[i];
	//	
	//	float denom = glm::dot(planeNormal, glm::normalize(direction));
	//	if (denom > 1e-6) { // not parallel
	//		
	//		glm::vec3 p0 = { vertices[indices[6 * i] * 6] , vertices[indices[6 * i] * 6 + 1] , vertices[indices[6 * i] * 6 + 2] };
	//		p0 = glm::normalize(model * glm::vec4(p0,1.0));

	//		glm::vec3 p0l0 = p0 - origin;
	//		float d = glm::dot(p0l0, planeNormal) / denom;
	//		if (minDis > d && d >= 0.0f){
	//			minDis = d;
	//		}
	//	}
	//}

	/*   y
		 |
		 |__ x
		/
	   z
	*/

	glm::vec3 modelSpaceOrigin = glm::inverse(model) * glm::vec4(origin, 1.0);
	glm::vec3 modelSpaceDirection = glm::mat3(glm::inverse(model)) * direction;

	// 0 -> min, 1 -> max

	float t0 = 0, t1 = distance;
	float tmin, tmax, tymin, tymax, tzmin, tzmax;
	if (modelSpaceDirection.x >= 0) {
		tmin = (vertex[0].x - modelSpaceOrigin.x) / modelSpaceDirection.x;
		tmax = (vertex[1].x - modelSpaceOrigin.x) / modelSpaceDirection.x;
	}
	else {
		tmin = (vertex[1].x - modelSpaceOrigin.x) / modelSpaceDirection.x;
		tmax = (vertex[0].x - modelSpaceOrigin.x) / modelSpaceDirection.x;
	}
	if (modelSpaceDirection.y >= 0) {
		tymin = (vertex[0].y - modelSpaceOrigin.y) / modelSpaceDirection.y;
		tymax = (vertex[1].y - modelSpaceOrigin.y) / modelSpaceDirection.y;
	}
	else {
		tymin = (vertex[1].y - modelSpaceOrigin.y) / modelSpaceDirection.y;
		tymax = (vertex[0].y - modelSpaceOrigin.y) / modelSpaceDirection.y;
	}
	if ((tmin > tymax) || (tymin > tmax))
		return false;
	if (tymin > tmin)
		tmin = tymin;
	if (tymax < tmax)
		tmax = tymax;
	if (modelSpaceDirection.z >= 0) {
		tzmin = (vertex[0].z - modelSpaceOrigin.z) / modelSpaceDirection.z;
		tzmax = (vertex[1].z - modelSpaceOrigin.z) / modelSpaceDirection.z;
	}
	else {
		tzmin = (vertex[1].z - modelSpaceOrigin.z) / modelSpaceDirection.z;
		tzmax = (vertex[0].z - modelSpaceOrigin.z) / modelSpaceDirection.z;
	}
	if ((tmin > tzmax) || (tzmin > tmax))
		return false;
	if (tzmin > tmin)
		tmin = tzmin;
	if (tzmax < tmax)
		tmax = tzmax;

	return ((tmin < t1) && (tmax > t0));
}

bool BoundingBox::collision(BoundingBox tbb, glm::mat4 model1, glm::mat4 model2)
{
	vector<glm::vec3> point;
	vector<glm::vec3> v1 = vertex;
	v1[0] = model1 * glm::vec4(v1[0], 1.0);
	v1[1] = model1 * glm::vec4(v1[1], 1.0);

	point.push_back(v1[0]);
	point.push_back(glm::vec3(v1[0].x, v1[0].y, v1[1].z));
	point.push_back(glm::vec3(v1[0].x, v1[1].y, v1[0].z));
	point.push_back(glm::vec3(v1[0].x, v1[1].y, v1[1].z));
	point.push_back(glm::vec3(v1[1].x, v1[0].y, v1[0].z));
	point.push_back(glm::vec3(v1[1].x, v1[0].y, v1[1].z));
	point.push_back(glm::vec3(v1[1].x, v1[1].y, v1[0].z));
	point.push_back(glm::vec3(v1[1].x, v1[1].y, v1[1].z));

	bool col = false;
	for (size_t i = 0; i < point.size(); i++)
	{
		if (tbb.collision(point[i], model2))
			return true;
	}
	return false;

}

bool BoundingBox::collision(glm::vec3 point, glm::mat4 model)
{
	vector<glm::vec3> v = vertex;
	v[0] = model * glm::vec4(v[0], 1.0);
	v[1] = model * glm::vec4(v[1], 1.0);
	return	(v[0].x <= point.x && point.x <= v[1].x) &&
		(v[0].y <= point.y && point.y <= v[1].y) &&
		(v[0].z <= point.z && point.z <= v[1].z);
}

void BoundingBox::draw(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{


}

//BoundingBoxViewer::BoundingBoxViewer()
//{
//
//}
//
//void BoundingBoxViewer::draw(Shader* shader, glm::mat4 model)
//{
//	shader->use();
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	/* update shader uniforms*/
//	shader->setMat4("model", model);
//
//	glBindVertexArray(VAO);
//	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	shader->unUse();
//
//}
//
//void BoundingBoxViewer::loadVAO(std::vector<glm::vec3> vertex)
//
//{
//	float cube_vertices[] = {
//		// front
//		vertex[0].x, vertex[1].y, vertex[1].z, 1.0, 0.0, 0.0,
//		vertex[1].x, vertex[1].y, vertex[1].z, 0.0, 1.0, 0.0,
//		vertex[1].x, vertex[0].y, vertex[1].z, 0.0, 0.0, 1.0,
//		vertex[0].x, vertex[0].y, vertex[1].z, 0.0, 1.0, 1.0,
//		// back												
//		vertex[0].x, vertex[1].y, vertex[0].z, 1.0, 0.0, 1.0,
//		vertex[1].x, vertex[1].y, vertex[0].z, 1.0, 1.0, 0.0,
//		vertex[1].x, vertex[0].y, vertex[0].z, 0.1, 0.1, 0.1,
//		vertex[0].x, vertex[0].y, vertex[0].z, 1.0, 1.0, 1.0,
//	};
//	unsigned int cube_elements[] = {
//		0,3,2,  //Front
//		2,1,0,
//		1,5,6,	//Right
//		6,2,1,
//		5,4,7,	//Back
//		7,6,5,
//		4,7,3,	//Left
//		3,0,4,
//		4,5,1,	//Top
//		1,0,4,
//		3,2,6,	//Bottom
//		6,7,3,
//	};
//	unsigned int cube_normal[] = {
//		0.0, 0.0, 1.0,   //Front
//		0.0, 0.0, 1.0,
//		1.0, 0.0, 0.0,	 //Right
//		1.0, 0.0, 0.0,
//		0.0, 0.0, -1.0,	 //Back
//		0.0, 0.0, -1.0,
//		-1.0, 0.0, 0.0,	 //Left
//		-1.0, 0.0, 0.0,
//		0.0, 1.0, 0.0,	 //Top
//		0.0, 1.0, 0.0,
//		0.0, -1.0, 0.0,	 //Bottom
//		0.0, -1.0, 0.0
//	};
//
//	/* create vao */
//	glGenVertexArrays(1, &VAO);
//	glGenBuffers(1, &VBO);
//	glGenBuffers(1, &EBO);
//	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//	glBindVertexArray(VAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
//
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
//	// vertex texture coords
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
//
//	glBindVertexArray(0);
//}
