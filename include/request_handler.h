#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <iostream>
#include "../config_parser.h"

#include "../request_parser.h"
#include "../request.h"
#include "../reply.h"

using http::server::request;
using http::server::request_parser;
using http::server::reply;

class RequestHandler {
public:
    virtual void handleRequest(const request &request_, reply *reply_) = 0;
    
};

#endif  
