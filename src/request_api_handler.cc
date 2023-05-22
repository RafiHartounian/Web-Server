
#include "request_api_handler.h"
#include "mime_types.h"
#include <boost/log/trivial.hpp>
#include <boost/filesystem.hpp>

request_api_handler::request_api_handler(std::string location, std::string root, std::string url, std::map<std::string, std::vector<int>>& path_counts) :
  location_(location), root_(root), request_url(url), path_counts(path_counts)
{
  for (auto file : boost::filesystem::recursive_directory_iterator(root)) {
    if (!is_regular_file(file)) {
        BOOST_LOG_TRIVIAL(info) << "ignoring non-file in crud directory: " << absolute(file).string() << std::endl;
        continue;
    }

    boost::filesystem::path file_path = absolute(file);
    std::string file_path_str = file_path.string();

    int id;
    // check that the filename is an integer
    if(isdigit(file_path_str[file_path_str.length()-1])) 
    {
      id = std::stoi(file_path.stem().string());
    }
    else
    {
      BOOST_LOG_TRIVIAL(info) << "ignoring file with incorrect name";
      continue;
    }

    // remove the id and trailing slash part of the file path
    std::string entity = file_path_str.substr(0,file_path_str.find_last_of("/"));
    entity = entity.substr(entity.find_last_of("/")+1);

    std::vector<int> ids = path_counts[entity];
    //find where to insert id into the id list
    if(ids.size() == 0)
    {
      ids.push_back(id);
    }
    else if(ids.size() == 1)
    {
      if(ids[0] < id)
      {
        ids.push_back(id);
      }
      else
      {
        ids.insert(ids.begin(), id);
      }
    }
    else
    {
      int index = 1;
      while(index != ids.size() && (ids[index] == ids[index-1]+1))
      {
        index++;
      }
      ids.insert(ids.begin()+index,id);
    }

    path_counts[entity] = ids;
  }
}

//checks the list of IDs associated with the given directory to find the lowest ID available
int request_api_handler::getNextID(std::string directory)
{
  std::vector<int> id_list = path_counts[directory];
  int index = 1;
  int id = 0;
  if(id_list.size() == 0)
  {
    id = 1;
  }
  else if(id_list.size() == 1)
  {
    if(id_list[0] == 1)
    {
      id = 2;
    }
    else
    {
      id = 1;
    }
  }
  else
  {
    while(index != id_list.size() && (id_list[index] == id_list[index-1]+1))
    {
      index++;
    }
    id = id_list[index-1]+1;
    index++;
  }

  id_list.insert(id_list.begin()+index-1,id);
  path_counts[directory] = id_list;

  return id;
}

bhttp::status request_api_handler::handle_post(bhttp::request<bhttp::dynamic_body> req, bhttp::response<bhttp::dynamic_body>& res, std::string directory)
{
  std::string path = root_ + "/" + directory;
  boost::system::error_code ec;
  //create the specified path if it doesn't already exist in the file system
  if (!boost::filesystem::exists(path)) 
  {
    boost::filesystem::create_directory(path,ec);
    if(ec)
    {
      BOOST_LOG_TRIVIAL(error) << "request_api_handler::handle_post : error creating directory - " << ec;
      return send_bad_request(res);
    }
  }

  int id = getNextID(directory);

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

bhttp::status request_api_handler::handle_delete(bhttp::request<bhttp::dynamic_body> req, bhttp::response<bhttp::dynamic_body>& res, std::string directory)
{
  std::string path = root_ + "/" + directory;
  boost::filesystem::path boost_path(path);
  //if the file doesn't exist, bad request 
  if (!boost::filesystem::exists(boost_path)) 
  {
    BOOST_LOG_TRIVIAL(error) << "request_api_handler::handle_delete path does not exist: " << path << std::endl; 
    return send_bad_request(res);
  }
  if (boost::filesystem::is_directory(boost_path)) {
    BOOST_LOG_TRIVIAL(error) << "request_api_handler::handle_delete path is a directory: " << path << std::endl; 
    return send_bad_request(res); 
  }
  // get id 
  std::size_t found = directory.find_last_of("/");
  std::string id_str = directory.substr(found+1);
  std::string entity = directory.substr(0, found);
  // id string -> int 
  int id; 
  try 
  {
    id = std::stoi(id_str); 
  }
  catch(const std::exception&) 
  {
    BOOST_LOG_TRIVIAL(error) << "request_api_handler::handle_delete invalid id: " << id_str << std::endl; 
    return send_bad_request(res); 
  }
  
  if (path_counts.find(entity) != path_counts.end()) 
  {
    if (boost::filesystem::remove(boost_path)) {
      // Remove from path_counts
      std::vector<int>::iterator vector_it; 
      vector_it = find(path_counts[entity].begin(), path_counts[entity].end(), id);
      if (vector_it == path_counts[entity].end()) {
        BOOST_LOG_TRIVIAL(error) << "Entity: " << entity << "/ID: " << id << " not found in path_counts\n";
        return send_bad_request(res); 
      } 
      path_counts[entity].erase(vector_it);

      BOOST_LOG_TRIVIAL(info) << "Deleting file:" << std::endl;
      res.result(bhttp::status::ok);
      std::string stock_reply = "<html>"
                "<head><title>Deleted</title></head>"
                "<body><h1>File Deleted.</h1></body>"
                "</html>";
      boost::beast::ostream(res.body()) << stock_reply;
      res.content_length((res.body().size()));
      res.set(bhttp::field::content_type, "text/html");
      return res.result();
    }
  }

  // remove 
  return send_bad_request(res); 

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
  BOOST_LOG_TRIVIAL(info) << "request_api_handler::handle_request : handling " << req.method_string() << " request with directory " << directory;

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
    
    case bhttp::verb::delete_: 
        //delete requests must have 1 id 
        if(directory.find("/") == -1)
        {
          BOOST_LOG_TRIVIAL(error) << "request_api_handler::handle_request : invalid directory for delete request";
          return send_bad_request(res);
        }
        return handle_delete(req,res,directory);
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