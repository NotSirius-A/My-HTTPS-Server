#include "../main.h"
#include <unordered_map>


struct HTTPParsed {
    bool is_valid;
    std::string method;
    std::string path;
    std::string http_version;
    std::unordered_map<std::string, std::string> headers;
    std::string payload;  
};

class HTTPRequest {       
    public:            
        void load_buffer(std::array<char, IN_BUF_SIZE>& recv_buf);
        const std::string get_buffer() const;
        void erase_buffer();
        void parse(HTTPParsed& parsed) const;
    private:
        void _parse_headers(std::unordered_map<std::string, std::string>& headers, std::string& headers_str) const;
        std::string _req_string;    
};