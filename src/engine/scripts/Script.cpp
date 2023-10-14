#include "stdafx.h"

#include "Script.h"

namespace LouiEriksson {
	
	Script::Script(const std::shared_ptr<GameObject>& _parent) : Component(_parent) {}
	
	void Script::Begin() {}
	void Script::Tick() {}
}