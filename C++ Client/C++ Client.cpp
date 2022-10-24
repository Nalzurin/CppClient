#pragma comment(lib, "Ws2_32.lib")
#pragma warning( disable : 4996)
#include <iostream>
#include <winsock2.h>

#include <cstring>
#include <WS2tcpip.h>
#include "GraphicsLib.cpp"

#define LINEMAX = 1024;
using namespace std;



int main()
{
	char* data;
	int x1, x2, y1, y2, val2;
	uint_least16_t color;
	int_least16_t widthImage, heightImage;
	const int maxnum = 32767;
	string ipaddr = "127.0.0.1", messagetext;
	int  port = 1984;
	short width = 0, height = 0;
	cout << "Input Ip Address: \n";
	cin >> ipaddr;
	cout << "Input port:\n";
	cin >> port;
	cout << "Input the size of window, max number: " << maxnum << ", min number: -" << maxnum << endl;;

	while (width == 0)
	{
		cout << "Input width:\n";
		cin >> width;
		if (width > maxnum || width < -maxnum)
		{
			cout << "Error, number too big/small!";
			width = 0;
		}


	}

	while (height == 0)
	{
		cout << "Input height: \n";
		cin >> height;
		if (height > maxnum || height < -maxnum)
		{
			cout << "Error, number too big/small!";
			height = 0;
		}


	}

	graphicsLib lib = graphicsLib(width, height);
	lib.setIP(ipaddr);
	lib.setPort(port);
	int client = lib.ClientStartup();
	lib.SetScreenSize();
	int command;
	string fileext, filename;
	while (true)
	{
		cout << "Input Command, for available commands enter 100, to exit enter 0; \n";
		cin >> command;

		switch (command)
		{

		case 100:
			cout << "Available commands: \n1.fillScreen\n2.drawPixel\n3.drawLine\n4.drawRect\n5.fillRect\n6.drawEllipse\n7.fillEllipse\n8.drawText\n9.loadSprite\n10.showSprite\n11.Dancefloor\n12.Set Rotation\n13.Get Width\n14.Get Height\n15.Upload GIF\n16.Play GIF\n";
			break;
		case 1:

			lib.fillScreen(lib.ColorInput());
			break;

		case 2:
			cout << "Input x1:\n";
			cin >> x1;
			cout << "Input y1:\n";
			cin >> y1;
			lib.drawPixel(x1, y1, lib.ColorInput());
			break;

		case 3:
			cout << "Input x1:\n";
			cin >> x1;
			cout << "Input y1:\n";
			cin >> y1;
			cout << "Input x2:\n";
			cin >> x2;
			cout << "Input y2:\n";
			cin >> y2;
			lib.drawLine(x1, y1, x2, y2, lib.ColorInput());
			break;
		case 4:
			cout << "Input x1:\n";
			cin >> x1;
			cout << "Input y1:\n";
			cin >> y1;
			cout << "Input width:\n";
			cin >> x2;
			cout << "Input height:\n";
			cin >> y2;
			lib.drawRect(x1, y1, x2, y2, lib.ColorInput());
			break;
		case 5:
			cout << "Input x1:\n";
			cin >> x1;
			cout << "Input y1:\n";
			cin >> y1;
			cout << "Input width:\n";
			cin >> x2;
			cout << "Input height:\n";
			cin >> y2;
			lib.fillRect(x1, y1, x2, y2, lib.ColorInput());
			break;
		case 6:
			cout << "Input x1:\n";
			cin >> x1;
			cout << "Input y1:\n";
			cin >> y1;
			cout << "Input horizontal radius:\n";
			cin >> x2;
			cout << "Input vertical radius:\n";
			cin >> y2;
			lib.drawEllipse(x1, y1, x2, y2, lib.ColorInput());
			break;
		case 7:
			cout << "Input x1:\n";
			cin >> x1;
			cout << "Input y1:\n";
			cin >> y1;
			cout << "Input horizontal radius:\n";
			cin >> x2;
			cout << "Input vertical radius:\n";
			cin >> y2;
			lib.fillEllipse(x1, y1, x2, y2, lib.ColorInput());
			break;
		case 8:
			cout << "Input x1:\n";
			cin >> x1;
			cout << "Input y1:\n";
			cin >> y1;
			cout << "Font Size:\n";
			cin >> x2;
			cout << "Input color:\n";
			color = lib.ColorInput();
			cout << "Input text:\n";
			fflush(stdin);
			cin.ignore();
			getline(cin, messagetext);
			fflush(stdin);
			lib.drawText(x1, y1, messagetext.c_str(), color, messagetext.length(), x2);
			break;
		case 9:
			cout << "Input Index:\n";
			cin >> x1;
			cout << "Input Image Name with Extension!!!:\n";
			fflush(stdin);
			cin.ignore();
			getline(cin, messagetext);
			fflush(stdin);
			data = lib.ConvertImage(messagetext, widthImage, heightImage);
			lib.loadSprite(x1, widthImage, heightImage, data);
			break;
		case 10:
			cout << "Input Index:\n";
			cin >> x2;
			cout << "Input x:\n";
			cin >> x1;
			cout << "Input y:\n";
			cin >> y1;
			lib.showSprite(x2, x1, y1);
			break;
		case 11:
			cout << "Input Num of iterations:\n";
			cin >> x2;
			cout << "Input x:\n";
			cin >> x1;
			cout << "Input y:\n";
			cin >> y1;
			lib.DanceFloor(x2, x1, y1);
			break;
		case 12:
			cout << "Input rotation(in degrees):\n";
			cin >> x1;
			lib.SetRotation(x1);
			break;
		case 13:
			 cout << "Width = " << lib.getWidth() << "px" << endl;
			 break;
		case 14:
			cout << "Height = " << lib.getHeight() << "px" << endl;
			break;
		case 15:
			cout << "Input first frame's name without the numbers or extensions\n";
			cin >> filename;
			cout << "Input frames extension:\n";
			cin >> fileext;
			cout << "Input starting index\n";
			cin >> x1;
			cout << "Input number of frames\n";
			cin >> x2;
			lib.UploadGIF(x1, x2, filename, fileext);
			break;
		case 16:
			cout << "Input x:";
			cin >> x1;
			cout << "Input y:";
			cin >> y1;
			cout << "Input starting index\n";
			cin >> x2;
			cout << "Input number of frames\n";
			cin >> y2;
			cout << "Input number of iterations\n";
			cin >> val2;
			lib.PlayGIF(val2, x2, y2 , x1, y1);
			break;

			
		case 0:
			return 0;
			break;

		default:
			cout << "Error: No Such Command!\n";
			break;

		}


	}

};


