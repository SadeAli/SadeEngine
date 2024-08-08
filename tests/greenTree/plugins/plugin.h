#pragma once

// plugins/plugin.h

#include <stdbool.h>

#define CANT_INITIALIZE_PLUGIN 1

typedef struct PluginState
{
    bool exitRequested : 1;
    int exitCode;
} PluginState;

typedef struct Plugin
{
    void *handle;
    const char *name;
    PluginState state;
    void *data;

    void (*update)(struct Plugin *);
    void (*exit)(struct Plugin *);
} Plugin;

typedef struct SubPluginList
{
    Plugin *plugins;
    int pluginCount;
} SubPluginList;

Plugin plugin_init(void);