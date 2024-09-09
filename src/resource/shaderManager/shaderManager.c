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
    char **pathShader;
    char **nameShader;
    Shader *shaders;

    char **nameShaderProgram;
    ShaderProgram *shaderPrograms;

    unsigned int numShader;
    unsigned int numShaderProgram;
} shaderManager = {0};

static EngineShader *loadedShaders = nullptr;

bool shaderManager_AppendShader(Shader shader) {
    return array_appendAndExpand((void **)&shaderManager.shaders,
                        &shaderManager.numShader,
                        &shaderManager.numShader,
                        sizeof(Shader),
                        &shader,
                        1);
}

bool shaderManager_AppendShaderProgram(ShaderProgram program) {
    return array_appendAndExpand((void **)&shaderManager.shaderPrograms,
                            &shaderManager.numShaderProgram,
                            &shaderManager.numShaderProgram,
                            sizeof(ShaderProgram),
                            &program,
                            1);
}

Shader shaderManager_loadShaderFromPath(const char *path, ShaderType shaderType) {
    // TODO: if shader previously loaded return loaded shader
    /*
    for (unsigned int i = 0; i < shaderManager.numShader; i++) {
        if (strcmp(path, shaderManager.shaderPaths[i])) {
            return shaderManager.shaders[i];
        }
    }
    */

    // if shader not loaded load the shader
    Shader s = construct_shaderFromFile(path, shaderType);
    shaderManager_AppendShader(s);
    return s;
}

char **shaderManager_getShaderList();

bool shaderManager_loadAllShaders(const char *dir);

bool shaderManager_saveShaderProgramList(const char *dir);
bool shaderManager_loadShaderProgramList(const char *dir);

// NOTE: requires shader to be loaded before being called
Shader shaderManager_getShader(const char *name);
unsigned int shaderManager_getShaderIndex(const char *name);
