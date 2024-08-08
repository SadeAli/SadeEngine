#include <dlfcn.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "plugins/plugin.h"

// Pluginator Data Structure
typedef struct PluginatorData {
    int pluginCount;
    Plugin *subPlugins;
} PluginatorData;

Plugin plugin_init(void);
void update(Plugin *self);
void loadPlugins(Plugin *pluginator, const char *pluginDir);
void loadCompiledPlugins(Plugin *pluginator, const char *pluginDir);

int main() {
    Plugin self = plugin_init();
    self.update(&self);
    
    PluginatorData *data = (PluginatorData *)self.data;
    for (int i = 0; i < data->pluginCount; i++) {
        if (data->subPlugins[i].update) {
            data->subPlugins[i].update(&data->subPlugins[i]);
        }
    }

    // Load compiled plugins
    loadCompiledPlugins(&self, "plugins");
    for (int i = 0; i < data->pluginCount; i++) {
        if (data->subPlugins[i].update) {
            data->subPlugins[i].update(&data->subPlugins[i]);
        }
    }

    // Cleanup
    free(data->subPlugins);
    return 0;
}

Plugin plugin_init(void) {
    const char *pluginDir = "plugins";

    Plugin self = {
        .handle = NULL,
        .name = "pluginator",
        .state = {
            .exitCode = 1,
            .exitRequested = 0,
        },
        .data = malloc(sizeof(PluginatorData)),
        .update = update,
        .exit = NULL,
    };

    if (self.data) {
        PluginatorData *data = (PluginatorData *)self.data;
        data->pluginCount = 0;
        data->subPlugins = NULL;
        loadPlugins(&self, pluginDir);
    }

    return self;
}

void update(Plugin *self) {
    PluginatorData *data = (PluginatorData *)self->data;
    if (!data) return;

    while (!WindowShouldClose()) {
        for (int i = 0; i < data->pluginCount; i++) {
            if (data->subPlugins[i].update) {
                data->subPlugins[i].update(&data->subPlugins[i]);
            }
        }
    }
}

void loadPlugins(Plugin *pluginator, const char *pluginDir) {
    PluginatorData *data = (PluginatorData *)pluginator->data;
    if (!data) return;

    DIR *dp = opendir(pluginDir);
    if (!dp) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dp))) {
        if (strstr(entry->d_name, ".so")) {
            char path[256];
            snprintf(path, sizeof(path), "%s/%s", pluginDir, entry->d_name);

            void *handle = dlopen(path, RTLD_LAZY);
            if (!handle) {
                fprintf(stderr, "Failed to load plugin %s: %s\n", entry->d_name, dlerror());
                continue;
            }

            Plugin (*initFunc)(void) = dlsym(handle, "plugin_init");
            if (!initFunc) {
                fprintf(stderr, "Failed to find plugin_init in %s: %s\n", entry->d_name, dlerror());
                dlclose(handle);
                continue;
            }

            Plugin subPlugin = initFunc();
            subPlugin.handle = handle;

            data->pluginCount++;
            data->subPlugins = realloc(data->subPlugins, data->pluginCount * sizeof(Plugin));
            data->subPlugins[data->pluginCount - 1] = subPlugin;
        }
    }
    closedir(dp);
}

void loadCompiledPlugins(Plugin *pluginator, const char *pluginDir) {
    PluginatorData *data = (PluginatorData *)pluginator->data;
    if (!data) return;

    DIR *dp = opendir(pluginDir);
    if (!dp) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dp))) {
        if (strstr(entry->d_name, ".so")) {
            char path[256];
            snprintf(path, sizeof(path), "%s/%s", pluginDir, entry->d_name);

            void *handle = dlopen(path, RTLD_LAZY);
            if (!handle) {
                fprintf(stderr, "Failed to load plugin %s: %s\n", entry->d_name, dlerror());
                continue;
            }

            Plugin (*initFunc)(void) = dlsym(handle, "plugin_init");
            if (!initFunc) {
                fprintf(stderr, "Failed to find plugin_init in %s: %s\n", entry->d_name, dlerror());
                dlclose(handle);
                continue;
            }

            Plugin subPlugin = initFunc();
            subPlugin.handle = handle;

            data->pluginCount++;
            data->subPlugins = realloc(data->subPlugins, data->pluginCount * sizeof(Plugin));
            data->subPlugins[data->pluginCount - 1] = subPlugin;
        }
    }
    closedir(dp);
}
