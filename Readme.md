# Crazy Code Documentation
## Source Code Structure
* `CMakeLists.txt`: Used to compile and generate our server binary.
* `configs/`
    * Contains configuration files to set which port number to use, as well as the root directory and echo paths.
* `docker/`
    * Contains the Dockerfile that creates the Docker conatainer that is used by the deployed webserver.
* `include/`
    * Contains all the header files.
    * `handler_factories/`: contains the header files specifically for the factories.
* `src/`
    * Contains all source files.
    * `handler_factories/`: contains the source files specifically for the factories.
* `static_files`
    * Contains different type of static files to be served.
    * `static1/`: contains simple static files like `.txt` and `.html`.
    * `static2/`: contains more complex static files like `.gif` and `.jpg`.
    * `static3/`: contains static file for `.zip`.
* `tests/`
    * Contains all of the unit tests and integration tests.
    * Contains some additional files that are used to test the config parsing as well as to check the output for the integration test.

## Prerequisites
* Clone Crazy-Code repository: `git clone https://github.com/RafiHartounian/Web-Server.git`

## How to build
* While in the root directory create a new folder called build: `mkdir build`
* Move into `build/` directory: `cd build/`
* Run `cmake ..`
* Run `make`
    
## How to test
* Change directory to the `build/` directory that was created above.
* Run `make test`
    
## How to run the server
* Locate and use one of the config files located under `configs/`
    * Alternatively a personal config file can be created to use as well.
* From the root directory run `./build/bin/server <path/to/config/file>`
* Connect to the server:
    * From the terminal: `curl http://localhost:{PORT_NUM}/echo` (this displays the echo page)
    * From web browser: `http://localhost:{PORT_NUM}/echo`

## How to contribute a new request handler
* Bind a new handler_factory to the corresponding endpoint locations in server.cc
    * Refer to https://github.com/RafiHartounian/Web-Server/blob/main/src/server.cc line 27
* Request handlers factories inherit from this interface: https://github.com/RafiHartounian/Web-Server/blob/main/include/handler_factories/request_handler_factory.h 
* An example handler factory for static requests can be found here: https://github.com/RafiHartounian/Web-Server/blob/main/src/handler_factories/static_handler_factory.cc
    * Factories take in an endpoint location and the entire config
    * Factories correspond to individual endpoint locations and are long-lived
* The create method in the factory interface produces an instance of a request handler
    * Request handlers are short-lived; they are created and destroyed per-request
* Request handlers inherit from this interface: https://github.com/RafiHartounian/Web-Server/blob/main/include/request_handler.h
* An example handler for static requests can be found here: https://github.com/RafiHartounian/Web-Server/blob/main/src/request_static_handler.cc
    * Note that all handlers must implement a handle_request method that takes a bhttp:request object and bhttp:response object by reference (the response is effectively returned here).