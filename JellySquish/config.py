import json
from pathlib import Path

class ProjectConfig:
    def __init__(self, json_path):
        with open(json_path, "r", encoding="utf-8") as f:
            data = json.load(f)

        # Diretórios
        self.assets_dir = Path(data["assets_dir"]).resolve()
        self.output_dir = Path(data["output_dir"]).resolve()

        if not self.assets_dir.exists():
            raise FileNotFoundError(f"Pasta de assets não encontrada: {self.assets_dir}")
        self.output_dir.mkdir(parents=True, exist_ok=True)

        # Configurações de textura
        tex_settings = data.get("texture_settings", {})
        self.texture_format = tex_settings.get("format", "BC7_UNORM")
        self.generate_mipmaps = tex_settings.get("generate_mipmaps", True)
        self.max_resolution = tex_settings.get("max_resolution", None)

        # Configurações de meta
        meta_settings = data.get("meta_settings", {})
        self.store_hash = meta_settings.get("store_hash", True)
        self.store_last_modified = meta_settings.get("store_last_modified", True)
