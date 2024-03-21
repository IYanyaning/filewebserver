#include "file_tree.h"
void buildFileTree(FileNode* node, MYSQL_RES* res){
    MYSQL_ROW row;
    while( (row = mysql_fetch_row(res)) ){
        int fileID = atoi(row[0]);
        string fileName = row[1];
        string userName = row[2];
        int parentFolderID = atoi(row[3]);
        int size = atoi(row[4]);
        string time = row[5];
        bool isDirectory  = atoi(row[6]);
        string MD5 = row[7];
        string filePath = row[8];
        //string folderName = row[9];
        string fileType = row[10];

        if(isDirectory){

            //创建文件夹节点
            FileNode* folderNode = new FileNode();
            folderNode->fileID = fileID;
            folderNode->fileName = fileName;
            folderNode->userName = userName;
            folderNode->parentFolderID = parentFolderID;
            folderNode->size = size;
            folderNode->time = time;
            folderNode->isDirectory = true;
            folderNode->MD5 = MD5;
            folderNode->filePath = filePath;
            folderNode->fileType = fileType;

            //递归构建子文件树
            buildFileTree(folderNode, res);

            //将文件夹节点添加到父节点的子节点列表中
            node->children.push_back(folderNode);
        }else{

            FileNode* fileNode = new FileNode();
            fileNode->fileID = fileID;
            fileNode->fileName = fileName;
            fileNode->userName = userName;
            fileNode->parentFolderID = parentFolderID;
            fileNode->size = size;
            fileNode->time = time;
            fileNode->isDirectory = false;
            fileNode->MD5 = MD5;
            fileNode->filePath = filePath;
            fileNode->fileType = fileType;

            //将文件节点添加到父节点的子节点列表中
            node->children.push_back(fileNode);
        }
    }
    
}

//将文件树转换为JSON格式
Json::Value converToJson(FileNode* node){
    Json::Value jsonValue;
    jsonValue["fileName"] = node->fileName;
    jsonValue["isDirectory"] = node->isDirectory;

    for( FileNode* child : node->children){
        jsonValue["children"].append(converToJson(child));
    }

    return jsonValue;
}