#include "Camera.h"
#include "Variables/CameraModelFilePaths.h"
#include "Variables/CameraTextureFilePaths.h"
#include "../Model/Data/ModelConstants.h"





void Camera::Load(const nlohmann::json& camerasJSON, ContextInfo& context, SceneInfo& scene)
{
    for(size_t i = 0; i < camerasJSON.size(); i++)
        Camera::Add(context, scene, context.swapChain.extent, ReadConfig(camerasJSON[i]));
    
    scene.cameraPack.selected = 0;
    SetUniformProjectionViewMatrices(scene, scene.cameraPack.cameras[scene.cameraPack.selected]);
    SetUniformCameraPosition(scene, scene.cameraPack.cameras[scene.cameraPack.selected]);
    SetUniformClippingPlanes(scene, scene.cameraPack.cameras[scene.cameraPack.selected]);    
}

CameraConfig Camera::ReadConfig(const nlohmann::json& j)
{
    CameraConfig config{};
    
    if(j.contains("lockedMotion"))
        config.lockedMotion = j.value("lockedMotion", config.lockedMotion);
    
    if(j.contains("lockedPanning"))
        config.lockedPanning = j.value("lockedPanning", config.lockedPanning);
    
    if(j.contains("enableCameraCollision"))
        config.enableCameraCollision = j.value("enableCameraCollision", config.enableCameraCollision);
    
    if(j.contains("speed"))
        config.speed = j.value("speed", config.speed);
    
    if(j.contains("lookAroundSpeed"))
        config.lookAroundSpeed = j.value("lookAroundSpeed", config.lookAroundSpeed);
    
    if(j.contains("fov"))
        config.fovDegrees = j.value("fov", config.fovDegrees);
    
    if(j.contains("nearClippingPlane"))
        config.nearClippingPlane = j.value("nearClippingPlane", config.nearClippingPlane);
    
    if(j.contains("farClippingPlane"))
        config.farClippingPlane = j.value("farClippingPlane", config.farClippingPlane);
    
    if(j.contains("position") && j["position"].is_array() && j["position"].size() == 4){
        config.position = glm::vec4(j["position"][0].get<float>(), j["position"][1].get<float>(), j["position"][2].get<float>(), j["position"][3].get<float>());
        config.originalPosition = config.position;
    }
    
    if(j.contains("minBoundaries") && j["minBoundaries"].is_array() && j["minBoundaries"].size() == 3)
        config.minBoundaries = glm::vec3(j["minBoundaries"][0].get<float>(), j["minBoundaries"][1].get<float>(), j["minBoundaries"][2].get<float>());

    if(j.contains("maxBoundaries") && j["maxBoundaries"].is_array() && j["maxBoundaries"].size() == 3)
        config.maxBoundaries = glm::vec3(j["maxBoundaries"][0].get<float>(), j["maxBoundaries"][1].get<float>(), j["maxBoundaries"][2].get<float>());
    
    if(j.contains("direction") && j["direction"].is_array() && j["direction"].size() == 4){
        config.direction = glm::vec4(j["direction"][0].get<float>(), j["direction"][1].get<float>(), j["direction"][2].get<float>(), j["direction"][3].get<float>());
        config.originalDirection = config.direction;
    }
    
    return config;
}

void Camera::Add(ContextInfo& context, SceneInfo& scene, VkExtent2D resolution, const CameraConfig& config)
{
    CameraPack& cameraPack = scene.cameraPack;
    cameraPack.cameras.resize(cameraPack.cameras.size()+1);
    cameraPack.names.resize(cameraPack.cameras.size()+1);
    cameraPack.selected = cameraPack.cameras.size() - 1;
    cameraPack.cameras[cameraPack.selected].config = config;
    cameraPack.cameras[cameraPack.selected].fovRadians = glm::radians(cameraPack.cameras[cameraPack.selected].config.fovDegrees);
    cameraPack.cameras[cameraPack.selected].resolution = resolution;
    cameraPack.cameras[cameraPack.selected].aspectRatio = (float) cameraPack.cameras[cameraPack.selected].resolution.width / (float) cameraPack.cameras[cameraPack.selected].resolution.height;
    ComputeAxes(cameraPack.cameras[cameraPack.selected]);
    UpdateProjectionViewMatrices(cameraPack.cameras[cameraPack.selected]);
    for(uint32_t i = 0; i < cameraPack.names.size(); i++){
        cameraPack.cameras[i].name = "Camera " + std::to_string(i+1);
        cameraPack.names[i] = cameraPack.cameras[i].name.c_str();
    }
}

void Camera::Delete(ContextInfo& context, SceneInfo& scene)
{
    CameraPack& cameraPack = scene.cameraPack;
    
    if(cameraPack.cameras.size() <= 1)
        return;
    
    CameraInfo& camera = cameraPack.cameras[cameraPack.selected];
        
    cameraPack.cameras.erase(cameraPack.cameras.begin() + cameraPack.selected);
    cameraPack.names.erase(cameraPack.names.begin() + cameraPack.selected);
    
    for(uint32_t i = 0; i < cameraPack.cameras.size(); i++){
        cameraPack.cameras[i].name = "Camera " + std::to_string(i+1);
        cameraPack.names[i] = cameraPack.cameras[i].name.c_str();
    }
    
    cameraPack.selected = 0;
}

nlohmann::json Camera::Save(CameraPack& cameraPack)
{
    nlohmann::json camerasJSON = nlohmann::json::array();
    for(size_t k = 0; k < cameraPack.cameras.size(); k++){
        CameraConfig config = cameraPack.cameras[k].config;
        nlohmann::json cameraJSON = {
            {"speed", config.speed},
            {"fov", config.fovDegrees},
            {"lookAroundSpeed", config.lookAroundSpeed},
            {"nearClippingPlane", config.nearClippingPlane},
            {"farClippingPlane", config.farClippingPlane},
            {"position", std::vector<float>{
                config.position.x, config.position.y, config.position.z, config.position.w}},
            {"direction", std::vector<float>{
                config.direction.x, config.direction.y, config.direction.z, config.direction.w}}
        };
        camerasJSON.push_back(cameraJSON);
    }
    
    return camerasJSON;
}

void Camera::Move(CameraInfo& camera, glm::vec4 direction, SceneInfo& scene)
{
    camera.config.position = camera.config.position + (camera.config.speed * direction);
    camera.config.position.x = std::min(camera.config.maxBoundaries.x, std::max(camera.config.minBoundaries.x, camera.config.position.x));
    camera.config.position.y = std::min(camera.config.maxBoundaries.y, std::max(camera.config.minBoundaries.y, camera.config.position.y));
    camera.config.position.z = std::min(camera.config.maxBoundaries.z, std::max(camera.config.minBoundaries.z, camera.config.position.z));
    
    if(camera.config.enableCameraCollision){
        BVH::CheckIntersection(camera.config.position, scene.modelPack.bvh.bvhNodes, scene.modelPack.bvh.rootIndex);
        for(int i = 0; i < scene.modelPack.inclusiveBoundingBoxes.size(); i++)
            BoundingBox::HandleInclusiveIntersection(camera.config.position, scene.modelPack.inclusiveBoundingBoxes[i]);
    }
    
    UpdateViewMatrix(camera);
    SetUniformViewMatrix(scene, camera);
    SetUniformCameraPosition(scene, camera);
}

void Camera::LookAround(CameraInfo& camera, glm::vec4 axis, float sign, float minYaw, float maxYaw, float minPitch, float maxPitch, SceneInfo& scene)
{
    if(axis == camera.up){
        camera.yaw = fmod(camera.yaw + (sign * glm::degrees(camera.config.lookAroundSpeed)), 360.0f);
        if(camera.yaw > maxYaw){
            camera.yaw = maxYaw;
            //return;
        }
        if(camera.yaw < minYaw){
            camera.yaw = minYaw;
            //return;
        }
    }
    else if(axis == camera.right){
        camera.pitch += sign * glm::degrees(camera.config.lookAroundSpeed);
        if(camera.pitch > maxPitch){
            camera.pitch = maxPitch;
            //return;
        }
        if(camera.pitch < minPitch){
            camera.pitch = minPitch;
            //return;
        }
    }
    else if(axis == camera.config.direction)
        camera.roll = fmod(camera.roll + (sign * glm::degrees(camera.config.lookAroundSpeed)), 360.0f);
    
    camera.rotation = camera.rotation * glm::rotate(glm::mat4(1.0f), sign * camera.config.lookAroundSpeed, glm::vec3(axis));
    camera.lookAroundRotation = glm::rotate(glm::mat4(1.0f), sign * camera.config.lookAroundSpeed, glm::vec3(axis));
    camera.config.direction = glm::vec4(camera.lookAroundRotation * glm::vec4(glm::vec3(camera.config.direction), 0.0f));
    
    ComputeAxes(camera);
    UpdateViewMatrix(camera);
    SetUniformViewMatrix(scene, camera);
    SetUniformCameraPosition(scene, camera);
}

void Camera::Switch(CameraPack& cameraPack, SceneInfo& scene, int newSelected)
{
    cameraPack.selected = (cameraPack.cameras.size() >= (newSelected+1))? newSelected: cameraPack.cameras.size()-1;
    cameraPack.cameras[cameraPack.selected].config.position = cameraPack.cameras[cameraPack.selected].config.originalPosition;
    cameraPack.cameras[cameraPack.selected].config.direction = cameraPack.cameras[cameraPack.selected].config.originalDirection;
    ComputeAxes(cameraPack.cameras[cameraPack.selected]);
    UpdateProjectionViewMatrices(cameraPack.cameras[cameraPack.selected]);
    SetUniformProjectionViewMatrices(scene, cameraPack.cameras[cameraPack.selected]);
    SetUniformCameraPosition(scene, cameraPack.cameras[cameraPack.selected]);
    SetUniformClippingPlanes(scene, cameraPack.cameras[cameraPack.selected]);
}

void Camera::HandleWindowResize(SceneInfo& scene, VkExtent2D newResolution)
{
    for(uint32_t i = 0; i < scene.cameraPack.cameras.size(); i++)
        UpdateResolution(scene.cameraPack.cameras[i], newResolution);
    
    SetUniformProjectionMatrix(scene, scene.cameraPack.cameras[scene.cameraPack.selected]);
}

void Camera::UpdateFov(CameraInfo& camera)
{
    camera.fovRadians = glm::radians(camera.config.fovDegrees);
    UpdateProjectionMatrix(camera);
}

void Camera::UpdateResolution(CameraInfo& camera, VkExtent2D newResolution)
{
    camera.resolution = newResolution;
    camera.aspectRatio = (float) camera.resolution.width / (float) camera.resolution.height;
    UpdateProjectionMatrix(camera);
}

void Camera::UpdateViewMatrix(CameraInfo& camera)
{
    camera.viewMatrix = ComputeViewMatrix(camera.config.position, camera.config.direction, camera.up);
    camera.projectionViewMatrix = camera.projectionMatrix * camera.viewMatrix;
    camera.viewFrustumPlanes = ComputeViewFrustumPlanes(camera.projectionViewMatrix);
}

void Camera::UpdateProjectionMatrix(CameraInfo& camera)
{
    ComputeProjectionMatrix(camera);
    camera.inverseProjectionMatrix = glm::inverse(camera.projectionMatrix);
    camera.projectionViewMatrix = camera.projectionMatrix * camera.viewMatrix;
    camera.viewFrustumPlanes = ComputeViewFrustumPlanes(camera.projectionViewMatrix);
}

void Camera::UpdateProjectionViewMatrices(CameraInfo& camera)
{
    camera.viewMatrix = ComputeViewMatrix(camera.config.position, camera.config.direction, camera.up);
    camera.projectionMatrix = ComputeProjectionMatrix(camera);
    camera.inverseProjectionMatrix = glm::inverse(camera.projectionMatrix);
    camera.projectionViewMatrix = camera.projectionMatrix * camera.viewMatrix;
    camera.viewFrustumPlanes = ComputeViewFrustumPlanes(camera.projectionViewMatrix);
}

void Camera::SetUniformCameraPosition(SceneInfo& scene, CameraInfo& camera)
{
    scene.modelPack.fsData.cameraPosition = camera.config.position;
}

void Camera::SetUniformClippingPlanes(SceneInfo& scene, CameraInfo& camera)
{
    scene.modelPack.fsData.nearClippingPlane = camera.config.nearClippingPlane;
    scene.modelPack.fsData.farClippingPlane = camera.config.farClippingPlane;
    scene.ssao.fsData.screenResolution_nearClippingPlane_farClippingPlane.b = camera.config.nearClippingPlane;
    scene.ssao.fsData.screenResolution_nearClippingPlane_farClippingPlane.a = camera.config.farClippingPlane;
}

void Camera::SetUniformViewMatrix(SceneInfo& scene, CameraInfo& camera)
{
    scene.depthPrepass.vsData.cameraViewMatrix = camera.viewMatrix;
    scene.normalPass.vsData.cameraViewMatrix = camera.viewMatrix;
    scene.modelPack.vsData.view = camera.viewMatrix;
    scene.ssao.fsData.cameraViewMatrix = camera.viewMatrix;
    scene.frustumCulling.uniformData.planes = camera.viewFrustumPlanes;
    scene.occlusionCulling.uniformData.cameraViewMatrix = camera.viewMatrix;
    
    for(size_t i = 0; i < scene.skyboxPack.skyboxes.size(); i++)
        scene.skyboxPack.skyboxes[i].vsData.view = camera.viewMatrix;
    
    for(size_t i = 0; i < scene.lightPack.numLights; i++)
        scene.lightPack.lightCulling.lightBoundingSpheresViewSpace[i] = glm::vec4(glm::vec3(camera.viewMatrix * glm::vec4(scene.lightPack.lights[i].boundingSphere.x, scene.lightPack.lights[i].boundingSphere.y, scene.lightPack.lights[i].boundingSphere.z, 1.0f)), scene.lightPack.lights[i].boundingSphere.w);
}

void Camera::SetUniformProjectionMatrix(SceneInfo& scene, CameraInfo& camera)
{
    scene.depthPrepass.vsData.cameraProjectionMatrix = camera.projectionMatrix;
    scene.modelPack.vsData.projection = camera.projectionMatrix;
    scene.normalPass.vsData.cameraProjectionMatrix = camera.projectionMatrix;
    scene.ssao.fsData.cameraProjectionMatrix = camera.projectionMatrix;
    scene.ssao.fsData.cameraInverseProjectionMatrix = camera.inverseProjectionMatrix;
    scene.frustumCulling.uniformData.planes = camera.viewFrustumPlanes;
    scene.occlusionCulling.uniformData.cameraProjectionMatrix = camera.projectionMatrix;

    for(size_t i = 0; i < scene.skyboxPack.skyboxes.size(); i++)
        scene.skyboxPack.skyboxes[i].vsData.projection = camera.projectionMatrix;
    
    scene.lightPack.lightCulling.tilesFrustumPlanes = LightCulling::ExtractTilesFrustumPlanes(scene.lightPack.lightCulling.numTiles2D, camera.projectionMatrix);
}

void Camera::SetUniformProjectionViewMatrices(SceneInfo& scene, CameraInfo& camera)
{
    scene.ssao.fsData.cameraViewMatrix = camera.viewMatrix;
    scene.depthPrepass.vsData.cameraViewMatrix = camera.viewMatrix;
    scene.depthPrepass.vsData.cameraProjectionMatrix = camera.projectionMatrix;
    scene.modelPack.vsData.view = camera.viewMatrix;
    scene.modelPack.vsData.projection = camera.projectionMatrix;
    scene.normalPass.vsData.cameraViewMatrix = camera.viewMatrix;
    scene.normalPass.vsData.cameraProjectionMatrix = camera.projectionMatrix;
    scene.ssao.fsData.cameraProjectionMatrix = camera.projectionMatrix;
    scene.ssao.fsData.cameraInverseProjectionMatrix = camera.inverseProjectionMatrix;
    scene.frustumCulling.uniformData.planes = camera.viewFrustumPlanes;
    scene.occlusionCulling.uniformData.cameraViewMatrix = camera.viewMatrix;
    scene.occlusionCulling.uniformData.cameraProjectionMatrix = camera.projectionMatrix;

    for(size_t i = 0; i < scene.skyboxPack.skyboxes.size(); i++)
        scene.skyboxPack.skyboxes[i].vsData.view = camera.viewMatrix;
    
    for(size_t i = 0; i < scene.skyboxPack.skyboxes.size(); i++)
        scene.skyboxPack.skyboxes[i].vsData.projection = camera.projectionMatrix;
    
    LightCulling::UpdateLightBoundingSpheres(scene.lightPack, camera);
    
    scene.lightPack.lightCulling.tilesFrustumPlanes = LightCulling::ExtractTilesFrustumPlanes(scene.lightPack.lightCulling.numTiles2D, camera.projectionMatrix);
}

void Camera::ComputeAxes(CameraInfo& camera)
{
    camera.up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    camera.right = glm::vec4(glm::normalize(glm::cross(glm::vec3(camera.config.direction), glm::vec3(camera.up))), 0.0f);
    camera.up = glm::vec4(glm::normalize(glm::cross(glm::vec3(camera.right), glm::vec3(camera.config.direction))), 0.0f);
}

glm::mat4 Camera::ComputeViewMatrix(glm::vec4 position, glm::vec4 direction, glm::vec4 up)
{
    return glm::lookAt(glm::vec3(position), glm::vec3(position + direction), glm::vec3(up));
}

glm::mat4 Camera::ComputeProjectionMatrix(CameraInfo& camera)
{
    glm::mat4 projectionMatrix = glm::perspective(camera.fovRadians, camera.aspectRatio, camera.config.nearClippingPlane, camera.config.farClippingPlane);
    projectionMatrix[1][1] *= -1;
    
    return projectionMatrix;
}

std::array<glm::vec4, 6> Camera::ComputeViewFrustumPlanes(glm::mat4 projectionViewMatrix)
{
    std::array<glm::vec4, 6> planes;
    planes[0] = glm::vec4(projectionViewMatrix[0][3] + projectionViewMatrix[0][0], projectionViewMatrix[1][3] + projectionViewMatrix[1][0], projectionViewMatrix[2][3] + projectionViewMatrix[2][0], projectionViewMatrix[3][3] + projectionViewMatrix[3][0]);
    planes[1] = glm::vec4(projectionViewMatrix[0][3] - projectionViewMatrix[0][0], projectionViewMatrix[1][3] - projectionViewMatrix[1][0], projectionViewMatrix[2][3] - projectionViewMatrix[2][0], projectionViewMatrix[3][3] - projectionViewMatrix[3][0]);
    planes[2] = glm::vec4(projectionViewMatrix[0][3] + projectionViewMatrix[0][1], projectionViewMatrix[1][3] + projectionViewMatrix[1][1], projectionViewMatrix[2][3] + projectionViewMatrix[2][1], projectionViewMatrix[3][3] + projectionViewMatrix[3][1]);
    planes[3] = glm::vec4(projectionViewMatrix[0][3] - projectionViewMatrix[0][1], projectionViewMatrix[1][3] - projectionViewMatrix[1][1], projectionViewMatrix[2][3] - projectionViewMatrix[2][1], projectionViewMatrix[3][3] - projectionViewMatrix[3][1]);
    planes[4] = glm::vec4(projectionViewMatrix[0][3] + projectionViewMatrix[0][2], projectionViewMatrix[1][3] + projectionViewMatrix[1][2], projectionViewMatrix[2][3] + projectionViewMatrix[2][2], projectionViewMatrix[3][3] + projectionViewMatrix[3][2]);
    planes[5] = glm::vec4(projectionViewMatrix[0][3] - projectionViewMatrix[0][2], projectionViewMatrix[1][3] - projectionViewMatrix[1][2], projectionViewMatrix[2][3] - projectionViewMatrix[2][2], projectionViewMatrix[3][3] - projectionViewMatrix[3][2]);
    
    for(int i = 0; i < 6; i++)
        planes[i] /= glm::length(glm::vec3(planes[i]));

    return planes;
}
