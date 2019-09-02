#include "filesystem.hpp"

Filesystem::Filesystem(shared_ptr<CLI> cli):
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
    if (filename[0] != '/'){ // relative path, add absolute prefix
        filename = actual_position->Path() + filename;
    }
    path.emplace_back("/");
    //find(filename.begin(), filename.end(), "/");

    return path;

}

const FS_entry Filesystem::Get_entry(vector<string> path){


}
