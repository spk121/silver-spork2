/*
 * Simple HTTP Server using CivetWeb
 * 
 * This server provides a skeleton implementation suitable for Fedora,
 * designed to work behind a Caddy reverse proxy that handles HTTPS.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "civetweb.h"

static struct mg_context *ctx = NULL;

/* Simple HTML responses */
static const char *INDEX_HTML =
    "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "<head>\n"
    "    <meta charset=\"UTF-8\">\n"
    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
    "    <title>Silver Spork 2</title>\n"
    "    <style>\n"
    "        body {\n"
    "            font-family: Arial, sans-serif;\n"
    "            max-width: 800px;\n"
    "            margin: 50px auto;\n"
    "            padding: 20px;\n"
    "            background-color: #f5f5f5;\n"
    "        }\n"
    "        h1 {\n"
    "            color: #333;\n"
    "        }\n"
    "        p {\n"
    "            color: #666;\n"
    "            line-height: 1.6;\n"
    "        }\n"
    "    </style>\n"
    "</head>\n"
    "<body>\n"
    "    <h1>Welcome to Silver Spork 2</h1>\n"
    "    <p>Hello, World! This is a skeleton HTTP server built with CivetWeb.</p>\n"
    "    <p>This server is designed to run behind a Caddy reverse proxy for HTTPS support.</p>\n"
    "</body>\n"
    "</html>\n";

static const char *NOT_FOUND_HTML =
    "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "<head>\n"
    "    <meta charset=\"UTF-8\">\n"
    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
    "    <title>404 Not Found</title>\n"
    "    <style>\n"
    "        body {\n"
    "            font-family: Arial, sans-serif;\n"
    "            max-width: 800px;\n"
    "            margin: 50px auto;\n"
    "            padding: 20px;\n"
    "            background-color: #f5f5f5;\n"
    "        }\n"
    "        h1 {\n"
    "            color: #d32f2f;\n"
    "        }\n"
    "        p {\n"
    "            color: #666;\n"
    "        }\n"
    "    </style>\n"
    "</head>\n"
    "<body>\n"
    "    <h1>404 - Page Not Found</h1>\n"
    "    <p>The requested page could not be found on this server.</p>\n"
    "    <p><a href=\"/\">Return to home page</a></p>\n"
    "</body>\n"
    "</html>\n";

/*
 * Handler for the index page (/ and /index.html)
 * Returns 1 if request was handled, 0 otherwise
 */
static int handle_index(struct mg_connection *conn, void *cbdata)
{
    (void)cbdata;  /* Unused parameter */
    
    mg_printf(conn,
              "HTTP/1.1 200 OK\r\n"
              "Content-Type: text/html\r\n"
              "Content-Length: %zu\r\n"
              "\r\n"
              "%s",
              strlen(INDEX_HTML), INDEX_HTML);
    return 1;
}

/*
 * Handler for 404 errors
 * Returns 1 if request was handled, 0 otherwise
 */
static int handle_404(struct mg_connection *conn, void *cbdata)
{
    (void)cbdata;  /* Unused parameter */
    
    mg_printf(conn,
              "HTTP/1.1 404 Not Found\r\n"
              "Content-Type: text/html\r\n"
              "Content-Length: %zu\r\n"
              "\r\n"
              "%s",
              strlen(NOT_FOUND_HTML), NOT_FOUND_HTML);
    return 1;
}

/*
 * Main request handler - routes requests to appropriate handlers
 * Returns 1 if request was handled, 0 otherwise
 */
static int handle_request(struct mg_connection *conn, void *cbdata)
{
    (void)cbdata;  /* Unused parameter */
    
    const struct mg_request_info *req_info = mg_get_request_info(conn);
    const char *uri = req_info->request_uri;
    
    /* Handle index routes */
    if (strcmp(uri, "/") == 0 || strcmp(uri, "/index.html") == 0) {
        return handle_index(conn, NULL);
    }
    
    /* All other routes get 404 */
    return handle_404(conn, NULL);
}
static void signal_handler(int signum)
{
    (void)signum;  /* Unused parameter */
    
    if (ctx) {
        printf("\nShutting down server...\n");
        mg_stop(ctx);
        ctx = NULL;
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    const char *port = "8080";
    
    /* Allow port to be specified via command line argument */
    if (argc > 1) {
        port = argv[1];
    }

    printf("Silver Spork 2 - CivetWeb HTTP Server\n");
    printf("======================================\n");
    printf("Starting server on http://localhost:%s\n", port);
    printf("Press Ctrl+C to stop the server\n\n");

    /* Set up signal handlers for graceful shutdown */
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    /* Configure server options */
    const char *options[] = {
        "listening_ports", port,
        "num_threads", "4",
        NULL
    };

    /* Initialize CivetWeb */
    mg_init_library(0);
    
    /* Start the server */
    ctx = mg_start(NULL, 0, options);
    if (ctx == NULL) {
        fprintf(stderr, "Error: Failed to start CivetWeb server\n");
        mg_exit_library();
        return 1;
    }

    /* Register main request handler for all routes */
    mg_set_request_handler(ctx, "**", handle_request, NULL);

    printf("Server started successfully!\n");
    printf("Endpoints:\n");
    printf("  GET /           -> Index page\n");
    printf("  GET /index.html -> Index page\n");
    printf("  GET /*          -> 404 page\n\n");

    /* Keep the server running */
    while (ctx) {
        sleep(1);
    }

    /* Cleanup */
    mg_exit_library();
    
    return 0;
}
