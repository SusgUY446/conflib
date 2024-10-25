/*
Configlib.h 0.1 by SusgUY446 (https://github.com/susguy446)


LICENSE: GPL 3.0 (see LICENSE)
*/



// defines
#define MAX_LINE_LENGTH 256
#define MAX_KEY_LENGTH 128
#define MAX_VALUE_LENGTH 128

// Includes 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




// types and structs

typedef struct ConfigEntry {
    char *key;
    char *value;
} ConfigEntry;

typedef struct Config {
    ConfigEntry *entries;
    size_t count;
} Config;


// function declarations

Config* loadConfig(const char* filename, int entrieCount);
void freeConfig(Config* config);
char* getValue(Config* config, const char* key);
int parseLine(char* line, char** key, char** value);



#define CONFIGLIB_IMPL

#ifdef CONFIGLIB_IMPL

    Config* loadConfig(const char* filename, int entrieCount) {
        FILE* file = fopen(filename, "r");
        if (file == NULL) {
            fprintf(stderr, "Could not open config file");
        }

        Config* conf = malloc(sizeof(Config));
        conf->entries = malloc(sizeof(ConfigEntry) * entrieCount);
        conf->count = 0;

        char line[MAX_LINE_LENGTH];

        while (fgets(line, sizeof(line), file)) {
            char *key;
            char *value;

            if (parseLine(line, &key, &value) == 0) {
                conf->entries[conf->count].key = strdup(key);
                conf->entries[conf->count].value = strdup(value);
                conf->count++;
            }
        }
        
        fclose(file);
        return conf;
    }


    int parseLine(char* line, char** key, char** value) {
        if (line[0] == ';' || line[0] == '#' || strlen(line)<3) {
            return -1;
        }

        char *equals = strchr(line, '=');
        if(equals == NULL) {
            return -1;
        }

        *equals = '\0';
        *key = strdup(line);
        *value = strdup(equals+1);
        (*value)[strcspn(*value, "\r\n")] = 0;
        return 0;
    }

    void free_config(Config *config) {
        for (size_t i = 0; i < config->count; i++) {
            free(config->entries[i].key);
            free(config->entries[i].value);
        }
        free(config->entries);
        free(config);
    }

#endif //CONFIGLIB_IMPL