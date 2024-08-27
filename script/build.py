# coding=utf-8
import shutil
import os
import sys
import argparse

SCRIPT_PATH = os.path.split(os.path.realpath(__file__))[0]
BUILD_DIR_PATH = SCRIPT_PATH + '/../build'


def clear():
    if os.path.exists(BUILD_DIR_PATH):
        shutil.rmtree(BUILD_DIR_PATH)


def build_windows(config='Release'):
    platform_dir = '%s/%s' % (BUILD_DIR_PATH, config)
    os.makedirs(platform_dir, exist_ok=True)

    os.chdir(platform_dir)

    build_cmd = 'cmake ../.. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=%s' % (
        config)

    print("build cmd:" + build_cmd)
    ret = os.system(build_cmd)
    if ret != 0:
        print('!!!!!!!!!!!!!!!!!!build fail')
        return False

    build_cmd = 'cmake --build . --config %s --parallel 8' % (config)
    ret = os.system(build_cmd)
    if ret != 0:
        print('build fail!!!!!!!!!!!!!!!!!!!!')
        return False
    return True


def main():
    clear()
    os.makedirs(BUILD_DIR_PATH, exist_ok=True)

    if not build_windows(config='Debug'):
        exit(1)


if __name__ == '__main__':
    main()
