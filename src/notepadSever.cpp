#include <App.h>
#include <iostream>
#include <fstream>
using namespace std;

struct PerSocketData
{
};

using websocket = uWS::WebSocket<false, true, PerSocketData>;

std::string shared_doc = "Welcome to Collobrative-notepad";

unordered_set<websocket *> clients;

std::string readFile(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file)
    {
        std::cerr << "Error opening file: " << filepath << std::endl;
        return "";
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}
int main()
{

    uWS::App()
        .get("/", [](auto *res, auto *req)
             {
                 std::string url = std::string(req->getUrl());
                 std::string content;

                 if (url == "/")
                 {
                    std::cout<<"Requested index.html"<<std::endl;
                    content = readFile("../apps/index.html");
                   res->writeHeader("Content-Type", "text/html")->end(content);
                 }
                 else
                 {
                     res->writeStatus("404 Not found")->end("404: File not found");
                 } })
        .ws<PerSocketData>("/", {// Designated initializers
                                 .open = [](websocket *ws)
                                 {
                                    //@ callback when the client connects
                                     std::cout << "Clinet got connected to websocket" << std::endl;
                                     ws->send(shared_doc, uWS::OpCode::TEXT);
                                     clients.insert(ws); },

                                 .message = [](websocket *ws, std::string_view message, uWS::OpCode opCode)
                                 {
                                     //@ callback - when message arrived from the client
                                     //! ws - socket_fd of client
                                     //! str - message from client
                                     //! opCode - Data is sent in Text/Binary format

                                     //@ what do we do here
                                     //! Read the message and broadcast the message to all the other clients
                                     shared_doc = std::string(message);

                                     for(auto* client : clients)
                                     {
                                        if(client != ws)
                                        {
                                            client->send(message, uWS::OpCode::TEXT);
                                        }
                                     } },
                                 .close = [](websocket *ws, int, std::string_view)
                                 {
                                     //@ callback - when the client gets disconnected
                                     std::cout << "Clinet got connected to websocket" << std::endl;
                                     clients.erase(ws); }})
        .listen(9001, [](auto *listen_socket)
                {
                //@ TCP server listening over the socket 9001
                if(listen_socket != NULL)
                {
                    std::cout << "Server listening over the socket 9001" << std::endl;
                }
                else
                {
                    std::cerr << "Failed to start server!" << std::endl;
                } })
        .run();
}
