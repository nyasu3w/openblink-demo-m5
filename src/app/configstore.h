
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

struct config_item_data{
    uint8_t size;
    uint8_t* data; 
};

struct config_item {
    const char* key;  // asciz-key
    struct config_item_data data;
};

bool config_data_load(void);
bool config_data_set(const char* key, const uint8_t* data, uint8_t size);
struct config_item_data* config_data_get(const char* key);
bool config_data_save(void);

int config_data_get_int(const char* key);

bool config_data_set_int(const char* key, int value);
bool config_data_set_string(const char* key, const uint8_t* str, size_t size);

void config_data_debug(void);

#ifdef __cplusplus
}
#endif