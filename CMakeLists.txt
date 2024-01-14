cmake_minimum_required(VERSION ${CMAKE_VERSION})
project(FinalYearProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -flto -DNDEBUG")

set(     GAME_DIR ${PROJECT_SOURCE_DIR}/src/engine)
set(   ENGINE_DIR ${PROJECT_SOURCE_DIR}/src/usr   )

set(GLEW_DIR         ${PROJECT_SOURCE_DIR}/contrib/glew)
set(GLEW_INCLUDE_DIR ${GLEW_DIR}/include)
set(GLEW_LIB_DIR     ${GLEW_DIR}/lib)

set(IMGUI_DIR          ${PROJECT_SOURCE_DIR}/contrib/imgui)
set(IMGUI_BACKENDS_DIR ${PROJECT_SOURCE_DIR}/contrib/imgui/backends)

set(PELLET_DIR         ${PROJECT_SOURCE_DIR}/contrib/pellet/src/)
set(PELLET_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/contrib/pellet/src/pellet)

set(SDL2_DIR         ${PROJECT_SOURCE_DIR}/contrib/SDL2)
set(SDL2_INCLUDE_DIR ${SDL2_DIR}/include)
set(SDL2_LIB_DIR     ${SDL2_DIR}/lib/x86/)

set(OPENAL_DIR         ${PROJECT_SOURCE_DIR}/contrib/openal)
set(OPENAL_INCLUDE_DIR ${OPENAL_DIR}/include)

include_directories(
        ${GLEW_INCLUDE_DIR}
        ${IMGUI_DIR}
        ${PELLET_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/contrib/cereal/include
        ${PROJECT_SOURCE_DIR}/contrib/glm
        ${PROJECT_SOURCE_DIR}/contrib/stb/include/
        ${PROJECT_SOURCE_DIR}/contrib/wavefront/include/
        ${SDL2_INCLUDE_DIR}
        ${OPENAL_INCLUDE_DIR}
)

# ADD IMGUI:
add_library(IMGUI STATIC
        ${IMGUI_DIR}/imgui.cpp                         # MAIN ...
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp

        ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp
        ${IMGUI_BACKENDS_DIR}/imgui_impl_sdl2.cpp      # BACKENDS ...
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

add_executable(FinalYearProject
        src/main.cpp
        src/engine/scripts/audio/AudioClip.cpp
        src/engine/scripts/audio/AudioClip.h
        src/engine/scripts/audio/AudioListener.cpp
        src/engine/scripts/audio/AudioListener.h
        src/engine/scripts/audio/AudioSource.cpp
        src/engine/scripts/audio/AudioSource.h
        src/engine/scripts/audio/Sound.cpp
        src/engine/scripts/audio/Sound.h
        src/engine/scripts/core/Application.cpp
        src/engine/scripts/core/Application.h
        src/engine/scripts/core/File.cpp
        src/engine/scripts/core/File.h
        src/engine/scripts/core/Resources.cpp
        src/engine/scripts/core/Resources.h
        src/engine/scripts/core/Screen.cpp
        src/engine/scripts/core/Screen.h
        src/engine/scripts/core/Script.cpp
        src/engine/scripts/core/Script.h
        src/engine/scripts/core/Serialisation.cpp
        src/engine/scripts/core/Serialisation.h
        src/engine/scripts/core/Settings.cpp
        src/engine/scripts/core/Settings.h
        src/engine/scripts/core/Time.cpp
        src/engine/scripts/core/Time.h
        src/engine/scripts/core/Transform.cpp
        src/engine/scripts/core/Transform.h
        src/engine/scripts/core/Window.cpp
        src/engine/scripts/core/Window.h
        src/engine/scripts/ecs/Component.cpp
        src/engine/scripts/ecs/Component.h
        src/engine/scripts/ecs/GameObject.cpp
        src/engine/scripts/ecs/GameObject.h
        src/engine/scripts/ecs/Scene.cpp
        src/engine/scripts/ecs/Scene.h
        src/engine/scripts/graphics/Camera.cpp
        src/engine/scripts/graphics/Camera.h
        src/engine/scripts/graphics/Light.cpp
        src/engine/scripts/graphics/Light.h
        src/engine/scripts/graphics/Material.cpp
        src/engine/scripts/graphics/Material.h
        src/engine/scripts/graphics/Mesh.cpp
        src/engine/scripts/graphics/Mesh.h
        src/engine/scripts/graphics/Renderer.cpp
        src/engine/scripts/graphics/Renderer.h
        src/engine/scripts/graphics/Shader.cpp
        src/engine/scripts/graphics/Shader.h
        src/engine/scripts/graphics/Texture.cpp
        src/engine/scripts/graphics/Texture.h
        src/engine/scripts/graphics/textures/Cubemap.cpp
        src/engine/scripts/graphics/textures/Cubemap.h
        src/engine/scripts/graphics/textures/RenderTexture.cpp
        src/engine/scripts/graphics/textures/RenderTexture.h
        src/engine/scripts/input/Cursor.cpp
        src/engine/scripts/input/Cursor.h
        src/engine/scripts/input/Input.cpp
        src/engine/scripts/input/Input.h
        src/engine/scripts/physics/Collider.cpp
        src/engine/scripts/physics/Collider.h
        src/engine/scripts/physics/colliders/PlaneCollider.cpp
        src/engine/scripts/physics/colliders/PlaneCollider.h
        src/engine/scripts/physics/colliders/SphereCollider.cpp
        src/engine/scripts/physics/colliders/SphereCollider.h
        src/engine/scripts/physics/Collision.cpp
        src/engine/scripts/physics/Collision.h
        src/engine/scripts/physics/Physics.cpp
        src/engine/scripts/physics/Physics.h
        src/engine/scripts/physics/Rigidbody.cpp
        src/engine/scripts/physics/Rigidbody.h
        src/engine/scripts/ui/GUI.cpp
        src/engine/scripts/ui/GUI.h
        src/engine/scripts/core/utils/Hashmap.h
        src/engine/scripts/core/utils/Utils.cpp
        src/engine/scripts/core/utils/Utils.h
)

add_library(Game
        src/usr/scripts/Ball.cpp
        src/usr/scripts/Ball.h
        src/usr/scripts/FlyCam.cpp
        src/usr/scripts/FlyCam.h
        src/usr/scripts/OrbitCam.cpp
        src/usr/scripts/OrbitCam.h
        src/usr/scripts/Plane.cpp
        src/usr/scripts/Plane.h
)

# TODO: Find out why OpenAL doesnt' work unless added like this, and replace this method.
add_subdirectory(${OPENAL_DIR})

link_directories(FinalYearProject
        ${GLEW_LIB_DIR}
        ${SDL2_LIB_DIR}
)

target_link_libraries(FinalYearProject
        GL
        GLEW
        IMGUI
        OpenAL
        PELLET
        SDL2
        SDL2main
        Game
)

set(    AUDIO_DIR ${PROJECT_SOURCE_DIR}/src/engine/audio    )
set(   LEVELS_DIR ${PROJECT_SOURCE_DIR}/src/engine/levels   )
set(   MODELS_DIR ${PROJECT_SOURCE_DIR}/src/engine/models   )
set(  SHADERS_DIR ${PROJECT_SOURCE_DIR}/src/engine/shaders  )
set( TEXTURES_DIR ${PROJECT_SOURCE_DIR}/src/engine/textures )
set(MATERIALS_DIR ${PROJECT_SOURCE_DIR}/src/engine/materials)

file(COPY     ${AUDIO_DIR} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY    ${LEVELS_DIR} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY    ${MODELS_DIR} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY   ${SHADERS_DIR} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY  ${TEXTURES_DIR} DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${MATERIALS_DIR} DESTINATION ${PROJECT_BINARY_DIR})

target_precompile_headers(FinalYearProject PRIVATE src/stdafx.h)
target_precompile_headers(Game PRIVATE src/stdafx.h)