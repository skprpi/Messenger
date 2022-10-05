import os
import subprocess
import time

class DockerCompose:

    def __init__(self, docker_compose_setup_script_path: str):
        assert(os.path.isfile(docker_compose_setup_script_path))
        self.docker_compose_setup_script_path = docker_compose_setup_script_path

    def run(self):
        self.process = subprocess.Popen(self.docker_compose_setup_script_path, shell=True)
        time.sleep(1) # waiting for setup TODO waiting for output message

    def stop(self):
        self.process.kill()
