
#include "boost/filesystem.hpp"
#include <boost/asio/buffers_iterator.hpp>
#include "gtest/gtest.h"
#include "request_api_handler.h"
class APIHandlerFixture : public :: testing::Test 
{
  public:
    APIHandlerFixture() {
        root = "../crud";
        base_uri = "/api";
        
        boost::filesystem::path root_path(root);
        if(!boost::filesystem::exists(root_path))
        {
            boost::filesystem::create_directory(root_path);
        }
    }
  protected:
    std::string root;
    std::string base_uri;
    std::map<std::string, std::vector<int>> path_counts;
};
// Bad request if the method is not GET, PUT, POST, DELETE
TEST_F(APIHandlerFixture, BadRequest) { 
    bhttp::request<bhttp::dynamic_body> request; 
    request.target("/api/Shoes"); 
    request.method(bhttp::verb::patch);
    bhttp::response<bhttp::dynamic_body> response; 
    request_api_handler api_handler(base_uri, root, request.target().to_string(), path_counts); 
    bhttp::status status = api_handler.handle_request(request, response); 
    EXPECT_EQ(bhttp::status::bad_request, status); 
}

TEST_F(APIHandlerFixture, InvalidNoEntity) { 
    bhttp::request<bhttp::dynamic_body> request; 
    request.target("/api/"); 
    request.method(bhttp::verb::post);
    boost::beast::ostream(request.body()) << "\'{\"name\":\"John\", \"age\":30, \"car\":null}\'";
    bhttp::response<bhttp::dynamic_body> response; 
    request_api_handler api_handler(base_uri, root, request.target().to_string(), path_counts); 
    bhttp::status status1 = api_handler.handle_request(request, response);
    
    EXPECT_EQ(bhttp::status::bad_request, status1);
    EXPECT_EQ(path_counts.size(), 0);
}

TEST_F(APIHandlerFixture, InvalidBadURI) { 
    bhttp::request<bhttp::dynamic_body> request; 
    request.target("/skdjfnksdj/"); 
    request.method(bhttp::verb::post);
    boost::beast::ostream(request.body()) << "\'{\"name\":\"John\", \"age\":30, \"car\":null}\'";
    bhttp::response<bhttp::dynamic_body> response; 
    request_api_handler api_handler(base_uri, root, request.target().to_string(), path_counts); 
    bhttp::status status1 = api_handler.handle_request(request, response);
    
    EXPECT_EQ(bhttp::status::bad_request, status1);
    EXPECT_EQ(path_counts.size(), 0);
}

TEST_F(APIHandlerFixture, PostInvalidDirectory) { 
    bhttp::request<bhttp::dynamic_body> request; 
    request.target("/api/Shoes/Books"); 
    request.method(bhttp::verb::post);
    boost::beast::ostream(request.body()) << "\'{\"name\":\"John\", \"age\":30, \"car\":null}\'";
    bhttp::response<bhttp::dynamic_body> response; 
    request_api_handler api_handler(base_uri, root, request.target().to_string(), path_counts); 
    bhttp::status status1 = api_handler.handle_request(request, response);
    
    EXPECT_EQ(bhttp::status::bad_request, status1);
    EXPECT_EQ(path_counts.size(), 0);
}

TEST_F(APIHandlerFixture, PostRequest) { 
    bhttp::request<bhttp::dynamic_body> request; 
    request.target("/api/Shoes"); 
    request.method(bhttp::verb::post);
    boost::beast::ostream(request.body()) << "\'{\"name\":\"John\", \"age\":30, \"car\":null}\'";
    bhttp::response<bhttp::dynamic_body> response; 
    request_api_handler api_handler(base_uri, root, request.target().to_string(), path_counts); 
    bhttp::status status1 = api_handler.handle_request(request, response);
    std::string body1 = boost::beast::buffers_to_string(response.body().data());
    std::cerr << "id list size final: " << path_counts["Shoes"].size() << std::endl;

    bhttp::request<bhttp::dynamic_body> request2; 
    request2.target("/api/Shoes"); 
    request2.method(bhttp::verb::post);
    boost::beast::ostream(request2.body()) << "\'{\"name\":\"Jeff\", \"age\":32, \"car\":null}\'";
    bhttp::response<bhttp::dynamic_body> response2; 
    request_api_handler api_handler2(base_uri, root, request2.target().to_string(), path_counts); 
    bhttp::status status2 = api_handler2.handle_request(request2, response2); 
    std::string body2 = boost::beast::buffers_to_string(response2.body().data());
    std::cerr << "id list size final: " << path_counts["Shoes"].size() << std::endl;

    bhttp::request<bhttp::dynamic_body> request3; 
    request3.target("/api/Books"); 
    request3.method(bhttp::verb::post);
    boost::beast::ostream(request3.body()) << "\'{\"name\":\"Jessie\", \"age\":45, \"car\":null}\'";
    bhttp::response<bhttp::dynamic_body> response3; 
    request_api_handler api_handler3(base_uri, root, request3.target().to_string(), path_counts); 
    bhttp::status status3 = api_handler3.handle_request(request3, response3); 
    std::string body3 = boost::beast::buffers_to_string(response3.body().data());
    std::cerr << "id list size final: " << path_counts["Shoes"].size() << std::endl;
    
    EXPECT_EQ(bhttp::status::created, status1);
    EXPECT_EQ(bhttp::status::created, status2);
    EXPECT_EQ(bhttp::status::created, status3);

    std::stringstream buffer;
    std::stringstream buffer2;
    std::stringstream buffer3;
    std::ifstream t("../crud/Shoes/1");
    std::ifstream t2("../crud/Shoes/2");
    std::ifstream t3("../crud/Books/1");
    buffer << t.rdbuf();
    buffer2 << t2.rdbuf();
    buffer3 << t3.rdbuf();
    EXPECT_EQ(buffer.str(), "\'{\"name\":\"John\", \"age\":30, \"car\":null}\'");
    EXPECT_EQ(buffer2.str(), "\'{\"name\":\"Jeff\", \"age\":32, \"car\":null}\'");
    EXPECT_EQ(buffer3.str(), "\'{\"name\":\"Jessie\", \"age\":45, \"car\":null}\'");

    std::string rep1 = "<html>"
        "<head><title>Created</title></head>"
        "<body><h1>201 Created file at ../crud/Shoes with ID 1</h1></body>"
        "</html>";
    std::string rep2 = "<html>"
        "<head><title>Created</title></head>"
        "<body><h1>201 Created file at ../crud/Shoes with ID 2</h1></body>"
        "</html>";
    std::string rep3 = "<html>"
        "<head><title>Created</title></head>"
        "<body><h1>201 Created file at ../crud/Books with ID 1</h1></body>"
        "</html>";
    EXPECT_EQ(body1, rep1);
    EXPECT_EQ(body2, rep2);
    EXPECT_EQ(body3, rep3);

    boost::filesystem::remove_all("../crud/Shoes");
    boost::filesystem::remove_all("../crud/Books");
}

TEST_F(APIHandlerFixture, PostWithDeleteds) { 
    std::vector<int> shoes_ids = {3};
    std::vector<int> books_ids = {1,2,4};
    path_counts["Shoes"] = shoes_ids;
    path_counts["Books"] = books_ids;

    bhttp::request<bhttp::dynamic_body> request; 
    request.target("/api/Shoes"); 
    request.method(bhttp::verb::post);
    boost::beast::ostream(request.body()) << "\'{\"name\":\"John\", \"age\":30, \"car\":null}\'";
    bhttp::response<bhttp::dynamic_body> response; 
    request_api_handler api_handler(base_uri, root, request.target().to_string(), path_counts); 
    bhttp::status status1 = api_handler.handle_request(request, response);
    std::string body1 = boost::beast::buffers_to_string(response.body().data());
    std::cerr << "id list size final: " << path_counts["Shoes"].size() << std::endl;

    bhttp::request<bhttp::dynamic_body> request2; 
    request2.target("/api/Books"); 
    request2.method(bhttp::verb::post);
    boost::beast::ostream(request2.body()) << "\'{\"name\":\"Jeff\", \"age\":32, \"car\":null}\'";
    bhttp::response<bhttp::dynamic_body> response2; 
    request_api_handler api_handler2(base_uri, root, request2.target().to_string(), path_counts); 
    bhttp::status status2 = api_handler2.handle_request(request2, response2); 
    std::string body2 = boost::beast::buffers_to_string(response2.body().data());
    std::cerr << "id list size final: " << path_counts["Books"].size() << std::endl;

    bhttp::request<bhttp::dynamic_body> request3; 
    request3.target("/api/Books"); 
    request3.method(bhttp::verb::post);
    boost::beast::ostream(request3.body()) << "\'{\"name\":\"Jessie\", \"age\":45, \"car\":null}\'";
    bhttp::response<bhttp::dynamic_body> response3; 
    request_api_handler api_handler3(base_uri, root, request3.target().to_string(), path_counts); 
    bhttp::status status3 = api_handler3.handle_request(request3, response3); 
    std::string body3 = boost::beast::buffers_to_string(response3.body().data());
    std::cerr << "id list size final: " << path_counts["Books"].size() << std::endl;
    
    EXPECT_EQ(bhttp::status::created, status1);
    EXPECT_EQ(bhttp::status::created, status2);
    EXPECT_EQ(bhttp::status::created, status3);

    std::stringstream buffer;
    std::stringstream buffer2;
    std::stringstream buffer3;
    std::ifstream t("../crud/Shoes/1");
    std::ifstream t2("../crud/Books/3");
    std::ifstream t3("../crud/Books/5");
    buffer << t.rdbuf();
    buffer2 << t2.rdbuf();
    buffer3 << t3.rdbuf();
    EXPECT_EQ(buffer.str(), "\'{\"name\":\"John\", \"age\":30, \"car\":null}\'");
    EXPECT_EQ(buffer2.str(), "\'{\"name\":\"Jeff\", \"age\":32, \"car\":null}\'");
    EXPECT_EQ(buffer3.str(), "\'{\"name\":\"Jessie\", \"age\":45, \"car\":null}\'");

    std::string rep1 = "<html>"
        "<head><title>Created</title></head>"
        "<body><h1>201 Created file at ../crud/Shoes with ID 1</h1></body>"
        "</html>";
    std::string rep2 = "<html>"
        "<head><title>Created</title></head>"
        "<body><h1>201 Created file at ../crud/Books with ID 3</h1></body>"
        "</html>";
    std::string rep3 = "<html>"
        "<head><title>Created</title></head>"
        "<body><h1>201 Created file at ../crud/Books with ID 5</h1></body>"
        "</html>";
    EXPECT_EQ(body1, rep1);
    EXPECT_EQ(body2, rep2);
    EXPECT_EQ(body3, rep3);

    boost::filesystem::remove_all("../crud/Shoes");
    boost::filesystem::remove_all("../crud/Books");
}

// TODO: Delete/modify this once GET functionality is created. 
// Before implementing post functionality, GET requests simply echo. 
TEST_F(APIHandlerFixture, GETRequestEcho) { 
    bhttp::request<bhttp::dynamic_body> request; 
    request.target("/api/Shoes/1"); 
    request.method(bhttp::verb::get);
    bhttp::response<bhttp::dynamic_body> response; 
    request_api_handler api_handler(base_uri, root, "/api/Shoes/1", path_counts); 
    bhttp::status status = api_handler.handle_request(request, response); 
    EXPECT_EQ(bhttp::status::ok, status); 
}
// TODO: Delete/modify this once PUT functionality is created. 
// Before implementing post functionality, PUT requests simply echo. 
TEST_F(APIHandlerFixture, PUTRequestEcho) { 
    bhttp::request<bhttp::dynamic_body> request; 
    request.target("/api/Shoes/1"); 
    request.method(bhttp::verb::put);
    bhttp::response<bhttp::dynamic_body> response; 
    request_api_handler api_handler(base_uri, root, "/api/Shoes/1", path_counts); 
    bhttp::status status = api_handler.handle_request(request, response); 
    EXPECT_EQ(bhttp::status::ok, status); 
}
// TODO: Delete/modify this once DELETE functionality is created. 
// Before implementing post functionality, DELETE requests simply echo. 
TEST_F(APIHandlerFixture, DELETERequestEcho) { 
    bhttp::request<bhttp::dynamic_body> request; 
    request.target("/api/Shoes/1");
    request.method(bhttp::verb::delete_);
    bhttp::response<bhttp::dynamic_body> response; 
    request_api_handler api_handler(base_uri, root, "/api/Shoes/1", path_counts); 
    bhttp::status status = api_handler.handle_request(request, response); 
    EXPECT_EQ(bhttp::status::ok, status); 
}
