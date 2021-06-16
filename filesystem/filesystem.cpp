#include "filesystem.hpp"

Filesystem::Filesystem(CLI *cli) :
    cli(cli){
    cli->Register_command("ls", "ls as in Linux", this, &Filesystem::Command_ls);
    cli->Register_command("cd", "cd as in Linux", this, &Filesystem::Command_cd);
    cli->Register_command("pwd", "pwd as in Linux", this, &Filesystem::Command_pwd);
    cli->Register_command("cat", "cat as in Linux", this, &Filesystem::Command_cat);
    cli->Register_command("fg", "set application to foreground", this, &Filesystem::Command_fg);
    cli->Set_line_prefix(actual_position->Path());
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
        return ENOENT;
    }

    if (list_directory->Type_of() != FS_entry::Type::Directory) {
        cli->Print("Target location is not a directory \r\n");
        return ENOTDIR;
    }

    string output = "";
    for (auto entry:list_directory->Contains()) {
        output += entry->Name() + "\r\n";
    }
    cli->Print(output);
    return 0;
}

int Filesystem::Set_location(string path){
    Directory *target_directory = static_cast<Directory *>(Get_entry(path));
    if (target_directory == nullptr) {
        return ENOENT;
    }

    if (target_directory->Type_of() != FS_entry::Type::Directory) {
        return ENOTDIR;
    }

    actual_position = target_directory;
    cli->Set_line_prefix(actual_position->Path());
    return 0;
}

int Filesystem::Command_cd(vector<string> args){
    string directory_name = Command_check(args);
    if (directory_name == "") {
        return -1;
    }

    int return_code = Set_location(directory_name);

    if (return_code == ENOENT){
        cli->Print("Target location is unreachable - null returned \r\n");
    } else if (return_code == ENOTDIR){
        cli->Print("Target location is not a directory \r\n");
    }

    return return_code;
}

int Filesystem::Command_cat(vector<string> args){
    string filename = Command_check(args);
    if (filename == "") {
        return EINVAL;
    }

    File<> *target_file = static_cast<File<> *>(Get_entry(filename));
    if (target_file == nullptr) {
        cli->Print("Target location is unreachable - null returned \r\n");
        return ENOENT;
    }

    if (target_file->Type_of() != FS_entry::Type::File) {
        cli->Print("Target location is not a file \r\n");
        return EISDIR;
    }

    string content = target_file->Read();
    if (content.substr(content.length()-2,content.length()) != "\r\n") {
        content += "\r\n";
    }
    cli->Print(content);
    return 0;
}

int Filesystem::Command_pwd(vector<string> args){
    if (args.size() != 1) {
        cli->Print("Command \"pwd\" does not accept any arguments \r\n");
        return E2BIG;
    }
    cli->Print(actual_position->Path() + "\r\n");
    return 0;
}

int Filesystem::Command_fg(vector<string> args){
    if(args.size() != 2){
        cli->Print("Invalid arguments\r\n");
        return -1;
    }

    string filename = args[1];

    Executable *target_file = static_cast<Executable *>(Get_entry(filename));
    if (target_file == nullptr) {
        cli->Print("Target location is unreachable\r\n");
        return ENOENT;
    }

    if (target_file->Type_of() != FS_entry::Type::Executable) {
        cli->Print("Target location is not an executable \r\n");
        return EISDIR;
    }

    cli->Start_foreground_application(filename);
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
    // Break string to part divided by /
    while (position != string::npos) {
        path.emplace_back(filename.substr(0, position));
        filename.erase(0, position + 1);
        if (filename.length() > 0) {
            position = filename.find("/");
        } else {
            break;
        }
    }
    // Add filename to end
    if (filename.length() > 0) {
        path.emplace_back(filename);
    }

    // Post procesing of path, remove ., when used .. go back one layer
    for (uint i = 0; i < path.size(); i++) {
        if (path[i] == ".") { // case: /test/. -> /test
            path.erase(path.begin() + i);
            i--;
        } else if (path[i] == "..") {
            if (i > 1) { // case: /test/.. -> /
                path.erase(path.begin() + i - 1);
                path.erase(path.begin() + i);
                i -= 2;
            } else { // case: /.. -> /
                path.erase(path.begin() + i);
                i--;
            }
        }
    }

    return path;
} // Filesystem::Create_entry_path

bool Filesystem::Entry_exists(string filename) const {
    return Entry_exists(Create_entry_path(filename));
}

FS_entry::Type Filesystem::Entry_type(string filename) const{
    if (Entry_exists(filename)){
        return Get_entry(filename)->Type_of();
    } else {
        return FS_entry::Type::Undefined;
    }
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

int Filesystem::Make_directory(string path){
    string directory_name = path.substr(path.find_last_of("/") + 1);
    Directory *new_directory = new Directory(directory_name);
    return Add_entry(path, new_directory);
}

int Filesystem::Make_file(string path, string content){
    string filename = path.substr(path.find_last_of("/") + 1);
    File<> *new_file = new File(filename, content);
    return Add_entry(path, new_file);
}

int Filesystem::Add_entry(string path, FS_entry *entry){
    path = Absolute_path(path);
    if (Entry_exists(path)) {
        cli->Print("Entry with that already exists in filesystem: " + path + "\r\n");
        return EEXIST;
    }

    if (path == "") {
        path = "/";
    }

    string parent_directory_name = path.substr(0, path.find_last_of("/"));

    Directory *parent_directory = static_cast<Directory *>(Get_entry(parent_directory_name));
    if (parent_directory == nullptr) {
        cli->Print("Cannot find parent \r\n");
        return ENOENT;
    } else if(parent_directory->Type_of() != FS_entry::Type::Directory){
        cli->Print("Parent is not a directory \r\n");
        return ENOTDIR;;
    }

    parent_directory->Add_entry(entry);

    return 0;

}

int Filesystem::Execute(string &path, vector<string> &args){
    FS_entry* exec_file = Get_entry(path);

    if (exec_file == nullptr){
        cli->Print("Command or executable with name: \"" + args[0] + "\" was not found\r\n");
    }

    if (exec_file->Type_of() == FS_entry::Type::Executable){
        return exec_file->Run(args);
    }
    return -1;
}

