#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
using json = nlohmann::json;

void handle_request(http::request<http::string_body> const& req, http::response<http::string_body>& res) {
    if (req.find("ce-id") != req.end()) {
        try {
            auto body = json::parse(req.body());
            std::cout << "Received CloudEvent: " << body << std::endl;
            res.result(200);
            res.body() = "Event received";
        } catch (json::parse_error& e) {
            res.result(400);
            res.body() = "Invalid JSON";
        }
    } else {
        res.result(400);
        res.body() = "Not a CloudEvent";
    }
    res.version(req.version());
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "text/plain");
    res.prepare_payload();
}

int main() {
    try {
        boost::asio::io_context ioc{1};
        tcp::acceptor acceptor{ioc, {tcp::v4(), 8080}};
        for (;;) {
            tcp::socket socket{ioc};
            acceptor.accept(socket);
            http::request<http::string_body> req;
            http::read(socket, boost::beast::flat_buffer{}, req);
            http::response<http::string_body> res;
            handle_request(req, res);
            http::write(socket, res);
            socket.shutdown(tcp::socket::shutdown_send);
        }
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
