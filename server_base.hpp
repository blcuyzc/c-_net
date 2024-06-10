#ifndef SERVER_BASE_HPP
#define SERVER_BASE_HPP

#include <boost/asio.hpp>
#include <regex>
#include <unordered_map>
#include <thread>

namespace ShiyanlouWeb {
    typedef std::map<std::string, std::unordered_map<std::string,
        std::function<void(std::ostream&, Request&)>>> resource_type;

    template <typename socket_type>
    class ServerBase {
    public:
        void start();
        resource_type resource;
        resource_type default_resource;
        ServerBase(unsigned short port, size_t num_threads=1) :
            endpoint(boost::asio::ip::tcp::v4(), port),
            acceptor(m_io_service, endpoint),
            num_threads(num_threads) {}
    protected:
        virtual void accept() {}
        void process_request_and_respond(std::shared_ptr<socket_type> socket) const;
        boost::asio::io_service m_io_service;
        boost::asio::ip::tcp::endpoint endpoint;
        boost::asio::ip::tcp::acceptor acceptor;
        size_t num_threads;
        std::vector<std::thread> threads;
        std::vector<resource_type::iterator> all_resources;
    };

    struct Request {
        std::string method, path, http_version;
        std::shared_ptr<std::istream> content;
        std::unordered_map<std::string, std::string> header;
        std::smatch path_match;
    };

    template<typename socket_type>
    class Server : public ServerBase<socket_type> {};
}

#endif // SERVER_BASE_HPP
