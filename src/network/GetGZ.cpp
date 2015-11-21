#include "GetGZ.h"

GetGZ::GetGZ() {
    rgxSize.assign(R"(.*Content-Length: (\d+)\r\nuuid_perft: (.+)\r\nuuid_task: (.*)\r.*)");
}

pair<string, string> GetGZ::get(const string &host, const int port, const string &url, const string &dataDir) {
    char buf[4096];
    debug("resolving host ", host);
    string ip = ResolveHost::getIP(host);
    if (!ip.size()) {
        error("unknow host ", host);
        return pair<string, string>("", "");
    }

    debug("resolved host ", host, " -> ", ip);
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(1,1), &wsaData) == SOCKET_ERROR) {
        error ("Error initialising WSA");
        return pair<string, string>("", "");
    }
#endif
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock != -1);
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    assert(connect(sock, (struct sockaddr *) &server, sizeof(server)) >= 0);

    std::ostringstream formBuffer;
    formBuffer << "GET /" << url << " HTTP/1.1\n";
    formBuffer << "Content-Type: application/x-gzip\n";
    formBuffer << "Host: " << host << "\n\n";

    string str = formBuffer.str();
    assert(send(sock, str.c_str(), str.size(), 0) == (int) str.size());
    int r;
    string receiveStd;
    int totBytes = -1;
    int totWritten = 0;
    string deb;
    string UUID_PERFT;
    string UUID_TASK;

    char *gzBuf = nullptr;

    ofstream tmp;
    string fileGzipped;
    while (1) {
#ifdef DEBUG_MODE
        memset(buf, 0, sizeof(buf));
#endif
        r = recv(sock, buf, sizeof(buf) - 1, 0);
        if (r <= 0)
            break;

#ifdef DEBUG_MODE
        deb.assign(buf);
#endif
        if (totBytes == -1) {
            buf[r] = 0;
            receiveStd.append(buf);
            std::smatch match;
            if (regex_search(receiveStd, match, rgxSize) && match.size() > 1) {
                string tot = match[1].str();
                UUID_PERFT = match[2].str();
                UUID_TASK = match[3].str();
                totBytes = std::stoi(tot);
                string ini = dataDir + PATH_SEPARATOR + UUID_PERFT + PATH_SEPARATOR + UUID_PERFT + ".ini";
                if (FileUtil::fileExists(ini)) {
                    info("file ", ini, " exists skip fetch")
                    return pair<string, string>(UUID_PERFT, UUID_TASK);
                }
                info("fetching...");
                FileUtil::createDirectory(dataDir + PATH_SEPARATOR + UUID_PERFT);
                fileGzipped = dataDir + PATH_SEPARATOR + UUID_PERFT + PATH_SEPARATOR + UUID_PERFT + ".ini.gz";
                tmp.open(fileGzipped,ios_base::out|ios_base::binary);
            } else {
                debug("no data fetched");
                return pair<string, string>("", "");
            }

            char *h = strstr(buf, (const char *) GZIP_HEADER);
            if (!h) {
                error("error on fatching data");
                return pair<string, string>("", "");
            }
            totWritten = r - (h - buf);
            tmp.write(h, totWritten);
            continue;
        }

        if (totWritten + r > totBytes) {
            int k = totBytes - totWritten;
            tmp.write(buf, k);
            totWritten += k;
            info("fetching tot", totWritten, "bytes");
            break;
        }
        info("fetching...", r, " bytes...");
        totWritten += r;
        tmp.write(buf, r);
    }


    info("fetched ", totWritten, " bytes");
    if (!tmp.is_open()) {
        return pair<string, string>("", "");
    }
    tmp.close();
    Compression compression;
    info("decompress..");
    compression.decompress(fileGzipped);

    //std::remove(fileGzipped.c_str());

    return pair<string, string>(UUID_PERFT, UUID_TASK);
}

