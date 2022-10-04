import os
import subprocess

class DockerCompose:

    def __init__(self, docker_compose_file_path: str, docker_compose_env_file_path: str):
        assert(os.path.isfile(docker_compose_file_path))
        assert(os.path.isfile(docker_compose_env_file_path))
        self.docker_compose_file_path = docker_compose_file_path
        self.docker_compose_env_file_path = docker_compose_env_file_path

    def run(self):
        self.process = subprocess.Popen(f"docker-compose --env-file {self.docker_compose_env_file_path} -f {self.docker_compose_file_path} up", shell=True)

    def stop(self):
        self.process.kill()
