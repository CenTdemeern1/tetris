PIECES = [
    # I piece
    [
        (-1, 0),
        (0, 0),
        (1, 0),
        (2, 0),
    ],
    # T piece
    [
        (-1, 0),
        (0, 0),
        (0, -1),
        (1, 0),
    ],
    # S piece
    [
        (-1, 0),
        (0, 0),
        (0, -1),
        (1, -1),
    ],
    # J piece
    [
        (-1, -1),
        (-1, 0),
        (0, 0),
        (1, 0),
    ],
    # O piece
    [
        (0, -1),
        (1, -1),
        (0, 0),
        (1, 0),
    ],
    # Z piece
    [
        (-1, -1),
        (0, 0),
        (0, -1),
        (1, 0),
    ],
    # L piece
    [
        (-1, 0),
        (0, 0),
        (1, 0),
        (1, -1),
    ],
]

def rotate_point(point, times: int = 1):
    if times == 0:
        return point
    if times > 1:
        point = rotate_point(point, times - 1)
    return (-point[1], point[0])

serialized_pieces = []
for piece in PIECES:
    for i in range(4):
        for point in piece:
            rotated_point = rotate_point(point, i)
            for a in rotated_point:
                serialized_pieces.append(a.to_bytes(1, signed=True)) # I wanted to pack these into signed nybbles but I couldn't figure out an ergonomic way to do it
serialized_data = b''.join(serialized_pieces)

with open("data/generated/tetromino_table.bin", "wb") as file:
    file.write(serialized_data)
