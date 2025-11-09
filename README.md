# Silver Spork 2

In which I hate Javascript and do something very silly.

## Overview

A lightweight HTTP server built with CivetWeb, designed to run on Fedora Linux behind a Caddy reverse proxy for HTTPS support.

## Technologies

- **CivetWeb** - Embedded C HTTP server (https://github.com/civetweb/civetweb)
- Markdown MD4C https://github.com/mity/md4c
- Mustache for C https://github.com/x86-64/mustache-c/
- cJSON or libxml
- ImageMagick for image resizing and rotation, thumbnails
- GCGI
- **Caddy** - For path remapping, reverse proxies, HTTPS, and SSH

## Building and Running on Fedora

### Prerequisites

This project requires CivetWeb development files and standard build tools. On Fedora, install them with:

```bash
make install-deps
```

Or manually:

```bash
sudo dnf install -y civetweb-devel gcc make
```

### Building

Build the server:

```bash
make
```

### Running

Start the server on the default port (8080):

```bash
./server
```

Or specify a custom port:

```bash
./server 3000
```

The server will be accessible at `http://localhost:8080` (or your specified port).

### Endpoints

- `GET /` - Returns the index HTML page
- `GET /index.html` - Returns the index HTML page
- `GET /*` - Any other path returns a 404 page

### Behind Caddy Reverse Proxy

This server is designed to run locally and have HTTPS handled by Caddy. Example Caddyfile configuration:

```
your-domain.com {
    reverse_proxy localhost:8080
}
```

The server handles HTTP only; Caddy handles TLS termination and proxies requests to the local server.

## Development

### Project Structure

```
.
├── server.c      - Main server implementation
├── Makefile      - Build configuration for Fedora
└── README.md     - This file
```

### Extending the Server

The server is structured for easy extension:

1. **Adding new routes**: Use `mg_set_request_handler()` to register handlers for specific paths
2. **Custom handlers**: Follow the pattern in `handle_index()` and `handle_404()`
3. **Static files**: CivetWeb supports serving static files from a directory
4. **POST handlers**: Add handlers that check `mg_get_request_info()->request_method`

Example of adding a new route:

```c
static int handle_about(struct mg_connection *conn, void *cbdata)
{
    const char *html = "<html><body><h1>About Page</h1></body></html>";
    mg_printf(conn,
              "HTTP/1.1 200 OK\r\n"
              "Content-Type: text/html\r\n"
              "Content-Length: %zu\r\n\r\n%s",
              strlen(html), html);
    return 1;
}

// In main(), before the 404 handler:
mg_set_request_handler(ctx, "/about", handle_about, NULL);
```

### Cleaning Build Artifacts

```bash
make clean
```

### Getting Help

```bash
make help
```

## License

This project is open source. See individual component licenses for details.
