#include<iostream>
#include<windows.h>
#include<string>
#include<list>
#include<tuple>

using namespace std;

typedef tuple<string, int> name_counter;

const string image_types[3] = {string(".jpg"), string(".jpeg"), string(".png")};

int is_image(string name){
    for (int i = 0; i < 3; i++){
        if (name.length() >= image_types[i].length()){
            int end = name.length() - image_types[i].length();
            if (name.compare(end, image_types[i].length(), image_types[i]) == 0)
                return 1;
        }
    }
    return 0;
}

string date_to_string(SYSTEMTIME date){
    string result = {""};
    result += to_string(date.wYear)+"-";

    if (date.wMonth < 10) result += "0";
    result += to_string(date.wMonth)+"-";

    if (date.wDay < 10) result += "0";
    result += to_string(date.wDay)+"_";

    if (date.wHour < 10) result += "0";
    result += to_string(date.wHour)+"-";

    if (date.wMinute < 10) result += "0";
    result += to_string(date.wMinute)+"-";

    if (date.wSecond < 10) result += "0";
    result += to_string(date.wSecond);

    return result;

}

string get_file_type(const char *fname){
    string result = "";
    int already_type_name = 0;
    cout << fname << endl;
    for (char *i = (char*)(fname); *i != '\0'; i++){
        if (*i == '.'){
            already_type_name = 1;
            result = string("");
        }
        if (already_type_name) result += *i;
    }
    return result;
}

int bypass_bir(std::string dir_path, string dirname = string("")){
    WIN32_FIND_DATAA findData;
    HANDLE hf;
 
    hf = FindFirstFileA((dir_path+"*").c_str(), &findData);
    if (hf == INVALID_HANDLE_VALUE)
    {
        cout << "Cannot find file" << endl;
        return -1;
    }
 
    list<name_counter> names;
    do
    {
        if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && findData.cFileName[0]!='.')
            bypass_bir(dir_path + findData.cFileName + string("\\"), string(findData.cFileName));
        else if (is_image(string(findData.cFileName))){
            string time("");
            SYSTEMTIME sys_time;
            FileTimeToSystemTime(&findData.ftLastWriteTime, &sys_time);
            string file_name = (dir_path + date_to_string(sys_time) + "_" + dirname );
            int is_already_was = 0;
            int number = 0;
            for (auto i { names.begin()}; i != names.end(); ++i){
                if (file_name == get<0>(*i)){
                    is_already_was = 1;
                    number = get<1>(*i);
                    *i = name_counter(file_name, number+1);
                    break;
                }
            }
            if (!is_already_was) names.push_front(name_counter(file_name, 0));
            else file_name += "__"+to_string(number)+"__";
            file_name += get_file_type((string(findData.cFileName)).c_str());
            rename((dir_path+string(findData.cFileName)).c_str(), file_name.c_str());
            cout << sys_time.wDay << " " << sys_time.wMonth << " " << sys_time.wYear << " ";
        }
            
        cout << dir_path << findData.cFileName << endl;
    }
    while (FindNextFileA(hf, &findData)); 
 
    FindClose(hf);
}

int main(){
    setlocale(LC_ALL, "Russian");

    bypass_bir(".\\");

}