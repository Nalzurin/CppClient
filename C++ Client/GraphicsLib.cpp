#pragma comment(lib, "Ws2_32.lib")

#pragma warning( disable : 4996)
#include <iostream>
#include "GraphicsLib.h"
#include <winsock2.h>
#include <stdlib.h> 
#include <string.h>
#include <vector>
#include <WS2tcpip.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
using namespace cv;
using namespace std;
class graphicsLib : GraphicsLib
{

private:

    int_least16_t width, height; //screen size
    string ipaddr;
    int port;
    SOCKET out;
public:


    int ClientStartup()
    {
        //Create and Open Socket

        WSADATA wsaData;
        int ws = WSAStartup(MAKEWORD(2, 2), &wsaData);
        out = socket(AF_INET, SOCK_DGRAM, 0);
        if (ws != 0)
        {
            cout << "Windows Socket Creation failed:" << socket;
            return 1;

        }

        return 0;
    }

    uint_least16_t ColorInput()
    {
        int_least16_t r, g, b;
        uint_least16_t output;
        cout << "Input red value(0-255): \n";
        cin >> r;
        cout << "Input green value(0-255): \n";
        cin >> g;
        cout << "Input blue value(0-255): \n";
        cin >> b;
        output = R_G_B(r, g, b);
        return output;
    }
    void ClientShutdown()
    {
        closesocket(out);
        // Close down Winsock   
        WSACleanup();
    }

    // Write out to that socket
    void SendMessageServer(vector<byte> v, int length)
    {

        sockaddr_in serveraddr;
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(port);
        inet_pton(AF_INET, ipaddr.c_str(), &serveraddr.sin_addr);
        int sendOk = sendto(out, (char*)v.data(), length, 0, (sockaddr*)&serveraddr, sizeof(serveraddr));
        if (sendOk == SOCKET_ERROR)
        {
            cout << "That didn't work! " << WSAGetLastError() << endl;
        }

    }

    char* ListenToServerString()
    {
        char buffer[10000];
        sockaddr_in serveraddr;
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(port);
        inet_pton(AF_INET, ipaddr.c_str(), &serveraddr.sin_addr);
        int receiveOk = recvfrom(out, (char*)buffer, 1024, 0, NULL, NULL);
        buffer[receiveOk] = '\0';
        cout << buffer << endl;
        return buffer;

    }
    char* ListenToServerBytes()
    {
        char buffer[10000];
        sockaddr_in serveraddr;
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(port);
        inet_pton(AF_INET, ipaddr.c_str(), &serveraddr.sin_addr);
        int receiveOk = recvfrom(out, (char*)buffer, 1024, 0, NULL, NULL);
        buffer[receiveOk] = '\0';
        return buffer;

    }


    void SetScreenSize()
    {
        byte widthArr[] = { width & 0x00FF, width >> 8 };
        byte heightArr[] = { height & 0x00FF, height >> 8 };
        vector<byte> v;
        v.resize(0);
        v.push_back(254);
        v.push_back(widthArr[0]);
        v.push_back(widthArr[1]);
        v.push_back(heightArr[0]);
        v.push_back(heightArr[1]);
        SendMessageServer(v, v.size());
        ListenToServerString();
    }

   
    char* ConvertImage(string path, short& width, short& height)
    {
        string pth(path);

        Mat image = imread("D:/Data/Uni/Year3/Part1/CSCS/Lab3/Images/" + pth);
        char* arr = new char[image.rows * image.cols * 3];
        int k = 0;
        Vec3b buf;
        width = image.cols;
        height = image.rows;
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                buf = image.at<Vec3b>(j, i);
                arr[k] = buf[2];
                arr[k + 1] = buf[1];
                arr[k + 2] = buf[0];
                k += 3;
            }
        }
        return arr;


    }
    graphicsLib(uint_least16_t _width, uint_least16_t _height) {

        width = _width;

        height = _height;

    };

    graphicsLib() {

        width = 0;

        height = 0;

    };


    int_least16_t getWidth(void) 
    {
        
        vector<byte> v;
        v.resize(0);
        v.push_back(15);
        SendMessageServer(v, v.size());
        char* b = ListenToServerBytes();
        memcpy(&width, b, sizeof(width));
        return width;
    };
    int_least16_t getHeight(void)
    { 
        vector<byte> v;
        v.resize(0);
        v.push_back(16);
        SendMessageServer(v, v.size());
        char* b = ListenToServerBytes();
        memcpy(&height, b, sizeof(height));
        return height;
    };

    string getIP(void) { return ipaddr; };
    int getPort(void) { return port; };
    void setIP(string _ipaddr)
    {
        ipaddr = _ipaddr;
    }
    void setPort(int _port)
    {
        port = _port;
    }


    void fillScreen(uint_least16_t color) override { //clear display


        vector<byte> v;
        v.resize(0);
        v.push_back(1);
        uint_least16_t r = (color & 0xF800) >> 8;
        uint_least16_t g = (color & 0x07E0) >> 3;
        uint_least16_t b = (color & 0x001F) << 3;
        v.push_back(r);
        v.push_back(g);
        v.push_back(b);

        SendMessageServer(v, v.size());
        ListenToServerString();
    }

    void SetRotation(int_least16_t rot)
    {
        vector<byte> v;
        v.resize(0);
        v.push_back(14);
        byte rotArr[] = { rot & 0x00FF, rot >> 8};
        v.push_back(rotArr[0]);
        v.push_back(rotArr[1]);
        SendMessageServer(v, v.size());
        ListenToServerString();
    }

    void drawPixel(int_least16_t x0, int_least16_t y0, uint_least16_t color) override
    {
        byte x0Arr[] = { x0 & 0x00FF, x0 >> 8 };
        byte y0Arr[] = { y0 & 0x00FF, y0 >> 8 };
        vector<byte> v;
        v.resize(0);
        v.push_back(2);
        v.push_back(x0Arr[0]);
        v.push_back(x0Arr[1]);
        v.push_back(y0Arr[0]);
        v.push_back(y0Arr[1]);
        uint_least16_t r = (color & 0xF800) >> 8;
        uint_least16_t g = (color & 0x07E0) >> 3;
        uint_least16_t b = (color & 0x001F) << 3;
        v.push_back(r);
        v.push_back(g);
        v.push_back(b);
        SendMessageServer(v, v.size());
        ListenToServerString();
    }
    void drawLine(int_least16_t x0, int_least16_t y0, int_least16_t x1, int_least16_t y1, uint_least16_t color) override
    {
        uint_least16_t r = (color & 0xF800) >> 8;
        uint_least16_t g = (color & 0x07E0) >> 3;
        uint_least16_t b = (color & 0x001F) << 3;
        byte x0Arr[] = { x0 & 0x00FF, x0 >> 8 };
        byte y0Arr[] = { y0 & 0x00FF, y0 >> 8 };
        byte x1Arr[] = { x1 & 0x00FF, x1 >> 8 };
        byte y1Arr[] = { y1 & 0x00FF, y1 >> 8 };
        vector<byte> v;
        v.resize(0);
        v.push_back(3);
        v.push_back(x0Arr[0]);
        v.push_back(x0Arr[1]);
        v.push_back(y0Arr[0]);
        v.push_back(y0Arr[1]);
        v.push_back(x1Arr[0]);
        v.push_back(x1Arr[1]);
        v.push_back(y1Arr[0]);
        v.push_back(y1Arr[1]);
        v.push_back(r);
        v.push_back(g);
        v.push_back(b);
        SendMessageServer(v, v.size());
        ListenToServerString();
    }
    void drawRect(int_least16_t x0, int_least16_t y0, int_least16_t w, int_least16_t h, uint_least16_t color) override
    {
        uint_least16_t r = (color & 0xF800) >> 8;
        uint_least16_t g = (color & 0x07E0) >> 3;
        uint_least16_t b = (color & 0x001F) << 3;
        byte x0Arr[] = { x0 & 0x00FF, x0 >> 8 };
        byte y0Arr[] = { y0 & 0x00FF, y0 >> 8 };
        byte wArr[] = { w & 0x00FF, w >> 8 };
        byte hArr[] = { h & 0x00FF, h >> 8 };
        vector<byte> v;
        v.resize(0);
        v.push_back(4);
        v.push_back(x0Arr[0]);
        v.push_back(x0Arr[1]);
        v.push_back(y0Arr[0]);
        v.push_back(y0Arr[1]);
        v.push_back(wArr[0]);
        v.push_back(wArr[1]);
        v.push_back(hArr[0]);
        v.push_back(hArr[1]);
        v.push_back(r);
        v.push_back(g);
        v.push_back(b);
        SendMessageServer(v, v.size());
        ListenToServerString();
    }
    void fillRect(int_least16_t x0, int_least16_t y0, int_least16_t w, int_least16_t h, uint_least16_t color) override
    {
        uint_least16_t r = (color & 0xF800) >> 8;
        uint_least16_t g = (color & 0x07E0) >> 3;
        uint_least16_t b = (color & 0x001F) << 3;
        byte x0Arr[] = { x0 & 0x00FF, x0 >> 8 };
        byte y0Arr[] = { y0 & 0x00FF, y0 >> 8 };
        byte wArr[] = { w & 0x00FF, w >> 8 };
        byte hArr[] = { h & 0x00FF, h >> 8 };
        vector<byte> v;
        v.resize(0);
        v.push_back(5);
        v.push_back(x0Arr[0]);
        v.push_back(x0Arr[1]);
        v.push_back(y0Arr[0]);
        v.push_back(y0Arr[1]);
        v.push_back(wArr[0]);
        v.push_back(wArr[1]);
        v.push_back(hArr[0]);
        v.push_back(hArr[1]);
        v.push_back(r);
        v.push_back(g);
        v.push_back(b);
        SendMessageServer(v, v.size());
        ListenToServerString();
    }
    void drawEllipse(int_least16_t x0, int_least16_t y0, int_least16_t r_x, int_least16_t r_y, uint_least16_t color) override
    {
        uint_least16_t r = (color & 0xF800) >> 8;
        uint_least16_t g = (color & 0x07E0) >> 3;
        uint_least16_t b = (color & 0x001F) << 3;
        byte x0Arr[] = { x0 & 0x00FF, x0 >> 8 };
        byte y0Arr[] = { y0 & 0x00FF, y0 >> 8 };
        byte r_xArr[] = { r_x & 0x00FF, r_x >> 8 };
        byte r_yArr[] = { r_y & 0x00FF, r_y >> 8 };
        vector<byte> v;
        v.resize(0);
        v.push_back(6);
        v.push_back(x0Arr[0]);
        v.push_back(x0Arr[1]);
        v.push_back(y0Arr[0]);
        v.push_back(y0Arr[1]);
        v.push_back(r_xArr[0]);
        v.push_back(r_xArr[1]);
        v.push_back(r_yArr[0]);
        v.push_back(r_yArr[1]);
        v.push_back(r);
        v.push_back(g);
        v.push_back(b);
        SendMessageServer(v, v.size());
        ListenToServerString();
    }
    void fillEllipse(int_least16_t x0, int_least16_t y0, int_least16_t r_x, int_least16_t r_y, uint_least16_t color) override
    {
        uint_least16_t r = (color & 0xF800) >> 8;
        uint_least16_t g = (color & 0x07E0) >> 3;
        uint_least16_t b = (color & 0x001F) << 3;
        byte x0Arr[] = { x0 & 0x00FF, x0 >> 8 };
        byte y0Arr[] = { y0 & 0x00FF, y0 >> 8 };
        byte r_xArr[] = { r_x & 0x00FF, r_x >> 8 };
        byte r_yArr[] = { r_y & 0x00FF, r_y >> 8 };
        vector<byte> v;
        v.resize(0);
        v.push_back(7);
        v.push_back(x0Arr[0]);
        v.push_back(x0Arr[1]);
        v.push_back(y0Arr[0]);
        v.push_back(y0Arr[1]);
        v.push_back(r_xArr[0]);
        v.push_back(r_xArr[1]);
        v.push_back(r_yArr[0]);
        v.push_back(r_yArr[1]);
        v.push_back(r);
        v.push_back(g);
        v.push_back(b);
        SendMessageServer(v, v.size());
        ListenToServerString();
    }
    int_least16_t drawChar(int_least16_t x, int_least16_t y, char c, uint_least16_t color, uint_least16_t bg, uint_least8_t size) override
    {
        return x;
    }
    int_least16_t drawChar(int_least16_t x, int_least16_t y, unsigned char c, uint_least16_t color, uint_least16_t bg, uint_least8_t size) override
    {
        return x;
    }
    int_least16_t drawText(int_least16_t x, int_least16_t y, const char* s, uint_least16_t color, uint_least16_t bg, uint_least8_t size) override
    {
        uint_least16_t r = (color & 0xF800) >> 8;
        uint_least16_t g = (color & 0x07E0) >> 3;
        uint_least16_t b = (color & 0x001F) << 3;
        byte x0Arr[] = { x & 0x00FF, x >> 8 };
        byte y0Arr[] = { y & 0x00FF, y >> 8 };
        byte length[] = { bg & 0x00FF, bg >> 8 };
        byte font[] = { size & 0x00FF, size >> 8 };
        byte command;
        vector<byte> v;
        v.resize(0);
        v.push_back(19);
        v.push_back(x0Arr[0]);
        v.push_back(x0Arr[1]);
        v.push_back(y0Arr[0]);
        v.push_back(y0Arr[1]);
        v.push_back(r);
        v.push_back(g);
        v.push_back(b);
        v.push_back(font[0]);
        v.push_back(font[1]);
        v.push_back(length[0]);
        v.push_back(length[1]);
        for (int i = 0; i < bg; i++)
        {
            v.push_back(s[i]);
        }
        SendMessageServer(v, v.size());
        ListenToServerString();
        return x;
    }
    void loadSprite(uint_least8_t index, int_least16_t width, int_least16_t height, char* data) override
    {
        int k = 0;
        byte widthArr[] = { width & 0x00FF, width >> 8 };
        byte heightArr[] = { height & 0x00FF, height >> 8 };
        vector<byte> v;
        v.resize(0);
        v.push_back(17);
        v.push_back(index);
        v.push_back(widthArr[0]);
        v.push_back(widthArr[1]);
        v.push_back(heightArr[0]);
        v.push_back(heightArr[1]);
        for (int i = 0; i < width * height * 3; i++)
        {
            v.push_back(data[i]);
        }
        SendMessageServer(v, v.size());
        ListenToServerString();
    }
    void showSprite(uint_least8_t index, uint_least16_t x, uint_least16_t y) override
    {
        vector<byte> v;
        v.resize(0);
        v.push_back(18);
        byte indexArr[] = { index & 0x00FF, index >> 8 };
        byte xArr[] = { x & 0x00FF, x >> 8 };
        byte yArr[] = { y & 0x00FF, y >> 8 };
        v.push_back(indexArr[0]);
        v.push_back(indexArr[1]);
        v.push_back(xArr[0]);
        v.push_back(xArr[1]);
        v.push_back(yArr[0]);
        v.push_back(yArr[1]);
        SendMessageServer(v, v.size());
        ListenToServerString();
    }
    void UploadGIF(int startIndex, int Frames, string name, string extension)
    {
        string fullname;
        int k = 1;
        short widthImage = 0, heightImage = 0;
        char* data;
        for (int i = startIndex; i < startIndex + Frames; i++)
        {
            fullname = name;
            fullname.append("_");
            fullname.append(to_string(k));
            fullname.append(".");
            fullname.append(extension);
            data = ConvertImage(fullname, widthImage, heightImage);
            loadSprite(i, widthImage, heightImage, data);
            k++;

        }

    }
    void PlayGIF(int iterations, int startIndex, int Frames, uint_least16_t x, uint_least16_t y)
    {

        for (int j = 0; j < iterations; j++)
        {
            for (int i = startIndex; i < startIndex + Frames; i++)
            {
                fillScreen(R_G_B(255, 255, 255));
                showSprite(i, x, y);
                Sleep(100);
            }
        }
        
        
    }
    void DanceFloor(int iterations, uint_least16_t x, uint_least16_t y)
    {
        vector<byte> v;
        for (int i = 0; i < iterations; i++)
        {
            fillScreen(R_G_B(255, 255, 255));
            fillRect(x, y, 100, 100, R_G_B(rand() % 256, rand() % 256, rand() % 256));
            fillRect(x + 100, y, 100, 100, R_G_B(rand() % 256, rand() % 256, rand() % 256));
            fillRect(x + 200, y, 100, 100, R_G_B(rand() % 256, rand() % 256, rand() % 256));
            fillRect(x, y + 100, 100, 100, R_G_B(rand() % 256, rand() % 256, rand() % 256));
            fillRect(x + 100, y + 100, 100, 100, R_G_B(rand() % 256, rand() % 256, rand() % 256));
            fillRect(x + 200, y + 100, 100, 100, R_G_B(rand() % 256, rand() % 256, rand() % 256));
            fillRect(x, y + 200, 100, 100, R_G_B(rand() % 256, rand() % 256, rand() % 256));
            fillRect(x + 100, y + 200, 100, 100, R_G_B(rand() % 256, rand() % 256, rand() % 256));
            fillRect(x + 200, y + 200, 100, 100, R_G_B(rand() % 256, rand() % 256, rand() % 256));
            Sleep(2000);


        }
        fillScreen(R_G_B(255, 255, 255));
    }


};
