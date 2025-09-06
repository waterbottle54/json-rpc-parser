// File: json_rpc.c
// Module: JSON-RPC 2.0 Parser
// Description: Implements JSON parsing utilities for JSON-RPC 2.0 format
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
#include <string.h>
#include "cJSON/cJSON.h"

typedef struct rpc_event {
    int success;
    int error_code;
    char error_message[32];
    int id; 
} rpc_event_t;

typedef struct rpc_request {
    char method[32];
    int id;
} rpc_request_t;


int check_rpc_version(cJSON* root, const char* version);

int is_valid_rpc_event(cJSON* root);

int is_valid_rpc_request(cJSON* root);

int parse_rpc_event(cJSON* root, rpc_event_t* rpc_event_out, cJSON** result_out);

int parse_rpc_request(cJSON* root, rpc_request_t* rpc_request_out, cJSON** params_out);