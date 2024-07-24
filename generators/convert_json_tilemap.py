import json
import sys
import os

split_source = os.path.split(sys.argv[1])

infile = sys.argv[1]
outfile = os.path.join(split_source[0], "generated", os.path.splitext(split_source[1])[0]+".bin")
no_padding = "--no-padding" in sys.argv

with open(infile, "r") as file:
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

if no_padding:
    y_iter = int(json_data["height"] / 8)
    x_iter = int(json_data["width"] / 8)
else:
    y_iter = range(32)
    x_iter = range(32)

with open(outfile, "wb") as tilemap:
    for y in y_iter:
        for x in x_iter:
            tile = get_tile(x, y, json_data["layers"][0])
            tilemap.write(tile.to_bytes(2, "little"))
    assert tilemap.tell() == 2048
