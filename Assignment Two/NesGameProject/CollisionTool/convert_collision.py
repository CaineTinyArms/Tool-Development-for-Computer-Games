
import sys
import re

def load_tilemap(map_path):
    tilemap = {}
    with open(map_path, 'r') as f:
        for line in f:
            line = line.strip()
            if ':' in line:
                key, val = line.split(':')
                tilemap[int(key.strip(), 16)] = int(val.strip())
    return tilemap

def extract_tile_data(header_path):
    with open(header_path, 'r') as f:
        data = f.read()
        matches = re.findall(r'\b0x([0-9A-Fa-f]{2})\b|\b([0-9]{1,3})\b', data)
        tiles = []
        for hex_val, dec_val in matches:
            if hex_val:
                tiles.append(int(hex_val, 16))
            elif dec_val:
                tiles.append(int(dec_val))
        if len(tiles) != 1024:
            print(f"Warning: Expected 1024 tiles, found {len(tiles)}")
        return tiles[:1024]

def convert_to_collision(tiles, tilemap):
    return [tilemap.get(tile, 1) for tile in tiles]  # Default to 1

def format_collision_array(collisions):
    output = "const unsigned char levelCollision[1024] = {\n"
    for i in range(0, 1024, 16):
        row = ", ".join(str(val) for val in collisions[i:i+16])
        output += "    " + row + ",\n"
    output += "};\n"
    return output

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python convert_collision.py <level.h> <tilemap.txt>")
        sys.exit(1)

    header_path = sys.argv[1]
    map_path = sys.argv[2]

    tilemap = load_tilemap(map_path)
    tiles = extract_tile_data(header_path)
    collisions = convert_to_collision(tiles, tilemap)
    print(format_collision_array(collisions))
