#include <iostream>
#include <string>

// for time control
#include <chrono>
#include <thread>

// ugv
//#include "../include/ugv.h"

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/StreamCopier.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include <iostream>


#include "Poco/Net/HTTPMessage.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/HTMLForm.h"

#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Stringifier.h"
#include "Poco/JSON/Object.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/JSON/Parser.h"

using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;

std::string Request_Post(URI& url, const std::string& sub_path, const Poco::JSON::Object& obj) {
    try {
        // TODO: prepare session.

        HTTPClientSession session(url.getHost(), url.getPort());
        session.setKeepAlive(true);
        // TODO: prepare path.
        url.setPath(sub_path);
        std::string path(url.getPathAndQuery());
        if (path.empty()) path = "/";

        // TODO: send request.

        HTTPRequest req(HTTPRequest::HTTP_POST, path, HTTPMessage::HTTP_1_1);

        req.setChunkedTransferEncoding(false);
        req.setContentType("application/json");
        req.setCredentials("Basic", "ZGlzdHJpYnV0b3I6NjJmMmYwZjFlZmYxMGQzMTUyYzk1ZjZmMDU5NjU3NmU0ODJiYjhlNDQ4MDY0MzNmNGNmOTI5NzkyODM0YjAxNA==");


        // TODO: set header here. For trans200, there is no need to set header.
        //

        // TODO: set request body.  obj

        std::ostringstream ss;
        obj.stringify(ss);
        std::string body;
        body = ss.str();
        req.setContentLength(body.length());

        // TODO: sends request, returns open stream.

        session.sendRequest(req) << body;

        //        req.write(std::cout); // print out request

        // TODO: get response.

        HTTPResponse res;
        std::cout << "POST_Status: " << res.getStatus() << " " << res.getReason() << std::endl;   // 200 OK.

        std::istream& is = session.receiveResponse(res);
        std::string result;
        StreamCopier::copyToString(is, result);
        std::cout << result << "\n";

        return  result;
    }

    catch (Poco::Net::NetException& ex) {
        auto result = ex.displayText();
        std::cout << "result: " << result;
        return "";
    }
}

std::string Request_Get(URI& url, const std::string& sub_path) {
    try {

        // TODO: prepare session.

        HTTPClientSession session(url.getHost(), url.getPort());

        // TODO: prepare path.
        url.setPath(sub_path);
        std::string path(url.getPathAndQuery());
        if (path.empty()) path = "/";

        // TODO: send request.


        HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
        req.setContentType("application/json");

        // TODO: set header here. For trans200, there is no need to set header.
        //

        // TODO: set request body, skipped.
        //

        // TODO: sends request, returns open stream.

        std::ostream& os = session.sendRequest(req);
        //        req.write(std::cout); // print out the

        // TODO: get response.

        HTTPResponse res;
        std::cout << "GET_Status: " << res.getStatus() << " " << res.getReason() << std::endl;

        std::istream& is = session.receiveResponse(res);
        std::string result;
        StreamCopier::copyToString(is, result);
        std::cout << result << "\n";

        return  result;

    }
    catch (Poco::Net::NetException& ex) {
        auto result = ex.displayText();
        std::cout << "result: " << result;
        return "";
    }
}

bool doRequest(Poco::Net::HTTPClientSession& session, Poco::Net::HTTPRequest& request, Poco::Net::HTTPResponse& response)
{
    session.sendRequest(request);
    std::istream& rs = session.receiveResponse(response);
    std::cout << response.getStatus() << " " << response.getReason() << std::endl;
    if (response.getStatus() != Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
    {
//        StreamCopier::copyStream(rs, std::cout);
        std::string result;
        StreamCopier::copyToString(rs, result);
        std::cout << "result: " << result << std::endl;
        return true;
    }
    else
    {
        Poco::NullOutputStream null;
        StreamCopier::copyStream(rs, null);
        return false;
    }
}

bool GetMethod(URI& uri, const std::string& sub_path)
{
    try
    {
        uri.setPath(sub_path);

        std::string path(uri.getPathAndQuery());
        if (path.empty()) path = "/";

        HTTPClientSession session(uri.getHost(), uri.getPort());
        HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
        request.setContentType("application/json");
        request.setCredentials("Basic", "ZGlzdHJpYnV0b3I6NjJmMmYwZjFlZmYxMGQzMTUyYzk1ZjZmMDU5NjU3NmU0ODJiYjhlNDQ4MDY0MzNmNGNmOTI5NzkyODM0YjAxNA==");

        HTTPResponse response;

        doRequest(session, request, response);
    }

    catch (Exception& exc)
    {
        std::cerr << exc.displayText() << std::endl;
        return false;
    }
}

bool PostMethod(URI& uri, const std::string& sub_path, const Poco::JSON::Object& obj)
{
    try
    {
        uri.setPath(sub_path);

        std::string path(uri.getPathAndQuery());
        if (path.empty()) path = "/";

        HTTPClientSession session(uri.getHost(), uri.getPort());
        HTTPRequest request(HTTPRequest::HTTP_POST, path, HTTPMessage::HTTP_1_1);
        request.setContentType("application/json");
        request.setCredentials("Basic", "ZGlzdHJpYnV0b3I6NjJmMmYwZjFlZmYxMGQzMTUyYzk1ZjZmMDU5NjU3NmU0ODJiYjhlNDQ4MDY0MzNmNGNmOTI5NzkyODM0YjAxNA==");

        HTTPResponse response;

        std::ostringstream ss;
        obj.stringify(ss);
        std::string body;
        body = ss.str();
        request.setContentLength(body.length());
        session.sendRequest(request) << body;

        doRequest(session, request, response);
    }

    catch (Exception& exc)
    {
        std::cerr << exc.displayText() << std::endl;
        return false;
    }
}

int main(int argc, char** argv)
{
    URI uri("http://192.168.2.111");
    std::string sub_path  = "/api/v2.0.0/registers/102";

    GetMethod(uri,sub_path);

    Poco::JSON::Object Register;

    float value = 1024.1024f;
    Register.set("value", value);

//    PostMethod(uri,sub_path,Register);

    Request_Post(uri,sub_path,Register);
#if 0
    try
    {
        URI uri("http://192.168.2.111/api/v2.0.0/registers/1");
        std::string path(uri.getPathAndQuery());
        if (path.empty()) path = "/";

        HTTPClientSession session(uri.getHost(), uri.getPort());
        HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
        request.setContentType("application/json");
        request.setCredentials("Basic", "ZGlzdHJpYnV0b3I6NjJmMmYwZjFlZmYxMGQzMTUyYzk1ZjZmMDU5NjU3NmU0ODJiYjhlNDQ4MDY0MzNmNGNmOTI5NzkyODM0YjAxNA==");

        HTTPResponse response;

        doRequest(session, request, response);

    }
    catch (Exception& exc)
    {
        std::cerr << exc.displayText() << std::endl;
        return 1;
    }

#endif




    return 0;
}