const glm::vec3 cubemapDirections[6] = {
    {  1,  0,  0},   // +X
    { -1,  0,  0},   // -X
    {  0,  1,  0},   // +Y
    {  0, -1,  0},   // -Y
    {  0,  0,  1},   // +Z
    {  0,  0, -1}    // -Z
};

const glm::vec3 cubemapUpVectors[6] = {
    { 0, -1,  0},    // +X
    { 0, -1,  0},    // -X
    { 0,  0,  1},    // +Y
    { 0,  0, -1},    // -Y
    { 0, -1,  0},    // +Z
    { 0, -1,  0}     // -Z
};

const std::string lightModelFilePath = "../Resources/assets/LightBulb/LightBulb.obj";

const std::array<float, 3> minPoissonSampling{-1.0f, -1.0f, -1.0f};

const std::array<float, 3> maxPoissonSampling{1.0f, 1.0f, 1.0f};

const std::array<float, 3> initialSample{0.0f, 0.0f, 0.0f};
