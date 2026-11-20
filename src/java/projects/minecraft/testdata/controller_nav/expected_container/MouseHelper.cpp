#include "stdafx.h"
#include "MouseHelper.h"

MouseHelper::MouseHelper(Component* var1)
{
	this->windowComponent = var1;
}

void MouseHelper::grabMouseCursor()
{
	Mouse::setGrabbed(true);   // CONSOLE grab (CodeBreaker): set the pad mode flag only -- NO AWT
	this->deltaX = 0;
	this->deltaY = 0;
}

void MouseHelper::ungrabMouseCursor()
{
	Mouse::setGrabbed(false);   // CONSOLE ungrab (CodeBreaker): set the pad mode flag only -- NO AWT
	this->deltaX = 0;
	this->deltaY = 0;
}

void MouseHelper::mouseXYChange()
{
	this->deltaX = 0;
	this->deltaY = 0;
}
