
#include "boost/filesystem.hpp"
#include <boost/asio/buffers_iterator.hpp>
#include "gtest/gtest.h"
#include "request_api_handler.h"
class APIHandlerFixture : public :: testing::Test
{
  protected:
    std::string root = "../crud";
    std::string base_uri = "/api";
};
// Bad request if the method is not GET, PUT, POST, DELETE
TEST_F(APIHandlerFixture, BadRequest) { 
    bhttp::request<bhttp::dynamic_body> request; 
    request.target(base_uri); 
    request.method(bhttp::verb::patch);
    bhttp::response<bhttp::dynamic_body> response; 
    request_api_handler api_handler(base_uri, root, request.target().to_string()); 
    bhttp::status status = api_handler.handle_request(request, response); 
    EXPECT_EQ(bhttp::status::bad_request, status); 
}
// TODO: Delete/modify this once POST functionality is created. 
// Before implementing post functionality, POST requests simply echo. 
TEST_F(APIHandlerFixture, POSTRequestEcho) { 
    bhttp::request<bhttp::dynamic_body> request; 
    request.target(base_uri); 
    request.method(bhttp::verb::post);
    bhttp::response<bhttp::dynamic_body> response; 
    request_api_handler api_handler(base_uri, root, request.target().to_string()); 
    bhttp::status status = api_handler.handle_request(request, response); 
    EXPECT_EQ(bhttp::status::created, status); // different from the others for testing
}
// TODO: Delete/modify this once GET functionality is created. 
// Before implementing post functionality, GET requests simply echo. 
TEST_F(APIHandlerFixture, GETRequestEcho) { 
    bhttp::request<bhttp::dynamic_body> request; 
    request.target(base_uri); 
    request.method(bhttp::verb::get);
    bhttp::response<bhttp::dynamic_body> response; 
    request_api_handler api_handler(base_uri, root, request.target().to_string()); 
    bhttp::status status = api_handler.handle_request(request, response); 
    EXPECT_EQ(bhttp::status::ok, status); 
}
// TODO: Delete/modify this once PUT functionality is created. 
// Before implementing post functionality, PUT requests simply echo. 
TEST_F(APIHandlerFixture, PUTRequestEcho) { 
    bhttp::request<bhttp::dynamic_body> request; 
    request.target(base_uri); 
    request.method(bhttp::verb::put);
    bhttp::response<bhttp::dynamic_body> response; 
    request_api_handler api_handler(base_uri, root, request.target().to_string()); 
    bhttp::status status = api_handler.handle_request(request, response); 
    EXPECT_EQ(bhttp::status::ok, status); 
}
// TODO: Delete/modify this once DELETE functionality is created. 
// Before implementing post functionality, DELETE requests simply echo. 
TEST_F(APIHandlerFixture, DELETERequestEcho) { 
    bhttp::request<bhttp::dynamic_body> request; 
    request.target(base_uri); 
    request.method(bhttp::verb::delete_);
    bhttp::response<bhttp::dynamic_body> response; 
    request_api_handler api_handler(base_uri, root, request.target().to_string()); 
    bhttp::status status = api_handler.handle_request(request, response); 
    EXPECT_EQ(bhttp::status::ok, status); 
}
