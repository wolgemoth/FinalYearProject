#pragma once

#ifndef _PLAYER_H
#define _PLAYER_H

#include "Scene.h"

class Player final : public Script {

friend class Scene;

private:

	/// <summary> Test model curuthers. </summary>
	std::weak_ptr<GameObject> curuthers;

	/// <summary> Player Camera. </summary>
	std::weak_ptr<Camera> camera;

protected:

	/// <summary> Called at the beginning of the first frame. </summary>
	void Begin();

	/// <summary> Called every frame. </summary>
	void Tick();

public:

	 Player(std::weak_ptr<GameObject> _parent);
	~Player();

};

#endif // !_PLAYER_H
