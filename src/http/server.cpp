#include "../main.h"
#include "server.h"

#include "../utils.h"

// #include "../connections/connection.h"





void handle_request(Connection conn, std::array<char, IN_BUF_SIZE>& recv_buf) {
    std::cout << "received: " << std::string(recv_buf.data()) << std::endl;
    
    HTTPRequest request;
    request.load_buffer(recv_buf);

    HTTPParsed parsed;
    request.parse(parsed);

    if (conn.is_conn_ssl() == false) {
        std::string location = "https://" + SERVER_HOST + ":" + std::to_string(PORT_HTTPS) + parsed.path;
        location += "\r\n\r\n";
        
        respond_redirect(conn, location);
        return;
    }


    namespace fs = std::filesystem;
    fs::path p = fs::current_path();

    if (parsed.path == "/") {
        p.append("site/index.html");
    } else {
        p.append("site/" + parsed.path);
    }



    std::array<char, OUT_BUF_SIZE> out_buf;

    std::ifstream f;
    f.open(p);

    if (f.good()) {
        uintmax_t size = std::filesystem::file_size(p);

        std::string extension = p.extension();
        std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c){ return std::tolower(c); });

        std::string mime_type = "text/html";

        if (extension == ".css") {
            mime_type = "text/css";
        } else if (extension == ".js" || extension == ".min.js") {
            mime_type = "text/javascript";
        } else if (extension == ".png" || extension == ".webp" || extension == ".jpg" || extension == ".jpeg") {
            mime_type = "image/" + extension;
        }


        std::string headers = "HTTP/1.1 200 OK\r\nContent-Type:" + mime_type + "\r\nContent-Length: ";
        headers += std::to_string(size);
        headers += "\r\n\r\n";
        std::array<char, OUT_BUF_SIZE> out_buf = {};
        headers.copy(out_buf.data(), headers.size());

        size_t msg_len = strlen(out_buf.data());
        conn.sendall(out_buf, msg_len);

        while (f.eof() == false) {
            f.read(out_buf.data(), OUT_BUF_SIZE);
            conn.sendall(out_buf, out_buf.size());
            out_buf.fill(0);
        }
    } else {
        respond_HTTPcode(conn, 404);
    }
    
    f.close();



    
}






void respond_HTTPcode(Connection conn, int HTTPcode) {
    std::string headers = "HTTP/1.1 " + std::to_string(HTTPcode) + " \r\nContent-Type: text/html\r\nContent-Length: 0";
    headers += "\r\n\r\n"; 
    std::array<char, OUT_BUF_SIZE> out_buf = {};
    headers.copy(out_buf.data(), headers.size());

    size_t msg_len = strlen(out_buf.data());

    conn.sendall(out_buf, msg_len);
}

void respond_redirect(Connection conn, std::string& location) {
    std::string headers = "HTTP/1.1 302 OK\r\nContent-Type:text/html\r\nContent-Length:0\r\nLocation:" + location;
    headers += "\r\n\r\n";
    std::array<char, OUT_BUF_SIZE> out_buf = {};
    headers.copy(out_buf.data(), headers.size());

    size_t msg_len = strlen(out_buf.data());

    conn.sendall(out_buf, msg_len);
}