import json
import hashlib

def hash_file(path):
    import hashlib
    h = hashlib.sha1()
    with open(path, "rb") as f:
        while chunk := f.read(8192):
            h.update(chunk)
    return h.hexdigest()

class MetaFile:
    def __init__(self, path):
        self.path = path

    def write(self, src_path):
        data = {
            "last_modified": int(src_path.stat().st_mtime),
            "hash": hash_file(src_path)
        }
        with open(self.path, "w") as f:
            json.dump(data, f, indent=4)
