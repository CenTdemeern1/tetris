import json
# from pprint import pprint

with open("mockup.json", "r") as file:
    json_data = json.load(file)

# print(json_data)

def get_tile(x: int, y: int, layer) -> int:
    try:
        cel = layer["cels"][0]
        bounds = cel["bounds"]
        tilemap = cel["tilemap"]
        nx = x - int(bounds["x"] / 8)
        if nx < 0: return 0
        if nx >= tilemap["width"]: return 0
        ny = y - int(bounds["y"] / 8)
        if ny < 0: return 0
        if ny >= tilemap["height"]: return 0
        flips = (tilemap["flips"][(ny * tilemap["width"]) + nx] >> 30)
        flips = (((flips & 0b01) << 1) | (flips >> 1)) << 14
        # if flips != 0: print(bin(flips))
        return tilemap["tiles"][(ny * tilemap["width"]) + nx] | flips
    except IndexError:
        return 0

with open("tilemap.bin", "wb") as tilemap:
    for y in range(32):#int(json_data["height"] / 8)):
        for x in range(32):#int(json_data["width"] / 8)):
            tile = get_tile(x, y, json_data["layers"][1])
            if tile == 0:
                tile = get_tile(x, y, json_data["layers"][0])
                if tile != 0:
                    if tile > 4:
                        tile |= (0b00_0_010_00 << 8)
                    else:
                        tile |= (0b00_0_001_00 << 8)
                    tile += 0x1F
            # else:
            #     tile = UI_TILE_MAP[tile]
            # print(tile)
            tilemap.write(tile.to_bytes(2, "little"))
    assert tilemap.tell() == 2048
