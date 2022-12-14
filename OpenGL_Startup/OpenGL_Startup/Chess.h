#pragma once

#include <iostream>
#include <memory>

#include "GameObject.h"
#include "BoundingBox.h"
#include "Board.h"

class Chess : public GameObject
{
public:
	enum class Color
	{
		Black = 0,
		White = 1
	};
	enum Status
	{
		OnBoard = 1,
		Hint = 1 << 1,
		fliping = 1 << 2,
		hover = 1 << 3,
	};

	Chess();
	Chess(const Color& color, shared_ptr<Model> black, shared_ptr<Model> white);
	Chess(const Chess& chess);

	void update(float deltaTime);
	virtual void draw(Shader* shader) override;

	void flipColor();
	Color getColor();

	void setStatus(unsigned int status, bool flag);
	bool getStatus(unsigned int status) const;

	BoundingBox bb;

	Model* chessModel;
private:
	Color _color;
	unsigned int _status;

	std::shared_ptr<Model> blackModel;
	std::shared_ptr<Model> whiteModel;

	float _alpha;
};

