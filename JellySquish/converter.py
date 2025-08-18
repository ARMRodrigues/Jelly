# converter.py
import OpenImageIO as oiio
import numpy as np
from pathlib import Path

def convert_to_raw_data(src_path: Path, dst_path: Path):
    """
    Reads any image supported by OpenImageIO and saves the RGBA pixels to a .data file.
    The .data file will have the following structure:
      - 4 bytes: width (uint32 little endian)
      - 4 bytes: height (uint32 little endian)
      - remainder: RGBA pixels, 8 bits per channel
    """
    # Open the image
    img = oiio.ImageInput.open(str(src_path))
    if not img:
        raise RuntimeError(f"Error opening {src_path}")

    spec = img.spec()
    width, height = spec.width, spec.height

    # Read pixels as uint8 RGBA
    pixels = img.read_image(format=oiio.UINT8)
    img.close()

    # Ensure pixels are in RGBA format
    if spec.nchannels == 3:
        # add alpha 255
        pixels = np.concatenate([pixels, np.full((height, width, 1), 255, dtype='uint8')], axis=2)

    pixels = np.flipud(pixels)

    # Save to .data file
    with open(dst_path, "wb") as f:
        f.write(width.to_bytes(4, "little"))
        f.write(height.to_bytes(4, "little"))
        f.write(pixels.tobytes())

    print(f"Image converted: {src_path} -> {dst_path}")