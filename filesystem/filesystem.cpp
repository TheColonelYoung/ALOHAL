#include "filesystem.hpp"

Filesystem::Filesystem(CLI* cli):
    cli(cli){
    cli->Register_command("ls", "ls as in Linux", this, &Filesystem::Command_ls);
    cli->Set_filesystem_prefix(actual_position->Path());
}

int Filesystem::Command_ls(vector<string> args){
    if (args.size() > 2){
        cli->Print("Invalid parameters \r\n");
        return -1;
    }
    string directory_name;
    if(args.size() == 1){
        directory_name = Absolute_path(".");
    } else {
        directory_name = Absolute_path(args[1]);
    }
    if (!Entry_exists(directory_name)){
        cli->Print("Target location does not exists \r\n");
        return -1;
    }
    Directory* list_directory = static_cast<Directory*>(Get_entry(directory_name));
    if(list_directory == nullptr){
        cli->Print("Target location is unreachable - null returned \r\n");
    }
    if(list_directory->Type_of() != FS_entry::Type::Directory){
        cli->Print("Target location is not a directory \r\n");
        return -1;
    }

    string output = "";
    for(auto entry:list_directory->Contains()){
        output += entry->Name() + "\r\n";
    }
    cli->Print(output);
    return 0;
}

string Filesystem::Absolute_path(string filename) const{
    if (filename == "."){
        return actual_position->Path();
    }

    if (filename == "/"){
        return "/";
    }

    if (filename[0] != '/'){ // Relative path, add absolute prefix
        filename = actual_position->Path() + filename;
    }
    if (filename[filename.length()-1] == '/'){ // Remove / at end of string
        filename.erase(filename.length()-1);
    }
    return filename;
}

vector<string> Filesystem::Create_entry_path(string filename) const{
    vector<string> path;
    filename = Absolute_path(filename);
    path.emplace_back("/");
    filename.erase(0,1);
    unsigned int position = filename.find("/");
    while (position != string::npos){
        path.emplace_back(filename.substr(0, position));
        filename.erase(0, position+1);
        if (filename.length() > 0){
            position = filename.find("/");
        } else {
            break;
        }
    }
    if (filename.length() > 0){
        path.emplace_back(filename);
    }
    return path;
}

bool Filesystem::Entry_exists(string filename) const{
    return Entry_exists(Create_entry_path(filename));
}

bool Filesystem::Entry_exists(vector<string> path) const{

    if (path.front() == "/" and root != nullptr and path.size() == 1){
        // Root handling
        return true;
    }
    path.erase(path.begin());
    Directory* actual = root;
    for(auto entry:path){
        if(actual->Exists(entry)){
            //cli->Print("Actual: " + actual->Name() + " Path.back(): " + path.back() +" \r\n");
            if(/*actual->Name() == path.back() && */entry == path.back()){ // File found
                return true;
            } else if(actual->Get_entry(entry)->Type_of() == FS_entry::Type::Directory){
                // Continue deepper
                actual = static_cast<Directory *>(actual->Get_entry(entry));
            }
        }
    }
    return false;
}

FS_entry* Filesystem::Get_entry(string filename) const{
    return Get_entry(Create_entry_path(filename));
}

FS_entry* Filesystem::Get_entry(vector<string> path) const{
    if (path.front() == "/" and root != nullptr and path.size() == 1){
        // Root handling
        return root;
    }
    path.erase(path.begin());
    Directory* actual = root;
    for(auto entry:path){
        if(actual->Exists(entry)){
            if(/*actual->Name() == path.back() && */entry == path.back()){
                // Entry found
                return actual->Get_entry(entry);
            } else if(actual->Get_entry(entry)->Type_of() == FS_entry::Type::Directory){
                // Continue deepper
                actual = static_cast<Directory *>(actual->Get_entry(entry));
            }
        }
    }
    return nullptr;
}

int Filesystem::Make_directory(string name){
    name = Absolute_path(name);
    if (Entry_exists(name)){
        cli->Print("Directory already exists\r\n");
        return -1;
    }
    unsigned int position = name.find_last_of("/");
    string parent_directory_name = name.substr(0, position);
    if (parent_directory_name == ""){
        parent_directory_name = "/";
    }

    string directory_name = name.substr(position+1);

    Directory* parent_directory = static_cast<Directory*>(Get_entry(parent_directory_name));
    if(parent_directory == nullptr){
        cli->Print("Cannot find parent \r\n");
        return -1;
    }
    parent_directory->Add_entry(new Directory(directory_name));

    return 0;
}
