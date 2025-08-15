import sys
import traceback

from config import ProjectConfig
from scanner import get_files_to_process
from processor import AssetProcessor

def main():
    try:
        if len(sys.argv) < 2:
            print("Uso: python -m build_tool <project.json>")
            sys.exit(1)
        config_path = sys.argv[1]
        config = ProjectConfig(config_path)

        arquivos_para_processar = get_files_to_process(config.assets_dir)

        processor = AssetProcessor(
            config.assets_dir,
            config.output_dir,
            texture_format=config.texture_format,
            mipmaps=config.generate_mipmaps
        )
        
        for src_path, rel_path in arquivos_para_processar:
            processor.process_texture(src_path, rel_path)
    
    except Exception as e:
        print("❌ Erro encontrado!")
        print(str(e))
        traceback.print_exc() 

if __name__ == "__main__":
    main()
