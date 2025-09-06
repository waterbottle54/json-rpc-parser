// File: test.c
// Module: CMake test cases for json_rpc parser
// Description: Test cases to validate functions in json_rpc module
// Author: Sungwon Jo
// Organization: DoubleO
// Created: 2025-09-06
// License: MIT License
// Dependencies: cJSON library
// NOTEs: 
// 'result', 'params' should be json object 
// 'error' (object) must have both 'code' (int) and 'message' (string)

#include <stdio.h>
#include <stdlib.h>
#include "cJSON/cJSON.h"
#include "json_rpc.h"

#define PASS 0
#define FAIL 1

int test_is_rpc_2_0()
{
    char str_json[256] = "{ \"jsonrpc\": \"2.0\" }";
    cJSON* root = cJSON_Parse(str_json);
    if (root == NULL) {
        printf("failed to create json object");
        return 1;
    }

    int pass = check_rpc_version(root, "2.0") == 1;

    if (root != NULL) {
        free((void*)root);
        root = NULL;
    }

    return pass ? PASS : FAIL;
}

int test_is_rpc_event()
{
    char str_json[256] = 
        "{\n" 
        "\"jsonrpc\": \"2.0\",\n"
        "\"result\":{ \"d\": 1 },\n"
        "\"error\":{ \"code\": 1, \"message\": \"hi\" },\n"
        "\"id\": 1\n"
        "}";
    cJSON* root = cJSON_Parse(str_json);
    if (root == NULL) {
        printf("failed to create json object");
        return 1;
    }

    int pass = is_valid_rpc_event(root) == 1;

    if (root != NULL) {
        free((void*)root);
        root = NULL;
    }

    return pass ? PASS : FAIL;
}

int test_is_rpc_request()
{
    char str_json[256] = 
        "{\n" 
        "\"jsonrpc\": \"2.0\",\n"
        "\"method\": \"get\",\n"
        "\"params\": {},\n"
        "\"id\": 1\n"
        "}";
    cJSON* root = cJSON_Parse(str_json);
    if (root == NULL) {
        printf("failed to create json object");
        return 1;
    }

    int pass = is_valid_rpc_request(root) == 1;

    if (root != NULL) {
        free((void*)root);
        root = NULL;
    }

    return pass ? PASS : FAIL;
}

int test_parse_event() 
{
    char str_json[256] = 
        "{\n" 
        "\"jsonrpc\": \"2.0\",\n"
        "\"result\":{ \"d\": 1 },\n"
        "\"error\":{ \"code\": 3, \"message\": \"hi\" },\n"
        "\"id\": 1\n"
        "}";
    cJSON* root = cJSON_Parse(str_json);
    if (root == NULL) {
        printf("failed to create json object");
        return 1;
    }

    rpc_event_t event = {};
    cJSON* p_result;
    parse_rpc_event(root, &event, &p_result);

    int pass = 1;
    if (event.id != 1) {
        pass = 0;
    }
    if (event.error_code != 3) {
        pass = 0;
    }
    if (event.success != 0) {
        pass = 0;
    }
    if (cJSON_HasObjectItem(p_result, "d") != 1) {
        pass = 0;
    }
    if (cJSON_GetObjectItem(p_result, "d")->valueint != 1) {
        pass = 0;
    }

    if (root != NULL) {
        free((void*)root);
        root = NULL;
    }

    return pass ? PASS : FAIL;
}

int test_parse_request()
{
    char str_json[256] = 
        "{\n" 
        "\"jsonrpc\": \"2.0\",\n"
        "\"method\": \"getRange\",\n"
        "\"params\": { \"target\": 34 },\n"
        "\"id\": 12\n"
        "}";
    cJSON* root = cJSON_Parse(str_json);
    if (root == NULL) {
        printf("failed to create json object");
        return 1;
    }
    
    rpc_request_t request = {};
    cJSON* p_params;
    parse_rpc_request(root, &request, &p_params);

    int pass = 1;
    if (request.id != 12) {
        pass = 0;
    }
    if (strcmp(request.method, "getRange") != 0) {
        pass = 0;
    }
    if (cJSON_HasObjectItem(p_params, "target") != 1) {
        pass = 0;
    }
    if (cJSON_GetObjectItem(p_params, "target")->valueint != 34) {
        pass = 0;
    }

    if (root != NULL) {
        free((void*)root);
        root = NULL;
    }

    return pass ? PASS : FAIL;
}

int main()
{
    // return test_is_rpc_2_0();
    // return test_is_rpc_event();
    // return test_is_rpc_request();
    // return test_parse_event();
    return test_parse_request();
}