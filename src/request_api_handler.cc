
#include "request_api_handler.h"
#include "mime_types.h"
#include <boost/log/trivial.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>

request_api_handler::request_api_handler(std::string location, std::string root, std::string url, std::map<std::string, int>& path_counts) :
  location_(location), root_(root), request_url(url), path_counts(path_counts)
{
}

bhttp::status request_api_handler::handle_post(bhttp::request<bhttp::dynamic_body> req, bhttp::response<bhttp::dynamic_body>& res, std::string directory)
{
  std::string path = root_ + "/" + directory;
  boost::system::error_code ec;
  //create the specified path if it doesn't already exist in the file system
  if (path_counts.find(directory) == path_counts.end() && !boost::filesystem::exists(path)) 
  {
    boost::filesystem::create_directory(path,ec);
    if(ec)
    {
      BOOST_LOG_TRIVIAL(error) << "request_api_handler::handle_post : error creating directory - " << ec;
      return send_bad_request(res);
    }
  }

  //path_counts maps entity names to the number of files in the directory
  //IDs are assigned using the (current max ID in the directory)+1
  int count = path_counts[directory];
  int id = count+1;
  std::string file_path = path + "/" + std::to_string(id);
  std::string json = boost::beast::buffers_to_string(req.body().data());

  BOOST_LOG_TRIVIAL(info) << "request_api_handler::handle_post : creating file with path " << path;

  std::ostringstream oss;     
  oss << json;
  std::string body = oss.str();
  oss.clear();
  std::ofstream file(file_path);
  file << body;
  file.close();

  path_counts[directory] = count+1;

  res.result(bhttp::status::created);
  std::string stock_reply = "<html>"
        "<head><title>Created</title></head>"
        "<body><h1>201 Created file at " + path + " with ID " + std::to_string(id) + "</h1></body>"
        "</html>";
  boost::beast::ostream(res.body()) << stock_reply;
  res.content_length((res.body().size()));
  res.set(bhttp::field::content_type, "text/html");
  return res.result();
}

bhttp::status request_api_handler::send_bad_request(bhttp::response<bhttp::dynamic_body>& res)
{
  res.result(bhttp::status::bad_request);
  boost::beast::ostream(res.body()) << rep.stock_reply(res.result_int());
  res.content_length((res.body().size()));
  res.set(bhttp::field::content_type, "text/html");
  return res.result();
}

bhttp::status request_api_handler::handle_request(const bhttp::request<bhttp::dynamic_body> req, bhttp::response<bhttp::dynamic_body>& res)
{
  std::string req_input = req.target().to_string();

  std::string directory;
  // target needs to be /api/ followed by an entity
  if(req_input.substr(0,5) != "/api/" || req_input == "/api/")
  {
    BOOST_LOG_TRIVIAL(error) << "request_api_handler::handle_request : bad request uri";
    return send_bad_request(res);
  }
  else
  {
    directory = req_input.substr(5);
  }

  bhttp::verb method = req.method(); 
  BOOST_LOG_TRIVIAL(info) << "request_api_handler::handle_post : handling " << req.method_string() << " request with directory " << directory;

  switch(method) 
  {
    case bhttp::verb::post:
        //post requests can only have one entity and no id
        if(directory.find("/") != -1)
        {
          BOOST_LOG_TRIVIAL(error) << "request_api_handler::handle_request : invalid directory for post request";
          return send_bad_request(res);
        }
        return handle_post(req,res,directory);
      break;
    
    case bhttp::verb::get: // TODO: DELETE CODE INSIDE BLOCK, ONLY ECHOES RIGHT NOW
        res.result(bhttp::status::ok);
        boost::beast::ostream(res.body()) << req;
        res.content_length((res.body().size()));
        res.set(bhttp::field::content_type, "text/plain");
        return res.result();
      break; 
    
    case bhttp::verb::put: // TODO: DELETE CODE INSIDE BLOCK, ONLY ECHOES RIGHT NOW
        res.result(bhttp::status::ok);
        boost::beast::ostream(res.body()) << req;
        res.content_length((res.body().size()));
        res.set(bhttp::field::content_type, "text/plain");
        return res.result();
      break; 
    
    case bhttp::verb::delete_: // TODO: DELETE CODE INSIDE BLOCK, ONLY ECHOES RIGHT NOW
        res.result(bhttp::status::ok);
        boost::beast::ostream(res.body()) << req;
        res.content_length((res.body().size()));
        res.set(bhttp::field::content_type, "text/plain");
        return res.result();
      break; 
    default: // Bad request 
        BOOST_LOG_TRIVIAL(error) << "request_api_handler::handle_request : invalid method";
        res.result(bhttp::status::bad_request);
        boost::beast::ostream(res.body()) << rep.stock_reply(res.result_int());
        res.content_length((res.body().size()));
        res.set(bhttp::field::content_type, "text/html");
        return res.result();
      break;  
  }
}
