# WWW

WWW is a static HTTP server for Linux (should work on other Unix systems)

WWW stands for Worst Web server in the World

I did this program for a school project. It was fun so maybe I'll dig in deeper at a later time

## Dependencies

Nothing really. Just the C++ standard library.

## Installation

```
cd build
make
```

## Configuration

The default config file demonstrates all the configuration options.

- pool_size sets the number of threads that can handle requests
- port_number sets the port number
- backlog_size sets the number of connections the kernel will hold while the main thread is busy

Starting a line with '#' will make it a comment

## License
MIT License