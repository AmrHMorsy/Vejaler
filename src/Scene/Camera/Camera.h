#pragma once
#include "../BVH/BVH.h"
#include "../Model/Model.h"
#include "Data/CameraPack.h"
#include "Data/CameraConfig.h"
#include "../../Core/Core.h"
#include "../Scene/Data/SceneInfo.h"
#include "../BoundingVolume/BoundingBox/Data/BoundingBoxInfo.h"



class Camera {
    
private:
    
    static glm::mat4 ComputeViewMatrix(glm::vec4 position, glm::vec4 direction, glm::vec4 up);
    static glm::mat4 ComputeProjectionMatrix(CameraInfo& camera);
    static std::array<glm::vec4, 6> ComputeViewFrustumPlanes(glm::mat4 projectionViewMatrix);
    static CameraConfig ReadConfig(const nlohmann::json& j);
    static void SetUniformCameraPosition(SceneInfo& scene, CameraInfo& camera);
    static void SetUniformClippingPlanes(SceneInfo& scene, CameraInfo& camera);
    static void SetUniformViewMatrix(SceneInfo& scene, CameraInfo& camera);
    static void SetUniformProjectionMatrix(SceneInfo& scene, CameraInfo& camera);
    static void SetUniformProjectionViewMatrices(SceneInfo& scene, CameraInfo& camera);
    static void ComputeAxes(CameraInfo& camera);
    static void UpdateResolution(CameraInfo& camera, VkExtent2D newResolution);
    static void UpdateViewMatrix(CameraInfo& camera);
    static void UpdateProjectionMatrix(CameraInfo& camera);
    static void UpdateProjectionViewMatrices(CameraInfo& camera);
    static void UpdateFov(CameraInfo& camera);
    
public:
    
    static void Load(const nlohmann::json& camerasJSON, ContextInfo& context, SceneInfo& scene);
    static void Add(ContextInfo& context, SceneInfo& scene, VkExtent2D resolution, const CameraConfig& config = CameraConfig());
    static void Delete(ContextInfo& context, SceneInfo& scene);
    static nlohmann::json Save(CameraPack& cameraPack);
    static void Move(CameraInfo& camera, glm::vec4 direction, SceneInfo& scene);
    static void LookAround(CameraInfo& camera, glm::vec4 axis, float sign, float minYaw, float maxYaw, float minPitch, float maxPitch, SceneInfo& scene);
    static void Switch(CameraPack& cameraPack, SceneInfo& scene, int newSelected);
    static void HandleWindowResize(SceneInfo& scene, VkExtent2D newResolution);
};
