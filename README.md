# Conflib
A lightweight single header C library








## Using Conflib
Get started using Conflib by looking into the examples folder. 

## Example

```c
#include <conflib.h>
#include <stdio.h>

int main() {
    Config* config = loadConfig("simple.ini", 5);

    if (config == NULL) {
        fprintf(stderr, "Error Opening Config File\n");
        return -1;
    }

    char* db_hostname = getValue(config, "host");
    char* db_port = getValue(config, "port");
    char* db_username = getValue(config, "username");
    char* db_password = getValue(config, "password");
    char* debug_mode = getValue(config, "debug");



    printf("Database Host: %s\n", db_hostname);
    printf("Database Port: %s\n", db_port);
    printf("Database Username: %s\n", db_username);
    printf("Database Password: %s\n", db_password);
    printf("Debug Mode: %s\n", debug_mode);


    freeConfig(config);


    return 0;
}
```



## Supported Formats
Currently Conflib only supports the INI Format but we will be adding more formats



## Support
Need help with Conflib?. Just open a issue or dm me on discord (@susguy446)