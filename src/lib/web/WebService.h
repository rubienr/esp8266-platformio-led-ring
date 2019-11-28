#pragma once

#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>

class Print;

class WebService : public ESP8266WebServer
{
private:

protected:
    ESP8266HTTPUpdateServer updateServer;
    MDNSResponder& mdns;

public:
    explicit WebService(int16_t port = 80);
    void init();
};

