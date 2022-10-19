import os
import subprocess

def get_ustyle_file_paths(files_paths, style_file_path):
    files_without_style = []
    for file_path in files_paths:
        stderr: str = subprocess.run(['clang-format', '--dry-run', '-i', f'-style=file:{style_file_path}', file_path],
            capture_output=True).stderr.decode('utf-8')
        if len(stderr) > 0:
            files_without_style.append(file_path)
    return files_without_style


# TODO: add flake8
def test_clang_format():
    # TODO: add black list files to config
    style_check_dir = os.path.abspath('./../microservices')
    assert os.path.isdir(style_check_dir)

    find_paths_command = f"find {style_check_dir} -regex '.*\.\(cpp\|h\)' | grep -v _build/ | grep -v build/"
    files_paths = subprocess.run(list(find_paths_command.split()), capture_output=True).stdout

    style_file_name = '.clang-format'
    project_dir = os.path.abspath('./..')
    style_file_path = f'{project_dir}/{style_file_name}'
    assert os.path.isfile(style_file_path)

    unstyle_paths = get_ustyle_file_paths(files_paths, style_file_path)
    assert '' == '\n'.join(unstyle_paths) # print files without style
