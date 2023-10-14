#pragma once

#ifndef FINALYEARPROJECT_SCRIPT_H
#define FINALYEARPROJECT_SCRIPT_H

#include "Time.h"

#include "Component.h"

namespace LouiEriksson {

	class Scene;
	
	/// <summary>
	/// Base class to be inherited by scriptable types.
	/// </summary>
	class Script : public Component {
	
		friend class Scene;
	
	protected:
	
		explicit Script(const std::shared_ptr<GameObject>& _parent);
	
		~Script() override = default;
	
		/// <summary> Called at the beginning of the first frame. </summary>
		virtual void Begin();
	
		/// <summary> Called every frame. </summary>
		virtual void Tick();
		
	};
}

#endif //FINALYEARPROJECT_SCRIPT_H