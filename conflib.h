/*
Configlib.h 1.0 by SusgUY446 (https://github.com/susguy446)

LICENSE: GPL 3.0 (see LICENSE)
*/

// Defines
#define MAX_LINE_LENGTH 256
#define MAX_KEY_LENGTH 128
#define MAX_VALUE_LENGTH 128

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Types and structs
typedef struct ConfigEntry {
    char *key;
    char *value;
} ConfigEntry;

typedef struct Config {
    ConfigEntry *entries;
    size_t count;
} Config;

// Function declarations
Config* loadConfig(const char* filename);
void freeConfig(Config* config);
char* getValue(Config* config, const char* key);
int parseLine(char* line, char** key, char** value);


int setValue(Config *config, const char *key, const char *newValue);
int saveConfig(Config* config, const char* filename);


#define CONFIGLIB_IMPL

#ifdef CONFIGLIB_IMPL

Config* loadConfig(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open config file\n");
        return NULL;
    }

    size_t count = 0;
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file)) {
        char* equals = strchr(line, '=');
        if (equals != NULL && line[0] != ';' && line[0] != '#' && strlen(line) > 3) {
            count++;
        }
    }

    Config* conf = malloc(sizeof(Config));
    if (conf == NULL) {
        fprintf(stderr, "Memory allocation failed for Config\n");
        fclose(file);
        return NULL;
    }
    conf->entries = malloc(sizeof(ConfigEntry) * count);
    if (conf->entries == NULL) {
        fprintf(stderr, "Memory allocation failed for Config entries\n");
        free(conf);
        fclose(file);
        return NULL;
    }
    conf->count = 0;

    rewind(file);
    while (fgets(line, sizeof(line), file)) {
        char *key;
        char *value;
        if (parseLine(line, &key, &value) == 0) {
            conf->entries[conf->count].key = strdup(key);
            conf->entries[conf->count].value = strdup(value);
            free(key);
            free(value);
            conf->count++;
        }
    }

    fclose(file);
    return conf;
}



int saveConfig(Config* config, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Could not open config file\n");
        return -1;
    }

    for (size_t i = 0; i < config->count; i++) {
        if (config->entries[i].key && config->entries[i].value) {
            fprintf(file, "%s=%s\n", config->entries[i].key, config->entries[i].value);
        }
    }

    fclose(file);
    return 0;
}

int setValue(Config *config, const char *key, const char *newValue) {
    for (size_t i = 0; i < config->count; i++) {
        if (strcmp(config->entries[i].key, key) == 0) {
            free(config->entries[i].value); 
            config->entries[i].value = strdup(newValue); 
            return 0;
        }
    }
    return -1;
}

int parseLine(char* line, char** key, char** value) {
    if (line[0] == ';' || line[0] == '#' || strlen(line) < 3) {
        return -1;
    }

    char *equals = strchr(line, '=');
    if (equals == NULL) {
        return -1;
    }

    *equals = '\0';  
    *key = strdup(line);  
    *value = strdup(equals + 1);  
    (*value)[strcspn(*value, "\r\n")] = 0;  
    return 0;
}

char* getValue(Config *config, const char *key) {
    for (size_t i = 0; i < config->count; i++) {
        if (strcmp(config->entries[i].key, key) == 0) {
            return config->entries[i].value;
        }
    }
    return NULL;
}

void freeConfig(Config *config) {
    for (size_t i = 0; i < config->count; i++) {
        free(config->entries[i].key);
        free(config->entries[i].value);
    }
    free(config->entries);
    free(config);
}




#endif //CONFIGLIB_IMPL
