#include "IBL.h"



void IBL::Build(ContextInfo& context, SkyboxInfo& skybox)
{
    std::thread brdfIntegrationThread = std::thread([&](){
        BRDFIntegrationMap::Build(context, skybox.ibl.brdfIntegrationMap, VkExtent2D{.width = 512, .height = 512});
    });
    
    
    std::thread irradianceThread = std::thread([&](){
        Irradiance::Build(context, skybox.ibl.irradiance, IrradianceConfig{
            .resolution = VkExtent2D{.width = 512, .height = 512},
            .skyboxTexturePack = skybox.skyboxTexturePack
        });
    });
    
    
    std::thread preFilteredEnvironmentThread = std::thread([&](){
        PreFilteredEnvironment::Build(context, skybox.ibl.preFilteredEnvironment, PreFilteredEnvironmentConfig{
            .resolution = VkExtent2D{.width = 512, .height = 512},
            .skyboxTexture = skybox.skyboxTexturePack
        });
    });
    
    
    irradianceThread.join();
    brdfIntegrationThread.join();
    preFilteredEnvironmentThread.join();
}

void IBL::Record(ContextInfo& context, SkyboxInfo& skybox)
{
    std::thread brdfIntegrationThread = std::thread([&](){
        BRDFIntegrationMap::Record(context, skybox.ibl.brdfIntegrationMap);
    });
    
    
    std::thread irradianceThread = std::thread([&](){
        Irradiance::Record(context, skybox.ibl.irradiance);
    });
    
    
    std::thread preFilteredEnvironmentThread = std::thread([&](){
        PreFilteredEnvironment::Record(context, skybox.ibl.preFilteredEnvironment);
    });
    
    
    irradianceThread.join();
    brdfIntegrationThread.join();
    preFilteredEnvironmentThread.join();
}

void IBL::Submit(ContextInfo& context, SkyboxInfo& skybox)
{
    Irradiance::Submit(context, skybox.ibl.irradiance, skybox.hdrToCubemapIrradianceSemaphore);
    
    BRDFIntegrationMap::Submit(context, skybox.ibl.brdfIntegrationMap);
    
    PreFilteredEnvironment::Submit(context, skybox.ibl.preFilteredEnvironment, skybox.hdrToCubemapPreFilteredEnvironmentSemaphore);
}
