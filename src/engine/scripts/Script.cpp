#include "stdafx.h"

#include "Script.h"

Script::Script(std::weak_ptr<GameObject> _parent) : Component(_parent) {}

void Script::Begin() {}
void Script::Tick() {}
