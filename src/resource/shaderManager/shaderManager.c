#include "dynamicArray/dynamicArray.h"
#include "shader.h"
#include "string.h"
#include "dynamicArray.h"

typedef struct EngineShader_t EngineShader;
struct EngineShader_t {
    ShaderProgram shaderProgram;

    unsigned int subShaderCount;
    Shader *subShaders;
};

static struct {
    unsigned int numShader;
    Shader *shaders;

    unsigned int numShaderProgram;
    ShaderProgram *shaderPrograms;
} shaderManager = {0};

static EngineShader *loadedShaders = nullptr;

bool shaderManager_AppendShader(Shader shader) {
    return array_appendDynamic((void **)&shaderManager.shaders,
                        &shaderManager.numShader,
                        &shaderManager.numShader,
                        sizeof(Shader),
                        &shader,
                        1);
}

bool shaderManager_AppendShaderProgram(ShaderProgram program) {
    return array_appendDynamic((void **)&shaderManager.shaderPrograms,
                            &shaderManager.numShaderProgram,
                            &shaderManager.numShaderProgram,
                            sizeof(ShaderProgram),
                            &program,
                            1);
}

Shader loadShader(const char *path, ShaderType shaderType) {
    // TODO: if shader previously loaded return loaded shader
    /*
    for (unsigned int i = 0; i < shaderManager.numShader; i++) {
        if (strcmp(path, shaderManager.shaderPaths[i])) {
            return shaderManager.shaders[i];
        }
    }
    */

    // if shader not loaded load the shader
    Shader s = construct_shader(path, shaderType);
    shaderManager_AppendShader(s);
    return s;
}

bool loadAllShaders(const char *dir);

bool saveShaderProgramList(const char *dir);
bool loadShaderProgramList(const char *dir);

// NOTE: requires shader to be loaded before being called
Shader getShader(const char *name);
unsigned int getShaderIndex(const char *name);
