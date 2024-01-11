cmake_minimum_required(VERSION ${CMAKE_VERSION})
project(FinalYearProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -flto -DNDEBUG")

add_executable(FinalYearProject

        src/engine/scripts/stdafx.h

        src/engine/scripts/main.cpp

        src/engine/scripts/Core/Application.cpp
        src/engine/scripts/Core/Application.h

        "src/engine/scripts/User Scripts/Ball.cpp"
        "src/engine/scripts/User Scripts/Ball.h"

        src/engine/scripts/Graphics/Camera.cpp
        src/engine/scripts/Graphics/Camera.h

        src/engine/scripts/Physics/Collider.cpp
        src/engine/scripts/Physics/Collider.h

        src/engine/scripts/Colliders.h

        src/engine/scripts/Physics/Collision.cpp
        src/engine/scripts/Physics/Collision.h

        src/engine/scripts/ECS/Component.cpp
        src/engine/scripts/ECS/Component.h

        src/engine/scripts/Components.h

        src/engine/scripts/ECS/Entity.cpp
        src/engine/scripts/ECS/Entity.h

        src/engine/scripts/Core/File.cpp
        src/engine/scripts/Core/File.h

        "src/engine/scripts/User Scripts/FlyCam.cpp"
        "src/engine/scripts/User Scripts/FlyCam.h"

        src/engine/scripts/ECS/GameObject.cpp
        src/engine/scripts/ECS/GameObject.h

        src/engine/scripts/Utils/Hashmap.h

        src/engine/scripts/Input/Input.cpp
        src/engine/scripts/Input/Input.h

        src/engine/scripts/Graphics/Light.cpp
        src/engine/scripts/Graphics/Light.h

        src/engine/scripts/Graphics/Material.cpp
        src/engine/scripts/Graphics/Material.h

        src/engine/scripts/Graphics/Mesh.cpp
        src/engine/scripts/Graphics/Mesh.h

        "src/engine/scripts/User Scripts/OrbitCam.cpp"
        "src/engine/scripts/User Scripts/OrbitCam.h"

        "src/engine/scripts/User Scripts/Plane.cpp"
        "src/engine/scripts/User Scripts/Plane.h"

        src/engine/scripts/Physics/Colliders/PlaneCollider.cpp
        src/engine/scripts/Physics/Colliders/PlaneCollider.h

        "src/engine/scripts/User Scripts/Player.cpp"
        "src/engine/scripts/User Scripts/Player.h"

        src/engine/scripts/Graphics/Textures/RenderTexture.cpp
        src/engine/scripts/Graphics/Textures/RenderTexture.h

        src/engine/scripts/Graphics/Renderer.cpp
        src/engine/scripts/Graphics/Renderer.h

        src/engine/scripts/Physics/Rigidbody.cpp
        src/engine/scripts/Physics/Rigidbody.h

        src/engine/scripts/ECS/Scene.cpp
        src/engine/scripts/ECS/Scene.h

        src/engine/scripts/Core/Script.cpp
        src/engine/scripts/Core/Script.h

        src/engine/scripts/Scripts.h

        src/engine/scripts/Core/Serialisation.cpp
        src/engine/scripts/Core/Serialisation.h

        src/engine/scripts/Graphics/Shader.cpp
        src/engine/scripts/Graphics/Shader.h

        src/engine/scripts/Physics/Colliders/SphereCollider.cpp
        src/engine/scripts/Physics/Colliders/SphereCollider.h

        src/engine/scripts/Core/Time.cpp
        src/engine/scripts/Core/Time.h

        src/engine/scripts/Core/Transform.cpp
        src/engine/scripts/Core/Transform.h

        src/engine/scripts/Core/Window.cpp
        src/engine/scripts/Core/Window.h

        src/engine/scripts/Graphics/Textures/Texture.cpp
        src/engine/scripts/Graphics/Textures/Texture.h

        src/engine/scripts/Graphics/Textures/Cubemap.cpp
        src/engine/scripts/Graphics/Textures/Cubemap.h

        src/engine/scripts/Core/Resources.cpp
        src/engine/scripts/Core/Resources.h

        src/engine/scripts/Utils/Utils.cpp
        src/engine/scripts/Utils/Utils.h

        src/engine/scripts/UI/GUI.cpp
        src/engine/scripts/UI/GUI.h

        src/engine/scripts/Core/Settings.cpp
        src/engine/scripts/Core/Settings.h

        src/engine/scripts/Input/Cursor.cpp
        src/engine/scripts/Input/Cursor.h

        src/engine/scripts/Core/Screen.cpp
        src/engine/scripts/Core/Screen.h

        src/engine/scripts/Audio/Sound.cpp
        src/engine/scripts/Audio/Sound.h

        src/engine/scripts/Physics/Physics.cpp
        src/engine/scripts/Physics/Physics.h

        src/engine/scripts/Audio/AudioSource.cpp
        src/engine/scripts/Audio/AudioSource.h

        src/engine/scripts/Audio/AudioListener.cpp
        src/engine/scripts/Audio/AudioListener.h

        src/engine/scripts/Audio/AudioClip.cpp
        src/engine/scripts/Audio/AudioClip.h
)

set(GLEW_DIR         ${PROJECT_SOURCE_DIR}/external/glew)
set(GLEW_INCLUDE_DIR ${GLEW_DIR}/include)
set(GLEW_LIB_DIR     ${GLEW_DIR}/lib)

set(IMGUI_DIR          ${PROJECT_SOURCE_DIR}/external/imgui)
set(IMGUI_BACKENDS_DIR ${PROJECT_SOURCE_DIR}/external/imgui/backends)

set(PELLET_DIR         ${PROJECT_SOURCE_DIR}/external/pellet/src/)
set(PELLET_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/pellet/src/pellet)

set(SDL2_DIR         ${PROJECT_SOURCE_DIR}/external/SDL2)
set(SDL2_INCLUDE_DIR ${SDL2_DIR}/include)
set(SDL2_LIB_DIR     ${SDL2_DIR}/lib/x86/)

set(OPENAL_DIR         ${PROJECT_SOURCE_DIR}/external/openal)
set(OPENAL_INCLUDE_DIR ${OPENAL_DIR}/include)

include_directories(
        ${PROJECT_SOURCE_DIR}/external/cereal/include
        ${GLEW_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/external/glm
        ${IMGUI_DIR}
        ${PELLET_INCLUDE_DIR}
        ${SDL2_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/external/stb/include/
        ${PROJECT_SOURCE_DIR}/external/wavefront/include/
)

# ADD IMGUI:
add_library(IMGUI STATIC
        ${IMGUI_DIR}/imgui.cpp                         # MAIN ...
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp

        ${IMGUI_BACKENDS_DIR}/imgui_impl_sdl2.cpp      # BACKENDS ...
        ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp
)

# ADD PELLET:
add_library(PELLET
        ${PELLET_INCLUDE_DIR}/Bullet3Collision/BroadPhaseCollision/b3DynamicBvh.cpp
        ${PELLET_INCLUDE_DIR}/Bullet3Collision/BroadPhaseCollision/b3DynamicBvhBroadphase.cpp
        ${PELLET_INCLUDE_DIR}/Bullet3Collision/BroadPhaseCollision/b3OverlappingPairCache.cpp
        ${PELLET_INCLUDE_DIR}/Bullet3Collision/NarrowPhaseCollision/b3ConvexUtility.cpp
        ${PELLET_INCLUDE_DIR}/Bullet3Collision/NarrowPhaseCollision/b3CpuNarrowPhase.cpp

        ${PELLET_INCLUDE_DIR}/Bullet3Common/b3AlignedAllocator.cpp
        ${PELLET_INCLUDE_DIR}/Bullet3Common/b3Logging.cpp
        ${PELLET_INCLUDE_DIR}/Bullet3Common/b3Vector3.cpp

        ${PELLET_INCLUDE_DIR}/Bullet3Dynamics/b3CpuRigidBodyPipeline.cpp
        ${PELLET_INCLUDE_DIR}/Bullet3Dynamics/ConstraintSolver/b3FixedConstraint.cpp
        ${PELLET_INCLUDE_DIR}/Bullet3Dynamics/ConstraintSolver/b3Generic6DofConstraint.cpp
        ${PELLET_INCLUDE_DIR}/Bullet3Dynamics/ConstraintSolver/b3PgsJacobiSolver.cpp
        ${PELLET_INCLUDE_DIR}/Bullet3Dynamics/ConstraintSolver/b3Point2PointConstraint.cpp
        ${PELLET_INCLUDE_DIR}/Bullet3Dynamics/ConstraintSolver/b3TypedConstraint.cpp

        ${PELLET_INCLUDE_DIR}/Bullet3Geometry/b3ConvexHullComputer.cpp
        ${PELLET_INCLUDE_DIR}/Bullet3Geometry/b3GeometryUtil.cpp

        ${PELLET_INCLUDE_DIR}/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/BroadphaseCollision/btDbvt.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/BroadphaseCollision/btDispatcher.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btCollisionObject.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btCollisionWorld.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btCollisionWorldImporter.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btCompoundCompoundCollisionAlgorithm.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btGhostObject.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btHashedSimplePairCache.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btManifoldResult.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/btUnionFind.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btBox2dShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btBoxShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btCapsuleShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btCollisionShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btCompoundShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btConcaveShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btConeShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btConvex2dShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btConvexHullShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btConvexInternalShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btConvexPolyhedron.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btConvexShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btCylinderShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btEmptyShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btMultiSphereShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btOptimizedBvh.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btShapeHull.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btSphereShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btTetrahedronShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btTriangleBuffer.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btTriangleCallback.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btTriangleMesh.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/CollisionShapes/btUniformScalingShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/Gimpact/btContactProcessing.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/Gimpact/btGenericPoolAllocator.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/Gimpact/btGImpactBvh.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/Gimpact/btGImpactShape.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/Gimpact/btTriangleShapeEx.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/Gimpact/gim_box_set.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/Gimpact/gim_contact.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/Gimpact/gim_memory.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/Gimpact/gim_tri_collision.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp
        ${PELLET_INCLUDE_DIR}/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp

        ${PELLET_INCLUDE_DIR}/BulletDynamics/Character/btKinematicCharacterController.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/ConstraintSolver/btContactConstraint.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/ConstraintSolver/btFixedConstraint.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/ConstraintSolver/btGearConstraint.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/ConstraintSolver/btGeneric6DofSpring2Constraint.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/ConstraintSolver/btNNCGConstraintSolver.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/Dynamics/btDiscreteDynamicsWorldMt.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/Dynamics/btRigidBody.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/Dynamics/btSimulationIslandManagerMt.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/Featherstone/btMultiBody.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/Featherstone/btMultiBodyConstraint.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/Featherstone/btMultiBodyConstraintSolver.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/Featherstone/btMultiBodyDynamicsWorld.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/Featherstone/btMultiBodyFixedConstraint.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/Featherstone/btMultiBodyJointLimitConstraint.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/Featherstone/btMultiBodyJointMotor.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/Featherstone/btMultiBodyPoint2Point.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/Featherstone/btMultiBodySliderConstraint.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/MLCPSolvers/btDantzigLCP.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/MLCPSolvers/btLemkeAlgorithm.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/MLCPSolvers/btMLCPSolver.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/Vehicle/btRaycastVehicle.cpp
        ${PELLET_INCLUDE_DIR}/BulletDynamics/Vehicle/btWheelInfo.cpp

        ${PELLET_INCLUDE_DIR}/BulletInverseDynamics/details/MultiBodyTreeImpl.cpp
        ${PELLET_INCLUDE_DIR}/BulletInverseDynamics/details/MultiBodyTreeInitCache.cpp
        ${PELLET_INCLUDE_DIR}/BulletInverseDynamics/IDMath.cpp
        ${PELLET_INCLUDE_DIR}/BulletInverseDynamics/MultiBodyTree.cpp

        ${PELLET_INCLUDE_DIR}/BulletSoftBody/btDefaultSoftBodySolver.cpp
        ${PELLET_INCLUDE_DIR}/BulletSoftBody/btSoftBody.cpp
        ${PELLET_INCLUDE_DIR}/BulletSoftBody/btSoftBodyConcaveCollisionAlgorithm.cpp
        ${PELLET_INCLUDE_DIR}/BulletSoftBody/btSoftBodyHelpers.cpp
        ${PELLET_INCLUDE_DIR}/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.cpp
        ${PELLET_INCLUDE_DIR}/BulletSoftBody/btSoftMultiBodyDynamicsWorld.cpp
        ${PELLET_INCLUDE_DIR}/BulletSoftBody/btSoftRigidCollisionAlgorithm.cpp
        ${PELLET_INCLUDE_DIR}/BulletSoftBody/btSoftRigidDynamicsWorld.cpp
        ${PELLET_INCLUDE_DIR}/BulletSoftBody/btSoftSoftCollisionAlgorithm.cpp

        ${PELLET_INCLUDE_DIR}/LinearMath/btAlignedAllocator.cpp
        ${PELLET_INCLUDE_DIR}/LinearMath/btConvexHull.cpp
        ${PELLET_INCLUDE_DIR}/LinearMath/btConvexHullComputer.cpp
        ${PELLET_INCLUDE_DIR}/LinearMath/btGeometryUtil.cpp
        ${PELLET_INCLUDE_DIR}/LinearMath/btPolarDecomposition.cpp
        ${PELLET_INCLUDE_DIR}/LinearMath/btQuickprof.cpp
        ${PELLET_INCLUDE_DIR}/LinearMath/btSerializer.cpp
        ${PELLET_INCLUDE_DIR}/LinearMath/btThreads.cpp
        ${PELLET_INCLUDE_DIR}/LinearMath/btVector3.cpp
)

link_directories(FinalYearProject
        ${GLEW_LIB_DIR}
        ${SDL2_LIB_DIR}
)

add_subdirectory(${OPENAL_DIR})

target_link_libraries(FinalYearProject
        GLEW
        GL
        IMGUI
        OpenAL
        PELLET
        SDL2main
        SDL2
)

set(    AUDIO_DIR ${PROJECT_SOURCE_DIR}/src/engine/audio    )
set(   LEVELS_DIR ${PROJECT_SOURCE_DIR}/src/engine/levels   )
set(   MODELS_DIR ${PROJECT_SOURCE_DIR}/src/engine/models   )
set(MATERIALS_DIR ${PROJECT_SOURCE_DIR}/src/engine/materials)
set(  SHADERS_DIR ${PROJECT_SOURCE_DIR}/src/engine/shaders  )
set( TEXTURES_DIR ${PROJECT_SOURCE_DIR}/src/engine/textures )

file(COPY     ${AUDIO_DIR} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY    ${LEVELS_DIR} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY    ${MODELS_DIR} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${MATERIALS_DIR} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY   ${SHADERS_DIR} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY  ${TEXTURES_DIR} DESTINATION ${PROJECT_BINARY_DIR})

target_precompile_headers(FinalYearProject PRIVATE src/engine/scripts/stdafx.h)