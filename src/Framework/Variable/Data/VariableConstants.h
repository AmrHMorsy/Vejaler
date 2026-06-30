#pragma once

#define STRING 0
#define INT 1
#define FLOAT 2




inline std::unordered_map<std::string, int> VariableTypes{
    {"string", STRING},
    {"int", INT},
    {"float", FLOAT}
};
