from typing import List
import pytest
import os
import subprocess


def find_paths_with_extention(extention: str, base_dir: str, black_list: set):
    # get all files with extention
    stdout = subprocess.run(['find', base_dir, '-name', f'*.{extention}'], capture_output=True).stdout
    file_paths = list(stdout.decode("utf-8").split())

    # ignore files via black list folder
    result = []
    for path in file_paths:
        if len(set(path.split('/')).intersection(black_list)) > 0:
            continue
        result.append(path)
    return result


# TODO: add flake8
def test_clang_format():
    # TODO: add black list files to config
    black_list_folder = {
        "_build",
        "build",
    }
    style_check_dir = os.path.abspath('./../microservices')
    assert os.path.isdir(style_check_dir)

    cpp_paths = find_paths_with_extention('cpp', style_check_dir, black_list_folder)
    h_paths = find_paths_with_extention('h', style_check_dir, black_list_folder)

    style_file_name = '.clang-format'
    project_dir = os.path.abspath('./..')
    style_file_path = f'{project_dir}/{style_file_name}'
    assert os.path.isfile(style_file_path)

    files_without_style = []
    for file_path in cpp_paths + h_paths:
        stderr: str = subprocess.run(['clang-format', '--dry-run', '-i', f'-style=file:{style_file_path}', file_path],
            capture_output=True).stderr.decode('utf-8')
        if len(stderr) > 0:
            files_without_style.append(file_path)
    
    assert '' == '\n'.join(files_without_style) # print files without style
