//
// Created by blueberry on 2025/10/5.
//

#pragma once
#include <string>


class FileUtil {
public:
    static FileUtil &shared() {
        static FileUtil _instance;
        return _instance;
    }

    void setResourceRootPath(std::string path) {
       m_ResourceRootPath = path;
    }

    std::string GetPath(std::string path) {
        return m_ResourceRootPath + "/" + path;
    }
private:
    std::string m_ResourceRootPath = ".";

    FileUtil() {
    }

    virtual ~FileUtil() {
    }
};
