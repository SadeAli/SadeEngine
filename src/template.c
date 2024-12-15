// stdlib
#include <assert.h>

// vector/matrix operations
#include <cfloat>
#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include <cglm/affine.h>
#include <cglm/affine-pre.h>
#include <cglm/cam.h>
#include <cglm/clipspace/persp_lh_no.h>
#include <cglm/util.h>

// image loader
#include <stb/stb_image.h>

// opengl loader
#include <glad/glad.h>
#include <stdio.h>

// GUI libs
#include <imgui.h>

// File system
#include <dirent.h>

// model loader libs
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>

// custom engine
#include "window.h"
#include "inputMap.h"
#include "gui.h"
#include "shader.h"
#include "render/drawable.h"
#include "defines.h"
#include "camera3d.h"
#include "glShapes.h"
#include "math/vec.h"
#include "modelLoader.h"

struct Vector2_t {
    float x, y;
};

struct Engine
{
    WindowSettings windowSettings;
} engine = {
    .windowSettings =
        {
            .title = (char *)"myLeaf",
            .width = 1920,
            .height = 1080,
            .fps = 60,
        },
};
//
// TODO: make a verrtex struct which can hold various attributes (attributes will be bynamically added (before making it concreate))

// IDEA: every object has its own hierarchy and objects will only hold indices to child objects
// TODO: systems

int main(void) {
    Window window = init_windowDefault();
    bool isGuiOpen;

    char *shaderDir = (char *)"resources/shaders/";

    DIR *dir;
    
    while (!window_shouldClose(window)) {
        // render settings
        {
            glClearColor(105 / 255.0, 18 / 255.0, 18 / 255.0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        // render objects here
        {
            using namespace ImGui;
            guiBegin();

            /* demo window */
            ShowDemoWindow();

            /* shader lister */
            Begin("shaders");
            ImGui::SetWindowFontScale(1.5);
            if (BeginListBox("##files", (ImVec2){-1,-1})) {
                DIR *d;
                struct dirent *dir;
                d = opendir(shaderDir);
                if (d) {
                    while ((dir = readdir(d)) != NULL) {
                        Text("%s", dir->d_name);
                    }
                closedir(d);
                }
                EndListBox();
            }
            ImGui::End();

            guiEnd();
        }


        // end frame
        {
            window_swapBuffers(&window);
            window_pollEvents();
        }
    }

    window_close(&window);
}


