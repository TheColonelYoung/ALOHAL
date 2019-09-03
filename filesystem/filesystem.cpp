#include "filesystem.hpp"

Filesystem::Filesystem(CLI* cli):
    cli(cli){

}

const int Filesystem::Command_ls(vector<string> args){
    if (args.size() != 0){
        return -1; //Invalid parameters
    }
    string name = args[0];
    vector<string> path;

    if (name[0] != '/'){ // relative path, add absolute prefix
        name = actual_position->Path() + name;
    }
    path.emplace_back("/");

    return 0;
}

const vector<string> Filesystem::Create_entry_path(string filename){
    vector<string> path;
    if (filename[0] != '/'){ // Relative path, add absolute prefix
        filename = actual_position->Path() + filename;
    }
    if (filename[filename.length()-1] == '/'){ // Remove / at end of string
        filename.erase(filename.length()-1);
    }
    cli->Print(filename + "\r\n");
    path.emplace_back("/");
    filename.erase(0,1);
    cli->Print(filename + "\r\n");
    unsigned int position = filename.find("/");
    while (position != string::npos){
        path.emplace_back(filename.substr(0, position-1));
        filename.erase(0, position);
        position = filename.find("/");
    }
    path.emplace_back(filename);
    return path;
}

const FS_entry Filesystem::Get_entry(vector<string> path){


}
