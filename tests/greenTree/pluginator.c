// main.c

#include <dlfcn.h>
#include <raylib.h>
#include <stdio.h>

#include "plugins/plugin.h"

// pluginator.c

typedef struct PluginatorData
{
    int pluginCount;
    Plugin *subPlugins;
} PluginatorData;

Plugin plugin_init()
{
    char *pluginDir = "plugins";

    Plugin self = {
        .handle = NULL,

        .name = "pluginator",

        .state =
            {
                .exitCode = 1,
                .exitRequested = 0,
            },

        .data =
            &(PluginatorData){
                .pluginCount = 1,
                .subPlugins =
                    (Plugin *){
                        
                    },
            },
    };

    return self;
}

void update(Plugin *self)
{
    PluginatorData pd = *(PluginatorData *)self->data;

    for (int i = 0; i < pd.pluginCount; i++)
    {
        pd.subPlugins[i].update(&pd.subPlugins[i]);
    }
}

void exit(Plugin *self)
{
    PluginatorData pd = *(PluginatorData *)self->data;

    for (int i = 0; i < pd.pluginCount; i++)
    {
        pd.subPlugins[i].exit(&pd.subPlugins[i]);
    }
}