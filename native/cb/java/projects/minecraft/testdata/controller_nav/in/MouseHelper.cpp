#include "stdafx.h"
#include "MouseHelper.h"

MouseHelper::MouseHelper(Component* var1)
{
	this->windowComponent = var1;
}

void MouseHelper::grabMouseCursor()
{
	try {
		this->robot = new Robot();
		Point* var1 = new Point(this->windowComponent->getWidth() / 2, this->windowComponent->getHeight() / 2);
		this->robot->mouseMove(var1->x, var1->y);
	} catch (Exception* var2) {
		var2->printStackTrace();
	}
	this->deltaX = 0;
}

void MouseHelper::ungrabMouseCursor()
{
	if (this->windowComponent != NULL) {
		this->robot->mouseMove(1, 1);
	} else {
		this->deltaY = 0;
	}
}

void MouseHelper::mouseXYChange()
{
	this->deltaX = 0;
	this->deltaY = 0;
}
