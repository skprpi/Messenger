from ipaddress import ip_address
import os
import subprocess
import time
import requests

from utils import DockerCompose

def get_ustyle_file_paths(files_paths, style_file_path):
    files_without_style = []
    for file_path in files_paths:
        stderr: str = subprocess.run(['clang-format', '--dry-run', '-i', f'-style=file:{style_file_path}', file_path],
            capture_output=True).stderr.decode('utf-8')
        if len(stderr) > 0:
            files_without_style.append(file_path)
    return files_without_style


# TODO: add flake8
def test_get_handler():
    docker_compose_path = os.getenv("docker_compose_m2m_chat_service")
    assert(docker_compose_path)

    docker_env_file_path = os.getenv("m2m_chat_service_docker_compose_env")
    assert(docker_env_file_path)

    docker_compose = DockerCompose(docker_compose_path, docker_env_file_path)
    docker_compose.run()

    time.sleep(30)

    result = requests.get('http://0.0.0.0:9000', timeout=10) # TODO: use config value
    print(result)
    assert(result.content.decode("utf-8") == "name=foo")
    docker_compose.stop()
