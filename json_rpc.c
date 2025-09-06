// File: json_rpc.c
// Module: JSON-RPC 2.0 Parser
// Description: Implements JSON parsing utilities for JSON-RPC 2.0 format

#include "json_rpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON/cJSON.h"


int check_rpc_version(cJSON* root, const char* version) {

    int is_valid_rpc_version = 1;

    // get json rpc attribute
    cJSON* item_jsonrpc = cJSON_GetObjectItemCaseSensitive(root, "jsonrpc");
    
    for (int i = 0; i < 1; i++)
    {
        if (item_jsonrpc == NULL || !cJSON_IsString(item_jsonrpc)) {
            is_valid_rpc_version = 0;
            continue;
        }
        if (item_jsonrpc->valuestring == NULL) {
            is_valid_rpc_version = 0;
            continue;
        }
        if (strcmp(item_jsonrpc->valuestring, version) != 0) {
            is_valid_rpc_version = 0;
            continue;
        } 
    }

    return is_valid_rpc_version;
}

int is_valid_rpc_event(cJSON* root)
{
    int is_valid_rpc_event = 1;

    if (!check_rpc_version(root, "2.0")) {
        return 0;
    }
    
    // get rpc event attribute
    cJSON* item_id = cJSON_GetObjectItemCaseSensitive(root, "id");
    cJSON* item_result = cJSON_GetObjectItemCaseSensitive(root, "result");
    cJSON* item_error = cJSON_GetObjectItemCaseSensitive(root, "error");
    cJSON* item_error_code = NULL;
    cJSON* item_error_message = NULL;

    for (int i = 0; i < 1; i++) {
        if (item_id == NULL || !cJSON_IsNumber(item_id)) {
            is_valid_rpc_event = 0;
            continue;
        }
        if (item_result == NULL && item_error == NULL) {
            is_valid_rpc_event = 0;
            continue;
        }
        if (item_result != NULL) {
            if (!cJSON_IsObject(item_result)) {
                is_valid_rpc_event = 0;
                continue;
            }
        }
        if (item_error != NULL) {
            item_error_code = cJSON_GetObjectItemCaseSensitive(item_error, "code");
            if (item_error_code == NULL || !cJSON_IsNumber(item_error_code)) {
                is_valid_rpc_event = 0;
                continue;
            }
            item_error_message = cJSON_GetObjectItemCaseSensitive(item_error, "message");
            if (item_error_message == NULL || !cJSON_IsString(item_error_message)) {
                is_valid_rpc_event = 0;
                continue;
            }
        }

    }

    return is_valid_rpc_event;
}

int is_valid_rpc_request(cJSON* root)
{
    int is_valid_rpc_request = 1;

    if (!check_rpc_version(root, "2.0")) {
        return 0;
    }

    // get rpc request attribute
    cJSON* item_id = cJSON_GetObjectItemCaseSensitive(root, "id");
    cJSON* item_method = cJSON_GetObjectItemCaseSensitive(root, "method");
    cJSON* item_params = cJSON_GetObjectItemCaseSensitive(root, "params");

    for (int i = 0; i < 1; i++) {
        if (item_id == NULL || !cJSON_IsNumber(item_id)) {
            is_valid_rpc_request = 0;
            continue;
        }
        if (item_method == NULL || !cJSON_IsString(item_method)) {
            is_valid_rpc_request = 0;
            continue;
        }
        if (item_params == NULL || !cJSON_IsObject(item_params)) {
            is_valid_rpc_request = 0;
            continue;
        }
    }

    return is_valid_rpc_request;
}

int parse_rpc_event(cJSON* root, rpc_event_t* rpc_event_out, cJSON** result_out)
{
    if (!is_valid_rpc_event(root)) {
        return 0;
    }
    
    cJSON* item_id = cJSON_GetObjectItemCaseSensitive(root, "id");
    cJSON* item_error = cJSON_GetObjectItemCaseSensitive(root, "error");
    cJSON* item_error_code = NULL;
    cJSON* item_error_message = NULL;

    if (rpc_event_out != NULL) {
        rpc_event_out->id = item_id->valueint;
        if (item_error != NULL) {
            rpc_event_out->success = 0;
            item_error_code = cJSON_GetObjectItemCaseSensitive(item_error, "code");
            item_error_message = cJSON_GetObjectItemCaseSensitive(item_error, "message");
            rpc_event_out->error_code = item_error_code->valueint;
            int size_buffer = sizeof(rpc_event_out->error_message);
            strncpy(rpc_event_out->error_message, item_error_message->valuestring, size_buffer - 1);
            rpc_event_out->error_message[size_buffer - 1] = '\0';
        } else {
            rpc_event_out->success = 1;
        }
    }

    if (result_out != NULL) {
        *result_out = cJSON_GetObjectItemCaseSensitive(root, "result");
    }

    return 1;
}

int parse_rpc_request(cJSON* root, rpc_request_t* rpc_request_out, cJSON** params_out)
{
    if (!is_valid_rpc_request(root)) {
        return 0;
    }
    
    cJSON* item_id = cJSON_GetObjectItemCaseSensitive(root, "id");
    cJSON* item_method = cJSON_GetObjectItemCaseSensitive(root, "method");
    
    if (rpc_request_out != NULL) {
        rpc_request_out->id = item_id->valueint;
        int size_buffer = sizeof(rpc_request_out->method);
        strncpy(rpc_request_out->method, item_method->valuestring, size_buffer - 1);
        rpc_request_out->method[size_buffer - 1] = '\0';
    }

    if (params_out != NULL) {
        *params_out = cJSON_GetObjectItemCaseSensitive(root, "params");
    }

    return 1;
}