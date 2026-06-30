#include "Scissor.h"



void Scissor::SetScissor(SetScissorConfiguration configuration)
{
    vkCmdSetScissor(configuration.commandBuffer, configuration.offset, static_cast<uint32_t>(configuration.scissors.size()), configuration.scissors.data()) ;
}
