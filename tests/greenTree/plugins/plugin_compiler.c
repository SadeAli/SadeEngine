// plugins/compiler_plugin.c

#include "plugin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

void compile(Plugin *plugin);

Plugin plugin_init(void) {
    Plugin self = {
        .handle = NULL,
        .name = "compiler_plugin",
        .state = {
            .exitRequested = 0,
            .exitCode = 0,
        },
        .update = compile,
        .exit = NULL,
        .data = NULL,
    };

    return self;
}

void compile(Plugin *plugin) {
    const char *pluginDir = "plugins";
    DIR *dp = opendir(pluginDir);
    if (!dp) {
        perror("opendir");
        plugin->state.exitCode = 1;
        plugin->state.exitRequested = 1;
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dp))) {
        if (strstr(entry->d_name, ".c")) {
            char srcPath[256];
            char soPath[256];
            snprintf(srcPath, sizeof(srcPath), "%s/%s", pluginDir, entry->d_name);
            snprintf(soPath, sizeof(soPath), "%s/%s.so", pluginDir, entry->d_name);
            soPath[strlen(soPath) - 2] = '\0';  // Remove .c extension

            char cmd[512];
            snprintf(cmd, sizeof(cmd), "gcc -shared -o %s -fPIC %s -lraylib", soPath, srcPath);
            int result = system(cmd);
            if (result != 0) {
                fprintf(stderr, "Failed to compile %s\n", srcPath);
            } else {
                printf("Compiled %s to %s\n", srcPath, soPath);
            }
        }
    }
    closedir(dp);
}
