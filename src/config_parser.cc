// An nginx config file parser.
//
// See:
//   http://wiki.nginx.org/Configuration
//   http://blog.martinfjordvald.com/2010/07/nginx-primer/
//
// How Nginx does it:
//   http://lxr.nginx.org/source/src/core/ngx_conf_file.c

#include <boost/log/trivial.hpp>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

#include "config_parser.h"
#include "constants.h"

std::string NginxConfig::ToString(int depth)
{
  std::string serialized_config;
  for (const auto& statement : statements_)
  {
    serialized_config.append(statement->ToString(depth));
  }
  return serialized_config;
}

int NginxConfig::get_listen_port() {
  int port = -1;
  for (auto s : statements_) {
    if (s->child_block_.get() != nullptr) {
      port = s->child_block_->get_listen_port();
    }
    else {
      if (s->tokens_.size() == 2 && s->tokens_[0] == "listen") {
        port = stoi(s->tokens_[1]);
        break;
      }
    }
  }
  if (port < 1 || port > 65535) {
    throw std::invalid_argument("Port " + std::to_string(port) +
                                " out of range");
  }
  return port;
}

std::vector<path> NginxConfig::get_paths() {
  for (auto s : statements_) {
    if (s->tokens_[0] != kUrlPathKeyword) {
      continue;
    }
    if (s->tokens_[2] == kStaticHandler) {
      path static_path;
      for (auto child_statement : s->child_block_->statements_) {
        if (child_statement->tokens_[0] == kResourcePathKeyword) {
          static_path.type = static_;
          static_path.endpoint = s->tokens_[1];
          static_path.root = child_statement->tokens_[1];
        }
      }
      paths.push_back(static_path);
    }
    else if (s->tokens_[2] == kEchoHandler) {
      path echo_path;
      echo_path.type = echo;
      echo_path.endpoint = s->tokens_[1];
      echo_path.root = "";
      paths.push_back(echo_path);
    }
  }

  for (auto p : paths) {
    BOOST_LOG_TRIVIAL(info) << "path endpoint_type " << p.type;
    BOOST_LOG_TRIVIAL(info) << "path endpoint: " << p.endpoint;
    BOOST_LOG_TRIVIAL(info) << "path root: " << p.root;
  }

  return paths;
}

std::string NginxConfigStatement::ToString(int depth) {
  std::string serialized_statement;
  for (int i = 0; i < depth; ++i) {
    serialized_statement.append("  ");
  }
  for (unsigned int i = 0; i < tokens_.size(); ++i) {
    if (i != 0) {
      serialized_statement.append(" ");
    }
    serialized_statement.append(tokens_[i]);
  }
  if (child_block_.get() != nullptr) {
    serialized_statement.append(" {\n");
    serialized_statement.append(child_block_->ToString(depth + 1));
    for (int i = 0; i < depth; ++i) {
      serialized_statement.append("  ");
    }
    serialized_statement.append("}");
  }
  else {
    serialized_statement.append(";");
  }
  serialized_statement.append("\n");
  return serialized_statement;
}

const char* NginxConfigParser::TokenTypeAsString(TokenType type)
{
  switch (type)
  {
    case TOKEN_TYPE_START:
      return "TOKEN_TYPE_START";
    case TOKEN_TYPE_NORMAL:
      return "TOKEN_TYPE_NORMAL";
    case TOKEN_TYPE_START_BLOCK:
      return "TOKEN_TYPE_START_BLOCK";
    case TOKEN_TYPE_END_BLOCK:
      return "TOKEN_TYPE_END_BLOCK";
    case TOKEN_TYPE_COMMENT:
      return "TOKEN_TYPE_COMMENT";
    case TOKEN_TYPE_STATEMENT_END:
      return "TOKEN_TYPE_STATEMENT_END";
    case TOKEN_TYPE_EOF:
      return "TOKEN_TYPE_EOF";
    case TOKEN_TYPE_ERROR:
      return "TOKEN_TYPE_ERROR";
    default:
      return "Unknown token type";
  }
}

NginxConfigParser::TokenType NginxConfigParser::ParseToken(std::istream* input,
                                                           std::string* value) {
  TokenParserState state = TOKEN_STATE_INITIAL_WHITESPACE;
  while (input->good()) {
    const char c = input->get();
    if (!input->good()) {
      break;
    }
    switch (state) {
      case TOKEN_STATE_INITIAL_WHITESPACE:
        switch (c) {
          case '{':
            *value = c;
            return TOKEN_TYPE_START_BLOCK;
          case '}':
            *value = c;
            return TOKEN_TYPE_END_BLOCK;
          case '#':
            *value = c;
            state = TOKEN_STATE_TOKEN_TYPE_COMMENT;
            continue;
          case '"':
            *value = c;
            state = TOKEN_STATE_DOUBLE_QUOTE;
            continue;
          case '\'':
            *value = c;
            state = TOKEN_STATE_SINGLE_QUOTE;
            continue;
          case ';':
            *value = c;
            return TOKEN_TYPE_STATEMENT_END;
          case ' ':
          case '\t':
          case '\n':
          case '\r':
            continue;
          default:
            *value += c;
            state = TOKEN_STATE_TOKEN_TYPE_NORMAL;
            continue;
        }
      case TOKEN_STATE_SINGLE_QUOTE:
        *value += c;
        if (c == '\'') {
          const char next_c = input->get();
          if (next_c == ' ' || next_c == '\n' || next_c == '\t' ||
              next_c == '{' || next_c == '}' || next_c == ';') {
            input->unget();
          }
          else {
            return TOKEN_TYPE_ERROR;
          }
          return TOKEN_TYPE_NORMAL;
        }
        else if (c == '\\') {
          value->pop_back();
          state = TOKEN_STATE_ESCAPE_SINGLE_QUOTE;
        }
        continue;
      case TOKEN_STATE_DOUBLE_QUOTE:
        *value += c;
        if (c == '"') {
          const char next_c = input->get();
          if (next_c == ' ' || next_c == '\n' || next_c == '\t' ||
              next_c == '{' || next_c == '}' || next_c == ';') {
            input->unget();
          }
          else {
            return TOKEN_TYPE_ERROR;
          }
          return TOKEN_TYPE_NORMAL;
        }
        else if (c == '\\') {
          value->pop_back();
          state = TOKEN_STATE_ESCAPE_DOUBLE_QUOTE;
        }
        continue;
      case TOKEN_STATE_TOKEN_TYPE_COMMENT:
        if (c == '\n' || c == '\r') {
          return TOKEN_TYPE_COMMENT;
        }
        *value += c;
        continue;
      case TOKEN_STATE_TOKEN_TYPE_NORMAL:
        if (c == ' ' || c == '\t' || c == '\n' || c == '\t' || c == ';' ||
            c == '{' || c == '}') {
          input->unget();
          return TOKEN_TYPE_NORMAL;
        }
        *value += c;
        continue;
      case TOKEN_STATE_ESCAPE_SINGLE_QUOTE:
        *value += c;
        state = TOKEN_STATE_SINGLE_QUOTE;
        continue;
      case TOKEN_STATE_ESCAPE_DOUBLE_QUOTE:
        *value += c;
        state = TOKEN_STATE_DOUBLE_QUOTE;
        continue;
    }
  }

  // If we get here, we reached the end of the file.
  if (state == TOKEN_STATE_SINGLE_QUOTE || state == TOKEN_STATE_DOUBLE_QUOTE) {
    return TOKEN_TYPE_ERROR;
  }

  return TOKEN_TYPE_EOF;
}

bool NginxConfigParser::Parse(std::istream* config_file, NginxConfig* config) {
  std::stack<NginxConfig*> config_stack;
  config_stack.push(config);
  TokenType last_token_type = TOKEN_TYPE_START;
  TokenType token_type;
  while (true) {
    std::string token;
    token_type = ParseToken(config_file, &token);
    BOOST_LOG_TRIVIAL(info)
      << TokenTypeAsString(token_type) << ": " << token.c_str();
    if (token_type == TOKEN_TYPE_ERROR) {
      break;
    }

    if (token_type == TOKEN_TYPE_COMMENT) {
      // Skip comments.
      continue;
    }

    if (token_type == TOKEN_TYPE_START) {
      // Error.
      break;
    }
    else if (token_type == TOKEN_TYPE_NORMAL) {
      if (last_token_type == TOKEN_TYPE_START ||
          last_token_type == TOKEN_TYPE_STATEMENT_END ||
          last_token_type == TOKEN_TYPE_START_BLOCK ||
          last_token_type == TOKEN_TYPE_END_BLOCK ||
          last_token_type == TOKEN_TYPE_NORMAL) {
        if (last_token_type != TOKEN_TYPE_NORMAL) {
          config_stack.top()->statements_.emplace_back(
              new NginxConfigStatement);
        }
        config_stack.top()->statements_.back().get()->tokens_.push_back(token);
      }
      else {
     // Error.
        break;
      }
    }
    else if (token_type == TOKEN_TYPE_STATEMENT_END) {
      if (last_token_type != TOKEN_TYPE_NORMAL) {
        // Error.
        break;
      }
    }
    else if (token_type == TOKEN_TYPE_START_BLOCK) {
      if (last_token_type != TOKEN_TYPE_NORMAL) {
        // Error.
        break;
      }
      NginxConfig* const new_config = new NginxConfig;
      config_stack.top()->statements_.back().get()->child_block_.reset(
          new_config);
      config_stack.push(new_config);
    }
    else if (token_type == TOKEN_TYPE_END_BLOCK) {
      if (last_token_type != TOKEN_TYPE_START_BLOCK &&
          last_token_type != TOKEN_TYPE_STATEMENT_END &&
          last_token_type != TOKEN_TYPE_END_BLOCK) {
        // Error.
        break;
      }
      for (auto s : config_stack.top()->statements_) {
        if (s->tokens_[0] == "listen") {
          if (s->tokens_.size() != 2) {
            break;
          }
        }
      }
      config_stack.pop();
    }
    else if (token_type == TOKEN_TYPE_EOF) {
      if (last_token_type != TOKEN_TYPE_STATEMENT_END &&
          last_token_type != TOKEN_TYPE_END_BLOCK &&
          last_token_type != TOKEN_TYPE_START) {
        // Error.
        break;
      }
      config_stack.pop();
      if (config_stack.size() > 0) {
        break;
      }
      return true;
    }
    else {
   // Error. Unknown token.
      break;
    }
    last_token_type = token_type;
  }

  BOOST_LOG_TRIVIAL(error) << "Bad transition from "
    << TokenTypeAsString(last_token_type) << " to "
    << TokenTypeAsString(token_type);
  return false;
}

bool NginxConfigParser::Parse(const char* file_name, NginxConfig* config) {
  std::ifstream config_file;
  config_file.open(file_name);
  if (!config_file.good()) {
    BOOST_LOG_TRIVIAL(error) << "Failed to open config file: " << file_name;
    return false;
  }

  const bool return_value =
    Parse(dynamic_cast<std::istream*>(&config_file), config);
  config_file.close();
  return return_value;
}
