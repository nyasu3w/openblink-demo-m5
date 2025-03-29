#include <nvs.h>
#include <nvs_flash.h>

#include <string.h>
#include "configstore.h"

#define CONFIG_DATA_SIZE 512
#define CONFIG_DATA_NUM 16

#define CONFIG_BLOCK_NAME "blink_config"

/* stored data format 
  asciz-key1(uint8_t[]),size(uint8_t),data1(uint8_t[]),  asciz-key2,size,data2,...,0,0,0

  loaded data format
  struct {
    uint8_t* key,  // asciz-key
    uint8_t size,
    uint8_t* data,}
*/

static uint8_t config_loaded_data[CONFIG_DATA_SIZE] = {0};
static struct config_item loaded_data[CONFIG_DATA_NUM]; 


bool config_data_load(void) {
    nvs_handle_t handle;
    size_t length = CONFIG_DATA_SIZE;
    loaded_data[0].key = NULL;

    if (ESP_OK != nvs_open(CONFIG_BLOCK_NAME, NVS_READONLY, &handle)) {
        printf("NVS open failed\n");
        return false;
    }
    //size check
    if (ESP_OK != nvs_get_blob(handle, "data", NULL, &length)) {
        nvs_close(handle);
        printf("NVS get size failed\n");
        return false;
    }
    if (length > CONFIG_DATA_SIZE) {
        nvs_close(handle);
        printf("NVS data size too large\n");
        return false;
    }
    // load
    if (ESP_OK != nvs_get_blob(handle, "data", config_loaded_data, &length)) {
      nvs_close(handle);
      printf("NVS get data failed\n");
      return false;
    }
    nvs_close(handle);

/*
    printf("Loaded data size: %d\n bin:", length);
    for(int i=0; i < length; i++) {
        printf("%02x:", config_loaded_data[i]);
    }
    printf("\n");
*/

    printf("Loaded Config from NVS\n");
    // make loaded_data
    uint8_t* p = config_loaded_data;
    uint8_t i = 0;
    while (i < CONFIG_DATA_NUM && *p != 0) {
        printf("loading key: %s\n",p);
        loaded_data[i].key = (char*) p;
        p += strlen((char*)p) + 1; // move to next key
        loaded_data[i].data.size = *p++;
        loaded_data[i].data.data = p;
        p += loaded_data[i].data.size;
        i++;
        if (*p == 0) {
            break;
        }
    }
    if(i<CONFIG_DATA_NUM) {
        loaded_data[i].key = NULL; // end marker
    }

    printf("#%d item loaded\n", i);
    return true;
}

void config_data_debug() {
    printf("DUMP: loaded_data@ %lx\n", (long)&loaded_data[0]);
    for (int i = 0; i < CONFIG_DATA_NUM; i++) {
        if (loaded_data[i].key == NULL) {
            break;
        }
        printf("[%d](%d): ",i, strlen((char*)loaded_data[i].key));
        for(int j=0; j < strlen((char*)loaded_data[i].key); j++) {
            printf("%02x ", loaded_data[i].key[j]);
        }
        printf("key=%s[%x], size=%d\n", loaded_data[i].key, (uintptr_t)loaded_data[i].key, loaded_data[i].data.size);
        printf("dump[%d] key=%s[%x], size=%d\n", i, loaded_data[i].key, (uintptr_t)loaded_data[i].key, loaded_data[i].data.size);
        for (int j = 0; j < loaded_data[i].data.size; j++) {
            printf("%02x ", loaded_data[i].data.data[j]);
        }
        printf("\n");
    }
}

struct config_item_data* config_data_get(const char* key) {
    for (int i = 0; i < CONFIG_DATA_NUM; i++) {
        if (loaded_data[i].key == NULL) {
            break;
        }

        if (strcmp((char*)loaded_data[i].key, key) == 0) {
            return &loaded_data[i].data;
        }
    }
    return NULL;
}

bool config_data_set(const char* key, const uint8_t* data, uint8_t size) {
    //search for key to replace or add
    for (int i = 0; i < CONFIG_DATA_NUM; i++) {
        if (loaded_data[i].key == NULL) {
            //add new key
            loaded_data[i].key = (char*)malloc(strlen(key) + 1);
            if (loaded_data[i].key == NULL) {
                return false;
            }
            strcpy((char*)loaded_data[i].key, key);
            loaded_data[i].data.size = size;
            loaded_data[i].data.data = (uint8_t*)malloc(size);  // note: not freed even when data is deleted or reloaded from nvs
            if (loaded_data[i].data.data == NULL) {
                return false;
            }
            memcpy(loaded_data[i].data.data, data, size);
            if(i+1 < CONFIG_DATA_NUM) {
                loaded_data[i+1].key = NULL; // end marker
            }
            break;
        }
        if (strcmp((char*)loaded_data[i].key, key) == 0) {
            //replace
            // check if size is enough
            if (loaded_data[i].data.size < size) {
                free(loaded_data[i].data.data);
                loaded_data[i].data.data = (uint8_t*)malloc(size);
                if (loaded_data[i].data.data == NULL) {
                    return false;
                }
            }
            loaded_data[i].data.size = size;
            memcpy(loaded_data[i].data.data, data, size);
            return true;
        }
    }
    return false;
}

int config_data_get_int(const char* key) {
    struct config_item_data* data = config_data_get(key);
    if (data == NULL) {
        return 0;
    }
    if (data->size != sizeof(int)) {
        return 0;
    }
    return *(int*)data->data;
}

bool config_data_set_int(const char* key, int value) {
    return config_data_set(key, (uint8_t*)&value, sizeof(int));
}
bool config_data_set_string(const char* key, const uint8_t* str, size_t size) {
    return config_data_set(key, (uint8_t*)str, size);
}

bool config_data_save(void) {
    nvs_handle_t handle;
    size_t length = 0;
    uint8_t config_save_data[CONFIG_DATA_SIZE] = {0};
    uint8_t* p = config_save_data;
    
    // make data
    for (int i = 0; i < CONFIG_DATA_NUM; i++) {
        if (loaded_data[i].key == NULL) {
            break;
        }

        length += strlen((char*)loaded_data[i].key) + 1; // key
        length += loaded_data[i].data.size + 1; // size + data
        memcpy(p, loaded_data[i].key, strlen((char*)loaded_data[i].key) + 1);
        p += strlen((char*)loaded_data[i].key) + 1;
        *p++ = loaded_data[i].data.size;
        memcpy(p, loaded_data[i].data.data, loaded_data[i].data.size);
        p += loaded_data[i].data.size;
    }
    *p++ = 0; // end marker
    length += 1; // end marker

    if (ESP_OK != nvs_open(CONFIG_BLOCK_NAME, NVS_READWRITE, &handle)) {
        return false;
      }

/*
    printf("saving data dump[%d]\n",length);
    for (int i = 0; i < length; i++) {
        printf("%02x:", config_save_data[i]);
    }  
    printf("\n");
*/

    // save
    if (ESP_OK != nvs_set_blob(handle, "data", config_save_data, length)) {
      printf("NVS set data failed\n");
      nvs_close(handle);
      return false;
    }
    if(ESP_OK != nvs_commit(handle)) {
        printf("NVS commit failed\n");
        nvs_close(handle);
        return false;
    }
    nvs_close(handle);
    return true;
}

bool config_data_clear() {
  loaded_data[0].key = NULL;
  return true;
}
