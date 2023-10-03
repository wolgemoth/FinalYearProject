#include "stdafx.h"

#include "Script.h"

namespace LouiEriksson {
	
	Script::Script(std::weak_ptr<GameObject> _parent) : Component(std::move(_parent)) {}
	
	void Script::Begin() {}
	void Script::Tick() {}
}