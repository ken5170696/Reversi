#include "Chess.h"

Chess::Chess()
	: GameObject()
{
	this->_color = Color::Black;
	this->_status = Status::OnBoard;

	_alpha = 1.0f;
}

Chess::Chess(const Color& color, shared_ptr<Model> black, shared_ptr<Model> white)
{
	std::cout << "[Debug] Initialize Chess: " << int(color) << "\n";
	this->_color = color;
	this->_status = Status::OnBoard;

	_alpha = 1.0f;

	blackModel = black;
	whiteModel = white;
	bb = BoundingBox(blackModel->meshes);
}

Chess::Chess(const Chess& chess)
{
	this->_color = chess._color;
	this->_status = chess._status;
	this->bb = chess.bb;
	this->blackModel = this->blackModel;
	this->whiteModel = this->whiteModel;

	_alpha = 1.0f;
}

void Chess::update(float deltaTime)
{
	static float now = 0.0;
	static float originY = m_transform.position.y;
	now += deltaTime;
	if (getStatus(Status::OnBoard) || getStatus(Status::hover)) {
		_alpha = 1.0f;
		m_transform.scale = glm::vec3(1, 1, 1);
	}
	else {
		_alpha = glm::min(float(sin(2.0f * 3.1415926f * now / 15.0f) + 1.0f) / 2.3f, 0.5f);
		m_transform.scale = glm::vec3(0.5, 1, 0.5);
	}

	if (getStatus(Status::hover)) {
		m_transform.position.y = originY + (float(sin(2 * 3.1415926 * now / 15) + 1.0) / 2) * 0.03;
	}
	else {
		m_transform.position.y = originY;
	}

	if (getStatus(Status::fliping)) {
		m_transform.rotation += glm::vec3(5.0, 0.0, 0.0) * deltaTime * 100.f * abs(sin(glm::radians(m_transform.rotation.x)));
		static float posY = m_transform.position.y;
		m_transform.position.y = posY * (1.0 + abs(sin(glm::radians(m_transform.rotation.x))));
		

		if (m_transform.rotation.x > 180) {
			setStatus(Status::fliping, false);
			m_transform.rotation = glm::vec3(0.0, 0.0, 0.0);
		}
	}

}

void Chess::draw(Shader* shader)
{
	glm::mat4 modelMatrix = m_transform.getModelMatrix();

	shader->setMat4("model", modelMatrix);
	shader->setFloat("alpha", _alpha);

	if (_color == Color::Black) {
		blackModel->Draw(*shader);
	}
	else if (_color == Color::White) {
		whiteModel->Draw(*shader);
	}
	shader->setFloat("alpha", 1.0f);
}

void Chess::flipColor()
{
	if (_color == Color::Black)
		_color = Color::White;
	else
		_color = Color::Black;

	setStatus(Status::fliping, true);
	m_transform.rotation = glm::vec3(1.0, 0.0, 0.0);
}

Chess::Color Chess::getColor()
{
	return _color;
}

void Chess::setStatus(unsigned int status, bool flag)
{
	if (flag) {
		_status = _status | (status);
	}
	else {
		_status = _status & (~status);
	}
}

bool Chess::getStatus(unsigned int status) const
{
	return ((_status & status) != 0);
}
