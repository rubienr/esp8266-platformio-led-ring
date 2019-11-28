#include "WebService.h"

#include <FS.h>

WebService::WebService(int16_t port) :
        ESP8266WebServer(port),
        updateServer(),
        mdns(MDNS)
{}

void WebService::init()
{
    mdns.begin("The Light of Frida");
    updateServer.setup(this, "/firmware");
    begin();
    MDNS.addService("http", "tcp", 80);

    on("/", [&]()
    {
        Serial.println("WebService::on: /");

             String info;
             info += "<html>";
        info += "  <h1> Firmware ";
        info += ESP.getSketchMD5();
        info += " size  ";
        info += ESP.getSketchSize();
        info += "  </h1> ";
        info += "  <a href=/status>device status</a><br/>";
        info += "  <a href=/firmware>upload firmware</a><br/>";
        info += "  <a href=/files>ls</a><br/>";
        info += "  <a href=/reboot>reboot (switch from wifi to normal mode)</a><br/>";
        info += "</html>";
        send(200, "text/html",info);
    });

    on("/files", [&]()
    {
        Serial.println("WebService::on: /files");
        Dir dir = SPIFFS.openDir("");
        String fileListing;
        while (dir.next())
        {
            File f = dir.openFile("r");
            fileListing.concat("name" + dir.fileName() + " size " + f.size() + "\n");
            f.close();
        }
        send(200, "text/plain", fileListing);
    });

    on("/reboot", [&]()
    {
        Serial.println("WebService::on: /reboot");
        ESP.restart();
    });

    on("/status", [&]()
    {
        Serial.println("WebService::on: /status");

        String status;
        status += "<html>";
        status += "shutdown reason ";
        status += ESP.getResetReason().c_str();

        status += "<br/>getVcc() ";
        status += ESP.getVcc();

        status += "getFreeHeap() ";
        status += ESP.getFreeHeap();

        status += "<br/>ESP.getMaxFreeBlockSize() ";
        status += ESP.getMaxFreeBlockSize();

        status += "<br/>ESP.getHeapFragmentation() "; // in %
        status += ESP.getHeapFragmentation(); // in %

        status += "<br/>ESP.getFreeContStack() ";
        status += ESP.getFreeContStack();

        status += "<br/>ESP.getSdkVersion() ";
        status += ESP.getSdkVersion();

        status += "<br/>ESP.getCoreVersion() ";
        status += ESP.getCoreVersion();

        status += "<br/>ESP.getFullVersion() ";
        status += ESP.getFullVersion();

        status += "<br/>ESP.getBootVersion() ";
        status += ESP.getBootVersion();

        status += "<br/>ESP. getBootMode() ";
        status += ESP.getBootMode();

        status += "<br/>ESP.getCpuFreqMHz() ";
        status += ESP.getCpuFreqMHz();

        status += "<br/>ESP. getFlashChipId() ";
        status += ESP.getFlashChipId();

        status += "<br/>ESP. getFlashChipVendorId() ";
        status += ESP.getFlashChipVendorId();

        status += "<br/>ESP.getFlashChipRealSize() ";
        status += ESP.getFlashChipRealSize();

        status += "<br/>ESP. getFlashChipSize() ";
        status += ESP.getFlashChipSize();

        status += "<br/>ESP.getFlashChipSpeed() ";
        status += ESP.getFlashChipSpeed();

        status += "<br/>ESP.getFlashChipMode() ";
        status += ESP.getFlashChipMode();

        status += "<br/>ESP. getFlashChipSizeByChipId() ";
        status += ESP.getFlashChipSizeByChipId();

        status += "<br/>ESP.getSketchSize() ";
        status += ESP.getSketchSize();

        status += "<br/>ESP.getSketchMD5() ";
        status += ESP.getSketchMD5();

        status += "<br/>ESP.getFreeSketchSpace() ";
        status += ESP.getFreeSketchSpace();

        status += "<br/>ESP.getResetReason() ";
        status += ESP.getResetReason();

        status += "<br/>ESP.getResetInfo() ";
        status += ESP.getResetInfo();

        status += "</html>";

        send(200, "text/html", status);
    });
}
