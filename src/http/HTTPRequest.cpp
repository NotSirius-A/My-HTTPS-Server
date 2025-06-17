#include "HTTPRequest.h"

void HTTPRequest::load_buffer(std::array<char, IN_BUF_SIZE>& recv_buf) {
    this->_req_string += std::string(recv_buf.data());
}

void HTTPRequest::erase_buffer() {
    this->_req_string.clear();
}

const std::string HTTPRequest::get_buffer() const {
    return this->_req_string;
}

void HTTPRequest::_parse_headers(std::unordered_map<std::string, std::string>& headers, std::string& headers_str) const {
    std::string& h = headers_str;

    while (h.find("\r\n") != std::string::npos) {
        std::string header = h.substr(0, h.find("\r\n"));

        std::string key = header.substr(0, header.find(": "));
        std::string value = header.substr(header.find(": ")+2);
        headers[key] = value;

        h.erase(0, h.find("\r\n") + 2);
    }
}

void HTTPRequest::parse(HTTPParsed& parsed) const{
    parsed.is_valid = false;

    const std::string &r = this->_req_string;
    size_t index, begin_index;

    begin_index = 0;
    index = r.find(" ", begin_index);
    if (index != std::string::npos) {
        parsed.method = r.substr(0, index);
    } else {
        return;
    }
    
    begin_index = index + 1;
    index = r.find(" ", begin_index);
    if (index != std::string::npos) {
        parsed.path = r.substr(begin_index, index-begin_index);
    } else {
        return;
    }

    begin_index = index + 1;
    index = r.find("\r\n", begin_index);
    if (index != std::string::npos) {
        parsed.http_version = r.substr(begin_index,  index-begin_index);
    } else {
        return;
    }

    begin_index = index + 2;
    index = r.find("\r\n\r\n", begin_index);
    if (index != std::string::npos) {
        std::string headers_str = r.substr(begin_index,  index-begin_index);

        _parse_headers(parsed.headers, headers_str);
    } else {
        return;
    }
    
    begin_index = index + 4;
    parsed.payload = r.substr(begin_index);

    
    parsed.is_valid = true;
    return;

}

