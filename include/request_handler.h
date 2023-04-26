#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include "config_parser.h"
#include <iostream>

#include "reply.h"
#include "request.h"
#include "request_parser.h"

using http::server::reply;
using http::server::request;
using http::server::request_parser;

class RequestHandler
{
public:
  virtual void handleRequest(const request &request_, reply *reply_) = 0;
};

#endif
