#include "stdafx.h"

#include "Script.h"

// @Assessor: This class was submitted for 3DGP. Please don't mark it for GACP.

namespace LouiEriksson {
	
	Script::Script(const std::shared_ptr<GameObject>& _parent) : Component(_parent) {}
	
	void Script::Begin() {}
	void Script::Tick() {}
}