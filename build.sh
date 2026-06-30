clear
clear
clear
clear
clear
clear
clear
clear
clear
clear
clear
clear
clear
clear
clear
clear
clear
clear
clear
clear
make

set -e

GLSLC="/opt/homebrew/bin/glslc"
      
$GLSLC -fshader-stage=vert --target-env=vulkan1.2 --target-spv=spv1.3 ../Resources/Shaders/PBR/PBR.vert -o ../Vejaler.app/Contents/Resources/Shaders/PBR/vert.spv
$GLSLC -fshader-stage=frag --target-env=vulkan1.2 --target-spv=spv1.3 ../Resources/Shaders/PBR/PBR.frag -o ../Vejaler.app/Contents/Resources/Shaders/PBR/frag.spv


$GLSLC ../Resources/Shaders/Skybox/EquirectToCubemap/EquirectToCubemap_vs.vert -o ../Vejaler.app/Contents/Resources/Shaders/Skybox/EquirectToCubemap/EquirectToCubemap_vs.spv
$GLSLC ../Resources/Shaders/Skybox/EquirectToCubemap/EquirectToCubemap_fs.frag -o ../Vejaler.app/Contents/Resources/Shaders/Skybox/EquirectToCubemap/EquirectToCubemap_fs.spv


$GLSLC ../Resources/Shaders/IBL/Irradiance/Irradiance_vs.vert -o ../Vejaler.app/Contents/Resources/Shaders/IBL/Irradiance/Irradiance_vs.spv
$GLSLC ../Resources/Shaders/IBL/Irradiance/Irradiance_fs.frag -o ../Vejaler.app/Contents/Resources/Shaders/IBL/Irradiance/Irradiance_fs.spv


$GLSLC ../Resources/Shaders/IBL/BRDF_IntegrationMap/BRDF_IntegrationMap_vs.vert -o ../Vejaler.app/Contents/Resources/Shaders/IBL/BRDF_IntegrationMap/BRDF_IntegrationMap_vs.spv
$GLSLC ../Resources/Shaders/IBL/BRDF_IntegrationMap/BRDF_IntegrationMap_fs.frag -o ../Vejaler.app/Contents/Resources/Shaders/IBL/BRDF_IntegrationMap/BRDF_IntegrationMap_fs.spv


$GLSLC ../Resources/Shaders/IBL/PreFiltrdEnv/PreFiltrdEnv_vs.vert -o ../Vejaler.app/Contents/Resources/Shaders/IBL/PreFiltrdEnv/PreFiltrdEnv_vs.spv
$GLSLC ../Resources/Shaders/IBL/PreFiltrdEnv/PreFiltrdEnv_fs.frag -o ../Vejaler.app/Contents/Resources/Shaders/IBL/PreFiltrdEnv/PreFiltrdEnv_fs.spv


$GLSLC ../Resources/Shaders/Skybox/Skybox/Skybox_vs.vert -o ../Vejaler.app/Contents/Resources/Shaders/Skybox/Skybox/Skybox_vs.spv
$GLSLC ../Resources/Shaders/Skybox/Skybox/Skybox_fs.frag -o ../Vejaler.app/Contents/Resources/Shaders/Skybox/Skybox/Skybox_fs.spv


$GLSLC ../Resources/Shaders/FrustumCulling/FrustumCulling.comp -o ../Vejaler.app/Contents/Resources/Shaders/FrustumCulling/FrustumCulling.spv

$GLSLC ../Resources/Shaders/LightCulling/LightCulling.comp -o ../Vejaler.app/Contents/Resources/Shaders/LightCulling/LightCulling.spv


$GLSLC ../Resources/Shaders/OcclusionCulling/cs.comp -o ../Vejaler.app/Contents/Resources/Shaders/OcclusionCulling/cs.spv
$GLSLC ../Resources/Shaders/DepthPyramid/cs.comp -o ../Vejaler.app/Contents/Resources/Shaders/DepthPyramid/cs.spv





$GLSLC ../Resources/Shaders/Shadow/Shadow.vert -o ../Vejaler.app/Contents/Resources/Shaders/Shadow/Shadow_vs.spv
$GLSLC ../Resources/Shaders/Shadow/Shadow.frag -o ../Vejaler.app/Contents/Resources/Shaders/Shadow/Shadow_fs.spv


$GLSLC ../Resources/Shaders/NormalPass/NormalPass.vert -o ../Vejaler.app/Contents/Resources/Shaders/NormalPass/vs.spv
$GLSLC ../Resources/Shaders/NormalPass/NormalPass.frag -o ../Vejaler.app/Contents/Resources/Shaders/NormalPass/fs.spv



$GLSLC ../Resources/Shaders/SSAO/SSAO.vert -o ../Vejaler.app/Contents/Resources/Shaders/SSAO/vs.spv
$GLSLC ../Resources/Shaders/SSAO/SSAO.frag -o ../Vejaler.app/Contents/Resources/Shaders/SSAO/fs.spv


$GLSLC ../Resources/Shaders/DepthPrepass/DepthPrepass.vert -o ../Vejaler.app/Contents/Resources/Shaders/DepthPrepass/DepthPrepass_vs.spv


rm -f imgui.ini

cp Vejaler ../Vejaler.app/Contents/MacOS/Vejaler
