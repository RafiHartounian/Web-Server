#include <boost/asio/buffers_iterator.hpp>
#include <cstring>
#include "gtest/gtest.h"
#include <iostream>
#include "request_404_handler.h"


TEST(Request404HandlerTest, NotFoundRequest)
{
  bhttp::request<bhttp::dynamic_body> request;

  bhttp::response<bhttp::dynamic_body> response;
  request_404_handler* error_handler = new request_404_handler("/", "test url");
  bhttp::status status = error_handler->handle_request(request, response);

  std::string body { boost::asio::buffers_begin(response.body().data()),
    boost::asio::buffers_end(response.body().data()) };

  std::vector<std::pair<std::string, std::string>> headers;
  for (auto const& field : response)
  {
    std::pair<std::string, std::string> header;
    header.first = std::string(field.name_string());
    header.second = std::string(field.value());
    headers.push_back(header);
  }

  reply rep;
  std::string expected_body = rep.stock_reply(404);

  bool success = (response.result() == bhttp::status::not_found &&
                  body == expected_body &&
                  response.has_content_length() &&
                  headers.at(0).first == "Content-Length" &&
                  headers.at(0).second == std::to_string(expected_body.size()) &&
                  headers.at(1).first == "Content-Type" &&
                  headers.at(1).second == "text/html" &&
                  status == response.result());

  EXPECT_TRUE(success);
}