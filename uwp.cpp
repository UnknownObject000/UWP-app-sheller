#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

bool FileExist(string file)
{
	fstream fs(file,ios::in);
	int i=GetLastError();
	if(!fs.is_open() && (i==2||i==3))
		return false;
	fs.close();
	return true;
}

int main()
{
	string path;
	cout<<"Input UWP Folder:";
	Input:
	getline(cin,path);
	if(path=="")
		goto Input;
	int pos1 = path.find("_");
	int pos2 = path.find("__");
	int pos3, pos4;
	string tmp;
	string fn = "C:\\Program Files\\WindowsApps\\";
	fn += path;
	fn += "\\AppxManifest.xml";
	fstream fin(fn, ios::in | ios::binary);
	if ((!fin.is_open()) || (pos1 == -1) || (pos2 == -1))
	{
		cout<<"Invalid UWP app folder!"<<endl;
		if(fin.is_open())
			fin.close(); 
		return -1;
	}
	while (!fin.eof())
	{
		getline(fin, tmp); 
		pos3 = tmp.find("<Logo>");
		pos4 = tmp.find("<Application Id=");
		if (pos3 != -1)
		{
			string logostr = tmp.substr(pos3 + 6, tmp.find("</Logo>") - 10);
			tmp= "C:\\Program Files\\WindowsApps\\";
			tmp += path;
			tmp += string("\\" + logostr);
			if(!FileExist(tmp))
			{
				string nonext=tmp.substr(0,tmp.length()-4);
				tmp=nonext+".scale-100.png";
				if(!FileExist(tmp))
					tmp=nonext+".scale-200.png";
			}
			cout<<"Icon: "<<tmp<<endl;
		}
		if (pos4 != -1)
		{
			tmp = tmp.substr(pos4 + 17);
			tmp=tmp.substr(0,tmp.find("\""));
			string folder = path;
			string folder_front = folder.substr(0, pos1 + 1);
			string folder_back = folder.substr(pos2 + 2);
			folder = folder_front + folder_back;
			tmp = " explorer shell:appsFolder\\" + folder + "!" + tmp;
			cout<<"Shell-command: "<<tmp<<endl;
			break;
		}
	}
	fin.close();
	cout<<"Shell this app? [Yes/No]";
	getline(cin,path);
	if(path=="Y"||path=="y")
		system(tmp.c_str());
	system("pause");
	return 0;
}
