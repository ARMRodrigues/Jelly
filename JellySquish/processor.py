from pathlib import Path
from meta import MetaFile
from converter import convert_to_raw_data

class AssetProcessor:
    def __init__(self, assets_dir: Path, output_dir: Path, texture_format="BC7_UNORM", mipmaps=True):
        self.assets_dir = assets_dir
        self.output_dir = output_dir
        self.texture_format = texture_format
        self.mipmaps = mipmaps

    def process_texture(self, src_path: Path, rel_path: Path):
        dst_path = (self.output_dir / rel_path).with_suffix(".data")
        dst_path.parent.mkdir(parents=True, exist_ok=True)

        #convert_to_ktx2(src_path, dst_path, self.texture_format, self.mipmaps)
        convert_to_raw_data(src_path, dst_path)

        meta_path = (self.assets_dir / rel_path).with_suffix(rel_path.suffix + ".meta")
        meta_path.parent.mkdir(parents=True, exist_ok=True)
        meta = MetaFile(meta_path)
        meta.write(src_path)

        print(f"[KTX2] {rel_path} → {dst_path}")
