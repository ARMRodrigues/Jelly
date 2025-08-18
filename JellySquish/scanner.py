from pathlib import Path
import json
import hashlib
import time

def hash_file(path: Path):
    h = hashlib.sha1()
    with open(path, "rb") as f:
        while chunk := f.read(8192):
            h.update(chunk)
    return h.hexdigest()

def scan_assets(assets_dir: Path):
    arquivos = []
    for path in assets_dir.rglob("*"):
        if path.is_file() and not path.suffix.endswith(".meta"):
            rel_path = path.relative_to(assets_dir)
            arquivos.append((path, rel_path))
    return arquivos

def needs_processing(src_path: Path, meta_path: Path):
    if not meta_path.exists():
        return True
    with open(meta_path, "r") as f:
        meta = json.load(f)
    last_modified = int(src_path.stat().st_mtime)
    file_hash = hash_file(src_path)
    return (
        meta.get("last_modified") != last_modified
        or meta.get("hash") != file_hash
    )

def get_files_to_process(assets_dir: Path):
    arquivos_para_processar = []
    for src_path, rel_path in scan_assets(assets_dir):
        meta_path = (assets_dir / rel_path).with_suffix(rel_path.suffix + ".meta")
        if needs_processing(src_path, meta_path):
            arquivos_para_processar.append((src_path, rel_path))
    return arquivos_para_processar
