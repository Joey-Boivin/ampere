from pathlib import Path
import subprocess

EXTENSIONS_TO_FORMAT = [".c", ".h"]
PROJECT_FOLDER_ROOT = "src"

files = [p for p in Path(PROJECT_FOLDER_ROOT).rglob('*') if p.suffix in EXTENSIONS_TO_FORMAT]

def format_file(file_name):
    cmd = f"clang-format {file_name} --style=file -i"
    print(cmd)
    subprocess.run(cmd, shell=True)

for file in files:
    format_file(file)