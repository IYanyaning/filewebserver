#ifndef MYCOMPRESS_H
#define MYCOMPRESS_H

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <minizip/zip.h>
#include <minizip/unzip.h>
#include<string>
#include<iostream>
namespace MyCompress{
    #define MAX_FILENAME 256
    #define WRITEBUFFERSIZE 8192

    bool compress_folder(const char* folder_path, const char* zip_path);
    int zip_folder(zipFile zip, const char* folder_path, const char* base_path);
}
bool compress_folder(const char* folder_path, const char* zip_path) {
    // 创建 zip 文件
    zipFile zip = zipOpen64(zip_path, 0);
    if (zip == NULL) {
        //printf("Failed to create zip file.\n");
        return false;
    }

    // 遍历文件夹及其子文件夹，并添加到 zip 中
    int result = zip_folder(zip, folder_path, folder_path);

    // 关闭 zip 文件
    zipClose(zip, NULL);

    if (result != 0) {
        //printf("Failed to compress folder.\n");
        return false;
    }

    //printf("Folder compressed successfully.\n");
    return true;
}

int zip_folder(zipFile zip, const char* folder_path, const char* base_path) {
    // 打开文件夹
    DIR* folder = opendir(folder_path);
    if (folder == NULL) {
        printf("Failed to open folder: %s\n", folder_path);
        return -1;
    }

    // 读取文件夹内容
    struct dirent* entry;
    while ((entry = readdir(folder)) != NULL) {
        // 忽略 . 和 .. 目录
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // 构建文件/文件夹路径
        char file_path[MAX_FILENAME];
        snprintf(file_path, sizeof(file_path), "%s/%s", folder_path, entry->d_name);

        // 判断文件类型
        if (entry->d_type == DT_REG) {
            // 构建 zip 中的路径，去除基本路径前缀
            const char* rel_path = file_path + strlen(base_path) + 1;

            // 添加文件到 zip
            FILE* file = fopen64(file_path, "rb");
            if (file == NULL) {
                printf("Failed to open file: %s\n", file_path);
                return -1;
            }

            // 创建 zip 中的文件
            zip_fileinfo file_info;
            memset(&file_info, 0, sizeof(file_info));
            zipOpenNewFileInZip(zip, rel_path, &file_info, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION);

            // 读取文件内容并写入 zip
            char buffer[WRITEBUFFERSIZE];
            int size;
            while ((size = fread(buffer, 1, sizeof(buffer), file)) > 0) {
                zipWriteInFileInZip(zip, buffer, size);
            }

            // 关闭文件和 zip 中的文件
            fclose(file);
            zipCloseFileInZip(zip);
        } else if (entry->d_type == DT_DIR) {
            // 递归压缩子文件夹
            int result = zip_folder(zip, file_path, base_path);
            if (result != 0) {
                closedir(folder);
                return result;
            }
        }
    }

    // 关闭文件夹
    closedir(folder);

    return 0;
}
#endif
