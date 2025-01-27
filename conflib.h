//============================================================================================//
//                                     (C) 2025 SusgUY446                                     //
//============================================================================================//
// Conflib - Simple library for loading and saving configurations in C                        //
//============================================================================================//
//                                            LICENSE                                         //
//                                    4-Clause BSD NON-AI License                             //
//                                      Copyright 2025 SusgUY446                              //
//                                                                                            //
// Redistribution and use in source and binary forms, with or without modification,           //
// are permitted provided that the following conditions are met:                              //
//                                                                                            //
// 1. Redistributions of source code must retain the above copyright notice, this list of     //
// conditions and the following disclaimer.                                                   //
//                                                                                            //
// 2. Redistributions in binary form must reproduce the above copyright notice, this list     //
// of conditions and the following disclaimer                                                 //
// in the documentation and/or other materials provided with the distribution.                //
//                                                                                            //
// 3. Neither the name of the copyright holder nor the names of its contributors may be used  //
// to endorse or promote products                                                             //
// derived from this software without specific prior written permission.                      //
//                                                                                            //
// 4. The source code and the binary form, and any modifications made to them may not be used //
// for the purpose of training or improving machine learning algorithms,                      //
// including but not limited to artificial intelligence, natural language processing, or data //
// mining. This condition applies to any derivatives,                                         //
// modifications, or updates based on the Software code. Any usage of the source code or the  //
// binary form in an AI-training dataset is considered a breach of this License.              //
//                                                                                            //
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY        //
// EXPRESS OR IMPLIED WARRANTIES,                                                             //
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR   //
// A PARTICULAR PURPOSE ARE DISCLAIMED.                                                       //
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, //
// INCIDENTAL, SPECIAL, EXEMPLARY,                                                            //
// OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS   //
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS;                                                //
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN        //
// CONTRACT, STRICT LIABILITY,                                                                //
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS      //
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                               //
//============================================================================================//


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
    char *key;         // The name of the key 
    char *value;       // The value of the key
} ConfigEntry;

typedef struct Config {
    ConfigEntry *entries;       // All the entries in the config
    size_t count;               // Amount of entries in the config
} Config;

// Function declarations

/* Load Config into memory  */
Config* loadConfig(const char* filename); 

/* Free config from memory  */
void freeConfig(Config* config);

/* Get the value of a key from a config  */
char* getValue(Config* config, const char* key);


// Internal function. Not exposed to the user
static int _parseLine(char* line, char** key, char** value);


/* Set the value of a key from a config  */
int setValue(Config *config, const char *key, const char *newValue);

/* Save a config to file  */
int saveConfig(Config* config, const char* filename);



#ifdef CONFLIB_IMPL


// Function definitons

Config* loadConfig(const char* filename) {
    // Open the file 
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Couldn not open config file\n");
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
        fclose(file);
        perror("Memory allocation failed for Config\n");
        return NULL;
    }
    conf->entries = malloc(sizeof(ConfigEntry) * count);
    if (conf->entries == NULL) {
        free(conf);
        fclose(file);
        perror("Memory allocation failed for Config entries\n");
        return NULL;
    }
    conf->count = 0;

    rewind(file);
    while (fgets(line, sizeof(line), file)) {
        char *key;
        char *value;
        if (_parseLine(line, &key, &value) == 0) {
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
        perror("Could not open config file\n");
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
