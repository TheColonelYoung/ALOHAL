#include "filesystem.hpp"

Filesystem::Filesystem(CLI *cli) :
    cli(cli){
    cli->Register_command("ls", "ls as in Linux", this, &Filesystem::Command_ls);
    cli->Register_command("cd", "cd as in Linux", this, &Filesystem::Command_cd);
    cli->Set_filesystem_prefix(actual_position->Path());
    root->Set_parent(root);
}

int Filesystem::Command_ls(vector<string> args){
    string directory_name = Command_check(args);
    if (directory_name == "") {
        return -1;
    }

    Directory *list_directory = static_cast<Directory *>(Get_entry(directory_name));
    if (list_directory == nullptr) {
        cli->Print("Target location is unreachable - null returned \r\n");
    }

    if (list_directory->Type_of() != FS_entry::Type::Directory) {
        cli->Print("Target location is not a directory \r\n");
        return -1;
    }

    string output = "";
    for (auto entry:list_directory->Contains()) {
        output += entry->Name() + "\r\n";
    }
    cli->Print(output);
    return 0;
}

int Filesystem::Command_cd(vector<string> args){
    string directory_name = Command_check(args);
    if (directory_name == "") {
        return -1;
    }

    cli->Print(directory_name + "\r\n");

    Directory *target_directory = static_cast<Directory *>(Get_entry(directory_name));
    if (target_directory == nullptr) {
        cli->Print("Target location is unreachable - null returned \r\n");
    }

    if (target_directory->Type_of() != FS_entry::Type::Directory) {
        cli->Print("Target location is not a directory \r\n");
        return -1;
    }

    actual_position = target_directory;
    cli->Set_filesystem_prefix(actual_position->Path());

    return 0;
}

string Filesystem::Command_check(vector<string> args) const {
    if (args.size() > 2) {
        cli->Print("Invalid parameters \r\n");
        return "";
    }
    string directory_name;
    if (args.size() == 1) {
        directory_name = Absolute_path(".");
    } else {
        directory_name = Absolute_path(args[1]);
    }
    if (!Entry_exists(directory_name)) {
        cli->Print("Target location does not exists \r\n");
        return "";
    }
    return directory_name;
}

string Filesystem::Absolute_path(string filename) const {
    if (filename == ".") {
        return actual_position->Path();
    }

    if (filename == "/") {
        return "/";
    }

    if (filename[0] != '/') {                     // Relative path, add absolute prefix
        filename = actual_position->Path() + filename;
    }
    if (filename[filename.length() - 1] == '/') { // Remove / at end of string
        filename.erase(filename.length() - 1);
    }
    return filename;
}

vector<string> Filesystem::Create_entry_path(string filename) const {
    vector<string> path;
    filename = Absolute_path(filename);
    path.emplace_back("/");
    filename.erase(0, 1);
    unsigned int position = filename.find("/");
    while (position != string::npos) {
        path.emplace_back(filename.substr(0, position));
        filename.erase(0, position + 1);
        if (filename.length() > 0) {
            position = filename.find("/");
        } else {
            break;
        }
    }
    if (filename.length() > 0) {
        path.emplace_back(filename);
    }
    return path;
}

bool Filesystem::Entry_exists(string filename) const {
    return Entry_exists(Create_entry_path(filename));
}

bool Filesystem::Entry_exists(vector<string> path) const {
    FS_entry *target = Get_entry(path);
    if (target != nullptr) {
        return true;
    }
    return false;
}

FS_entry * Filesystem::Get_entry(string filename) const {
    return Get_entry(Create_entry_path(filename));
}

FS_entry * Filesystem::Get_entry(vector<string> path) const {
    if (path.front() == "/" and root != nullptr and path.size() == 1) {
        // Root handling
        return root;
    }
    path.erase(path.begin()); // Remove /
    Directory *actual = root;
    for (uint i = 0; i < path.size(); i++) {
        if (actual->Exists(path[i])) {
            if ((i == (path.size() - 1)) && (path[i] == path.back())) {
                // Entry found
                return actual->Get_entry(path[i]);
            } else if (actual->Get_entry(path[i])->Type_of() == FS_entry::Type::Directory) {
                // Continue deepper
                actual = static_cast<Directory *>(actual->Get_entry(path[i]));
            } else {
                // Cannot go further
                return nullptr;
            }
        }
    }
    return nullptr;
}

int Filesystem::Make_directory(string name){
    name = Absolute_path(name);
    if (Entry_exists(name)) {
        cli->Print("Directory already exists\r\n");
        return -1;
    }
    unsigned int position        = name.find_last_of("/");
    string parent_directory_name = name.substr(0, position);
    if (parent_directory_name == "") {
        parent_directory_name = "/";
    }

    string directory_name = name.substr(position + 1);

    Directory *parent_directory = static_cast<Directory *>(Get_entry(parent_directory_name));
    if (parent_directory == nullptr) {
        cli->Print("Cannot find parent \r\n");
        return -1;
    }

    Directory *new_directory = new Directory(directory_name);

    parent_directory->Add_entry(new_directory);

    return 0;
}
