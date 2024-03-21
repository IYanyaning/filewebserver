#include <json/json.h>
#include <mysql/mysql.h>
#include <string>
using std::string;

/*
    CREATE TABLE file_table (
        id INT AUTO_INCREMENT PRIMARY KEY,
        file_name VARCHAR(255),
        user_name VARCHAR(255),
        parent_folder_id INT,
        file_size BIGINT,
        upload_time DATETIME,
        is_directory TINYINT,
        md5 VARCHAR(32),
        relative_file_path VARCHAR(255)
    );
    CREATE TABLE folder_table (
        id INT AUTO_INCREMENT PRIMARY KEY,
        folder_name VARCHAR(255),
        user_name VARCHAR(255),
        parent_folder_id INT
    );

*/
/*
    SELECT 
        f.id, f.file_name, f.user_name, f.parent_folder_id, f.file_size, f.upload_time, f.is_directory, f.md5, f.relative_file_path, NULL as folder_name, 'file' as type
    FROM 
        file_table f
    WHERE 
        f.user_name = 'chen'
    UNION ALL
    SELECT 
        fo.id, NULL as file_name, fo.user_name, fo.parent_folder_id, NULL as file_size, NULL as upload_time, 1 as is_directory, NULL as md5, NULL as relative_file_path, fo.folder_name, 'folder' as type
    FROM 
        folder_table fo
    WHERE 
        fo.user_name = 'chen'
    ORDER BY 
        parent_folder_id;
*/

/*
    根据上面提供的 SQL 查询语句，输出表的每行结构如下所示：

    id：文件或文件夹的唯一标识符
    file_name：文件名（对于文件）或文件夹名（对于文件夹）
    user_name：用户名称
    parent_folder_id：父文件夹的 ID
    file_size：文件大小（仅对文件有效）
    upload_time：上传时间（仅对文件有效）
    is_directory：是否为文件夹（1 表示是文件夹，0 表示是文件）
    md5：文件的 MD5 值（仅对文件有效）
    relative_file_path：文件的相对路径（仅对文件有效）
    folder_name：文件夹名（仅对文件夹有效）
    type：类型

*/
struct FileNode
{
    string fileName;
    bool isDirectory;
    string filePath;
    string MD5;
    string UUID;
    int size;
    int fileID;
    int parentFolderID;
    string time;
    string userName;
    string fileType;
    string folderName;
    std::vector<FileNode *> children;
};

void buildFileTree(FileNode* node, MYSQL_RES* res);

//将文件树转换为JSON格式
Json::Value converToJson(FileNode* node);