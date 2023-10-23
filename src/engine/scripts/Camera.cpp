#include "stdafx.h"

#include "Camera.h"

namespace LouiEriksson {

	Camera::Camera(const std::shared_ptr<GameObject>& _parent) : Component(_parent), m_RT(1, 1) {
		
		m_Window    = std::shared_ptr<Window>(nullptr);
		m_Transform = std::shared_ptr<Transform>(nullptr);
	
		m_FOV      = 90.0f;
		m_NearClip = 0.1f;
		m_FarClip  = 1000.0f;
	
		m_Projection = glm::mat4(1.0f);
		
		VAO0 = -1;
		VBO  = -1;
		
		m_IsDirty = true;
	}
	
	Camera::~Camera() {
	
		// Unlink the camera from the window. Catch and log any errors.
		try {
			if (m_Window != nullptr) {
				m_Window->Unlink(*this);
			}
		}
		catch (std::exception& e) {
			std::cout << e.what() << "\n";
		}
	}
	
	void Camera::Clear() {
	
		// Clear the camera.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	void Camera::PreRender() {
		
		// Resize the frame buffer.
		// TODO: Set up enum flags for dirtying instead of m_IsDirty so that this doesn't happen every frame.
		auto dimensions = GetWindow()->Dimensions();
		m_RT.Resize(dimensions[0], dimensions[1]);
		
		/* BIND FRAME BUFFER */
		RenderTexture::Bind(m_RT);
	}
	
	void Camera::Render(const std::vector<std::shared_ptr<Renderer>>& _renderers) {
		
		glEnable(GL_DEPTH_TEST);
		
		for (const auto& renderer : _renderers) {
			
			// Get references to various components needed for rendering.
			const auto transform = renderer->GetTransform();
			const auto material  = renderer->GetMaterial();
			const auto mesh      = renderer->GetMesh();
			const auto program   = material->GetShader();
			
			// Bind program.
			Shader::Bind(program->ID());
			
			// Assign matrices.
			program->Assign(material->m_ProjectionMatrixID, Projection()    ); /* PROJECTION */
			program->Assign(material->m_ViewMatrixID,       View()          ); /* VIEW       */
			program->Assign(material->m_ModelMatrixID,      transform->TRS()); /* MODEL      */

			// Assign parameters (PBR).
			program->Assign(program->AttributeID("u_Metallic"), 0.1f);
			program->Assign(program->AttributeID("u_Roughness"), 0.334f);
			program->Assign(program->AttributeID("u_CameraPosition"), GetTransform()->m_Position);
			program->Assign(program->AttributeID("u_AmbientLighting"), glm::vec4(0.5f, 0.5f, 0.8f, 1.0f));

			program->Assign(program->AttributeID("u_PointLightPosition"), glm::vec3(0.0f, 5.0f, 0.0f));
			program->Assign(program->AttributeID("u_PointLightRange"), 100.0f);
			program->Assign(program->AttributeID("u_PointLightBrightness"), 1.2f);
			program->Assign(program->AttributeID("u_PointLightColor"), glm::vec4(1, 1, 1, 1));

//			// Assign parameters.
//			program->Assign(program->AttributeID("u_Roughness"), 0.1f);
//			program->Assign(program->AttributeID("u_CameraPosition"), GetTransform()->m_Position);
//			program->Assign(program->AttributeID("u_AmbientLighting"), glm::vec4(0.5f, 0.5f, 0.8f, 1.0f));
//
//			program->Assign(program->AttributeID("u_PointLightPosition"), glm::vec3(-5.0f, 5.0f, 0.0f));
//			program->Assign(program->AttributeID("u_PointLightRange"), 30.0f);
//			program->Assign(program->AttributeID("u_PointLightBrightness"), 2.2f);
//			program->Assign(program->AttributeID("u_PointLightColor"), glm::vec4(1, 1, 1, 1));
//
//			 const auto dirRot = glm::quat(
//				glm::radians(
//					glm::vec3(
//						-25.0f, 90.0f, 0.0f
//					)
//				)
//			);
//
//			program->Assign(program->AttributeID("u_DirectionalLightNormal"), dirRot * glm::vec3(0, 0, 1));
//			program->Assign(program->AttributeID("u_DirectionalLightBrightness"), 0.5f);
//			program->Assign(program->AttributeID("u_DirectionalLightColor"), glm::vec4(1, 1, 0.7, 1));
//
//			program->Assign(program->AttributeID("u_FogColor"), ClearColor());
//			program->Assign(program->AttributeID("u_FogDensity"), 0.0002f);
//
			// Bind texture and VAO.
			glBindVertexArray(mesh->VAO_ID());
			glBindTexture(GL_TEXTURE_2D, material->Texture_ID());
			
			/* DRAW */
			glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(mesh->VertexCount()));
			
			// Unbind program.
			Shader::Unbind();
			
			// Unbind texture and VAO.
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
		}
		
		glDisable(GL_DEPTH_TEST);
	}
	
	void Camera::PostRender() {
		
		/* UNBIND FRAME BUFFER */
		RenderTexture::Unbind();
		
		// Buffers to store mesh data.
		glGenVertexArrays(1, &VAO0);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, Mesh::Quad::s_VertexCount, &Mesh::Quad::s_VertexData, GL_STATIC_DRAW);
		
		// Positions, triangles (encoded within winding order).
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
		
		// Texture coordinates.
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
		
		/* POST PROCESSING */
		
		// Load effects:
		// TODO: Allow modification of effect order and parameters outside of function.
		std::queue<std::shared_ptr<Shader>> effects;
		
		auto aces = Shader::m_Cache.Return("aces");
		Shader::Bind(aces->ID());
		aces->Assign(aces->AttributeID("u_Gain"), -0.10f);
		aces->Assign(aces->AttributeID("u_Exposure"), 1.30f);
		Shader::Unbind();
		
		auto grain = Shader::m_Cache.Return("grain");
		Shader::Bind(grain->ID());
		grain->Assign(grain->AttributeID("u_Amount"), 0.03f);
		grain->Assign(grain->AttributeID("u_Time"), Time::Elapsed());
		Shader::Unbind();
		
		auto vignette = Shader::m_Cache.Return("vignette");
		Shader::Bind(vignette->ID());
		vignette->Assign(vignette->AttributeID("u_Amount"), 0.33f);
		Shader::Unbind();
		
		// Push effects to queue.
		effects.push(aces); // TONEMAPPING
		effects.push(grain); // GRAIN
		effects.push(vignette); // VIGNETTE
		
		Bloom();
		
		// Draw post processing.
		PostProcess(effects);
		
		/* RENDER TO SCREEN */
		Shader::Bind(Shader::m_Cache.Return("passthrough")->ID());
		glDrawArrays(GL_TRIANGLES, 0, Mesh::Quad::s_VertexCount);
		Shader::Unbind();
		
		// Unbind texture and VAO.
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
	}
	
	void Camera::Bloom() const {
		
		const auto dimensions = GetWindow()->Dimensions();
		
		// Get each shader used for rendering the effect.
		auto upscale = Shader::m_Cache.Return("upscale");
		auto combine = Shader::m_Cache.Return("combine");
		
		/* SET BLOOM PARAMETERS */
		
		float diffusion = 6.0f;
		
		auto threshold = Shader::m_Cache.Return("threshold");
		Shader::Bind(threshold->ID());
		threshold->Assign(threshold->AttributeID("u_Threshold"), 1.2f);
		Shader::Unbind();
		
		// Determine number of passes to perform using the amount of times the screen can be divided by 2.
		// Clamp the number of passes to a maximum value.
		const int scalingPasses =
			std::min(
				32,
				(int)(std::ceil(std::log2((float)std::min(dimensions[0], dimensions[1]))) * diffusion)
			);
		
		auto downscale = Shader::m_Cache.Return("downscale");
		Shader::Bind(downscale->ID());
		downscale->Assign(downscale->AttributeID("u_Resolution"), glm::vec2(dimensions[0], dimensions[1]));
		downscale->Assign(downscale->AttributeID("u_Diffusion"), diffusion);
		
		float intensity = 0.1f;
		
		RenderTexture rt1(dimensions[0], dimensions[1]);
		RenderTexture rt2(dimensions[0], dimensions[1]);
		
		Copy(m_RT, rt1, *threshold);
		
		for (size_t i = 0; i < scalingPasses; i++) {
			
			Copy(rt1, rt2, *downscale);
			Copy(rt2, rt1, *upscale);
		}
		
		Shader::Unbind();
		Shader::Bind(combine->ID());
		
		combine->Assign(combine->AttributeID("u_Strength"), intensity);
		
		glActiveTexture(GL_TEXTURE1);
		glEnable(GL_TEXTURE_2D);
		combine->Assign(combine->AttributeID("u_Texture1"), 1);
		glBindTexture(GL_TEXTURE_2D, rt1.ID());
		
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		combine->Assign(combine->AttributeID("u_Texture0"), 0);
		glBindTexture(GL_TEXTURE_2D, m_RT.ID());
		
		RenderTexture::Bind(m_RT);
		glDrawArrays(GL_TRIANGLES, 0, Mesh::Quad::s_VertexCount);
		RenderTexture::Unbind();
		
		Shader::Unbind();
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	void Camera::PostProcess(std::queue<std::shared_ptr<Shader>> _effects) const {
		
		while (!_effects.empty()) {
			
			const auto shader = _effects.front();
			_effects.pop();
			
			Copy(m_RT, m_RT, *shader);
		}
	}
	
	void Camera::Copy(const RenderTexture& _src, const RenderTexture& _dest) {
		
		Copy(_src, _dest, *Shader::m_Cache.Return("passthrough"));
	}
	
	void Camera::Copy(const RenderTexture& _src, const RenderTexture& _dest, const Shader& _shader) {
		
		Shader::Bind(_shader.ID());
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _src.ID());
		
		RenderTexture::Bind(_dest);
		glDrawArrays(GL_TRIANGLES, 0, Mesh::Quad::s_VertexCount);
		RenderTexture::Unbind();
		
		Shader::Unbind();
	}
	
	void Camera::SetWindow(std::shared_ptr<Window> _window) {
		_window->Link(*this);
	}
	
	std::shared_ptr<Window> Camera::GetWindow() const {
		return m_Window;
	}
	
	void Camera::SetTransform(std::shared_ptr<Transform> _transform) {
		m_Transform = _transform;
	}
	
	std::shared_ptr<Transform> Camera::GetTransform() const {
		return m_Transform;
	}
	
	float Camera::Aspect() const {
		return m_Window->Aspect();
	}
	
	void Camera::FOV(const float& _fov) {
		
		m_IsDirty = true; // Set dirty.
		
		m_FOV = _fov;
	}
	
	const float& Camera::FOV() const {
		return m_FOV;
	}
	
	void Camera::NearClip(const float& _nearClip) {
		
		m_IsDirty = true; // Set dirty.
		
		m_NearClip = _nearClip;
	}
	
	const float& Camera::NearClip() const {
		return m_NearClip;
	}
	
	void Camera::FarClip(const float& _farClip) {
		
		m_IsDirty = true; // Set dirty.
		
		m_FarClip = _farClip;
	}
	
	const float& Camera::FarClip() const {
		return m_FarClip;
	}
	
	void Camera::ClearColor(glm::vec4 _color) {
		
		// Set the clear color to the provided.
		glClearColor(_color[0], _color[1], _color[2], _color[3]);
	}
	
	glm::vec4 Camera::ClearColor() const {
		
		glm::vec4 result;
		
		// Read the clear value from opengl into the result.
		glGetFloatv(GL_COLOR_CLEAR_VALUE, &result[0]);
		
		return result;
	}
	
	const glm::mat4& Camera::Projection() {
		
		// Recalculate the perspective matrix if the camera is dirtied.
		if (m_IsDirty) {
			m_IsDirty = false;
			
			m_Projection = glm::perspective(glm::radians(m_FOV), Aspect(), m_NearClip, m_FarClip);
		}
		
		return m_Projection;
	}
	
	glm::mat4 Camera::View() const {
		
		const auto transform = GetTransform();
		
		return glm::lookAt(
			transform->m_Position,
			transform->m_Position + transform->FORWARD,
			transform->UP
		);
	}
	
	void Camera::SetDirty() {
		m_IsDirty = true;
	}
}