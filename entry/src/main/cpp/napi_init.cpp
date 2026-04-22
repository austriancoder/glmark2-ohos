#include <stdlib.h>
#include "napi/native_api.h"
#include "manager/plugin_manager.h"
#include "rawfile/raw_file_manager.h"

#include <iostream>
#include <filesystem>
#include <fstream> 
#include <sys/stat.h>

#define LENGTH_OF_ARRAY(ARRAY) ((int)(sizeof(ARRAY) / sizeof((ARRAY)[0])))

extern "C" {
extern int main(int argc, char *argv[]);
}

static void CreateDirectory(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) {
        mkdir(path.c_str(), 0755);  // Create directory with read/write/execute permissions
    }
}

static void CopyFile(NativeResourceManager* mNativeResMg, const std::string& src, const std::string& dest) {
    RawFile *srcFile = OH_ResourceManager_OpenRawFile(mNativeResMg, src.c_str());
    if (!srcFile) {
        throw std::runtime_error("Unable to open source file: " + src);
    }

    std::ofstream destFile(dest, std::ios::binary);
    if (!destFile.is_open()) {
        throw std::runtime_error("Unable to open destination file: " + dest);
    }

    char buffer[100];

    while (OH_ResourceManager_GetRawFileRemainingLength(srcFile) > 0) {
        int read = OH_ResourceManager_ReadRawFile(srcFile, buffer, sizeof(buffer));

        if (read == 0)
            continue;

        destFile.write(buffer, read);
        if (!destFile.good()) {
            throw std::runtime_error("Error writing to destination file: " + dest);
        }
    }

    OH_ResourceManager_CloseRawFile(srcFile);
}

void CopyDirectoryStructure(NativeResourceManager* mNativeResMgr, const std::string& srcPath, const std::string& destPath) {
    RawDir* rawDir = OH_ResourceManager_OpenRawDir(mNativeResMgr, srcPath.c_str());
    if (!rawDir) {
        // Failed to open directory
        return;
    }

    int count = OH_ResourceManager_GetRawFileCount(rawDir);
    for (int i = 0; i < count; i++) {
        std::string filename = OH_ResourceManager_GetRawFileName(rawDir, i);
        std::string fullSrcPath = srcPath + "/" + filename;
        
        if (srcPath.empty())
            fullSrcPath = filename;
    
        std::string fullDestPath = destPath + "/" + filename;

        if (OH_ResourceManager_IsRawDir(mNativeResMgr, fullSrcPath.c_str())) {
            // Create directory at destination
            CreateDirectory(fullDestPath);
            // Recursively copy subdirectory
            CopyDirectoryStructure(mNativeResMgr, fullSrcPath, fullDestPath);
        } else {
            // Copy file
            CopyFile(mNativeResMgr, fullSrcPath, fullDestPath);
        }
    }
}

static napi_value CopyAssets(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value argv[2] = {nullptr};

    napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);

    napi_valuetype valueType;
    napi_typeof(env, argv[0], &valueType);
    NativeResourceManager *mNativeResMgr = OH_ResourceManager_InitNativeResourceManager(env, argv[0]);
    size_t strSize;
    char strBuf[256];
    napi_get_value_string_utf8(env, argv[1], strBuf, sizeof(strBuf), &strSize);
    std::string filename(strBuf, strSize);
    
    CopyDirectoryStructure(mNativeResMgr,filename.c_str(), "/data/storage/el2/base/files/");

    OH_ResourceManager_ReleaseNativeResourceManager(mNativeResMgr);
    
    return nullptr;
}

static napi_value Run(napi_env env, napi_callback_info info)
{
    const char *argv[] = {
        "glmark2",
    };
    
    const int argc = LENGTH_OF_ARRAY(argv);
    
    setenv("MESA_LOADER_DRIVER_OVERRIDE", "zink", 1);
    setenv("EGL_LOG_LEVEL", "debug", 1);
    setenv("MESA_LOG", "ohos", 1);
    
    main(argc, (char **)argv);
    
    napi_value sum;

    return sum;
}

static napi_value Add(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value args[2] = {nullptr};

    napi_get_cb_info(env, info, &argc, args , nullptr, nullptr);

    napi_valuetype valuetype0;
    napi_typeof(env, args[0], &valuetype0);

    napi_valuetype valuetype1;
    napi_typeof(env, args[1], &valuetype1);

    double value0;
    napi_get_value_double(env, args[0], &value0);

    double value1;
    napi_get_value_double(env, args[1], &value1);

    napi_value sum;
    napi_create_double(env, value0 + value1, &sum);

    return sum;

}

EXTERN_C_START
static napi_value Init(napi_env env, napi_value exports)
{
    napi_property_descriptor desc[] = {
        {"copyAssets", nullptr, CopyAssets, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"createNativeNode", nullptr, NativeXComponentSample::PluginManager::createNativeNode, nullptr, nullptr, nullptr,
         napi_default, nullptr },
        { "run", nullptr, Run, nullptr, nullptr, nullptr, napi_default, nullptr },
        { "add", nullptr, Add, nullptr, nullptr, nullptr, napi_default, nullptr }
    };
    napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);
    return exports;
}
EXTERN_C_END

static napi_module demoModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init,
    .nm_modname = "entry",
    .nm_priv = ((void*)0),
    .reserved = { 0 },
};

extern "C" __attribute__((constructor)) void RegisterEntryModule(void)
{
    napi_module_register(&demoModule);
}
