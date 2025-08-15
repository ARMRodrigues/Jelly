# converter.py
import OpenImageIO as oiio
from pathlib import Path

def convert_to_raw_data(src_path: Path, dst_path: Path):
    """
    Lê qualquer imagem suportada pelo OpenImageIO e salva os pixels RGBA em um arquivo .data.
    O arquivo .data terá a seguinte estrutura:
      - 4 bytes: largura (uint32 little endian)
      - 4 bytes: altura (uint32 little endian)
      - resto: pixels RGBA, 8 bits por canal
    """
    # Abrir a imagem
    img = oiio.ImageInput.open(str(src_path))
    if not img:
        raise RuntimeError(f"Erro ao abrir {src_path}")

    spec = img.spec()
    width, height = spec.width, spec.height

    # Ler os pixels como uint8 RGBA
    pixels = img.read_image(format=oiio.UINT8)
    img.close()

    # Garantir que os pixels estão em RGBA
    if spec.nchannels == 3:
        # adicionar alpha 255
        import numpy as np
        pixels = np.concatenate([pixels, np.full((height, width, 1), 255, dtype='uint8')], axis=2)

    # Salvar em .data
    with open(dst_path, "wb") as f:
        f.write(width.to_bytes(4, "little"))
        f.write(height.to_bytes(4, "little"))
        f.write(pixels.tobytes())

    print(f"Imagem convertida: {src_path} -> {dst_path}")
