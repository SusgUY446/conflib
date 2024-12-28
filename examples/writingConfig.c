#include "conflib.h"
#include <stdio.h>

int main() {
    Config* config = loadConfig("simple.ini");

    if (config == NULL) {
        fprintf(stderr, "Error loading configuration file\n");
        return -1;
    }

    
    if (setValue(config, "host", "newhost.example.com") == 0) {
        printf("Updated 'host' key successfully.\n");
    } else {
        fprintf(stderr, "Error: Key 'host' not found.\n");
    }

    if (saveConfig(config, "simple.ini") == 0) {
        printf("Configuration saved successfully.\n");
    } else {
        fprintf(stderr, "Error saving configuration.\n");
    }

    freeConfig(config);

    return 0;
}
