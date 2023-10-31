#include "stdafx.h"

#include "Camera.h"

namespace LouiEriksson {

	Camera::Camera(const std::shared_ptr<GameObject>& _parent) : Component(_parent), m_RT(1, 1) {
		
		m_Window    = std::shared_ptr<Window>   (nullptr);
		m_Transform = std::shared_ptr<Transform>(nullptr);
	
		m_FOV      = 90.0f;
		m_NearClip = 0.1f;
		m_FarClip  = 1000.0f;
	
		m_Projection = glm::mat4(1.0f);
		
		VAO = -1;
		VBO = -1;
		
		m_IsDirty = true;
		
		m_Cube = Mesh::Load("models/cube/cube.obj");
		
		m_Sky = std::move(
			File::Load(
				{
					"textures/cubemaps/san_francisco_3/posx.jpg",
					"textures/cubemaps/san_francisco_3/negx.jpg",
					"textures/cubemaps/san_francisco_3/posy.jpg",
					"textures/cubemaps/san_francisco_3/negy.jpg",
					"textures/cubemaps/san_francisco_3/posz.jpg",
					"textures/cubemaps/san_francisco_3/negz.jpg"
				},
				GL_RGB,
				true
			)
		);
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
		
		const auto  cullMode = GL_BACK;
		const auto depthMode = GL_LESS;
		
		const float skyExposure = 1.6f;
		
		glm::mat4 lightSpaceMatrix(1);
		
		GLuint shadowDepthMap;
		
		const int shadowResolution = 2048;
		
		const float shadowDistance   = glm::min(100.0f, m_FarClip);
		const float shadowBias       = 0.002f;
		const float shadowNormalBias = 0.05f;
		
		const bool twoSidedShadows = false;
		
		glDepthFunc(depthMode);
		
		/* DRAW SHADOWS */
		{
			// Create FBO for depth.
			GLuint depthMapFBO;
			glGenFramebuffers(1, &depthMapFBO);
			
			// Generate texture for shadow map (will bind it to the FBO).
			glGenTextures(1, &shadowDepthMap);
			glBindTexture(GL_TEXTURE_2D, shadowDepthMap);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowResolution, shadowResolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			glViewport(0, 0, shadowResolution, shadowResolution);
			
			// Bind shadow texture to FBO.
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowDepthMap, 0);
			
			// Explicitly tell opengl that we're not rendering any color data in this FBO.
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
			
			// Set culling to front.
			glCullFace (twoSidedShadows ? GL_NONE : GL_FRONT);
			
			auto shadowShader = Shader::m_Cache.Return("shadowDepth");
			
			Shader::Bind(shadowShader->ID());
			
			const glm::mat4 lightProjection =
				glm::ortho(
				   -shadowDistance / 2.0f,
				    shadowDistance / 2.0f,
				   -shadowDistance / 2.0f,
				    shadowDistance / 2.0f,
					0.0f,          // Near plane.
					shadowDistance // Far plane.
				);
			
			const auto lightRot = glm::quat(glm::radians(glm::vec3(-45.0f, 135.0f, 0.0f)));
			const auto lightDir = VEC_FORWARD * lightRot;
			
			// Compute the size of a texel in world space.
			// We can round the light's position to these coordinates
			// to reduce an artifact known as "shimmering".
			const float texelSize = shadowDistance / (shadowResolution / 2);
			
			// Artificially add
			const glm::vec3 truncatedCamPos = glm::floor(
					GetTransform()->m_Position / texelSize) * texelSize;
			
			const glm::vec3 lightPos = truncatedCamPos + (lightDir * (shadowDistance / 2.0f));
			
			const glm::mat4 lightView = glm::lookAt(
				lightPos,
				lightPos - lightDir,
				VEC_UP
			);
			
			lightSpaceMatrix = lightProjection * lightView;
			
			shadowShader->Assign(shadowShader->AttributeID("u_LightSpaceMatrix"), lightSpaceMatrix);
			
			// We need to render the scene from the light's perspective.
			for (const auto& renderer : _renderers) {
				
				const auto transform = renderer->GetTransform();
				const auto mesh      = renderer->GetMesh();
				
				// Bind VAO.
				glBindVertexArray(mesh->VAO_ID());
				
				shadowShader->Assign(shadowShader->AttributeID("u_Model"), transform->TRS());
				
				/* DRAW */
				glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(mesh->VertexCount()));
				
				// Unbind VAO.
				glBindVertexArray(0);
			}
			
			// Unbind the program.
			Shader::Unbind();
			
			// Unbind the texture.
			Texture::Unbind();
			
			// Unbind the FBO
			RenderTexture::Unbind();
			
			glDeleteFramebuffers(1, &depthMapFBO);
			
			auto dimensions = m_Window->Dimensions();
			glViewport(0, 0, dimensions[0], dimensions[1]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			RenderTexture::Bind(m_RT);
		}
		
		glCullFace(cullMode);
		
		/* DRAW OBJECTS */
		for (const auto& renderer : _renderers) {
			
			// Get references to various components needed for rendering.
			const auto transform = renderer->GetTransform();
			const auto material  = renderer->GetMaterial();
			const auto mesh      = renderer->GetMesh();
			const auto program   = material->GetShader();
			
			// Bind program.
			Shader::Bind(program->ID());
			
			// Assign matrices.
			program->Assign(material->m_ProjectionMatrixID, Projection()); /* PROJECTION */
			program->Assign(material->m_ViewMatrixID,             View()); /* VIEW       */
			program->Assign(material->m_ModelMatrixID,  transform->TRS()); /* MODEL      */
			
			program->Assign(program->AttributeID("u_LightSpaceMatrix"), lightSpaceMatrix);
			
			// Assign parameters (PBR).
			program->Assign(
				program->AttributeID("u_Albedo"),
				material->Texture_ID(),
				0,
				GL_TEXTURE_2D
			);
			
			program->Assign(
				program->AttributeID("u_ShadowMap"),
				shadowDepthMap,
				1,
				GL_TEXTURE_2D
			);
			
			program->Assign(program->AttributeID("u_Time"), Time::Elapsed());
			
			program->Assign(program->AttributeID("u_CameraPosition"), GetTransform()->m_Position);
			program->Assign(program->AttributeID("u_Metallic"), 0.0f);
			program->Assign(program->AttributeID("u_Roughness"), 0.3f);
			
			program->Assign(
				program->AttributeID("u_Ambient"),
				m_Sky.ID(),
				2,
				GL_TEXTURE_CUBE_MAP
			);
			
			program->Assign(program->AttributeID("u_AmbientExposure"), skyExposure);
			
			program->Assign(program->AttributeID("u_ShadowBias"), shadowBias);
			program->Assign(program->AttributeID("u_ShadowNormalBias"), shadowNormalBias);
			
			program->Assign(program->AttributeID("u_PointLightPosition"), glm::vec3(0.0f, 5.0f, 0.0f));
			program->Assign(program->AttributeID("u_PointLightRange"), 100.0f);
			program->Assign(program->AttributeID("u_PointLightBrightness"), 1.2f);
			program->Assign(program->AttributeID("u_PointLightColor"), glm::vec3(1, 1, 1));
			
			// Bind VAO.
			glBindVertexArray(mesh->VAO_ID());
			
			/* DRAW */
			glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(mesh->VertexCount()));
			
			// Unbind program.
			Shader::Unbind();
			
			// Unbind textures.
			Texture::Unbind();
			Cubemap::Unbind();
			
			// Unbind VAO.
			glBindVertexArray(0);
		}
		
		glDeleteTextures(1, &shadowDepthMap);
		
		/* DRAW SKY */
		{
			glCullFace (GL_FRONT );
			glDepthFunc(GL_LEQUAL);
		
			auto skybox = Shader::m_Cache.Return("skybox");
			
			Shader::Bind(skybox->ID());
			
			auto trs = glm::scale(
				glm::mat4(1.0),
				glm::vec3(2.0)
			);
			
			// Assign matrices.
			skybox->Assign(skybox->AttributeID("u_Projection"),           Projection()); /* PROJECTION */
			skybox->Assign(skybox->AttributeID("u_View"), glm::mat4(glm::mat3(View()))); /* VIEW       */
			skybox->Assign(skybox->AttributeID("u_Model"),                         trs); /* MODEL      */
			
			skybox->Assign(
				skybox->AttributeID("u_Texture"),
				m_Sky.ID(),
				0,
				GL_TEXTURE_CUBE_MAP
			);
			
			skybox->Assign(skybox->AttributeID("u_Exposure"), skyExposure);
			
			// Bind VAO.
			glBindVertexArray(m_Cube->VAO_ID());
			
			/* DRAW */
			glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(m_Cube->VertexCount()));
			
			// Unbind program.
			Shader::Unbind();
			
			// Unbind textures.
			Cubemap::Unbind();
			
			// Unbind VAO.
			glBindVertexArray(0);
			
			glCullFace ( cullMode);
			glDepthFunc(depthMode);
		}
		
		glDisable(GL_DEPTH_TEST);
	}
	
	void Camera::PostRender() {
		
		/* UNBIND FRAME BUFFER */
		RenderTexture::Unbind();
		
		// Buffers to store mesh data.
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, Mesh::Quad::s_VertexCount, &Mesh::Quad::s_VertexData, GL_STATIC_DRAW);
		
		// Positions, triangles (encoded within winding order).
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
		
		// Texture coordinates.
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
		
		/* POST PROCESSING */
		
		// Load effects:
		// TODO: Allow modification of effect order and parameters outside of function.
		std::queue<std::shared_ptr<Shader>> effects;
		
		auto aces = Shader::m_Cache.Return("aces");
		Shader::Bind(aces->ID());
		aces->Assign(aces->AttributeID("u_Gain"), -0.1f);
		aces->Assign(aces->AttributeID("u_Exposure"), 1.3f);
		Shader::Unbind();
		
		auto grain = Shader::m_Cache.Return("grain");
		Shader::Bind(grain->ID());
		grain->Assign(grain->AttributeID("u_Amount"), 0.02f);
		grain->Assign(grain->AttributeID("u_Time"), Time::Elapsed());
		Shader::Unbind();
		
		auto vignette = Shader::m_Cache.Return("vignette");
		Shader::Bind(vignette->ID());
		vignette->Assign(vignette->AttributeID("u_Amount"), 0.33f);
		Shader::Unbind();
		
		// Push effects to queue.
		effects.push(aces);     // TONEMAPPING
		effects.push(grain);    // GRAIN
		effects.push(vignette); // VIGNETTE
		
		Bloom();
		
		// Draw post processing.
		PostProcess(effects);
		
		/* RENDER TO SCREEN */
		Shader::Bind(Shader::m_Cache.Return("passthrough")->ID());
		glDrawArrays(GL_TRIANGLES, 0, Mesh::Quad::s_VertexCount);
		Shader::Unbind();
		
		// Unbind texture.
		Texture::Unbind();
		Cubemap::Unbind();
		
		// Unbind VAO.
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
		
		const float intensity = 0.1f;
		
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
		
		combine->Assign(combine->AttributeID("u_Texture0"), m_RT.ID(), 0, GL_TEXTURE_2D);
		combine->Assign(combine->AttributeID("u_Texture1"),  rt1.ID(), 1, GL_TEXTURE_2D);

		RenderTexture::Bind(m_RT);
		glDrawArrays(GL_TRIANGLES, 0, Mesh::Quad::s_VertexCount);
		RenderTexture::Unbind();
		
		Shader::Unbind();
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