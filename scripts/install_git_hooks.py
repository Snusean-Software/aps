import shutil, os

SCRIPT_PATH=os.path.abspath(__file__)
SCRIPT_DIR, SCRIPT = os.path.split(SCRIPT_PATH)
print(SCRIPT_DIR)
try:
    shutil.copy(f"{SCRIPT_DIR}\githooks\pre-commit", f"{SCRIPT_DIR}\..\.git\hooks\pre-commit")
except:
    shutil.copy(f"{SCRIPT_DIR}/githooks/pre-commit", f"{SCRIPT_DIR}/../.git/hooks/pre-commit")
